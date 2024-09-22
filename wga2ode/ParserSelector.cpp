#include "StdAfx.h"
#include "parserselector.h"

#include "aseparser.h"
#include "3dsparser.h"

CParserSelector::CParserSelector()
{
	//## Ȯ���� 
}

CParserSelector::~CParserSelector()
{
}

CParser *CParserSelector::Select( const char *pFilePathName )
{
	//## ���� Ȯ���� üũ
	char	*pStr = NULL;
	char	*pToken = NULL;
	char	cTemp[255];
	char	sep[] = ".";

	strcpy( cTemp, pFilePathName);

	pToken = strtok( cTemp, sep );

	while( pToken != 0 )
	{
		pStr = pToken;
		pToken = strtok( 0, sep);
	}

	//## �ش� PARSER �۾��� pointer ��ȯ
	if( _stricmp( pStr, ASE_FORMAT ) == 0 )
	{
		return new CAseParser( pFilePathName );
	}
	else if( _stricmp( pStr, TDS_FORMAT ) == 0 )
	{
		return new C3dsParser( pFilePathName );
	}

	WGA2_MESSAGE( "3D ����Ÿ ������ �ƴմϴ�." );

	return NULL;
}