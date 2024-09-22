#include "StdAfx.h"
#include "gameworldmanager.h"
#include "texturemanager.h"
#include "observer.h"

CGameWorldManager::CGameWorldManager(void)
{
	m_pd3dDevice = NULL;
	Clear();
}

CGameWorldManager::~CGameWorldManager(void)
{
	Destroy();
}

void CGameWorldManager::Clear()
{
	m_bOctreeBbox = false;
	m_bRenderingSwitch = false;
}

void CGameWorldManager::Destroy()
{
	m_Octree.Destroy();
	m_LandscapeSceneMng.Destroy();
	m_TextureMng.Destroy();
	Clear();
}


//## ���� �ʿ�
void CGameWorldManager::InitGameWorld(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	if(m_pd3dDevice==NULL)
	{
		m_pd3dDevice = pd3dDevice;
	}

	if( m_hWnd == NULL )
		m_hWnd = hWnd;


	//## ī�޶� �ʱ�ȭ
	m_CameraMng.Init(pd3dDevice,hWnd);

	//## texture manager
	m_TextureMng.Init( pd3dDevice );

	//## ���� ������ �ʱ�ȭ
	//## TEXTURE ������ pointer�� �Ѱ��ش�.
	m_LandscapeSceneMng.Init( &m_TextureMng, pd3dDevice );
}

//####################################################
// octree ����
//####################################################
void CGameWorldManager::CreateGameWorld()
{
	CTerrain *pTerrain = m_LandscapeSceneMng.GetTerrainPtr();
	CPatch *pPatch = pTerrain->GetPatchBeginPtr();

	//####################################################
	//## octree object list�� �ۼ��Ѵ�.
	//## octree object�� octree�� ���� �� �ִ� ���� ��ü�̴�.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	ObjList *pOctObjList = new ObjList;

	int nNumPatchesPerSide = pTerrain->GetNumPatchesPerSide();
	int nNumAllPatches = nNumPatchesPerSide*nNumPatchesPerSide;

	//## ���� �� patch �� ��ü�� �ش� octree object list�� �Ҵ�
	//## octree�� ������ object ����ü
	S_OCT_OBJ *pOctObj;

	for( int i = 0; i < nNumAllPatches; i++ )
	{
		pOctObj = new S_OCT_OBJ;
		pOctObj->pObj = (CDrawMesh *)(pPatch+i);

		pOctObjList->push_back( pOctObj );
	}

	//## octree object list pointer�� �Ѱ��ش�.
	m_Octree.SetOctreeObjListPtr(pOctObjList);

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//####################################################

	//####################################################
	//## octree ����
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## octree ������ ���� ������

	//## octree ��� ���� ũ��
	int nLeafNodeSize = pPatch->GetPatchSpace();

	//## ������ ������ �߽� ���ϱ�
	D3DXVECTOR3 vCenter;
	vCenter.x = pTerrain->GetEdgeSpace()/2;
	vCenter.z = -vCenter.x;
	vCenter.y = abs( pPatch->GetMostMaxHeight() - pPatch->GetMostMinHeight() )/2;

	//## octree bounding box �� ���� ũ�� 
	int nWidth = MAX( vCenter.x*2, vCenter.y*2 );

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## ��Ʈ���� �����Ѵ�.
	//if( 

	m_Octree.Create( nWidth, vCenter, nLeafNodeSize );

	//)
	//{
		//## ��Ʈ�� ����
		//m_Picker.SetOctree(&m_Octree);
	//}
	
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//####################################################

	//## ī�޶� ���� �߽ɿ�
	vCenter.y += 50.0f;
	m_CameraMng.GetCameraPtr()->SetPosition( vCenter );
}

void CGameWorldManager::UpdateGameWorld(float &fDeltaTime, POINT &ptCursor)
{

	D3DXVECTOR3 vCameraPos;
	D3DXMATRIXA16	matProj, matView, matViewProj;

	if( m_bRenderingSwitch )
	{
		vCameraPos = m_CameraMng.GetCameraPtr()->GetPosition();

		//## ī�޶� ���̸� ���� ���� ����
		m_LandscapeSceneMng.AdjustVec3OverLand( &vCameraPos );
		m_CameraMng.GetCameraPtr()->SetLowLimitHeight( vCameraPos.y+10.0f );

		//## �� �κ��� �������� ������Ų��.
		//m_CameraMng.GetCameraPtr()->SetPosition( vCameraPos );

		//## ī�޶� update
		m_CameraMng.Update(fDeltaTime, ptCursor);

		//####################################################
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//## projection ���
		m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

		//## view ���
		m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );

		//## view ���
		//D3DXMATRIXA16 matWorld;
		//m_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );

		//## view * projection
		matViewProj = matView * matProj;

		//## frustum update
		m_Frustum.Update(matViewProj);
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//####################################################

		//## ��� flag ����
		m_Octree.CullOutsideFrustum( m_Frustum );
		//m_Octree.CullInsideOcclusion( &matView, &vCameraPos );
	}
}


void CGameWorldManager::RenderGameWorld()
{
	if( m_bRenderingSwitch )
	{
		m_pd3dDevice->SetFVF( FVF );
		if( m_bOctreeBbox )
            m_Octree.RenderBbox(m_pd3dDevice);

		m_LandscapeSceneMng.Render();
	}
}