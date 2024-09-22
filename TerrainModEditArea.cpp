#include "StdAfx.h"
#include "terrainmodeditarea.h"

CTerrainModEditArea::CTerrainModEditArea(void)
{
	m_uiHeightAdjustment = UP;
}

CTerrainModEditArea::~CTerrainModEditArea(void)
{
}

void CTerrainModEditArea::SetHeightAdjustment(UINT uiType)
{
	m_uiHeightAdjustment = uiType;
}

void CTerrainModEditArea::Show()
{
	CTerrainEditArea::Show();

	if ( m_uiHeightAdjustment != UP )
		return ;

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## �ι�° �� ���� ǥ�� VERTEX �ۼ�
	int nCellSpace = m_pTerrain->GetPatchBeginPtr()->GetCellSpace();
	int nNumSegs = m_nHalfSize * nCellSpace * 8;
	for( int seg = 0; seg < (nNumSegs+1); seg++ )
	{
		m_aArea[seg].p.y += m_uiHeight;

		//## ������ ���������
		m_aArea[seg].color = D3DCOLOR_XRGB(200,200,0);
	}
	//## �ι�° �� ���� ǥ�� VERTEX �ۼ�
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ���� �ø��� ����� �� ���� ���Ѽ� ���
	LINEVERTEX Line[2];

	Line[0].p = m_vMousePos;
	Line[1].p = m_vMousePos;
	Line[1].p.y = m_vMousePos.y + m_uiHeight;

	Line[0].color = D3DCOLOR_XRGB(200,200,0);
	Line[1].color = D3DCOLOR_XRGB(200,200,0);

	WORD waIndex[] = { 0, 1 };
	//## ���� �ø��� ����� �� ���� ���Ѽ� ���
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD dwTempFVF;
	m_pd3dDevice->GetFVF(&dwTempFVF);
	m_pd3dDevice->SetFVF( LINEVERTEX::FVF );

	//###################################################################
	//## ���� �ø��� ����� �� ���� ���Ѽ� ���
	m_pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_LINESTRIP,
										0,
										2,
										1,
										waIndex,
										D3DFMT_INDEX16,
										Line,
										sizeof( LINEVERTEX ) );

	//## ���� ���� �׸���
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nNumSegs, m_aArea, sizeof(LINEVERTEX) );

	m_pd3dDevice->SetFVF(dwTempFVF);

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}