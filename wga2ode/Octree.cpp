#include "StdAfx.h"
#include "octree.h"
#include "base\sorts.h"

//## 옥트리 전체 객체수
//Array<S_VISIBLE_OBJ>		COctree::m_sVisibleObj;
//int							COctree::m_snObjCount = 0;

Array<S_VISIBLE_OBJ>			COctree::m_saVisibleObj(100000);

//## 비교 함수
int CompareNodeObj( S_VISIBLE_OBJ l, S_VISIBLE_OBJ r )
{
	if( l.fDistFromCamera < r.fDistFromCamera )
        return -1;
    if( l.fDistFromCamera > r.fDistFromCamera )
        return 1;

    return 0;
}

COctree::COctree(void)
{
	Clear();
}

COctree::~COctree(void)
{
	Destroy();
}

void COctree::Clear()
{
	m_pObjList = NULL;
	m_pRoot = NULL;
	m_bIsCompleted = false;
}

void COctree::Destroy()
{
	//## 옥트리 모든 OBJECT 삭제
	if( m_pObjList )
	{
		for( ObjListItor itor=m_pObjList->begin(); itor != m_pObjList->end(); ++itor )
		{
			SAFE_DELETE( (S_OCT_OBJ*)(*itor) );
		}
	}

	SAFE_DELETE(m_pObjList);

	SAFE_DELETE(m_pRoot);

	Clear();
}

//####################################################
// octree 생성
//####################################################
bool COctree::Create( int nWidth, D3DXVECTOR3 vCenter, int nLimitWidth )
{
	//## 옥트리가 생성되어 있으면 본 함수의 호출을 받아들이지 않는다.
	if( m_bIsCompleted )
		return false;

	m_nWidth = nWidth;
	int nHalfWidth = nWidth>>1;
	m_nLimitWidth = nLimitWidth;
	m_vCenter = vCenter;

	//## bounding box
	D3DXVECTOR3 vMin, vMax;

	vMin.x = vCenter.x - nHalfWidth;
	vMin.y = vCenter.y - nHalfWidth;
	vMin.z = vCenter.z + nHalfWidth;

	vMax.x = vCenter.x + nHalfWidth;
	vMax.y = vCenter.y + nHalfWidth;
	vMax.z = vCenter.z - nHalfWidth;

	//####################################################
	//##  root node 생성
	//## root node를 생성, 모든 객체들을 담는다.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	S_OCT_NODE *pCurNode = new S_OCT_NODE;
	if( pCurNode == NULL )
		return false;

	//## AABB
	pCurNode->AABB.m_vExtent.x = fabs( (vMax.x - vMin.x)/2 );
	pCurNode->AABB.m_vExtent.y = fabs( (vMax.y - vMin.y)/2 );
	pCurNode->AABB.m_vExtent.z = fabs( (vMax.z - vMin.z)/2 );

	pCurNode->AABB.m_vPos.x = vMin.x + pCurNode->AABB.m_vExtent.x;
	pCurNode->AABB.m_vPos.y = vMin.y + pCurNode->AABB.m_vExtent.y;
	pCurNode->AABB.m_vPos.z = vMin.z - pCurNode->AABB.m_vExtent.z;

	//## bbox 설정
	pCurNode->Bbox.vMax = vMax;
	pCurNode->Bbox.vMin = vMin;

	S_OCTNODE_OBJ *pNodeObj;

	pCurNode->pThisNodeObjList = new NodeObjList;

	//## object 를 모두 담는다.
	for( ObjListItor itor=m_pObjList->begin(); itor != m_pObjList->end(); ++itor )
	{
		pNodeObj = new S_OCTNODE_OBJ;
		if( pNodeObj == NULL )
			return false;

		pNodeObj->pOctObj = (S_OCT_OBJ *)(*itor);

		if( ((S_OCT_OBJ *)(*itor))->pObj == NULL )
		{
			pNodeObj->nNumFace = 1;			//## 정적인 객체이면 node 안에 있는 face 수를 담아둔다.
		}

		pCurNode->pThisNodeObjList->push_back( pNodeObj );
	}

	//## 보이는 객체 배열
	//m_sVisibleObj.Resize(nObjCounter);

	//## child 초기화
	for( int i=0; i < 8; i++ )
		pCurNode->pChild[i] = NULL;

	//## neighbor 초기화
	for( int j=0; j < 6; j++ )
		pCurNode->pNeighbor[j] = NULL;

	//## octree root
	m_pRoot = pCurNode;


	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//####################################################

	//## octree 생성 시작
	_BuildOctree( pCurNode );

	m_bIsCompleted = true;

	return m_bIsCompleted;
}

void COctree::_BuildOctree(S_OCT_NODE *pCurNode)
{
	//## bbox의 한변의 길이
    float fWidth = fabs( pCurNode->Bbox.vMax.x - pCurNode->Bbox.vMin.x );

	if( fWidth >= (float)m_nLimitWidth*2 )
	{
		for( int i=0; i < 8; i++ )
		{
			_BuildNode( pCurNode, i );
			if( pCurNode->pChild[i] != NULL )
				_BuildOctree( pCurNode->pChild[i]);
		}
	}

	return;
}

//####################################################
// Octree node 생성
//####################################################
void COctree::_BuildNode( S_OCT_NODE *pParentNode, short sOctNode )
{
	D3DXVECTOR3 vMax, vMin, vParentMax, vParentMin;

	//## node 작성 시작
	vParentMax = pParentNode->Bbox.vMax;
	vParentMin = pParentNode->Bbox.vMin;

    float fWidth = fabs( vParentMax.x - vParentMin.x );

	//## 현재 node size
	float fCurWidth = fWidth / 2;

	//## 어느 위치의 노드인가?
	switch( sOctNode )
	{
	case TOP_LEFT_FRONT :
		vMax.x = vParentMax.x - fCurWidth;
		vMax.y = vParentMax.y;
		vMax.z = vParentMax.z + fCurWidth;		//## d3dx 좌표

		vMin.x = vParentMin.x;
		vMin.y = vParentMin.y + fCurWidth;
		vMin.z = vParentMin.z;	
		break;

	case TOP_RIGHT_FRONT :
		vMax.x = vParentMax.x;
		vMax.y = vParentMax.y;
		vMax.z = vParentMax.z + fCurWidth;		//## d3dx 좌표

		vMin.x = vParentMin.x + fCurWidth;
		vMin.y = vParentMin.y + fCurWidth;
		vMin.z = vParentMin.z;	
		break;

	case TOP_LEFT_BACK :
		vMax.x = vParentMax.x - fCurWidth;
		vMax.y = vParentMax.y;
		vMax.z = vParentMax.z;		//## d3dx 좌표

		vMin.x = vParentMin.x;
		vMin.y = vParentMin.y + fCurWidth;
		vMin.z = vParentMin.z - fCurWidth;	
		break;

	case TOP_RIGHT_BACK :
		vMax = vParentMax;

		vMin.x = vParentMin.x + fCurWidth;
		vMin.y = vParentMin.y + fCurWidth;
		vMin.z = vParentMin.z - fCurWidth;	
		break;

	case BOTTOM_LEFT_FRONT :
		vMax.x = vParentMax.x - fCurWidth;
		vMax.y = vParentMax.y - fCurWidth;
		vMax.z = vParentMax.z + fCurWidth;		//## d3dx 좌표

		vMin = vParentMin;

		break;

	case BOTTOM_RIGHT_FRONT :
		vMax.x = vParentMax.x;
		vMax.y = vParentMax.y - fCurWidth;
		vMax.z = vParentMax.z + fCurWidth;		//## d3dx 좌표

		vMin.x = vParentMin.x + fCurWidth;
		vMin.y = vParentMin.y;
		vMin.z = vParentMin.z;	
		break;

	case BOTTOM_LEFT_BACK :
		vMax.x = vParentMax.x - fCurWidth;
		vMax.y = vParentMax.y - fCurWidth;
		vMax.z = vParentMax.z;		//## d3dx 좌표

		vMin.x = vParentMin.x;
		vMin.y = vParentMin.y;
		vMin.z = vParentMin.z - fCurWidth;	
		break;

	case BOTTOM_RIGHT_BACK :
		vMax.x = vParentMax.x;
		vMax.y = vParentMax.y - fCurWidth;
		vMax.z = vParentMax.z;

		vMin.x = vParentMin.x + fCurWidth;
		vMin.y = vParentMin.y;
		vMin.z = vParentMin.z - fCurWidth;	
		break;
	}

	S_BBOX CurBbox;
	CurBbox.vMax = vMax;
	CurBbox.vMin = vMin;

	//## AABB
	CAABB ThisAABB;
	ThisAABB.m_vExtent.x = fabs( (vMax.x - vMin.x)/2 );
	ThisAABB.m_vExtent.y = fabs( (vMax.y - vMin.y)/2 );
	ThisAABB.m_vExtent.z = fabs( (vMax.z - vMin.z)/2 );

	ThisAABB.m_vPos.x = vMin.x + ThisAABB.m_vExtent.x;
	ThisAABB.m_vPos.y = vMin.y + ThisAABB.m_vExtent.y;
	ThisAABB.m_vPos.z = vMin.z - ThisAABB.m_vExtent.z;

	//## parent node의 object가 현재 node에 하나도 없으면 return
//	S_TRI tri;

	int nObjCount = 0;

	NodeObjList *pThisNodeObjList = NULL;

	for( NodeObjListItor itor=pParentNode->pThisNodeObjList->begin();
		itor!=pParentNode->pThisNodeObjList->end();	++itor )
	{
		S_OCTNODE_OBJ* pCurNodeObj = (S_OCTNODE_OBJ*)(*itor);
		int nNumFace = pCurNodeObj->nNumFace;

		if( nNumFace == 0 )
		{
			CAABB ObjAABB = pCurNodeObj->pOctObj->pObj->GetMeshPtr()->GetAABB();
			
			//## bounding box와 triangle 교차 판정
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			if( ThisAABB.Overlaps( ObjAABB ) )
			{
				if(nObjCount==0)
				{
					pThisNodeObjList = new NodeObjList;
					if( pThisNodeObjList == NULL )
						return;
				}
				S_OCTNODE_OBJ* pNewNodeObj = new S_OCTNODE_OBJ;
				pNewNodeObj->pOctObj = pCurNodeObj->pOctObj;

				pThisNodeObjList->push_back(pNewNodeObj);
				nObjCount++;
			}
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		}
		else
		{
			//## 삼각형 하나씩 테스트
			for( int i=0; i < nNumFace; i++ )
			{

			}
		}
	}

	//## 객체가 없으면 노드를 생성하지 않는다.
	if( nObjCount == 0 )
	{
		delete(pThisNodeObjList);
		return;
	}

	//####################################################
	//##  child node 생성
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	S_OCT_NODE *pCurNode = new S_OCT_NODE;
	pParentNode->pChild[sOctNode] = pCurNode;

	pCurNode->AABB = ThisAABB;

	//## bbox 설정
	pCurNode->Bbox.vMax = vMax;
	pCurNode->Bbox.vMin = vMin;

	//## object list link
	pCurNode->pThisNodeObjList = pThisNodeObjList;

	//## child 초기화
	for( int i=0; i < 8; i++ )
		pCurNode->pChild[i] = NULL;

	//## neighbor 초기화
	for( int j=0; j < 6; j++ )
		pCurNode->pNeighbor[j] = NULL;

	//delete(pThisNodeObjList);
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//####################################################
}

//####################################################
// 옥트리 경계 상자 그리기
//####################################################
void COctree::RenderBbox(LPDIRECT3DDEVICE9 pd3dDevice)
{
	_RecurRendering(pd3dDevice,m_pRoot);
}

void COctree::_RecurRendering(LPDIRECT3DDEVICE9 &pd3dDevice, S_OCT_NODE* pCurNode)
{
	D3DXVECTOR3 vMax = pCurNode->Bbox.vMax;
	D3DXVECTOR3 vMin = pCurNode->Bbox.vMin;

	S_BBOX CurBbox;
	CurBbox.vMax = vMax;
	CurBbox.vMin = vMin;

	//## 현재 노드가 NULL이 아니면
	//## bounding box 출력
	//## 수정 : 여기서 octree node를 출력
	int c = 0;

	if( pCurNode!= NULL )
	{
		for( int i=0; i < 8; i++ )
		{
			if( pCurNode->pChild[i] != NULL )
				_RecurRendering( pd3dDevice, pCurNode->pChild[i] );
			else
				c++;

		}
		if(c==8)
			pCurNode->AABB.RenderBbox(pd3dDevice,0xff0000ff);
	}
}


//####################################################
// 옥트리 culling
//####################################################
void COctree::CullOutsideFrustum(CFrustum &Frustum)
{
	m_nNumVisibleObj = 0;

	_RecurCulling(m_pRoot, Frustum);

	//## 보이는 객체 거리 순위로 quick sort
	QuickSort( m_saVisibleObj, 0, m_nNumVisibleObj, CompareNodeObj );
}

//####################################################
// 옥트리 culling 처리 재귀 함수
//####################################################
void COctree::_RecurCulling(S_OCT_NODE* pCurNode, CFrustum &Frustum)
{
	S_OCTNODE_OBJ *pCurNodeObj;
	D3DXVECTOR3 vCenter, vMax;
	float fRadius, fDistFromCamera;
	int nNumNoChild = 0;
	int nInState;			//## 프러스텀에 포함된 상태 : 모두 포함, 일부 포함, 포함 안함

	//## 현재 노드가 null이면 pass
	if( pCurNode != NULL )
	{
		//## 옥트리 현재 노드 bounding box
		vCenter = pCurNode->AABB.m_vPos;		//## 중심
		vMax.x = vCenter.x + pCurNode->AABB.m_vExtent.x;
		vMax.y = vCenter.y + pCurNode->AABB.m_vExtent.y;
		vMax.z = vCenter.z - pCurNode->AABB.m_vExtent.z;

		//## 경계구 반지름
		fRadius = D3DXVec3Length( &(vMax-vCenter) );

		//## 경계구 테스트
        nInState = Frustum.TestBoundingSphere( &vCenter, fRadius );

		//## 포함안함이면 모두 포함이거나 일부 포함 상태일 것이다.
		if( nInState != CFrustum::ALL_OUT )
		{
			//## 일부 포함 상태이면 child를 조사한다.
			if( nInState == CFrustum::SOME_IN )
			{
				for( int i=0; i < 8; i++ )
				{
					if( pCurNode->pChild[i] != NULL && 
						pCurNode->pChild[i]->pThisNodeObjList != NULL )
						_RecurCulling( pCurNode->pChild[i], Frustum );
					else
						nNumNoChild++;		//## null인 child 수가 얼마나 되는지
				}
			}

			//## 모두 포함이거나 null child 수가 8이면 ( 즉 leaf node이면 ) 보이는 객체로 판정
			if( nInState == CFrustum::ALL_IN || nNumNoChild==8 )
			{

				for( NodeObjListItor itor=pCurNode->pThisNodeObjList->begin();
					itor != pCurNode->pThisNodeObjList->end(); ++itor )
				{
					pCurNodeObj = (S_OCTNODE_OBJ*)(*itor);

					//## 거리를 계산
					fDistFromCamera = D3DXVec3Length( &(Frustum.GetViewPosition() - 
						pCurNodeObj->pOctObj->pObj->GetMeshPtr()->GetAABB().m_vPos) );

					m_saVisibleObj[m_nNumVisibleObj].pOctNode = pCurNode;
					m_saVisibleObj[m_nNumVisibleObj].pOctNodeObj = pCurNodeObj;
					m_saVisibleObj[m_nNumVisibleObj].fDistFromCamera = fDistFromCamera;

					pCurNodeObj->pOctObj->pObj->SetRenderSw(true);

					m_nNumVisibleObj++;
				}
			}

		}//## if( nInState != CFrustum::ALL_OUT )
		else
		{
			for( NodeObjListItor itor=pCurNode->pThisNodeObjList->begin();
				itor != pCurNode->pThisNodeObjList->end(); ++itor )
			{
				pCurNodeObj = (S_OCTNODE_OBJ*)(*itor);
				pCurNodeObj->pOctObj->pObj->SetRenderSw(false);
			}
		}

	}//## if( pCurNode != NULL )


}



void COctree::CullInsideOcclusion( D3DXMATRIXA16 *pViewTransform, D3DXVECTOR3 *pvCameraPos )
{

//	for( int i = 0; i < m_snObjCount; i++ )
	//{
		//m_sVisibleObj[i]->pOctObj->pObj->GetOcclusionPtr()->SetupOcclusion( pViewTransform );
	//}

}

void COctree::AddObjInNode( S_OCT_NODE *pOctNode, S_DRAW_MODEL *pDrawModel, int nObjType )
{
	S_OCT_OBJ *pCurOctObj;
	int nNumMtrl = pDrawModel->nNumMtrl;
	S_DRAW_MTRLMESH *pDrawMtrlMesh = pDrawModel->pMtrlMesh;

	for( int i = 0; i < nNumMtrl; i++ )
	{
		for( int j = 0; j < (pDrawMtrlMesh+i)->nNumMesh; j++ )
		{
			pCurOctObj = new S_OCT_OBJ;
			pCurOctObj->pObj = (pDrawMtrlMesh+i)->pMesh;
			(pDrawMtrlMesh+i)->pMesh->SetMeshObjType( nObjType );

			//## octree object list에 추가
			m_pObjList->push_back(pCurOctObj);

			S_OCTNODE_OBJ *pCurOctNodeObj = new S_OCTNODE_OBJ;
			pCurOctNodeObj->pOctObj = pCurOctObj;

			//## 노드에 추가
            pOctNode->pThisNodeObjList->push_back( pCurOctNodeObj );
		}
	}
}