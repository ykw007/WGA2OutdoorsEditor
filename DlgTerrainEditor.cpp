// DlgTerrainEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainEditor.h"

#include "MainFrm.h"

#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

//##################################################################
//## editor가 추가되면 여기에 DIALOG CLASS 헤더 INCLUDE
//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include "DlgTerrainModification.h"
#include "DlgTerrainTexture.h"
#include "DlgTerrainObject.h"

// CDlgTerrainEditor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTerrainEditor, CDialog)
CDlgTerrainEditor::CDlgTerrainEditor(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgTerrainEditor::IDD, pParent)
{
}

CDlgTerrainEditor::~CDlgTerrainEditor()
{
	//## editor 삭제
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


// CDlgTerrainEditor 메시지 처리기입니다.

BOOL CDlgTerrainEditor::OnInitDialog()
{
	CDialog::OnInitDialog();

	//##################################################################
	//## editor가 추가되면 여기에 CWGA2Button을 추가
	//## S_EDITOR 구조체 배열에 추가
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## 노란색 바탕
	COLORREF	crBtnColor;
	crBtnColor = RGB(255, 255, 0);

	//## 버튼이 추가되면 여기서 추가된 버튼의 배경색을 설정해 준다.
	//## 지형, 텍스쳐, 객체 버튼 칼라 설정
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

	//## 최초 지형 편집 모드 DIALOGBAR 상태를 설정한다.
	//## 최초 화면상 메시가 없을 경우 버튼 DISABLE 로 지정
	ActiveTerrainEditor( false );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgTerrainEditor::SetActiveButton(CWGA2Button &btn, bool bActive)
{
	//## 최초 화면상 메시가 없을 경우 버튼 DISABLE 로 지정
	btn.SetEnabled(bActive);	//## 사용할 수 없는 버튼으로 표시 (체크상태 해제)
	btn.EnableWindow(bActive);	//## 버튼 DISABLE
}


//####################################################
// 지형 편집 dlg 활성화에 따른 하위 항목 상태 설정
//####################################################
void CDlgTerrainEditor::ActiveTerrainEditor( bool bActive )
{
//	m_aEditor[0].pDialog->Create( IDD_DLG_TERRAIN_MODIFY, this );

//	bActive = false;

	for ( int i = 0; i < T_NUM_MODE; i++ )
	{
		//## terrain editor를 초기화하는 것이므로 일단은 모두 숨긴다.
		if( m_aEditor[i].pDialog->GetSafeHwnd() != NULL )
			m_aEditor[i].pDialog->ShowWindow( SW_HIDE );

		//## 이것도 다 false로
		m_aEditor[i].bShow = false;

		//## 이것만 bActive 값을 적용한다.
		SetActiveButton( *(m_aEditor[i].pButton), bActive );

		//## OnInitDialog()에서 생성 에러가 나므로
		//## 최초 메시 생성시 editor를 생성한다.
		if( bActive )
		{
			if( m_aEditor[i].pDialog->GetSafeHwnd() == NULL )
			{
				UINT uiDlgId;

				//##################################################################
				//## editor가 추가되면 여기에 DLG ID를 추가
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
// 지형 수정 모드 선택시
// 수정 : 텍스쳐링 모드와 중복 코드
//####################################################
void CDlgTerrainEditor::OnBnClickedTerrainModification()
{
	Display( T_HEIGHT );

	//## 영역 반지름 설정
	//## 이부분을 이전 설정으로 초기화하는 공통함수로 연결
	//## Setup(); 이런 함수
	((CDlgTerrainModification *)(m_aEditor[T_HEIGHT].pDialog))->UpdateRadius();
}

//####################################################
// 지형 텍스쳐링 모드 선택시
//####################################################
void CDlgTerrainEditor::OnBnClickedTerrainTexture()
{
	Display( T_TEXTURE );

	//## 영역 반지름 설정
	//## 이부분을 이전 설정으로 초기화하는 공통함수로 연결
	//## Setup(); 이런 함수
	((CDlgTerrainTexture *)(m_aEditor[T_TEXTURE].pDialog))->UpdateRadius();
}

//####################################################
// 지형 객체 모드 선택시
//####################################################
void CDlgTerrainEditor::OnBnClickedTerrainObject()
{
	Display( T_OBJECT );
}

//####################################################
// editor 표시 함수
//####################################################
void CDlgTerrainEditor::Display( BYTE byMode )
{
	CDialog *pDialog;
	bool *pbShow;
	CWGA2Button *pButton;

	//## view 포인터 구함
	CWGA2OutdoorsEditorView *pView = GetViewPointer();
	pView->m_TerrainEditingMng.ClearEditMode();

	//## 모든 editor loop
	for(int i = 0; i < T_NUM_MODE; i++ )
	{
		pDialog = m_aEditor[i].pDialog;
		pbShow = &(m_aEditor[i].bShow);
		pButton = m_aEditor[i].pButton;

		//## 클릭한 모드는
		if( byMode == i )
		{
			//## 보이는 상태면 보이지 않는 상태로 전환
			*pbShow = !(*pbShow);

			if( *pbShow )
                pView->m_TerrainEditingMng.InitEditMode( byMode );

			//## 위 변수 상태에 따라 editor 를 보여줄지 결정
			pDialog->ShowWindow( (*pbShow) ? SW_SHOW : SW_HIDE );
		}
		else
		{
			//## 나머지 editor는 off
			//## 버튼 off
			pButton->SetChecking( 0 );

			//## 보이지 않는 상태로
			*pbShow = false;

			//## 숨긴다.
			if( pDialog->GetSafeHwnd() != NULL )
                pDialog->ShowWindow( SW_HIDE );
		}
	}
}
