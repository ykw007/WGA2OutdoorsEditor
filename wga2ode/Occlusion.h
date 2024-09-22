#ifndef _OCCLUSION_H_
#define _OCCLUSION_H_

#include "misc.h"

class COcclusion
{
public:
	COcclusion(void);
	~COcclusion(void);

	void SetPoint( D3DXVECTOR3 vP[] )
	{
		m_vP[0] = vP[0]; 
		m_vP[1] = vP[1]; 
		m_vP[2] = vP[2]; 
		m_vP[3] = vP[3]; 
	}

	void SetTransPos(D3DXMATRIXA16 *pmatTransform);
    void SetupOcclusion( D3DXMATRIXA16 *pViewTransform );
	void Draw(LPDIRECT3DDEVICE9 pd3dDevice);
private:
	D3DXVECTOR3				m_vP[4];
	float					m_fMinZ;
	D3DXPLANE				m_FrontPlane;
	D3DXPLANE				m_FirstPlane;
	D3DXPLANE				m_SecondPlane;
	D3DXPLANE				m_ThirdPlane;
	D3DXPLANE				m_FourthPlane;

	D3DXVECTOR3				m_vTP[4];

};

#endif