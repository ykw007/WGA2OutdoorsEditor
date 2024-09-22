#include "StdAfx.h"
#include "terrainworker.h"

CTerrainWorker::CTerrainWorker(void)
{
	m_pEditArea = NULL;
}

CTerrainWorker::~CTerrainWorker(void)
{
}


void CTerrainWorker::Work(POINT &ptCursor)
{
	//## 편집 영역 update
	if( !m_pEditArea->Update(ptCursor) )
		return;

	//## update 순수가상
	Update();
}

