#include "StdAfx.h"
#include "patch.h"
#include "terrain.h"
#include "dib.h"

//## observer
#include "observer.h"

//#######################################################################################
//#######################################################################################
//#######################################################################################
//## CHeightmap ������
CHeightmap::CHeightmap()
{
	m_pfHeightData = NULL;

	m_fMapSize = 0;
	m_uiNumPixel = 0;
}

CHeightmap::~CHeightmap()
{
	SAFE_DELETE_ARRAY(m_pfHeightData);
}


inline void CHeightmap::SetHeight(int nX, int nZ, float fY)
{
	m_pfHeightData[ nX + ( nZ * m_uiNumPixel ) ] = fY;
}

inline float CHeightmap::GetHeight(UINT uiX, UINT uiZ)
{
	return m_pfHeightData[ abs(uiX) + ( abs(uiZ) * m_uiNumPixel ) ];
}

inline float CHeightmap::GetMapSize()
{
	return m_fMapSize; 
}

//####################################################
// ���� ����Ÿ �ۼ�
//####################################################
bool CHeightmap::Build(BYTE ucTerrainType, float fMapSize, int nCellSpace, LPCTSTR lpHeightMapPath)
{
	CDib Heightmap;

	//## ���̸� ������ ������ CDib Ŭ���� ��� ���̸� ���� ���� �о��
	if( ucTerrainType==HEIGHTMAP && lpHeightMapPath != NULL )
	{
		//## height map ���� �ε�
		if( !Heightmap.LoadHandle(lpHeightMapPath) )
			return false;

		m_uiNumPixel = Heightmap.GetWidth();
		m_fMapSize = m_uiNumPixel - 1;
	}
	else
	{
		m_uiNumPixel = fMapSize + 1;
		m_fMapSize = fMapSize;
	}

	//## ���� ����Ÿ ������ ���� �޸� �Ҵ�
	SAFE_DELETE_ARRAY(m_pfHeightData);
	m_pfHeightData = new float[ m_uiNumPixel * m_uiNumPixel ];

	if( m_pfHeightData == NULL )
	{
		g_pObserver->Display( 1, "##CHeightmap Heightmap buffer ���� ����##", 0 );
		SAFE_DELETE_ARRAY( m_pfHeightData );
		return false;
	}

	UINT x, z;
	float fHeightValue;

	for (z = 0; z < m_uiNumPixel; z++)
	{
		for (x = 0; x < m_uiNumPixel; x++)
		{
			//## ���� ���� ���� Ÿ�Կ� ���� ���� ����Ÿ�� ����
			switch( ucTerrainType )
			{
			case FLATLAND:
				fHeightValue = 0;
				break;
			case HEIGHTMAP:
				fHeightValue = (float)(*(Heightmap.GetPixelColorPointer(x,z) ))*((float)nCellSpace/10.0f);
				break;
			case RANDOM:
				fHeightValue = 0;
				break;
			}

			m_pfHeightData[ (z * m_uiNumPixel) + x ] = fHeightValue;
		}
	}

	return true;
}



//#######################################################################################
//#######################################################################################
//#######################################################################################
//## CPatch ������

//## ���� ������
LPDIRECT3DDEVICE9	CPatch::m_pd3dDevice = NULL;		//## patch���� �����ϰԵ� d3d device

int					CPatch::m_snCellSpace;
int					CPatch::m_snPatchSpace;
int					CPatch::m_snNumCells;					//## patch side �� cell ��
int					CPatch::m_snNumVertices;				//## vertex ��
CHeightmap			*CPatch::m_spHeightmap = NULL;

float				CPatch::m_sfMaxHeight = 0;
float				CPatch::m_sfMinHeight = 0;

CPatch::CPatch(void)
{
	m_nBeginX = 0;
	m_nBeginZ = 0;
	m_nEndX = 0;
	m_nEndZ = 0;

	m_fMaxHeight = -10000;	//## patch�� �ִ� ����
	m_fMinHeight = 10000;	//## patch�� ���� ����

	m_bRenderSw = false;

	m_nTexId[0] = -1;
	m_nTexId[1] = -1;
	m_nTexId[2] = -1;
}

CPatch::~CPatch(void)
{
	m_sfMaxHeight = 0;
	m_sfMinHeight = 0;

	SAFE_DELETE( m_spHeightmap );

	//## patch �޽ô� ���⼭ ����
	SAFE_DELETE( m_pMesh );
}


//####################################################
// ������ ����̽� �ʱ�ȭ
//####################################################
void CPatch::InitDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if( m_pd3dDevice == NULL )
        m_pd3dDevice = pd3dDevice;
}


//####################################################
//## ��� patch�� ������ �׸� �ʱ�ȭ
//####################################################
void CPatch::InitShareEntry( CHeightmap *pHeightmap,
							 int nPatchSpace, int nCellSpace,
							 int nNumCells, int nNumVertices )
{
	m_spHeightmap = pHeightmap;
	m_snPatchSpace = nPatchSpace;
	m_snCellSpace = nCellSpace;
	m_snNumCells = nNumCells;			//## patch side �� cell ��
	m_snNumVertices = nNumVertices;		//## vertex ��
}

//####################################################
//## patch �ʱ�ȭ
//####################################################
void CPatch::Init( int nBeginX, int nBeginZ, int nEndX, int nEndZ, int nTexId )
{
	//## Ÿ�� ����
	m_uiType = LAND;

	//## ���� ��ġ, �� ��ġ ����
	m_nBeginX = nBeginX;
	m_nBeginZ = nBeginZ;
	m_nEndX = nEndX;
	m_nEndZ = nEndZ;

	//## patch�� ����� �ؽ��� id
	m_nTexId[0] = nTexId;

	//################################################
	// patch ���� - vb, ib �ۼ�
	Create();

	//## �ְ�, ���� ���� 
	GetMaxMinHeight();
}


//####################################################
//## ���� patch vb,ib �ۼ�
//####################################################
void CPatch::Create()
{
	//## mesh ����
	m_pMesh = new CMeshObj;

	_BuildVB();
	_BuildIB();
}

//####################################################
// ���� �ۼ�
//####################################################
void CPatch::_BuildVB()
{
	//############################################################################

	//## patch mesh vertex�� �����մϴ�.
	//## ������ buffer ũ��
	int nVertexBufferSize = m_snNumVertices * m_snNumVertices;
	S_CUSTOMVERTEX *pVertices = new S_CUSTOMVERTEX[ nVertexBufferSize ];

	S_CUSTOMVERTEX v;

	int nNumVertices = 0;

	UINT uiMapSize = m_spHeightmap->GetMapSize();

	//## vertex buffer�� vertex ����
	//## ��ǥ ������ CELL SIZE �� �����Ѵ�.
	int x, z;
	for( z = -m_nBeginZ; z <= -m_nEndZ; z++ )
	{
		for( x = m_nBeginX; x <= m_nEndX; x++ )
		{
			//## vertex
			v.vP.x = (float)x * m_snCellSpace;
			v.vP.z = -(float)z * m_snCellSpace;
			v.vP.y = m_spHeightmap->GetHeight( x, z );

			//## normal �ۼ�
			BuildNormal( v.vP.x, v.vP.z, &(v.vN) );

			//## texture ��ǥ
			v.fTu = (float)(abs(m_nBeginX - x))/(float)(abs(m_nBeginX - m_nEndX));
			v.fTv = (float)(abs(m_nBeginZ + z))/(float)(abs(m_nBeginX - m_nEndX));
			
			pVertices[ nNumVertices ] = v;

			//## MAP POINT ����Ÿ �ۼ�
			//## ���⼭ TERRAIN�� MAP POINT�� �����Ͽ� �ۼ��Ѵ�.
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

			for( int i = 0; i < 4; i++ )
			{
				S_CUSTOMVERTEX *pTemp = 
					CTerrain::m_spMapPatch[x+z*(uiMapSize+1)].paMapPoint[i];

				//## NULL�� ��Ҹ� ä�� �ִ´�.
				if( pTemp == NULL )
				{
                    CTerrain::m_spMapPatch[x+z*(uiMapSize+1)].paMapPoint[i] = 
															(pVertices+nNumVertices);

					CTerrain::m_spMapPatch[x+z*(uiMapSize+1)].pPatch[i] = this;

					break;
				}
			}

			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//## MAP POINT ����Ÿ �ۼ�

			nNumVertices++;
		}
	}

	//## mesh vertex data ����
	m_pMesh->SetVertices( pVertices );
	m_pMesh->SetNumVertices( nNumVertices );
}

//####################################################
// index �ۼ�
//####################################################
void CPatch::_BuildIB()
{
	int nNumFaces = 0;
	int nIndicesSize = (m_snNumVertices-1)*(m_snNumVertices-1)*2;
	S_CUSTOMINDEX *pIndices = new S_CUSTOMINDEX[nIndicesSize];

	//m_nNumFaces = 0;
	int nVertexSize = (m_nEndX - m_nBeginX) + 1;

	S_CUSTOMINDEX	i;

	//## index buffer�� index ����
	for (int z = 0; z < nVertexSize-1 ; z++)
	{
		for (int x = 0; x < nVertexSize-1 ; x++)
		{
			i._0 = ( (z * nVertexSize) + x );
			i._1 = ( (z * nVertexSize) + x + 1 );
			i._2 = ( ((z + 1) * nVertexSize) + x );

			pIndices[nNumFaces++] = i;

			//CTerrain::m_snNumFaces++;		//## �� face ��

			i._0 = ( ((z + 1) * nVertexSize) + x );
			i._1 = ( (z * nVertexSize) + x + 1 );
			i._2 = ( ((z + 1) * nVertexSize) + x + 1 );

			pIndices[nNumFaces++] = i;

			//CTerrain::m_snNumFaces++;		//## �� face ��
		}
	}

	//## mesh index data ����
	m_pMesh->SetIndices( pIndices );
	m_pMesh->SetNumFaces( nNumFaces );
}

//####################################################
// normal �ۼ�
//####################################################
void CPatch::BuildNormal(int nX, int nZ, D3DXVECTOR3 *pvN)
{
	nX /= m_snCellSpace;
	nZ /= m_snCellSpace;

	//## normal
	//## ������ ��..............>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	float fHeightA = m_spHeightmap->GetHeight(nX, nZ);
	float fHeightB = m_spHeightmap->GetHeight(nX, nZ+1);
	float fHeightC = m_spHeightmap->GetHeight(nX+1, nZ);


	// build two vectors on the quad
	D3DXVECTOR3 vU(m_snCellSpace*2.0f/5.0f, fHeightB - fHeightA, 0.0f);
	D3DXVECTOR3 vV(0.0f, fHeightC - fHeightA, -m_snCellSpace*2.0f/5.0f);

	D3DXVECTOR3 n;
	D3DXVec3Cross(&n, &vU, &vV);
	D3DXVec3Normalize(&n, &n);

	n.y *= -1.0f;

	*pvN = n;
}

//####################################################
// �����ڸ� ����� ���� �ִ�, �ּ� ����
//####################################################
void CPatch::GetMaxMinHeight()
{
	float fHeight;

	//## vertex buffer�� vertex ����
	//## ��ǥ ������ CELL SIZE �� �����Ѵ�.
	for(int z = -m_nBeginZ; z <= -m_nEndZ; z++)
	{
		for(int x = m_nBeginX; x <= m_nEndX; x++)
		{
			fHeight = m_spHeightmap->GetHeight(x,z);

			m_fMaxHeight = MAX(m_fMaxHeight,fHeight);		//## patch�� �ִ� ����
			m_sfMaxHeight = MAX(m_sfMaxHeight,m_fMaxHeight);	//## ��ü patch �ִ� ����

			m_fMinHeight = MIN(m_fMinHeight,fHeight);		//## patch�� ���� ����
			m_sfMinHeight = MIN(m_sfMinHeight,m_fMinHeight);	//## ��ü patch ���� ����
		}
	}

	//###############################################################################
	//## ���⼭ AABB ����
	CAABB &ThisAABB = m_pMesh->GetAABB();
	ThisAABB.m_vExtent.x = abs( (m_nEndX*m_snCellSpace - m_nBeginX*m_snCellSpace)>>1 );
	ThisAABB.m_vExtent.y = fabs( ( m_fMaxHeight - m_fMinHeight )/2.0f );
	ThisAABB.m_vExtent.z = abs( (m_nEndZ*m_snCellSpace - m_nBeginZ*m_snCellSpace)>>1 );

	ThisAABB.m_vPos.x = m_nBeginX*m_snCellSpace + ThisAABB.m_vExtent.x;
	ThisAABB.m_vPos.y = m_fMinHeight + ThisAABB.m_vExtent.y;
	ThisAABB.m_vPos.z = m_nBeginZ*m_snCellSpace - ThisAABB.m_vExtent.z;
}



//########################################################################
// ��� ���� �׸���
//########################################################################
void CPatch::RenderBbox(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pMesh->GetAABB().RenderBbox(pd3dDevice,0xffffffff);
	//m_Occlusion.Draw( pd3dDevice );
}

void CPatch::AdjustHeightmap(float fX, float fZ, float fY)
{
	m_spHeightmap->SetHeight( (int)fX/m_snCellSpace, (int)fZ/m_snCellSpace, fY );
}


//#######################################################################
//## bbox ����
//#######################################################################
void CPatch::AdjustBbox()
{
	float fCurY;

	for( int i = 0; i < m_pMesh->GetNumVertices(); i++ )
	{
		fCurY = (m_pMesh->GetVBPtr()+i)->vP.y;

		if( i == 0 )
		{
			m_fMaxHeight = m_fMinHeight = fCurY;
		}
		else
		{
			if( m_fMaxHeight < fCurY )
				m_fMaxHeight = fCurY;

			if( m_fMinHeight > fCurY )
				m_fMinHeight = fCurY;

			if( m_sfMaxHeight < fCurY )
				m_sfMaxHeight = fCurY;

			if( m_sfMinHeight > fCurY )
				m_sfMinHeight = fCurY;
		}
	}

	//## AABB update
	m_pMesh->GetAABB().m_vExtent.y = fabs( ( m_fMaxHeight - m_fMinHeight )/2.0f );
	m_pMesh->GetAABB().m_vPos.y = m_fMinHeight + m_pMesh->GetAABB().m_vExtent.y;

	//D3DXVECTOR3 vOPos[4];
	//vOPos[0] = D3DXVECTOR3( m_AABB.m_vPos.x-m_AABB.m_vExtent.x,
	//	m_AABB.m_vPos.y+m_AABB.m_vExtent.y, m_AABB.m_vPos.z );
	//vOPos[1] = D3DXVECTOR3( m_AABB.m_vPos.x+m_AABB.m_vExtent.x,
	//	m_AABB.m_vPos.y+m_AABB.m_vExtent.y, m_AABB.m_vPos.z );
	//vOPos[2] = D3DXVECTOR3( m_AABB.m_vPos.x-m_AABB.m_vExtent.x,
	//	m_AABB.m_vPos.y-m_AABB.m_vExtent.y, m_AABB.m_vPos.z );
	//vOPos[3] = D3DXVECTOR3( m_AABB.m_vPos.x+m_AABB.m_vExtent.x,
	//	m_AABB.m_vPos.y-m_AABB.m_vExtent.y, m_AABB.m_vPos.z );

	//m_Occlusion.SetPoint( vOPos );
}


