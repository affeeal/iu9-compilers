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
%parse-param {frontend::Driver& driver}

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

%token <std::string>   IDENT  "identifier";
%token <std::uint64_t> NUMBER "number";

%token
  CASE "case"
  END  "end"
  INT  "int"
  IS   "is"
  OF   "of"

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

%%

%%

void frontend::Parser::error(const location_type& loc, const std::string& msg) {
  throw syntax_error(loc, msg);
}
