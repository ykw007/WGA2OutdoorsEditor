#ifndef _TERRAINEDITINGMNG_H_
#define _TERRAINEDITINGMNG_H_

#include ".\WGA2ode\terrain.h"
#include "terrainheightworker.h"
#include "terraintextureworker.h"
#include "terrainobjectworker.h"

#include "terraineditarea.h"
#include "terrainmodeditarea.h"

//##################################################################
//## editor�� �߰��Ǹ� ���⿡ mode �߰�
//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//## DETAIL EDITOR ����
enum E_TERRAIN_MODE { 
						T_HEIGHT = 0,
						T_TEXTURE,
						T_OBJECT,
						//T_... ,	<< �߰�
                        T_NUM_MODE
};


//####################################################
//####################################################
// CTerrainEditingMng
// ���� ������ ���� ������ ���� ó���� �ô� Ŭ����
//####################################################
//####################################################
class CTerrainEditingMng
{

public:
	CTerrainEditingMng(void);
	~CTerrainEditingMng(void);

	CTerrainHeightWorker *GetHeightWorkerPtr(){ return &m_HeightWorker; }
	CTerrainTextureWorker *GetTextureWorkerPtr(){ return &m_TexWorker; }
	CTerrainObjectWorker *GetObjectWorkerPtr(){ return &m_ObjWorker; }

	CTerrainModEditArea *GetModEditAreaPtr(){ return &m_ModEditArea; }
	CTerrainEditArea *GetTexEditAreaPtr(){ return &m_TexEditArea; }
	CTerrainEditArea *GetObjEditAreaPtr(){ return &m_ObjEditArea; }

	UINT GetCurEditMode(){ return m_uiCurMode; }

	void Clear();

	void Init(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd, CTerrain *pTerrain, COctree *pOctree);
	void Update(POINT &ptCursor);

	void InitEditMode(UINT uiMode);
	void ClearEditMode();

private:
	CTerrainHeightWorker			m_HeightWorker;		//## ���� ���� �۾���
	CTerrainTextureWorker			m_TexWorker;		//## �ؽ��� �۾���
	CTerrainObjectWorker			m_ObjWorker;		//## �ؽ��� �۾���

	CTerrain						*m_pTerrain;
	COctree							*m_pOctree;

	UINT							m_uiCurMode;

	HWND							m_hWnd;

	CTerrainModEditArea				m_ModEditArea;
	CTerrainEditArea				m_TexEditArea;
	CTerrainEditArea				m_ObjEditArea;
};

#endif