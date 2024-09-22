#ifndef _TERRAINEDITAREA_H_
#define _TERRAINEDITAREA_H_

#include ".\wga2ode\terrain.h"
#include ".\wga2ode\octreepicker.h"

#define AREA_BASE_HEIGHT	2.0f
//##########################################################
// ���� �ȿ� ���� ������ ���Ѵ�.
//##########################################################
struct S_MAPPOINT_INAREA
{
	S_MAP_POINT			*pMapPoint;
	float				fDistPow;			//## �Ÿ� ����
};

//##########################################################
//##########################################################
// terrain ���� ���� Ŭ����
// COctreePicker���� ��� �޴´�.
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
	//## ������ terrain
	CTerrain							*m_pTerrain;

	//## ���� ������ ǥ���ϴ� ���̳� �簢���� �������� ��Ƶδ� �迭
	LINEVERTEX							m_aArea[10000];

	int									m_nHalfSize;

	//## �ش� ������ �ִ� �������� ��Ƶδ� �迭
	S_MAPPOINT_INAREA					m_aSelected[10000];

	//## ���� ���� MAP POINT ī����
	int									m_nMapPointCounter;
	POINT								m_ptPrev;
	D3DXVECTOR3							m_vPrevMousePos;
	D3DXMATRIXA16						m_matPrevView;

	bool								m_bFixed;			//## ������ �����Ǿ���?

	int									m_nTargetType;
	//int									m_nNumPointPerSide;
};

#endif