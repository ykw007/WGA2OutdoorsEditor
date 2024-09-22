#include "StdAfx.h"

#include "terraineditingmng.h"
#include ".\WGA2ode\d3dgraphics.h"


//## 연속적인 업데이트 체크를 위한 값
static int s_nCounter = 0;

CTerrainEditingMng::CTerrainEditingMng(void)
{
	Clear();
	m_hWnd = NULL;
	m_uiCurMode = T_NUM_MODE;
}

CTerrainEditingMng::~CTerrainEditingMng(void)
{
	Clear();
}

void CTerrainEditingMng::Clear()
{
	m_pTerrain = NULL;
	m_pOctree = NULL;

	//## 현재 수정 모드인지 알려주는 변수
	//m_bTerrainModification = false;
	//m_bTerrainTexture = false;
	m_uiCurMode = T_NUM_MODE;
	ClearEditMode();
}


//## 중복 코드가 없도록 수정할 것
void CTerrainEditingMng::Init(	LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, CTerrain *pTerrain,
								COctree *pOctree )
{
	m_pOctree = pOctree;

	if( m_hWnd == NULL )
	{
		m_hWnd = hWnd; 
		m_ModEditArea.Init( pd3dDevice, hWnd );		//## 처음 한번만 초기화
		m_TexEditArea.Init( pd3dDevice, hWnd );		//## 처음 한번만 초기화
		m_ObjEditArea.Init( pd3dDevice, hWnd );		//## 처음 한번만 초기화
	}

	if( m_pTerrain == NULL )
	{
		m_pTerrain = pTerrain;
		m_ModEditArea.SetTerrain( pTerrain );
		m_TexEditArea.SetTerrain( pTerrain );
		m_ObjEditArea.SetTerrain( pTerrain );

		m_ModEditArea.SetOctree( m_pOctree );
		m_TexEditArea.SetOctree( m_pOctree );
		m_ObjEditArea.SetOctree( m_pOctree );
	}
}

//####################################################
// 지형 수정 모드 사용을 위한 초기화
//####################################################
void CTerrainEditingMng::InitEditMode(UINT uiMode)
{
	//## 현재 모드 설정
	m_uiCurMode = uiMode;


	//## 지형 수정 모드 ON
	switch( uiMode )
	{
	case T_HEIGHT :
		m_HeightWorker.SetEditAreaPtr(&m_ModEditArea);
		m_ModEditArea.SetTargetType( LAND );
		break;

	case T_TEXTURE :
		m_TexWorker.SetEditAreaPtr(&m_TexEditArea);
		m_ModEditArea.SetTargetType( LAND );
		break;

	case T_OBJECT :
		m_ObjWorker.SetEditAreaPtr(&m_ObjEditArea);
		m_ObjEditArea.SetTargetType( LAND );
		break;
	}

}

//####################################################
// 지형 수정 모드 종료에 따른 처리
//####################################################
void CTerrainEditingMng::ClearEditMode()
{
	//## 초기 상태로 반드시 돌려준다.
	//## 그렇지 않으면 에러의 원인이 된다. 

	//## 지형 수정 모드 ON
	//## 지형 수정 모드 OFF
	switch( m_uiCurMode )
	{
	case T_HEIGHT  :
		m_ModEditArea.Destroy();
		break;

	case T_TEXTURE :
		m_TexEditArea.Destroy();
		break;

	case T_OBJECT :
		m_ObjEditArea.Destroy();
		break;
	}

	m_uiCurMode = T_NUM_MODE;
}

void CTerrainEditingMng::Update(POINT &ptCursor)
{
	switch( m_uiCurMode )
	{
	case T_HEIGHT :
		//## 높이 작업
		m_HeightWorker.Work(ptCursor);
		break;

	case T_TEXTURE :
		//## 텍스쳐링 작업
		m_TexWorker.Work(ptCursor);
		break;

	case T_OBJECT:
		m_ObjWorker.Work(ptCursor);
		break;
	}
}
