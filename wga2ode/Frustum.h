#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_

#include "misc.h"
#include "aabb.h"
#include "intersectiontester.h"

//## ���Ƿ�
#define PLANE_EPSILON	0.01f

class CFrustum
{
	D3DXVECTOR3					m_aVertex[8];			//## ���������� ������ ���� 8��
	D3DXPLANE					m_aPlane[6];			//## 6���� ���

public:
	//## frustum sides
	enum E_SIDE_TYPE
	{
		 SNR,
		 SFR,
		 SLEFT,
		 SRIGHT,
		 STOP,
		 SBOTTOM
	};

	//## ����
	enum E_TEST_RESULT
	{
		ALL_IN, SOME_IN, ALL_OUT
	};

	CFrustum(void);
	~CFrustum(void);


	D3DXVECTOR3 GetViewPosition()
	{
		D3DXVECTOR3 vTemp = (m_aVertex[0] + m_aVertex[5]);
		vTemp.x = vTemp.x /2.0f;
		vTemp.y = vTemp.y /2.0f;
		vTemp.z = vTemp.z /2.0f;

		return vTemp;
	}

	int TestBoundingSphere(D3DXVECTOR3 *pCenter, float fRadius);
	void Update(D3DXMATRIXA16 &matViewProj);
	void Draw(LPDIRECT3DDEVICE9 pd3dDevice);
};

#endif