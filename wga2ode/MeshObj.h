#ifndef _MESHOBJ_H_
#define _MESHOBJ_H_

#include "misc.h"
#include "aabb.h"
#include "frustum.h"
#include "occlusion.h"


//##############################################################
// 기본 메시 클래스
//##############################################################
class CMeshObj
{
public:
	CMeshObj();
	~CMeshObj();

	//UINT GetMeshObjType(){ return m_uiType; }
	S_CUSTOMVERTEX *GetVBPtr(){ return m_pVertices; }
	S_CUSTOMINDEX *GetIBPtr(){ return m_pIndices; }
	int	GetNumVertices(){ return m_nNumVertices; }
	int GetNumFaces(){ return m_nNumFaces; }
	CAABB &GetAABB(){ return m_AABB; }
	//bool GetRenderingSwitch(){ return m_bRenderingSwitch; }
	D3DXMATRIXA16 &GetMatrixTM(){ return m_matTm; }
	COcclusion *GetOcclusionPtr(){ return &m_Occlusion; }

	void SetBbox( D3DXVECTOR3 *pvMax, D3DXVECTOR3 *pvMin );
	void SetNumVertices(int nNumVertices){ m_nNumVertices = nNumVertices; }
	void SetNumFaces(int nNumFaces){ m_nNumFaces = nNumFaces; }
	void SetVertices( S_CUSTOMVERTEX *pVertices ){ m_pVertices = pVertices; }
	void SetIndices( S_CUSTOMINDEX *pIndices ){ m_pIndices = pIndices; }
	void SetMatrixTM(D3DXMATRIXA16 &matTm){ m_matTm = matTm; }
	void SetMatrixInverseTM(D3DXMATRIXA16 &matInvTm){ m_matInvTm = matInvTm; }

	//void SetRenderingSwitch(bool bSwitch){ m_bRenderingSwitch = bSwitch; }


protected:
	CAABB						m_AABB;
	COcclusion					m_Occlusion;

	S_CUSTOMVERTEX				*m_pVertices;
	S_CUSTOMINDEX				*m_pIndices;

	int							m_nNumVertices;		//## 정점수
	int							m_nNumFaces;		//## 삼각형수

	//bool						m_bRenderingSwitch;

	D3DXMATRIXA16				m_matTm;			//## tm 행렬
	D3DXMATRIXA16				m_matInvTm;			//## inverse tm 행렬
};

#endif