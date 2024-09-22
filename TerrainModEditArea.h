#ifndef _TERRAINMODEDITAREA_H_
#define _TERRAINMODEDITAREA_H_

#include "terraineditarea.h"

//## 지형 고도 편집 타입
enum E_HEIGHT_ADJUSTMENT
{
	UP, DOWN, FLAT
};

class CTerrainModEditArea : public CTerrainEditArea
{
	UINT						m_uiHeightAdjustment;
	UINT						m_uiHeight;

public:
	CTerrainModEditArea(void);
	~CTerrainModEditArea(void);

	void SetHeightAdjustment(UINT uiType);
	void SetHeight(UINT uiHeight){ m_uiHeight = uiHeight; }

	void Show();
};

#endif