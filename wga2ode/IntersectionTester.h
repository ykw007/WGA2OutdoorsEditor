#ifndef _INTERSECTIONTESTER_H_
#define _INTERSECTIONTESTER_H_

#include "misc.h"
#include "aabb.h"

//## 교차 판정자
class CIntersectionTester
{
public:
	CIntersectionTester(void);
	~CIntersectionTester(void);

	bool TestTriangleNRay(	const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
							D3DXVECTOR3& v0,
							D3DXVECTOR3& v1, D3DXVECTOR3& v2,
							FLOAT* t, FLOAT* u, FLOAT* v );

	bool TestAABBNRay(	const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
						CAABB &AABB,
                        FLOAT* t, FLOAT* u, FLOAT* v );
};

#endif