#ifndef _LANDSCAPESCENEMANAGER_H_
#define _LANDSCAPESCENEMANAGER_H_

#include "misc.h"
#include "terrain.h"
#include "skybox.h"
#include "octree.h"
#include "texturemanager.h"
#include "modelmanager.h"

class AFX_EXT_CLASS CLandscapeSceneManager
{

public:
	CLandscapeSceneManager(void);
	~CLandscapeSceneManager(void);

	void Clear();
	void Destroy();

	void SetRenderingSwitch(bool bSetValue){ m_bRenderingSwitch = bSetValue; }

	CModelManager *GetTerrainObjectMngPtr(){ return &m_ObjMng; }
	bool GetRenderingSwitch(){ return m_bRenderingSwitch; }
	CTerrain *GetTerrainPtr(){ return m_pTerrain; }

	//## 위치를 지형 위로 조정한다.
	void AdjustVec3OverLand(D3DXVECTOR3 *pvPos);

	//## FOG
	bool SetupFogEffect();

	//## 초기화
	void Init(CTextureManager *pTextureMng, LPDIRECT3DDEVICE9 pd3dDevice=NULL);

	//## 지형 생성
	bool CreateTerrain( int nTerrainType,
						int nMapSize, int nNumPatchesPerSide, int nCellSpace,
						LPCTSTR lpHeightMapPath, LPCTSTR lpTexturePath  );

	void DestroyTerrain(){ SAFE_DELETE( m_pTerrain ); }

	//## skybox 생성
	void CreateSkybox(){ m_pSkybox = new CSkybox(); }
	void Render();							//## 렌더링

private:
    LPDIRECT3DDEVICE9			m_pd3dDevice;			//## The D3D rendering device
	CTextureManager				*m_pTextureMng;			//## texture 관리자

	ID3DXEffect*				m_pFogEffect;
	D3DXHANDLE					m_FogHandle;

	CTerrain					*m_pTerrain;		//## 지형
	CSkybox						*m_pSkybox;			//## SKYBOX
	
	CModelManager				m_ObjMng;			//## 지형지물 모델 관리자

	bool						m_bRenderingSwitch;
};

#endif