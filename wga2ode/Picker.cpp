#include "StdAfx.h"
#include "picker.h"

CPicker::CPicker(void)
{
	m_pd3dDevice = NULL;
	m_hWnd = NULL;
}

CPicker::~CPicker(void)
{
}

//####################################################
// 2d 화면상의 좌표를 입력받아
// 카메라,마우스 위치 월드 좌표
//####################################################
void CPicker::GetRay(	POINT &ptCursor,
						D3DXVECTOR3 *vpPickRayDir,
						D3DXVECTOR3 *vpPickRayOrig	)
{
	//## 입력받은 마우스 좌표를 3d 공간 좌표로 변환

	//## 뷰포트 정보 얻어냄
	D3DVIEWPORT9 vp;
	m_pd3dDevice->GetViewport( &vp );	

	//## 프로젝션 행렬 얻어냄
	D3DXMATRIXA16 matProj;
	m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	//## client 영역 크기를 알아냄
	RECT rt;
	::GetClientRect(m_hWnd, &rt);

	//## 카메라 공간상 좌표로 변환
	D3DXVECTOR3 v;
	v.x = ((  (((ptCursor.x - vp.X)*2.0f/(float)rt.right ) - 1.0f)) - matProj._31 ) / matProj._11;
	v.y = ((- (((ptCursor.y - vp.Y)*2.0f/(float)rt.bottom) - 1.0f)) - matProj._32 ) / matProj._22;
	v.z =  1.0f;

	//## 월드 공간 상의 좌표를 구하기 위해 카메라 행렬의 역행렬을 구함
	D3DXMATRIXA16 matView, matInvView;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
	D3DXMatrixInverse( &matInvView, NULL, &matView );

	//## 카메라 위치와 월드 상의 공간 좌표를 구함
	(*vpPickRayDir).x  = v.x*matInvView._11 + v.y*matInvView._21 + v.z*matInvView._31;
	(*vpPickRayDir).y  = v.x*matInvView._12 + v.y*matInvView._22 + v.z*matInvView._32;
	(*vpPickRayDir).z  = v.x*matInvView._13 + v.y*matInvView._23 + v.z*matInvView._33;
	(*vpPickRayOrig).x = matInvView._41;
	(*vpPickRayOrig).y = matInvView._42;
	(*vpPickRayOrig).z = matInvView._43;
}
