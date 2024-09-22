#ifndef _MODEL_H_
#define _MODEL_H_

#include "misc.h"
#include "MeshObj.h"

#include <list>

using namespace std;

typedef list<CMeshObj *>			MeshObjList;
typedef	MeshObjList::iterator		MeshObjListItor;

//## material ���ؿ� ���� ���� mesh ����Ʈ ����ü
struct S_MATERIAL_MESH {
	D3DMATERIAL9				Mtrl;

	//## �̰��� ���� ���Ϸ� ������ �� �̹������� ���� �����ϱ� ���� ���̴�.
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
// ������ : CMeshObj, (convert : CParserSelector, CParser )
//###########################################################
//###########################################################
class AFX_EXT_CLASS CModel
{
public:
	CModel(void);
	~CModel(void);

	MtrlMeshList *GetMtrlMeshListPtr(){ return &m_MtrlMeshList; }

	//## S_MATERIAL_MESH ���� -> �����͸� list�� ����
	S_MATERIAL_MESH *CreateMtrlMesh()
	{
		S_MATERIAL_MESH *pMtrlMesh = new S_MATERIAL_MESH;

		m_MtrlMeshList.push_back( pMtrlMesh );

		return pMtrlMesh;
	}

	//## CMeshObj list ���� -> pointer ��ȯ
	MeshObjList *CreateMeshObjList(){ return new MeshObjList; }

	//## CMeshObj ���� -> pointer ��ȯ
	CMeshObj *CreateMeshObj(){ return new CMeshObj; }


	//## 3d ���ϸ��� �Է¹޾� ���α׷����� ����� �𵨷� ��ȯ
	bool ConvertRaw3dData(const char *pFilePathName, LPDIRECT3DDEVICE9 pd3dDevice );

private:
	//## material �� ���� ������ CMeshObj list
	MtrlMeshList				m_MtrlMeshList;
};

#endif