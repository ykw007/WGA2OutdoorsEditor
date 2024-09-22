// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	 
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	//## ���콺 ��ǥ�� ǥ���ϴ� ��
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 100);


	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	//## ���� ���� ������ ���̾�α׹� ����
	if (!m_wndMainEditor.Create(this, &m_wndDlgbarTabmenu, CString("Editor"), IDD_DLGBAR_TABMENU))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;      // fail to create
	}

    m_wndMainEditor.SetBarStyle(CBRS_ALIGN_RIGHT|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
	m_wndMainEditor.EnableDocking(CBRS_ALIGN_RIGHT);
	DockControlBar(&m_wndMainEditor);
	/*
	//## ����� dlgbar ����
	
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}


// CMainFrame ����

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


// CMainFrame �޽��� ó����

//####################################################
// maineditor ���̱�or ���߱� �޴�
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
// â > MainEditor �޴� üũ or  üũ����
//####################################################
void CMainFrame::OnUpdateWindowMainEditor(CCmdUI *pCmdUI)
{
	if( m_wndMainEditor.IsVisible() )
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

//####################################################
// �������� IMPORT�� ���� ���� ��ȭ ���� ����
//####################################################
void CMainFrame::OnImportTerrainObject()
{
	//## ���� ase �� �޴´�.
	OPENFILENAME OFN;

	char lpstrFile[MAX_PATH] = "";
	char lpDir[MAX_PATH] = "";
	char lpCopyDir[MAX_PATH] = "";

	GetCurrentDirectory( MAX_PATH, lpDir );
	strcpy( lpCopyDir, lpDir );

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = "ASE Files (*.ase)\0*.ase\0��� ���� (*.*)\0*.*\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	OFN.lpstrInitialDir = strcat( lpDir, TOBJ_DIR );

	if( GetOpenFileName( &OFN ) != 0 )
	{
		//## �޸� ���� ���� ��
		CModel *pModel = new CModel;

		//## �Է� ���� 3d format ������ CModel ��ü�� ��ȯ�Ѵ�.
		if( !pModel->ConvertRaw3dData( lpstrFile, GetViewPointer()->m_pd3dDevice ) )
		{
			::MessageBox( NULL, "�۾��� ����մϴ�.", "Error", MB_OK );

			SAFE_DELETE( pModel );
		}
		else
		{

			//## ��ȯ�� ���� ���� ��ü ���̺귯�� tree�� ��Ƶд�.
			//## MAINFRAME POINTER
			CMainFrame *pFr = (CMainFrame *)AfxGetMainWnd();

			//## main frame �����͸� ��� TERRAIN OBJECT�� �׸� �߰�
			((CDlgTerrainObject *)pFr->m_wndDlgbarTabmenu.m_pdlgTerrainEditor->
				m_aEditor[T_OBJECT].pDialog)->InsertTerrainObject( pModel, lpstrFile );
		}
	}

	//## ���� ���丮�� �����Ѵ�.
	SetCurrentDirectory( lpCopyDir );
}

void CMainFrame::OnUpdateImportTerrainObject(CCmdUI *pCmdUI)
{
	//## ���� ��µ� �� ���� ��ü import �޴� Ȱ��ȭ
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
			((LPTOOLTIPTEXT)lParam)->lpszText = "���� ����";
			break;
		case ID_OCTREE_BBOX:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Octree ������";
			break;
		case ID_TERRAIN_BBOX:
			((LPTOOLTIPTEXT)lParam)->lpszText = "Terrain ������";
			break;
		}
	}

	return CFrameWnd::OnNotify(wParam, lParam, pResult);
}
