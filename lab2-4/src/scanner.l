%{
#include "scanner.h"

#define yyterminate() return lexer::DomainTag::kEndOfProgram

#define YY_USER_ACTION AdjustCoords(coords);

using lexer::DomainTag;
using lexer::MessageType;
%}

%option c++
%option debug
%option noyywrap

IDENT      [A-Za-z_][A-Za-z_0-9]*
INT_CONST  [0-9]+

%%

[ \t\r\n]+   /* pass */
\+           { return DomainTag::kPlus; }
\-           { return DomainTag::kMinus; }
\*           { return DomainTag::kStar; }
\/           { return DomainTag::kSlash; }
=            { return DomainTag::kEqual; }
,            { return DomainTag::kComma; }
:            { return DomainTag::kColon; }
::           { return DomainTag::kColonColon; }
;            { return DomainTag::kSemicolon; }
\(           { return DomainTag::kParanthesisLeft; }
\)           { return DomainTag::kParanthesisRight; }
\{           { return DomainTag::kCurlyBracketLeft; }
\}           { return DomainTag::kCurlyBracketRight; }
\[           { return DomainTag::kSquareBracketLeft; }
\]           { return DomainTag::kSquareBracketRight; }
int          { return DomainTag::kInt; }
is           { return DomainTag::kIs; }
end          { return DomainTag::kEnd; }
{IDENT}      { return HandleIdent(attr); }
{INT_CONST}  { return HandleIntConst(attr); }
@.*          { comments_.emplace_back(coords.starting, coords.following); }
.            { compiler_->AddMessage(MessageType::kError, coords.starting,
                                     "unexpected character"); }

%%

int yyFlexLexer::yylex() {
  return 0;
}
