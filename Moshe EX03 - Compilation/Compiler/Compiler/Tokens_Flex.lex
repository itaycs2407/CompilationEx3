
%{ 

#include <stdio.h>
#include "Token.h"  
#include "Parsers.h"

void error(char *character ,int numOfLine);
int numOfLine=0;
	
%}

DIGIT 			[0-9]
NONZERODIGIT 	[1-9]
ALPHABET 		[a-zA-Z]
ALPHABETNUM 	[a-zA-Z0-9]

ID 				{ALPHABET}("_"?{ALPHABETNUM})*
TYPE_NAME 		("_"){ALPHABET}("_"?{ALPHABETNUM})*
INT_NUM 		{DIGIT}+
REAL_NUM 		{DIGIT}+"."{DIGIT}+

COMMENT			"$$".*
ENDLN			[\n]
WHITE			[\t]
BLANKSPACE      " "

%%

"block"			{create_and_store_token(TOK_KW_BLOCK, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_BLOCK, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"begin"			{create_and_store_token(TOK_KW_BEGIN, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_BEGIN, lexeme: %s, found in line: %d\n",yytext,numOfLine);}			
"end"			{create_and_store_token(TOK_KW_END, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_END, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"array"			{create_and_store_token(TOK_KW_ARRAY, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_ARRAY, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"of"        	{create_and_store_token(TOK_KW_OF, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_OF, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"integer"   	{create_and_store_token(TOK_KW_INTEGER, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_INTEGER, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"real"      	{create_and_store_token(TOK_KW_REAL, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_REAL, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"type"      	{create_and_store_token(TOK_KW_TYPE, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_TYPE, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"is"        	{create_and_store_token(TOK_KW_IS, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_IS, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"enum"      	{create_and_store_token(TOK_KW_ENUM, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_ENUM, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"struct"    	{create_and_store_token(TOK_KW_STRUCT, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_STRUCT, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"switch"    	{create_and_store_token(TOK_KW_SWITCH, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_SWITCH, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"default"   	{create_and_store_token(TOK_KW_DEFAULT, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_DEFAULT, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"break"     	{create_and_store_token(TOK_KW_BREAK, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_BREAK, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"case"      	{create_and_store_token(TOK_KW_CASE, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_KW_CASE, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
	
"," 			{create_and_store_token(TOK_SEP_COMMA, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_COMMA, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"."         	{create_and_store_token(TOK_SEP_DOT, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_DOT, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
":"         	{create_and_store_token(TOK_SEP_COLON, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_COLON, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
";"         	{create_and_store_token(TOK_SEP_SEMICOLON, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_SEMICOLON, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"("         	{create_and_store_token(TOK_SEP_L_PARENTHESES, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_L_PARENTHESES, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
")"         	{create_and_store_token(TOK_SEP_R_PARENTHESES, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_R_PARENTHESES, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"["         	{create_and_store_token(TOK_SEP_L_BRACKETS, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_L_BRACKETS, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"]"         	{create_and_store_token(TOK_SEP_R_BRACKETS, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_R_BRACKETS, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"{"         	{create_and_store_token(TOK_SEP_L_CURLY_BRACES, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_L_CURLY_BRACES, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"}"         	{create_and_store_token(TOK_SEP_R_CURLY_BRACES, yytext, numOfLine);
				fprintf(yyout,"Token of type TOK_SEP_R_CURLY_BRACES, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
	
"+"         	{create_and_store_token(TOKEN_OP_ADD, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_OP_ADD, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"-"         	{create_and_store_token(TOKEN_OP_SUB, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_OP_SUB, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"*"         	{create_and_store_token(TOKEN_OP_MUL, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_OP_MUL, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"/"         	{create_and_store_token(TOKEN_OP_DIV, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_OP_DIV, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
"="         	{create_and_store_token(TOKEN_OP_EQUAL, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_OP_EQUAL, lexeme: %s, found in line: %d\n",yytext,numOfLine);}

{ID} 			{create_and_store_token(TOKEN_ID, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_ID, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
{TYPE_NAME}		{create_and_store_token(TOKEN_REAL_NAME, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_REAL_NAME, lexeme: %s, found in line: %d\n",yytext,numOfLine);}

{INT_NUM}		{create_and_store_token(TOKEN_NUMBER_INTEGER, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_NUMBER_INTEGER, lexeme: %s, found in line: %d\n",yytext,numOfLine);}
{REAL_NUM} 		{create_and_store_token(TOKEN_NUMBER_REAL, yytext, numOfLine);
				fprintf(yyout,"Token of type TOKEN_NUMBER_REAL, lexeme: %s, found in line: %d\n",yytext,numOfLine);}

{ENDLN}			{numOfLine++;}

{COMMENT}		{printf("%s is comment int line number %d",yytext,numOfLine);}
{WHITE}			{}
{BLANKSPACE}	{}

<<EOF>> 		{create_and_store_token(TOK_EOF, yytext,numOfLine);
				fprintf(yyout,"Token of type TOK_EOF, lexeme: %s, found in line: %d\n",yytext,numOfLine);
				return 0;}

.				{error(yytext,numOfLine);}

%%

int yywrap()
{
	return 1;
}

void error(char *character ,int numOfLine)
{
	printf("Character %s in line: %d does not begin any legal token in the language\n",character,numOfLine);
	fprintf(yyout,"Character %s in line: %d does not begin any legal token in the language\n",character,numOfLine);
}

int main(int argc, char *argv[])
{
int test;
	int flagin,flagout,flagout2,flagout3;
	char filename[70]={0};
	FILE *yyout2;
	FILE *yyout3;
	eTOKENS kind;


	for(test = 1 ; test <= 2 ; test++)
	{
		
		flagin = flagout =flagout2 =flagout3= 0;

		/*generating input file name*/
		sprintf(filename,"%s%d%s","C:\\temp\\test",test,".txt");
		
		/*open input file*/
		if(!(yyin	=	fopen(filename,"r")))
		{ 
			printf("cannot open input file %s; please press any key to continue" ,filename);
			getchar();
			continue;
		}
		else
		{
			flagin = 1;
			numOfLine = 1;

			/*generating lex output file name*/
			sprintf(filename,"%s%d%s","C:\\temp\\test",test,"_201555430_203379086_300508348_lex.txt");

			/*open lex output file*/
			if(!(yyout	=	fopen(filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue" ,filename);
				getchar();
				continue;
			}
			else
			{
				flagout = 1;
			}
			
			/*generating syntactic output file name*/
			sprintf(filename,"%s%d%s","C:\\temp\\test",test,"_201555430_203379086_300508348_syntactic.txt");

			/*open syntactic output file*/
			if(!(yyout2	=	fopen(filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue" ,filename);
				getchar();
				continue;
			}
			else
			{
				flagout2 = 1;
			}
			
			sprintf(filename,"%s%d%s","C:\\temp\\test",test,"_201555430_203379086_300508348_semantic.txt");

			/*open syntactic output file*/
			if(!(yyout3	=	fopen(filename,"w")))
			{ 
				printf("cannot open output file %s; please press any key to continue" ,filename);
				getchar();
				continue;
			}
			else
			{
				flagout3 = 1;
			}
		}

		/*both files opened*/
		if(flagin*flagout*flagout2*flagout3)
		{
			/*looping yylex till EOF*/
			do{
				kind = yylex();
			} while(kind != TOK_EOF);
			syntax_Analysis(yyout2,yyout3);
		}

		/*close files*/
		if(flagin)
			fclose(yyin);
		if(flagout)
			fclose(yyout);
		if(flagout2)
			fclose(yyout2);
		if(flagout3)
			fclose(yyout3);
		deleteList();
	}
	printf("\nPAUSE TO SEE ERROR MSG: Press any key to continue\n");
	getchar();
	return 0;
}


