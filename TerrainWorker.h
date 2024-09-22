#ifndef _TERRAINWORKER_H_
#define _TERRAINWORKER_H_

#include "wga2ode\terrain.h"
#include "terraineditarea.h"



class CTerrainWorker
{
protected:
	CTerrainEditArea				*m_pEditArea;

protected:

	virtual void Update()=0;

public:

	CTerrainWorker(void);
	virtual ~CTerrainWorker(void);

	void SetEditAreaPtr(CTerrainEditArea *pEditArea)
	{
		if( m_pEditArea == NULL )
            m_pEditArea = pEditArea;

		m_pEditArea->ClearPicking();
	}
	void Work(POINT &ptCursor);
};

#endif