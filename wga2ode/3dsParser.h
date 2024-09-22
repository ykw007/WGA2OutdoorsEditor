#ifndef _3DSPARSER_H_
#define _3DSPARSER_H_

#include "parser.h"

class C3dsParser : public CParser
{
public:
	C3dsParser( const char *pFilePathName );
	~C3dsParser(void);

	bool Run();
	bool Convert( CModel *pModel, LPDIRECT3DDEVICE9 pd3dDevice );
};

#endif