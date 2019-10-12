%{
//Declarations
int nextToken;

enum{
    ID = 258,
    INT,
    FLO, 
    STR,
    SYM,
    OPE,
    EQU,
    GEQU,
    LEQU,
    AND,
    OR,
    INVERT,
    ERROR
};

char *keywords[] = {"abstract", "boolean", "break", "byte", "case",
                    "class", "continue", "default", "do", "double", "false", "final", "float",
                    "for", "if", "implements", "import", "instanceof", "int", "interface",
                    "long", "native", "package", "private", "protected", "public", "return",
                    "short", "static", "super", "switch", "synchronized", "this", "threadsafe",
                    "throw", "transient", "true", "try", "void", "while", 0};

void checkKeywords (char *pword)
{
	int i = 0;
	while (keywords[i] != 0)
	{
		char *pkeyword = keywords[i];
		if (strcmp(pword, pkeyword) == 0)
		{
			nextToken = 300 + i;
			return ;
		}
		i++;
	}
}

%}

INTEGER   [+-]?[0-9]+[0-9]*

FLOAT [+-]?[0-9]+"."[0-9]*

SYMBOL 		[~!@#$%^&|*+-<>:?=/{}\[\]\(\)]

STRING		\"([^\"]|(\\\"))*\"

OPERATER	"=="|">="|"<="|"&&"|"!"|"!="
	
B_COMMENT 	"/*"([^\*]|(\*)+[^/])*"*/"

L_COMMENT	"//"(.*)

KEYWORDS1 "abstract"|"boolean"|"break"|"byte"|"case"|"class"|"continue"|"default"|"do"|"double"|"false"|"final"|"float""for"|"if"|"implements"|"import"|"instanceof"|"int"|"interface"|"long"|"native"|"package"|"private"|"protected"|"public"|"return"|"short"|"static""super"|"switch"|"synchronized"|"this"|"threadsafe"|"throw"|"transient"|"true"|"try"|"void"|"while"|"0"
%%

{KEYWORDS1}	{ checkKeywords(yytext); printf(" <%6d,  %s  > \n", nextToken, yytext); }


[a-zA-Z][a-zA-Z_0-9]* | //标识符
[a-zA-Z][a-zA-Z0-9]*"_"[~!@#$%^&|*+-<>:?=/][~!@#$%^&|*+-<>:?=/]* | 
"`"(.*)"`"	{ nextToken = ID; printf(" <%6d,  %s  > \n", nextToken, yytext); }

{INTEGER} 	{ nextToken = INT; printf(" <%6d,  %s  > \n", nextToken, yytext); }

{FLOAT} 	{ nextToken = FLO; printf(" <%6d,  %s  > \n", nextToken, yytext); }

{STRING} 	{ nextToken = STR; printf(" <%6d,  %s  > \n", nextToken, yytext); }

{SYMBOL}	{ nextToken = SYM; printf(" <%6d,  %s  > \n", nextToken, yytext); }

{OPERATER}	{ nextToken = OPE; printf(" <%6d,  %s  > \n", nextToken, yytext); }

{B_COMMENT} { /*忽略注释*/ }
{L_COMMENT} { /*忽略注释*/ }

[ \n\t] { /*忽略空格和换行符*/ }

. { printf("Unknow: %s\n", yytext); }

%%

//user function
int main(int argc, char *argv[]){
	if (argc < 2)
	{
		printf("ERROR: input file name is needed. \n");
		exit(0);
	}
	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
	{
		printf("ERROR:can not open file.\n");
		exit(0);
	}
	
	yylex();
	
}

int yywrap(){
	return 1;
}


