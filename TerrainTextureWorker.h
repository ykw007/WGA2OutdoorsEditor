#ifndef _TERRAINTEXTUREWORKER_H_
#define _TERRAINTEXTUREWORKER_H_

#include "terrainworker.h"

class CTerrainTextureWorker : public CTerrainWorker
{

public:
	CTerrainTextureWorker(void);
	~CTerrainTextureWorker(void);
	
	void Update();
};

#endif