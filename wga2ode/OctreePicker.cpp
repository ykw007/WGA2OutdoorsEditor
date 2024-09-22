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
// pick �� ���� ã�Ƴ���.
//####################################################
bool COctreePicker::FindFacePicked(POINT &ptCursor,UINT uiTarget)
{

	m_bIsPicked = false;

	//## pick ���������� ���� ���� �غ�
    D3DXVECTOR3 vPickRayDir, vPickRayOrig;

	//## ���������� �Ǿ��� �� u, v, t ���� ���� ���� �غ�
	FLOAT fBary1, fBary2;
	FLOAT fDist;

	D3DXVECTOR3 vP[3];

	//## ������ ��
	int nNumIntersection = 0;

	//## �� ��ü�� face ��
	int nNumFaces;

	//## 2d ���콺 ��ǥ�� 3d world ��ǥ ������
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

		//## AABB �� Ray ���� ����
		if( (uiThisType == uiTarget) && 
			m_IntersectionTester.TestAABBNRay(	vPickRayOrig, vPickRayDir,
												thisBbox,
												&fDist, &fBary1, &fBary2 ) )
		{
			nNumFaces = pCurObj->GetMeshPtr()->GetNumFaces();

			S_CUSTOMVERTEX *pVertices = pCurObj->GetMeshPtr()->GetVBPtr();
			S_CUSTOMINDEX *pIndices = pCurObj->GetMeshPtr()->GetIBPtr();

			//## ���� �����Ǿ�����
			//## AABB �ȿ� face loop
			for( int c = 0; c < nNumFaces ; c++ )
			{
				//## 3���� ��
				vP[0] = pVertices[ pIndices[c]._0 ].vP;
				vP[1] = pVertices[ pIndices[c]._1 ].vP;
				vP[2] = pVertices[ pIndices[c]._2 ].vP;

				if( m_IntersectionTester.TestTriangleNRay(	vPickRayOrig, vPickRayDir,
															vP[0], vP[1], vP[2],
															&fDist, &fBary1, &fBary2 ) )
				{

					//## ���� patch ���� �ݴ� ���� �������� ��� 0�̻��� �Ҽ��� ���̳� - ���� ���´�.
					//## �̷� ���� ���� �ʱ� ���� fDist > 1 �� �� �־���.
					if( ( nNumIntersection == 0 || ( m_Intersection.fDist > fDist ) ) &&
						( fDist > 1 ) )
					{
						//## �����Ǿ����� ���� ���� �ӽ� ����

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

			//## ������ �κ��� ������
			if( nNumIntersection > 0 )
			{
				m_bIsPicked = true;

				//## ���콺 ��ǥ
				m_vMousePos = vPickRayOrig + vPickRayDir * m_Intersection.fDist;

				break;
			}
		}
	}


	return m_bIsPicked;
}