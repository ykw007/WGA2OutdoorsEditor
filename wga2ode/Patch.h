//####################################################
//####################################################
// CPatch
// 지형 기본 단위
//####################################################
//####################################################
#ifndef _Patch_H_
#define _Patch_H_

#include "misc.h"
#include "drawmesh.h"

//####################################################
//####################################################
// CHeightmap
// 의존성 : CDib
//####################################################
//####################################################
class AFX_EXT_CLASS CHeightmap
{
	float					*m_pfHeightData;
	float					m_fMapSize;
	UINT					m_uiNumPixel;

public:
	//## 지형 생성 타입 ( 평지, 높이맵, 랜덤 )
	enum E_TERRAIN_TYPE	
	{
		FLATLAND = 0, HEIGHTMAP, RANDOM 
	};

	CHeightmap();
	~CHeightmap();

	void SetHeight(int nX, int nZ, float fY);
	float GetMapSize();
	float GetHeight(UINT uiX, UINT uiZ);

	bool Build(BYTE ucTerrainType, float fMapSize, int nCellSpace, LPCTSTR lpHeightMapPath);
};

//####################################################
//####################################################
// CPatch
//####################################################
//####################################################
class AFX_EXT_CLASS CPatch : public CDrawMesh
{

public:
	CPatch();
	~CPatch();

	static void InitDevice(LPDIRECT3DDEVICE9 pd3dDevice);
	static void InitShareEntry( CHeightmap *pHeightmap,
								int nPatchSpace, int nCellSpace,
								int nNumCells, int nNumVertices );

	//##################################################################
	//## GET
	static float GetMostMaxHeight(){ return m_sfMaxHeight; }
	static float GetMostMinHeight(){ return m_sfMinHeight; }
	static int GetPatchSpace(){ return m_snPatchSpace; }
	static int GetCellSpace(){ return m_snCellSpace; }
	static int GetNumCells(){ return m_snNumCells; }
	static CHeightmap *GetHeightmapPtr(){ return m_spHeightmap; }

	int GetTextureId(int nIndex=0){ return m_nTexId[nIndex]; }
	float GetMaxHeightInPatch(){ return m_fMaxHeight; }
	float GetMinHeightInPatch(){ return m_fMinHeight; }
	void GetMaxMinHeight();

	//##################################################################
	//## SET
	void SetTextureId( int nId, int nIndex ){ m_nTexId[nIndex] = nId; }

	//##################################################################
	//## METHODS
	void AdjustHeightmap(float fX, float fZ, float fY);
	void AdjustBbox();

	void Init(int nBeginX, int nBeginZ, int EndX, int EndZ, int nTexId);
	void RenderBbox(LPDIRECT3DDEVICE9 pd3dDevice);
	void BuildNormal(int nX, int nZ, D3DXVECTOR3 *pvN);
	void Create();

private:
	void _BuildVB();
	void _BuildIB();

private:

	static LPDIRECT3DDEVICE9	m_pd3dDevice;			//## patch들이 공유하게될 d3d device
	static int					m_snCellSpace;			//## cell 간격 
	static int					m_snPatchSpace;			//## patch 간격
	static int					m_snNumCells;			//## patch 당 cell 수
	static int					m_snNumVertices;		//## patch 당 vertex 수
	static CHeightmap			*m_spHeightmap;

	static float				m_sfMaxHeight;
	static float				m_sfMinHeight;

	float						m_fMaxHeight;			//## patch의 최대 높이
	float						m_fMinHeight;			//## patch의 최저 높이

	int							m_nTexId[3];			//## texture id;

	int							m_nBeginX;
	int							m_nBeginZ;
	int							m_nEndX;
	int							m_nEndZ;
};

#endif