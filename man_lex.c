/*auther:@newhans
keywords从320开始
注释不输出
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int charClass;
#define MAX_LEN 1000
char lexeme[MAX_LEN];
char nextChar;
char next2Char;
int lexLen;
int token;
int nextToken;
FILE *inFile;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 999


enum
{
    ID = 258,
    INT,
    LONG,
    STRING,
    EQU,
    GEQU,
    LEQU,
    ERROR,
    COMMENT
};

char *keywords[] = {"abstract", "boolean", "break", "byte", "case",
                    "class", "continue", "default", "do", "double", "false", "final", "float",
                    "for", "if", "implements", "import", "instanceof", "int", "interface",
                    "long", "native", "package", "private", "protected", "public", "return",
                    "short", "static", "super", "switch", "synchronized", "this", "threadsafe",
                    "throw", "transient", "true", "try", "void", "while", 0};
void addChar()
{
    if (lexLen <= MAX_LEN - 2)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
        //0 -> NULL
    }
    else
    {
        printf("ERROR:lexeme is too long. \n");
    }
}

void getChar()
{
    static int firstRun = 1;
    if (firstRun)
    {
        nextChar = getc(inFile);
        //getc读字符
        next2Char = getc(inFile);
        firstRun = 0;
    }
    else
    {
        nextChar = next2Char;
        next2Char = getc(inFile);
    }

    if (nextChar == EOF)
    {
        charClass = EOF;
    }
    else
    {
        if (isalpha(nextChar))
        {
            //isalpha()判断一个字符是否是字符，若参数为英文字母（a ~ z A ~ Z），则返回非 0 值，否则返回 0。
            charClass = LETTER;
        }
        else if (isdigit(nextChar))
        {
            charClass = DIGIT;
        }
        else
        {
            charClass = UNKNOWN;
        }
    }
}

void getNonBlank()
{
    while (isspace(nextChar))
    {
        getChar();
    }
}

//handle the string
void pstring()
{
    addChar();
    nextToken = nextChar;
    printf("<%6d,       %s      > \n", nextToken, lexeme); //输出字符串开头的 “
    lexLen = 0;
    while (next2Char != '\"' || (next2Char == '\"' && nextChar == '\\'))
    {
        /*
        
        ???
        */
        getChar();
        addChar();
    }
    nextToken = STRING;
    printf("<%6d,        %s      > \n", nextToken, lexeme); //输出字符串
    lexLen = 0;
    getChar();
    addChar();
    nextToken = nextChar; //处理结尾的 ”
}

//handle the comment
void comment()
{
    if (next2Char == '/')
    {
        while (nextChar != '\n')
        {
            getChar();
        }
    }
    else
    {
        while (nextChar != '*' || next2Char != '/')
        {
            getChar();
        }
        getChar();
    }
    nextToken = COMMENT;
}

void checkKeywords(char *pword)
{
    int i = 0;
    while (keywords[i] != 0)
    {
        char *pkeyword = keywords[i];
        if (strcmp(pword, pkeyword) == 0)
        {
            //若参数s1 和s2 字符串相同则返回0。s1 若大于s2 则返回大于0 的值。s1 若小于s2 则返回小于0 的值。
            nextToken = 350 + i;
            return;
        }
        i++;
    }
}

int checkSymbol(char ch, char nextCh)
{
    switch (ch)
    {
    case '(': //40对应的ascii码表
    case ')': //41
    case ';': //59
    case '+': //43
    case '-':
    case '*':
    case '{':
    case '}':
    case '[':
    case ']':
    case ',':
    case '.':
    case '?':
    case '@':
    case ':':
        addChar();
        nextToken = ch;
        break;
    case '"':
        pstring(); //进入字符串处理
        break;
    case '/':
        if (nextCh == '*' || nextCh == '/')
        {
            comment();
            break;
        }
        addChar();
        nextToken = ch;
        break;
    case '=': //61
        addChar();
        nextToken = ch;
        if (nextCh == '=')
        {
            getChar();
            addChar();
            nextToken = EQU;
        }
        break;
    case '>':
        addChar();
        nextToken = ch;
        if (nextCh == '=')
        {
            getChar();
            addChar();
            nextToken = GEQU;
        }
        break;
    case '<':
        addChar();
        nextToken = ch;
        if (nextCh == '=')
        {
            getChar();
            addChar();
            nextToken = LEQU;
        }
        break;
    case EOF:
        addChar();
        nextToken = EOF;
        //EOF : end of file 文件结束符
    /*case ':':
        addChar();
        checkKeywords(":");
        break;
    case '.':
        addChar();
        checkKeywords(".");
        break;
    case ',':
        addChar();
        checkKeywords(",");
        break;
    case '?':
        addChar();
        checkKeywords("?");
        break;
    case '@':
        addChar();
        checkKeywords("@");
        break;*/
    default:
        printf("ERROR:unknown character '%c'. \n", ch);
        nextToken = ERROR;
    }
    return nextToken;
}
//return nextToken

//处理含有_和$的标识符id
int isSymbol(char ch)
{
    switch (ch)
    {
    case '.':
    case '_':
    case '$':
        return 1;
        break;
    default:
        return 0;
        break;
    }
}

int lexer()
{
    //lexer:词法分析器
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
    case DIGIT:
        //digit:数字
        addChar();
        getChar();
        int islong = 0;
        while (charClass == DIGIT || nextChar == 'L' )
        {
            if (nextChar == 'L')
            {
                islong = 1;
            }
            addChar();
            getChar();
        }
        if (islong)
        {
            nextToken = LONG;
        }
        else
        {
            nextToken = INT;
        }
        break;

    case LETTER:
        //letter:字母
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT )
        //|| (isSymbol(nextChar))
        {
            addChar();
            getChar();
        }
        nextToken = ID;
        checkKeywords(lexeme);
        //void checkKeywords(char *pword)
        //检查当前标识符是否是关键字
        break;

    case UNKNOWN:
        checkSymbol(nextChar, next2Char);
        getChar();
        break;

    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    if (nextToken != COMMENT) //注释不输出
    {
        printf("<%6d,       %s      >\n", nextToken, lexeme);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("ERROR:input file name is needed. \n");
        exit(0);
    }
    inFile = fopen(argv[1], "r");
    if (inFile == NULL)
    {
        printf("ERROR:can not open file.\n");
        exit(0);
    }

    getChar();
    while (nextToken != EOF)
        lexer();
}