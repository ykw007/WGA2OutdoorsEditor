// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"

#include "MainFrm.h"

#include "wga2ode\model.h"
#include "DlgTerrainObject.h"

#include ".\WGA2ode\D3DGraphics.h"
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MAIN_EDITOR, OnWindowMainEditor)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_MAIN_EDITOR, OnUpdateWindowMainEditor)
	ON_COMMAND(ID_IMPORT_TERRAIN_OBJECT, OnImportTerrainObject)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_TERRAIN_OBJECT, OnUpdateImportTerrainObject)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_bTerrainObjImport = FALSE;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_FLYBY | CBRS_SIZE_DYNAMIC ) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	 
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	//## 마우스 좌표를 표시하는 팬
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 100);


	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	//## 지형 편집 에디터 다이얼로그바 생성
	if (!m_wndMainEditor.Create(this, &m_wndDlgbarTabmenu, CString("Editor"), IDD_DLGBAR_TABMENU))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}

    m_wndMainEditor.SetBarStyle(CBRS_ALIGN_RIGHT|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_wndMainEditor.EnableDocking(CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndMainEditor);
	/*
	//## 디버거 dlgbar 생성
	
	if (!m_wndObserver.Create(this, &m_wndDlgbarObserver, CString("Observer"), IDD_DLGBAR_OBSERVER))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}

    m_wndObserver.SetBarStyle(CBRS_ALIGN_BOTTOM|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_wndObserver.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockControlBar(&m_wndObserver);
*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 메시지 처리기

//####################################################
// maineditor 보이기or 감추기 메뉴
//####################################################
void CMainFrame::OnWindowMainEditor()
{
	CMenu *menu = GetMenu();

	if( menu->GetMenuState(ID_WINDOW_MAIN_EDITOR,MF_BYCOMMAND) & MF_CHECKED )
	{
		ShowControlBar(&m_wndMainEditor,FALSE,FALSE);
	}
	else
	{
		ShowControlBar(&m_wndMainEditor,TRUE,FALSE);
	}
}


//####################################################
// 창 > MainEditor 메뉴 체크 or  체크해제
//####################################################
void CMainFrame::OnUpdateWindowMainEditor(CCmdUI *pCmdUI)
{
	if( m_wndMainEditor.IsVisible() )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

//####################################################
// 지형지물 IMPORT를 위한 파일 대화 상자 열기
//####################################################
void CMainFrame::OnImportTerrainObject()
{
	//## 현재 ase 만 받는다.
	OPENFILENAME OFN;

	char lpstrFile[MAX_PATH] = "";
	char lpDir[MAX_PATH] = "";
	char lpCopyDir[MAX_PATH] = "";

	GetCurrentDirectory( MAX_PATH, lpDir );
	strcpy( lpCopyDir, lpDir );

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = "ASE Files (*.ase)\0*.ase\0모든 파일 (*.*)\0*.*\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	OFN.lpstrInitialDir = strcat( lpDir, TOBJ_DIR );

	if( GetOpenFileName( &OFN ) != 0 )
	{
		//## 메모리 삭제 해줄 것
		CModel *pModel = new CModel;

		//## 입력 받은 3d format 파일을 CModel 객체로 변환한다.
		if( !pModel->ConvertRaw3dData( lpstrFile, GetViewPointer()->m_pd3dDevice ) )
		{
			::MessageBox( NULL, "작업을 취소합니다.", "Error", MB_OK );

			SAFE_DELETE( pModel );
		}
		else
		{

			//## 변환된 모델을 지형 객체 라이브러리 tree에 담아둔다.
			//## MAINFRAME POINTER
			CMainFrame *pFr = (CMainFrame *)AfxGetMainWnd();

			//## main frame 포인터를 얻고 TERRAIN OBJECT에 항목 추가
			((CDlgTerrainObject *)pFr->m_wndDlgbarTabmenu.m_pdlgTerrainEditor->
				m_aEditor[T_OBJECT].pDialog)->InsertTerrainObject( pModel, lpstrFile );
		}
	}

	//## 현재 디렉토리를 설정한다.
	SetCurrentDirectory( lpCopyDir );
}

void CMainFrame::OnUpdateImportTerrainObject(CCmdUI *pCmdUI)
{
	//## 지형 출력된 후 지형 객체 import 메뉴 활성화
	pCmdUI->Enable( m_bTerrainObjImport );
}

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch( ((LPNMHDR)lParam)->code )
	{
	case TTN_NEEDTEXT:
		switch( wParam )
		{
		case ID_SOLID:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Solid rendering";
			break;
		case ID_WIREFRAME:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Wireframe rendering";
			break;
		case ID_POINT:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Point rendering";
			break;
		case ID_CREATE_TERRAIN:
			((LPTOOLTIPTEXT)lParam)->lpszText = "지형 생성";
			break;
		case ID_OCTREE_BBOX:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Octree 경계상자";
			break;
		case ID_TERRAIN_BBOX:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Terrain 경계상자";
			break;
		}
	}

	return CFrameWnd::OnNotify(wParam, lParam, pResult);
}
