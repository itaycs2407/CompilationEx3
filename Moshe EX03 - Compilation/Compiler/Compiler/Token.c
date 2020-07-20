#include "Token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/


char* eTokensString[] =
{
	"TOK_EOF",
	"TOK_KW_BLOCK",
	"TOK_KW_BEGIN",
	"TOK_KW_END",
	"TOK_KW_ARRAY",
	"TOK_KW_OF",
	"TOK_KW_INTEGER",
	"TOK_KW_REAL",
	"TOK_KW_TYPE",
	"TOK_KW_IS",
	"TOK_KW_ENUM",
	"TOK_KW_STRUCT",
	"TOK_KW_SWITCH",
	"TOK_KW_DEFAULT",
	"TOK_KW_BREAK",
	"TOK_KW_CASE",
	
	"TOKEN_ID",
	"TOKEN_REAL_NAME",
	
	"TOKEN_NUMBER_INTEGER",
	"TOKEN_NUMBER_REAL",
	
	"TOK_SEP_COMMA",
	"TOK_SEP_DOT",
	"TOK_SEP_COLON",
	"TOK_SEP_SEMICOLON",
	"TOK_SEP_L_PARENTHESES", 
	"TOK_SEP_R_PARENTHESES", 
	"TOK_SEP_L_BRACKETS", 
	"TOK_SEP_R_BRACKETS", 
	"TOK_SEP_L_CURLY_BRACES",
	"TOK_SEP_R_CURLY_BRACES", 

	"TOKEN_OP_ADD",
	"TOKEN_OP_SUB", 
	"TOKEN_OP_MUL", 
	"TOKEN_OP_DIV", 
	"TOKEN_OP_EQUAL" 

};


int currentIndex = 0;
Node* currentNode = NULL;
Node* headNode = NULL;
extern FILE* yyin, * yyout;
int count = 0;

#define TOKEN_ARRAY_SIZE 100

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	
	// case 1: there is still no tokens in the storage.

	if (headNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
		headNode = currentNode;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);

			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif		
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token()
{ 
	if (headNode == NULL || headNode == currentNode && currentIndex == 0) {
		return NULL;
	}
	else if (currentIndex == 0) {
		currentNode = currentNode->prev;
		currentIndex = TOKEN_ARRAY_SIZE - 1;
	}
	else {
		currentIndex--;
	}
	return &(currentNode->tokensArray[currentIndex]);
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token *next_token() {
	if (currentNode == NULL) {
		return NULL;
	}
	if (currentIndex == TOKEN_ARRAY_SIZE - 1) {
		currentNode = currentNode->next;
		currentIndex = 0;
	}
	else {
		currentIndex++;
	}
	return &(currentNode->tokensArray[currentIndex]);
}

//Get the first token
Token* getFirstToken() {
	currentNode = headNode;
	currentIndex = 0;
	return &(currentNode->tokensArray[currentIndex]);
}

void deleteList()
{
	Node* current = headNode->next;
	Node* next;

	while (current != NULL)
	{
		next = current->next;
		free(current->tokensArray);
		free(current);
		current = next;
	}
	currentNode = NULL;
	headNode = NULL;
	currentIndex = 0;
	count = 0;
}