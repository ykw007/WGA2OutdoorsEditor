#pragma once
#include "picker.h"
#include "octree.h"
#include "patch.h"


//####################################################
// 옥트리에서 picking 처리 클래스
//####################################################
class AFX_EXT_CLASS COctreePicker : public CPicker
{
protected:
	COctree					*m_pOctree;

	D3DXVECTOR3				m_vMousePos;
	D3DXVECTOR3				m_vCenter;		//## heightmap 기준 중심 index
	S_OCT_NODE				*m_pCurOctNode;
public:
	COctreePicker(void);
	~COctreePicker(void);

	COctree *GetCurOctTreePtr(){ return m_pOctree; }
	S_OCT_NODE *GetCurOctNodePtr(){ return m_pCurOctNode; }
	D3DXVECTOR3 &GetMousePos(){ return m_vMousePos; }

	void SetOctree(COctree *pOctree){ m_pOctree = pOctree; }

	//## picking 된 면을 찾아낸다.
	bool FindFacePicked(POINT &ptCursor, UINT uiTarget);
};
