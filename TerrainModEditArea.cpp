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
	//## 두번째 원 영역 표시 VERTEX 작성
	int nCellSpace = m_pTerrain->GetPatchBeginPtr()->GetCellSpace();
	int nNumSegs = m_nHalfSize * nCellSpace * 8;
	for( int seg = 0; seg < (nNumSegs+1); seg++ )
	{
		m_aArea[seg].p.y += m_uiHeight;

		//## 라인은 노란색으로
		m_aArea[seg].color = D3DCOLOR_XRGB(200,200,0);
	}
	//## 두번째 원 영역 표시 VERTEX 작성
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## 높이 올리기 모드일 때 높이 제한선 출력
	LINEVERTEX Line[2];

	Line[0].p = m_vMousePos;
	Line[1].p = m_vMousePos;
	Line[1].p.y = m_vMousePos.y + m_uiHeight;

	Line[0].color = D3DCOLOR_XRGB(200,200,0);
	Line[1].color = D3DCOLOR_XRGB(200,200,0);

	WORD waIndex[] = { 0, 1 };
	//## 높이 올리기 모드일 때 높이 제한선 출력
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	DWORD dwTempFVF;
	m_pd3dDevice->GetFVF(&dwTempFVF);
	m_pd3dDevice->SetFVF( LINEVERTEX::FVF );

	//###################################################################
	//## 높이 올리기 모드일 때 높이 제한선 출력
	m_pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_LINESTRIP,
										0,
										2,
										1,
										waIndex,
										D3DFMT_INDEX16,
										Line,
										sizeof( LINEVERTEX ) );

	//## 선택 영역 그리기
	m_pd3dDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, nNumSegs, m_aArea, sizeof(LINEVERTEX) );

	m_pd3dDevice->SetFVF(dwTempFVF);

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
}