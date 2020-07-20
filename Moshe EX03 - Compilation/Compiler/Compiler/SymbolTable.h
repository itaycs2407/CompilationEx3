#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"
#include "hashtable.h"

//enumration of the role of an id - type can be either enum or struct
typedef enum Role {
	varviable,
	type
}Role;

//catgory set the kind of the id
typedef enum Category {
	_simple,
	_enum,
	_array,
	_struct
}Category;


typedef struct SymbolTableNode {
	SymobleTable* next;
	SymobleTable* back;
	SymobleTable* symboleTable;
}SymbolTableNode;

//Create a new Symbal table node in the Symbol Table list
SymbolTableNode* addSymbolTableNode(SymbolTableNode* currentTable);
//Delete a table and get to the privous Symbol Table in the Symbole Table list
SymbolTableNode* popTable(SymbolTableNode* currentTable);
//Inseart a new table at the tail of the Symbol Table list
Attributes* insert(SymobleTable* currentTable, Token* current_token);
//Look for key id inside a spesific hastable
Attributes* lookup(SymobleTable* currentTable, char* id);
//Look for key id inside all the hastable in the Symbol Table list from the last to the first
Attributes* find(SymbolTableNode* currentTable, char* id);
//initialize a SymbolTableNode
SymbolTableNode* initializeTable();

SymbolTableNode* currTable;

#endif#pragma once
