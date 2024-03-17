%option c++

WHITESPACE     [ \t\r\n]
LETTER         [a-zA-Z]
CAPITAL_LETTER [A-Z]
DIGIT          [0-9]
SUBSTANCE      {CAPITAL_LETTER}{LETTER}*({CAPITAL_LETTER}{LETTER}*|{DIGIT}+)*
COEFFICIENT    {DIGIT}+
PLUS           \+
ARROW          \-\>

%%

{WHITESPACE}+

{SUBSTANCE}   { std::cout << "SUBSTANCE: " << yytext << '\n'; return 1; }

{COEFFICIENT} { std::cout << "COEFFICIENT: " << yytext << '\n'; return 1; }

{PLUS}        { std::cout << "PLUS\n"; return 1; }

{ARROW}       { std::cout << "ARROW\n"; return 1; }

.             { std::cerr << "UNKNOWN: " << yytext[0] << '\n'; return 1; }

%%
