#ifndef PARSERS_H
#define PARSERS_H
#include <stdio.h>
#include "Token.h"
#include <stdbool.h> 
#include "DataItem.h"
#endif


// Handler Function Defenition 

int tokenMatcher(eTOKENS token, FILE* yyout);
void defineToketToName(eTOKENS token, char* str[]);
void InitializeFirstToken();
void errorHandler(int* followArray, int size);

// Entry point for the parsing process

void startParsing(FILE* yyout,int number);

// Function Defenition

void parse_GLOBAL_VARS(FILE* yyout);
void parse_PROG(FILE* yyout);
void HashCarrierCreation();
int parse_DIM_SIZES_(FILE* yyout);
int parse_DIM_SIZES(FILE* yyout);
int parse_VAR_DEC_(FILE* yyout);
DataItem* parse_VAR_DEC(FILE* yyout);
void parse_GLOBAL_VARS_(FILE* yyout);
void parse_PARAM_LIST_(FILE* yyout,ArrayList* list);
ArrayList* parse_PARAMS(FILE* yyout);
int parse_PARAM_(FILE* yyout);
ItemType parse_RETURNED_TYPE(FILE* yyout);
void parse_FUNC_FULL_DEFS_(FILE* yyout);
void parse_FUNC_FULL_DEFS(FILE* yyout);
void parse_FUNC_WITH_BODY(FILE* yyout);
ItemType parse_TYPE(FILE* yyout);
void parse_FUNC_PREDEFS(FILE* yyout);
void parse_FUNC_PREDEFS_(FILE* yyout);
DataItem* parse_FUNC_PROTOTYPE(FILE* yyout);
void parse_STMT_LIST(FILE* yyout);
ArrayList* parse_PARAM_LIST(FILE* yyout);
DataItem* parse_PARAM(FILE* yyout);
void parse_VAR_DEC_LIST(FILE* yyout);
void parse_COMP_STMT(FILE* yyout);
int parse_ARGS(FILE* yyout);
ItemType parse_MOMO(FILE* yyout,char* id );
ItemType parse_FACTOR(FILE* yyout);
void parse_STMT_(FILE* yyout,char* id);
void parse_STMT(FILE* yyout);
int parse_ARG_LIST(FILE* yyout);
int parse_ARG_LIST_(FILE* yyout);
void parse_STMT_LIST_(FILE* yyout);
void parse_RETURN_STMT(FILE* yyout);
ItemType parse_RETURN_STMT_(FILE* yyout);
void parse_IF_STMT(FILE* yyout);
ItemType parse_TERM(FILE* yyout);
ItemType parse_TERM_(FILE* yyout);
ItemType parse_EXPR_(FILE* yyout);
ItemType parse_EXPR(FILE* yyout);
void parse_CONDITION(FILE* yyout);
int parse_EXPR_LIST_(FILE* yyout);
int parse_EXPR_LIST(FILE* yyout);
int parse_VAR_(FILE* yyout);
void PrintHashTable(HashTable* t);
int FunctionDefinision(DataItem* data, DataItem* data2);
int isExistParam(DataItem* data);
void InsertToTopHashTable(DataItem* data);
DataItem* GetDataFromHashTables(char* id);
void GenerateHashTable();
void PopFromHashTable();