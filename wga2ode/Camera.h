#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "misc.h"

// 카메라 최초 위치
#define START_CAMERA_POSITION		D3DXVECTOR3(50.0f,20.0f,-150.0f)

struct CameraControl
{
	char key;
	float x,y,z;
};


//####################################################
//####################################################
// 카메라 class
//####################################################
//####################################################
class CCamera
{
	D3DXVECTOR3		m_vPosition;
	D3DXMATRIXA16	m_matOrientation;
	D3DXMATRIXA16	m_matView;

	D3DXVECTOR3		m_vVelocity;
	float			m_fPitch, m_fYaw, m_fRoll;
	float			m_fPitchVelocity, m_fYawVelocity, m_fRollVelocity;

	float			m_fSpeed, m_fAngularSpeed;

	float						m_fLowLimitHeight;
public:
	CCamera(void);
	~CCamera(void);

	void Clear();

	D3DXMATRIXA16 &GetViewMatrix(){ return m_matView; }
	D3DXVECTOR3 &GetPosition(){ return m_vPosition; }
	float &GetPitch(){ return m_fPitch; }

	void SetLowLimitHeight(float fHeight){ m_fLowLimitHeight = fHeight; }
	void SetViewMatrix(D3DXMATRIXA16 &mat){ m_matView = mat; }
	void SetPosition(D3DXVECTOR3 &vPos){ m_vPosition = vPos; }

	void AddToVelocity(D3DXVECTOR3 &vVec);
	void AddToYawPitchRoll(float fYaw, float fPitch, float fRoll);

	void Update(float &fDeltaTime);
};


//####################################################
//####################################################
// 카메라 관리자 class
//####################################################
//####################################################
class CCameraMng
{
    LPDIRECT3DDEVICE9			m_pd3dDevice;
	//HWND						m_hWnd;

	CCamera						*m_pCamera;

	//float						m_fSpinX, m_fSpinY;
	CPoint						m_ptLastMousePosit;
	CPoint						m_ptCurrentMousePosit;
	bool						m_bMouseControl;

	bool						m_bCameraControl;//## 카메라 컨트롤 가능한가



public:

	CCameraMng();
	~CCameraMng();

	CCamera* GetCameraPtr(){ return m_pCamera; }


	void SetCameraControl(bool bEnabled){ m_bCameraControl = bEnabled; }
	void SetMouseControl(bool bEnabled){ m_bMouseControl = bEnabled; }

	void Init(LPDIRECT3DDEVICE9 pd3dDevice=NULL, HWND hWnd=NULL);
	void Update(float &fDeltaTime,POINT &ptCursor);

	void ControlByKey();
	void ControlByInputDevice(POINT &ptCursor);
};

#endif