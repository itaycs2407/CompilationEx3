#include "hashtable.h"


#define TABLE_SIZE 1000

/*
typedef struct Attributes {
	int idNumber;
	eTOKENS type;
}Attributes;

typedef struct entry_t {
	char* key;
	Attributes* attributes;
	struct entry_t* next;
} entry_t;

typedef struct {
	entry_t** entries;
} ht_t;
*/

unsigned int hash(const char* key) {
	unsigned long int value = 0;
	unsigned int i = 0;
	unsigned int key_len = strlen(key);

	// do several rounds of multiplication
	for (; i < key_len; ++i) {
		value = value * 37 + key[i];
	}

	// make sure value is 0 <= value < TABLE_SIZE
	value = value % TABLE_SIZE;

	return value;
}

entry_t* ht_pair(const char* key, const Attributes* attributes) {
	// allocate the entry
	entry_t* entry = malloc(sizeof(entry_t) * 1);
	entry->key = malloc(strlen(key) + 1);
	entry->attributes = malloc(sizeof(Attributes));

	// copy the key and value in place
	strcpy(entry->key, key);
	entry->attributes= attributes;

	// next starts out null but may be set later on
	entry->next = NULL;

	return entry;
}

SymobleTable* ht_create(void) {
	// allocate table
	SymobleTable* hashtable = malloc(sizeof(SymobleTable) * 1);

	// allocate table entries
	hashtable->entries = malloc(sizeof(entry_t*) * TABLE_SIZE);

	// set each to null (needed for proper operation)
	int i = 0;
	for (; i < TABLE_SIZE; ++i) {
		hashtable->entries[i] = NULL;
	}

	return hashtable;
}

void ht_set(SymobleTable* hashtable, const char* key, const Attributes* attributes) {
	unsigned int slot = hash(key);

	// try to look up an entry set
	entry_t* entry = hashtable->entries[slot];

	// no entry means slot empty, insert immediately
	if (entry == NULL) {
		hashtable->entries[slot] = ht_pair(key, attributes);
		return;
	}

	entry_t* prev=NULL;

	// walk through each entry until either the end is
	// reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (strcmp(entry->key, key) == 0) {
			// match found, replace value
			free(entry->attributes);
			entry->attributes = malloc(sizeof(Attributes));
			entry->attributes= attributes;
			return;
		}

		// walk to next
		prev = entry;
		entry = prev->next;
	}

	// end of chain reached without a match, add new
	prev->next = ht_pair(key, attributes);
}

char* ht_get(SymobleTable* hashtable, const char* key) {
	unsigned int slot = hash(key);

	// try to find a valid slot
	entry_t* entry = hashtable->entries[slot];

	// no slot means no entry
	if (entry == NULL) {
		return NULL;
	}

	// walk through each entry in the slot, which could just be a single thing
	while (entry != NULL) {
		// return value if found
		if (strcmp(entry->key, key) == 0) {
			return entry->attributes;
		}

		// proceed to next key if available
		entry = entry->next;
	}

	// reaching here means there were >= 1 entries but no key match
	return NULL;
}

void ht_del(SymobleTable* hashtable, const char* key) {
	unsigned int bucket = hash(key);

	// try to find a valid bucket
	entry_t* entry = hashtable->entries[bucket];

	// no bucket means no entry
	if (entry == NULL) {
		return;
	}

	entry_t* prev=NULL;
	int idx = 0;

	// walk through each entry until either the end is reached or a matching key is found
	while (entry != NULL) {
		// check key
		if (strcmp(entry->key, key) == 0) {
			// first item and no next entry
			if (entry->next == NULL && idx == 0) {
				hashtable->entries[bucket] = NULL;
			}

			// first item with a next entry
			if (entry->next != NULL && idx == 0) {
				hashtable->entries[bucket] = entry->next;
			}

			// last item
			if (entry->next == NULL && idx != 0) {
				prev->next = NULL;
			}

			// middle item
			if (entry->next != NULL && idx != 0) {
				prev->next = entry->next;
			}

			// free the deleted entry
			free(entry->key);
			free(entry->attributes);
			free(entry);

			return;
		}

		// walk to next
		prev = entry;
		entry = prev->next;

		++idx;
	}
}

void ht_dump(SymobleTable* hashtable) {
	for (int i = 0; i < TABLE_SIZE; ++i) {
		entry_t* entry = hashtable->entries[i];

		if (entry == NULL) {
			continue;
		}

		printf("slot[%4d]: ", i);

		for (;;) {
			printf("%s ", entry->key);

			if (entry->next == NULL) {
				break;
			}

			entry = entry->next;
		}

		printf("\n");
	}
}
