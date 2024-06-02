%require "3.8.2"
%language "c++"
%skeleton "lalr1.cc"

%header
%locations

%define api.location.file "location.h"
%define api.namespace {frontend} 
%define api.parser.class {Parser}
%define api.token.constructor
%define api.token.prefix {TOKEN_}
%define api.token.raw
%define api.value.automove
%define api.value.type variant

%define parse.assert
%define parse.error detailed
%define parse.trace
%define parse.lac full

%parse-param {Scanner& scanner}

%param {Driver& driver}

%code requires {

#include "ast.h"

namespace frontend {

class Driver;
class Scanner;

}  // namespace frontend

}

%code top {

#include <sstream>
#include <memory>

#include "driver.h"

#define yylex scanner.Get

}

%token
  <std::size_t>
    IDENT "identifier"
    NUMBER "number"

%token
  CASE "case"
  END  "end"
  INT  "int"
  IS   "is"
  OF   "of"

  EQUALS      "="
  COMMA       ","
  SEMICOLON   ";"
  COLON_COLON "::"

  LEFT_PARENTHESIS     "("
  RIGHT_PARENTHESIS    ")"
  LEFT_CURLY_BRACKET   "{"
  RIGHT_CURLY_BRACKET  "}"
  LEFT_SQUARE_BRACKET  "["
  RIGHT_SQUARE_BRACKET "]"

%right
  CONS_OP
  COLON ":"

%left
  ADD_OP
  PLUS  "+"
  MINUS "-"
  
%left 
  MUL_OP
  STAR  "*"
  SLASH "/"

%precedence
  FUNC_CALL

%nterm
  <std::unique_ptr<Func>> func
  <std::vector<std::unique_ptr<Func>>> funcs
  <std::unique_ptr<FuncType>> func_type
  <std::unique_ptr<FuncBody>> func_body
  
  <std::unique_ptr<IType>> type
  <std::vector<std::unique_ptr<IType>>>
    tuple_type_content
    tuple_type_items
  <std::unique_ptr<ElementaryType>> elementary_type
  <std::unique_ptr<ListType>> list_type
  <std::unique_ptr<TupleType>> tuple_type

  <std::unique_ptr<Statement>> statement
  <std::vector<std::unique_ptr<Statement>>> statements
  
  <std::unique_ptr<IPattern>> pattern
  <std::unique_ptr<PatternList>> pattern_list
  <std::unique_ptr<PatternTuple>> pattern_tuple
  <std::vector<std::unique_ptr<IPattern>>>
    pattern_list_content
    pattern_list_items
    pattern_tuple_content
    pattern_tuple_items

  <std::unique_ptr<IResult>>
    result
    cons_expr
    cons_term
    arithm_expr
    arithm_term
    func_arg
  <std::unique_ptr<ResultList>> result_list
  <std::unique_ptr<ResultTuple>> result_tuple
  <std::unique_ptr<FuncCall>> func_call
  <std::unique_ptr<CaseExpr>> case_expr
  <std::vector<std::unique_ptr<IResult>>>
    result_list_content
    result_list_items
    result_tuple_content
    result_tuple_items

  <std::unique_ptr<Ident>> ident
  <std::unique_ptr<IConst>> const
  <Op>
    cons_op
    add_op
    mul_op

%%

program:
  funcs
  {
    driver.set_program(std::make_unique<Program>($1));
  }

funcs:
  funcs func
  {
    $$ = $1;
    $$.push_back($2);
  }
| %empty
  {
  }

func:
  IDENT func_type IS func_body END
  {
    $$ = std::make_unique<Func>($2, $4, $1);
  }

func_type:
  type "::" type
  {
    $$ = std::make_unique<FuncType>($1, $3);
  }

type:
  elementary_type
  {
    $$ = $1;
  }
| list_type
  {
    $$ = $1;
  }
| tuple_type
  {
    $$ = $1;
  }

elementary_type:
  INT
  {
    $$ = std::make_unique<ElementaryType>(ElementaryType::Kind::kInt);
  }

list_type:
  STAR type
  {
    $$ = std::make_unique<ListType>($2);
  }

tuple_type:
  "(" tuple_type_content ")"
  {
    $$ = std::make_unique<TupleType>($2);
  }

tuple_type_content:
  tuple_type_items
| %empty
  {
  }

tuple_type_items:
  type
  {
    $$.push_back($1);
  }
| tuple_type_items "," type
  {
    $$ = $1;
    $$.push_back($3);
  }

func_body:
  statements
  {
    $$ = std::make_unique<FuncBody>($1);
  }

statements:
  statement
  {
    $$.push_back($1);
  }
| statements ";" statement
  {
    $$ = $1;
    $$.push_back($3);
  }

statement:
  pattern "=" result
  {
    $$ = std::make_unique<Statement>($1, $3);
  }

pattern:
  ident
  {
    $$ = $1;
  }
| const
  {
    $$ = $1;
  }
| pattern_list
  {
    $$ = $1;
  }
| pattern_tuple
  {
    $$ = $1;
  }
| "[" pattern "]"
  {
    $$ = $2;
  }
| pattern cons_op pattern %prec CONS_OP
  {
    $$ = std::make_unique<PatternBinary>($1, $3, $2);
  }

pattern_list:
  "{" pattern_list_content "}"
  {
    $$ = std::make_unique<PatternList>($2);
  }

pattern_list_content:
  %empty
  {
  }
| pattern_list_items

pattern_list_items:
  pattern
  {
    $$.push_back($1);
  }
| pattern_list_items "," pattern
  {
    $$ = $1;
    $$.push_back($3);
  }

pattern_tuple:
  "(" pattern_tuple_content ")"
  {
    $$ = std::make_unique<PatternTuple>($2);
  }

pattern_tuple_content:
  %empty
  {
  }
| pattern_tuple_items

pattern_tuple_items:
  pattern
  {
    $$.push_back($1);
  }
| pattern_tuple_items "," pattern
  {
    $$ = $1;
    $$.push_back($3);
  }

result:
  case_expr
  {
    $$ = $1;
  }
| cons_expr

case_expr:
  CASE IDENT OF statements END
  {
    $$ = std::make_unique<CaseExpr>($4, $2);
  }

cons_expr:
  cons_term
| cons_expr cons_op cons_expr %prec CONS_OP
  {
    $$ = std::make_unique<ResultBinary>($1, $3, $2);
  }

cons_term:
  arithm_expr
| result_list
  {
    $$ = $1;
  }
| result_tuple
  {
    $$ = $1;
  }

arithm_expr:
  arithm_term
| "[" arithm_expr "]"
  {
    $$ = $2;
  }
| arithm_expr add_op arithm_expr %prec ADD_OP
  {
    $$ = std::make_unique<ResultBinary>($1, $3, $2);
  }
| arithm_expr mul_op arithm_expr %prec MUL_OP
  {
    $$ = std::make_unique<ResultBinary>($1, $3, $2);
  }

arithm_term:
  ident
  {
    $$ = $1;
  }
| const
  {
    $$ = $1;
  }
| func_call
  {
    $$ = $1;
  }

func_call:
  IDENT func_arg
  {
    $$ = std::make_unique<FuncCall>($2, $1);
  }

func_arg:
  arithm_term
| result_list
  {
    $$ = $1;
  }
| result_tuple
  {
    $$ = $1;
  }
| "[" cons_expr "]"
  {
    $$ = $2;
  }

result_list:
  "{" result_list_content "}"
  {
    $$ = std::make_unique<ResultList>($2);
  }

result_list_content:
  %empty
  {
  }
| result_list_items

result_list_items:
  cons_expr
  {
    $$.push_back($1);
  }
| result_list_items "," cons_expr
  {
    $$ = $1;
    $$.push_back($3);
  }

result_tuple:
  "(" result_tuple_content ")"
  {
    $$ = std::make_unique<ResultTuple>($2);
  }

result_tuple_content:
  %empty
  {
  }
| result_tuple_items

result_tuple_items:
  cons_expr
  {
    $$.push_back($1);
  }
| result_tuple_items "," cons_expr
  {
    $$ = $1;
    $$.push_back($3);
  }

ident:
  IDENT
  {
    $$ = std::make_unique<Ident>($1);
  }

const:
  NUMBER
  {
    $$ = std::make_unique<IntConst>($1);
  }

cons_op:
  COLON
  {
    $$ = Op::kCons;
  }

add_op:
  PLUS
  {
    $$ = Op::kAdd;
  }
| MINUS
  {
    $$ = Op::kSub;
  }

mul_op:
  STAR
  {
    $$ = Op::kMul;
  }
| SLASH
  {
    $$ = Op::kDiv;
  }

%%

void frontend::Parser::error(const location_type& loc, const std::string& msg) {
  throw syntax_error(loc, msg);
}
