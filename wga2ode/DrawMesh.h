#ifndef _DRAWMESH_H_
#define _DRAWMESH_H_

#include "meshobj.h"
#include "aabb.h"

//## 객체 타입 ( 지형객체, 장애물 객체, 캐릭터 객체 )
enum E_OBJ_TYPE
{
	LAND=1,
	OBSTACLE,
	CHARACTER
};

//##############################################################
// 출력할 메시 클래스
// 이 클래스에서는 기본 메시의 포인터를 가지고 있다.
// 같은 형태의 메시인 경우 중복되는 데이타를 가지지 않기 위해서
//##############################################################
class CDrawMesh
{
public:
	CDrawMesh(void);
	~CDrawMesh(void);

	void SetMesh(CMeshObj *pMesh){ m_pMesh = pMesh; }
	void SetRenderSw(bool bRenderSw){ m_bRenderSw = bRenderSw; }
	void SetMeshObjType(int nType){ m_uiType = nType; }

	CAABB &GetAABB(){ return m_AABB; }
	CMeshObj *GetMeshPtr(){ return m_pMesh; }
	bool GetRenderSw(){ return m_bRenderSw; }
	UINT GetMeshObjType(){ return m_uiType; }

protected:
	CMeshObj						*m_pMesh;
	bool							m_bRenderSw;
	UINT							m_uiType;	//## 메시 객체 타입 : 지형, 장애물, 캐릭터
	CAABB							m_AABB;
};

#endif