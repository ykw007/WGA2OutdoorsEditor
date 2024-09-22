#include "StdAfx.h"
#include "patch.h"
#include "terrain.h"
#include "dib.h"

//## observer
#include "observer.h"

//#######################################################################################
//#######################################################################################
//#######################################################################################
//## CHeightmap 구현부
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
// 높이 데이타 작성
//####################################################
bool CHeightmap::Build(BYTE ucTerrainType, float fMapSize, int nCellSpace, LPCTSTR lpHeightMapPath)
{
	CDib Heightmap;

	//## 높이맵 파일이 있으면 CDib 클래스 사용 높이맵 파일 정보 읽어옴
	if( ucTerrainType==HEIGHTMAP && lpHeightMapPath != NULL )
	{
		//## height map 파일 로드
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

	//## 높이 데이타 저장을 위한 메모리 할당
	SAFE_DELETE_ARRAY(m_pfHeightData);
	m_pfHeightData = new float[ m_uiNumPixel * m_uiNumPixel ];

	if( m_pfHeightData == NULL )
	{
		g_pObserver->Display( 1, "##CHeightmap Heightmap buffer 생성 실패##", 0 );
		SAFE_DELETE_ARRAY( m_pfHeightData );
		return false;
	}

	UINT x, z;
	float fHeightValue;

	for (z = 0; z < m_uiNumPixel; z++)
	{
		for (x = 0; x < m_uiNumPixel; x++)
		{
			//## 최초 지형 생성 타입에 따라 높이 데이타를 지정
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
//## CPatch 구현부

//## 정적 변수들
LPDIRECT3DDEVICE9	CPatch::m_pd3dDevice = NULL;		//## patch들이 공유하게될 d3d device

int					CPatch::m_snCellSpace;
int					CPatch::m_snPatchSpace;
int					CPatch::m_snNumCells;					//## patch side 당 cell 수
int					CPatch::m_snNumVertices;				//## vertex 수
CHeightmap			*CPatch::m_spHeightmap = NULL;

float				CPatch::m_sfMaxHeight = 0;
float				CPatch::m_sfMinHeight = 0;

CPatch::CPatch(void)
{
	m_nBeginX = 0;
	m_nBeginZ = 0;
	m_nEndX = 0;
	m_nEndZ = 0;

	m_fMaxHeight = -10000;	//## patch의 최대 높이
	m_fMinHeight = 10000;	//## patch의 최저 높이

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

	//## patch 메시는 여기서 제거
	SAFE_DELETE( m_pMesh );
}


//####################################################
// 렌더링 디바이스 초기화
//####################################################
void CPatch::InitDevice(LPDIRECT3DDEVICE9 pd3dDevice)
{
	if( m_pd3dDevice == NULL )
        m_pd3dDevice = pd3dDevice;
}


//####################################################
//## 모든 patch가 공유할 항목 초기화
//####################################################
void CPatch::InitShareEntry( CHeightmap *pHeightmap,
							 int nPatchSpace, int nCellSpace,
							 int nNumCells, int nNumVertices )
{
	m_spHeightmap = pHeightmap;
	m_snPatchSpace = nPatchSpace;
	m_snCellSpace = nCellSpace;
	m_snNumCells = nNumCells;			//## patch side 당 cell 수
	m_snNumVertices = nNumVertices;		//## vertex 수
}

//####################################################
//## patch 초기화
//####################################################
void CPatch::Init( int nBeginX, int nBeginZ, int nEndX, int nEndZ, int nTexId )
{
	//## 타입 설정
	m_uiType = LAND;

	//## 시작 위치, 끝 위치 설정
	m_nBeginX = nBeginX;
	m_nBeginZ = nBeginZ;
	m_nEndX = nEndX;
	m_nEndZ = nEndZ;

	//## patch에 적용될 텍스쳐 id
	m_nTexId[0] = nTexId;

	//################################################
	// patch 생성 - vb, ib 작성
	Create();

	//## 최고, 최저 높이 
	GetMaxMinHeight();
}


//####################################################
//## 지형 patch vb,ib 작성
//####################################################
void CPatch::Create()
{
	//## mesh 생성
	m_pMesh = new CMeshObj;

	_BuildVB();
	_BuildIB();
}

//####################################################
// 정점 작성
//####################################################
void CPatch::_BuildVB()
{
	//############################################################################

	//## patch mesh vertex를 생성합니다.
	//## 생성할 buffer 크기
	int nVertexBufferSize = m_snNumVertices * m_snNumVertices;
	S_CUSTOMVERTEX *pVertices = new S_CUSTOMVERTEX[ nVertexBufferSize ];

	S_CUSTOMVERTEX v;

	int nNumVertices = 0;

	UINT uiMapSize = m_spHeightmap->GetMapSize();

	//## vertex buffer에 vertex 저장
	//## 좌표 생성시 CELL SIZE 를 지정한다.
	int x, z;
	for( z = -m_nBeginZ; z <= -m_nEndZ; z++ )
	{
		for( x = m_nBeginX; x <= m_nEndX; x++ )
		{
			//## vertex
			v.vP.x = (float)x * m_snCellSpace;
			v.vP.z = -(float)z * m_snCellSpace;
			v.vP.y = m_spHeightmap->GetHeight( x, z );

			//## normal 작성
			BuildNormal( v.vP.x, v.vP.z, &(v.vN) );

			//## texture 좌표
			v.fTu = (float)(abs(m_nBeginX - x))/(float)(abs(m_nBeginX - m_nEndX));
			v.fTv = (float)(abs(m_nBeginZ + z))/(float)(abs(m_nBeginX - m_nEndX));
			
			pVertices[ nNumVertices ] = v;

			//## MAP POINT 데이타 작성
			//## 여기서 TERRAIN의 MAP POINT를 참조하여 작성한다.
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

			for( int i = 0; i < 4; i++ )
			{
				S_CUSTOMVERTEX *pTemp = 
					CTerrain::m_spMapPatch[x+z*(uiMapSize+1)].paMapPoint[i];

				//## NULL인 요소만 채워 넣는다.
				if( pTemp == NULL )
				{
                    CTerrain::m_spMapPatch[x+z*(uiMapSize+1)].paMapPoint[i] = 
															(pVertices+nNumVertices);

					CTerrain::m_spMapPatch[x+z*(uiMapSize+1)].pPatch[i] = this;

					break;
				}
			}

			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//## MAP POINT 데이타 작성

			nNumVertices++;
		}
	}

	//## mesh vertex data 저장
	m_pMesh->SetVertices( pVertices );
	m_pMesh->SetNumVertices( nNumVertices );
}

//####################################################
// index 작성
//####################################################
void CPatch::_BuildIB()
{
	int nNumFaces = 0;
	int nIndicesSize = (m_snNumVertices-1)*(m_snNumVertices-1)*2;
	S_CUSTOMINDEX *pIndices = new S_CUSTOMINDEX[nIndicesSize];

	//m_nNumFaces = 0;
	int nVertexSize = (m_nEndX - m_nBeginX) + 1;

	S_CUSTOMINDEX	i;

	//## index buffer에 index 저장
	for (int z = 0; z < nVertexSize-1 ; z++)
	{
		for (int x = 0; x < nVertexSize-1 ; x++)
		{
			i._0 = ( (z * nVertexSize) + x );
			i._1 = ( (z * nVertexSize) + x + 1 );
			i._2 = ( ((z + 1) * nVertexSize) + x );

			pIndices[nNumFaces++] = i;

			//CTerrain::m_snNumFaces++;		//## 총 face 수

			i._0 = ( ((z + 1) * nVertexSize) + x );
			i._1 = ( (z * nVertexSize) + x + 1 );
			i._2 = ( ((z + 1) * nVertexSize) + x + 1 );

			pIndices[nNumFaces++] = i;

			//CTerrain::m_snNumFaces++;		//## 총 face 수
		}
	}

	//## mesh index data 저장
	m_pMesh->SetIndices( pIndices );
	m_pMesh->SetNumFaces( nNumFaces );
}

//####################################################
// normal 작성
//####################################################
void CPatch::BuildNormal(int nX, int nZ, D3DXVECTOR3 *pvN)
{
	nX /= m_snCellSpace;
	nZ /= m_snCellSpace;

	//## normal
	//## 정리할 것..............>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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
// 경계상자를 만들기 위한 최대, 최소 높이
//####################################################
void CPatch::GetMaxMinHeight()
{
	float fHeight;

	//## vertex buffer에 vertex 저장
	//## 좌표 생성시 CELL SIZE 를 지정한다.
	for(int z = -m_nBeginZ; z <= -m_nEndZ; z++)
	{
		for(int x = m_nBeginX; x <= m_nEndX; x++)
		{
			fHeight = m_spHeightmap->GetHeight(x,z);

			m_fMaxHeight = MAX(m_fMaxHeight,fHeight);		//## patch의 최대 높이
			m_sfMaxHeight = MAX(m_sfMaxHeight,m_fMaxHeight);	//## 전체 patch 최대 높이

			m_fMinHeight = MIN(m_fMinHeight,fHeight);		//## patch의 최저 높이
			m_sfMinHeight = MIN(m_sfMinHeight,m_fMinHeight);	//## 전체 patch 최저 높이
		}
	}

	//###############################################################################
	//## 여기서 AABB 설정
	CAABB &ThisAABB = m_pMesh->GetAABB();
	ThisAABB.m_vExtent.x = abs( (m_nEndX*m_snCellSpace - m_nBeginX*m_snCellSpace)>>1 );
	ThisAABB.m_vExtent.y = fabs( ( m_fMaxHeight - m_fMinHeight )/2.0f );
	ThisAABB.m_vExtent.z = abs( (m_nEndZ*m_snCellSpace - m_nBeginZ*m_snCellSpace)>>1 );

	ThisAABB.m_vPos.x = m_nBeginX*m_snCellSpace + ThisAABB.m_vExtent.x;
	ThisAABB.m_vPos.y = m_fMinHeight + ThisAABB.m_vExtent.y;
	ThisAABB.m_vPos.z = m_nBeginZ*m_snCellSpace - ThisAABB.m_vExtent.z;
}



//########################################################################
// 경계 상자 그리기
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
//## bbox 조정
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


