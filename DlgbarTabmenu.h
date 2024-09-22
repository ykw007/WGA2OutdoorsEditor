#pragma once


#include "afxcmn.h"
#include "DlgTerrainEditor.h"
#include "DlgObjectEditor.h"
#include "SubDialog.h"

// CDlgbarTabmenu ��ȭ �����Դϴ�.

class CDlgbarTabmenu : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgbarTabmenu)

public:
	CDlgbarTabmenu(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgbarTabmenu();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGBAR_TABMENU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
