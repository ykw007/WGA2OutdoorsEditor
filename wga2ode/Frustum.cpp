#include "StdAfx.h"
#include "frustum.h"

CFrustum::CFrustum(void)
{
}

CFrustum::~CFrustum(void)
{
}

void CFrustum::Update(D3DXMATRIXA16 &matViewProj)
{
	//## frustum의 각 정점 초기화
	m_aVertex[0] = D3DXVECTOR3( -1, -1, 0 );
	m_aVertex[1] = D3DXVECTOR3( 1, -1, 0 );
	m_aVertex[2] = D3DXVECTOR3( 1, -1, 1 );
	m_aVertex[3] = D3DXVECTOR3( -1, -1, 1 );
	m_aVertex[4] = D3DXVECTOR3( -1, 1, 0 );
	m_aVertex[5] = D3DXVECTOR3( 1, 1, 0 );
	m_aVertex[6] = D3DXVECTOR3( 1, 1, 1 );
	m_aVertex[7] = D3DXVECTOR3( -1, 1, 1 );

	//## view * proj의 역행렬을 구한다.
	D3DXMATRIXA16	matInv;

	//## view*proj의 역행렬
	D3DXMatrixInverse(&matInv, NULL, &matViewProj );

	for( int i = 0; i < 8; i++ )
		D3DXVec3TransformCoord( &m_aVertex[i], &m_aVertex[i], &matInv );

	//## 평면 생성
	D3DXPlaneFromPoints(&m_aPlane[STOP], m_aVertex+4, m_aVertex+7, m_aVertex+6);	// 상 평면(top)
	D3DXPlaneFromPoints(&m_aPlane[SBOTTOM], m_aVertex  , m_aVertex+1, m_aVertex+2);	// 하 평면(bottom)
	D3DXPlaneFromPoints(&m_aPlane[SNR], m_aVertex  , m_aVertex+4, m_aVertex+5);	// 근 평면(near)
	D3DXPlaneFromPoints(&m_aPlane[SFR], m_aVertex+2, m_aVertex+6, m_aVertex+7);	// 원 평면(far)
	D3DXPlaneFromPoints(&m_aPlane[SLEFT], m_aVertex  , m_aVertex+3, m_aVertex+7);	// 좌 평면(left)
	D3DXPlaneFromPoints(&m_aPlane[SRIGHT], m_aVertex+1, m_aVertex+5, m_aVertex+6);	// 우 평면(right)
}

//####################################################
// 경계구 테스트
//####################################################
int CFrustum::TestBoundingSphere(D3DXVECTOR3 *pCenter, float fRadius)
{
	//## 평면과 중점과의 거리를 구한다.
	float fDist[6];
	int c = 0;

	int nNumPlanes = 4;

	//## 평면과 구 거리 체크
	for( int i = 0; i < nNumPlanes; i++ )
	{
		fDist[i] = D3DXPlaneDotCoord( &m_aPlane[i], pCenter );

		if( fDist[i] < -(fRadius + PLANE_EPSILON) )
			c++;
		else if( fDist[i] > (fRadius + PLANE_EPSILON) )
			return ALL_OUT;		//## 프러스텀 밖에 있다.
	}

	//## 전부 포함하는지 일부만 포함하는지
	if( c == nNumPlanes )
		return ALL_IN;

	return SOME_IN;
}


//####################################################
// 프러스텀 그리기 : 디버그용
//####################################################
void CFrustum::Draw(LPDIRECT3DDEVICE9 pd3dDevice)
{
	WORD		wIndex[] = { 0, 1, 2,
							0, 2, 3,
							4, 7, 6,
							4, 6, 5,
							1, 5, 6,
							1, 6, 2,
							0, 3, 7,
							0, 7, 4,
							0, 4, 5,
							0, 5, 1,
							3, 7, 6,
							3, 6, 2 };

   // D3DMATERIAL9 mtrl;
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );


	LINEVERTEX		FrustumVertex[8];

	for( int i = 0 ; i < 8 ; i++ )
		FrustumVertex[i].p = m_aVertex[i];

	pd3dDevice->SetFVF( LINEVERTEX::FVF );
	pd3dDevice->SetStreamSource( 0, NULL, 0, sizeof(LINEVERTEX) );
	pd3dDevice->SetTexture( 0, NULL );
	pd3dDevice->SetIndices( 0 );
	//m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	//m_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	//m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	//m_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	//## 파란색으로 상,하 평면을 그린다.
   // m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    //mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    //m_pd3dDevice->SetMaterial( &mtrl );
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, wIndex, D3DFMT_INDEX16, m_aVertex, sizeof( m_aVertex[0] ) );

	//## 녹색으로 좌,우 평면을 그린다.
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    //mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    //m_pd3dDevice->SetMaterial( &mtrl );
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, wIndex+4*3, D3DFMT_INDEX16, m_aVertex, sizeof( m_aVertex[0] ) );

	//## 붉은색으로 원,근 평면을 그린다.
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    //mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    //m_pd3dDevice->SetMaterial( &mtrl );
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, wIndex+8*3, D3DFMT_INDEX16, m_aVertex, sizeof( m_aVertex[0] ) );

	//m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    //m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
}
