#include "Token.h"
#define TOKEN_ARRAY_SIZE 1000

extern FILE* yyin, * yyout;

Node* headNode = NULL;
Node* currentNode = NULL;
int count = 0;
int currentIndex = 0;

 // init of the currentIndex

void  getFirstToken() {
	currentNode = headNode;
	currentIndex = 0;
}

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	int length = strlen(lexeme) + 1;
	if (headNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
		headNode = currentNode;
	}
	else
	{
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif		
}

	Token* back_token()
	{
		Token* currentToken;
		if (headNode == NULL || headNode == currentNode && currentIndex == 0)
		{
			// Already in first token or there are no token at all - do nothing.
			return NULL;
		}
		else if (currentIndex == 0)
		{
			currentNode = currentNode->prev;
			currentIndex = TOKEN_ARRAY_SIZE - 1;
			currentToken = &(currentNode->tokensArray[currentIndex - 1]);
		}
		else
		{
			currentIndex--;
			if (currentIndex == 0)
			{
				currentToken = &(currentNode->prev->tokensArray[TOKEN_ARRAY_SIZE - 1]);
			}
			else
			{
				currentToken = &(currentNode->tokensArray[currentIndex - 1]);
			}
		}
		return currentToken;
	}

// old next token

/*
Token* next_token() {
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
*/

	// new next token
	Token* next_token()
	{
		Token* currentToken = NULL;
		if (currentNode == NULL)
		{
			return NULL;
		}
		else if (currentNode->tokensArray[currentIndex].kind == EOF_TOK)
		{
			currentToken = &(currentNode->tokensArray[currentIndex]);
		}
		else if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentToken = &(currentNode->tokensArray[currentIndex]);
			currentNode = currentNode->next;
			currentIndex = 0;
		}
		else
		{
			currentToken = &(currentNode->tokensArray[currentIndex]);
			currentIndex++;
		}
		return currentToken;
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
}

void false_create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));
		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*) calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	else
	{
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
			currentNode->prev = NULL;
			headNode = currentNode;
		}
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