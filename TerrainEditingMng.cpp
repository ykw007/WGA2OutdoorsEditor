#include "StdAfx.h"

#include "terraineditingmng.h"
#include ".\WGA2ode\d3dgraphics.h"


//## �������� ������Ʈ üũ�� ���� ��
static int s_nCounter = 0;

CTerrainEditingMng::CTerrainEditingMng(void)
{
	Clear();
	m_hWnd = NULL;
	m_uiCurMode = T_NUM_MODE;
}

CTerrainEditingMng::~CTerrainEditingMng(void)
{
	Clear();
}

void CTerrainEditingMng::Clear()
{
	m_pTerrain = NULL;
	m_pOctree = NULL;

	//## ���� ���� ������� �˷��ִ� ����
	//m_bTerrainModification = false;
	//m_bTerrainTexture = false;
	m_uiCurMode = T_NUM_MODE;
	ClearEditMode();
}


//## �ߺ� �ڵ尡 ������ ������ ��
void CTerrainEditingMng::Init(	LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, CTerrain *pTerrain,
								COctree *pOctree )
{
	m_pOctree = pOctree;

	if( m_hWnd == NULL )
	{
		m_hWnd = hWnd; 
		m_ModEditArea.Init( pd3dDevice, hWnd );		//## ó�� �ѹ��� �ʱ�ȭ
		m_TexEditArea.Init( pd3dDevice, hWnd );		//## ó�� �ѹ��� �ʱ�ȭ
		m_ObjEditArea.Init( pd3dDevice, hWnd );		//## ó�� �ѹ��� �ʱ�ȭ
	}

	if( m_pTerrain == NULL )
	{
		m_pTerrain = pTerrain;
		m_ModEditArea.SetTerrain( pTerrain );
		m_TexEditArea.SetTerrain( pTerrain );
		m_ObjEditArea.SetTerrain( pTerrain );

		m_ModEditArea.SetOctree( m_pOctree );
		m_TexEditArea.SetOctree( m_pOctree );
		m_ObjEditArea.SetOctree( m_pOctree );
	}
}

//####################################################
// ���� ���� ��� ����� ���� �ʱ�ȭ
//####################################################
void CTerrainEditingMng::InitEditMode(UINT uiMode)
{
	//## ���� ��� ����
	m_uiCurMode = uiMode;


	//## ���� ���� ��� ON
	switch( uiMode )
	{
	case T_HEIGHT :
		m_HeightWorker.SetEditAreaPtr(&m_ModEditArea);
		m_ModEditArea.SetTargetType( LAND );
		break;

	case T_TEXTURE :
		m_TexWorker.SetEditAreaPtr(&m_TexEditArea);
		m_ModEditArea.SetTargetType( LAND );
		break;

	case T_OBJECT :
		m_ObjWorker.SetEditAreaPtr(&m_ObjEditArea);
		m_ObjEditArea.SetTargetType( LAND );
		break;
	}

}

//####################################################
// ���� ���� ��� ���ῡ ���� ó��
//####################################################
void CTerrainEditingMng::ClearEditMode()
{
	//## �ʱ� ���·� �ݵ�� �����ش�.
	//## �׷��� ������ ������ ������ �ȴ�. 

	//## ���� ���� ��� ON
	//## ���� ���� ��� OFF
	switch( m_uiCurMode )
	{
	case T_HEIGHT  :
		m_ModEditArea.Destroy();
		break;

	case T_TEXTURE :
		m_TexEditArea.Destroy();
		break;

	case T_OBJECT :
		m_ObjEditArea.Destroy();
		break;
	}

	m_uiCurMode = T_NUM_MODE;
}

void CTerrainEditingMng::Update(POINT &ptCursor)
{
	switch( m_uiCurMode )
	{
	case T_HEIGHT :
		//## ���� �۾�
		m_HeightWorker.Work(ptCursor);
		break;

	case T_TEXTURE :
		//## �ؽ��ĸ� �۾�
		m_TexWorker.Work(ptCursor);
		break;

	case T_OBJECT:
		m_ObjWorker.Work(ptCursor);
		break;
	}
}
