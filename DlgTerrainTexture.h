#ifndef _DLG_TERRAINTEXTURE_H_
#define _DLG_TERRAINTEXTURE_H_

#include "SubDialog.h"
#include "afxcmn.h"

// CDlgTerrainTexture ��ȭ �����Դϴ�.

class CDlgTerrainTexture : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgTerrainTexture)

public:
	CDlgTerrainTexture(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTerrainTexture();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_TERRAIN_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditCircleRadius();
	void UpdateRadius();

	//## ��Ʈ�� ����
	CSliderCtrl m_ctrlSliderRadius;

	//## 
	int m_nRadius;
};

#endif