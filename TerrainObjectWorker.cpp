#include "StdAfx.h"
#include "terrainobjectworker.h"
#include "wga2ode\d3dgraphics.h"
#include "wga2ode\octree.h"
#include "wga2ode\modelmanager.h"
#include "wga2ode\aabb.h"

//## view 포인터를 얻기 위해 INCLUDE
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

CTerrainObjectWorker::CTerrainObjectWorker(void)
{
	m_pTerrainObj = NULL;
}

CTerrainObjectWorker::~CTerrainObjectWorker(void)
{
}

void CTerrainObjectWorker::Update()
{
	//if( m_sCurMode == DEL_OBJ &&
	//	m_pEditArea->IsPicked() &&
	//	m_pEditArea->GetIntersectionPtr()->pMeshObj != NULL )
	//	m_pEditArea->GetIntersectionPtr()->pMeshObj->GetAABB().RenderBbox( g_d3dDevice, 0xffffffff );
}


//####################################################################
// 작업함수
// 이 함수를 호출하는 곳은 view에 있음
//####################################################################
void CTerrainObjectWorker::WorkThis()
{
	switch( m_sCurMode )
	{
	case ADD_OBJ:
		AddObject();
		break;
	case DEL_OBJ:
		DelObject();
		break;
	}
}

void CTerrainObjectWorker::AddObject()
{
	//## view 포인터
	CWGA2OutdoorsEditorView *pView = GetViewPointer();

	D3DXVECTOR3 vPos = m_pEditArea->GetMousePos();

	if( m_pTerrainObj != NULL )
	{
		//## OCTREE PICKER 로 PICKING된 지점의 OCTREE NODE를 구한다.
		S_OCT_NODE *pCurOctNode = pView->m_TerrainEditingMng.GetObjEditAreaPtr()->GetCurOctNodePtr();

		COctree *pCurOctree = pView->m_TerrainEditingMng.GetObjEditAreaPtr()->GetCurOctTreePtr();

		//## 모델 관리자에 모델을 등록
		S_DRAW_MODEL *pDrawModel = pView->m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainObjectMngPtr()->
														AddModel( m_pTerrainObj, vPos );

		//## OCTREE NODE 에 OBJECT로 등록한다.
		pCurOctree->AddObjInNode( pCurOctNode, pDrawModel, OBSTACLE );
	}
}

void CTerrainObjectWorker::DelObject()
{
	if( m_pEditArea->IsPicked() )
        AfxMessageBox("삭제");
}