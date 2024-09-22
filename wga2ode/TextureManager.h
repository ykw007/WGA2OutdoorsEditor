#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//## INCLUDES
#include "misc.h"
#include "singleton.h"
#include <vector>

using namespace std;

struct S_TEXTURE
{
	char					*pTextureFile;
	LPDIRECT3DTEXTURE9		pd3dTexture;

	S_TEXTURE()
	{
		pTextureFile = NULL;
		pd3dTexture = NULL;
	}

	~S_TEXTURE()
	{
		SAFE_DELETE_ARRAY( pTextureFile );
		SAFE_RELEASE( pd3dTexture );
	}
};

typedef vector<S_TEXTURE *>				vecTexture;
typedef vecTexture::iterator			vecTextureItor;

//####################################################
//####################################################
// texture ������
// singleton
//####################################################
//####################################################
class AFX_EXT_CLASS CTextureManager : public Singleton <CTextureManager>
{
public:
	CTextureManager(void);
	~CTextureManager(void);

	void Destroy();

	void Init(LPDIRECT3DDEVICE9 pd3dDevice);

	//## texture�� ����Ѵ�.
	int RegisterTexture( char *pImageFile );

	//## texture id�� �Է��ϸ� �ؽ��� pointer�� ��ȯ
	S_TEXTURE *GetTexture(int nId);

private:
	LPDIRECT3DDEVICE9			m_pd3dDevice;
	vecTexture					m_vecTexture;
};

#define g_pTextureMng			CTextureManager::GetSingletonPtr()

#endif