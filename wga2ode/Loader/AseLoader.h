#ifndef _ASELOADER_H_
#define _ASELOADER_H_

#include "..\misc.h"

#include <vector>
#include <list>

using namespace std;

//## define
#define BUFFERSIZE 256

#define T_ASE					"*3DSMAX_ASCIIEXPORT"
#define T_MATERIAL_LIST			"*MATERIAL_LIST"
#define T_MATERIAL_COUNT		"*MATERIAL_COUNT"
#define T_MATERIAL				"*MATERIAL"
#define T_NUMSUBMTLS			"*NUMSUBMTLS"
#define T_SUBMATERIAL			"*SUBMATERIAL"
#define T_MATERIAL_AMBIENT		"*MATERIAL_AMBIENT"
#define T_MATERIAL_DIFFUSE		"*MATERIAL_DIFFUSE"
#define T_MATERIAL_SPECULAR		"*MATERIAL_SPECULAR"
#define T_MAP_DIFFUSE			"*MAP_DIFFUSE"

#define T_MAP_AMBIENT			"*MAP_AMBIENT"
#define T_MAP_SHINESTRENGTH		"*MAP_SHINESTRENGTH"
#define T_MAP_BUMP				"*MAP_BUMP"
#define T_MAP_SPECULAR			"*MAP_SPECULAR"
#define T_MAP_REFLECT			"*MAP_REFLECT"
#define T_MAP_REFRACT			"*MAP_REFRACT"

#define T_BITMAP				"*BITMAP"
#define T_GEOMOBJECT			"*GEOMOBJECT"
#define T_NODE_NAME				"*NODE_NAME"
#define T_NODE_PARENT			"*NODE_PARENT"
#define T_NODE_TM				"*NODE_TM"
#define T_TM_ROW0				"*TM_ROW0"
#define T_TM_ROW1				"*TM_ROW1"
#define T_TM_ROW2				"*TM_ROW2"
#define T_TM_ROW3				"*TM_ROW3"
#define T_MESH					"*MESH"
#define T_MESH_NUMVERTEX		"*MESH_NUMVERTEX"
#define T_MESH_NUMFACE			"*MESH_NUMFACES"
#define T_MESH_VERTEX_LIST		"*MESH_VERTEX_LIST"
#define T_MESH_FACE_LIST		"*MESH_FACE_LIST"
#define T_MESH_VERTEX			"*MESH_VERTEX"
#define T_MESH_FACE				"*MESH_FACE"
#define T_MESH_MTLID			"*MESH_MTLID"
#define T_MESH_NUMTVERTEX		"*MESH_NUMTVERTEX"
#define T_MESH_TVERTLIST		"*MESH_TVERTLIST"
#define T_MESH_TVERT			"*MESH_TVERT"
#define T_MESH_NUMTVFACES		"*MESH_NUMTVFACES"
#define T_MESH_TFACELIST		"*MESH_TFACELIST"
#define T_MESH_TFACE			"*MESH_TFACE"
#define T_MESH_NORMALS			"*MESH_NORMALS"
#define T_MESH_FACENORMAL		"*MESH_FACENORMAL"
#define T_MESH_VERTEXNORMAL		"*MESH_VERTEXNORMAL"
#define T_MATERIAL_REF			"*MATERIAL_REF"

#define T_TM_ANIMATION			"*TM_ANIMATION"
#define T_CONTROL_ROT_TCB		"*CONTROL_ROT_TCB"
#define T_CONTROL_POS_TRACK		"*CONTROL_POS_TRACK"
#define T_CONTROL_ROT_TRACK		"*CONTROL_ROT_TRACK"


/****************************************************************
* STRUCTURES
****************************************************************/


//## ase 데이타 parsing 후 가공된 데이타 를 담는 구조체

//## OBJECT 구조체
struct OBJECT{
	D3DXMATRIXA16			matTm;				// tm 행렬
	D3DXMATRIXA16			matInvTm;			// inverse tm 행렬

	int						nNumFace;
	S_CUSTOMVERTEX*			pVertex;
	OBJECT*					pNext;

	OBJECT()
	{
		D3DXMatrixIdentity(&matTm);
		D3DXMatrixIdentity(&matInvTm);

		nNumFace=0;
		pVertex=0;
		pNext=0;
	}

	~OBJECT()
	{
		SAFE_DELETE_ARRAY(pVertex)
		SAFE_DELETE(pNext)
	}

};

//## material 기준에 따라 나눈 mesh 리스트 구조체
struct MODEL {
	D3DMATERIAL9		Mtrl;
	LPDIRECT3DTEXTURE9	pTexture;

	OBJECT*				pObjList;
	MODEL*				pNext;

	MODEL()
	{
		pNext=0;
		pObjList=0;
		pTexture=0;
		ZeroMemory( &Mtrl, sizeof(D3DMATERIAL9) );
		Mtrl.Diffuse.r = Mtrl.Ambient.r = 1.0f;
		Mtrl.Diffuse.g = Mtrl.Ambient.g = 1.0f;
		Mtrl.Diffuse.b = Mtrl.Ambient.b = 1.0f;
		Mtrl.Diffuse.a = Mtrl.Ambient.a = 1.0f;
	}

	~MODEL()
	{
		SAFE_DELETE(pObjList)
		SAFE_DELETE(pNext)
	}
};


//## 아래 구조체들은 parsing에서 잠시 사용될 구조체
//## 수정 사항 : 좀더 간결하게 

// 메시의 면 구조체
struct FACE {
	S_CUSTOMINDEX				Index;			// 메시 face 인덱스
	S_CUSTOMINDEX				TIndex;			// 텍스쳐 uv 좌표 인덱스 
	D3DXVECTOR3			vN;				// 면의 normal 벡터
	int					nSubMtrlID;		// 해당 면의 submaterial의 ID

	FACE()
	{
		Index._0=0;
		Index._1=0;
		Index._2=0;

		TIndex._0=0;
		TIndex._1=0;
		TIndex._2=0;

		vN=D3DXVECTOR3(0.0f,0.0f,0.0f);

		nSubMtrlID=0;
	}

	~FACE()
	{
	}
};

// 텍스쳐 UV 좌표 구조체
struct TUV {
	float				fU,fV;
};

// MESH 구조체
struct MESH {
	char*				pNodeName;			// 노드 이름 - 계층관계로 이루어질 때 사용되는 인덱스
	char*				pParentName;		// 하위 계층일 때 부모 노드 이름
	S_CUSTOMVERTEX*			pVertex;			// 정점 배열 포인터
	FACE*				pFace;				// 면 배열 data 포인터
	TUV*				pUV;				// 텍스쳐 uv 좌표 배열 포인터

	D3DXMATRIXA16		matTm;				// tm 행렬
	D3DXMATRIXA16		matInvTm;			// inverse tm 행렬

	int					nNumVertex;
	int					nNumFace;
	int					nNumTVertex;
	int					nNumTVFace;

	int					nMtrlRef;

	MESH()
	{
		pNodeName=0;
		pParentName=0;
		pVertex=0;
		pFace=0;
		pUV=0;

		nNumVertex=0;
		nNumFace=0;
		nNumTVertex=0;
		nNumTVFace=0;

		nMtrlRef=0;

		D3DXMatrixIdentity(&matTm);
		D3DXMatrixIdentity(&matInvTm);
	}

	~MESH()
	{
		SAFE_DELETE_ARRAY(pNodeName)
		SAFE_DELETE_ARRAY(pParentName)
		SAFE_DELETE_ARRAY(pVertex)
		SAFE_DELETE_ARRAY(pFace)
		SAFE_DELETE_ARRAY(pUV)
	}
};

// MATERIAL 구조체
struct MATERIAL {
	int					nID;
	D3DMATERIAL9		Mtrl;
	char*				sTexFile;
	MATERIAL*			pSubMtrl;
	int					nNumSubMtrl;

	MATERIAL()
	{
		nID=0;
		sTexFile=0;
		pSubMtrl=0;
		nNumSubMtrl=0;
		ZeroMemory( &Mtrl, sizeof(D3DMATERIAL9) );
		Mtrl.Diffuse.r = Mtrl.Ambient.r = 1.0f;
		Mtrl.Diffuse.g = Mtrl.Ambient.g = 1.0f;
		Mtrl.Diffuse.b = Mtrl.Ambient.b = 1.0f;
		Mtrl.Diffuse.a = Mtrl.Ambient.a = 1.0f;
	}

	~MATERIAL()
	{
		SAFE_DELETE_ARRAY(sTexFile)

		if(nNumSubMtrl>1)
            SAFE_DELETE_ARRAY(pSubMtrl)
		else if(nNumSubMtrl==1)
			SAFE_DELETE(pSubMtrl)
	}
};


template<typename TContainer>
void stl_wipe(TContainer& container)
{
	for(TContainer::iterator i=container.begin();i!=container.end();++i)
		delete *i;

	container.clear();
}

//####################################################
//####################################################
// class CAseLoader
//####################################################
//####################################################
class AFX_EXT_CLASS CAseLoader
{
private:

	FILE*					m_fpFile;
	MATERIAL*				m_pMtrl;			// material 배열 포인터
	int						m_nNumMtrl;			// material 갯수
	MESH*					m_pMesh;
	list<MESH *>			m_pMeshList;		// 메시 리스트
	MODEL*					m_pMdEntry;

	char					m_sBuffer[BUFFERSIZE];
	char					m_sHeader[BUFFERSIZE];


	inline void ClearBuffer();

	void Parse_MATERIAL_LIST();
	void Parse_MATERIAL(MATERIAL *);
	void Parse_MAP_DIFFUSE(MATERIAL *);
	void Parse_GEOMOBJECT();
	void Parse_NODE_TM();
	void Parse_MESH();
	void Parse_MESH_VERTEX_LIST();
	void Parse_MESH_FACE_LIST();
	void Parse_MESH_TVERTLIST();
	void Parse_MESH_TFACELIST();
	void Parse_MESH_NORMALS();
	void TransformToLocal();

	void Parse_ONLY_PASS();

	void Parse_TM_ANIMATION();
	void Parse_CONTROL_ROT_TCB();
	void Parse_CONTROL_POS_TRACK();
	void Parse_CONTROL_ROT_TRACK();

	void ProcessSubMaterial(MATERIAL*,MESH*);
	void MakeModel(MODEL**,MATERIAL*,MESH*);
	MODEL*	MakeModelList();

public:
	CAseLoader(){ Clear(); }
	~CAseLoader(){ Destroy(); }

	MODEL *ReadAse(const char *);

	bool Clear();
	void Destroy();
};

#endif