#ifndef _TERRAINHEIGHTWORKER_H_
#define _TERRAINHEIGHTWORKER_H_

#include "terrainworker.h"



//## 지형 높이 작업자
class CTerrainHeightWorker : public CTerrainWorker
{
public:

	CTerrainHeightWorker(void);
	~CTerrainHeightWorker(void);

	void SetLimitHeight(int nHeight){ m_nLimitHeight = nHeight; }
	void SetUpdate(bool bUpdate){ m_bUpdate = bUpdate; }
	void SetSineHeight(float fHeight){ m_fSineHeight = fHeight; }
	void ToggleSerialUpdate(){ m_bSerialUpdate = !m_bSerialUpdate; }
	void SetHeightAdjustment(UINT uiType){ m_uiHeightAdjustment = uiType; }
	void SetUpdateType(int nUpdateType){ m_nUpdateType = nUpdateType; }
	void SetUpdateSpeed(int nUpdateSpeed){ m_nUpdateSpeed = nUpdateSpeed; }

	void Update();

private:
	UINT						m_uiHeightAdjustment;	//## 지형 고도 조정 모드

	bool						m_bSerialUpdate;
	int							m_nUpdateType;
	int							m_nUpdateSpeed;
	int							m_nLimitHeight;

	float						m_fSineHeight;

	bool						m_bUpdate;
	bool						m_bUpdateStart;
	float						m_faFinalHeight[1000];//## 제한 높이 최종 계산 높이값
};

#endif