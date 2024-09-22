#pragma once
#include "afxwin.h"

#define HEIGHTMAP_DIR				"\\Data\\Source\\Terrain\\Heightmaps\\"
#define PATCH_BASETEXTURE_DIR		"\\Data\\Source\\Terrain\\Textures\\"
#define BASE_TEXTURE				".\\Data\\Source\\Terrain\\Textures\\grass.jpg"

// CCreateTerrainDlg 대화 상자입니다.

class CCreateTerrainDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateTerrainDlg)

public:
	CCreateTerrainDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCreateTerrainDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CREATE_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioHeightmap();
	afx_msg void OnBnClickedRadioFlat();
	afx_msg void OnBnClickedRadioRandom();
	afx_msg void OnBnClickedBtnHeightopen();
	afx_msg void OnEnChangeCellSpace();

	afx_msg void OnCbnSelchangeComboTerrainSize();
	afx_msg void OnCbnSelchangeComboNumPatch();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnOpenTexture();

	void Update();
	void Adjust( CComboBox *pCombo, int nMapSize, int nNumPatchesPerSide,
		void (CCreateTerrainDlg::*pf)() );
public:
	int m_nMapSize;
	int m_nNumPatchesPerSide;
	int m_nCellSpace;
	int m_nTerrainType;		//## 생성할 Terrain 타입
	char m_strHeightMapPath[255];
	char m_strTexturePath[255];

	CEdit m_ctrlMapInfo;
	CButton m_ctrlBtnHeightMap;
	CComboBox m_comboTerrainSize;
	CComboBox m_comboNumPatches;

	CEdit m_editTexFileName;
	CStatic m_stcHeightmapSize;
	CEdit m_editHeightmapFile;
	CStatic m_stcNumCellsPerPatch;
};
