// DlgTerrainModification.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainModification.h"
#include "terraineditingmng.h"

//## view �����͸� ��� ���� INCLUDE
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

//## combo �׸�
static char* g_strModType[] = {"Up","Down","Flat"};

// CDlgTerrainModification ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTerrainModification, CDialog)
CDlgTerrainModification::CDlgTerrainModification(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgTerrainModification::IDD, pParent)
	, m_nRadius(5)
	, m_nUpdateType(1)
	,m_nUpdateSpeed(15)
	, m_fSineHeight(1.0f)
	, m_nLimitHeight(5)
{

}

CDlgTerrainModification::~CDlgTerrainModification()
{
}

void CDlgTerrainModification::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS, m_nRadius);
	DDX_Text(pDX, IDC_EDIT3, m_nUpdateSpeed);
	DDX_Text(pDX, IDC_EDIT4, m_fSineHeight);
	DDX_Control(pDX, IDC_SLIDER_CIRCLE_RADIUS, m_ctrlSliderRadius);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlModType);
	DDX_Control(pDX, IDC_CHECK_SERIAL_UPDATE, m_ctrlCheckSerialUpdate);
	DDX_Control(pDX, IDC_STATIC_TEXT6, m_ctrlText6);
	DDX_Control(pDX, IDC_EDIT4, m_ctrlSineHeight);
	DDX_Control(pDX, IDC_SPIN1, m_ctrlSpinUpdateSpeed);
	DDX_Radio(pDX, IDC_RADIO_SYNC, m_nUpdateType);
	DDV_MinMaxInt(pDX, m_nUpdateSpeed, 1, 50);
	DDV_MinMaxFloat(pDX, m_fSineHeight, 0.0f, 2.0f);
	DDX_Control(pDX, IDC_SLIDER_HEIGHT, m_ctrlSliderLimitHeight);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nLimitHeight);
	DDV_MinMaxInt(pDX, m_nLimitHeight, 1, 30);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_ctrlEditLimitHeight);
}


BEGIN_MESSAGE_MAP(CDlgTerrainModification, CDialog)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_CIRCLE_RADIUS, OnEnChangeEditCircleRadius)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK_SERIAL_UPDATE, OnBnClickedSerialUpdate)
	ON_BN_CLICKED(IDC_RADIO_SYNC, OnBnClickedRadioSync)
	ON_BN_CLICKED(IDC_RADIO_SINE, OnBnClickedRadioSine)
	ON_EN_CHANGE(IDC_EDIT4, OnEnChangeEditSineHeight)
	ON_EN_CHANGE(IDC_EDIT3, OnEnChangeUpdateSpeed)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnEnChangeEditLimitHeight)
END_MESSAGE_MAP()


// CDlgTerrainModification �޽��� ó�����Դϴ�.

BOOL CDlgTerrainModification::OnInitDialog()
{
	CDialog::OnInitDialog();

	//## combo box �ʱ�ȭ
//	for(int i=0; i<4; i++)
//		m_ctrlModType.AddString( (LPCTSTR)g_strModType[i] );

	m_ctrlModType.SetCurSel(0);	//## ù��° �׸��� �����ش�.

	//## slider1 ���� ���� ���� ���� �� �ʱ�ȭ
	m_ctrlSliderRadius.SetRange(3,10);
	m_ctrlSliderRadius.SetPos(m_nRadius);
	UpdateRadius();

	//## slider2 ���� ���� �� �ʱ�ȭ
	m_ctrlSliderLimitHeight.SetRange(1,30);
	m_ctrlSliderLimitHeight.SetPos(m_nLimitHeight);
	UpdateLimitHeight();

	//## �������� ������Ʈ ��ư �ʱ�ȭ
	m_ctrlCheckSerialUpdate.SetCheck(true);

	//## update speed ����
	m_ctrlSpinUpdateSpeed.SetRange(1,50);

	//## ������Ʈ ���� (���ÿ� ������Ʈ�ϴ���, ���� ��� �����ϴ���)
	SetDisableRadioItem();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgTerrainModification::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_nRadius = m_ctrlSliderRadius.GetPos();
	m_nLimitHeight = m_ctrlSliderLimitHeight.GetPos();
	UpdateData(FALSE);

	UpdateRadius();
	UpdateLimitHeight();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgTerrainModification::OnEnChangeEditCircleRadius()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	if(UpdateData(TRUE))
	{
		m_ctrlSliderRadius.SetPos(m_nRadius);
		UpdateRadius();
	}
}

void CDlgTerrainModification::OnEnChangeEditLimitHeight()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CSubDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	if(UpdateData(TRUE))
	{
		m_ctrlSliderLimitHeight.SetPos(m_nLimitHeight);
		UpdateLimitHeight();
	}
}

//####################################################
// ������ ����
//####################################################
void CDlgTerrainModification::UpdateRadius()
{
	//## GetViewPointer() : view �����͸� ���� �� �ִ� �Լ�
	GetViewPointer()->m_TerrainEditingMng.GetModEditAreaPtr()->SetAreaHalfSize(m_nRadius);
}

//####################################################
// ���� ���̰� ����
//####################################################
void CDlgTerrainModification::UpdateLimitHeight()
{
	//## GetViewPointer() : view �����͸� ���� �� �ִ� �Լ�
	//## view ���Ͽ� �ۼ��Ǿ� ���� doc.h�� view.h �� include �ϸ� ������ ���� �� ����
	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetLimitHeight(m_nLimitHeight);
	GetViewPointer()->m_TerrainEditingMng.GetModEditAreaPtr()->SetHeight(m_nLimitHeight);
}

//####################################################
// combo box�� ���� update type ����
//####################################################
void CDlgTerrainModification::OnCbnSelchangeCombo1()
{
	//## ���° �׸��� ���õǾ��°�?
	UINT uiTemp = m_ctrlModType.GetCurSel();

	//## �ʿ��� �Լ��� ȣ��
	GetViewPointer()->m_TerrainEditingMng.GetModEditAreaPtr()->SetHeightAdjustment( uiTemp );
	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetHeightAdjustment( uiTemp );

	//## ���� �ø��� �׸��� �ƴϸ� 2��° slider�� disable �Ѵ�.
	m_ctrlSliderLimitHeight.EnableWindow( ( uiTemp == UP || uiTemp == DOWN ) );
	m_ctrlEditLimitHeight.EnableWindow( ( uiTemp == UP || uiTemp == DOWN ) );
}

void CDlgTerrainModification::OnBnClickedSerialUpdate()
{
	//## �������� ������Ʈ üũ
	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->ToggleSerialUpdate();
}

void CDlgTerrainModification::SetDisableRadioItem()
{
	//## ���� ���� �׸� ��� ���� �ϰ� �� ������
	bool bEnabled = (m_nUpdateType == 0) ? true : false;

	m_ctrlSineHeight.EnableWindow(!bEnabled);
	m_ctrlText6.EnableWindow(!bEnabled);
}

void CDlgTerrainModification::OnBnClickedRadioSync()
{
	m_nUpdateType = 0;
	SetDisableRadioItem();

	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetUpdateType(m_nUpdateType);
}

void CDlgTerrainModification::OnBnClickedRadioSine()
{
	m_nUpdateType = 1;
	SetDisableRadioItem();

	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetUpdateType(m_nUpdateType);
}

void CDlgTerrainModification::OnEnChangeEditSineHeight()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CSubDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	if( UpdateData(TRUE) )
		GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetSineHeight(m_fSineHeight);
}

void CDlgTerrainModification::OnEnChangeUpdateSpeed()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CSubDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	//## ���� �Է�â�� �������� �ӽ� ����� ��
	//## OnInitDialog�� ȣ��Ǳ� ���� ������ �޽����� ���� �� �Լ��� ȣ��ȴ�.
	//## OnInitDialog �Լ��� ȣ����� ������ UpdateData(FALSE)�� ȣ����� �����Ƿ�
	//## ��Ʈ�ѿ� ���� ����ְ� �ȴ�.
	//## �׷��� ���⼭ ��Ʈ�ѿ� ����ִ� ���� �˻��Ͽ�UpdateData(FALSE)�� ȣ��
	//## �̰��� ���� ���� ���̾�α� �����ÿ��� �ش�Ǵ� �����̹Ƿ�
	//## �� �ѹ� ȣ��ȴ�.
	CString str;
	GetDlgItem(IDC_EDIT3)->GetWindowText(str);
	int temp = atoi(str);
	if( temp == 0 )
		UpdateData(FALSE);

	if( UpdateData(TRUE) )
	{
		GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetUpdateSpeed(m_nUpdateSpeed);
	}
}