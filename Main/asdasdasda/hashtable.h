/* This file was automatically generated.  Do not edit! */

typedef struct hashtable HashTable;


void hashtable_destroy(HashTable* t);
typedef struct hashtable_entry hashtable_entry;

hashtable_entry* hashtable_body_allocate(unsigned int capacity);
HashTable* HashTableCreate();
void hashtable_remove(HashTable* t, char* key);
void hashtable_resize(HashTable* t, unsigned int capacity);
void HashTableSet(HashTable* t, char* key, void* value);
void* HashTableGet(HashTable* t, char* key);
unsigned int hashtable_find_slot(HashTable* t, char* key);

unsigned long hashtable_hash(char* str);

typedef struct hashtable {
	unsigned int size;
	unsigned int capacity;
	hashtable_entry* body;
}HashTable;
typedef struct hashtable_entry {
	char* key;
	void* value;
}hashtable_entry;

typedef struct HashTableHolder
{
	HashTable** tables;
	int size;
}HashTableHolder;

#define INTERFACE 0
