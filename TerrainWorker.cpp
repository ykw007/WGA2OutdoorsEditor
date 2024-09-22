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
	//## ���� ���� update
	if( !m_pEditArea->Update(ptCursor) )
		return;

	//## update ��������
	Update();
}

