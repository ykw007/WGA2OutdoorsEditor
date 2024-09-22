#ifndef _D3DGRAPHICS_H_
#define _D3DGRAPHICS_H_

//## INCLUDES
#include "misc.h"

//####################################################
//####################################################
// d3d 출력을 위한 class
// singleton
//####################################################
//####################################################
class AFX_EXT_CLASS CD3DGraphics
{

public:
    CD3DGraphics();
    virtual ~CD3DGraphics();

    HRESULT CreateDevice(HWND hWnd, int nWidth, int nHeight);

public:
    LPDIRECT3D9			m_pD3D;              // The main D3D object
    LPDIRECT3DDEVICE9	m_pd3dDevice;        // The D3D rendering device
};


#endif