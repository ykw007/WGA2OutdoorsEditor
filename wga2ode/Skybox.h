#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "misc.h"

class CSkybox
{
	LPDIRECT3DDEVICE9			m_pd3dDevice;

	LPDIRECT3DTEXTURE9			m_pTexture[6];

	float						m_fSize;

	enum E_BOX_FACE
	{
		FRONT, BACK, LEFT, RIGHT, TOP, BOTTOM
	};

public:

	CSkybox(void);
	~CSkybox(void);

	void SetSize(float fSize){ m_fSize = fSize; }
	HRESULT SetTexture(char *pFileName, int nBoxFace);

	void Init(LPDIRECT3DDEVICE9 pd3dDevice){ m_pd3dDevice = pd3dDevice;	}
	void RenderOneFace( S_CUSTOMVERTEX * pVertices, SHORT sFace );
	void Render();
};

#endif