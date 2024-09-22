// WGA2OutdoorsEditorDoc.cpp : CWGA2OutdoorsEditorDoc 클래스의 구현
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"

#include "WGA2OutdoorsEditorDoc.h"

#include ".\WGA2ode\misc.h"
#include ".\WGA2ode\D3DGraphics.h"
#include ".\WGA2ode\observer.h"

#include "DlgTerrainObject.h"
#include "dialog\CreateTerrainDlg.h"

#include "WGA2OutdoorsEditorView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWGA2OutdoorsEditorDoc

IMPLEMENT_DYNCREATE(CWGA2OutdoorsEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CWGA2OutdoorsEditorDoc, CDocument)
	ON_COMMAND(ID_CREATE_TERRAIN, OnCreateTerrain)
END_MESSAGE_MAP()


// CWGA2OutdoorsEditorDoc 생성/소멸

CWGA2OutdoorsEditorDoc::CWGA2OutdoorsEditorDoc()
{
}

CWGA2OutdoorsEditorDoc::~CWGA2OutdoorsEditorDoc()
{
}


void CWGA2OutdoorsEditorDoc::ClearGameWorld()
{
	//## view 포인터 구함
	CWGA2OutdoorsEditorView *pView = GetViewPointer();

	if( pView->m_GameWorldMng.GetRenderingSwitch() )
	{
		g_pObserver->Display( D_FPS, 0.0f, 0 );

		pView->m_GameWorldMng.SetRenderingSwitch(false);

		//## 지형 삭제
		pView->m_GameWorldMng.GetLandscapeSceneMngPtr()->DestroyTerrain();
		pView->m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainObjectMngPtr()->Destroy();

		//## 옥트리 초기화
		pView->m_GameWorldMng.Destroy();
		pView->m_GameWorldMng.InitGameWorld();

		pView->m_TerrainEditingMng.Clear();

		//## main frame 포인터를 얻고
		CMainFrame *pFr = (CMainFrame *)AfxGetMainWnd();

		//## MAINFRAME 메뉴 비활성화
		pFr->m_bTerrainObjImport = FALSE;

		//## 지형 객체 트리 제거
		((CDlgTerrainObject*)(pFr->m_wndDlgbarTabmenu.m_pdlgTerrainEditor->m_aEditor[T_OBJECT].pDialog))->Destroy();
		//## terrain editor 의 지형 편집 모드 비활성화
		pFr->m_wndDlgbarTabmenu.m_pdlgTerrainEditor->ActiveTerrainEditor(false);
	}
}

BOOL CWGA2OutdoorsEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	//## view 포인터 구함
	//CWGA2OutdoorsEditorView *pView = (CWGA2OutdoorsEditorView *)m_viewList.GetHead();

	//## 지형 vb ib 데이타 삭제
	if( GetViewPointer()!=NULL )
		ClearGameWorld();

	return TRUE;
}


// CWGA2OutdoorsEditorDoc serialization

void CWGA2OutdoorsEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
//		ar << m_pTerrainData->GetSquareCellX() << m_pTerrainData->GetSquareCellZ();
//		int nTotalVertex = ( m_pTerrainData->GetSquareVtxX() * m_pTerrainData->GetSquareVtxX() );
//		ar << nTotalVertex;
//		for(int i=0;i<nTotalVertex;i++)
//		{
//			ar << *(m_pTerrainData->GetHeightPtr()+i);
//		}
	}
	else
	{
//		int cellx,cellz,totalvertex;

//		m_pTerrainData = new CTerrainData();

//		ar >> cellx >> cellz >> totalvertex;

//		float *pfHeight = new float[totalvertex];
//
//		for (int i = 0; i < totalvertex; i++)
//		{
//			ar >> pfHeight[ i ];
//		}

//		m_pTerrainData->SetTerrainData(cellx,cellz,pfHeight);
	}
	
}


// CWGA2OutdoorsEditorDoc 진단

#ifdef _DEBUG
void CWGA2OutdoorsEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWGA2OutdoorsEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWGA2OutdoorsEditorDoc 명령


//####################################################
// 지형 생성
//####################################################
void CWGA2OutdoorsEditorDoc::OnCreateTerrain()
{
	CCreateTerrainDlg dlg;

	//## view 포인터
	CWGA2OutdoorsEditorView *pView = GetViewPointer();

	if( dlg.DoModal() == IDOK )
	{
		//## 지형 데이타 삭제
		ClearGameWorld();

		
		//## 지형 생성
		if( pView->m_GameWorldMng.GetLandscapeSceneMngPtr()->CreateTerrain(	dlg.m_nTerrainType,
														dlg.m_nMapSize,
														dlg.m_nNumPatchesPerSide,
														dlg.m_nCellSpace,
														dlg.m_strHeightMapPath,
														dlg.m_strTexturePath ) )
		{
			//## octree 초기화
			pView->m_GameWorldMng.CreateGameWorld();

			//## view의 지형 편집 관리자 초기화
			pView->m_TerrainEditingMng.Init( GetViewPointer()->m_pd3dDevice,
				pView->m_hWnd,
				pView->m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainPtr(),
				pView->m_GameWorldMng.GetOctreePtr() );

			//## view를 업데이트할 수 있게 셋팅
			((CWGA2OutdoorsEditorApp *)AfxGetApp())->SetUpdatingView(true);

			//## MAINFRAME POINTER
			CMainFrame *pFr = (CMainFrame *)AfxGetMainWnd();

			//## MAINFRAME 메뉴 활성화
			pFr->m_bTerrainObjImport = TRUE;

			//## main frame 포인터를 얻고 terrain editor 의 지형 편집 모드 활성화
			pFr->m_wndDlgbarTabmenu.m_pdlgTerrainEditor->ActiveTerrainEditor(TRUE);

			//## 렌더링 스위치 on
			pView->m_GameWorldMng.SetRenderingSwitch(true);
		}


	}

}