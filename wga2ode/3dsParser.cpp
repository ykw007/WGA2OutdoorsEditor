#include "StdAfx.h"
#include "3dsparser.h"

C3dsParser::C3dsParser( const char *pFilePathName ) : CParser( pFilePathName )
{
	::MessageBox(NULL,"3dsParser 생성","TEST",MB_OK);
}

C3dsParser::~C3dsParser(void)
{
}

//####################################################
// run parsing
//####################################################
bool C3dsParser::Run()
{
	::MessageBox(NULL,"3dsParser 실행","TEST",MB_OK);
	::MessageBox(NULL,m_pFilePathName,"TEST",MB_OK);

	return true;
}

//####################################################
// converting
//####################################################
bool C3dsParser::Convert( CModel *pModel, LPDIRECT3DDEVICE9 pd3dDevice )
{
	return true;
}
