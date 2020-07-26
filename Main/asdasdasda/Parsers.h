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
void getFirstToken();
void errorHandler(int* followArray, int size);

// Entry point for the parsing process

void startParsing(FILE* yyout,int number);

// Function Defenition

void parse_GLOBAL_VARS(FILE* yyout);
void parse_PROG(FILE* yyout);
int parse_DIM_SIZES_(FILE* yyout);
int parse_DIM_SIZES(FILE* yyout);
int parse_VAR_DEC_(FILE* yyout);
DataStrctuer* parse_VAR_DEC(FILE* yyout);
void parse_GLOBAL_VARS_(FILE* yyout);
void parse_PARAM_LIST_(FILE* yyout,arraylist* list);
arraylist* parse_PARAMS(FILE* yyout);
int parse_PARAM_(FILE* yyout);
eType parse_RETURNED_TYPE(FILE* yyout);
void parse_FUNC_FULL_DEFS_(FILE* yyout);
void parse_FUNC_FULL_DEFS(FILE* yyout);
void parse_FUNC_WITH_BODY(FILE* yyout);
eType parse_TYPE(FILE* yyout);
void parse_FUNC_PREDEFS(FILE* yyout);
void parse_FUNC_PREDEFS_(FILE* yyout);
DataStrctuer* parse_FUNC_PROTOTYPE(FILE* yyout);
void parse_STMT_LIST(FILE* yyout);
arraylist* parse_PARAM_LIST(FILE* yyout);
DataStrctuer* parse_PARAM(FILE* yyout);
void parse_VAR_DEC_LIST(FILE* yyout);
void parse_COMP_STMT(FILE* yyout);
int parse_ARGS(FILE* yyout);
eType parse_MOMO(FILE* yyout,char* id );
eType parse_FACTOR(FILE* yyout);
void parse_STMT_(FILE* yyout,char* id);
void parse_STMT(FILE* yyout);
int parse_ARG_LIST(FILE* yyout);
int parse_ARG_LIST_(FILE* yyout);
void parse_STMT_LIST_(FILE* yyout);
void parse_RETURN_STMT(FILE* yyout);
eType parse_RETURN_STMT_(FILE* yyout);
void parse_IF_STMT(FILE* yyout);
eType parse_TERM(FILE* yyout);
eType parse_TERM_(FILE* yyout);
eType parse_EXPR_(FILE* yyout);
eType parse_EXPR(FILE* yyout);
void parse_CONDITION(FILE* yyout);
int parse_EXPR_LIST_(FILE* yyout);
int parse_EXPR_LIST(FILE* yyout);
int parse_VAR_(FILE* yyout);
void print_hashtable(hashtable* t);
int checkTheFunctionSignature(DataStrctuer* data, DataStrctuer* data2);
int checkForDuplicateParams(DataStrctuer* data);
void insertToTopHashTable(DataStrctuer* data);
DataStrctuer* getFromAllHashTables(char* id);
void createNewHashTable();
void popHashTable();