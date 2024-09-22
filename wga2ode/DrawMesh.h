#ifndef _DRAWMESH_H_
#define _DRAWMESH_H_

#include "meshobj.h"
#include "aabb.h"

//## ��ü Ÿ�� ( ������ü, ��ֹ� ��ü, ĳ���� ��ü )
enum E_OBJ_TYPE
{
	LAND=1,
	OBSTACLE,
	CHARACTER
};

//##############################################################
// ����� �޽� Ŭ����
// �� Ŭ���������� �⺻ �޽��� �����͸� ������ �ִ�.
// ���� ������ �޽��� ��� �ߺ��Ǵ� ����Ÿ�� ������ �ʱ� ���ؼ�
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
	UINT							m_uiType;	//## �޽� ��ü Ÿ�� : ����, ��ֹ�, ĳ����
	CAABB							m_AABB;
};

#endif