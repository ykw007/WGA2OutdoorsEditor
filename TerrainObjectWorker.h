#ifndef _TERRAINOBJECTWORKER_H_
#define _TERRAINOBJECTWORKER_H_

#include "terrainworker.h"
#include "wga2ode\model.h"


enum E_OBJECT_EDIT_MODE
{
	ADD_OBJ, DEL_OBJ, MOD_OBJ
};


enum E_OBJECT_MODIFY_MODE
{
	TRANS_OBJ, ROT_OBJ, SCALE_OBJ
};


class CTerrainObjectWorker : public CTerrainWorker
{
public:
	CTerrainObjectWorker(void);
	~CTerrainObjectWorker(void);

	CModel *GetTerrainObjectPtr(){ return m_pTerrainObj; }

	void SetTerrainObject( CModel *pModel ){ m_pTerrainObj = pModel; }
	void SetCurWorkingMode( SHORT sMode ){ m_sCurMode = sMode; }

	void WorkThis();

	void AddObject();
	void DelObject();

	void Update();

private:
	CModel				*m_pTerrainObj;
	SHORT				m_sCurMode;
};

#endif