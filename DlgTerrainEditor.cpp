// DlgTerrainEditor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainEditor.h"

#include "MainFrm.h"

#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

//##################################################################
//## editor�� �߰��Ǹ� ���⿡ DIALOG CLASS ��� INCLUDE
//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "DlgTerrainModification.h"
#include "DlgTerrainTexture.h"
#include "DlgTerrainObject.h"

// CDlgTerrainEditor ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTerrainEditor, CDialog)
CDlgTerrainEditor::CDlgTerrainEditor(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgTerrainEditor::IDD, pParent)
{
}

CDlgTerrainEditor::~CDlgTerrainEditor()
{
	//## editor ����
	for( int i = 0; i < T_NUM_MODE; i++ )
		SAFE_DELETE( m_aEditor[i].pDialog );
}

void CDlgTerrainEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TERRAIN_MODIFICATION, m_ctrlBtnModification);
	DDX_Control(pDX, IDC_TERRAIN_TEXTURE, m_ctrlBtnTexture);
	DDX_Control(pDX, IDC_TERRAIN_OBJECT, m_ctrlBtnObject);
}


BEGIN_MESSAGE_MAP(CDlgTerrainEditor, CDialog)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFICATION, OnBnClickedTerrainModification)
	ON_BN_CLICKED(IDC_TERRAIN_TEXTURE, OnBnClickedTerrainTexture)
	ON_BN_CLICKED(IDC_TERRAIN_OBJECT, OnBnClickedTerrainObject)
END_MESSAGE_MAP()


// CDlgTerrainEditor �޽��� ó�����Դϴ�.

BOOL CDlgTerrainEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	//##################################################################
	//## editor�� �߰��Ǹ� ���⿡ CWGA2Button�� �߰�
	//## S_EDITOR ����ü �迭�� �߰�
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## ����� ����
	COLORREF	crBtnColor;
	crBtnColor = RGB(255, 255, 0);

	//## ��ư�� �߰��Ǹ� ���⼭ �߰��� ��ư�� ������ ������ �ش�.
	//## ����, �ؽ���, ��ü ��ư Į�� ����
	m_ctrlBtnModification.SetColor(CWGA2Button::STATE_COLOR::BG_PUSH, crBtnColor);
	m_ctrlBtnTexture.SetColor(CWGA2Button::STATE_COLOR::BG_PUSH, crBtnColor);
	m_ctrlBtnObject.SetColor(CWGA2Button::STATE_COLOR::BG_PUSH, crBtnColor);

	m_aEditor[T_HEIGHT].pDialog = new CDlgTerrainModification;
	m_aEditor[T_HEIGHT].pButton = &m_ctrlBtnModification;


	m_aEditor[T_TEXTURE].pDialog = new CDlgTerrainTexture;
	m_aEditor[T_TEXTURE].pButton = &m_ctrlBtnTexture;

	m_aEditor[T_OBJECT].pDialog = new CDlgTerrainObject;
	m_aEditor[T_OBJECT].pButton = &m_ctrlBtnObject;

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//##################################################################

	//## ���� ���� ���� ��� DIALOGBAR ���¸� �����Ѵ�.
	//## ���� ȭ��� �޽ð� ���� ��� ��ư DISABLE �� ����
	ActiveTerrainEditor( false );

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgTerrainEditor::SetActiveButton(CWGA2Button &btn, bool bActive)
{
	//## ���� ȭ��� �޽ð� ���� ��� ��ư DISABLE �� ����
	btn.SetEnabled(bActive);	//## ����� �� ���� ��ư���� ǥ�� (üũ���� ����)
	btn.EnableWindow(bActive);	//## ��ư DISABLE
}


//####################################################
// ���� ���� dlg Ȱ��ȭ�� ���� ���� �׸� ���� ����
//####################################################
void CDlgTerrainEditor::ActiveTerrainEditor( bool bActive )
{
//	m_aEditor[0].pDialog->Create( IDD_DLG_TERRAIN_MODIFY, this );

//	bActive = false;

	for ( int i = 0; i < T_NUM_MODE; i++ )
	{
		//## terrain editor�� �ʱ�ȭ�ϴ� ���̹Ƿ� �ϴ��� ��� �����.
		if( m_aEditor[i].pDialog->GetSafeHwnd() != NULL )
			m_aEditor[i].pDialog->ShowWindow( SW_HIDE );

		//## �̰͵� �� false��
		m_aEditor[i].bShow = false;

		//## �̰͸� bActive ���� �����Ѵ�.
		SetActiveButton( *(m_aEditor[i].pButton), bActive );

		//## OnInitDialog()���� ���� ������ ���Ƿ�
		//## ���� �޽� ������ editor�� �����Ѵ�.
		if( bActive )
		{
			if( m_aEditor[i].pDialog->GetSafeHwnd() == NULL )
			{
				UINT uiDlgId;

				//##################################################################
				//## editor�� �߰��Ǹ� ���⿡ DLG ID�� �߰�
				//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				switch( i )
				{
				case T_HEIGHT :
					uiDlgId = IDD_DLG_TERRAIN_MODIFY;
					break;
				case T_TEXTURE :
					uiDlgId = IDD_DLG_TERRAIN_TEXTURE;
					break;
				case T_OBJECT :
					uiDlgId = IDD_DLG_TERRAIN_OBJECT;
					break;
				}

				m_aEditor[i].pDialog->Create( uiDlgId, this );
				m_aEditor[i].pDialog->MoveWindow( 0, 40, 400, 800 );

			}
		}
	}
}

//####################################################
// ���� ���� ��� ���ý�
// ���� : �ؽ��ĸ� ���� �ߺ� �ڵ�
//####################################################
void CDlgTerrainEditor::OnBnClickedTerrainModification()
{
	Display( T_HEIGHT );

	//## ���� ������ ����
	//## �̺κ��� ���� �������� �ʱ�ȭ�ϴ� �����Լ��� ����
	//## Setup(); �̷� �Լ�
	((CDlgTerrainModification *)(m_aEditor[T_HEIGHT].pDialog))->UpdateRadius();
}

//####################################################
// ���� �ؽ��ĸ� ��� ���ý�
//####################################################
void CDlgTerrainEditor::OnBnClickedTerrainTexture()
{
	Display( T_TEXTURE );

	//## ���� ������ ����
	//## �̺κ��� ���� �������� �ʱ�ȭ�ϴ� �����Լ��� ����
	//## Setup(); �̷� �Լ�
	((CDlgTerrainTexture *)(m_aEditor[T_TEXTURE].pDialog))->UpdateRadius();
}

//####################################################
// ���� ��ü ��� ���ý�
//####################################################
void CDlgTerrainEditor::OnBnClickedTerrainObject()
{
	Display( T_OBJECT );
}

//####################################################
// editor ǥ�� �Լ�
//####################################################
void CDlgTerrainEditor::Display( BYTE byMode )
{
	CDialog *pDialog;
	bool *pbShow;
	CWGA2Button *pButton;

	//## view ������ ����
	CWGA2OutdoorsEditorView *pView = GetViewPointer();
	pView->m_TerrainEditingMng.ClearEditMode();

	//## ��� editor loop
	for(int i = 0; i < T_NUM_MODE; i++ )
	{
		pDialog = m_aEditor[i].pDialog;
		pbShow = &(m_aEditor[i].bShow);
		pButton = m_aEditor[i].pButton;

		//## Ŭ���� ����
		if( byMode == i )
		{
			//## ���̴� ���¸� ������ �ʴ� ���·� ��ȯ
			*pbShow = !(*pbShow);

			if( *pbShow )
                pView->m_TerrainEditingMng.InitEditMode( byMode );

			//## �� ���� ���¿� ���� editor �� �������� ����
			pDialog->ShowWindow( (*pbShow) ? SW_SHOW : SW_HIDE );
		}
		else
		{
			//## ������ editor�� off
			//## ��ư off
			pButton->SetChecking( 0 );

			//## ������ �ʴ� ���·�
			*pbShow = false;

			//## �����.
			if( pDialog->GetSafeHwnd() != NULL )
                pDialog->ShowWindow( SW_HIDE );
		}
	}
}
