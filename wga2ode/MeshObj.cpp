#include "StdAfx.h"
#include "MeshObj.h"

CMeshObj::CMeshObj(void)
{
	m_pVertices = NULL;
	m_pIndices = NULL;

	D3DXMatrixIdentity(&m_matTm);
	D3DXMatrixIdentity(&m_matInvTm);

}

CMeshObj::~CMeshObj(void)
{
	SAFE_DELETE_ARRAY(m_pVertices);
	SAFE_DELETE_ARRAY(m_pIndices);
}

void CMeshObj::SetBbox( D3DXVECTOR3 *pvMax, D3DXVECTOR3 *pvMin )
{
	m_AABB.SetBbox( pvMax, pvMin );
}