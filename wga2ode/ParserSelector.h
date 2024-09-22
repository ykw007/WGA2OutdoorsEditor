#ifndef _PARSERSELECTOR_H_
#define _PARSERSELECTOR_H_

#include "parser.h"

//class CParser;
#define ASE_FORMAT		"ase"
#define TDS_FORMAT		"3ds"

class CParserSelector
{
public:
	enum E_PARSER_ID
	{
		ASE, TDS
	};

	virtual CParser *Select( const char *pFilePathName );

	CParserSelector();
	~CParserSelector();
};

#endif