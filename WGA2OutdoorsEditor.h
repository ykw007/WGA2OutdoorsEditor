// WGA2OutdoorsEditor.h : WGA2OutdoorsEditor 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // 주 기호


#include <d3d9.h>
#include <d3dx9.h>

// CWGA2OutdoorsEditorApp:
// 이 클래스의 구현에 대해서는 WGA2OutdoorsEditor.cpp을 참조하십시오.
//


class CWGA2OutdoorsEditorApp : public CWinApp
{
	bool				m_bUpdatingView;

public:
	CWGA2OutdoorsEditorApp();


// 재정의
public:
	afx_msg void OnAppExit();
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnFileOpen();

	void SetUpdatingView(bool bUpdatingView){ m_bUpdatingView = bUpdatingView; }

// 구현
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


	virtual BOOL InitApplication();

};

extern CWGA2OutdoorsEditorApp theApp;

