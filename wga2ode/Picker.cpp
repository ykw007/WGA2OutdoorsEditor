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
// 2d ȭ����� ��ǥ�� �Է¹޾�
// ī�޶�,���콺 ��ġ ���� ��ǥ
//####################################################
void CPicker::GetRay(	POINT &ptCursor,
						D3DXVECTOR3 *vpPickRayDir,
						D3DXVECTOR3 *vpPickRayOrig	)
{
	//## �Է¹��� ���콺 ��ǥ�� 3d ���� ��ǥ�� ��ȯ

	//## ����Ʈ ���� ��
	D3DVIEWPORT9 vp;
	m_pd3dDevice->GetViewport( &vp );	

	//## �������� ��� ��
	D3DXMATRIXA16 matProj;
	m_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj );

	//## client ���� ũ�⸦ �˾Ƴ�
	RECT rt;
	::GetClientRect(m_hWnd, &rt);

	//## ī�޶� ������ ��ǥ�� ��ȯ
	D3DXVECTOR3 v;
	v.x = ((  (((ptCursor.x - vp.X)*2.0f/(float)rt.right ) - 1.0f)) - matProj._31 ) / matProj._11;
	v.y = ((- (((ptCursor.y - vp.Y)*2.0f/(float)rt.bottom) - 1.0f)) - matProj._32 ) / matProj._22;
	v.z =  1.0f;

	//## ���� ���� ���� ��ǥ�� ���ϱ� ���� ī�޶� ����� ������� ����
	D3DXMATRIXA16 matView, matInvView;
	m_pd3dDevice->GetTransform( D3DTS_VIEW, &matView );
	D3DXMatrixInverse( &matInvView, NULL, &matView );

	//## ī�޶� ��ġ�� ���� ���� ���� ��ǥ�� ����
	(*vpPickRayDir).x  = v.x*matInvView._11 + v.y*matInvView._21 + v.z*matInvView._31;
	(*vpPickRayDir).y  = v.x*matInvView._12 + v.y*matInvView._22 + v.z*matInvView._32;
	(*vpPickRayDir).z  = v.x*matInvView._13 + v.y*matInvView._23 + v.z*matInvView._33;
	(*vpPickRayOrig).x = matInvView._41;
	(*vpPickRayOrig).y = matInvView._42;
	(*vpPickRayOrig).z = matInvView._43;
}
