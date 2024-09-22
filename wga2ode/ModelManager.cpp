#include "StdAfx.h"
#include "modelmanager.h"

//## observer 
#include "observer.h"

CModelManager::CModelManager(void)
{
	m_pd3dDevice = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
}

CModelManager::~CModelManager(void)
{
	Destroy();
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVB);
}

void CModelManager::Destroy()
{
	//## 모델 제거
	if( !m_ModelList.empty() )
		stl_wipe( m_ModelList );
}


HRESULT CModelManager::Init( LPDIRECT3DDEVICE9 pd3dDevice )
{
	if( m_pd3dDevice == NULL )
	{
		m_pd3dDevice = pd3dDevice;

		//## vertex buffer 생성 
		if( FAILED( m_pd3dDevice->CreateVertexBuffer( 100000* sizeof(S_CUSTOMVERTEX),
														D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY ,
														FVF,
														D3DPOOL_DEFAULT,
														&m_pVB,
														NULL ) ) )
		{
			return E_FAIL;
		}


		//## index buffer 생성 
		if( FAILED( m_pd3dDevice->CreateIndexBuffer( 200000*sizeof(S_CUSTOMINDEX),
													D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
													D3DFMT_INDEX16,
													D3DPOOL_DEFAULT,
													&m_pIB,
													NULL ) ) )
		{
			return E_FAIL;
		}

	}

	return S_OK;

}

void CModelManager::Render()
{
	//## 모델 순차적으로 출력
	S_DRAW_MODEL *pDrawModel;
	S_DRAW_MTRLMESH *pDrawMtrlMesh;
	S_MATERIAL_MESH *pMtrlObj;

	MtrlMeshList *pMtrlMeshList;

	CMeshObj *pMeshObj;

	//## iterator
	ModelListItor itor;
	MtrlMeshListItor itor2;
	MeshObjListItor itor3;

	//## vertex, face 수
	int nNumVertices, nNumFaces;

	m_nNumFaces = 0;

	//## buffer pointer
	S_CUSTOMVERTEX *pVertices;
	S_CUSTOMINDEX *pIndices;
	S_CUSTOMVERTEX *pMeshVB;
	S_CUSTOMINDEX *pMeshIB;

	//## 이전 환경 백업
	D3DXMATRIXA16 matWorld,matTm;//, matPos , matScale;
	D3DMATERIAL9 mtrlCurState;
	IDirect3DBaseTexture9 *pCurTexture;

	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	m_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );
	m_pd3dDevice->GetMaterial( &mtrlCurState );
	m_pd3dDevice->GetTexture( 0, &pCurTexture );
	m_pd3dDevice->SetFVF( FVF );

	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//##  대기모델 loop
	for( itor = m_ModelList.begin(); itor != m_ModelList.end(); ++itor )
	{
		pDrawModel = (S_DRAW_MODEL *)(*itor);

		int nNumMtrl = pDrawModel->nNumMtrl;
		pDrawMtrlMesh = pDrawModel->pMtrlMesh;

		for( int i = 0; i < nNumMtrl; i++ )
		{
			pMtrlObj = (pDrawMtrlMesh+i)->pMtrlMesh;
			m_pd3dDevice->SetMaterial( &(pMtrlObj->Mtrl) );
			m_pd3dDevice->SetTexture( 0, pMtrlObj->pTexture );

			for( int j = 0; j < (pDrawMtrlMesh+i)->nNumMesh; j++ )
			{
				//## 렌더링 스위치가 켜져있으면 출력
				if( (pDrawMtrlMesh+i)->pMesh[j].GetRenderSw() )
				{
					pMeshObj = (pDrawMtrlMesh+i)->pMesh[j].GetMeshPtr();

					pMeshVB = pMeshObj->GetVBPtr();
					pMeshIB = pMeshObj->GetIBPtr();

					nNumVertices = pMeshObj->GetNumVertices();
					nNumFaces = pMeshObj->GetNumFaces();

					//###################################################################
					//## tm 행렬 적용
					matTm = pMeshObj->GetMatrixTM();//matScale *  * matPos;// * matScale * matPos;
					matTm *= pDrawModel->matPos;

					m_pd3dDevice->SetTransform( D3DTS_WORLD, &matTm );


					//## vb 생성을 위한 lock
					if( FAILED( m_pVB->Lock( 0, 0, (void**)&pVertices, D3DLOCK_DISCARD ) ) )
					{
						return ;
					}

					//## IB 생성을 위한 lock
					if( FAILED( m_pIB->Lock( 0, 0, (void**)&pIndices, D3DLOCK_DISCARD ) ) )
					{
						return ;
					}

					memcpy( pVertices ,pMeshVB , sizeof(S_CUSTOMVERTEX)*nNumVertices );
					memcpy( pIndices ,pMeshIB, sizeof(S_CUSTOMINDEX)*nNumFaces );

					//## ib Unlock
					m_pIB->Unlock();

					//## vb Unlock
					m_pVB->Unlock();

					m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(S_CUSTOMVERTEX) );
					m_pd3dDevice->SetIndices( m_pIB );

					m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,
														0,
														0,
														nNumVertices,
														0,
														nNumFaces );

					m_nNumFaces += nNumFaces;

					//pMeshObj->GetAABB().RenderBbox( m_pd3dDevice, 0xffffffff );

				}//## if( (pDrawMtrlMesh+i)->pMesh[j].GetRenderSw() )

			}//## for( int j = 0; j < (pDrawMtrlMesh+i)->nNumMesh; j++ )

		}//## for( int i = 0; i < nNumMtrl; i++ )

	}//## for( itor = m_ModelList.begin(); itor != m_ModelList.end(); ++itor )

	//## 이전 환경으로 복귀
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	m_pd3dDevice->SetMaterial( &mtrlCurState );
	m_pd3dDevice->SetTexture( 0, pCurTexture );
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	g_pObserver->Display( 3, m_nNumFaces, 0 );
}


//############################################################################
// 모델 
//############################################################################
S_DRAW_MODEL *CModelManager::AddModel( CModel *pModel, D3DXVECTOR3 &vPos )
{
	S_MATERIAL_MESH *pMtrlObj;
	CMeshObj *pMeshObj;
	D3DXMATRIXA16		matTemp, matRotate;

	S_DRAW_MODEL *pDrawModel = new S_DRAW_MODEL;

	//## 랜덤 회전
	D3DXMatrixRotationY( &matRotate, rand()%30 );
	pDrawModel->matRotate = matRotate;

	//## 위치
	pDrawModel->vPos = vPos;

	D3DXMATRIXA16 matScale, matPos;

	D3DXMatrixTranslation( &matPos, pDrawModel->vPos.x, pDrawModel->vPos.y, pDrawModel->vPos.z );
	D3DXMatrixScaling( &matScale, 1.0f, 1.0f, 1.0f );

	pDrawModel->matPos = matPos;
	pDrawModel->matScale = matScale;


	//## iterator
	MtrlMeshListItor itor;
	MeshObjListItor itor2;

	//## 
	MtrlMeshList *pMtrlMeshList = pModel->GetMtrlMeshListPtr();

	S_DRAW_MTRLMESH *pDrawMtrlMesh;

	int nNumMtrlMesh, nNumMesh;
	nNumMtrlMesh = pMtrlMeshList->size();

	if( nNumMtrlMesh == 1 )
	{
		pDrawMtrlMesh = new S_DRAW_MTRLMESH;
	}
	else if( nNumMtrlMesh > 1 )
	{
        pDrawMtrlMesh = new S_DRAW_MTRLMESH[ nNumMtrlMesh ];
	}


	CDrawMesh *pDrawMesh;

	int c1, c2;
	c1 = 0;
	for( itor = pMtrlMeshList->begin(); itor != pMtrlMeshList->end(); ++itor )
	{
		(pDrawMtrlMesh+c1)->pMtrlMesh = pMtrlObj = (S_MATERIAL_MESH *)(*itor);

		nNumMesh = pMtrlObj->pObjList->size();

		if( nNumMesh == 1 )
		{
			pDrawMesh = new CDrawMesh;
		}
		else if ( nNumMesh > 1 )
		{
			pDrawMesh = new CDrawMesh[ nNumMesh ];
		}

		c2 = 0;
		for( itor2 = pMtrlObj->pObjList->begin(); itor2 != pMtrlObj->pObjList->end(); ++itor2 )
		{
			pMeshObj = (CMeshObj *)(*itor2);

			(pDrawMesh+c2)->GetAABB().m_vPos = pMeshObj->GetAABB().m_vPos;
			(pDrawMesh+c2)->GetAABB().m_vExtent = pMeshObj->GetAABB().m_vExtent;
			(pDrawMesh+c2)->SetMesh( pMeshObj );

			c2++;
		}

		(pDrawMtrlMesh+c1)->pMesh = pDrawMesh;
		(pDrawMtrlMesh+c1)->nNumMesh = nNumMesh;

		c1++;
	}


	pDrawModel->nNumMtrl = nNumMtrlMesh;
	pDrawModel->pMtrlMesh = pDrawMtrlMesh;

	m_ModelList.push_back( pDrawModel ); 

	return pDrawModel;
}