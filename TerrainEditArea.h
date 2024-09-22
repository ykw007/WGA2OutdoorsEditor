#ifndef _TERRAINEDITAREA_H_
#define _TERRAINEDITAREA_H_

#include ".\wga2ode\terrain.h"
#include ".\wga2ode\octreepicker.h"

#define AREA_BASE_HEIGHT	2.0f
//##########################################################
// 영역 안에 들어가는 정점을 말한다.
//##########################################################
struct S_MAPPOINT_INAREA
{
	S_MAP_POINT			*pMapPoint;
	float				fDistPow;			//## 거리 제곱
};

//##########################################################
//##########################################################
// terrain 편집 영역 클래스
// COctreePicker에서 상속 받는다.
//##########################################################
//##########################################################
class CTerrainEditArea : public COctreePicker
{
public:
	CTerrainEditArea(void);
	~CTerrainEditArea(void);


	//## GET
	//###################################################################
	S_INTERSECTION *GetIntersectionPtr(){ return &m_Intersection; }
	S_MAPPOINT_INAREA *GetSelectedMapPointsPtr(){ return m_aSelected; }
	int GetSelectedMapPointCount(){ return m_nMapPointCounter; }
	CTerrain *GetTerrainPtr(){ return m_pTerrain; }
	bool IsFixed(){ return m_bFixed; }
	///UINT GetNumPointPerSide(){ return m_nNumPointPerSide; }

	//## SET
	//###################################################################
	void SetAreaHalfSize(int nHalfSize)
	{
		m_nHalfSize = nHalfSize;
	}
	void SetTerrain(CTerrain *pTerrain){ m_pTerrain = pTerrain; }

	void SetTargetType(int nType){ m_nTargetType = nType; }

	//## FUNCTIONS
	//###################################################################
	void Clear();
	void Destroy();
	bool Update(POINT &ptCursor);
	virtual void Show();

protected:
	//## 참조할 terrain
	CTerrain							*m_pTerrain;

	//## 편집 영역을 표시하는 원이나 사각형의 정점들을 담아두는 배열
	LINEVERTEX							m_aArea[10000];

	int									m_nHalfSize;

	//## 해당 영역에 있는 정점들을 담아두는 배열
	S_MAPPOINT_INAREA					m_aSelected[10000];

	//## 영역 안의 MAP POINT 카운터
	int									m_nMapPointCounter;
	POINT								m_ptPrev;
	D3DXVECTOR3							m_vPrevMousePos;
	D3DXMATRIXA16						m_matPrevView;

	bool								m_bFixed;			//## 영역이 고정되었나?

	int									m_nTargetType;
	//int									m_nNumPointPerSide;
};

#endif