%{
#include "driver.h"

#define yyterminate() return Parser::make_YYEOF(loc_)

#define YY_USER_ACTION loc_.columns(yyleng);

using frontend::Parser;
%}

%option c++
%option yyclass="frontend::Scanner"
%option noyywrap nounput noinput
%option batch
%option debug

BLANK   [ \t\r]
IDENT   [A-Za-z_][A-Za-z_0-9]*
NUMBER  [0-9]+

%%

%{
  loc_.step();
%}

"@".*     { loc_.step(); }
{BLANK}+  { loc_.step(); }
\n+       { loc_.lines(yyleng); loc_.step(); }
"="       { return Parser::make_EQUALS(loc_); } 
","       { return Parser::make_COMMA(loc_); }
";"       { return Parser::make_SEMICOLON(loc_); }
"::"      { return Parser::make_COLON_COLON(loc_); }
"("       { return Parser::make_LEFT_PARENTHESIS(loc_); }
")"       { return Parser::make_RIGHT_PARENTHESIS(loc_); }
"{"       { return Parser::make_LEFT_CURLY_BRACKET(loc_); }
"}"       { return Parser::make_RIGHT_CURLY_BRACKET(loc_); }
"["       { return Parser::make_LEFT_SQUARE_BRACKET(loc_); }
"]"       { return Parser::make_RIGHT_SQUARE_BRACKET(loc_); }
":"       { return Parser::make_COLON(loc_); }
"+"       { return Parser::make_PLUS(loc_); }
"-"       { return Parser::make_MINUS(loc_); }
"*"       { return Parser::make_STAR(loc_); }
"/"       { return Parser::make_SLASH(loc_); }
"case"    { return Parser::make_CASE(loc_); }
"end"     { return Parser::make_END(loc_); }
"int"     { return Parser::make_INT(loc_); }
"is"      { return Parser::make_IS(loc_); }
"of"      { return Parser::make_OF(loc_); }
{IDENT}   {
            auto& ident_table = driver.get_ident_table();
            return Parser::make_IDENT(ident_table.GetCode(yytext), loc_);
          }
{NUMBER}  {
            try {
              return Parser::make_NUMBER(std::stoll(yytext), loc_);
            } catch (const std::logic_error& e) {
              throw Parser::syntax_error(loc_, e.what());
            }
          }
.         {
            const auto msg = "unexpected character: " + std::string{yytext};
            throw Parser::syntax_error(loc_, msg);
          }

%%
