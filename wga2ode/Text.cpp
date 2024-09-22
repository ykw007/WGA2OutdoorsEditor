#include "StdAfx.h"
#include "text.h"

void CText::Init(LPDIRECT3DDEVICE9 pd3dDevice, int nFontWidth, int nFontHeight)
{
	HDC		hTextDC = NULL;
	HFONT	hFont = NULL, hOldFont = NULL;
	
	hTextDC = CreateCompatibleDC(NULL);
    hFont = CreateFont( nFontHeight, nFontWidth, 0, 0,
                      FW_REGULAR,
                      FALSE, FALSE, FALSE,
                      SHIFTJIS_CHARSET,
                      OUT_DEFAULT_PRECIS,
                      CLIP_DEFAULT_PRECIS,
                      DEFAULT_QUALITY,
                      DEFAULT_PITCH,
                      "MS °íµñ"
                      );

	if(!hFont) return;
	hOldFont = (HFONT)SelectObject(hTextDC, hFont);
	
//	if( FAILED( D3DXCreateFont( pd3dDevice, hFont, &m_lpFont ) ) ) {
//		MessageBox( 0, "D3DXCreateFontIndirect FALSE", "ok", MB_OK);
//		return;
//	}

	SelectObject(hTextDC, hOldFont);
	DeleteObject(hFont);
	DeleteDC(hTextDC);
}

void CText::Print( char *str, int nPosX, int nPosY, D3DCOLOR color )
{
	RECT rect;
	rect.left = nPosX; 
	rect.right = 640;
	rect.top = nPosY;
	rect.bottom = 480;

//	m_lpFont->DrawText(str, -1, &rect,DT_LEFT | DT_EXPANDTABS,color);
}

void CText::Clean()
{
	SAFE_RELEASE(m_lpFont);
}