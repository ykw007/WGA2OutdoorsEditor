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


//## 3d 파일명을 입력받아 프로그램에서 사용할 모델로 변환
bool CModel::ConvertRaw3dData( const char *pFilePathName, LPDIRECT3DDEVICE9 pd3dDevice )
{
	bool bReturn = true;

	if( pFilePathName != NULL )
	{
		//## parsing start
		//## 파일에 해당하는 parser 생성
		CParserSelector ParserSelector;
		CParser *pParser = ParserSelector.Select( pFilePathName );

		if( pParser == NULL )
		{
			bReturn = false;
		}
		else
		{
			//## parser 실행
			if( !pParser->Run() )
			{
				bReturn = false;
			}
			else
			{
				//## CModel class 로 변환
				if( !pParser->Convert( this, pd3dDevice ) )
					bReturn = false;
			}

			//## parser 제거
			SAFE_DELETE( pParser );
		}
	}

	return bReturn;
}