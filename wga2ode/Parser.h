#ifndef _PARSER_H_
#define _PARSER_H_

#include "misc.h"
#include "model.h"

class CParser
{
public:
	CParser( const char *pFilePathName );
	virtual ~CParser(void);

	virtual bool Run() = 0;
	virtual bool Convert( CModel *pModel, LPDIRECT3DDEVICE9 pd3dDevice ) = 0;

protected:
	const char			*m_pFilePathName;
	FILE				*m_fpFile;
};

#endif