#ifndef DATASTRCTUER_H
#define DATASTRCTUER_H
#include "Arrlist.h"
#include "hashtable.h"

typedef enum ItemType
{
	Intt,
	Floatt,
	Voidd,
	Error
}ItemType;


typedef enum eJob
{
	Function,
	Var,
	Arr
}eJob;


typedef struct DataItem
{
	int lineNumber;
	int isImplemented;
	eJob role;
	ItemType type;
	char* name;
	ArrayList* params;
	int dimnestion;
}DataItem;






#endif