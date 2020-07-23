#pragma once


typedef enum typeOfItem
{
	iInteger,
	iFloat,
	iVoid,
	iError
}typeOfItem;

typedef enum Role
{
	rVariable,
	rFunction,
	rArray
}Role;

typedef struct DataItem
{
	int lineNumber;
	char* itemName;
	Role role;
	typeOfItem itemType;
	int dim;
	bool isUsed;
};