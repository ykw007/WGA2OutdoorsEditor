// MainFrm.h : CMainFrame Ŭ������ �������̽�
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
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư��
public:

// ������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif



// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowMainEditor();
	afx_msg void OnUpdateWindowMainEditor(CCmdUI *pCmdUI);
	afx_msg void OnImportTerrainObject();
	afx_msg void OnUpdateImportTerrainObject(CCmdUI *pCmdUI);

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.

	CToolBar    m_wndToolBar;
	CCoolDialogBar		m_wndMainEditor;
	CCoolDialogBar		m_wndObserver;			//## ����ſ� dlgbar

// �۾�
public:

	CStatusBar			m_wndStatusBar;
	CDlgbarTabmenu		m_wndDlgbarTabmenu;
	CDlgbarObserver		m_wndDlgbarObserver;

	BOOL				m_bTerrainObjImport;
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


#endif