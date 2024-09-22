#include "StdAfx.h"
#include "terrain.h"
#include "base\utility.h"
#include "observer.h"

//####################################################
//####################################################
//## CTerrain 구현부


int				CTerrain::m_snNumFaces = 0;
S_MAP_POINT		*CTerrain::m_spMapPatch = NULL;

CTerrain::CTerrain(void)
{
    m_pd3dDevice = NULL;
	Clear();
}

CTerrain::~CTerrain(void)
{
	Destroy();
}

void CTerrain::Clear()
{
	m_bPatchBbox = false;
	m_nNumPatchesPerSide = 0;
	m_pPatch = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
	m_pTextureMng = NULL;
}

void CTerrain::Destroy()
{
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
	SAFE_DELETE_ARRAY(m_pPatch);
	SAFE_DELETE_ARRAY(m_spMapPatch);
	Clear();
}


//####################################################
// vertex 버퍼 생성
//####################################################
HRESULT CTerrain::_BuildVB(int nVertexBufferSize)
{
	//## vertex buffer 생성 
	if( FAILED( m_pd3dDevice->CreateVertexBuffer(	nVertexBufferSize * sizeof(S_CUSTOMVERTEX),
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY ,
													FVF,
													D3DPOOL_DEFAULT,
													&m_pVB,
													NULL ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}

//####################################################
// index 버퍼 생성
//####################################################
HRESULT CTerrain::_BuildIB(int nIndexBufferSize)
{
	//## index buffer 생성 
	if( FAILED( m_pd3dDevice->CreateIndexBuffer( nIndexBufferSize*sizeof(S_CUSTOMINDEX),
												D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
												D3DFMT_INDEX16,
												D3DPOOL_DEFAULT,
												&m_pIB,
												NULL ) ) )
	{
		return E_FAIL;
	}


	return S_OK;
}

//####################################################
// 지형 생성
// 지형 타입, 크기, 타일 수 , 셀 크기, heightmap 경로
// 타일 텍스쳐 경로 등을 입력 받아
// 지형 vertex, index 작성
//####################################################
bool CTerrain::Create(	int nTerrainType,
						int nMapSize, int nNumPatchesPerSide, int nCellSpace,
						LPCTSTR lpHeightMapPath, LPCTSTR lpTexturePath )
{
	//## terrain side 당 patch 수를 저장한다.
	m_nNumPatchesPerSide = nNumPatchesPerSide;
	
	//## heightmap 데이타를 작성한다.
	CHeightmap *pHeightmap = new CHeightmap;
	if( pHeightmap->Build(nTerrainType, nMapSize, nCellSpace, lpHeightMapPath) )
	{
		//## 실제 지형 가로, 세로 크기
		m_nEdgeSpace = pHeightmap->GetMapSize() * nCellSpace;

		//## patch space를 구한다.
		int nPatchSpace = m_nEdgeSpace / m_nNumPatchesPerSide;

		int nNumCells = nPatchSpace / nCellSpace;
		int nNumVertices = nNumCells + 1;

		//## PATCH 생성
		if( BuildPatches( pHeightmap, nPatchSpace, nCellSpace, nNumCells, nNumVertices,
							lpTexturePath ) )
		{
			int nVertexBufferSize = nNumVertices * nNumVertices;
			int nIndexBufferSize = (nNumVertices-1) * (nNumVertices-1) * 2;

			//## 정점, 인덱스 버퍼 생성
			if( FAILED( _BuildVB( nVertexBufferSize ) ) )
				return false;

			if( FAILED( _BuildIB( nIndexBufferSize ) ) )
				return false;
		}
	}
	else
		return false;

	return true;
}

//####################################################
// 지형 patch를 생성한다.
//####################################################
bool CTerrain::BuildPatches( CHeightmap *pHeightmap,
							 int nPatchSpace, int nCellSpace,
							 int nNumCells, int nNumVertices, LPCTSTR lpTexturePath )
{
	//## heightmap의 mapsize
	float fMapSize = pHeightmap->GetMapSize();

	int nNumAllVertices = (int)(fMapSize+1)*(fMapSize+1);
	//## MAP POINT 메모리 할당
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	//m_spMapPatch = new S_MAP_POINT[ (fMapSize+1)*(fMapSize+1) ];
	m_spMapPatch = new S_MAP_POINT[ nNumAllVertices ];

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## MAP POINT 메모리 할당

	//## 생성해야 할 총 patch 수
	int nNumAllPatches = m_nNumPatchesPerSide*m_nNumPatchesPerSide;

	//## 총 patch 수 만큼 CPatch 메모리 할당
	m_pPatch = new CPatch[ nNumAllPatches ];

	int nPatchTextureId = -1;

	if( m_pPatch != NULL )
	{
		//###################################################################################
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		//## texture 관리자에 의한 텍스쳐 등록
		if( strlen(lpTexturePath) > 0 && m_pTextureMng != NULL )
            nPatchTextureId = m_pTextureMng->RegisterTexture( (char*)lpTexturePath );

		//## patch들이 공유할 렌더링 device 초기화( device 는 static )
		//## 처음 patch만 적용
		//m_pPatch->InitDevice( m_pd3dDevice );

		//## 공유 항목 초기화
		//## cellspace 
		//## 수정 : 공유 항목 추가( patch size, vb, ib 등등)
		m_pPatch->InitShareEntry(pHeightmap, nPatchSpace, nCellSpace, nNumCells, nNumVertices);

		int i, nX, nZ;
		for(int z = 0; z < m_nNumPatchesPerSide; z++)
		{
			for(int x = 0; x < m_nNumPatchesPerSide; x++)
			{
				i = ( z * m_nNumPatchesPerSide ) + x;
				nX = x * nNumCells;
				nZ = z * nNumCells;
				
				//## 각 patch 초기화
				m_pPatch[i].Init(nX, -nZ, nX + nNumCells, -(nZ + nNumCells), nPatchTextureId);
			}
		}
	}

	return true;
}


void CTerrain::Render()
{
	//## vertex buffer 와 연결할 void 포인터
	int nNumVertices, nNumFaces;
	S_CUSTOMVERTEX *pVertices;
	S_CUSTOMINDEX *pIndices;

	m_snNumFaces = 0;
	//int c = 0;

	for( int i = 0; i < m_nNumPatchesPerSide * m_nNumPatchesPerSide; i++ )
	{
		if( m_pPatch[i].GetRenderSw() )
		{
			S_CUSTOMVERTEX *pPatchVB = m_pPatch[i].GetMeshPtr()->GetVBPtr();
			S_CUSTOMINDEX *pPatchIB = m_pPatch[i].GetMeshPtr()->GetIBPtr();
			nNumVertices = m_pPatch[i].GetMeshPtr()->GetNumVertices();
			nNumFaces = m_pPatch[i].GetMeshPtr()->GetNumFaces();

			//## vb 생성을 위한 lock
			if( FAILED( m_pVB->Lock( 0,
									0,
									(void**)&pVertices, D3DLOCK_DISCARD ) ) )
			{
				return ;
			}

			//## IB 생성을 위한 lock
			if( FAILED( m_pIB->Lock( 0,
									0,
									(void**)&pIndices, D3DLOCK_DISCARD ) ) )
			{
				return ;
			}

			memcpy( pVertices ,pPatchVB , sizeof(S_CUSTOMVERTEX)*nNumVertices );
			memcpy( pIndices ,pPatchIB, sizeof(S_CUSTOMINDEX)*nNumFaces );

			//## ib Unlock
			m_pIB->Unlock();

			//## vb Unlock
			m_pVB->Unlock();

			//#############################################################################
			//## 텍스쳐 적용
			LPDIRECT3DTEXTURE9 pCurTexture = NULL;
			int nTex1Id = m_pPatch[i].GetTextureId();

			if( m_pTextureMng != NULL )
			{
				if( nTex1Id != -1 )
				{
                    pCurTexture = m_pTextureMng->GetTexture( nTex1Id )->pd3dTexture;
				}

			}

			m_pd3dDevice->SetTexture( 0, pCurTexture );

			//## 지형 출력!!
			m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(S_CUSTOMVERTEX) );
			m_pd3dDevice->SetIndices( m_pIB );
			m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
												0,
												0,
												nNumVertices,
												0,
												nNumFaces );

			m_snNumFaces += m_pPatch[i].GetMeshPtr()->GetNumFaces();

			if( m_bPatchBbox )
                m_pPatch[i].RenderBbox(m_pd3dDevice);

			m_pPatch[i].SetRenderSw( false );



			//c++;
		}
	}

	g_pObserver->Display( 2, m_snNumFaces, 0 );

}

float CTerrain::GetHeight(float fX, float fZ)
{
	CHeightmap *pHeightmap = m_pPatch->GetHeightmapPtr();

	float fHeight;

	//## cellspace
	float fCellSpace = m_pPatch->GetCellSpace();

	fX /= fCellSpace;
	fZ /= fCellSpace;

	float fCol = ::floorf(fX);
	float fRow = ::floorf(fZ);

	//## A B
	//## *-*
	//## |/|
	//## *-*
	//## C D
	float fA = pHeightmap->GetHeight(fCol, fRow);
	float fB = pHeightmap->GetHeight(fCol+1, fRow);
	float fC = pHeightmap->GetHeight(fCol, fRow+1);
	float fD = pHeightmap->GetHeight(fCol+1,fRow+1);

	float fDx = fX - fCol;
	float fDz = fZ - fRow;

	float fUy, fVy;

	if(fDz < 1.0f - fDx) // upper triangle ABC
	{
		fUy = fB - fA; // A->B
		fVy = fC - fA; // A->C

		fHeight = fA + Lerp(0.0f, fUy, fDx) + Lerp(0.0f, fVy, fDz);
	}
	else // lower triangle DCB
	{
		fUy = fC - fD; // D->C
		fVy = fB - fD; // D->B

		fHeight = fD + Lerp(0.0f, fUy, 1.0f - fDx) + Lerp(0.0f, fVy, 1.0f - fDz);
	}

	return fHeight;
}