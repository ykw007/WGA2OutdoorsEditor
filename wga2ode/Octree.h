#ifndef _OCTREE_H_
#define _OCTREE_H_

#include <list>
#include <vector>
#include "drawmesh.h"
#include "aabb.h"
#include "frustum.h"
#include "base\array.h"
#include "model.h"
#include "modelmanager.h"

using namespace std;

enum E_OCTNODE
{
	TOP_LEFT_FRONT,
	TOP_RIGHT_FRONT,
	TOP_LEFT_BACK,
	TOP_RIGHT_BACK,
	BOTTOM_LEFT_FRONT,
	BOTTOM_RIGHT_FRONT,
	BOTTOM_LEFT_BACK,
	BOTTOM_RIGHT_BACK
};

//struct S_TRI
//{
//	D3DXVECTOR3 vP[3];
//};

//####################################################
// S_OCT_OBJ
// 옥트리에 적용할 객체 정보
//####################################################
struct S_OCT_OBJ
{
	CDrawMesh		*pObj;				//## 동적인 객체인 경우

	S_OCT_OBJ()
	{
		pObj = NULL; 
	}
};

typedef list<S_OCT_OBJ*>		ObjList;
typedef	ObjList::iterator		ObjListItor;

//####################################################
// S_OCTNODE_OBJ
// 해당 노드의 object 정보
//####################################################
struct S_OCTNODE_OBJ
{
	S_OCT_OBJ			*pOctObj;

	S_CUSTOMINDEX		*pIndices;			//## index 배열
	int					nNumFace;			//## face 수

	S_OCTNODE_OBJ()
	{
		pOctObj = NULL;
		pIndices = NULL;
		nNumFace = 0;
	}

	~S_OCTNODE_OBJ()
	{
		if(	nNumFace > 1 )
		{
			SAFE_DELETE_ARRAY(pIndices);
		}
		else
		{
			SAFE_DELETE(pIndices);
		}
	}
};

typedef list<S_OCTNODE_OBJ*>		NodeObjList;
typedef	NodeObjList::iterator		NodeObjListItor;

//####################################################
// S_OCT_NODE
//####################################################
struct S_OCT_NODE
{
	CAABB				AABB;
	S_BBOX				Bbox;
	NodeObjList			*pThisNodeObjList;		//## 노드상 객체 리스트
	S_OCT_NODE			*pChild[8];
	S_OCT_NODE			*pNeighbor[6];

	S_OCT_NODE()
	{
		pThisNodeObjList = NULL;
	}

	~S_OCT_NODE()
	{

		for( NodeObjListItor itor=pThisNodeObjList->begin();
			itor != pThisNodeObjList->end(); ++itor )
		{
			SAFE_DELETE( (S_OCTNODE_OBJ*)(*itor) );
		}
		SAFE_DELETE( pThisNodeObjList );

		for(int i=0;i<8;i++)
		{
			SAFE_DELETE(pChild[i]);
		}

		for(int j=0;j<6;j++)
		{
			SAFE_DELETE(pNeighbor[j]);
		}

	}
};

//########################################################################
// 보이는 oct node 객체
//########################################################################
struct S_VISIBLE_OBJ
{
	S_OCT_NODE			*pOctNode;
	S_OCTNODE_OBJ		*pOctNodeObj;

	float				fDistFromCamera;	//## 카메라로부터 거리

	S_VISIBLE_OBJ()
	{
		pOctNode = NULL;
		pOctNodeObj = NULL;
		fDistFromCamera = 0.0f;	//## 카메라로부터 거리
	}
};

//####################################################
//####################################################
// COctree
//####################################################
//####################################################
class AFX_EXT_CLASS COctree
{
private:
	void _BuildOctree( S_OCT_NODE *pCurNode );
	void _BuildNode( S_OCT_NODE *pParentNode, short sOctNode );
	void _RecurRendering(LPDIRECT3DDEVICE9 &pd3dDevice, S_OCT_NODE* pCurNode);
	void _RecurCulling(S_OCT_NODE* pCurNode, CFrustum &Frustum);

public:
	COctree(void);
	~COctree(void);

	void Clear();
	void Destroy();

	static Array<S_VISIBLE_OBJ> *GetVisibleObjPtr(){ return &m_saVisibleObj; }
	int GetNumVisibleObj(){ return m_nNumVisibleObj; }

	void SetOctreeObjListPtr(ObjList *pObjList){ m_pObjList = pObjList; }

	void AddObjInNode( S_OCT_NODE *pOctNode, S_DRAW_MODEL *pDrawModel, int nObjType );

	bool Create( int nWidth, D3DXVECTOR3 vCenter, int nLimitWidth );
	void RenderBbox(LPDIRECT3DDEVICE9 pd3dDevice);
	void CullOutsideFrustum(CFrustum &Frustum);
	void CullInsideOcclusion( D3DXMATRIXA16 *pViewTransform, D3DXVECTOR3 *pvCameraPos );

private:
	ObjList								*m_pObjList;		//## list of all objects
	int									m_nWidth;
	int									m_nLimitWidth;		//## octree 제한 크기
	D3DXVECTOR3							m_vCenter;

	S_OCT_NODE							*m_pRoot;

	bool								m_bIsCompleted;		//## 옥트리 생성 완료되었는가?

	static Array<S_VISIBLE_OBJ>			m_saVisibleObj;		//## 보이는 객체만 저장해둘 배열(거리순으로)
	int									m_nNumVisibleObj;
};

#endif