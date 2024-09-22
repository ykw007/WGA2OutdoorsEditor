// DlgbarTebmenu.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgbarTabmenu.h"
#include ".\WGA2ode\misc.h"


// CDlgbarTabmenu ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgbarTabmenu, CDialog)
CDlgbarTabmenu::CDlgbarTabmenu(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgbarTabmenu::IDD, pParent)
{
	m_pdlgTerrainEditor = 0;
	m_pdlgObjectEditor = 0;
}

CDlgbarTabmenu::~CDlgbarTabmenu()
{
	SAFE_DELETE(m_pdlgTerrainEditor);
	SAFE_DELETE(m_pdlgObjectEditor);
}

void CDlgbarTabmenu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DLG_TAB, m_ctrlTabMenu);
}


BEGIN_MESSAGE_MAP(CDlgbarTabmenu, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_DLG_TAB, OnTcnSelchangeDlgTab)
END_MESSAGE_MAP()


// CDlgbarTebmenu �޽��� ó�����Դϴ�.

BOOL CDlgbarTabmenu::OnInitDialog()
{
	CDialog::OnInitDialog();

	//## �� ��Ʈ�� �̹��� ����Ʈ
	m_ImageList.Create(IDB_BITMAP1, 20, 1, RGB(255,255,255));
	m_ctrlTabMenu.SetImageList(&m_ImageList);

	//## tab control �޴� ����
	CString strMenu[] = {"Terrain", "Object"};

	//m_ctrlTabMenu.DeleteAllItems();

	for( int i=0; i < 2; i++ )
	{
		m_ctrlTabMenu.InsertItem( i, strMenu[i], i );
	}

	m_pdlgTerrainEditor = new CDlgTerrainEditor;
	m_pdlgTerrainEditor->Create(IDD_DLG_TERRAIN_EDITOR, this);
	m_pdlgTerrainEditor->ShowWindow(SW_SHOW);
	m_pdlgTerrainEditor->MoveWindow(0, 40, 400, 800);

	m_pdlgObjectEditor = new CDlgObjectEditor;
	m_pdlgObjectEditor->Create(IDD_DLG_OBJECT_EDITOR, this);
	m_pdlgObjectEditor->ShowWindow(SW_HIDE);
	m_pdlgObjectEditor->MoveWindow(0, 40, 176, 800);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

int CDlgbarTabmenu::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->MoveWindow(0, 0, 250, 650);

	return 0;
}

void CDlgbarTabmenu::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_pdlgTerrainEditor)
	{
		m_pdlgTerrainEditor->DestroyWindow();
		SAFE_DELETE( m_pdlgTerrainEditor );
	}

	if(m_pdlgObjectEditor)
	{
		m_pdlgObjectEditor->DestroyWindow();
		SAFE_DELETE( m_pdlgObjectEditor );
	}
}

void CDlgbarTabmenu::OnTcnSelchangeDlgTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int num = m_ctrlTabMenu.GetCurSel();

	switch(num)
	{
	case 0:
		m_pdlgTerrainEditor->ShowWindow(SW_SHOW);
		m_pdlgObjectEditor->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pdlgTerrainEditor->ShowWindow(SW_HIDE);
		m_pdlgObjectEditor->ShowWindow(SW_SHOW);
		break;
	}

	UpdateData(FALSE);

	*pResult = 0;
}

