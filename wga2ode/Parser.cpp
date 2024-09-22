#include "StdAfx.h"
#include "parser.h"

CParser::CParser( const char *pFilePathName )
{
	m_pFilePathName = pFilePathName;
	m_fpFile = NULL;
}

CParser::~CParser(void)
{
}
