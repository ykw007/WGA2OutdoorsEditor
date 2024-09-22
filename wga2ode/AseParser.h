#ifndef _ASEPARSER_H_
#define _ASEPARSER_H_

#include "parser.h"
#include "meshobj.h"

#include <list>

using namespace std;


#define BUFFER_SIZE 256

//#########################################################################
//## define

#define T_ASE					"*3DSMAX_ASCIIEXPORT"
#define T_SCENE					"*SCENE"
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
#define T_HELPEROBJECT			"*HELPEROBJECT"
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


#define BEGIN_PARSING()			_ClearBuffer();	\
								while( fgets( m_cLineBuffer, BUFFER_SIZE, m_fpFile ) != NULL &&	\
										m_cLineBuffer[0] != '\0' )	\
								{	\
									char cTailTemp[255];\
									sscanf( m_cLineBuffer, "%s%s", m_cCompBuffer, cTailTemp );	\
									if( _stricmp( m_cCompBuffer, "}" ) == 0 )	\
									{	\
										_ClearBuffer();\
										return;	\
									}														

#define END_PARSING()				if( _stricmp( cTailTemp, "{" ) == 0 )	\
										if( m_cCompBuffer[0] == '*' )	\
											_Parse_WASTE();	\
									_ClearBuffer();	\
								}

//#########################################################################

//## MATERIAL ����ü
struct S_MATERIAL {
	UINT				uiId;
	D3DMATERIAL9		Mtrl;
	char				*pTexFile;

	S_MATERIAL			*pSubMtrl;
	int					nNumSubMtrl;		//## submaterial ��

	S_MATERIAL()
	{
		uiId = 0;
		pTexFile = NULL;
		pSubMtrl = NULL;
		nNumSubMtrl = 0;
		ZeroMemory( &Mtrl, sizeof(D3DMATERIAL9) );
		Mtrl.Diffuse.r = Mtrl.Ambient.r = 1.0f;
		Mtrl.Diffuse.g = Mtrl.Ambient.g = 1.0f;
		Mtrl.Diffuse.b = Mtrl.Ambient.b = 1.0f;
		Mtrl.Diffuse.a = Mtrl.Ambient.a = 1.0f;
	}

	~S_MATERIAL()
	{
		SAFE_DELETE_ARRAY(pTexFile);

		if(nNumSubMtrl>1)
		{
            SAFE_DELETE_ARRAY(pSubMtrl);
		}
		else if(nNumSubMtrl==1)
		{
			SAFE_DELETE(pSubMtrl);
		}
	}
};

struct S_MESH_NORMAL {
	D3DXVECTOR3					vFaceNormal;
	D3DXVECTOR3					vVertexNormal[3];
};

//## �޽��� �� ����ü
struct S_MESH_FACE {
	S_CUSTOMINDEX				Index;			//## �޽� face �ε���
	S_CUSTOMINDEX				TexIndex;		//## �ؽ��� uv ��ǥ �ε��� 
	//D3DXVECTOR3					vN;				//## ���� normal ����
	S_MESH_NORMAL				FaceNormal;

	UINT						uiSubMtrlId;	//## �ش� ���� submaterial�� ID
};

//## texture ��ǥ ����ü
struct S_MESH_TUV {

	float fTu,fTv;

	S_MESH_TUV &operator=(const S_MESH_TUV &Tuv)
	{
		fTu = Tuv.fTu;
		fTv = Tuv.fTv;

		return *this;
	}
};

//## GEOMOBJECT ����ü
struct S_GEOMOBJECT {
	//## BASE DATA
	char				*pNodeName;			//## ��� �̸� - ��������� �̷���� �� ���Ǵ� �ε���
	char				*pParentNodeName;	//## ���� ������ �� �θ� ��� �̸�

	//## MATERIAL
	int					nMtrlId;			//## MATERIAL ID

	//## MESH DATA
	D3DXMATRIXA16		matTm;				//## tm ���
	D3DXMATRIXA16		matInvTm;			//## inverse tm ���
	int					nNumVertex;			//## VERTEX ��
	int					nNumFace;			//## FACE ��
	int					nNumTVertex;		//## texture vertex ��
	int					nNumTFace;			//## texture face ��
	D3DXVECTOR3			*pvVertices;			//## VERTEX ������
	S_MESH_FACE			*pFace;				//## Face ������
	S_MESH_TUV			*pTuv;				//## texuture uv ��ǥ ������

	S_GEOMOBJECT()
	{
		pNodeName = NULL;
		pParentNodeName = NULL;

		nMtrlId = 0;

		D3DXMatrixIdentity(&matTm);
		D3DXMatrixIdentity(&matInvTm);

		nNumVertex = 0;
		nNumFace = 0;
		nNumTVertex = 0;
		nNumTFace = 0;

		pvVertices = NULL;
		pFace =	NULL;
		pTuv = NULL;
	}

	~S_GEOMOBJECT()
	{
		SAFE_DELETE_ARRAY(pNodeName);
		SAFE_DELETE_ARRAY(pParentNodeName);
		SAFE_DELETE_ARRAY(pvVertices);
		SAFE_DELETE_ARRAY(pFace);
		SAFE_DELETE_ARRAY(pTuv);
	}
};


typedef list<S_GEOMOBJECT *>		GeomObjList;
typedef	GeomObjList::iterator		GeomObjListItor;

//####################################################
//####################################################
// class CAseParser
// CParser���� �Ļ�
//####################################################
//####################################################
class CAseParser : public CParser
{
private:
	inline void _ClearBuffer();
	void _TransformToLocal( S_GEOMOBJECT *pGeomObj );
	bool _BuildTexture(LPDIRECT3DDEVICE9 pd3dDevice, 
							  char *pFilePathName,
							  LPDIRECT3DTEXTURE9 *pTexture );
	//## SUBMATERIAL ����� ���� ���� ���� ���̱� ���� �Լ�
	S_GEOMOBJECT *_OptimizeVertices( S_GEOMOBJECT *pGeomObj );


	//## ������ ó��
	void _Parse_WASTE();
	void _Check_MAP(S_MATERIAL *pCurMtrl);
	
	//## SCENE
	void _Parse_SCENE();

	//## MATERIAL
	void _Parse_MATERIAL_LIST();
	void _Parse_MATERIAL(S_MATERIAL *pCurMtrl);
	void _Parse_MAP_DIFFUSE(S_MATERIAL *pCurMtrl);

	//## GEOMOBJECT
	void _Parse_GEOMOBJECT();
	void _Parse_NODE_TM( S_GEOMOBJECT *pGeomObj );
	void _Parse_MESH( S_GEOMOBJECT *pGeomObj );
	void _Parse_MESH_VERTEX_LIST( S_GEOMOBJECT *pGeomObj );
	void _Parse_MESH_FACE_LIST( S_GEOMOBJECT *pGeomObj );
	void _Parse_MESH_TVERTLIST( S_GEOMOBJECT *pGeomObj );
	void _Parse_MESH_TFACELIST( S_GEOMOBJECT *pGeomObj );
	void _Parse_MESH_NORMALS( S_GEOMOBJECT *pGeomObj );

	//## ANIMATION
	void _Parse_TM_ANIMATION();
	void _Parse_CONTROL_ROT_TCB();
	void _Parse_CONTROL_POS_TRACK();
	void _Parse_CONTROL_ROT_TRACK();

	//## CONVERTING
	CMeshObj *_CreateModel( CModel *pModel, S_GEOMOBJECT *pGeomObj );
	void _ProcessSubMaterial(	CModel *pModel,
								S_MATERIAL *pMtrl,
								S_GEOMOBJECT *pGeomObj,
								LPDIRECT3DDEVICE9 pd3dDevice );

public:
	CAseParser( const char *pFilePathName );
	~CAseParser(void);

	void Clear();
	void Destroy();

	bool Run();
	bool Convert( CModel *pModel, LPDIRECT3DDEVICE9 pd3dDevice );

private:
	bool							m_bTexture;
	int								m_nMtrlCount;
	S_MATERIAL						*m_pMtrl;					//## material ������
	GeomObjList						m_GeomObjList;

	D3DXVECTOR3						m_vMax;
	D3DXVECTOR3						m_vMin;

	char							m_cLineBuffer[BUFFER_SIZE];
	char							m_cCompBuffer[BUFFER_SIZE];
};

#endif