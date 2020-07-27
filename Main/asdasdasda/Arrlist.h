/* This file was automatically generated.  Do not edit! */
typedef struct arraylist ArrayList;
void arraylist_destroy(ArrayList* l);
void arraylist_splice(ArrayList* l, ArrayList* source, unsigned int index);
void arraylist_join(ArrayList* l, ArrayList* source);
ArrayList* arraylist_copy(ArrayList* l);
ArrayList* arraylist_slice_end(ArrayList* l, unsigned int index);
ArrayList* arraylist_slice(ArrayList* l, unsigned int index, unsigned int length);
void arraylist_clear(ArrayList* l);
void* arraylist_remove(ArrayList* l, unsigned int index);
void arraylist_insert(ArrayList* l, unsigned int index, void* value);
void arraylist_set(ArrayList* l, unsigned int index, void* value);
void* ArrayListGet(ArrayList* l, unsigned int index);
void* arraylist_pop(ArrayList* l);
void ArrayListAdd(ArrayList* l, void* item);
inline unsigned int ArrayListSize(ArrayList* l);
void arraylist_allocate(ArrayList* l, unsigned int size);
ArrayList* ArrayListCreate();
#define arraylist_iterate(l, index, item) \
	for (index = 0, item = l->body[0]; index < l->size; item = l->body[++index])
struct arraylist {
	unsigned int size; // Count of items currently in list
	unsigned int capacity; // Allocated memory size, in items
	void** body; // Pointer to allocated memory for items (of size capacity * sizeof(void*))
};
#define INTERFACE 0
