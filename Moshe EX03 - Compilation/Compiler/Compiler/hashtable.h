#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"

//This is the Hash table Header file, it does all the base function of the hash table

//Each Id has attributes like its type, a Role which can be either variable or type, and Category. some variables are for spesific catgory
typedef struct Attributes {
	char* name;
	eTOKENS type;
	enum Role role;
	enum Category category;
	int size;
}Attributes;

//A culmn in the table
typedef struct entry_t {
	char* key;
	Attributes* attributes;
	struct entry_t* next;
} entry_t;

//This is a hash table type
typedef struct ht_t {
	entry_t** entries;
} SymobleTable;

unsigned int hash(const char* key);
//pair between a key and his attributes
entry_t* ht_pair(const char* key, const Attributes* attributes);
//Create a hash table
SymobleTable* ht_create(void);
//Set attribute into a key in a table
void ht_set(SymobleTable* hashtable, const char* key, const Attributes* attributes);
//Get attribute value from the hastable based on the key value
char* ht_get(SymobleTable* hashtable, const char* key);
//Delete a value from the hash tash table
void ht_del(SymobleTable* hashtable, const char* key);
//Delete an entire table and print its keys values
void ht_dump(SymobleTable* hashtable);

#endif#pragma once