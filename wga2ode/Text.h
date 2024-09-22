#ifndef _CTEXT_H_
#define _CTEXT_H_

#include "misc.h"

class AFX_EXT_CLASS CText
{

	LPD3DXFONT	m_lpFont;

public:
	CText(void){ m_lpFont = NULL; }
	~CText(void){ Clean(); }

	virtual void Init(LPDIRECT3DDEVICE9 pd3dDevice, int nFontWidth, int nFontHeight);
	virtual void Print(char *str, int nPosX, int nPosY, D3DCOLOR color=0xffffffff);
	virtual void Clean();
};

#endif