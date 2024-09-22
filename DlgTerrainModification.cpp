// DlgTerrainModification.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainModification.h"
#include "terraineditingmng.h"

//## view 포인터를 얻기 위해 INCLUDE
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

//## combo 항목
static char* g_strModType[] = {"Up","Down","Flat"};

// CDlgTerrainModification 대화 상자입니다.

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


// CDlgTerrainModification 메시지 처리기입니다.

BOOL CDlgTerrainModification::OnInitDialog()
{
	CDialog::OnInitDialog();

	//## combo box 초기화
//	for(int i=0; i<4; i++)
//		m_ctrlModType.AddString( (LPCTSTR)g_strModType[i] );

	m_ctrlModType.SetCurSel(0);	//## 첫번째 항목을 보여준다.

	//## slider1 편집 영역 범위 설정 및 초기화
	m_ctrlSliderRadius.SetRange(3,10);
	m_ctrlSliderRadius.SetPos(m_nRadius);
	UpdateRadius();

	//## slider2 범위 설정 및 초기화
	m_ctrlSliderLimitHeight.SetRange(1,30);
	m_ctrlSliderLimitHeight.SetPos(m_nLimitHeight);
	UpdateLimitHeight();

	//## 연속적인 업데이트 버튼 초기화
	m_ctrlCheckSerialUpdate.SetCheck(true);

	//## update speed 범위
	m_ctrlSpinUpdateSpeed.SetRange(1,50);

	//## 업데이트 형태 (동시에 업데이트하는지, 사인 곡선을 적용하는지)
	SetDisableRadioItem();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	if(UpdateData(TRUE))
	{
		m_ctrlSliderRadius.SetPos(m_nRadius);
		UpdateRadius();
	}
}

void CDlgTerrainModification::OnEnChangeEditLimitHeight()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CSubDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	if(UpdateData(TRUE))
	{
		m_ctrlSliderLimitHeight.SetPos(m_nLimitHeight);
		UpdateLimitHeight();
	}
}

//####################################################
// 반지름 적용
//####################################################
void CDlgTerrainModification::UpdateRadius()
{
	//## GetViewPointer() : view 포인터를 얻을 수 있는 함수
	GetViewPointer()->m_TerrainEditingMng.GetModEditAreaPtr()->SetAreaHalfSize(m_nRadius);
}

//####################################################
// 제한 높이값 적용
//####################################################
void CDlgTerrainModification::UpdateLimitHeight()
{
	//## GetViewPointer() : view 포인터를 얻을 수 있는 함수
	//## view 파일에 작성되어 있음 doc.h와 view.h 만 include 하면 포인터 얻을 수 있음
	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetLimitHeight(m_nLimitHeight);
	GetViewPointer()->m_TerrainEditingMng.GetModEditAreaPtr()->SetHeight(m_nLimitHeight);
}

//####################################################
// combo box에 의한 update type 설정
//####################################################
void CDlgTerrainModification::OnCbnSelchangeCombo1()
{
	//## 몇번째 항목이 선택되었는가?
	UINT uiTemp = m_ctrlModType.GetCurSel();

	//## 필요한 함수를 호출
	GetViewPointer()->m_TerrainEditingMng.GetModEditAreaPtr()->SetHeightAdjustment( uiTemp );
	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetHeightAdjustment( uiTemp );

	//## 지형 올리기 항목이 아니면 2번째 slider를 disable 한다.
	m_ctrlSliderLimitHeight.EnableWindow( ( uiTemp == UP || uiTemp == DOWN ) );
	m_ctrlEditLimitHeight.EnableWindow( ( uiTemp == UP || uiTemp == DOWN ) );
}

void CDlgTerrainModification::OnBnClickedSerialUpdate()
{
	//## 연속적인 업데이트 체크
	GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->ToggleSerialUpdate();
}

void CDlgTerrainModification::SetDisableRadioItem()
{
	//## 라디오 관련 항목 사용 가능 하게 할 것인지
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
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CSubDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	if( UpdateData(TRUE) )
		GetViewPointer()->m_TerrainEditingMng.GetHeightWorkerPtr()->SetSineHeight(m_fSineHeight);
}

void CDlgTerrainModification::OnEnChangeUpdateSpeed()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CSubDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	//## 정수 입력창이 나옴으로 임시 방법한 것
	//## OnInitDialog가 호출되기 전에 윈도우 메시지로 인해 본 함수가 호출된다.
	//## OnInitDialog 함수가 호출되지 않으면 UpdateData(FALSE)가 호출되지 않으므로
	//## 컨트롤에 값이 비어있게 된다.
	//## 그래서 여기서 컨트롤에 들어있는 값을 검사하여UpdateData(FALSE)를 호출
	//## 이것은 거의 최초 다이얼로그 생성시에만 해당되는 사항이므로
	//## 단 한번 호출된다.
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