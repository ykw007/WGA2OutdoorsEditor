// D:\WORK\Terrain_Map_Tool\WGA2OutdoorsEditor\dialog\CreateTerrainDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "..\WGA2OutdoorsEditor.h"
#include "..\wga2ode\dib.h"
#include "..\wga2ode\math\mathutil.h"
#include "..\wga2ode\file\fileutil.h"
#include "CreateTerrainDlg.h"

//##############################################################
//## combo �׸�
static CString g_sTerrainSize[][2] = {
		{ "64", "64 x 64" },
		{ "128", "128 x 128" },
		{ "256", "256 x 256" },
		{ "512", "512 x 512" } 
};

static CString g_sNumPatches[] = { "4", "8", "16", "32", "64" };



// CCreateTerrainDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCreateTerrainDlg, CDialog)
CCreateTerrainDlg::CCreateTerrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateTerrainDlg::IDD, pParent)
	, m_nMapSize(128), m_nNumPatchesPerSide(8)
	, m_nTerrainType(0)
	, m_nCellSpace(4)
{
}

CCreateTerrainDlg::~CCreateTerrainDlg()
{
}

void CCreateTerrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDV_MinMaxInt(pDX, m_nMapSize, 4, 2048);
	DDX_Radio(pDX, IDC_RADIO_FLAT, m_nTerrainType);
	DDX_Control(pDX, IDC_BTN_HEIGHTOPEN, m_ctrlBtnHeightMap);
	DDX_Text(pDX, IDC_CELL_SPACE, m_nCellSpace);
	DDV_MinMaxInt(pDX, m_nCellSpace, 1, 64);
	DDX_Control(pDX, IDC_MAP_INFO, m_ctrlMapInfo);
	DDX_Control(pDX, IDC_COMBO_TERRAIN_SIZE, m_comboTerrainSize);
	DDX_Control(pDX, IDC_COMBO_NUM_PATCH, m_comboNumPatches);
	DDX_Control(pDX, IDC_EDIT_TEX_FILENAME, m_editTexFileName);
	DDX_Control(pDX, IDC_HEIGHTMAP_SIZE, m_stcHeightmapSize);
	DDX_Control(pDX, IDC_HEIGHTMAP_FILE, m_editHeightmapFile);
	DDX_Control(pDX, IDC_NUM_CELLS_PER_PATCH, m_stcNumCellsPerPatch);
}


BEGIN_MESSAGE_MAP(CCreateTerrainDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_HEIGHTMAP, OnBnClickedRadioHeightmap)
	ON_BN_CLICKED(IDC_RADIO_FLAT, OnBnClickedRadioFlat)
	ON_BN_CLICKED(IDC_RADIO_RANDOM, OnBnClickedRadioRandom)
	ON_BN_CLICKED(IDC_BTN_HEIGHTOPEN, OnBnClickedBtnHeightopen)
	ON_CBN_SELCHANGE(IDC_COMBO_TERRAIN_SIZE, OnCbnSelchangeComboTerrainSize)
	ON_EN_CHANGE(IDC_CELL_SPACE, OnEnChangeCellSpace)
	ON_CBN_SELCHANGE(IDC_COMBO_NUM_PATCH, OnCbnSelchangeComboNumPatch)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_OPEN_TEXTURE, OnBnClickedBtnOpenTexture)
END_MESSAGE_MAP()


// CCreateTerrainDlg �޽��� ó�����Դϴ�.

BOOL CCreateTerrainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//## �⺻ �ؽ���
	strcpy( m_strTexturePath, BASE_TEXTURE );

	//## terrain size
	for( int i = 0; i < 4; i++ )
		m_comboTerrainSize.AddString( g_sTerrainSize[i][1] );

	m_comboTerrainSize.SetCurSel(1);	//## ù��° �׸��� �����ش�.
	OnCbnSelchangeComboTerrainSize();

	//## patch ��
	for( int j = 0; j < sizeof(g_sNumPatches)/sizeof(CString); j++ )
		m_comboNumPatches.AddString( g_sNumPatches[j] );

	m_comboNumPatches.SetCurSel(1);	//## ù��° �׸��� �����ش�.
	OnCbnSelchangeComboNumPatch();



	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CCreateTerrainDlg::OnCbnSelchangeComboTerrainSize()
{
	//## ���° �׸��� ���õǾ��°�?
	if( !m_ctrlBtnHeightMap.IsWindowEnabled() )
	{
		int nSelItem = m_comboTerrainSize.GetCurSel();

		m_nMapSize = atoi( g_sTerrainSize[ nSelItem ][0] );
	}

	int nNumCells = m_nMapSize/ m_nNumPatchesPerSide;

	if( nNumCells > 16 )
		Adjust( &m_comboNumPatches, 0, m_nNumPatchesPerSide , OnCbnSelchangeComboNumPatch );

	Update();
}

void CCreateTerrainDlg::OnCbnSelchangeComboNumPatch()
{
	//## ���° �׸��� ���õǾ��°�?
	int nSelItem = m_comboNumPatches.GetCurSel();

	m_nNumPatchesPerSide = atoi( g_sNumPatches[ nSelItem ] );

	int nNumCells = m_nMapSize/ m_nNumPatchesPerSide;

	if( nNumCells > 16 )
		Adjust( &m_comboTerrainSize, m_nMapSize, 0 , OnCbnSelchangeComboTerrainSize );

	Update();
}


//#############################################################################
// combo ���� �Լ�
// ��ġ ���� 4�ε� �ʻ���� 512�� �����ϰ� �Ǹ�
// �ε����� ���ڶ� �������� �������� ��ġ�� ���´�.
// ���� ���� ���ý� ������ ���� �������� �����ϰ� �Ѵ�.
// �ʻ����� / ��ġ�� = 16 �̻��� �Ǹ� ��ġ�� ������� ������ ���´�.
// �׷��� 16 �̻��̸� ���� �޺� ���� �̵��Ѵ�.
//#############################################################################
void CCreateTerrainDlg::Adjust( CComboBox *pCombo, int nMapSize, int nNumPatchesPerSide,
						  void (CCreateTerrainDlg::*pf)() )
{
	int nNumCellsPerPatch, nMove;

	int nNextMapSize = 0;
	int nNextNumPatches = 0;

	int nSelItem;

	if( nMapSize == 0 )
	{
		nNumCellsPerPatch = m_nMapSize / nNumPatchesPerSide;
		nMove = 1;

		nSelItem = pCombo->GetCurSel();

		if( nSelItem == ((sizeof g_sNumPatches) / sizeof(CString)) )
			nSelItem = -1;
		nNextNumPatches = atoi( g_sNumPatches[ nSelItem+1 ] );
	}

	if( nNumPatchesPerSide == 0 )
	{
		nNumCellsPerPatch = nMapSize / m_nNumPatchesPerSide;
		nMove = -1;

		nSelItem = pCombo->GetCurSel();

		if( nSelItem == 0 )
			nSelItem = (sizeof g_sTerrainSize)/ (sizeof(CString)*2);

		nNextMapSize = atoi( g_sTerrainSize[ nSelItem-1 ][0] );
	}

	if( nNumCellsPerPatch > 16 )
	{
		pCombo->SetCurSel( nSelItem + nMove );
		Adjust( pCombo, nNextMapSize, nNextNumPatches, pf );
	}
	else
	{
		(this->*pf)();
	}
}



void CCreateTerrainDlg::OnBnClickedRadioHeightmap()
{
	m_comboTerrainSize.EnableWindow(FALSE);
	m_ctrlBtnHeightMap.EnableWindow(TRUE);
}

void CCreateTerrainDlg::OnBnClickedRadioFlat()
{
	m_comboTerrainSize.EnableWindow(TRUE);
	m_ctrlBtnHeightMap.EnableWindow(FALSE);

	strcpy( m_strHeightMapPath, "");
	OnCbnSelchangeComboTerrainSize();
	Update();
}

void CCreateTerrainDlg::OnBnClickedRadioRandom()
{
	m_comboTerrainSize.EnableWindow(TRUE);
	m_ctrlBtnHeightMap.EnableWindow(FALSE);
}

void CCreateTerrainDlg::OnEnChangeCellSpace()
{
	if( UpdateData(TRUE) )
		Update();
}


void CCreateTerrainDlg::OnBnClickedOk()
{
	if( m_ctrlBtnHeightMap.IsWindowEnabled() )
	{
		if( strlen( m_strHeightMapPath ) == 0 )
		{
			AfxMessageBox("Heightmap ������ �����ϼ���.");		
			return;
		}
	}
	OnOK();
}

//########################################################################
// Texture ���� ����
//########################################################################
void CCreateTerrainDlg::OnBnClickedBtnOpenTexture()
{
	OPENFILENAME OFN;

	char lpstrFile[MAX_PATH] = "";
	char lpDir[MAX_PATH] = "";
	char lpCopyDir[MAX_PATH] = "";

	GetCurrentDirectory( MAX_PATH, lpDir );
	strcpy( lpCopyDir, lpDir );

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = "JPG Files (*.jpg,*.jpeg)\0*.jpg;*.jpeg\0BMP Files (*.bmp)\0*.bmp\0TGA Files (*.tga)\0*.tga\0��� ���� (*.*)\0*.*\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	OFN.lpstrInitialDir = strcat( lpDir, PATCH_BASETEXTURE_DIR );

	if( GetOpenFileName( &OFN ) != 0 )
	{
		strcpy( m_strTexturePath, lpstrFile );
		Update();
	}

	//## ���� ���丮�� �����Ѵ�.
	SetCurrentDirectory( lpCopyDir );
}

//########################################################################
// Heightmap ���� ����
//########################################################################
void CCreateTerrainDlg::OnBnClickedBtnHeightopen()
{
	bool bErrFlag = false;

	OPENFILENAME OFN;

	char lpstrFile[MAX_PATH] = "";
	char lpDir[MAX_PATH] = "";
	char lpCopyDir[MAX_PATH] = "";

	GetCurrentDirectory( MAX_PATH, lpDir );
	strcpy( lpCopyDir, lpDir );

	memset( &OFN, 0, sizeof(OPENFILENAME) );
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = m_hWnd;
	OFN.lpstrFilter = "bmp Files (*.bmp)\0*.bmp\0��� ���� (*.*)\0*.*\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	OFN.lpstrInitialDir = strcat( lpDir, HEIGHTMAP_DIR );

	if( GetOpenFileName( &OFN ) != 0 )
	{
		CDib dib;

		if( !dib.LoadHandle( lpstrFile ) )
		{
			return;
		}

		//## 2^n+1 ũ������ üũ�Ѵ�.
		int nWidth = dib.GetWidth();
		int nHeight = dib.GetHeight();

		if( nWidth != nHeight )
		{
			AfxMessageBox("Height Map �̹��� ������ ũ�Ⱑ ����, ���� ���ƾ� �մϴ�."
				"                           �ٸ� ������ ������ �ּ���.");
			return;
		}

		if( ISPOW2( nWidth-1 ) )
		{
			if( !ISPOW2( nHeight-1 ) )
				bErrFlag = true;
		}
		else
			bErrFlag = true;

		if( bErrFlag )
		{
			AfxMessageBox("Height Map �̹��� ������"
				"���ο� ������ ũ�Ⱑ ��� 2^n+1 �̾�� �մϴ�.\r\n"
				"                           �ٸ� ������ ������ �ּ���.");
			return;
		}

		m_nMapSize =  nWidth - 1;
		strcpy( m_strHeightMapPath, lpstrFile );
		OnCbnSelchangeComboTerrainSize();
		Update();
	}

	//## ���� ���丮�� �����Ѵ�.
	SetCurrentDirectory( lpCopyDir );
}


void CCreateTerrainDlg::Update()
{
	char cMapInfo[255];
	char cCellsPerPatch[255];
	char cHeightmapFile[255];
	char cHeightmapSize[255];
	char cFileName[255];

	//## ���� ���� ũ��
	int nRealSize = m_nMapSize * m_nCellSpace;
	sprintf( cMapInfo, "%d X %d", nRealSize, nRealSize );
	m_ctrlMapInfo.SetWindowText( cMapInfo );

	//## patch �� �� �� ## �� ���� 1 ���ٴ� Ŀ���Ѵ�.
	int nNumCells = m_nMapSize / m_nNumPatchesPerSide;
	sprintf( cCellsPerPatch, "%d", nNumCells );
	m_stcNumCellsPerPatch.SetWindowText( cCellsPerPatch );

	if( strlen( m_strHeightMapPath ) > 0 )
	{
		int nHeightmapSize = m_nMapSize + 1;
		GetFilePathToken( (const char *)m_strHeightMapPath , NULL, NULL, cHeightmapFile, NULL );

		sprintf( cHeightmapSize, "%d X %d", nHeightmapSize, nHeightmapSize );
	}
	else
	{
		strcpy( cHeightmapFile, "�������� ����" );
		strcpy( cHeightmapSize, "�������� ����" );
	}

	m_stcHeightmapSize.SetWindowText( cHeightmapSize );
	m_editHeightmapFile.SetWindowText( cHeightmapFile );

	//## �ؽ��� ���ϸ� ǥ��
	if( strlen( m_strTexturePath ) > 0 )
	{
		GetFilePathToken( (const char *)m_strTexturePath , NULL, NULL, cFileName, NULL );
		m_editTexFileName.SetWindowText( cFileName );
	}
	else
		strcpy( cFileName, "�������� ����" );

	m_editTexFileName.SetWindowText( cFileName );
}