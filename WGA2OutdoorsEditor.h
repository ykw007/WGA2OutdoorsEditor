// WGA2OutdoorsEditor.h : WGA2OutdoorsEditor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // �� ��ȣ


#include <d3d9.h>
#include <d3dx9.h>

// CWGA2OutdoorsEditorApp:
// �� Ŭ������ ������ ���ؼ��� WGA2OutdoorsEditor.cpp�� �����Ͻʽÿ�.
//


class CWGA2OutdoorsEditorApp : public CWinApp
{
	bool				m_bUpdatingView;

public:
	CWGA2OutdoorsEditorApp();


// ������
public:
	afx_msg void OnAppExit();
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnFileOpen();

	void SetUpdatingView(bool bUpdatingView){ m_bUpdatingView = bUpdatingView; }

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()


	virtual BOOL InitApplication();

};

extern CWGA2OutdoorsEditorApp theApp;

