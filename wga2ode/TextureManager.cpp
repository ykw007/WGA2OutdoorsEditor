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
//## texture�� ����Ѵ�.
//######################################################################
int CTextureManager::RegisterTexture( char *pImageFile )
{
	char *pTextureFile;
	int nIndex = 0;

	//## ��ϵ� �ؽ��ĸ� ��� üũ�ؼ� ���� �ؽ����̸� id�� �Ѱ��ش�.
	//## vector�� ������� �ʴٸ� 
	if( !m_vecTexture.empty() )
	{
		//## loop �ϸ鼭 �ߺ��Ǵ� ctrlID�� �ִ��� �˻��Ѵ�.
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
		AfxMessageBox("texture ��� ����! �۾��� ����մϴ�.");
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