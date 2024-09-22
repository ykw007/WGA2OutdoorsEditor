// WGA2OutdoorsEditorView.cpp : CWGA2OutdoorsEditorView 클래스의 구현

//



// INCLUDES //////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "WGA2OutdoorsEditor.h"



#include "WGA2OutdoorsEditorDoc.h"

#include "WGA2OutdoorsEditorView.h"

#include "MainFrm.h"



#include ".\WGA2ode\D3DGraphics.h"

#include ".\WGA2ode\observer.h"

#include ".\WGA2ode\text.h"

#include <mmsystem.h>

#ifdef _DEBUG

#define new DEBUG_NEW

#endif




// fps 계산을 위한 전역 변수

DWORD g_dwFrameCnt;

float g_fTimeElapsed;

float g_fFps = 0.0f;



//####################################################

// fps 계산

//####################################################

void CalcFPS(float fDeltaTime)

{

	g_dwFrameCnt++;

	g_fTimeElapsed += fDeltaTime;



	if( g_fTimeElapsed >= 1.0f )

	{

		g_fFps = (float)g_dwFrameCnt / g_fTimeElapsed;



		g_dwFrameCnt = 0;

		g_fTimeElapsed = 0.0f;

	}

}





//####################################################

// fps 출력

//####################################################

void OutputFPS(float fDeltaTime)

{

	char sDebug[256];



	// font 초기화

	CText Text;

	Text.Init( GetViewPointer()->m_pd3dDevice,10,14);



	RECT destRect;

	SetRect( &destRect, 100, 100, 0, 0 );

	D3DXCOLOR Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);



	CalcFPS( fDeltaTime );

	sprintf(sDebug,"FPS : %f",g_fFps);



	Text.Print(sDebug,10,10,Color);

}





CWGA2OutdoorsEditorView *GetViewPointer()

{

	//## view 포인터 구함

	CWGA2OutdoorsEditorView *pView = (CWGA2OutdoorsEditorView *) ((CMainFrame *)AfxGetMainWnd())->GetActiveView();



	return pView;

}





//####################################################

// LIGHT SET (임시함수)

//####################################################

VOID SetupLights()

{

    /// 재질(material)설정

    /// 재질은 디바이스에 단 하나만 설정될 수 있다.

    D3DMATERIAL9 mtrl;

    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;

    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;

    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;

    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

    GetViewPointer()->m_pd3dDevice->SetMaterial( &mtrl );



    /// 광원 설정

    D3DLIGHT9 light;

    D3DXVECTOR3 vDir = D3DXVECTOR3( 0, 1, 1 );

    ZeroMemory( &light, sizeof(D3DLIGHT9) );

    light.Type       = D3DLIGHT_DIRECTIONAL;

    light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;

    light.Ambient.r = light.Ambient.g = light.Ambient.b = 0.0f;

    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vDir );



    GetViewPointer()->m_pd3dDevice->SetLight( 0, &light );

    GetViewPointer()->m_pd3dDevice->LightEnable( 0, TRUE );

    GetViewPointer()->m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	//g_d3dDevice->SetRenderState( D3DRS_AMBIENT, 0xff222222 );		/// 환경광원(ambient light)의 값 설정

	GetViewPointer()->m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );		/// 환경광원(ambient light)의 값 설정

}



// CWGA2OutdoorsEditorView



IMPLEMENT_DYNCREATE(CWGA2OutdoorsEditorView, CView)



BEGIN_MESSAGE_MAP(CWGA2OutdoorsEditorView, CView)

	// 표준 인쇄 명령입니다.

	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)

	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)

	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

	ON_WM_SIZE()

	ON_WM_ERASEBKGND()

	ON_COMMAND_RANGE(ID_POINT, ID_SOLID, OnFillmode)

	ON_UPDATE_COMMAND_UI_RANGE(ID_POINT, ID_SOLID, OnUpdateFillmode)

	ON_COMMAND(ID_OCTREE_BBOX, OnOctreeBbox)

	ON_UPDATE_COMMAND_UI(ID_OCTREE_BBOX, OnUpdateOctreeBbox)

	ON_COMMAND(ID_TERRAIN_BBOX, OnTerrainBbox)

	ON_UPDATE_COMMAND_UI(ID_TERRAIN_BBOX, OnUpdateTerrainBbox)

END_MESSAGE_MAP()



// CWGA2OutdoorsEditorView 생성/소멸



CWGA2OutdoorsEditorView::CWGA2OutdoorsEditorView()

{

	m_nFillmode = ID_SOLID;
	m_pd3dDevice = NULL;
}



CWGA2OutdoorsEditorView::~CWGA2OutdoorsEditorView()

{

}



BOOL CWGA2OutdoorsEditorView::PreCreateWindow(CREATESTRUCT& cs)

{

	// TODO: CREATESTRUCT cs를 수정하여 여기에서

	// Window 클래스 또는 스타일을 수정합니다.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;



	return CView::PreCreateWindow(cs);

}



// CWGA2OutdoorsEditorView 인쇄



BOOL CWGA2OutdoorsEditorView::OnPreparePrinting(CPrintInfo* pInfo)

{

	// 기본적인 준비

	return DoPreparePrinting(pInfo);

}



void CWGA2OutdoorsEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)

{

	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.

}



void CWGA2OutdoorsEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)

{

	// TODO: 인쇄 후 정리 작업을 추가합니다.

}





// CWGA2OutdoorsEditorView 진단



#ifdef _DEBUG

void CWGA2OutdoorsEditorView::AssertValid() const

{

	CView::AssertValid();

}



void CWGA2OutdoorsEditorView::Dump(CDumpContext& dc) const

{

	CView::Dump(dc);

}



CWGA2OutdoorsEditorDoc* CWGA2OutdoorsEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.

{

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWGA2OutdoorsEditorDoc)));

	return (CWGA2OutdoorsEditorDoc*)m_pDocument;

}

#endif //_DEBUG





void CWGA2OutdoorsEditorView::OnExit()

{

	CWGA2OutdoorsEditorApp *App = (CWGA2OutdoorsEditorApp*)AfxGetApp();

	App->OnAppExit();

}





//static CD3DGraphics* pC;



//####################################################

// view 초기화

//####################################################

bool CWGA2OutdoorsEditorView::Init()

{

	// get our main window rectangle

	CRect rect;

	GetClientRect(&rect);


	CD3DGraphics* pGraphics = new CD3DGraphics;

	if ( FAILED( pGraphics->CreateDevice(GetSafeHwnd(), rect.right, rect.bottom) ) )
	{

		// post some kind of error msg

		return false;

	}



	//g_d3dDevice = pGraphics->GetDevice();

	m_pd3dDevice = pGraphics->m_pd3dDevice;

	//## game world manager 에 의한 gameworld 초기화 -> octree 적용

	m_GameWorldMng.InitGameWorld(m_pd3dDevice, m_hWnd);

	//## light : 임시

	SetupLights();

	//## size initial viewport

	//OnSize(NULL, rect.right, rect.bottom);



	return true;

}



//####################################################

// 새로 만들기를 선택할 때마다 호출된다는 것을 주의

//####################################################

void CWGA2OutdoorsEditorView::OnInitialUpdate()

{

	CView::OnInitialUpdate();



	//## D3D rendering device가 존재하지 않으면 초기화한다.

	if( m_pd3dDevice == NULL )

		Init();

}



void CWGA2OutdoorsEditorView::OnSize(UINT nType, int cx, int cy)

{

	CView::OnSize(nType, cx, cy);



	if( m_pd3dDevice == NULL )

		return;



    D3DXMATRIXA16 matProj;



	float fAspectRatio = (float)800/(float)600;



    float fFov = M_PI / 4.0f; // 2 over pi or 90 degrees

   

    D3DXMatrixPerspectiveFovLH(&matProj, fFov, fAspectRatio, 1.0f, 12000.0f);



    if (m_pd3dDevice)

	{

         m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		 m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		 m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	}

}



BOOL CWGA2OutdoorsEditorView::OnEraseBkgnd(CDC* pDC)

{

	return 0; 

}





//####################################################

// 상태바에 마우스 좌표 출력

//####################################################

void CWGA2OutdoorsEditorView::OutputMouseCoordInStatusBar(POINT &ptCursor)

{

	CString strText;

	strText.Format( "X : %d, Y : %d", ptCursor.x, ptCursor.y );



	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	pFrame->m_wndStatusBar.SetPaneText(1,strText);

}



//####################################################

// OnDraw

// mainloop가 되는 곳이다.

//####################################################

void CWGA2OutdoorsEditorView::OnDraw(CDC* pDC)

{

	//## frame rate

	static float fLastTime = (float)timeGetTime();

	float fCurrTime = (float)timeGetTime();

	float fDeltaTime = (fCurrTime - fLastTime) * 0.001f;

	fLastTime = fCurrTime;



	CWGA2OutdoorsEditorDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);





	//## m_pTerrain이 NULL이 아니고 지형 vb ib가 모두 셋팅 되었다면 렌더링 함수를 호출

	if( m_GameWorldMng.GetRenderingSwitch() )

	{

		//## 마우스 좌표 읽기

		POINT ptCursor;

		GetCursorPos( &ptCursor );

		::ScreenToClient( m_hWnd, &ptCursor );	//## client를 기준으로 좌표 변환



		//## Game world update

		m_GameWorldMng.UpdateGameWorld(fDeltaTime, ptCursor);



		//## 공통 부분

		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 

						D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

		m_pd3dDevice->BeginScene();



		//## 렌더링!!!!!!!!!!!!!!!!!!!

		m_GameWorldMng.RenderGameWorld();



		//## 지형 편집 관리자 - 선택 영역 라인을 출력하기 위해 렌더링 블럭에 넣었다.

		m_TerrainEditingMng.Update(ptCursor);



		//## fps 표시

		//OutputFPS(fDeltaTime);



		//## 공통 부분

		m_pd3dDevice->EndScene();

		m_pd3dDevice->Present(NULL, NULL, NULL, NULL);



		//## fps를 계산

		CalcFPS(fDeltaTime);



		//## FPS DEBUG CONTROL에 표시

		//## DEBUG CONTROL 얻어온다.

		//## 0번 control에 fDeltaTime 값을 5초 간격으로 표시

		//## control id 등록은 해당 컨트롤이 있는 대화상자 초기화시 연결한다.

//		g_pObserver->Display( D_FPS, g_fFps, 5 );



		//## 마우스 좌표 상태표시줄에 표시

		OutputMouseCoordInStatusBar(ptCursor);

	}

	else

	{

		CBrush BackGroundBrush( RGB(0, 0, 0) );

		CBrush* pOldBrush = pDC->SelectObject(&BackGroundBrush);



		CRect rect;

		pDC->GetClipBox(&rect);



		pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

		pDC->SelectObject(pOldBrush);



		//## view update를 하지 않는다.

		((CWGA2OutdoorsEditorApp *)AfxGetApp())->SetUpdatingView(false);

	}

}



void CWGA2OutdoorsEditorView::OnFillmode(UINT nID)

{

	m_nFillmode = nID;

	m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, nID-137 );

}



void CWGA2OutdoorsEditorView::OnUpdateFillmode(CCmdUI *pCmdUI)

{

	pCmdUI->SetRadio(m_nFillmode == pCmdUI->m_nID);

}







LRESULT CWGA2OutdoorsEditorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)

{

	int nEditMode = m_TerrainEditingMng.GetCurEditMode();



	switch( message )

	{



	case WM_RBUTTONDOWN:

		if( m_GameWorldMng.GetCameraMngPtr()->GetCameraPtr() != NULL )

			m_GameWorldMng.GetCameraMngPtr()->SetMouseControl(true);

		break;



	case WM_RBUTTONUP:

		if( m_GameWorldMng.GetCameraMngPtr()->GetCameraPtr() != NULL )

			m_GameWorldMng.GetCameraMngPtr()->SetMouseControl(false);

		break;



	case WM_LBUTTONDOWN:

		switch( nEditMode )

		{

		case T_OBJECT:

			m_TerrainEditingMng.GetObjectWorkerPtr()->WorkThis();

			break;

		case T_HEIGHT:

			m_TerrainEditingMng.GetHeightWorkerPtr()->SetUpdate( true );

			break;

		}

		break;



	case WM_LBUTTONUP:

		if( nEditMode == T_HEIGHT )

		{

			m_TerrainEditingMng.GetHeightWorkerPtr()->SetUpdate( false );

		}

		break;



	case WM_MOUSEMOVE:

		if( nEditMode == T_OBJECT )

		{

			m_TerrainEditingMng.GetObjectWorkerPtr()->Update();

		}

		break;

	}



	return CView::WindowProc(message, wParam, lParam);

}



void CWGA2OutdoorsEditorView::OnOctreeBbox()

{

	m_GameWorldMng.ToggleOctreeBboxRenderSwitch();

}



void CWGA2OutdoorsEditorView::OnUpdateOctreeBbox(CCmdUI *pCmdUI)

{

	if( !m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainPtr() )

		pCmdUI->Enable( FALSE );

	else

		pCmdUI->SetCheck(m_GameWorldMng.GetOctreeBboxRenderSwitch());

}



void CWGA2OutdoorsEditorView::OnTerrainBbox()

{

	m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainPtr()->TogglePatchBboxRenderSwitch();

}



void CWGA2OutdoorsEditorView::OnUpdateTerrainBbox(CCmdUI *pCmdUI)

{

	if( !m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainPtr() )

		pCmdUI->Enable( FALSE );

	else

        pCmdUI->SetCheck(m_GameWorldMng.GetLandscapeSceneMngPtr()->GetTerrainPtr()->GetPatchBboxRenderSwitch());

}











































