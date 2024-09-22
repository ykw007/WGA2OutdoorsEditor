#include "StdAfx.h"
#include "texturemanager.h"

CTextureManager::CTextureManager()
{
	m_pd3dDevice = NULL;
}

CTextureManager::~CTextureManager(void)
{
	Destroy();
}

void CTextureManager::Destroy()
{
	if( !m_vecTexture.empty() )
		stl_wipe( m_vecTexture );
}

void CTextureManager::Init(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if( m_pd3dDevice == NULL )
        m_pd3dDevice = pd3dDevice;
}

//######################################################################
//## texture를 등록한다.
//######################################################################
int CTextureManager::RegisterTexture( char *pImageFile )
{
	char *pTextureFile;
	int nIndex = 0;

	//## 등록된 텍스쳐를 모두 체크해서 같은 텍스쳐이면 id를 넘겨준다.
	//## vector가 비어있지 않다면 
	if( !m_vecTexture.empty() )
	{
		//## loop 하면서 중복되는 ctrlID가 있는지 검사한다.
		for( vecTextureItor Itor = m_vecTexture.begin(); Itor != m_vecTexture.end(); ++Itor )
		{
			pTextureFile = (static_cast<S_TEXTURE *>(*Itor))->pTextureFile;
			if( _stricmp( pTextureFile, pImageFile ) == 0  )
			{
				return nIndex;
			}

			nIndex++;
		}
	}

	S_TEXTURE *pTexture = new S_TEXTURE;

	pTexture->pTextureFile = new char[ strlen(pImageFile) + 1 ];
	strcpy( pTexture->pTextureFile, pImageFile );

	if( FAILED( D3DXCreateTextureFromFileEx( m_pd3dDevice, pImageFile,
											 D3DX_DEFAULT, D3DX_DEFAULT,
											 1, 0,
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											 D3DX_DEFAULT, D3DX_DEFAULT,
											 0, NULL, NULL, &(pTexture->pd3dTexture) ) ) )
	{

		SAFE_DELETE( pTexture );

#ifdef _DEBUG
		AfxMessageBox("texture 등록 실패! 작업을 취소합니다.");
#endif
		return -1;
	}


	m_vecTexture.push_back( pTexture );

	return nIndex;
}

S_TEXTURE *CTextureManager::GetTexture(int nId)
{
	return (nId > -1) ? m_vecTexture[nId] : NULL;
}