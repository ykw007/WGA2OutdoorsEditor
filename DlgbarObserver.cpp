// DlgbarObserver.cpp : 구현 파일입니다.
//

#include "stdafx.h"


#include "WGA2OutdoorsEditor.h"
#include "DlgbarObserver.h"

//## observer 
#include ".\WGA2ode\observer.h"


// CDlgbarObserver 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgbarObserver, CDialog)
CDlgbarObserver::CDlgbarObserver(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgbarObserver::IDD, pParent)
{
}

CDlgbarObserver::~CDlgbarObserver()
{
}

void CDlgbarObserver::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlEditRecord);
	DDX_Control(pDX, IDC_FPS, m_StcFps);
	DDX_Control(pDX, IDC_NUM_TERRAIN_FACE, m_stcNumTerrainFaces);
	DDX_Control(pDX, IDC_NUM_TOBSTACLES_FACE, m_stcNumTerrainObstacleFaces);
}


BEGIN_MESSAGE_MAP(CDlgbarObserver, CDialog)
END_MESSAGE_MAP()


// CDlgbarObserver 메시지 처리기입니다.

BOOL CDlgbarObserver::OnInitDialog()
{
	CSubDialog::OnInitDialog();

	//## observer에서 사용할 컨트롤을 등록한다.
	//## 0번
//	if( g_pObserver->RegisterControl( D_FPS, m_StcFps.m_hWnd ) )
  //      g_pObserver->Display( D_FPS, 0.0f, 0 );

//	g_pObserver->RegisterControl( 1, m_ctrlEditRecord.m_hWnd );

//	g_pObserver->RegisterControl( 2, m_stcNumTerrainFaces.m_hWnd );

//	g_pObserver->RegisterControl( 3, m_stcNumTerrainObstacleFaces.m_hWnd );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
