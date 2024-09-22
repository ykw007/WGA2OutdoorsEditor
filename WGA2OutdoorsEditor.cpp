// WGA2OutdoorsEditor.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
END_MESSAGE_MAP()


// CWGA2OutdoorsEditorApp ����

CWGA2OutdoorsEditorApp::CWGA2OutdoorsEditorApp()
{
	m_bUpdatingView = false;
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CWGA2OutdoorsEditorApp ��ü�Դϴ�.

CWGA2OutdoorsEditorApp theApp;

// CWGA2OutdoorsEditorApp �ʱ�ȭ

BOOL CWGA2OutdoorsEditorApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.
	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	// ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWGA2OutdoorsEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CWGA2OutdoorsEditorView));
	AddDocTemplate(pDocTemplate);
	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// ����ٿ� ������ ����� ����ġ�մϴ�. ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	// SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.
	return TRUE;
}



// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
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

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CWGA2OutdoorsEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CWGA2OutdoorsEditorApp �޽��� ó����

BOOL CWGA2OutdoorsEditorApp::OnIdle(LONG lCount)
{
    CWinApp::OnIdle(lCount);

	//## view pointer ����
	CWGA2OutdoorsEditorView *pView = (CWGA2OutdoorsEditorView *) ((CMainFrame *)AfxGetMainWnd())->GetActiveView();

	CClientDC dc(this->GetMainWnd());
	//## view�� update
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
	//## graphic d3dDevice singleton �ν��Ͻ� ����
	//## �ý��� ��ü���� ����ü�� �۵� - �������� g_pGraphics, g_d3dDevice ��밡��
	static CObserver s_Observer;



	return CWinApp::InitApplication();
}
