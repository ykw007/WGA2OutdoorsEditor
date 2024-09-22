#ifndef _PICKER_H_
#define _PICKER_H_

#include "intersectiontester.h"

//####################################################
// Picker 弃府帮 急琶 贸府 包府磊
//####################################################
#include "misc.h"
#include "meshobj.h"

struct S_INTERSECTION
{
	FLOAT			fBary1, fBary2;		// barycentric coords of intersection
	FLOAT			fDist;				// distance from ray origin to intersection
	CMeshObj		*pMeshObj;
	UINT			uiType;
	DWORD			dwFace;				// mesh face that was intersected 
	
};


class CPicker
{
protected:
    LPDIRECT3DDEVICE9			m_pd3dDevice;			//## The D3D rendering device
	HWND						m_hWnd;

	CIntersectionTester			m_IntersectionTester;

	bool						m_bIsPicked;
	S_INTERSECTION				m_Intersection;

public:
	CPicker(void);
	~CPicker(void);

	void ClearPicking(){ m_bIsPicked = false; }

	void Init( LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd )
	{
		if(m_pd3dDevice==NULL)
            m_pd3dDevice = pd3dDevice;

		if(m_hWnd==NULL)
			m_hWnd = hWnd;
	}

	bool IsPicked(){ return m_bIsPicked; }
	void GetRay(POINT &ptCursor,D3DXVECTOR3 *vpPickRayDir, D3DXVECTOR3 *vpPickRayOrig);

	//virtual void FindFacePicked(POINT &ptCursor)=0;
};

#endif