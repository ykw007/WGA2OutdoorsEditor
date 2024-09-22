#pragma once


#include "afxcmn.h"
#include "DlgTerrainEditor.h"
#include "DlgObjectEditor.h"
#include "SubDialog.h"

// CDlgbarTabmenu 대화 상자입니다.

class CDlgbarTabmenu : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgbarTabmenu)

public:
	CDlgbarTabmenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgbarTabmenu();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGBAR_TABMENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CImageList				m_ImageList;
	CTabCtrl				m_ctrlTabMenu;
	CDlgTerrainEditor*		m_pdlgTerrainEditor;
	CDlgObjectEditor*		m_pdlgObjectEditor;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeDlgTab(NMHDR *pNMHDR, LRESULT *pResult);
};
