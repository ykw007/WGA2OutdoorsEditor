#ifndef _DLG_TERRAINEDITOR_H_
#define _DLG_TERRAINEDITOR_H_


#include "afxwin.h"
#include "SubDialog.h"
#include "wga2button.h"
#include "terraineditingmng.h"
// CDlgTerrainEditor 대화 상자입니다.



class CDlgTerrainEditor : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgTerrainEditor)

public:


	CDlgTerrainEditor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTerrainEditor();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TERRAIN_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

public:
	virtual BOOL OnInitDialog();
	void ActiveTerrainEditor(bool bActive);
	void SetActiveButton(CWGA2Button &btn, bool bActive);

	//##################################################################
	//## editor가 추가되면 여기에 CWGA2Button Click event 추가
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	afx_msg void OnBnClickedTerrainModification();
	afx_msg void OnBnClickedTerrainTexture();
	afx_msg void OnBnClickedTerrainObject();

private:
	void Display( BYTE byMode );

public:

	struct S_EDITOR
	{
		CDialog			*pDialog;	//## dlg pointer
		CWGA2Button		*pButton;	//## button pointer
		bool			bShow;		//## visible 상태
		
		S_EDITOR()
		{
			pDialog = NULL;			
			pButton = NULL;			
			bShow = false;			
		}
	};

	//##################################################################
	//## editor가 추가되면 여기에 CWGA2Button 추가
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## terrain 편집 버튼 컨트롤 변수
	CWGA2Button						m_ctrlBtnModification;		
	CWGA2Button						m_ctrlBtnTexture;
	CWGA2Button						m_ctrlBtnObject;

	S_EDITOR						m_aEditor[ T_NUM_MODE ];

};

#endif