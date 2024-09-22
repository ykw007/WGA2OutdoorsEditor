// WGA2OutdoorsEditorView.cpp : CWGA2OutdoorsEditorView Ŭ������ ����

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




// fps ����� ���� ���� ����

DWORD g_dwFrameCnt;

float g_fTimeElapsed;

float g_fFps = 0.0f;



//####################################################

// fps ���

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

// fps ���

//####################################################

void OutputFPS(float fDeltaTime)

{

	char sDebug[256];



	// font �ʱ�ȭ

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

	//## view ������ ����

	CWGA2OutdoorsEditorView *pView = (CWGA2OutdoorsEditorView *) ((CMainFrame *)AfxGetMainWnd())->GetActiveView();



	return pView;

}





//####################################################

// LIGHT SET (�ӽ��Լ�)

//####################################################

VOID SetupLights()

{

    /// ����(material)����

    /// ������ ����̽��� �� �ϳ��� ������ �� �ִ�.

    D3DMATERIAL9 mtrl;

    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;

    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;

    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;

    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

    GetViewPointer()->m_pd3dDevice->SetMaterial( &mtrl );



    /// ���� ����

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

	//g_d3dDevice->SetRenderState( D3DRS_AMBIENT, 0xff222222 );		/// ȯ�汤��(ambient light)�� �� ����

	GetViewPointer()->m_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );		/// ȯ�汤��(ambient light)�� �� ����

}



// CWGA2OutdoorsEditorView



IMPLEMENT_DYNCREATE(CWGA2OutdoorsEditorView, CView)



BEGIN_MESSAGE_MAP(CWGA2OutdoorsEditorView, CView)

	// ǥ�� �μ� ����Դϴ�.

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



// CWGA2OutdoorsEditorView ����/�Ҹ�



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

	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��

	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;



	return CView::PreCreateWindow(cs);

}



// CWGA2OutdoorsEditorView �μ�



BOOL CWGA2OutdoorsEditorView::OnPreparePrinting(CPrintInfo* pInfo)

{

	// �⺻���� �غ�

	return DoPreparePrinting(pInfo);

}



void CWGA2OutdoorsEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)

{

	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

}



void CWGA2OutdoorsEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)

{

	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.

}





// CWGA2OutdoorsEditorView ����



#ifdef _DEBUG

void CWGA2OutdoorsEditorView::AssertValid() const

{

	CView::AssertValid();

}



void CWGA2OutdoorsEditorView::Dump(CDumpContext& dc) const

{

	CView::Dump(dc);

}



CWGA2OutdoorsEditorDoc* CWGA2OutdoorsEditorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.

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

// view �ʱ�ȭ

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

	//## game world manager �� ���� gameworld �ʱ�ȭ -> octree ����

	m_GameWorldMng.InitGameWorld(m_pd3dDevice, m_hWnd);

	//## light : �ӽ�

	SetupLights();

	//## size initial viewport

	//OnSize(NULL, rect.right, rect.bottom);



	return true;

}



//####################################################

// ���� ����⸦ ������ ������ ȣ��ȴٴ� ���� ����

//####################################################

void CWGA2OutdoorsEditorView::OnInitialUpdate()

{

	CView::OnInitialUpdate();



	//## D3D rendering device�� �������� ������ �ʱ�ȭ�Ѵ�.

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

// ���¹ٿ� ���콺 ��ǥ ���

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

// mainloop�� �Ǵ� ���̴�.

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





	//## m_pTerrain�� NULL�� �ƴϰ� ���� vb ib�� ��� ���� �Ǿ��ٸ� ������ �Լ��� ȣ��

	if( m_GameWorldMng.GetRenderingSwitch() )

	{

		//## ���콺 ��ǥ �б�

		POINT ptCursor;

		GetCursorPos( &ptCursor );

		::ScreenToClient( m_hWnd, &ptCursor );	//## client�� �������� ��ǥ ��ȯ



		//## Game world update

		m_GameWorldMng.UpdateGameWorld(fDeltaTime, ptCursor);



		//## ���� �κ�

		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 

						D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

		m_pd3dDevice->BeginScene();



		//## ������!!!!!!!!!!!!!!!!!!!

		m_GameWorldMng.RenderGameWorld();



		//## ���� ���� ������ - ���� ���� ������ ����ϱ� ���� ������ ���� �־���.

		m_TerrainEditingMng.Update(ptCursor);



		//## fps ǥ��

		//OutputFPS(fDeltaTime);



		//## ���� �κ�

		m_pd3dDevice->EndScene();

		m_pd3dDevice->Present(NULL, NULL, NULL, NULL);



		//## fps�� ���

		CalcFPS(fDeltaTime);



		//## FPS DEBUG CONTROL�� ǥ��

		//## DEBUG CONTROL ���´�.

		//## 0�� control�� fDeltaTime ���� 5�� �������� ǥ��

		//## control id ����� �ش� ��Ʈ���� �ִ� ��ȭ���� �ʱ�ȭ�� �����Ѵ�.

//		g_pObserver->Display( D_FPS, g_fFps, 5 );



		//## ���콺 ��ǥ ����ǥ���ٿ� ǥ��

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



		//## view update�� ���� �ʴ´�.

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











































