#include "StdAfx.h"
#include "terraineditarea.h"
#include "wga2ode\terrain.h"

//#######################################################################################
//#######################################################################################
//#######################################################################################
//## CTerrainEditArea ������

CTerrainEditArea::CTerrainEditArea(void)
{
	m_pTerrain = NULL;

}

CTerrainEditArea::~CTerrainEditArea(void)
{
	Destroy();
}

void CTerrainEditArea::Clear()
{
	m_bFixed = false;
	m_nMapPointCounter = 0;
	m_nHalfSize = 3;
}

void CTerrainEditArea::Destroy()
{
	Clear();
}


bool CTerrainEditArea::Update(POINT &ptCursor)
{
	if( m_pTerrain == NULL )
		return false;

	//## ���콺�� �����Ǵ� ���� ã�´�.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	FindFacePicked( ptCursor, m_nTargetType );

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ���콺�� �����Ǵ� ���� ã�´�.


	//## pick �Ǿ����� ���� �ڵ��
	if( !m_bIsPicked)
		return false;

	if( m_nTargetType == OBSTACLE )
        return true;

	//## ���� ���
	m_bFixed = false;
	D3DXMATRIXA16 matView;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView);
	if( ( m_ptPrev.x == ptCursor.x && m_ptPrev.y == ptCursor.y ) &&
		m_matPrevView == matView )
	{
		m_bFixed = true;
		return true;
	}

	//## map point ī���͸� �ʱ�ȭ��Ų��.
	m_nMapPointCounter = 0;

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ������ �������� ���� S_CUSTOMVERTEX TYPE

	int nRealMapSize = m_pTerrain->GetEdgeSpace();
	int nHeightMapSize = m_pTerrain->GetPatchBeginPtr()->GetHeightmapPtr()->GetMapSize();

	D3DXVECTOR3 vMousePos = m_vMousePos;

	vMousePos = (m_vMousePos*nHeightMapSize) / nRealMapSize;

	//## loop ������, ������ ����
	int nBeginX, nEndX;
	int nBeginZ, nEndZ;
	nBeginX = vMousePos.x - m_nHalfSize;
	nEndX = vMousePos.x + m_nHalfSize;
	nBeginZ = vMousePos.z + m_nHalfSize;
	nEndZ = vMousePos.z - m_nHalfSize;


	if( nBeginX < 0 ) nBeginX = 0;
	if( nEndX > nHeightMapSize ) nEndX = nHeightMapSize;
	if( nBeginZ > 0 ) nBeginZ = 0;
	if( nEndZ < -nHeightMapSize ) nEndZ = -nHeightMapSize;

	//m_nNumPointPerSide = (nEndX - nBeginX) + 1;

	//## �Ÿ� ����
	float fDistPow;
	int x, z, nIndex;
	for( z = -nBeginZ; z <= -nEndZ; z++ )
	{
		for( x = nBeginX; x <= nEndX; x++ )
		{
			//##  ������ ���� ��� ���� ������ ����
			fDistPow =	(x - vMousePos.x)*(x - vMousePos.x)+
						(-z - vMousePos.z)*(-z - vMousePos.z);

			//## ���� ���� ũ���� ������ �� ( ���ȿ� ��� �ִ��� üũ)
			if( fDistPow < m_nHalfSize*m_nHalfSize )
			{
				nIndex = x+z*(nHeightMapSize+1);

				m_aSelected[m_nMapPointCounter].pMapPoint =
					((m_pTerrain->m_spMapPatch)+nIndex);
				m_aSelected[m_nMapPointCounter].fDistPow = fDistPow;

				m_nMapPointCounter++;
			}
		}
	}

	//## ������ �������� ���� S_CUSTOMVERTEX TYPE
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## ����� ����� �д�.
	m_ptPrev = ptCursor;
	m_vPrevMousePos = m_vMousePos;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &m_matPrevView);

	return true;
}

void CTerrainEditArea::Show()
{
	if(!m_bIsPicked)
		return;

	int nCellSpace = m_pTerrain->GetPatchBeginPtr()->GetCellSpace();

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## �� ���� ǥ�� VERTEX �ۼ�
	int nNumSegs = m_nHalfSize * nCellSpace * 8;

	float fSegAngle = ( 2.0f * M_PI / nNumSegs );

	float fMapSize = (float)(m_pTerrain->GetEdgeSpace());

	float x1, z1, x, z;

	for( int seg = 0; seg < (nNumSegs+1); seg++ )
	{
		x1 =  (float)sin( seg * fSegAngle );
		z1 =  (float)cos( seg * fSegAngle );

		if( !m_bFixed )
		{
			x = ( m_nHalfSize * x1 * nCellSpace ) + m_vMousePos.x;
			z = ( m_nHalfSize * z1 *nCellSpace ) + m_vMousePos.z;
		}
		else
		{
			x = ( m_nHalfSize * x1 * nCellSpace ) + m_vPrevMousePos.x;
			z = ( m_nHalfSize * z1 *nCellSpace ) + m_vPrevMousePos.z;
		}

		m_aArea[seg].p.x = x;
		m_aArea[seg].p.z = z;

		//## ���� �� ���� ���� ����
		m_aArea[seg].p.y = ( fabs(x) > fMapSize || fabs(z) > fMapSize )
								? AREA_BASE_HEIGHT : m_pTerrain->GetHeight( x, z ) + AREA_BASE_HEIGHT;

		//## ������ ���������
		m_aArea[seg].color = D3DCOLOR_XRGB(200,200,0);
	}
	//## �� ���� ǥ�� VERTEX �ۼ�
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD dwTempFVF;
	m_pd3dDevice->GetFVF(&dwTempFVF);

	//## ������ ȯ�� ����
	m_pd3dDevice->SetFVF( LINEVERTEX::FVF );
	m_pd3dDevice->SetTexture(0,NULL);


	//## ���� ���� �׸���
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nNumSegs, m_aArea, sizeof(LINEVERTEX) );

	m_pd3dDevice->SetFVF(dwTempFVF);

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}