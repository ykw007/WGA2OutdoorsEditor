#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "model.h"
#include "drawmesh.h"

#include <list>

using namespace std;

struct S_DRAW_MTRLMESH
{
	//## material 구분을 위한
	S_MATERIAL_MESH *pMtrlMesh;
	CDrawMesh		*pMesh;
	int				nNumMesh;


	S_DRAW_MTRLMESH()
	{
		pMtrlMesh = NULL;
		pMesh = NULL;
	}

	~S_DRAW_MTRLMESH()
	{

		if( nNumMesh == 1 )
		{
			SAFE_DELETE( pMesh );
		}
		else if( nNumMesh > 1 )
		{
            SAFE_DELETE_ARRAY( pMesh );
		}

	}
};

//## 렌더링 대기 모델
struct S_DRAW_MODEL
{
	D3DXVECTOR3			vPos;
	D3DXMATRIXA16		matScale;
	D3DXMATRIXA16		matPos;
	D3DXMATRIXA16		matRotate;
	S_DRAW_MTRLMESH		*pMtrlMesh;
	int					nNumMtrl;

	S_DRAW_MODEL()
	{
		D3DXMatrixIdentity(&matRotate);
		D3DXMatrixIdentity(&matPos);
		D3DXMatrixIdentity(&matScale);
	}
	~S_DRAW_MODEL()
	{

		if( nNumMtrl == 1 )
		{
            SAFE_DELETE( pMtrlMesh );
		}
		else if( nNumMtrl > 1 )
		{
			SAFE_DELETE_ARRAY( pMtrlMesh );
		}
	}
};

typedef list<S_DRAW_MODEL *>	ModelList;
typedef ModelList::iterator		ModelListItor;

//###########################################################
//###########################################################
// class CModelManager
//###########################################################
//###########################################################
class AFX_EXT_CLASS CModelManager
{
public:
	CModelManager(void);
	virtual ~CModelManager(void);

	void Destroy();

	HRESULT Init( LPDIRECT3DDEVICE9 pd3dDevice );
	S_DRAW_MODEL *AddModel( CModel *pModel, D3DXVECTOR3 &vPos );
	void Render();

private:

	LPDIRECT3DDEVICE9			m_pd3dDevice;
    LPDIRECT3DVERTEXBUFFER9		m_pVB;                  //## vextex buffer 
	LPDIRECT3DINDEXBUFFER9		m_pIB;					//## index buffer

	int							m_nNumFaces;
	//## 모델 리스트
	ModelList					m_ModelList;
};

#endif