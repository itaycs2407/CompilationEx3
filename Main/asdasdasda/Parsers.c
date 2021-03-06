﻿#define _CRT_SECURE_NO_WARNINGS
#include "Parsers.h"
#include "Token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HashTableHolder* HashCarrier;
Token* current_token;
int currentIndex;
Node* currentNode;
FILE* SemanticFilePtr;
ItemType GlobalReturnType;
int isFunctionReturned = 0;

// Handler Function
int tokenMatcher(eTOKENS token, FILE* file)
{
	current_token = next_token();
	if (token ==  current_token->kind) {

		return 1;
	}
	else if (current_token->kind == EOF_TOK)
	{
		char* currentTokenName, * correctTokenName;
		defineToketToName(token, &correctTokenName);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s at line %d,\n", correctTokenName, current_token->lineNumber);
		fprintf(file, "Actual token of type %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		return 0;
	}
}

Token* lookAhead(int numberOfTimes)
{
	Token* tempToken = NULL;
	Token* current_token = next_token();
	back_token();
	if (current_token->kind == EOF_TOK)
	{
		return current_token;
	}
	for (int i = 0; i < numberOfTimes; i++)
	{
		tempToken = next_token();
	}
	for (int i = 0; i < numberOfTimes; i++)
	{
		back_token();
	}
	return tempToken;
}

void errorHandler(int *followArray, int size)
{
	Token* searchToken = current_token;
	while (followArray[size-1] != EOF_TOK && size > 0) {
		if (followArray[size - 1] != searchToken)
			size--;
		else
		{
			current_token = searchToken;
			break;
		}
	}
}

void defineToketToName(eTOKENS token, char* str[]) {
	char* tokenName = (char*)malloc(sizeof(char) * 20);
	switch (token)
	{
	case KEY_INT:
		tokenName = "int";
		break;
	case KEY_FLOAT:
		tokenName = "float";
		break;
	case KEY_VOID:
		tokenName = "void";
		break;
	case KEY_IF:
		tokenName = "if";
		break;
	case KEY_RETURN:
		tokenName = "return";
		break;
	case INT_NUMBER:
		tokenName = "int number";
		break;
	
	case FLOAT_NUMBER:
		tokenName = "float number";
			break;
	case COMMA_SIGN:
		tokenName = "sign : \"";
			break;
	case COLON_SIGN:
		tokenName = "sign : \,";
			break;
	case SEMICOLON_SIGN:
		tokenName = "sign : \;";
			break;
	case PARENTHESES_OPEN:
		tokenName = "sign : (";
			break;
	case PARENTHESES_CLOSE:
		tokenName = "sign : )";
			break;
	case BRACKETS_OPEN:
		tokenName = "sign : [";
			break;
	case BRACKETS_CLOSE:
		tokenName = "sign : ]";
			break;
	case CURLY_BRACES_OPEN:
		tokenName = "sign : {";
			break;
		case CURLY_BRACES_CLOSE:
			tokenName = "sign : }";
			break;
	case ARGUMENT_OPR_PLUS:
		tokenName = "sign : +";
			break;
	case ARGUMENT_OPR_MULTIPLICATION:
		tokenName = "sign : *";
			break;
	case OP_LE:
		tokenName = "sign : <=";
			break;
	case OP_L:
		tokenName = "sign : <";
			break;
	case OP_GE:
		tokenName = "sign : =>";
			break;
	case OP_G:
		tokenName = "sign : >";
			break;
	case OP_E:
		tokenName = "sign : ==";
			break;
	case OP_NE:
		tokenName = "sign : !=";
			break;
	case EQUAL_OP:
		tokenName = "sign : =";
			break;
	case OTHER_ID:
		tokenName = "sign : ID";
			break;
	case EOF_TOK:
		tokenName = "sign : END OF FILE";
			break;
	default:
		break;
	}

	*str = tokenName;

}

// Entry point for the parsing process
void startParsing(FILE* file,int number) {
	if (number == 1)
	{
		SemanticFilePtr = fopen("c:\\temp\\test1_066524737_302788781_204072524_semantic_output.txt", "w+");
	}
	else
	{
		SemanticFilePtr = fopen("c:\\temp\\test2_066524737_302788781_204072524_semantic_output.txt", "w+");
	}
	InitializeFirstToken();
	parse_PROG(file);
	tokenMatcher(EOF_TOK, file);
}

// Parsers 
void parse_PROG(FILE* file) {
	fprintf(file, "PROG -> GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS\n");
	HashCarrierCreation();
	GenerateHashTable();
	parse_GLOBAL_VARS(file);
	parse_FUNC_PREDEFS(file);
	parse_FUNC_FULL_DEFS(file);
}

void HashCarrierCreation()
{
	HashCarrier = (HashTableHolder*)calloc(1, sizeof(HashTableHolder));
	HashCarrier->size = -1;
	HashCarrier->tables = (HashTable**)calloc(80, sizeof(HashTable*));
}

void parse_GLOBAL_VARS(FILE* file) {

	fprintf(file, "GLOBAL_VARS -> VAR_DEC GLOBAL_VARS_\n");
	parse_VAR_DEC(file);
	parse_GLOBAL_VARS_(file);
}

void parse_GLOBAL_VARS_(FILE* file) {
	int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
	Token* dummyToken = NULL;
	char* tokenSemiColonSign, * tokenKeyVoid, * tokenKeyInt, * keyFloatToken, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
		dummyToken = lookAhead(2);
		switch (dummyToken->kind)
		{
		case SEMICOLON_SIGN:
		case BRACKETS_OPEN:
			fprintf(file, "GLOBAL_VARS_ -> VAR_DEC GLOBAL_VARS_\n");
			back_token();
			parse_VAR_DEC(file);
			parse_GLOBAL_VARS_(file);
			return;
		default:
			fprintf(file, "GLOBAL_VARS_ -> epsilon \n");
			 back_token();
			return;
		}
		break;
	case KEY_VOID:
		back_token();
		fprintf(file, "GLOBAL_VARS_ -> epsilon \n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &keyFloatToken);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColonSign);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, %s, %s,at line %d,\n", tokenKeyInt, keyFloatToken, tokenSemiColonSign, tokenKeyVoid, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 3);
		break;
	}
}

DataItem* parse_VAR_DEC(FILE* file) {
	DataItem* tempDataItem = (DataItem*)calloc(1, sizeof(DataItem));
	fprintf(file, "VAR_DEC->TYPE id VAR_DEC_\n");
	tempDataItem->type = parse_TYPE(file);
	tempDataItem->lineNumber = current_token->lineNumber;
	tokenMatcher(OTHER_ID,file);
	tempDataItem->name = current_token->lexeme;
	int resultOfParser = tempDataItem->dimnestion = parse_VAR_DEC_(file);
	tempDataItem->role = Arr;
	if (resultOfParser == -1)
	{
		tempDataItem->role = Var;
	}
	tempDataItem->dimnestion = resultOfParser;
	InsertToTopHashTable(tempDataItem);
}

int parse_VAR_DEC_(FILE* file) {
	int followToken[] = { KEY_INT,KEY_FLOAT };
	char* keySemicolon, * keyBracketsOpen, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(file, "VAR_DEC_ -> ; \n");		
		return -1;
	case BRACKETS_OPEN:
		fprintf(file, "VAR_DEC_ -> [ DIM_SIZES ] ; \n");
		int resultOfParser = parse_DIM_SIZES(file);
		tokenMatcher(BRACKETS_CLOSE, file);
		tokenMatcher(SEMICOLON_SIGN, file);
		return resultOfParser;
	default:
		defineToketToName(SEMICOLON_SIGN, &keySemicolon);
		defineToketToName(BRACKETS_OPEN, &keyBracketsOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", keySemicolon, keyBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 2);
		break;
	}
}

int parse_DIM_SIZES(FILE* file) {
	fprintf(file, "parse_DIM_SIZES -> int_num DIM_SIZES_ \n");
	tokenMatcher(INT_NUMBER, file);
	return parse_DIM_SIZES_(file) + 1;
}
		
int parse_DIM_SIZES_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	int followIntegerToken[] = { INT_NUMBER  };
	char* tokenCommaSign, tokenIntNumber,*tokenIntNumberZ, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(file, "DIM_SIZES_ -> , int_num DIM_SIZES_\n");
		tokenMatcher(INT_NUMBER, file);
		return 1 + parse_DIM_SIZES_(file);
	case BRACKETS_CLOSE:
		back_token();
		fprintf(file, "DIM_SIZES_ -> epsilon \n");
		return 0;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s at line %d,\n", tokenCommaSign,  current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

ItemType parse_TYPE(FILE* file) {
	int followToken[] = { OTHER_ID };
	char *tokenInt, *tokenFloat, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind)
	{
	case KEY_INT:		
		fprintf(file, "TYPE -> int \n");
		return Intt;
	case KEY_FLOAT:
		fprintf(file, "TYPE -> float \n");
		return Floatt;
	default:
		defineToketToName(KEY_INT, &tokenInt);
		defineToketToName(KEY_FLOAT, &tokenFloat);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_FUNC_PREDEFS(FILE * file) {
	fprintf(file, "FUNC_PREDEFS -> FUNC_PROTOTYPE ; FUNC_PREDEFS_\n");
	DataItem* tempDataItem = parse_FUNC_PROTOTYPE(file);
	if (isExistParam(tempDataItem) == 0)
	{
		HashTable* tempHashTableItem = HashCarrier->tables[HashCarrier->size];
		DataItem* result = (DataItem*)HashTableGet(tempHashTableItem, tempDataItem->name);
		if (result == NULL)
		{
			HashTableSet(tempHashTableItem, tempDataItem->name, tempDataItem);
		}
		else
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, there is id - %s - that is already exists\n", current_token->lineNumber, tempDataItem->name);
		}
	}
	tokenMatcher(SEMICOLON_SIGN, file);
	parse_FUNC_PREDEFS_(file);
}

void parse_FUNC_PREDEFS_(FILE* file) {
	int numOfTimes = 1, followToken[] = { KEY_VOID, KEY_INT, KEY_FLOAT };
	char* tokenSemiColonSign, * tokenKeyVoid, * tokenCurlyOpen, * tokenKeyInt, * tokenKeyFloat, * currentTokenName;
	Token* dummyToken = lookAhead(1);
	current_token = next_token();
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
	case KEY_VOID:
		while (dummyToken->kind != SEMICOLON_SIGN && dummyToken->kind != CURLY_BRACES_OPEN && dummyToken->kind != EOF_TOK)
		{
			numOfTimes++;
			dummyToken = lookAhead(numOfTimes);
		}
		switch (dummyToken->kind)
		{
		case SEMICOLON_SIGN:
		case EOF_TOK:
			back_token();
			DataItem* tempDataItem = parse_FUNC_PROTOTYPE(file);
			fprintf(file, "FUNC_PREDEFS -> FUNC_PROTOTYPE ; FUNC_PREDEFS_\n");
			if (!isExistParam(tempDataItem))
			{
				HashTable* tempHashTable = HashCarrier->tables[HashCarrier->size];
				DataItem* DataItemResult = (DataItem*)HashTableGet(tempHashTable, tempDataItem->name);
				if (DataItemResult == NULL)
				{
					HashTableSet(tempHashTable, tempDataItem->name, tempDataItem);
				}
				else
				{
					fprintf(SemanticFilePtr, "Problem! In line %d, there is id - %s - that is already exists\n", current_token->lineNumber, tempDataItem->name);
				}
			}
				tokenMatcher(SEMICOLON_SIGN, file);
				parse_FUNC_PREDEFS_(file);
				return;
		case CURLY_BRACES_OPEN:
			fprintf(file, "FUNC_PREDEFS -> epsilon\n");
			back_token();
			return;
			}
		
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &tokenKeyFloat);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColonSign);
		defineToketToName(CURLY_BRACES_OPEN, &tokenCurlyOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s , %s ,%s, at line %d\n", tokenSemiColonSign, tokenCurlyOpen, tokenKeyFloat, tokenKeyVoid, tokenKeyInt, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 3);
		break;

	}
}

DataItem* parse_FUNC_PROTOTYPE(FILE* file) {

	DataItem* tempDataItem = (DataItem*)calloc(1, sizeof(DataItem));
	fprintf(file, "FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )\n");
	tempDataItem->type = parse_RETURNED_TYPE(file);
	tokenMatcher(OTHER_ID, file);
	tempDataItem->lineNumber = current_token->lineNumber;
	tempDataItem->name = current_token->lexeme;
	tempDataItem->role = Function;
	tokenMatcher(PARENTHESES_OPEN, file);
	tempDataItem->params = parse_PARAMS(file);
	tokenMatcher(PARENTHESES_CLOSE,file);
	return tempDataItem;
}

void parse_FUNC_FULL_DEFS(FILE* file) {
	fprintf(file, "FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS_\n");
	parse_FUNC_WITH_BODY(file);
	parse_FUNC_FULL_DEFS_(file);
}
 
void parse_FUNC_WITH_BODY(FILE* file) {
	fprintf(file, "FUNC_WITH_BODY -> FUNC_PROTOTYPE COMP_STMT\n");
	DataItem* tempDataItem = parse_FUNC_PROTOTYPE(file);

	if (!isExistParam(tempDataItem))
	{
		HashTable* tempHashTable = HashCarrier->tables[HashCarrier->size];
		DataItem* DataItemResult = (DataItem*)HashTableGet(tempHashTable, tempDataItem->name);
		if (DataItemResult == NULL)
		{
			tempDataItem->isImplemented = 1;
			HashTableSet(tempHashTable, tempDataItem->name, tempDataItem);
		}
		else
		{
			if (DataItemResult->role != Function)
			{
				fprintf(SemanticFilePtr, "Problem! In line %d, the id %s is not a function! \n", current_token->lineNumber, DataItemResult->name);
			}
			else
			{
				if (FunctionDefinision(tempDataItem, DataItemResult))
				{
					if (DataItemResult->isImplemented)
					{
						fprintf(SemanticFilePtr, "Problem! In line %d,the function %s is already implemented\n", current_token->lineNumber, DataItemResult->name);
					}
					else
					{
						DataItemResult->isImplemented = 1;
					}
				}
			}
		}
	}
	GenerateHashTable();
	int size = tempDataItem->params == NULL ? 0 : ArrayListSize(tempDataItem->params);
	for (int i = 0; i < size; i++)
	{
		DataItem* temp = ((DataItem*)ArrayListGet(tempDataItem->params, i));
		InsertToTopHashTable(temp);
	}
	GlobalReturnType = tempDataItem->type;
	isFunctionReturned = 0;
	parse_COMP_STMT(file);
	PopFromHashTable();
	if (GlobalReturnType != Voidd && !isFunctionReturned)
	{
		fprintf(SemanticFilePtr, "Problem! In line %d,the function %s, not returning any statment\n", current_token->lineNumber, tempDataItem->name);
	}
	
}

void parse_FUNC_FULL_DEFS_(FILE* file) {

	int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
	char*  tokenKeyVoid,* tokenEOF ,* tokenKeyInt, * tokenKeyFloat, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case KEY_VOID:
	case KEY_FLOAT:
	case KEY_INT: 		
		fprintf(file, "FUNC_FULL_DEFS_ -> FUNC_WITH_BODY FUNC_FULL_DEFS_\n");
		back_token();
		parse_FUNC_WITH_BODY(file);
		parse_FUNC_FULL_DEFS_(file);
		break;
	case EOF_TOK:
		fprintf(file, "FUNC_FULL_DEFS_ -> epsilon\n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &tokenKeyFloat);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(EOF_TOK, &tokenEOF);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s,  %s ,%s , %s, at line %d,\n", tokenKeyFloat, tokenEOF, tokenKeyVoid, tokenKeyInt ,current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 3);
		break;
	}
}

ItemType parse_RETURNED_TYPE(FILE* file) {
	int followToken[] = { OTHER_ID };
	char* tokenKeyInt, * tokenKeyFloat, * tokenKeyVoid, *currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
		back_token();
		fprintf(file, "RETURNED_TYPE -> TYPE \n");
		return parse_TYPE(file);
	case KEY_VOID:
		fprintf(file, "RETURNED_TYPE -> void\n");
		return Voidd;
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &tokenKeyFloat);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenKeyInt, tokenKeyFloat, tokenKeyVoid, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}

}

ArrayList* parse_PARAMS(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenInt, * tokenFloat, *tokenParentesesClose, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
		back_token();
		fprintf(file, "PARAMS -> PARAM_LIST\n");
		return parse_PARAM_LIST(file);
	case PARENTHESES_CLOSE:
		back_token();
		fprintf(file, "PARAMS -> epsilon\n");
		return NULL;
	default:
		defineToketToName(KEY_INT, &tokenInt);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesesClose);
		defineToketToName(KEY_FLOAT, &tokenFloat);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, at line %d\n", tokenInt, tokenFloat, tokenParentesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_PARAM_LIST_(FILE* file,ArrayList* list) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenCommaSign, * currentTokenName, * tokenParentesesClose;
	current_token = next_token();
	switch (current_token->kind) {
	case COMMA_SIGN:
		fprintf(file, "PARAM_LIST_ ->, PARAM PARAM_LIST_ \n");
		DataItem* resultOfParser = parse_PARAM(file);
		ArrayListAdd(list, resultOfParser);
		parse_PARAM_LIST_(file,list);
		break;
	case PARENTHESES_CLOSE:
		back_token();
		fprintf(file, "PARAM_LIST_ -> epsilon\n");
		break;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesesClose);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, at line %d,\n", tokenCommaSign, tokenParentesesClose, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

int parse_PARAM_(FILE* file) {
	int followToken[] = { COMMA_SIGN };
	char* tokenCommaSign, * currentTokenName, * tokenBracketsOpen;
	current_token = next_token();
	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "PARAM_ -> [ DIM_SIZES ]\n");
		int resultOfParser = parse_DIM_SIZES(file);
		tokenMatcher(BRACKETS_CLOSE, file);
		return resultOfParser;
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:   
		back_token();
		fprintf(file, "PARAM_ -> epsilon \n");
		return -1;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(BRACKETS_OPEN, &tokenBracketsOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s  at line %d,\n", tokenCommaSign, tokenBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_COMP_STMT(FILE* file) {
	
	GenerateHashTable();
	fprintf(file, "COMP_STMT -> { VAR_DEC_LIST_ STMT_LIST }\n");
	tokenMatcher(CURLY_BRACES_OPEN, file);
	parse_VAR_DEC_LIST(file);
	parse_STMT_LIST(file);
	tokenMatcher(CURLY_BRACES_CLOSE, file);
	PopFromHashTable();
}

void parse_VAR_DEC_LIST(FILE* file) {
	int followToken[] = { OTHER_ID, KEY_INT, KEY_FLOAT, PARENTHESES_CLOSE, KEY_IF, KEY_RETURN };
	char* tokenInt, * tokenFloat, *tokenOtherID, *tokenParentsisClose, *tokenIf, *tokenRetrun,* currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
		fprintf(file, "VAR_DEC_LIST_ -> VAR_DEC VAR_DEC_LIST_\n");
		back_token();
		parse_VAR_DEC(file);
		parse_VAR_DEC_LIST(file);
		break;
	case OTHER_ID:
	case PARENTHESES_CLOSE:
	case KEY_IF:
	case KEY_RETURN:
		fprintf(file, "VAR_DEC_LIST_ -> epsilon\n");
		back_token();
		break;
	default:
		defineToketToName(KEY_INT, &tokenInt);
		defineToketToName(KEY_RETURN, &tokenRetrun);
		defineToketToName(KEY_IF, &tokenIf);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentsisClose);
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(KEY_FLOAT, &tokenFloat);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s,  %s, %s, %s, %s, %s at line %d,\n", tokenOtherID, tokenParentsisClose, tokenIf, tokenRetrun,  tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 6);
		break;
	}
}

DataItem* parse_PARAM(FILE* file) {
	DataItem* tempDataItem = (DataItem*)calloc(1, sizeof(DataItem));
	fprintf(file, "PARAM -> TYPE id PARAM_\n");
	tempDataItem->type = parse_TYPE(file);
	tokenMatcher(OTHER_ID, file);
	tempDataItem->lineNumber = current_token->lineNumber;
	tempDataItem->name = current_token->lexeme;
	tempDataItem->dimnestion = parse_PARAM_(file);
	tempDataItem->role = Arr;
	if (tempDataItem->dimnestion == -1) {
		tempDataItem->role = Var;
	}
	return tempDataItem;
}

ArrayList* parse_PARAM_LIST(FILE* file) {
	ArrayList* tempArrayList = ArrayListCreate();
	fprintf(file, "PARAM_LIST -> PARAM PARAM_LIST_\n");
	DataItem* resultOfParser = parse_PARAM(file);
	ArrayListAdd(tempArrayList, resultOfParser);
	parse_PARAM_LIST_(file,tempArrayList);
	return tempArrayList;
}

void parse_STMT_LIST(FILE* file) {
	fprintf(file, "STMT_LIST -> STMT STMT_LIST_\n");
	parse_STMT(file);
	parse_STMT_LIST_(file);
}

void parse_IF_STMT(FILE* file) {
	fprintf(file, "IF_STMT -> if ( CONDITION ) STMT\n");
	tokenMatcher(KEY_IF, file);
	tokenMatcher(PARENTHESES_OPEN, file);
	parse_CONDITION(file);
	tokenMatcher(PARENTHESES_CLOSE, file);
	parse_STMT(file);
}

void parse_RETURN_STMT(FILE* file) {
	fprintf(file, "RETURN_STMT -> return RETURN_STMT_\n");
	tokenMatcher(KEY_RETURN, file);
	ItemType tempItemType = parse_RETURN_STMT_(file);
	if (tempItemType != GlobalReturnType)
	{
		fprintf(SemanticFilePtr, "Problem! In line %d,the return type is not the same as signature type\n", current_token->lineNumber);
	}
	else
	{
		isFunctionReturned = 1;
	}
}

void parse_STMT_LIST_(FILE* file) {
	int followToken[] = { CURLY_BRACES_CLOSE };
	char* tokenCurlyClose, * tokenSemicolon ,* currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(file, "STMT_LIST_ -> ; STMT STMT_LIST_ \n");
		parse_STMT(file);
		parse_STMT_LIST_(file);
		break;
	case CURLY_BRACES_CLOSE:
		fprintf(file, "STMT_LIST_ -> ; epsilon \n");
		back_token();
		break;
	default:
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemicolon);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s,at line %d,\n", tokenCurlyClose, tokenSemicolon, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}		

void parse_STMT(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN };
	char* tokenOtherID, * tokenCurlyBraces, * tokenKeyIf, * tokenKeyReturn, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case OTHER_ID:
		fprintf(file, "STMT -> id STMT_\n");	
		parse_STMT_(file, current_token->lexeme);
		break;
	case CURLY_BRACES_OPEN:
		fprintf(file, "STMT -> COMP_STMT\n");
		back_token();
		parse_COMP_STMT(file);
		break;
	case KEY_IF:
		fprintf(file, "STMT -> IF_STMT \n");
		back_token();
		parse_IF_STMT(file);
		break;
	case KEY_RETURN:
		fprintf(file, "STMT -> RETURN_STMT \n");
		back_token();
		parse_RETURN_STMT(file);
		break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(CURLY_BRACES_OPEN, &tokenCurlyBraces);
		defineToketToName(KEY_IF, &tokenKeyIf);
		defineToketToName(KEY_RETURN, &tokenKeyReturn);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, %s,at line %d,\n", tokenOtherID, tokenCurlyBraces, tokenKeyIf, tokenKeyReturn, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_STMT_(FILE* file, char* id) {
	int followToken[] = { SEMICOLON_SIGN };
	char* tokenBracketsOpen, * tokenParenthesesOpen, * currentTokenName;
	current_token = next_token();
	DataItem* tempDataItem = GetDataFromHashTables(id);
	switch (current_token->kind) {
	case BRACKETS_OPEN:
	case EQUAL_OP:
		fprintf(file, "STMT_ -> VAR_ = EXPR \n");
		back_token();
		ItemType tempItemType1;
		int resultOfParser = parse_VAR_(file);
		if (tempDataItem == NULL || tempDataItem->role == Function)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s dosen't exists\n", current_token->lineNumber, id);
			tempItemType1 = Error;
		}
		else if (tempDataItem->role == Var && resultOfParser != -1)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s isn't variable\n", current_token->lineNumber, tempDataItem->name);
			tempItemType1 = Error;
		}
		else if (tempDataItem->role == Arr && resultOfParser == -1)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s isn't an array\n", current_token->lineNumber, tempDataItem->name);
			tempItemType1 = Error;
		}
		else if (tempDataItem->role == Arr && tempDataItem->dimnestion != resultOfParser)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, id %s dimnesion is not the same\n", current_token->lineNumber, tempDataItem->name);
			tempItemType1 = Error;
		}
		else
		{
			tempDataItem->isImplemented = 1;
			tempItemType1 = tempDataItem->type;
		}
		tokenMatcher(EQUAL_OP, file);
		ItemType tempItemType2 = parse_EXPR(file);
		if (tempItemType1 == Intt && tempItemType2 == Floatt)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, left side is Integer when right side is Float\n", current_token->lineNumber);
		}
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "STMT_ -> ( ARGS ) \n");
		int resultOfParser2 = parse_ARGS(file);
		tokenMatcher(PARENTHESES_CLOSE, file);
		if (tempDataItem == NULL || tempDataItem->role != Function)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the function %s dosen't exists\n", current_token->lineNumber, id);
			return;
		}
		int ParamListSize = tempDataItem->params == NULL ? 0 : ArrayListSize(tempDataItem->params);
		if (ParamListSize != resultOfParser2)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the function %s dosen't exists\n", current_token->lineNumber, tempDataItem->name);
			return Error;
		}
		break;
	default:
		defineToketToName(BRACKETS_OPEN, &tokenBracketsOpen);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, at line %d,\n", tokenBracketsOpen, tokenParenthesesOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

ItemType parse_FACTOR(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION, PARENTHESES_CLOSE, COMMA_SIGN, SEMICOLON_SIGN , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE , CURLY_BRACES_CLOSE };
	char* tokenOtherID, *tokenIntNum, *tokenFloatNum,* tokenZeroNum, * tokenParenthesesClose, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case OTHER_ID:
		fprintf(file, "FACTOR -> id MOMO\n");
		return parse_MOMO(file,current_token->lexeme);
		break;
	case INT_NUMBER:
		fprintf(file, "FACTOR -> int_num \n");
		return Intt;
	case FLOAT_NUMBER:
		fprintf(file, "FACTOR -> float_num\n");
		return Floatt;
	case PARENTHESES_OPEN:
		fprintf(file, "FACTOR -> ( EXPR )\n");
		ItemType tempItemType = parse_EXPR(file);
		tokenMatcher(PARENTHESES_CLOSE, file);
		return tempItemType;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER, &tokenZeroNum);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(PARENTHESES_CLOSE, &tokenParenthesesClose);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s,%s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenZeroNum, tokenParenthesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 11);
		break;
	}
}

ItemType parse_MOMO(FILE* file, char* id) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION, ARGUMENT_OPR_PLUS , PARENTHESES_CLOSE, 
		COMMA_SIGN, SEMICOLON_SIGN,BRACKETS_CLOSE,CURLY_BRACES_CLOSE , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE };
	char* tokenARGMulti, * currentTokenName, tokenOpE, tokenOpLE, tokenOpNE, tokenOpL, tokenOpGE, tokenOpG,
		tokenCurlyClose, tokenBracketsClose, tokenSemiColon, tokenComma, tokenParentesisClose;
	current_token = next_token();
	DataItem* tempDataItem = GetDataFromHashTables(id);
	switch (current_token->kind) {
	case PARENTHESES_OPEN:
		fprintf(file, "MOMO-> ( ARGS )\n");
		int resultOfParser = parse_ARGS(file);
		tokenMatcher(PARENTHESES_CLOSE, file);
		if (tempDataItem == NULL || tempDataItem->role != Function)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the function %s dosen't exists\n", current_token->lineNumber, id);
			return Error;
		}
		int ParamListSize = tempDataItem->params == NULL ? 0 : ArrayListSize(tempDataItem->params);
		if (ParamListSize != resultOfParser)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the function %s dosen't exists\n", current_token->lineNumber, tempDataItem->name);
			return Error;
		}
		return tempDataItem->type;
	case ARGUMENT_OPR_MULTIPLICATION:
	case ARGUMENT_OPR_PLUS:
	case CURLY_BRACES_CLOSE:
	case BRACKETS_CLOSE:
	case BRACKETS_OPEN:
	case SEMICOLON_SIGN:
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:
	case OP_NE:
	case OP_LE:
	case OP_L:
	case OP_GE:
	case OP_G:
	case OP_E:
	case EQUAL_OP:
		fprintf(file, "MOMO-> VAR_\n");
		back_token();
		int resultOfParser2 = parse_VAR_(file);
		if (tempDataItem == NULL || tempDataItem->role == Function)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s dosen't exists\n", current_token->lineNumber, id);
			return Error;
		}
		if (tempDataItem->role == Var && resultOfParser2 != -1)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s isn't variable\n", current_token->lineNumber, tempDataItem->name);
			return Error;
		}
		else if (tempDataItem->role == Arr && resultOfParser2 == -1)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s isn't an array\n", current_token->lineNumber, tempDataItem->name);
			return Error;
		}
		else if (tempDataItem->role == Arr && tempDataItem->dimnestion != resultOfParser2)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, the id %s dimnesion is not equal\n", current_token->lineNumber, tempDataItem->name);
			return Error;
		}
		tempDataItem->isImplemented = 1;
		return tempDataItem->type;
	default:
		defineToketToName(ARGUMENT_OPR_MULTIPLICATION, &tokenARGMulti);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(BRACKETS_CLOSE, &tokenBracketsClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColon);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesisClose);
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(OP_NE, &tokenOpNE);
		defineToketToName(OP_G, &tokenOpG);
		defineToketToName(OP_GE, &tokenOpGE);
		defineToketToName(OP_L, &tokenOpL);
		defineToketToName(OP_LE, &tokenOpLE);
		defineToketToName(OP_E, &tokenOpE);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s, at line %d,\n", tokenARGMulti, tokenBracketsClose, tokenOpL, tokenOpGE, tokenCurlyClose, tokenOpG, tokenOpNE, tokenComma, tokenParentesisClose, tokenSemiColon, tokenOpE, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 13);
		break;
	}
}

int parse_ARG_LIST_(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenComma,* currentTokenName;
	current_token = next_token();
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(file, "ARG_LIST_ -> , EXPR ARG_LIST_ \n");
		parse_EXPR(file);
		return parse_ARG_LIST_(file)+1;
	case PARENTHESES_CLOSE:
		fprintf(file, "ARG_LIST_ -> epsilon\n");
		back_token();
		return 0;
	default:
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, %s, at line %d,\n", tokenComma, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

int parse_ARG_LIST(FILE* file) {
	fprintf(file, "ARG_LIST -> EXPR ARG_LIST_\n");
	parse_EXPR(file);
	return parse_ARG_LIST_(file) + 1;
}

int parse_ARGS(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesClose, *tokenParenthesesOpen, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case OTHER_ID:
	case INT_NUMBER:
	case FLOAT_NUMBER:
	case PARENTHESES_OPEN:
		fprintf(file, "ARGS -> ARG_LIST\n");
		back_token();
		return parse_ARG_LIST(file);
	case PARENTHESES_CLOSE:
		fprintf(file, "ARGS -> ARG_LIST | ɛ\n");
		back_token();
		return 0;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(PARENTHESES_CLOSE, &tokenParenthesesClose);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenParenthesesOpen, tokenZeroNum, tokenParenthesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}
		
ItemType parse_RETURN_STMT_(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN  , CURLY_BRACES_CLOSE};
	char* tokenOtherID, * tokenIntNum,* tokenCurlyClose ,* tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case INT_NUMBER:
	case FLOAT_NUMBER:
	case OTHER_ID:
	case PARENTHESES_OPEN:
		fprintf(file, "RETURN_STMT_ -> EXPR\n");
		back_token();
		return parse_EXPR(file);
	case SEMICOLON_SIGN:
	case CURLY_BRACES_CLOSE:
		fprintf(file, "RETURN_STMT_ -> epsilon\n");
		back_token();
		return Voidd;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenCurlyClose, tokenOtherID, tokenIntNum, tokenFloatNum, tokenZeroNum, tokenParenthesesOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 2);
		break;
	}
}
		
int parse_VAR_(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION , EQUAL_OP };
	char* tokenBracketOpen, * tokenOp_e, * tokenArgMulti, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "VAR_ -> [ EXPR_LIST ]\n");
		int resultOfParser=parse_EXPR_LIST(file);
		tokenMatcher(BRACKETS_CLOSE, file);
		return resultOfParser;
	case ARGUMENT_OPR_MULTIPLICATION:
	case ARGUMENT_OPR_PLUS:
	case PARENTHESES_CLOSE:
	case OP_G:
	case OP_E:
	case OP_GE:
	case OP_L:
	case OP_LE:
	case OP_NE:
	case COMMA_SIGN:
	case SEMICOLON_SIGN:
	case BRACKETS_CLOSE:
	case CURLY_BRACES_CLOSE:
	case EQUAL_OP:
		fprintf(file, "VAR_ -> epsilon\n");
		back_token();
		return -1;
	default:
		defineToketToName(BRACKETS_OPEN, &tokenBracketOpen);
		defineToketToName(ARGUMENT_OPR_MULTIPLICATION, &tokenArgMulti);
		defineToketToName(EQUAL_OP, &tokenOp_e);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s , %s, %s, at line %d,\n", tokenBracketOpen, tokenArgMulti, tokenOp_e, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 2);
		break;
	}
}

int parse_EXPR_LIST(FILE* file)
{
	fprintf(file, "EXPR_LIST -> EXPR EXPR_LIST_\n");
	parse_EXPR(file);
	return parse_EXPR_LIST_(file) +1;
}

int parse_EXPR_LIST_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenCommaSign, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case COMMA_SIGN:
		fprintf(file, "EXPR_LIST_ -> , EXPR EXPR_LIST_\n" );
		parse_EXPR(file);
		return parse_EXPR_LIST_(file)+1;
	case BRACKETS_CLOSE:
		fprintf(file, "EXPR_LIST_ -> epsilon\n");
		back_token();
		return 0;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, at line %d,\n", tokenCommaSign, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_CONDITION(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName, * tokenOPLE, * tokenOPL, * tokenOPGE, * tokenOPG, * tokenOPE, * tokenOPNE;
	fprintf(file, "CONDITION -> EXPR rel_op EXPR\n");
	parse_EXPR(file);
	current_token = next_token();
	switch (current_token->kind)
	{
		case OP_LE:	
		case OP_L:					
		case OP_GE:					
		case OP_G:					
		case OP_E:					
		case OP_NE:
			parse_EXPR(file);
			break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, %s ,%s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenZeroNum, tokenParenthesesOpen, tokenFloatNum,  current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

ItemType parse_EXPR(FILE* file) {
	fprintf(file, "EXPR -> TERM EXPR_\n");
	ItemType tempItemType1 = parse_TERM(file);
	ItemType tempItemType2 = parse_EXPR_(file);
	return ItemCompare(tempItemType1, tempItemType2);
}			

ItemType parse_EXPR_(FILE* file) {
	int followToken[] = {  PARENTHESES_CLOSE, COMMA_SIGN, SEMICOLON_SIGN , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE , CURLY_BRACES_CLOSE};
	char * currentTokenName, tokenOpE, tokenOpLE, tokenOpNE, tokenOpL, tokenOpGE, tokenOpG,
		 tokenSemiColon, tokenComma, tokenParentesisClose, tokenCurlyClose;
	current_token = next_token();
	switch (current_token->kind) {
	case ARGUMENT_OPR_PLUS:
		fprintf(file, "EXPR_ -> + TERM EXPR_\n");
		ItemType tempItemType1 = parse_TERM(file);
		ItemType tempItemType2 = parse_EXPR_(file);
		return ItemCompare(tempItemType1, tempItemType2);
	case PARENTHESES_CLOSE :
	case CURLY_BRACES_CLOSE :
	case COMMA_SIGN:
	case SEMICOLON_SIGN :
	case BRACKETS_CLOSE:
	case OP_E:
	case OP_G:
	case OP_GE:
	case OP_L:
	case OP_LE:
	case OP_NE:
		fprintf(file, "EXPR_ -> epsilon\n");
		back_token();
		return Intt;
	default:
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesisClose);
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColon);
		defineToketToName(OP_E, &tokenOpE);
		defineToketToName(OP_G, &tokenOpG);
		defineToketToName(OP_L, &tokenOpL);
		defineToketToName(OP_LE, &tokenOpLE);
		defineToketToName(OP_GE, &tokenOpGE);
		defineToketToName(OP_NE, &tokenOpNE);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s,%s, %s, %s, %s, %s, %s, %s,  at line %d,\n", tokenParentesisClose, tokenCurlyClose, tokenOpGE, tokenOpLE, tokenOpL, tokenOpG, tokenOpE, tokenSemiColon, tokenComma, tokenOpNE, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 10);
	}
}

ItemType parse_TERM_(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_PLUS , PARENTHESES_CLOSE, COMMA_SIGN, SEMICOLON_SIGN,
		BRACKETS_CLOSE,CURLY_BRACES_CLOSE , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE};
	char* tokenARGMulti, * currentTokenName , *tokenOpE, *tokenOpLE, *tokenOpNE, *tokenOpL, *tokenOpGE, *tokenOpG, 
			*tokenCurlyClose ,*tokenBracketsClose , *tokenSemiColon, *tokenComma, *tokenParentesisClose;
	current_token = next_token();
	switch (current_token->kind) {
	case ARGUMENT_OPR_MULTIPLICATION:
		fprintf(file, "TERM_ -> * FACTOR TERM_\n");
		ItemType tempItemType1 = parse_FACTOR(file);
		ItemType tempItemType2 = parse_TERM_(file);
		return ItemCompare(tempItemType1, tempItemType2);
	case ARGUMENT_OPR_PLUS:
	case CURLY_BRACES_CLOSE:
	case BRACKETS_CLOSE:
	case SEMICOLON_SIGN:
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:
	case OP_NE:
	case OP_LE:
	case OP_L:
	case OP_GE:
	case OP_G:
	case OP_E:
	case EQUAL_OP:
		fprintf(file, "TERM_ -> epsilon\n");
		back_token();
		return Intt;
	default:
		defineToketToName(ARGUMENT_OPR_MULTIPLICATION, &tokenARGMulti);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(BRACKETS_CLOSE, &tokenBracketsClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColon);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesisClose);
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(OP_NE, &tokenOpNE);
		defineToketToName(OP_G, &tokenOpG);
		defineToketToName(OP_GE, &tokenOpGE);
		defineToketToName(OP_L, &tokenOpL);
		defineToketToName(OP_LE, &tokenOpLE);
		defineToketToName(OP_E, &tokenOpE);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s, at line %d,\n", tokenARGMulti, tokenBracketsClose, tokenOpL, tokenOpGE,  tokenCurlyClose, tokenOpG, tokenOpNE, tokenComma, tokenParentesisClose, tokenSemiColon,  tokenOpE,   current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme: * %s * \n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 11);
	}
}

ItemType parse_TERM(FILE* file) {
	fprintf(file, "TERM -> FACTOR TERM_\n");
	ItemType tempItemType1 = parse_FACTOR(file);
	ItemType tempItemType2 =  parse_TERM_(file);
	return ItemCompare(tempItemType1, tempItemType2);
}

ItemType ItemCompare(ItemType firstItem, ItemType secondItem)
{
	if (firstItem == Error || secondItem == Error)
	{
		return Error;
	}
	else if (firstItem == Floatt || secondItem == Floatt)
	{
		return Floatt;
	}
	else
	{
		return Intt;
	}
}

void GenerateHashTable()
{
	HashCarrier->size = HashCarrier->size++;
	HashCarrier->tables[HashCarrier->size] = HashTableCreate();
}

void PopFromHashTable()
{
	HashTable* tempHashTable = HashCarrier->tables[HashCarrier->size--];
	PrintHashTable(tempHashTable);
	free(tempHashTable);
}

void InsertToTopHashTable(DataItem* data)
{
	HashTable* tempHashTable = HashCarrier->tables[HashCarrier->size];
	DataItem* tempDataItem = (DataItem*)HashTableGet(tempHashTable, data->name);
	if (tempDataItem == NULL)
	{
		HashTableSet(tempHashTable, data->name, data);
	}
	else
	{
		fprintf(SemanticFilePtr, "Problem! In line %d, the id %s is already exists\n", current_token->lineNumber, data->name);
	}
}

DataItem* GetDataFromHashTables(char* id)
{
	DataItem* result = NULL;
	for (int i = HashCarrier->size; i >= 0; i--)
	{
		DataItem* result = (DataItem*)HashTableGet(HashCarrier->tables[i], id);
		if (result != NULL)
		{
			return result;
		}
	}
	return NULL;

}

void PrintHashTable(HashTable* t)
{
	for (int i = 0; i < t->capacity; i++)
	{
		if (t->body[i].key != NULL)
		{
			DataItem* tempDataItem = (DataItem*)t->body[i].value;
			if (tempDataItem->isImplemented == 0)
			{
				if (tempDataItem->role == Function)
				{
					fprintf(SemanticFilePtr, "Problem! In line %d, the function '%s' is not implemented\n", tempDataItem->lineNumber, tempDataItem->name);
				}
				else
				{
					fprintf(SemanticFilePtr, "Problem! In line %d, the id %s is never used\n", tempDataItem->lineNumber, tempDataItem->name);

				}
			}

		}
	}
}

int isExistParam(DataItem* data)
{
	if (data->params == NULL)
	{
		return 0;
	}
	int size = ArrayListSize(data->params);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{

			if (i != j)
			{
				if (strcmp(((DataItem*)ArrayListGet(data->params, i))->name, ((DataItem*)ArrayListGet(data->params, j))->name) == 0)
				{
					fprintf(SemanticFilePtr, "Problem! In line %d, there are duplicate params : %s is already exists\n", current_token->lineNumber, ((DataItem*)ArrayListGet(data->params, i))->name);
					return 1;
				}
			}
		}
	}
	return 0;
}

int FunctionDefinision(DataItem* DataItem1, DataItem* DataItem2)
{
	if (DataItem1->type != DataItem2->type)
	{
		fprintf(SemanticFilePtr, "Problem! In line %d, the function '%s' has diffrent types in definision and implementation\n", current_token->lineNumber, DataItem1->name);
		return 0;
	}
	int size1 = DataItem1->params == NULL ? 0 : ArrayListSize(DataItem1->params);
	int size2 = DataItem2->params == NULL ? 0 : ArrayListSize(DataItem2->params);
	if (size1 != size2)
	{
		fprintf(SemanticFilePtr, "Problem! In line %d, function '%s' diffrent length of params\n", current_token->lineNumber, DataItem1->name);
		return 0;
	}
	for (int i = 0; i < size1; i++)
	{
		if (((DataItem*)ArrayListGet(DataItem1->params, i))->type != ((DataItem*)ArrayListGet(DataItem2->params, i))->type)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d, function '%s' has different params type\n", current_token->lineNumber, DataItem1->name);
			return 0;
		}
		if (((DataItem*)ArrayListGet(DataItem1->params, i))->role != ((DataItem*)ArrayListGet(DataItem2->params, i))->role)
		{
			fprintf(SemanticFilePtr, "Problem! In line %d,the function '%s' has different params role\n", current_token->lineNumber, DataItem1->name);
			return 0;
		}
	}
	return 1;
}
