#include "StdAfx.h"
#include "occlusion.h"

COcclusion::COcclusion(void)
{
}

COcclusion::~COcclusion(void)
{
}

void COcclusion::SetupOcclusion( D3DXMATRIXA16 *pViewTransform )
{
	D3DXVECTOR3 vCamera(0.0f,0.0f,0.0f);
	
	D3DXMatrixInverse( pViewTransform, NULL, pViewTransform );

	for( int i = 0; i < 4; i++ )
		D3DXVec3TransformCoord( &m_vTP[i], &m_vP[i], pViewTransform );

	m_fMinZ = m_vTP[0].z;
	if( m_vTP[1].z < m_fMinZ )
		m_fMinZ = m_vTP[1].z;
	if( m_vTP[2].z < m_fMinZ )
		m_fMinZ = m_vTP[2].z;
	if( m_vTP[3].z < m_fMinZ )
		m_fMinZ = m_vTP[3].z;

	D3DXPlaneFromPoints( &m_FrontPlane, &m_vTP[0], &m_vTP[1], &m_vTP[2] );

	if( m_FrontPlane.d > 0.0f )
	{
		D3DXPlaneFromPoints( &m_FirstPlane, &vCamera, &m_vTP[0], &m_vTP[1] );
		D3DXPlaneFromPoints( &m_SecondPlane, &vCamera, &m_vTP[1], &m_vTP[2] );
		D3DXPlaneFromPoints( &m_ThirdPlane, &vCamera, &m_vTP[2], &m_vTP[3] );
		D3DXPlaneFromPoints( &m_FourthPlane, &vCamera, &m_vTP[3], &m_vTP[0] );
	}
	else
	{
		D3DXPlaneFromPoints( &m_FrontPlane, &m_vTP[2], &m_vTP[1], &m_vTP[0] );
		D3DXPlaneFromPoints( &m_FirstPlane, &vCamera, &m_vTP[1], &m_vTP[0] );
		D3DXPlaneFromPoints( &m_SecondPlane, &vCamera, &m_vTP[2], &m_vTP[1] );
		D3DXPlaneFromPoints( &m_ThirdPlane, &vCamera, &m_vTP[3], &m_vTP[2] );
		D3DXPlaneFromPoints( &m_FourthPlane, &vCamera, &m_vTP[0], &m_vTP[3] );
	}
}

void COcclusion::Draw(LPDIRECT3DDEVICE9 pd3dDevice)
{
	LINEVERTEX		OVertex[4];

	for(int i=0;i<4;i++)
	{
		OVertex[i].color = 0x00ff00ff;
		OVertex[i].p = m_vP[i];
	}

	short saIndices[]={
		0,1,3,2,0
	};

	DWORD dwFVF;
	pd3dDevice->GetFVF( &dwFVF );

	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	pd3dDevice->SetFVF( LINEVERTEX::FVF );
	pd3dDevice->SetTexture(0,NULL);
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_LINESTRIP,
										0,
										4,
										4,
										saIndices,
										D3DFMT_INDEX16,
										OVertex,
										sizeof( LINEVERTEX ) );

	pd3dDevice->SetFVF( dwFVF );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}
