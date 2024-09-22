// DlgTerrainTexture.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainTexture.h"
#include "terraineditingmng.h"

//## view 포인터를 얻기 위해 INCLUDE
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

// CDlgTerrainTexture 대화 상자입니다.

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


// CDlgTerrainTexture 메시지 처리기입니다.

void CDlgTerrainTexture::OnEnChangeEditCircleRadius()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CSubDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	if(UpdateData(TRUE))
	{
		m_ctrlSliderRadius.SetPos(m_nRadius);
		UpdateRadius();
	}
}

BOOL CDlgTerrainTexture::OnInitDialog()
{
	CSubDialog::OnInitDialog();

	//## slider1 편집 영역 범위 설정 및 초기화
	m_ctrlSliderRadius.SetRange(1,10);
	m_ctrlSliderRadius.SetPos(m_nRadius);
	UpdateRadius();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//####################################################
// 반지름 적용
//####################################################
void CDlgTerrainTexture::UpdateRadius()
{
	//## GetViewPointer() : view 포인터를 얻을 수 있는 함수
	GetViewPointer()->m_TerrainEditingMng.GetTexEditAreaPtr()->SetAreaHalfSize(m_nRadius);
}

void CDlgTerrainTexture::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_nRadius = m_ctrlSliderRadius.GetPos();
	UpdateData(FALSE);

	UpdateRadius();

	CSubDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
