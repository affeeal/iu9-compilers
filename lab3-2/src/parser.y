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

%parse-param {frontend::Scanner& scanner}

%param {frontend::Driver& driver}

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

%token <std::size_t> IDENT  "identifier";
%token <std::size_t> NUMBER "number";

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

%nterm <std::vector<std::unique_ptr<frontend::Func>>> funcs
%nterm <std::unique_ptr<frontend::Func>> func
%nterm <std::unique_ptr<frontend::FuncType>> func_type
%nterm <std::unique_ptr<frontend::FuncBody>> func_body
%nterm <std::unique_ptr<frontend::IType>> type
%nterm <std::unique_ptr<frontend::ElementaryType>> elementary_type
%nterm <std::unique_ptr<frontend::ListType>> list_type
%nterm <std::unique_ptr<frontend::TupleType>> tuple_type
%nterm <std::vector<std::unique_ptr<frontend::IType>>> tuple_type_content
%nterm <std::vector<std::unique_ptr<frontend::IType>>> tuple_type_items

%%

program:
  funcs
  {
    driver.set_program(std::make_unique<frontend::Program>($1));
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
    $$ = std::make_unique<frontend::Func>($2, $4, $1);
  }

func_type:
  type "::" type
  {
    $$ = std::make_unique<frontend::FuncType>($1, $3);
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
    $$ = std::make_unique<frontend::ElementaryType>(
        frontend::ElementaryType::Kind::kInt);
  }

list_type:
  STAR type
  {
    $$ = std::make_unique<frontend::ListType>($2);
  }

tuple_type:
  "(" tuple_type_content ")"
  {
    $$ = std::make_unique<frontend::TupleType>($2);
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
  %empty
  {
    $$ = nullptr; // TODO
  }
  
%%

void frontend::Parser::error(const location_type& loc, const std::string& msg) {
  throw syntax_error(loc, msg);
}
