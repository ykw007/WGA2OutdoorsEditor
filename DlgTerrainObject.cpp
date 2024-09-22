// DlgTerrainObject.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgTerrainObject.h"
#include "terraineditingmng.h"
#include "terrainobjectworker.h"

#include "MainFrm.h"

//## view �����͸� ��� ���� INCLUDE
#include "WGA2OutdoorsEditorDoc.h"
#include "WGA2OutdoorsEditorView.h"

// CDlgTerrainObject ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgTerrainObject, CDialog)
CDlgTerrainObject::CDlgTerrainObject(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgTerrainObject::IDD, pParent)
{
}

CDlgTerrainObject::~CDlgTerrainObject()
{
	Destroy();
}

void CDlgTerrainObject::Destroy()
{
	if( !m_lstTreeObj.empty() )
	{
		stl_wipe( m_lstTreeObj );
		m_treeTObj.DeleteAllItems();
	}
}

void CDlgTerrainObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TERRAIN_OBJECT_LIB, m_treeTObj);
	DDX_Control(pDX, IDC_SELECT_OBJECT, m_stcObjName);
	DDX_Control(pDX, IDC_ADD_OBJECT, m_rdoAddObject);
}


BEGIN_MESSAGE_MAP(CDlgTerrainObject, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TERRAIN_OBJECT_LIB, OnTvnSelchangedTerrainObjectLib)
	ON_BN_CLICKED(IDC_TRANS_OBJECT, OnBnClickedTransObject)
	ON_BN_CLICKED(IDC_ADD_OBJECT, OnBnClickedAddObject)
	ON_BN_CLICKED(IDC_DELETE_OBJECT, OnBnClickedDeleteObject)
	ON_BN_CLICKED(IDC_IMPORT_TERRAIN_OBJ, OnBnClickedImportTerrainObj)
END_MESSAGE_MAP()


// CDlgTerrainObject �޽��� ó�����Դϴ�.

BOOL CDlgTerrainObject::OnInitDialog()
{
	CSubDialog::OnInitDialog();

	//## ��� �տ� �̹��� ����
	m_ImageList.Create( IDB_BITMAP2, 16, 2, RGB(255, 255, 255) );
	m_treeTObj.SetImageList(&m_ImageList, TVSIL_NORMAL);

	//## �ʱ� �߰� ��ư�� ������ ���·� ����
	m_rdoAddObject.SetCheck( 1 );
	OnBnClickedAddObject();
/*

	for( int l2=0; l2<2 ; l2++)
	{
		//## level 2
		hLevel2 = m_treeTObj.InsertItem(pLevel2[l2], 0, 1, hLevel1);

		//## level 3
		for( int l3 = 0; l3 < 3; l3++ )
			m_treeTObj.InsertItem( pLevel3[l2][l3], 0, 1, hLevel2);

		m_treeTObj.Expand( hLevel2, TVE_EXPAND );
	}

	m_treeTObj.Expand( hLevel1, TVE_EXPAND );
    
*/
	UpdateRadius();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

bool CDlgTerrainObject::InsertTerrainObject(CModel *pModel,
											const char *pFilePathName)
{
	//###################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ���ϸ� ����
	char	*pStr = NULL;
	char	*pToken = NULL;
	char	cTemp[255];
	char	sep[] = "\\";

	strcpy( cTemp, pFilePathName);

	pToken = strtok( cTemp, sep );

	while( pToken != 0 )
	{
		pStr = pToken;
		pToken = strtok( 0, sep);
	}
	char *pName = new char[ strlen(pStr)+1 ];
	strcpy( pName, pStr );

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//###################################################

	//## ���� ����Ÿ
	S_TREE_OBJECT *pTreeObj = new S_TREE_OBJECT;
	pTreeObj->pName = pName;
	pTreeObj->pModel = pModel;
	m_lstTreeObj.push_back( pTreeObj );

	//## node �߰�
	HTREEITEM hNow;
	TVINSERTSTRUCT	Tis;

	hNow = TreeView_GetSelection( m_treeTObj.GetSafeHwnd() );

	Tis.hParent = 0;
	Tis.hInsertAfter = TVI_LAST;
	Tis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	Tis.item.iImage = 0;
	Tis.item.iSelectedImage = 1;
	Tis.item.pszText = pName;
	Tis.item.lParam = (LPARAM)pModel;

	hNow = m_treeTObj.InsertItem( &Tis );
	m_treeTObj.Expand( hNow, TVE_EXPAND );

	return true;
}

//####################################################
// tree�� �ִ� object ����
//####################################################
void CDlgTerrainObject::OnTvnSelchangedTerrainObjectLib(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	char cText[255];

	TVITEM Item;
	Item.mask = TVIF_PARAM | TVIF_TEXT;
	Item.hItem = pNMTreeView->itemNew.hItem;
	Item.pszText = cText;
	Item.cchTextMax = 255;
	m_treeTObj.GetItem( &Item );

	CModel *pModel = (CModel *)Item.lParam;

	//## ���� ������ ���� �۾��ڿ��� �˸�
	GetViewPointer()->m_TerrainEditingMng.GetObjectWorkerPtr()->SetTerrainObject( pModel );

	//## static â�� ǥ��
	m_stcObjName.SetWindowText( cText );

	*pResult = 0;
}

//####################################################
// ������ ����
//####################################################
void CDlgTerrainObject::UpdateRadius()
{
	//## GetViewPointer() : view �����͸� ���� �� �ִ� �Լ�
	GetViewPointer()->m_TerrainEditingMng.GetObjEditAreaPtr()->SetAreaHalfSize(5);
}

void CDlgTerrainObject::OnBnClickedTransObject()
{
	//GetViewPointer()->m_TerrainEditingMng.GetObjectWorkerPtr()->SetCurWorkingMode( TRANS );
}

void CDlgTerrainObject::OnBnClickedAddObject()
{
	GetViewPointer()->m_TerrainEditingMng.GetObjEditAreaPtr()->SetTargetType( LAND );
	GetViewPointer()->m_TerrainEditingMng.GetObjectWorkerPtr()->SetCurWorkingMode( ADD_OBJ );
}

void CDlgTerrainObject::OnBnClickedDeleteObject()
{
	GetViewPointer()->m_TerrainEditingMng.GetObjEditAreaPtr()->SetTargetType( OBSTACLE );
	GetViewPointer()->m_TerrainEditingMng.GetObjectWorkerPtr()->SetCurWorkingMode( DEL_OBJ );
}

void CDlgTerrainObject::OnBnClickedImportTerrainObj()
{
	//## MAINFRAME POINTER
	CMainFrame *pFr = (CMainFrame *)AfxGetMainWnd();

	pFr->OnImportTerrainObject();
}
