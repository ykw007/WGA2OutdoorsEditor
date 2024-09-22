#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "misc.h"
#include "patch.h"
#include "texturemanager.h"

using namespace std;


//############################################################
// MAP �� ���εǴ� �� PATCH�� VERTEX
//
// HEIGHTMAP�� PATCH�� �������� ������
// PATCH�� PATCH�� ������ �κ��� �� PATCH���� ������ ����
// �����ؾ� ���� ������Ʈ�� �����ϴ�.
//############################################################
struct S_MAP_POINT
{
	//## ���� �� ������ �ִ� 4�� PATCH�� ������ �� �ִ�.
	S_CUSTOMVERTEX *paMapPoint[4];

	CPatch			*pPatch[4];

	//## ������ NULL�� �ʱ�ȭ�Ͽ� 
	//## ����Ÿ �ۼ��� �ۼ��ؾ��ϴ� �κ����� �Ǻ��� �� �ְ� �Ѵ�.
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
// ������ : CTextureManager, CPatch, LPDIRECT3D,
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

	CTextureManager				*m_pTextureMng;			//## texture ������
	CPatch						*m_pPatch;

	int							m_nNumPatchesPerSide;		//## side�� patch ��
	int							m_nEdgeSpace;
	float						m_fMaxHeight;
	float						m_fMinHeight;

	bool						m_bPatchBbox;
};

#endif