#include "StdAfx.h"
#include "skybox.h"

CSkybox::CSkybox(void)
{
	for( int i = 0; i < 6; i++ )
        m_pTexture[i] = NULL;

	m_fSize = 10.0f;
}

CSkybox::~CSkybox(void)
{
	for( int i = 0; i < 6; i++ )
        SAFE_RELEASE( m_pTexture[i] );
}

HRESULT CSkybox::SetTexture(char *pFileName, int nBoxFace)
{
	//## 파일이 있는지 체크
	FILE *fp;

	char str[255];

    if( ( fp = fopen(pFileName,"r") ) == NULL )
	{
		strcpy(str,".");
		strcat(str,pFileName);
	}
	else
        strcpy(str,pFileName);


    if( FAILED( D3DXCreateTextureFromFileEx(	m_pd3dDevice, str,
												D3DX_DEFAULT, D3DX_DEFAULT,
												1, 0,
												D3DFMT_UNKNOWN,	D3DPOOL_MANAGED,
												D3DX_DEFAULT, D3DX_DEFAULT,
												0, NULL, NULL, &m_pTexture[nBoxFace] ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

void CSkybox::Render()
{
	D3DXMATRIXA16	matSkyBoxView, matView;

	S_CUSTOMVERTEX vSkyBox[4];

	//## 항상 카메라와 함께 움직인다.
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matSkyBoxView );
	matView = matSkyBoxView;
	matSkyBoxView._41 = matSkyBoxView._42 = matSkyBoxView._43 = 0.0f;

	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matSkyBoxView );

	D3DSURFACE_DESC desc;
	m_pTexture[FRONT]->GetLevelDesc(0, &desc);

	float f = 0.5f/(float)(desc.Width);

	m_pd3dDevice->SetFVF( FVF );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	vSkyBox[0].fTu = 0.0f + f;
	vSkyBox[0].fTv = 0.0f + f;
	vSkyBox[1].fTu = 0.0f + f;
	vSkyBox[1].fTv = 1.0f - f;
	vSkyBox[2].fTu = 1.0f - f;
	vSkyBox[2].fTv = 0.0f + f;
	vSkyBox[3].fTu = 1.0f - f;
	vSkyBox[3].fTv = 1.0f - f;

	f = m_fSize * 30.0f;


	D3DXVECTOR3 vP[8];
	vP[0] = D3DXVECTOR3( -f,  f,  f );
	vP[1] = D3DXVECTOR3(  f,  f,  f );
	vP[2] = D3DXVECTOR3( -f - 3000.0f , -f,  f + 3000.0f );
	vP[3] = D3DXVECTOR3(  f + 3000.0f , -f,  f + 3000.0f );
	vP[4] = D3DXVECTOR3( -f,  f, -f );
	vP[5] = D3DXVECTOR3(  f,  f, -f );
	vP[6] = D3DXVECTOR3( -f - 3000.0f , -f, -f - 3000.0f );
	vP[7] = D3DXVECTOR3(  f + 3000.0f , -f, -f - 3000.0f );

	//## front
	vSkyBox[0].vP = vP[1];
	vSkyBox[1].vP = vP[3];
	vSkyBox[2].vP = vP[0];
	vSkyBox[3].vP = vP[2];

	RenderOneFace( vSkyBox, FRONT );

	//## back
	vSkyBox[0].vP = vP[4];
	vSkyBox[1].vP = vP[6];
	vSkyBox[2].vP = vP[5];
	vSkyBox[3].vP = vP[7];

	RenderOneFace( vSkyBox, BACK );

	//## left
	vSkyBox[0].vP = vP[0];
	vSkyBox[1].vP = vP[2];
	vSkyBox[2].vP = vP[4];
	vSkyBox[3].vP = vP[6];

	RenderOneFace( vSkyBox, LEFT );

	//## right
	vSkyBox[0].vP = vP[5];
	vSkyBox[1].vP = vP[7];
	vSkyBox[2].vP = vP[1];
	vSkyBox[3].vP = vP[3];

	RenderOneFace( vSkyBox, RIGHT );

	//## top
	vSkyBox[0].vP = vP[0];
	vSkyBox[1].vP = vP[4];
	vSkyBox[2].vP = vP[1];
	vSkyBox[3].vP = vP[5];

	RenderOneFace( vSkyBox, TOP );

	//## bottom
	vSkyBox[0].vP = vP[7];
	vSkyBox[1].vP = vP[6];
	vSkyBox[2].vP = vP[3];
	vSkyBox[3].vP = vP[2];

	RenderOneFace( vSkyBox, BOTTOM );

	m_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}

void CSkybox::RenderOneFace( S_CUSTOMVERTEX * pVertices, SHORT sFace )
{
	m_pd3dDevice->SetTexture( 0, m_pTexture[sFace] );

	m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, pVertices, sizeof(S_CUSTOMVERTEX) );
}