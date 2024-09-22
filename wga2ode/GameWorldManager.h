//####################################################
//####################################################
// CGameWorldManager
//####################################################
//####################################################
#ifndef _GAMEWORLDMANAGER_H_
#define _GAMEWORLDMANAGER_H_

#include "landscapescenemanager.h"
#include "charactermanager.h"

#include "octree.h"
#include "camera.h"
#include "frustum.h"


#include "octreepicker.h"

class AFX_EXT_CLASS CGameWorldManager
{
public:
	CGameWorldManager(void);
	~CGameWorldManager(void);

	void Clear();
	void Destroy();
	
	void SetHwnd(HWND hWnd){ m_hWnd = hWnd; }
	void SetRenderingSwitch(bool bSwitch){ m_bRenderingSwitch = bSwitch; }
	void ToggleOctreeBboxRenderSwitch(){ m_bOctreeBbox = !m_bOctreeBbox; }

	bool GetOctreeBboxRenderSwitch(){ return m_bOctreeBbox; }
	bool GetRenderingSwitch(){ return m_bRenderingSwitch; }
	CLandscapeSceneManager *GetLandscapeSceneMngPtr(){ return &m_LandscapeSceneMng; }
	CCameraMng *GetCameraMngPtr(){ return &m_CameraMng; }
	COctree *GetOctreePtr(){ return &m_Octree; }

	void InitGameWorld(LPDIRECT3DDEVICE9 pd3dDevice=NULL, HWND hWnd=NULL);
	void CreateGameWorld();
	void UpdateGameWorld(float &fDeltaTime, POINT &ptCursor);
	void RenderGameWorld();

private:
    LPDIRECT3DDEVICE9				m_pd3dDevice;			//## The D3D rendering device
	HWND							m_hWnd;
	COctree							m_Octree;				//## game world를 octree로 나눈다.

	CTextureManager					m_TextureMng;			//## texture 관리자
	CLandscapeSceneManager			m_LandscapeSceneMng;

	CCharacterManager				m_CharacterMng;
	CCameraMng						m_CameraMng;
	CFrustum						m_Frustum;
	
	bool							m_bRenderingSwitch;
	bool							m_bOctreeBbox;
};

#endif