#ifndef DATASTRCTUER_H
#define DATASTRCTUER_H
#include "Arrlist.h"
#include "hashtable.h"

typedef enum eType
{
	Intt,
	Floatt,
	Voidd,
	Error
}eType;


typedef enum eJob
{
	Function,
	Var,
	Arr

}eJob;


typedef struct DataStrctuer
{
	int lineNumber;
	int usedOrImplemented;
	eJob job;
	eType type;
	char* name;
	arraylist* params;
	int dimnestion;
}DataStrctuer;






#endif