#ifndef PARSERS_H
#define PARSERS_H
#include <stdio.h>
#include "Token.h"
#include "SymbolTable.h"
#include "hashtable.h"


void parse_PROGRAM(FILE* yyout,FILE* yyout2);
void parse_BLOCK(FILE* yyout,FILE* yyout2);
void parse_DECLARATIONS(FILE* yyout,FILE* yyout2);
void parse_DECLARATIONS_TAG(FILE* yyout,FILE* yyout2);
void parse_DECLARATION(FILE* yyout,FILE* yyout2);
void parse_VAR_DECLARATION(FILE* yyout,FILE* yyout2);
void parse_VAR_DECLARATION_TAG(FILE* yyout,FILE* yyout2);
void parse_SIZE(FILE* yyout,FILE* yyout2);
void parse_SIMPLE_TYPE(FILE* yyout,FILE* yyout2);
void parse_TYPE_DECLARATION(FILE* yyout,FILE* yyout2);
void parse_TYPE_INDICATOR(FILE* yyout,FILE* yyout2);
void parse_ENUM_TYPE(FILE* yyout,FILE* yyout2);
void parse_ID_LIST(FILE* yyout,FILE* yyout2);
void parse_ID_LIST_TAG(FILE* yyout,FILE* yyout2);
void parse_STRUCTURE_TYPE(FILE* yyout,FILE* yyout2);
void parse_FIELDS(FILE* yyout,FILE* yyout2);
void parse_FIELDS_TAG(FILE* yyout,FILE* yyout2);
void parse_FIELD(FILE* yyout,FILE* yyout2);
void parse_STATEMENTS(FILE* yyout,FILE* yyout2);
void parse_STATEMENTS_TAG(FILE* yyout,FILE* yyout2);
void parse_STATEMENT(FILE* yyout,FILE* yyout2);
void parse_VAR_ELEMENT(FILE* yyout,FILE* yyout2);
void parse_KEY(FILE* yyout,FILE* yyout2);
void parse_CASE_LIST(FILE* yyout,FILE* yyout2);
void parse_CASE_LIST_TAG(FILE* yyout,FILE* yyout2);
void parse_CASE(FILE* yyout,FILE* yyout2);
void parse_KEY_VALUE(FILE* yyout,FILE* yyout2);
void parse_FIELD_ACCESS(FILE* yyout,FILE* yyout2);
void parse_FIELD_ACCESS_TAG(FILE* yyout,FILE* yyout2);
void parse_EXPRESSION(FILE* yyout,FILE* yyout2);
void parse_EXPRESSION_TAG(FILE* yyout,FILE* yyout2);
void parse_SIMPLE_EXPRESSION(FILE* yyout,FILE* yyout2);

//Mathc between current token to the expected token
void match(eTOKENS, FILE* yyout);
//Return the next token without changing the token index
Token* peekNext();
//Get the string name of the token type
void tokenByName(eTOKENS token, char* str[]);
//Check if a token is the follow token
bool isFollow(int* followToken, int numOfFollow, eTOKENS testedToken);
//Convert number in string to int
int numCharToInt(char* ch);
//Check if 2 ideas around a operator has matched type
void variablesTypeMatched(Token* leftToken, FILE* yyout2);
#endif#pragma once