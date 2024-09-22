#include "StdAfx.h"
#include "octreepicker.h"
#include "base\array.h"

#include "observer.h"

COctreePicker::COctreePicker(void)
{
	m_pOctree = NULL;
	m_bIsPicked = false;
	m_pCurOctNode = NULL;
}

COctreePicker::~COctreePicker(void)
{
}

//####################################################
// pick 된 면을 찾아낸다.
//####################################################
bool COctreePicker::FindFacePicked(POINT &ptCursor,UINT uiTarget)
{

	m_bIsPicked = false;

	//## pick 교차판정을 위한 변수 준비
    D3DXVECTOR3 vPickRayDir, vPickRayOrig;

	//## 교차판정이 되었을 때 u, v, t 값을 담을 변수 준비
	FLOAT fBary1, fBary2;
	FLOAT fDist;

	D3DXVECTOR3 vP[3];

	//## 교차된 수
	int nNumIntersection = 0;

	//## 각 객체의 face 수
	int nNumFaces;

	//## 2d 마우스 좌표를 3d world 좌표 정보로
	GetRay( ptCursor, &vPickRayDir, &vPickRayOrig );

	CDrawMesh *pCurObj;
	S_OCT_NODE *pCurNode;
	CAABB thisBbox;
	UINT uiThisType;

	Array<S_VISIBLE_OBJ> *pVisibleObj = m_pOctree->GetVisibleObjPtr();
	int nNumNodeObjs = m_pOctree->GetNumVisibleObj();

	for( int i = 0; i < nNumNodeObjs; i++ )
	{
		pCurObj = (*pVisibleObj)[i].pOctNodeObj->pOctObj->pObj;

		pCurNode = (*pVisibleObj)[i].pOctNode;

		uiThisType = pCurObj->GetMeshObjType();

		thisBbox = pCurObj->GetMeshPtr()->GetAABB();

		//## AABB 와 Ray 교차 판정
		if( (uiThisType == uiTarget) && 
			m_IntersectionTester.TestAABBNRay(	vPickRayOrig, vPickRayDir,
												thisBbox,
												&fDist, &fBary1, &fBary2 ) )
		{
			nNumFaces = pCurObj->GetMeshPtr()->GetNumFaces();

			S_CUSTOMVERTEX *pVertices = pCurObj->GetMeshPtr()->GetVBPtr();
			S_CUSTOMINDEX *pIndices = pCurObj->GetMeshPtr()->GetIBPtr();

			//## 교차 판정되었으면
			//## AABB 안에 face loop
			for( int c = 0; c < nNumFaces ; c++ )
			{
				//## 3개의 점
				vP[0] = pVertices[ pIndices[c]._0 ].vP;
				vP[1] = pVertices[ pIndices[c]._1 ].vP;
				vP[2] = pVertices[ pIndices[c]._2 ].vP;

				if( m_IntersectionTester.TestTriangleNRay(	vPickRayOrig, vPickRayDir,
															vP[0], vP[1], vP[2],
															&fDist, &fBary1, &fBary2 ) )
				{

					//## 같은 patch 내에 반대 방향 폴리곤일 경우 0이상의 소수점 값이나 - 값이 나온다.
					//## 이런 값을 받지 않기 위해 fDist > 1 를 해 주었다.
					if( ( nNumIntersection == 0 || ( m_Intersection.fDist > fDist ) ) &&
						( fDist > 1 ) )
					{
						//## 교차되었으면 현재 정보 임시 저장

						m_Intersection.fBary1 = fBary1;
						m_Intersection.fBary2 = fBary2;
						m_Intersection.fDist = fDist;
						m_Intersection.pMeshObj = pCurObj->GetMeshPtr();
						m_Intersection.dwFace = c;
						m_Intersection.uiType = uiTarget;

						m_pCurOctNode = pCurNode;

						nNumIntersection++;
					}
				}
			}

			//## 교차된 부분이 있으면
			if( nNumIntersection > 0 )
			{
				m_bIsPicked = true;

				//## 마우스 좌표
				m_vMousePos = vPickRayOrig + vPickRayDir * m_Intersection.fDist;

				break;
			}
		}
	}


	return m_bIsPicked;
}