#include "StdAfx.h"

// INCLUDES //////////////////////////////////////////////////////////////
#include "d3dgraphics.h"

// FUNCTIONS /////////////////////////////////////////////////////////////


CD3DGraphics::CD3DGraphics(void)
{
	m_pD3D			= NULL;
	m_pd3dDevice	= NULL;
}

CD3DGraphics::~CD3DGraphics(void)
{
	SAFE_RELEASE(m_pd3dDevice)
	SAFE_RELEASE(m_pD3D)
}



HRESULT CD3DGraphics::CreateDevice(HWND hWnd, int nWidth, int nHeight)
{
    /// 디바이스를 생성하기위한 D3D객체 생성
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

	// Get the current desktop display mode, so we can set up a back
	// buffer of the same format
	D3DDISPLAYMODE d3ddm;
	if(FAILED( m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "GetAdapterDisplayMode Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return E_FAIL;
	}

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat		= D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth = nWidth;
	d3dpp.BackBufferHeight = nHeight;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.Flags					= D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    /// 디바이스 생성
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pd3dDevice ) ) )
    {
        return E_FAIL;
    }
	
	return S_OK;
}