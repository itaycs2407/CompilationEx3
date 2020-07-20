#include "Parsers.h"



//Tokens mangement for syntactic part
Token* current_token;
Token* Another_Token;
int currentIndex;
Node* currentNode;

//Tokens mangement for semantic part
eTOKENS rightToken;
Attributes* currentAttributes=NULL;//current attribute struct
SymbolTableNode* currentTable=NULL;
bool inStruct = false;
bool inEnum = false;
Token* leftToken;
Token* currentArrayToken;

void errorRecover(int* followToken, int numOfFollow) 
{
	Token* token = current_token;
	while (token->kind != TOK_EOF && (!isFollow(followToken, numOfFollow, token->kind))) {
		token = next_token();
	}
	current_token = token;
}

int numCharToInt(char *ch)
{
	int size = strlen(ch);
	int res = 0;
	for (int i = 0; i < size; i++)
	{
		res = res * 10;
		res += (int)(ch[i] - '0');
	}
	return res;
}

//Check if both variable have the same type
void variablesTypeMatched(Token* leftToken, FILE* yyout2)
{
	eTOKENS idTypeToken;
	if (currentAttributes != NULL)
	{
		idTypeToken = currentAttributes->type;
		Another_Token = peekNext();
		if (Another_Token->kind == TOKEN_ID) {
			currentAttributes = find(currentTable, Another_Token->lexeme);
			if (currentAttributes != NULL)
			{
				rightToken = currentAttributes->type;

				if (((idTypeToken == TOK_KW_REAL) && ((rightToken == TOKEN_NUMBER_INTEGER) || (rightToken == TOK_KW_INTEGER))) || ((idTypeToken == TOK_KW_INTEGER) && (rightToken == TOKEN_NUMBER_REAL) || (rightToken == TOK_KW_REAL))) {
					fprintf(yyout2, "Type inconsistency between the left side and the right side at line: %d\n", current_token->lexeme, current_token->lineNumber);
				}
			}
			else
			{
				fprintf(yyout2, "%s not declared in this scope: %d\n", Another_Token->lexeme, current_token->lineNumber);
			}
		}
	}
	else
	{
		fprintf(yyout2, "%s not declared in this scope: %d\n", leftToken->lexeme, current_token->lineNumber);
	}

}

bool isFollow(int* followToken, int numOfFollow, eTOKENS testedToken) 
{
	int i = 0;
	for (i = 0; i < numOfFollow; i++) {
		if (followToken[i] == testedToken)
			return true;
	}
	return false;
}

Token* peekNext() {
	Token* nextTokenTemp = next_token();
	back_token();
	return nextTokenTemp;
}

void tokenByName(eTOKENS token, char* str[]) 
{
	char* tokenName = (char*)malloc(sizeof(char) * 20);
	switch (token)
	{
			case TOK_EOF:
				tokenName = "EOF";
				break;

			case TOK_KW_BLOCK:
				tokenName = "Block";
				break;

			case TOK_KW_BEGIN:
				tokenName = "Begin";
				break;

			case TOK_KW_END:
				tokenName = "End";
				break;

			case TOK_KW_ARRAY:
				tokenName = "Array";
				break;

			case TOK_KW_OF:
				tokenName = "Of";
				break;

			case TOK_KW_INTEGER:
				tokenName = "Integer";
				break;

			case TOK_KW_REAL:
				tokenName = "Real";
				break;

			case TOK_KW_TYPE:
				tokenName = "type";
				break;

			case TOK_KW_IS:
				tokenName = "Is";
				break;

			case TOK_KW_ENUM:
				tokenName = "Enum";
				break;

			case TOK_KW_STRUCT:
				tokenName = "Struct";
				break;

			case TOK_KW_SWITCH:
				tokenName = "Switch";
				break;

			case TOK_KW_DEFAULT:
				tokenName = "Default";
				break;

			case TOK_KW_BREAK:
				tokenName = "Break";
				break;

			case TOK_KW_CASE:
				tokenName = "Case";
				break;


			case TOKEN_ID:
				tokenName = "ID";
				break;

			case TOKEN_REAL_NAME:
				tokenName = "Name";
				break;

			case TOKEN_NUMBER_INTEGER:
				tokenName = "Integer Number";
				break;

			case TOKEN_NUMBER_REAL:
				tokenName = "Real Number";
				break;

			case TOK_SEP_COMMA:
				tokenName = "Comma";
				break;

			case TOK_SEP_DOT:	
				tokenName = "Dot";
				break;

			case TOK_SEP_COLON:
				tokenName = "Colon";
				break;

			case TOK_SEP_SEMICOLON:
				tokenName = "Semicolon";
				break;

			case TOK_SEP_L_PARENTHESES:
				tokenName = "Left Parentheses";
				break;

			case TOK_SEP_R_PARENTHESES:
				tokenName = "Right Parentheses";
				break;

			case TOK_SEP_L_BRACKETS:
				tokenName = "Left Brackets";
				break;

			case TOK_SEP_R_BRACKETS:
				tokenName = "Right Brackets";
				break;

			case TOK_SEP_L_CURLY_BRACES:
				tokenName = "Left Braces";
				break;

			case TOK_SEP_R_CURLY_BRACES:
				tokenName = "Right Braces";
				break;

			case TOKEN_OP_ADD:
				tokenName = "Addition";
				break;

			case TOKEN_OP_SUB:
				tokenName = "Subtraction";
				break;

			case TOKEN_OP_MUL:
				tokenName = "Multiplication,";
				break;

			case TOKEN_OP_DIV:
				tokenName = "Division";
				break;

			case TOKEN_OP_EQUAL:
				tokenName = "Assignment";
				break;

			default:
			break;
	}

	*str = tokenName;

}

void match(eTOKENS token, FILE* yyout) 
{

	if (token == currentNode->tokensArray[currentIndex].kind) {
		current_token = next_token();
	}
	else if (token == TOK_EOF)
	{
		//do nothing;
	}
	else
	{
		char* currentTokenName, * correctTokenName;

		tokenByName(token, &correctTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout, "Expected token of type %s at line %d,\n", correctTokenName, current_token->lineNumber);
		fprintf(yyout, "Actual token of type %s, lexeme:\'\'%s\'\'\n", currentTokenName , current_token->lexeme);
		current_token = next_token();

	}
}


void syntax_Analysis(FILE* yyout,FILE* yyout2) {
	current_token = getFirstToken();
	if (current_token != NULL)
	{
		parse_PROGRAM(yyout, yyout2);
	}
	match(TOK_EOF, yyout);
}

void parse_PROGRAM(FILE* yyout,FILE* yyout2)
{
	
	fprintf(yyout, "PROGRAM -> BLOCK\n");
	parse_BLOCK(yyout, yyout2);
	match(TOK_EOF,yyout);
}

void parse_BLOCK(FILE* yyout,FILE* yyout2)
{
	currentTable = addSymbolTableNode(currentTable);
	if (currentTable != NULL) {
		printf("\nCurrent table not null\n");
	}
	fprintf(yyout, "BLOCK -> block DECLARATIONS begin STATEMENTS end\n");
	match(TOK_KW_BLOCK, yyout);
	parse_DECLARATIONS(yyout, yyout2);
	match(TOK_KW_BEGIN, yyout);
	parse_STATEMENTS(yyout, yyout2);
	match(TOK_KW_END, yyout);
	currentTable = popTable(currentTable);
}

void parse_DECLARATIONS(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "DECLARATIONS -> DECLARATION DECLARATIONS_TAG\n");
	parse_DECLARATION(yyout, yyout2);
	parse_DECLARATIONS_TAG(yyout, yyout2);
}


void parse_DECLARATIONS_TAG(FILE* yyout,FILE* yyout2)
{
	int followToken[] = {TOK_KW_BEGIN };
	char* SemicolonTokenName, * BeginTokenName, * currentTokenName;

	switch (current_token->kind) 
	{
	case TOK_SEP_SEMICOLON:
		Another_Token = peekNext();
		switch (Another_Token->kind)
		{
		case TOKEN_ID:
			fprintf(yyout, "DECLARATIONS_TAG -> ; DECLARATIONS\n");
			current_token = next_token();
			parse_DECLARATIONS(yyout, yyout2);
			break;

		case TOK_KW_TYPE:
			fprintf(yyout, "DECLARATIONS_TAG -> ; DECLARATIONS\n");
			current_token = next_token();
			parse_DECLARATIONS(yyout, yyout2);
			break;

		case TOK_KW_BEGIN:
			match(TOK_KW_BEGIN, yyout);
			break;

		default:
			break;
		}
		break;

	case TOK_KW_BEGIN:
		fprintf(yyout, "DECLARATIONS_TAG -> Epsilon\n");
		break;

	default:
		tokenByName(TOK_SEP_SEMICOLON, &SemicolonTokenName);
		tokenByName(TOK_KW_BEGIN, &BeginTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", SemicolonTokenName, BeginTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_DECLARATION(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_SEMICOLON,TOK_KW_BEGIN };
	char* IDTokenName, * TypeTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOKEN_ID:
		fprintf(yyout, "DECLARATION -> VAR_DECLARATION\n");
		parse_VAR_DECLARATION(yyout, yyout2);
		break;

	case TOK_KW_TYPE:
		fprintf(yyout, "DECLARATION -> TYPE_DECLARATION\n");
		parse_TYPE_DECLARATION(yyout, yyout2);
		break;

	default:
		tokenByName(TOKEN_ID, &IDTokenName);
		tokenByName(TOK_KW_TYPE, &TypeTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", IDTokenName, TypeTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 2);
		break;
	}
}

void parse_VAR_DECLARATION(FILE* yyout,FILE* yyout2)
{

	if (TOKEN_ID==current_token->kind) {
		currentAttributes = insert(currentTable->symboleTable, current_token);
		if (currentAttributes == NULL) {
			fprintf(yyout2, "Duplicate declaration of the same name, line: %d\n", current_token->lineNumber);
		}
	}
	fprintf(yyout, "VAR_DECLARATION->id: VAR_DECLARATION_TAG\n");
	match(TOKEN_ID, yyout);
	match(TOK_SEP_COLON, yyout);
	parse_VAR_DECLARATION_TAG(yyout, yyout2);

}

void parse_VAR_DECLARATION_TAG(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_SEMICOLON,TOK_KW_BEGIN,TOK_SEP_R_CURLY_BRACES};
	char* ArrayTokenName, * RealNameTokenName, * IntegerTokenName, * RealNumberTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOK_KW_ARRAY:
		fprintf(yyout, "VAR_DECLARATION_TAG  -> array [SIZE] of SIMPLE_TYPE\n");
		if (currentAttributes != NULL)
		{
			currentAttributes->role = varviable;
			currentAttributes->category= _array;
		}
		current_token = next_token();
		match(TOK_SEP_L_BRACKETS, yyout);
		parse_SIZE(yyout, yyout2);

		match(TOK_SEP_R_BRACKETS, yyout);
		match(TOK_KW_OF, yyout);
		parse_SIMPLE_TYPE(yyout, yyout2);
		break;

	case TOKEN_REAL_NAME:
		fprintf(yyout, "VAR_DECLARATION_TAG  -> type_name\n");
		current_token = next_token();
		break;

	case TOK_KW_INTEGER:
		fprintf(yyout, "VAR_DECLARATION_TAG  -> SIMPLE_TYPE\n");
	
		parse_SIMPLE_TYPE(yyout, yyout2);
		break;

	case TOK_KW_REAL:
		fprintf(yyout, "VAR_DECLARATION_TAG  -> SIMPLE_TYPE\n");
		if (currentAttributes != NULL) {
			currentAttributes->role = varviable;
			currentAttributes->category = _simple;
		}
		parse_SIMPLE_TYPE(yyout, yyout2);
		break;

	default:
		tokenByName(TOK_KW_ARRAY, &ArrayTokenName);
		tokenByName(TOKEN_REAL_NAME, &RealNameTokenName);
		tokenByName(TOKEN_NUMBER_INTEGER, &IntegerTokenName);
		tokenByName(TOKEN_NUMBER_REAL, &RealNumberTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s, %s, %s at line %d,\n", ArrayTokenName, RealNameTokenName, IntegerTokenName, RealNumberTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 3);
		break;
	}
}

void parse_SIZE(FILE* yyout,FILE* yyout2)
{
	int size;
	int followToken[] = { TOK_SEP_R_BRACKETS };
	fprintf(yyout, "SIZE -> int_num\n");
	if (current_token->kind == TOKEN_NUMBER_INTEGER)
	{
		size = numCharToInt(current_token->lexeme);
		if (currentAttributes != NULL)
		{
			currentAttributes->size = size;
		}
		match(TOKEN_NUMBER_INTEGER, yyout);
	}
}

void parse_SIMPLE_TYPE(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_SEMICOLON };
	char* IntegerTokenName, * RealNumberTokenName, * currentTokenName;

	
	char* tokenName;
	tokenByName(current_token->kind, &tokenName);

	switch (current_token->kind)
	{
	case TOK_KW_INTEGER:
		fprintf(yyout, "SIMPLE_TYPE -> integer\n");
		if (currentAttributes != NULL)
		{
			currentAttributes->type = TOK_KW_INTEGER;
		}
		current_token = next_token();
		break;

	case TOK_KW_REAL:
		fprintf(yyout, "SIMPLE_TYPE ->real\n");
		if (currentAttributes != NULL)
		{
			currentAttributes->type = TOK_KW_REAL;
		}
		current_token = next_token();
		break;

	default:
		tokenByName(TOKEN_NUMBER_INTEGER, &IntegerTokenName);
		tokenByName(TOKEN_NUMBER_REAL, &RealNumberTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", IntegerTokenName, RealNumberTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);		
		break;
	}

}

void parse_TYPE_DECLARATION(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "TYPE_DECLARATION -> type type_name is TYPE_INDICATOR\n");
	match(TOK_KW_TYPE, yyout);
	if (TOKEN_REAL_NAME == current_token->kind) {
		currentAttributes = insert(currentTable->symboleTable, current_token);
		if (currentAttributes == NULL) {
			fprintf(yyout2, "Duplicate declaration of the same name, line: %d\n", current_token->lineNumber);
		}
	}
	match(TOKEN_REAL_NAME, yyout);

	match(TOK_KW_IS, yyout);
	parse_TYPE_INDICATOR(yyout, yyout2);
}

void parse_TYPE_INDICATOR(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_SEMICOLON };
	char* EnumTokenName, * StructNumberTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOK_KW_ENUM:
		fprintf(yyout, "TYPE_INDICATOR -> ENUM_TYPE\n");
		parse_ENUM_TYPE(yyout, yyout2);
		break;

	case TOK_KW_STRUCT:
		fprintf(yyout, "TYPE_INDICATOR -> STRUCTURE_TYPE\n");
		parse_STRUCTURE_TYPE(yyout, yyout2);
		break;

	default:
		tokenByName(TOK_KW_ENUM, &EnumTokenName);
		tokenByName(TOK_KW_STRUCT, &StructNumberTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", EnumTokenName, StructNumberTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_ENUM_TYPE(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "ENUM_TYPE -> enum { ID_LIST }\n");
	inEnum = true;
	match(TOK_KW_ENUM, yyout);

	match(TOK_SEP_L_CURLY_BRACES, yyout);
	parse_ID_LIST(yyout, yyout2);
	match(TOK_SEP_R_CURLY_BRACES, yyout);
	inEnum = false;
}

void parse_ID_LIST(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "ID_LIST -> id ID_LIST_TAG\n");
	match(TOKEN_ID, yyout);
	parse_ID_LIST_TAG(yyout, yyout2);
}

void parse_ID_LIST_TAG(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_R_CURLY_BRACES };
	char* CommaTokenName, * RightCurrlyBracesTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOK_SEP_COMMA:
		fprintf(yyout, "ID_LIST_TAG -> ID_LIST\n");
		current_token = next_token();
		parse_ID_LIST(yyout, yyout2);
		break;

	case TOK_SEP_R_CURLY_BRACES:
		fprintf(yyout, "ID_LIST_TAG -> epsilon\n");
		break;

	default:
		tokenByName(TOK_SEP_COMMA, &CommaTokenName);
		tokenByName(TOK_SEP_R_CURLY_BRACES, &RightCurrlyBracesTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", CommaTokenName, RightCurrlyBracesTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_STRUCTURE_TYPE(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "STRUCTURE_TYPE -> struct { FIELDS }\n");
	match(TOK_KW_STRUCT, yyout);
	match(TOK_SEP_L_CURLY_BRACES, yyout);
	parse_FIELDS(yyout, yyout2);
	match(TOK_SEP_R_CURLY_BRACES, yyout);
}

void parse_FIELDS(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "FIELDS -> FIELD FIELDS_TAG\n");
	parse_FIELD(yyout, yyout2);
	parse_FIELDS_TAG(yyout, yyout2);
}

void parse_FIELDS_TAG(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_R_CURLY_BRACES };
	char* SemicolonTokenName, * RightCurrlyBracesTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOK_SEP_SEMICOLON:
		fprintf(yyout, "FIELDS_TAG  -> ; FIELD FIELDS_TAG\n");
		current_token = next_token();
		parse_FIELD(yyout, yyout2);
		parse_FIELDS_TAG(yyout, yyout2);
		break;

	case TOK_SEP_R_CURLY_BRACES:
		fprintf(yyout, "FIELDS_TAG  -> epsilon\n");
		break;

	default:
		tokenByName(TOK_SEP_SEMICOLON, &SemicolonTokenName);
		tokenByName(TOK_SEP_R_CURLY_BRACES, &RightCurrlyBracesTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", SemicolonTokenName, RightCurrlyBracesTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}

}

void parse_FIELD(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "FIELD -> VAR_ DECLARATION\n");
	parse_VAR_DECLARATION(yyout, yyout2);
}


void parse_STATEMENTS(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "STATEMENTS -> STATEMENT STATEMENTS_TAG\n");
	parse_STATEMENT(yyout, yyout2);
	parse_STATEMENTS_TAG(yyout, yyout2);
}

void parse_STATEMENTS_TAG(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_R_CURLY_BRACES,TOK_KW_END };
	char* SemicolonTokenName, * RightCurrlyBracesTokenName, * EndTokenNamem,* currentTokenName;

	switch (current_token->kind)
	{
	case TOK_SEP_SEMICOLON:
		fprintf(yyout, "STATEMENTS_TAG  -> ; STATEMENT STATEMENTS_TAG\n");
		current_token = next_token();
		parse_STATEMENT(yyout, yyout2);
		parse_STATEMENTS_TAG(yyout, yyout2);
		break;

	case TOK_SEP_R_CURLY_BRACES:
		fprintf(yyout, "STATEMENTS_TAG  -> epsilon\n");
		break;

	case TOK_KW_END:
		fprintf(yyout, "STATEMENTS_TAG  -> epsilon\n");
		break;

	default:
		tokenByName(TOK_SEP_SEMICOLON, &SemicolonTokenName);
		tokenByName(TOK_SEP_R_CURLY_BRACES, &RightCurrlyBracesTokenName);
		tokenByName(TOK_KW_END, &EndTokenNamem);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s, %s at line %d,\n", SemicolonTokenName, RightCurrlyBracesTokenName, EndTokenNamem, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 2);	
		break;
	}

}

void parse_STATEMENT(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_SEMICOLON, TOK_KW_END, TOK_SEP_R_CURLY_BRACES };
	char* IdTokenName, * SwtichTokenName, * BreakTokenNamem, * BlockTokenNamem,* currentTokenName;

	switch (current_token->kind)
	{
	case TOKEN_ID:
		fprintf(yyout, "STATEMENT -> VAR_ELEMENT = EXPRESSION\n");
		parse_VAR_ELEMENT(yyout, yyout2);
		match(TOKEN_OP_EQUAL, yyout, yyout2);
		parse_EXPRESSION(yyout, yyout2);
		break;

	case TOK_KW_SWITCH:
		fprintf(yyout, "STATEMENT -> switch (KEY) { CASE_LIST; default: STATEMENTS }\n");
		current_token = next_token();
		match(TOK_SEP_L_PARENTHESES, yyout);
		parse_KEY(yyout, yyout2);
		match(TOK_SEP_R_PARENTHESES, yyout);
		match(TOK_SEP_L_CURLY_BRACES, yyout);
		parse_CASE_LIST(yyout, yyout2);
		match(TOK_SEP_SEMICOLON, yyout);
		match(TOK_KW_DEFAULT, yyout);
		match(TOK_SEP_COLON, yyout);
		parse_STATEMENTS(yyout, yyout2);
		match(TOK_SEP_R_CURLY_BRACES, yyout);
		break;

	case TOK_KW_BREAK:
		fprintf(yyout, "STATEMENT -> break\n");
		current_token = next_token();
		break;

	case TOK_KW_BLOCK:
		fprintf(yyout, "STATEMENT -> BLOCK\n");
		parse_BLOCK(yyout, yyout2);
		break;

	default:
		tokenByName(TOKEN_ID, &IdTokenName);
		tokenByName(TOK_KW_SWITCH, &SwtichTokenName);
		tokenByName(TOK_KW_BREAK, &BreakTokenNamem);
		tokenByName(TOK_KW_BLOCK, &BlockTokenNamem);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s, %s, %s at line %d,\n", IdTokenName, SwtichTokenName, BreakTokenNamem, BlockTokenNamem, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 3);
		break;
	}
}

void parse_VAR_ELEMENT(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOKEN_OP_EQUAL,TOKEN_OP_ADD,TOKEN_OP_DIV,TOKEN_OP_MUL,TOKEN_OP_SUB,TOK_SEP_SEMICOLON,TOK_SEP_R_BRACKETS,TOK_SEP_R_PARENTHESES };
	char* IdTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOKEN_ID:
		Another_Token = peekNext();
		switch (Another_Token->kind)
		{
		case TOK_SEP_L_BRACKETS:
			fprintf(yyout, "VAR_ELEMENT -> id[EXPRESSION]\n");
			currentArrayToken = current_token;
			current_token = next_token();
			match(TOK_SEP_L_BRACKETS, yyout);
			parse_EXPRESSION(yyout, yyout2);
			match(TOK_SEP_R_BRACKETS, yyout);
			break;

		case TOK_SEP_DOT:
			fprintf(yyout, "VAR_ELEMENT -> FIELD_ACCESS\n");
			parse_FIELD_ACCESS(yyout, yyout2);
			break;

		case TOKEN_OP_ADD:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			if (current_token == TOK_SEP_R_BRACKETS)
			{
				leftToken = currentArrayToken;
			}
			else
			{
				leftToken = current_token;
			}
			currentAttributes = find(currentTable, current_token->lexeme);
			current_token = next_token();
			variablesTypeMatched(leftToken, yyout2);
			break;

		case TOKEN_OP_SUB:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			if (current_token == TOK_SEP_R_BRACKETS)
			{
				leftToken = currentArrayToken;
			}
			else
			{
				leftToken = current_token;
			}
			currentAttributes = find(currentTable, current_token->lexeme);
			current_token = next_token();
			variablesTypeMatched(leftToken, yyout2);
			break;

		case TOKEN_OP_MUL:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			if (current_token == TOK_SEP_R_BRACKETS)
			{
				leftToken = currentArrayToken;
			}
			else
			{
				leftToken = current_token;
			}
			currentAttributes = find(currentTable, current_token->lexeme);
			current_token = next_token();
			variablesTypeMatched(leftToken, yyout2);
			break;

		case TOKEN_OP_DIV:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			if (current_token == TOK_SEP_R_BRACKETS)
			{
				leftToken = currentArrayToken;
			}
			else
			{
				leftToken = current_token;
			}
			currentAttributes = find(currentTable, current_token->lexeme);
			current_token = next_token();
			variablesTypeMatched(leftToken, yyout2);
			break;

		case TOKEN_OP_EQUAL:
			fprintf(yyout, "VAR_ELEMENT -> id\n");

			if (current_token->kind == TOK_SEP_R_BRACKETS)
			{
				leftToken = currentArrayToken;
			}
			else
			{
				leftToken = current_token;
			}
			currentAttributes = find(currentTable, current_token->lexeme);
			current_token = next_token();
			variablesTypeMatched(leftToken, yyout2);
			break;

		case TOK_SEP_SEMICOLON:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			current_token = next_token();
			break;

		case TOK_SEP_R_BRACKETS:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			current_token = next_token();
			break;

		case TOK_SEP_R_PARENTHESES:
			fprintf(yyout, "VAR_ELEMENT -> id\n");
			current_token = next_token();
			break;

		default:
			break;
		}
		break;

	default:
		tokenByName(TOKEN_ID, &IdTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s at line %d,\n", IdTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 8);
		break;
	}
}

void parse_KEY(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "KEY -> VAR_ELEMENT\n");
	parse_VAR_ELEMENT(yyout, yyout2);
}

void parse_CASE_LIST(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "CASE_LIST -> CASE CASE_LIST_TAG\n");
	parse_CASE(yyout, yyout2);
	parse_CASE_LIST_TAG(yyout, yyout2);
}


void parse_CASE_LIST_TAG(FILE* yyout,FILE* yyout2) 
{
		int followToken[] = { TOK_SEP_SEMICOLON };
		char* CaseTokenName, * SemicolonTokenName, * currentTokenName;

		switch (current_token->kind)
		{
		case TOK_KW_CASE:
			fprintf(yyout, "CASE_LIST_TAG  -> CASE CASE_LIST_TAG\n");
			parse_CASE(yyout, yyout2);
			parse_CASE_LIST_TAG(yyout, yyout2);
			break;
		
		case TOK_SEP_SEMICOLON:
			fprintf(yyout, "CASE_LIST_TAG  -> epsilon\n");
			break;

		default:
			tokenByName(TOK_KW_CASE, &CaseTokenName);
			tokenByName(TOK_SEP_SEMICOLON, &SemicolonTokenName);
			tokenByName(current_token->kind, &currentTokenName);
			fprintf(yyout,"Expected token of type %s, %s at line %d,\n", CaseTokenName, SemicolonTokenName, current_token->lineNumber);
			fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
			errorRecover(followToken, 1);
			break;
		}
}

void parse_CASE(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "CASE -> case KEY_VALUE : { STATEMENTS }\n");
	match(TOK_KW_CASE, yyout);
	parse_KEY_VALUE(yyout, yyout2);
	match(TOK_SEP_COLON, yyout);
	match(TOK_SEP_L_CURLY_BRACES, yyout);
	parse_STATEMENTS(yyout, yyout2);
	match(TOK_SEP_R_CURLY_BRACES, yyout);
}

void parse_KEY_VALUE(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_COLON };
	char* IntegerTokenName, * IDTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOKEN_NUMBER_INTEGER:
		fprintf(yyout, "KEY_VALUE -> int_num\n");
		current_token = next_token();
		break;

	case TOKEN_ID:
		fprintf(yyout, "KEY_VALUE -> id\n");
		current_token = next_token();
		break;

	default:
		tokenByName(TOKEN_NUMBER_INTEGER, &IntegerTokenName);
		tokenByName(TOKEN_ID, &IDTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s at line %d,\n", IntegerTokenName, IDTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_FIELD_ACCESS(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "FIELD_ACCESS -> id. FIELD_ACCESS_TAG\n");
	match(TOKEN_ID, yyout);
	match(TOK_SEP_DOT, yyout);
	parse_FIELD_ACCESS_TAG(yyout, yyout2);
}

void parse_FIELD_ACCESS_TAG(FILE* yyout,FILE* yyout2)
{
	
	int followToken[] = { TOKEN_OP_EQUAL,TOKEN_OP_ADD,TOKEN_OP_DIV,TOKEN_OP_MUL,TOKEN_OP_SUB,TOK_SEP_SEMICOLON,TOK_SEP_R_BRACKETS,TOK_SEP_R_PARENTHESES };
	char* IdTokenName, * currentTokenName;

	switch (current_token->kind)
	{

	case TOKEN_ID:

		Another_Token = peekNext();
		switch (Another_Token->kind)
		{

		case TOK_SEP_DOT:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->FIELD_ACCESS\n");
			parse_FIELD_ACCESS(yyout, yyout2);
			break;
		
		case TOKEN_OP_ADD:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;

		case TOKEN_OP_SUB:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;

		case TOKEN_OP_MUL:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");

			break;

		case TOKEN_OP_DIV:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;

		case TOKEN_OP_EQUAL:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;

		case TOK_SEP_SEMICOLON:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;

		case TOK_SEP_R_BRACKETS:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;
		case TOK_SEP_R_PARENTHESES:
			fprintf(yyout, "FIELD_ACCESS_TAG  ->id\n");
			current_token = next_token();
			break;

		default:
			break;
		}
		break;
	
	default:
		tokenByName(TOKEN_ID, &IdTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s at line %d,\n", IdTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 8);
		break;
	}
}

void parse_EXPRESSION(FILE* yyout,FILE* yyout2)
{
	fprintf(yyout, "EXPRESSION -> SIMPLE_EXPRESSION EXPRESSION_TAG \n");
	parse_SIMPLE_EXPRESSION(yyout, yyout2);
	parse_EXPRESSION_TAG(yyout, yyout2);
}

void parse_EXPRESSION_TAG(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOK_SEP_R_BRACKETS, TOK_SEP_SEMICOLON, TOK_KW_END, TOK_SEP_L_CURLY_BRACES};
	char* AddTokenName, * SubTokenName, * MulTokenName, * DivTokenName,* RightBracketsTokenName,* SemicolonTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOKEN_OP_ADD:
		fprintf(yyout, "EXPRESSION_TAG  -> ar_op EXPRESSION\n");
		current_token = next_token();
		parse_EXPRESSION(yyout, yyout2);
		break;

	case TOKEN_OP_SUB:
		fprintf(yyout, "EXPRESSION_TAG  -> ar_op EXPRESSION\n");
		current_token = next_token();
		parse_EXPRESSION(yyout, yyout2);
		break;

	case TOKEN_OP_MUL:
		fprintf(yyout, "EXPRESSION_TAG  -> ar_op EXPRESSION\n");
		current_token = next_token();
		parse_EXPRESSION(yyout, yyout2);
		break;

	case TOKEN_OP_DIV:
		fprintf(yyout, "EXPRESSION_TAG  -> ar_op EXPRESSION\n");
		current_token = next_token();
		parse_EXPRESSION(yyout, yyout2);
		break;

	case TOK_SEP_R_BRACKETS:
		fprintf(yyout, "EXPRESSION_TAG  -> epsilonn");
		break;

	case TOK_SEP_SEMICOLON:
		fprintf(yyout, "EXPRESSION_TAG  -> epsilonn\n");
		break;

	case TOK_SEP_R_CURLY_BRACES:
		fprintf(yyout, "EXPRESSION_TAG  -> epsilonn\n");
		break;

	case TOK_KW_END:
		fprintf(yyout, "EXPRESSION_TAG  -> epsilonn\n");
		break;

	default:
		tokenByName(TOKEN_OP_ADD, &AddTokenName);
		tokenByName(TOKEN_OP_SUB, &SubTokenName);
		tokenByName(TOKEN_OP_MUL, &MulTokenName);
		tokenByName(TOKEN_OP_DIV, &DivTokenName);
		tokenByName(TOK_SEP_R_BRACKETS, &RightBracketsTokenName);
		tokenByName(TOK_SEP_SEMICOLON, &SemicolonTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout,"Expected token of type %s, %s, %s, %s, %s, %s at line %d,\n", AddTokenName, SubTokenName, MulTokenName, DivTokenName, RightBracketsTokenName, SemicolonTokenName, current_token->lineNumber);
		fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 4);
		break;
	}
}

void parse_SIMPLE_EXPRESSION(FILE* yyout,FILE* yyout2)
{
	int followToken[] = { TOKEN_OP_ADD,TOKEN_OP_DIV,TOKEN_OP_MUL,TOKEN_OP_SUB,TOK_SEP_SEMICOLON,TOK_SEP_R_BRACKETS };
	char* IntegerTokenName, * RealNumberTokenName, * IDTokenName, * currentTokenName;

	switch (current_token->kind)
	{
	case TOKEN_NUMBER_INTEGER:
		fprintf(yyout, "SIMPLE_EXPRESSION -> int_num\n");
		current_token = next_token();
		break;

	case TOKEN_NUMBER_REAL:
		fprintf(yyout, "SIMPLE_EXPRESSION -> real_num\n");
		current_token = next_token();
		break;

	case TOKEN_ID:
		fprintf(yyout, "SIMPLE_EXPRESSION -> VAR_ELEMENT\n");
		parse_VAR_ELEMENT(yyout, yyout2);
		break;
	
		default:
			tokenByName(TOKEN_NUMBER_INTEGER, &IntegerTokenName);
			tokenByName(TOKEN_NUMBER_REAL, &RealNumberTokenName);
			tokenByName(TOKEN_ID, &IDTokenName);
			tokenByName(current_token->kind, &currentTokenName);
			fprintf(yyout,"Expected token of type %s, %s, %s at line %d,\n", IntegerTokenName,RealNumberTokenName,IDTokenName, current_token->lineNumber);
			fprintf(yyout,"Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
			errorRecover(followToken, 7);
			break;
	}
}