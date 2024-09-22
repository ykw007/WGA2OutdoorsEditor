#include "StdAfx.h"
#include "model.h"
#include "parserselector.h"

CModel::CModel(void)
{

}

CModel::~CModel(void)
{
	if( !m_MtrlMeshList.empty() )
		stl_wipe( m_MtrlMeshList );
}


//## 3d ���ϸ��� �Է¹޾� ���α׷����� ����� �𵨷� ��ȯ
bool CModel::ConvertRaw3dData( const char *pFilePathName, LPDIRECT3DDEVICE9 pd3dDevice )
{
	bool bReturn = true;

	if( pFilePathName != NULL )
	{
		//## parsing start
		//## ���Ͽ� �ش��ϴ� parser ����
		CParserSelector ParserSelector;
		CParser *pParser = ParserSelector.Select( pFilePathName );

		if( pParser == NULL )
		{
			bReturn = false;
		}
		else
		{
			//## parser ����
			if( !pParser->Run() )
			{
				bReturn = false;
			}
			else
			{
				//## CModel class �� ��ȯ
				if( !pParser->Convert( this, pd3dDevice ) )
					bReturn = false;
			}

			//## parser ����
			SAFE_DELETE( pParser );
		}
	}

	return bReturn;
}