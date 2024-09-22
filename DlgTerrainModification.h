#ifndef _DLG_TERRAINMODIFICATION_H_
#define _DLG_TERRAINMODIFICATION_H_

#include "afxcmn.h"
#include "afxwin.h"

#include "SubDialog.h"

// CDlgTerrainModification 대화 상자입니다.

class CDlgTerrainModification : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgTerrainModification)

public:
	CDlgTerrainModification(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTerrainModification();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TERRAIN_MODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditCircleRadius();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedSerialUpdate();
	afx_msg void OnBnClickedRadioSync();
	afx_msg void OnBnClickedRadioSine();
	afx_msg void OnEnChangeEditSyncHeight();
	afx_msg void OnEnChangeEditSineHeight();
	afx_msg void OnEnChangeUpdateSpeed();
	afx_msg void OnEnChangeEditLimitHeight();
	void SetDisableRadioItem();
	void UpdateRadius();
	void UpdateLimitHeight();

	CStatic m_ctrlText6;
	CEdit m_ctrlSineHeight;
	CEdit m_ctrlEditLimitHeight;
	CButton m_ctrlCheckSerialUpdate;
	CComboBox m_ctrlModType;
	CSliderCtrl m_ctrlSliderRadius;
	CSliderCtrl m_ctrlSliderLimitHeight;
	CSpinButtonCtrl m_ctrlSpinUpdateSpeed;

	int m_nRadius;
	int m_nUpdateSpeed;
	int m_nUpdateType;
	float m_fSyncHeight;
	float m_fSineHeight;
	int m_nLimitHeight;
};

#endif