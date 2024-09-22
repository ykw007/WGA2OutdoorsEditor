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


//## 수정 필요
void CGameWorldManager::InitGameWorld(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	if(m_pd3dDevice==NULL)
	{
		m_pd3dDevice = pd3dDevice;
	}

	if( m_hWnd == NULL )
		m_hWnd = hWnd;


	//## 카메라 초기화
	m_CameraMng.Init(pd3dDevice,hWnd);

	//## texture manager
	m_TextureMng.Init( pd3dDevice );

	//## 지형 관리자 초기화
	//## TEXTURE 관리자 pointer를 넘겨준다.
	m_LandscapeSceneMng.Init( &m_TextureMng, pd3dDevice );
}

//####################################################
// octree 생성
//####################################################
void CGameWorldManager::CreateGameWorld()
{
	CTerrain *pTerrain = m_LandscapeSceneMng.GetTerrainPtr();
	CPatch *pPatch = pTerrain->GetPatchBeginPtr();

	//####################################################
	//## octree object list를 작성한다.
	//## octree object는 octree에 담을 수 있는 공통 객체이다.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	ObjList *pOctObjList = new ObjList;

	int nNumPatchesPerSide = pTerrain->GetNumPatchesPerSide();
	int nNumAllPatches = nNumPatchesPerSide*nNumPatchesPerSide;

	//## 지형 각 patch 및 객체를 해당 octree object list에 할당
	//## octree에 적용할 object 구조체
	S_OCT_OBJ *pOctObj;

	for( int i = 0; i < nNumAllPatches; i++ )
	{
		pOctObj = new S_OCT_OBJ;
		pOctObj->pObj = (CDrawMesh *)(pPatch+i);

		pOctObjList->push_back( pOctObj );
	}

	//## octree object list pointer를 넘겨준다.
	m_Octree.SetOctreeObjListPtr(pOctObjList);

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//####################################################

	//####################################################
	//## octree 생성
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## octree 생성시 기초 설정값

	//## octree 노드 제한 크기
	int nLeafNodeSize = pPatch->GetPatchSpace();

	//## 생성된 지형의 중심 구하기
	D3DXVECTOR3 vCenter;
	vCenter.x = pTerrain->GetEdgeSpace()/2;
	vCenter.z = -vCenter.x;
	vCenter.y = abs( pPatch->GetMostMaxHeight() - pPatch->GetMostMinHeight() )/2;

	//## octree bounding box 한 변의 크기 
	int nWidth = MAX( vCenter.x*2, vCenter.y*2 );

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//## 옥트리를 생성한다.
	//if( 

	m_Octree.Create( nWidth, vCenter, nLeafNodeSize );

	//)
	//{
		//## 옥트리 연결
		//m_Picker.SetOctree(&m_Octree);
	//}
	
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//####################################################

	//## 카메라를 지형 중심에
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

		//## 카메라 높이를 지형 위로 고정
		m_LandscapeSceneMng.AdjustVec3OverLand( &vCameraPos );
		m_CameraMng.GetCameraPtr()->SetLowLimitHeight( vCameraPos.y+10.0f );

		//## 이 부분은 땅쪽으로 고정시킨다.
		//m_CameraMng.GetCameraPtr()->SetPosition( vCameraPos );

		//## 카메라 update
		m_CameraMng.Update(fDeltaTime, ptCursor);

		//####################################################
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//## projection 행렬
		m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

		//## view 행렬
		m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );

		//## view 행렬
		//D3DXMATRIXA16 matWorld;
		//m_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );

		//## view * projection
		matViewProj = matView * matProj;

		//## frustum update
		m_Frustum.Update(matViewProj);
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//####################################################

		//## 출력 flag 설정
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