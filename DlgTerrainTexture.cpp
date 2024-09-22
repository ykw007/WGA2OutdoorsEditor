// DlgTerrainTexture.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainTexture.h"
#include "terraineditingmng.h"

//## view �����͸� ��� ���� INCLUDE
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

// CDlgTerrainTexture ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTerrainTexture, CDialog)
CDlgTerrainTexture::CDlgTerrainTexture(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgTerrainTexture::IDD, pParent)
	, m_nRadius(3)
{
}

CDlgTerrainTexture::~CDlgTerrainTexture()
{
}

void CDlgTerrainTexture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_CIRCLE_RADIUS, m_ctrlSliderRadius);
	DDX_Text(pDX, IDC_EDIT_CIRCLE_RADIUS, m_nRadius);
	DDV_MinMaxInt(pDX, m_nRadius, 1, 10);
}


BEGIN_MESSAGE_MAP(CDlgTerrainTexture, CDialog)
	ON_EN_CHANGE(IDC_EDIT_CIRCLE_RADIUS, OnEnChangeEditCircleRadius)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlgTerrainTexture �޽��� ó�����Դϴ�.

void CDlgTerrainTexture::OnEnChangeEditCircleRadius()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CSubDialog::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	if(UpdateData(TRUE))
	{
		m_ctrlSliderRadius.SetPos(m_nRadius);
		UpdateRadius();
	}
}

BOOL CDlgTerrainTexture::OnInitDialog()
{
	CSubDialog::OnInitDialog();

	//## slider1 ���� ���� ���� ���� �� �ʱ�ȭ
	m_ctrlSliderRadius.SetRange(1,10);
	m_ctrlSliderRadius.SetPos(m_nRadius);
	UpdateRadius();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//####################################################
// ������ ����
//####################################################
void CDlgTerrainTexture::UpdateRadius()
{
	//## GetViewPointer() : view �����͸� ���� �� �ִ� �Լ�
	GetViewPointer()->m_TerrainEditingMng.GetTexEditAreaPtr()->SetAreaHalfSize(m_nRadius);
}

void CDlgTerrainTexture::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_nRadius = m_ctrlSliderRadius.GetPos();
	UpdateData(FALSE);

	UpdateRadius();

	CSubDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
