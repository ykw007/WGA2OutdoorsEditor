#ifndef _AABB_H_
#define _AABB_H_

// An axis-aligned bounding box
#include "misc.h"

typedef D3DXVECTOR3 Vector3;

class AFX_EXT_CLASS CAABB
{
public:
	Vector3			m_vPos; //position
	Vector3			m_vExtent; //x,y,z extents

	CAABB(){}
	CAABB( const Vector3 &vPos, const Vector3 &vExtent ): m_vPos(vPos), m_vExtent(vExtent)
	{}

	void SetBbox( D3DXVECTOR3 *pvMax, D3DXVECTOR3 *pvMin )
	{
		m_vPos.x = (pvMax->x + pvMin->x) / 2.0f;
		m_vPos.y = (pvMax->y + pvMin->y) / 2.0f;
		m_vPos.z = (pvMax->z + pvMin->z) / 2.0f;

		m_vExtent.x = (pvMax->x - pvMin->x) / 2.0f;
		m_vExtent.y = (pvMax->y - pvMin->y) / 2.0f;
		m_vExtent.z = (pvMax->z - pvMin->z) / 2.0f;
	}

	//## returns true if this is overlapping b
	const bool Overlaps( const CAABB& b ) const
	{
		const Vector3 T = b.m_vPos - m_vPos;//vector from A to B

		bool bx = ( fabs(T.x) < (m_vExtent.x + b.m_vExtent.x) );

		bool by = ( fabs(T.y) <= (m_vExtent.y + b.m_vExtent.y) );

		bool bz = ( fabs(T.z) < (m_vExtent.z + b.m_vExtent.z) );

		return (bx && by && bz);
	}

	void operator=(const CAABB &b)
	{
		m_vPos = b.m_vPos;
		m_vExtent = b.m_vExtent;
	}

	void RenderBbox(LPDIRECT3DDEVICE9 pd3dDevice, DWORD dwColor);
};

#endif