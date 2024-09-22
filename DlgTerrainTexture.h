#ifndef _DLG_TERRAINTEXTURE_H_
#define _DLG_TERRAINTEXTURE_H_

#include "SubDialog.h"
#include "afxcmn.h"

// CDlgTerrainTexture 대화 상자입니다.

class CDlgTerrainTexture : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgTerrainTexture)

public:
	CDlgTerrainTexture(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTerrainTexture();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TERRAIN_TEXTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditCircleRadius();
	void UpdateRadius();

	//## 컨트롤 변수
	CSliderCtrl m_ctrlSliderRadius;

	//## 
	int m_nRadius;
};

#endif