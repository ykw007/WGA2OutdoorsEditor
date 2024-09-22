#ifndef _TERRAINEDITINGMNG_H_
#define _TERRAINEDITINGMNG_H_

#include ".\WGA2ode\terrain.h"
#include "terrainheightworker.h"
#include "terraintextureworker.h"
#include "terrainobjectworker.h"

#include "terraineditarea.h"
#include "terrainmodeditarea.h"

//##################################################################
//## editor가 추가되면 여기에 mode 추가
//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//## DETAIL EDITOR 종류
enum E_TERRAIN_MODE { 
						T_HEIGHT = 0,
						T_TEXTURE,
						T_OBJECT,
						//T_... ,	<< 추가
                        T_NUM_MODE
};


//####################################################
//####################################################
// CTerrainEditingMng
// 현재 툴에서 지형 수정에 관한 처리를 맡는 클래스
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
	CTerrainHeightWorker			m_HeightWorker;		//## 지형 높이 작업자
	CTerrainTextureWorker			m_TexWorker;		//## 텍스쳐 작업자
	CTerrainObjectWorker			m_ObjWorker;		//## 텍스쳐 작업자

	CTerrain						*m_pTerrain;
	COctree							*m_pOctree;

	UINT							m_uiCurMode;

	HWND							m_hWnd;

	CTerrainModEditArea				m_ModEditArea;
	CTerrainEditArea				m_TexEditArea;
	CTerrainEditArea				m_ObjEditArea;
};

#endif