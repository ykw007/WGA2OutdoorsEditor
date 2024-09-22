#ifndef _MODEL_H_
#define _MODEL_H_

#include "misc.h"
#include "MeshObj.h"

#include <list>

using namespace std;

typedef list<CMeshObj *>			MeshObjList;
typedef	MeshObjList::iterator		MeshObjListItor;

//## material 기준에 따라 나눈 mesh 리스트 구조체
struct S_MATERIAL_MESH {
	D3DMATERIAL9				Mtrl;

	//## 이것은 모델을 파일로 저장할 때 이미지까지 같이 저장하기 위한 것이다.
	char						*pTexFile;

	LPDIRECT3DTEXTURE9			pTexture;

	MeshObjList					*pObjList;

	S_MATERIAL_MESH()
	{
		pTexture = NULL;
		pObjList = NULL;
		pTexFile = NULL;
		ZeroMemory( &Mtrl, sizeof(D3DMATERIAL9) );
		Mtrl.Diffuse.r = Mtrl.Ambient.r = 1.0f;
		Mtrl.Diffuse.g = Mtrl.Ambient.g = 1.0f;
		Mtrl.Diffuse.b = Mtrl.Ambient.b = 1.0f;
		Mtrl.Diffuse.a = Mtrl.Ambient.a = 1.0f;
	}

	~S_MATERIAL_MESH()
	{
		if( !pObjList->empty() )
			stl_wipe( *pObjList );

		SAFE_DELETE( pObjList );
		SAFE_DELETE_ARRAY( pTexFile );
		SAFE_RELEASE( pTexture );
	}
};

typedef list<S_MATERIAL_MESH *>		MtrlMeshList;
typedef	MtrlMeshList::iterator		MtrlMeshListItor;

//###########################################################
//###########################################################
// class CModel
//
// 의존성 : CMeshObj, (convert : CParserSelector, CParser )
//###########################################################
//###########################################################
class AFX_EXT_CLASS CModel
{
public:
	CModel(void);
	~CModel(void);

	MtrlMeshList *GetMtrlMeshListPtr(){ return &m_MtrlMeshList; }

	//## S_MATERIAL_MESH 생성 -> 포인터를 list에 삽입
	S_MATERIAL_MESH *CreateMtrlMesh()
	{
		S_MATERIAL_MESH *pMtrlMesh = new S_MATERIAL_MESH;

		m_MtrlMeshList.push_back( pMtrlMesh );

		return pMtrlMesh;
	}

	//## CMeshObj list 생성 -> pointer 반환
	MeshObjList *CreateMeshObjList(){ return new MeshObjList; }

	//## CMeshObj 생성 -> pointer 반환
	CMeshObj *CreateMeshObj(){ return new CMeshObj; }


	//## 3d 파일명을 입력받아 프로그램에서 사용할 모델로 변환
	bool ConvertRaw3dData(const char *pFilePathName, LPDIRECT3DDEVICE9 pd3dDevice );

private:
	//## material 에 따라 저장한 CMeshObj list
	MtrlMeshList				m_MtrlMeshList;
};

#endif