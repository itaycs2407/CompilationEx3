#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	TOK_EOF,

	TOK_KW_BLOCK,
	TOK_KW_BEGIN,
	TOK_KW_END,
	TOK_KW_ARRAY,
	TOK_KW_OF,
	TOK_KW_INTEGER,
	TOK_KW_REAL,
	TOK_KW_TYPE,
	TOK_KW_IS,
	TOK_KW_ENUM,
	TOK_KW_STRUCT,
	TOK_KW_SWITCH,
	TOK_KW_DEFAULT,
	TOK_KW_BREAK,
	TOK_KW_CASE,

	TOKEN_ID,
	TOKEN_REAL_NAME,

	TOKEN_NUMBER_INTEGER,
	TOKEN_NUMBER_REAL,

	TOK_SEP_COMMA,//,
	TOK_SEP_DOT, //.	
	TOK_SEP_COLON, //:
	TOK_SEP_SEMICOLON, //;
	TOK_SEP_L_PARENTHESES, // (
	TOK_SEP_R_PARENTHESES, // )
	TOK_SEP_L_BRACKETS, // [
	TOK_SEP_R_BRACKETS, // ]
	TOK_SEP_L_CURLY_BRACES, // {
	TOK_SEP_R_CURLY_BRACES, // }


	TOKEN_OP_ADD, // +
	TOKEN_OP_SUB, // -
	TOKEN_OP_MUL, // *
	TOKEN_OP_DIV, // /	
	TOKEN_OP_EQUAL // =



}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
//Return the next token and move the index forward
Token *next_token();
//Return the privious token and move the index backward
Token *back_token();
//Return the first token from the Token list
Token* getFirstToken();
//Delete the token list
void deleteList();

#endif