#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "misc.h"
#include "patch.h"
#include "texturemanager.h"

using namespace std;


//############################################################
// MAP 과 매핑되는 각 PATCH의 VERTEX
//
// HEIGHTMAP을 PATCH로 나누었기 때문에
// PATCH와 PATCH가 만나는 부분은 각 PATCH별로 정점을 따로
// 관리해야 편집 업데이트가 용이하다.
//############################################################
struct S_MAP_POINT
{
	//## 맵의 한 지점은 최대 4개 PATCH가 공유할 수 있다.
	S_CUSTOMVERTEX *paMapPoint[4];

	CPatch			*pPatch[4];

	//## 생성시 NULL로 초기화하여 
	//## 데이타 작성시 작성해야하는 부분인지 판별할 수 있게 한다.
	S_MAP_POINT()
	{
		for( int i = 0; i < 4; i++ )
		{
			paMapPoint[i] = NULL;
			pPatch[i] = NULL;
		}
	}
};

//####################################################
//####################################################
// CTerrain
// 의존성 : CTextureManager, CPatch, LPDIRECT3D,
// S_TRITREENODE
//####################################################
//####################################################
class AFX_EXT_CLASS CTerrain
{
private:
	HRESULT _BuildVB(int nVertexBufferSize);
	HRESULT _BuildIB(int nIndexBufferSize);

public:

	CTerrain(void);
	~CTerrain(void);

	void Clear();
	void Destroy();

	//######################################################################
	//## SET
	void SetTextureManager( CTextureManager *pTextureMng ){ m_pTextureMng = pTextureMng; }
	void SetPatchTexture( CPatch *pPatch, char *pTexture, int nIndex )
	{
		int nPatchTextureId = -1;

		if( strlen(pTexture) > 0 && m_pTextureMng != NULL )
		{
            nPatchTextureId = m_pTextureMng->RegisterTexture( pTexture );

			pPatch->SetTextureId( nPatchTextureId, nIndex );
		}
	}

	//######################################################################
	//## GET
	CTextureManager *GetTextureMngPtr(){ return m_pTextureMng; }
	void TogglePatchBboxRenderSwitch(){ m_bPatchBbox = !m_bPatchBbox; }
	bool GetPatchBboxRenderSwitch(){ return m_bPatchBbox; }
	int GetEdgeSpace(){ return m_nEdgeSpace; }
	int GetNumPatchesPerSide(){ return m_nNumPatchesPerSide; }
	CPatch *GetPatchBeginPtr(){ return m_pPatch; }
	float GetHeight(float fX, float fZ);


	void InitDevice(LPDIRECT3DDEVICE9 pd3dDevice){ m_pd3dDevice = pd3dDevice; }
	bool Create(int nTerrainType, int nMapSize, int nNumPatchesPerSide, int nCellSpace,
				LPCTSTR lpHeightMapPath, LPCTSTR lpTexturePath);
	bool BuildPatches(	CHeightmap *pHeightmap,
						int nPatchSpace, int nCellSpace,
						int nNumCells, int nNumVertices, LPCTSTR lpTexturePath 	);
	void Render();

public:
	static int							m_snNumFaces;
	static S_MAP_POINT					*m_spMapPatch;

private:

	LPDIRECT3DDEVICE9			m_pd3dDevice;			//## The D3D rendering device
    LPDIRECT3DVERTEXBUFFER9		m_pVB;                  //## vextex buffer 
	LPDIRECT3DINDEXBUFFER9		m_pIB;					//## index buffer

	CTextureManager				*m_pTextureMng;			//## texture 관리자
	CPatch						*m_pPatch;

	int							m_nNumPatchesPerSide;		//## side당 patch 수
	int							m_nEdgeSpace;
	float						m_fMaxHeight;
	float						m_fMinHeight;

	bool						m_bPatchBbox;
};

#endif