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
	//## frustum�� �� ���� �ʱ�ȭ
	m_aVertex[0] = D3DXVECTOR3( -1, -1, 0 );
	m_aVertex[1] = D3DXVECTOR3( 1, -1, 0 );
	m_aVertex[2] = D3DXVECTOR3( 1, -1, 1 );
	m_aVertex[3] = D3DXVECTOR3( -1, -1, 1 );
	m_aVertex[4] = D3DXVECTOR3( -1, 1, 0 );
	m_aVertex[5] = D3DXVECTOR3( 1, 1, 0 );
	m_aVertex[6] = D3DXVECTOR3( 1, 1, 1 );
	m_aVertex[7] = D3DXVECTOR3( -1, 1, 1 );

	//## view * proj�� ������� ���Ѵ�.
	D3DXMATRIXA16	matInv;

	//## view*proj�� �����
	D3DXMatrixInverse(&matInv, NULL, &matViewProj );

	for( int i = 0; i < 8; i++ )
		D3DXVec3TransformCoord( &m_aVertex[i], &m_aVertex[i], &matInv );

	//## ��� ����
	D3DXPlaneFromPoints(&m_aPlane[STOP], m_aVertex+4, m_aVertex+7, m_aVertex+6);	// �� ���(top)
	D3DXPlaneFromPoints(&m_aPlane[SBOTTOM], m_aVertex  , m_aVertex+1, m_aVertex+2);	// �� ���(bottom)
	D3DXPlaneFromPoints(&m_aPlane[SNR], m_aVertex  , m_aVertex+4, m_aVertex+5);	// �� ���(near)
	D3DXPlaneFromPoints(&m_aPlane[SFR], m_aVertex+2, m_aVertex+6, m_aVertex+7);	// �� ���(far)
	D3DXPlaneFromPoints(&m_aPlane[SLEFT], m_aVertex  , m_aVertex+3, m_aVertex+7);	// �� ���(left)
	D3DXPlaneFromPoints(&m_aPlane[SRIGHT], m_aVertex+1, m_aVertex+5, m_aVertex+6);	// �� ���(right)
}

//####################################################
// ��豸 �׽�Ʈ
//####################################################
int CFrustum::TestBoundingSphere(D3DXVECTOR3 *pCenter, float fRadius)
{
	//## ���� �������� �Ÿ��� ���Ѵ�.
	float fDist[6];
	int c = 0;

	int nNumPlanes = 4;

	//## ���� �� �Ÿ� üũ
	for( int i = 0; i < nNumPlanes; i++ )
	{
		fDist[i] = D3DXPlaneDotCoord( &m_aPlane[i], pCenter );

		if( fDist[i] < -(fRadius + PLANE_EPSILON) )
			c++;
		else if( fDist[i] > (fRadius + PLANE_EPSILON) )
			return ALL_OUT;		//## �������� �ۿ� �ִ�.
	}

	//## ���� �����ϴ��� �Ϻθ� �����ϴ���
	if( c == nNumPlanes )
		return ALL_IN;

	return SOME_IN;
}


//####################################################
// �������� �׸��� : ����׿�
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

	//## �Ķ������� ��,�� ����� �׸���.
   // m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    //mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    //m_pd3dDevice->SetMaterial( &mtrl );
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, wIndex, D3DFMT_INDEX16, m_aVertex, sizeof( m_aVertex[0] ) );

	//## ������� ��,�� ����� �׸���.
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    //mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    //m_pd3dDevice->SetMaterial( &mtrl );
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, wIndex+4*3, D3DFMT_INDEX16, m_aVertex, sizeof( m_aVertex[0] ) );

	//## ���������� ��,�� ����� �׸���.
    //ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    //mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    //m_pd3dDevice->SetMaterial( &mtrl );
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, wIndex+8*3, D3DFMT_INDEX16, m_aVertex, sizeof( m_aVertex[0] ) );

	//m_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    //m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
}
