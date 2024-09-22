// WGA2OutdoorsEditor.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "MainFrm.h"

#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

//#######################################################333
//## include
//## d3dDevice, observer
#include ".\WGA2ode\D3DGraphics.h"
#include ".\WGA2ode\observer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWGA2OutdoorsEditorApp

BEGIN_MESSAGE_MAP(CWGA2OutdoorsEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
END_MESSAGE_MAP()


// CWGA2OutdoorsEditorApp 생성

CWGA2OutdoorsEditorApp::CWGA2OutdoorsEditorApp()
{
	m_bUpdatingView = false;
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CWGA2OutdoorsEditorApp 개체입니다.

CWGA2OutdoorsEditorApp theApp;

// CWGA2OutdoorsEditorApp 초기화

BOOL CWGA2OutdoorsEditorApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControls()가 필요합니다. 
	// InitCommonControls()를 사용하지 않으면 창을 만들 수 없습니다.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.
	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	// 문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWGA2OutdoorsEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CWGA2OutdoorsEditorView));
	AddDocTemplate(pDocTemplate);
	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 명령줄에 지정된 명령을 디스패치합니다. 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	// SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.
	return TRUE;
}



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원

// 구현
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CWGA2OutdoorsEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CWGA2OutdoorsEditorApp 메시지 처리기

BOOL CWGA2OutdoorsEditorApp::OnIdle(LONG lCount)
{
    CWinApp::OnIdle(lCount);

	//## view pointer 구함
	CWGA2OutdoorsEditorView *pView = (CWGA2OutdoorsEditorView *) ((CMainFrame *)AfxGetMainWnd())->GetActiveView();

	CClientDC dc(this->GetMainWnd());
	//## view만 update
	if( m_bUpdatingView )
		pView->OnDraw(&dc);


    return TRUE;
}

void CWGA2OutdoorsEditorApp::OnFileOpen()
{
	char szFilter[] = "WGA2 Outdoors Data Files(*.wod)|*.wod|All Files(*.*)|*.*||";
	
	CFileDialog dlg( TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter );

	if( dlg.DoModal() == IDOK )
		OpenDocumentFile( dlg.GetPathName() );
}

void CWGA2OutdoorsEditorApp::OnAppExit()
{
	m_pMainWnd->SendMessage(WM_CLOSE);
}

//	static CD3DGraphics s_d3dGraphics;

BOOL CWGA2OutdoorsEditorApp::InitApplication()
{
	//####################################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## graphic d3dDevice singleton 인스턴스 생성
	//## 시스템 전체에서 단일체로 작동 - 전역변수 g_pGraphics, g_d3dDevice 사용가능
	static CObserver s_Observer;



	return CWinApp::InitApplication();
}
