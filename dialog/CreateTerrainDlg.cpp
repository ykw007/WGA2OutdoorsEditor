// D:\WORK\Terrain_Map_Tool\WGA2OutdoorsEditor\dialog\CreateTerrainDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "..\WGA2OutdoorsEditor.h"
#include "..\wga2ode\dib.h"
#include "..\wga2ode\math\mathutil.h"
#include "..\wga2ode\file\fileutil.h"
#include "CreateTerrainDlg.h"

//##############################################################
//## combo 항목
static CString g_sTerrainSize[][2] = {
		{ "64", "64 x 64" },
		{ "128", "128 x 128" },
		{ "256", "256 x 256" },
		{ "512", "512 x 512" } 
};

static CString g_sNumPatches[] = { "4", "8", "16", "32", "64" };



// CCreateTerrainDlg 대화 상자입니다.

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


// CCreateTerrainDlg 메시지 처리기입니다.

BOOL CCreateTerrainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//## 기본 텍스쳐
	strcpy( m_strTexturePath, BASE_TEXTURE );

	//## terrain size
	for( int i = 0; i < 4; i++ )
		m_comboTerrainSize.AddString( g_sTerrainSize[i][1] );

	m_comboTerrainSize.SetCurSel(1);	//## 첫번째 항목을 보여준다.
	OnCbnSelchangeComboTerrainSize();

	//## patch 수
	for( int j = 0; j < sizeof(g_sNumPatches)/sizeof(CString); j++ )
		m_comboNumPatches.AddString( g_sNumPatches[j] );

	m_comboNumPatches.SetCurSel(1);	//## 첫번째 항목을 보여준다.
	OnCbnSelchangeComboNumPatch();



	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCreateTerrainDlg::OnCbnSelchangeComboTerrainSize()
{
	//## 몇번째 항목이 선택되었는가?
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
	//## 몇번째 항목이 선택되었는가?
	int nSelItem = m_comboNumPatches.GetCurSel();

	m_nNumPatchesPerSide = atoi( g_sNumPatches[ nSelItem ] );

	int nNumCells = m_nMapSize/ m_nNumPatchesPerSide;

	if( nNumCells > 16 )
		Adjust( &m_comboTerrainSize, m_nMapSize, 0 , OnCbnSelchangeComboTerrainSize );

	Update();
}


//#############################################################################
// combo 조정 함수
// 패치 수가 4인데 맵사이즈를 512로 선택하게 되면
// 인덱스가 모자라서 렌더링시 없어지는 패치가 나온다.
// 지형 생성 선택시 오류가 없는 조건으로 선택하게 한다.
// 맵사이즈 / 패치수 = 16 이상이 되면 패치가 사라지는 현상이 나온다.
// 그래서 16 이상이면 하위 콤보 모드로 이동한다.
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
			AfxMessageBox("Heightmap 파일을 지정하세요.");		
			return;
		}
	}
	OnOK();
}

//########################################################################
// Texture 파일 열기
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
	OFN.lpstrFilter = "JPG Files (*.jpg,*.jpeg)\0*.jpg;*.jpeg\0BMP Files (*.bmp)\0*.bmp\0TGA Files (*.tga)\0*.tga\0모든 파일 (*.*)\0*.*\0";
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = MAX_PATH;
	OFN.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ENABLESIZING;
	OFN.lpstrInitialDir = strcat( lpDir, PATCH_BASETEXTURE_DIR );

	if( GetOpenFileName( &OFN ) != 0 )
	{
		strcpy( m_strTexturePath, lpstrFile );
		Update();
	}

	//## 현재 디렉토리를 설정한다.
	SetCurrentDirectory( lpCopyDir );
}

//########################################################################
// Heightmap 파일 열기
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
	OFN.lpstrFilter = "bmp Files (*.bmp)\0*.bmp\0모든 파일 (*.*)\0*.*\0";
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

		//## 2^n+1 크기인지 체크한다.
		int nWidth = dib.GetWidth();
		int nHeight = dib.GetHeight();

		if( nWidth != nHeight )
		{
			AfxMessageBox("Height Map 이미지 파일의 크기가 가로, 세로 같아야 합니다."
				"                           다른 파일을 선택해 주세요.");
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
			AfxMessageBox("Height Map 이미지 파일은"
				"가로와 세로의 크기가 모두 2^n+1 이어야 합니다.\r\n"
				"                           다른 파일을 선택해 주세요.");
			return;
		}

		m_nMapSize =  nWidth - 1;
		strcpy( m_strHeightMapPath, lpstrFile );
		OnCbnSelchangeComboTerrainSize();
		Update();
	}

	//## 현재 디렉토리를 설정한다.
	SetCurrentDirectory( lpCopyDir );
}


void CCreateTerrainDlg::Update()
{
	char cMapInfo[255];
	char cCellsPerPatch[255];
	char cHeightmapFile[255];
	char cHeightmapSize[255];
	char cFileName[255];

	//## 지형 실제 크기
	int nRealSize = m_nMapSize * m_nCellSpace;
	sprintf( cMapInfo, "%d X %d", nRealSize, nRealSize );
	m_ctrlMapInfo.SetWindowText( cMapInfo );

	//## patch 당 셀 수 ## 셀 수가 1 보다는 커야한다.
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
		strcpy( cHeightmapFile, "지정되지 않음" );
		strcpy( cHeightmapSize, "지정되지 않음" );
	}

	m_stcHeightmapSize.SetWindowText( cHeightmapSize );
	m_editHeightmapFile.SetWindowText( cHeightmapFile );

	//## 텍스쳐 파일명 표시
	if( strlen( m_strTexturePath ) > 0 )
	{
		GetFilePathToken( (const char *)m_strTexturePath , NULL, NULL, cFileName, NULL );
		m_editTexFileName.SetWindowText( cFileName );
	}
	else
		strcpy( cFileName, "지정되지 않음" );

	m_editTexFileName.SetWindowText( cFileName );
}