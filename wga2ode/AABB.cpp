#include "StdAfx.h"
#include "aabb.h"


//####################################################
// bounding box ±×¸®±â
//####################################################
void CAABB::RenderBbox(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwColor)
{
	D3DXVECTOR3		vMax, vMin;
	LINEVERTEX		BboxVertex[8];

	short saIndices[]={
		0,1,2,3,0,4,5,1,5,6,2,6,7,3,7,4
	};

	vMax.x = m_vPos.x + m_vExtent.x;
	vMax.y = m_vPos.y + m_vExtent.y;
	vMax.z = m_vPos.z - m_vExtent.z;

	vMin.x = m_vPos.x - m_vExtent.x;
	vMin.y = m_vPos.y - m_vExtent.y;
	vMin.z = m_vPos.z + m_vExtent.z;

	for( int i = 0; i < 8; i++ )
	{
		BboxVertex[i].color = dwColor;
	}

	BboxVertex[0].p = vMax;

	BboxVertex[1].p.x = vMin.x;
	BboxVertex[1].p.y = vMax.y;
	BboxVertex[1].p.z = vMax.z;

	BboxVertex[2].p.x = vMin.x;
	BboxVertex[2].p.y = vMax.y;
	BboxVertex[2].p.z = vMin.z;

	BboxVertex[3].p.x = vMax.x;
	BboxVertex[3].p.y = vMax.y;
	BboxVertex[3].p.z = vMin.z;

	BboxVertex[4].p.x = vMax.x;
	BboxVertex[4].p.y = vMin.y;
	BboxVertex[4].p.z = vMax.z;

	BboxVertex[5].p.x = vMin.x;
	BboxVertex[5].p.y = vMin.y;
	BboxVertex[5].p.z = vMax.z;

	BboxVertex[6].p = vMin;

	BboxVertex[7].p.x = vMax.x;
	BboxVertex[7].p.y = vMin.y;
	BboxVertex[7].p.z = vMin.z;

	DWORD dwFVF;
	pd3dDevice->GetFVF( &dwFVF );

	pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	pd3dDevice->SetFVF( LINEVERTEX::FVF );
	pd3dDevice->SetTexture(0,NULL);
	pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_LINESTRIP,
										0,
										8,
										15,
										saIndices,
										D3DFMT_INDEX16,
										BboxVertex,
										sizeof( LINEVERTEX ) );

	pd3dDevice->SetFVF( dwFVF );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

}