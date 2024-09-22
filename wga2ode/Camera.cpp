#include "StdAfx.h"
#include "camera.h"

CCamera::CCamera(void)
{
	Clear();
}

CCamera::~CCamera(void)
{
}

void CCamera::Clear()
{
	m_vVelocity = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_fYaw = m_fPitch = m_fRoll = 0.0f;
	m_fYawVelocity = m_fPitchVelocity = m_fRollVelocity = 0.0f;
	m_vPosition = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXMatrixIdentity(&m_matOrientation);

	m_fSpeed = 1.0f;
	m_fAngularSpeed = 7.0f;
}

void CCamera::AddToVelocity(D3DXVECTOR3 &vVec)
{
	m_vVelocity += vVec;
}

void CCamera::AddToYawPitchRoll(float fYaw, float fPitch, float fRoll)
{
	m_fYawVelocity += fYaw;
	m_fPitchVelocity += fPitch;
	m_fRollVelocity += fRoll;
}

void CCamera::Update(float &fDeltaTime)
{
	FLOAT fTime;
  
	if( fDeltaTime > 0.0f )
		fTime = fDeltaTime;
    else
		fTime = 0.05f;

	FLOAT fSpeed        = m_fSpeed * fTime;
	FLOAT fAngularSpeed = m_fAngularSpeed * fTime;
    
	// Update the position vector
	D3DXVECTOR3 vT = m_vVelocity * fSpeed;
	D3DXVec3TransformNormal( &vT, &vT, &m_matOrientation );
	m_vPosition += vT;
  
	if( m_vPosition.y < m_fLowLimitHeight )
			m_vPosition.y = m_fLowLimitHeight;

	// Update the yaw-pitch-rotation vector
	m_fYaw   += fAngularSpeed * m_fYawVelocity;
	m_fPitch += fAngularSpeed * m_fPitchVelocity;

	if(m_fPitch > 1.57f)
		m_fPitch = 1.57f;

	if(m_fPitch < -1.57f)
		m_fPitch = -1.57f;

	// Set the view matrix
	D3DXQUATERNION qR;
	D3DXQuaternionRotationYawPitchRoll(&qR, m_fYaw, m_fPitch, m_fRoll);
	D3DXMatrixAffineTransformation(&m_matOrientation, 1.25f, NULL, &qR, &m_vPosition );
	D3DXMatrixInverse( &m_matView, NULL, &m_matOrientation );

	// Decelerate only the camera's position velocity (for smooth motion)
	m_vVelocity      *= 0.9f;

	// Deceleating the yaw/pitch/roll velocities works great for keyboard control, 
	// but it gives me motion sickness when used with mouselook, so I commented it out
	// and replaced it with multiplication by zero, which disables the velocities.  
	// Your mileage may vary.

  	/*
	m_fYawVelocity   *= 0.9f;
	m_fPitchVelocity *= 0.9f;
	*/

	m_fYawVelocity   *= 0.0f;
	m_fPitchVelocity *= 0.0f;
}


//#################################################### CCameraMng Methods

const int g_nKey = 6;
CameraControl g_CameraControl[6] = { 
	{ 'D', 50.9f, 0.0f, 0.0f },
	{ 'A', -50.9f, 0.0f, 0.0f },
	{ 'W', 0.0f, 0.0f, 50.9f },
	{ 'S', 0.0f, 0.0f, -50.9f },
	{ VK_UP, 0.0f, 50.9f, 0.0f },
	{ VK_DOWN, 0.0f, -50.9f, 0.0f }
};

CCameraMng::CCameraMng()
{
	m_pd3dDevice = 0;
	//m_hWnd = NULL;
	m_pCamera = 0;
	m_bMouseControl = false;
	m_bCameraControl = false;
}

CCameraMng::~CCameraMng()
{
	SAFE_DELETE( m_pCamera );
}


//####################################################
// 카메라 초기화
//####################################################
void CCameraMng::Init(LPDIRECT3DDEVICE9 pd3dDevice, HWND hWnd)
{
	//## d3d rendering device 연결
	if( m_pd3dDevice == NULL && pd3dDevice != NULL )
	{
		m_pd3dDevice = pd3dDevice;
	}

	//## 카메라가 없으면 카메라 인스턴스 생성
	if( m_pCamera == NULL )
	{
		//SAFE_DELETE( m_pCamera );
		m_pCamera = new CCamera();
	}

	//if( m_hWnd == NULL && hWnd != NULL )
    //    m_hWnd = hWnd;

	//## 카메라 초기 위치
	m_pCamera->Clear();
	m_pCamera->SetPosition(START_CAMERA_POSITION);
}

void CCameraMng::Update(float &fDeltaTime, POINT &ptCursor)
{
	if(m_pCamera)
	{
		// 입력device에 의한 카메라 이동
		ControlByInputDevice(ptCursor);

		m_pCamera->Update(fDeltaTime);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &(m_pCamera->GetViewMatrix()) );
	}
}

void CCameraMng::ControlByInputDevice(POINT &ptCursor)
{
	//## 마우스에 의한 카메라 컨트롤
	float fSpinX = 0.0f;
	float fSpinY = 0.0f;

	m_ptCurrentMousePosit.x = ptCursor.x;
	m_ptCurrentMousePosit.y = ptCursor.y;

	if( m_bMouseControl )
	{
		fSpinX = (m_ptCurrentMousePosit.x - m_ptLastMousePosit.x);
		fSpinY = (m_ptCurrentMousePosit.y - m_ptLastMousePosit.y);
	}

	m_ptLastMousePosit.x = m_ptCurrentMousePosit.x;
	m_ptLastMousePosit.y = m_ptCurrentMousePosit.y;

	if( m_bMouseControl )
		m_pCamera->AddToYawPitchRoll( fSpinX*(M_PI/180), fSpinY*(M_PI/180), 0.0f );

	//## 키에 의한 카메라 컨트롤
	ControlByKey();
}


void CCameraMng::ControlByKey()
{
	for( int i = 0; i < g_nKey; i++ )
	{
		if( GetAsyncKeyState( g_CameraControl[i].key ) != 0 )
		{
			m_pCamera->AddToVelocity(D3DXVECTOR3(	g_CameraControl[i].x,
													g_CameraControl[i].y,
													g_CameraControl[i].z	) );
		}
	}
}