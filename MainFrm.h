// MainFrm.h : CMainFrame 클래스의 인터페이스
//
#include ".\WGA2ode\misc.h"
#include "cooldialogbar.h"
#include "dlgbartabmenu.h"
#include "dlgbarobserver.h"

//LPDIRECT3DDEVICE9	g_pd3dDevice;

#define TOBJ_DIR				"\\Data\\Source\\Terrain\\Objects\\"

#pragma once

#ifndef MAINFRM_H
#define MAINFRM_H

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성
public:

// 재정의
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



// 메시지 맵 함수를 생성했습니다.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowMainEditor();
	afx_msg void OnUpdateWindowMainEditor(CCmdUI *pCmdUI);
	afx_msg void OnImportTerrainObject();
	afx_msg void OnUpdateImportTerrainObject(CCmdUI *pCmdUI);

protected:  // 컨트롤 모음이 포함된 멤버입니다.

	CToolBar    m_wndToolBar;
	CCoolDialogBar		m_wndMainEditor;
	CCoolDialogBar		m_wndObserver;			//## 디버거용 dlgbar

// 작업
public:

	CStatusBar			m_wndStatusBar;
	CDlgbarTabmenu		m_wndDlgbarTabmenu;
	CDlgbarObserver		m_wndDlgbarObserver;

	BOOL				m_bTerrainObjImport;
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


#endif