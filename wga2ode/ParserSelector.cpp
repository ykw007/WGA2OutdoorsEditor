#include "StdAfx.h"
#include "parserselector.h"

#include "aseparser.h"
#include "3dsparser.h"

CParserSelector::CParserSelector()
{
	//## 확장자 
}

CParserSelector::~CParserSelector()
{
}

CParser *CParserSelector::Select( const char *pFilePathName )
{
	//## 파일 확장자 체크
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

	//## 해당 PARSER 작업자 pointer 반환
	if( _stricmp( pStr, ASE_FORMAT ) == 0 )
	{
		return new CAseParser( pFilePathName );
	}
	else if( _stricmp( pStr, TDS_FORMAT ) == 0 )
	{
		return new C3dsParser( pFilePathName );
	}

	WGA2_MESSAGE( "3D 데이타 형식이 아닙니다." );

	return NULL;
}