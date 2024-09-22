#ifndef _DLG_TERRAINEDITOR_H_
#define _DLG_TERRAINEDITOR_H_


#include "afxwin.h"
#include "SubDialog.h"
#include "wga2button.h"
#include "terraineditingmng.h"
// CDlgTerrainEditor ��ȭ �����Դϴ�.



class CDlgTerrainEditor : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgTerrainEditor)

public:


	CDlgTerrainEditor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTerrainEditor();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_TERRAIN_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

public:
	virtual BOOL OnInitDialog();
	void ActiveTerrainEditor(bool bActive);
	void SetActiveButton(CWGA2Button &btn, bool bActive);

	//##################################################################
	//## editor�� �߰��Ǹ� ���⿡ CWGA2Button Click event �߰�
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
		bool			bShow;		//## visible ����
		
		S_EDITOR()
		{
			pDialog = NULL;			
			pButton = NULL;			
			bShow = false;			
		}
	};

	//##################################################################
	//## editor�� �߰��Ǹ� ���⿡ CWGA2Button �߰�
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## terrain ���� ��ư ��Ʈ�� ����
	CWGA2Button						m_ctrlBtnModification;		
	CWGA2Button						m_ctrlBtnTexture;
	CWGA2Button						m_ctrlBtnObject;

	S_EDITOR						m_aEditor[ T_NUM_MODE ];

};

#endif