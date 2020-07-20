#include "SymbolTable.h"


//Add another Symbol table node to the Symbol table node list, if the list is empty, create a new symbole table list
SymbolTableNode* addSymbolTableNode(SymbolTableNode* currentTable){
	SymbolTableNode* temp;
	temp = initializeTable();

	//If the there is no table, then this is the first table
	if (currentTable== NULL) {
		currentTable = temp;
		return temp;
	}
	else{
		currentTable->next = temp;
		temp->back = currentTable;
		return temp;
	}
	return NULL;
}


//Create a new symbol Table Node adding by initliaztion the next and privous nodes, and creating a new hash table
SymbolTableNode* initializeTable() {
	SymbolTableNode* symbolTableNode = (SymbolTableNode*)malloc(sizeof(SymbolTableNode));
	if (symbolTableNode == NULL) {
		printf("Falied to allocate memory to symbol table");
		return NULL;
	}
	symbolTableNode->symboleTable=ht_create();
	symbolTableNode->next = NULL;
	symbolTableNode->back = NULL;

	return symbolTableNode;
}


SymbolTableNode* popTable(SymbolTableNode* currentTableListNode) {
	SymbolTableNode* temp;
	if (currentTableListNode == NULL) {
		return NULL;
	}
	else {
		temp = currentTableListNode->back;
		ht_dump(currentTableListNode->symboleTable);
		free(currentTableListNode);
		return temp;
	}
}

Attributes* lookup(SymobleTable* currentTable, char* id){
	Attributes* attributes = NULL;
	attributes=ht_get(currentTable, id);
	if (attributes) {
		return attributes;
	}
	return NULL;
}

Attributes* insert(SymobleTable* currentTable,char* id,Attributes* value) {
	if (ht_get(currentTable, id) == NULL) {
		Attributes* attributes = value;
		ht_set(currentTable, attributes->name, attributes);
		return attributes;
	}
	else {
		return NULL;
	}
}

//Finiding an Attributes struct in all the symbole table list nodes, starting from the last one
Attributes* find(SymbolTableNode* currentTable, char* id){

	SymbolTableNode* temp = currentTable;
	Attributes* attributes =NULL;

	while (temp->back != NULL) {
		attributes = lookup(temp->symboleTable, id);
			 if(attributes != NULL) {
				return attributes;
			}
			 else
			 {
				 temp = temp->back;
			 }
	}
	return NULL;
}

/*
eTOKENS get_id_type(TableEntry entryId) {
	return entryId.attributes.simpleType;
}

void set_id_type(TableEntry* entryId, eTOKENS type) {
	entryId->attributes.simpleType = type;
}
*/