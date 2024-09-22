#include "StdAfx.h"
#include "intersectiontester.h"

CIntersectionTester::CIntersectionTester(void)
{
}

CIntersectionTester::~CIntersectionTester(void)
{
}


//####################################################
// d3d 예제 - 삼각형과 반직선의 교차점 판정 함수
//####################################################
bool CIntersectionTester::TestTriangleNRay(	const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
											D3DXVECTOR3& v0,
											D3DXVECTOR3& v1,
											D3DXVECTOR3& v2,
											FLOAT* t, FLOAT* u, FLOAT* v )
{
    // Find vectors for two edges sharing vert0
    D3DXVECTOR3 edge1 = v1 - v0;
    D3DXVECTOR3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    D3DXVECTOR3 pvec;
    D3DXVec3Cross( &pvec, &dir, &edge2 );

    // If determinant is near zero, ray lies in plane of triangle
    FLOAT det = D3DXVec3Dot( &edge1, &pvec );

    D3DXVECTOR3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f )
        return FALSE;

    // Calculate U parameter and test bounds
    *u = D3DXVec3Dot( &tvec, &pvec );
    if( *u < 0.0f || *u > det )
        return FALSE;

    // Prepare to test V parameter
    D3DXVECTOR3 qvec;
    D3DXVec3Cross( &qvec, &tvec, &edge1 );

    // Calculate V parameter and test bounds
    *v = D3DXVec3Dot( &dir, &qvec );
    if( *v < 0.0f || *u + *v > det )
        return FALSE;

    // Calculate t, scale parameters, ray intersects triangle
    *t = D3DXVec3Dot( &edge2, &qvec );
    FLOAT fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return TRUE;
}


bool CIntersectionTester::TestAABBNRay(	const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
										CAABB &AABB,
										FLOAT* t, FLOAT* u, FLOAT* v )
{
	D3DXVECTOR3 vMax, vMin;
	vMax.x = AABB.m_vPos.x + AABB.m_vExtent.x;
	vMax.y = AABB.m_vPos.y + AABB.m_vExtent.y;
	vMax.z = AABB.m_vPos.z - AABB.m_vExtent.z;

	vMin.x = AABB.m_vPos.x - AABB.m_vExtent.x;
	vMin.y = AABB.m_vPos.y - AABB.m_vExtent.y;
	vMin.z = AABB.m_vPos.z + AABB.m_vExtent.z;

	//## 8개의 점
	D3DXVECTOR3 vP[8];
	vP[0] = vMin;
	vP[1] = D3DXVECTOR3(vMax.x,vMin.y,vMin.z);
	vP[2] = D3DXVECTOR3(vMax.x,vMin.y,vMax.z);
	vP[3] = D3DXVECTOR3(vMin.x,vMin.y,vMax.z);
	vP[4] = D3DXVECTOR3(vMin.x,vMax.y,vMin.z);
	vP[5] = D3DXVECTOR3(vMax.x,vMax.y,vMin.z);
	vP[6] = vMax;
	vP[7] = D3DXVECTOR3(vMin.x,vMax.y,vMax.z);

	//## 12개의 삼각형을 작성한다.
	D3DXVECTOR3 vTri[12][3];

	//## bottom 2개
	vTri[0][0] = vP[0];
	vTri[0][1] = vP[1];
	vTri[0][2] = vP[3];

	vTri[1][0] = vP[3];
	vTri[1][1] = vP[1];
	vTri[1][2] = vP[2];

	//## front 2
	vTri[2][0] = vP[7];
	vTri[2][1] = vP[6];
	vTri[2][2] = vP[3];

	vTri[3][0] = vP[3];
	vTri[3][1] = vP[6];
	vTri[3][2] = vP[2];

	//## top 2
	vTri[4][0] = vP[4];
	vTri[4][1] = vP[5];
	vTri[4][2] = vP[7];

	vTri[5][0] = vP[7];
	vTri[5][1] = vP[5];
	vTri[5][2] = vP[6];

	//## left 2
	vTri[6][0] = vP[4];
	vTri[6][1] = vP[7];
	vTri[6][2] = vP[0];

	vTri[7][0] = vP[0];
	vTri[7][1] = vP[7];
	vTri[7][2] = vP[3];

	//## right 2
	vTri[8][0] = vP[6];
	vTri[8][1] = vP[5];
	vTri[8][2] = vP[2];

	vTri[9][0] = vP[2];
	vTri[9][1] = vP[5];
	vTri[9][2] = vP[1];

	//## back 2
	vTri[10][0] = vP[5];
	vTri[10][1] = vP[4];
	vTri[10][2] = vP[1];

	vTri[11][0] = vP[1];
	vTri[11][1] = vP[4];
	vTri[11][2] = vP[0];


	for(int i=0;i<12;i++)
	{
		if( TestTriangleNRay(	orig, dir, vTri[i][0], vTri[i][1], vTri[i][2],
								t, u, v ) )
		{
			return true;
		}

	}

	return false;
}