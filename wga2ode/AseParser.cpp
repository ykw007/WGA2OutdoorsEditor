#include "StdAfx.h"
#include "aseparser.h"


CAseParser::CAseParser( const char *pFilePathName ) : CParser( pFilePathName )
{
	Clear();
}

CAseParser::~CAseParser(void)
{
	Destroy();
}

void CAseParser::Clear()
{
	m_bTexture = false;
	m_nMtrlCount = 0;
	m_pMtrl = NULL;
	m_fpFile = NULL;

	m_vMax = D3DXVECTOR3( -1000.0f, -1000.0f, -1000.0f );
	m_vMin = D3DXVECTOR3( 1000.0f, 1000.0f, 1000.0f );
}

void CAseParser::Destroy()
{
	if( m_nMtrlCount > 1 )
	{
		SAFE_DELETE_ARRAY( m_pMtrl );
	}
	else if( m_nMtrlCount == 1 )
	{
		SAFE_DELETE( m_pMtrl );
	}

	if( !m_GeomObjList.empty() )
		stl_wipe( m_GeomObjList );
}


inline void CAseParser::_ClearBuffer()
{
	memset( m_cLineBuffer, 0, BUFFER_SIZE );
	memset( m_cCompBuffer, 0, BUFFER_SIZE );
}

//####################################################
// run parsing
//####################################################
bool CAseParser::Run()
{
	bool bReturn = true;

	_ClearBuffer();

	//## 파일을 연다.
	if( ( m_fpFile = fopen( m_pFilePathName, "r" ) ) == NULL )
	{
		//## 다시 파일을 연다.
		//if( (m_fpFile = fopen(string,"r")) == NULL )
		::MessageBox( NULL, "파일을 열 수 없습니다.", "Error", MB_OK );
		bReturn = false;
	}
		
	if( fgets( m_cLineBuffer, BUFFER_SIZE, m_fpFile ) != NULL )
	{
		sscanf( m_cLineBuffer, "%s", m_cCompBuffer );
	}

	//##################################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## 시작!!!!!!!!!
	//## ASE 파일인지 체크
	if( _stricmp( m_cCompBuffer ,T_ASE ) != 0 )
	{
		::MessageBox( NULL, "ASE 형식이 아닙니다.", "Error", MB_OK );
		bReturn = false;
	}

	_ClearBuffer();

	//## 파일의 끝까지 루프
	while( fgets( m_cLineBuffer, BUFFER_SIZE, m_fpFile ) != NULL )// && )m_cLineBuffer[0] != '\0'
	{
		sscanf( m_cLineBuffer, "%s", m_cCompBuffer );

		//## parsing SCENE
		if( _stricmp( m_cCompBuffer, T_SCENE ) == 0 )
			_Parse_SCENE();

		//## parsing MATERIAL_LIST
		if( _stricmp( m_cCompBuffer, T_MATERIAL_LIST ) == 0 )
			_Parse_MATERIAL_LIST();

		//## parsing GEOMOBJECT
		if( _stricmp( m_cCompBuffer, T_GEOMOBJECT ) == 0 ||
			_stricmp( m_cCompBuffer, T_HELPEROBJECT ) == 0 )
			_Parse_GEOMOBJECT();

		_ClearBuffer();
	}

	fclose( m_fpFile );

	return bReturn;
}

//####################################################
// Parse_WASTE
//####################################################
void CAseParser::_Parse_WASTE()
{
	BEGIN_PARSING();

	END_PARSING();

	return;	
}


//####################################################
// Parse_SCENE
//####################################################
void CAseParser::_Parse_SCENE()
{
	BEGIN_PARSING();

	END_PARSING();

	return;	
}

//####################################################
// Parse_MATERIAL_LIST
//####################################################
void CAseParser::_Parse_MATERIAL_LIST()
{
	int nCount = 0;

	BEGIN_PARSING();

		//## MATERIAL_COUNT 구하기
		if( _stricmp( m_cCompBuffer, T_MATERIAL_COUNT ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &m_nMtrlCount );

			if( m_nMtrlCount > 0 )
			{
				if( m_nMtrlCount == 1 )
					m_pMtrl = new S_MATERIAL;
				else
					m_pMtrl = new S_MATERIAL[m_nMtrlCount];
			}
			else
			{
				m_pMtrl = new S_MATERIAL;
				m_nMtrlCount = 1;
				return;
			}
		}

		//## 각 MATERIAL 구하기
		if( _stricmp( m_cCompBuffer, T_MATERIAL ) == 0 )
		{
			//## material id 저장
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(m_pMtrl[nCount].uiId) );

			//## parsing material
			_Parse_MATERIAL( &(m_pMtrl[nCount]) );

			nCount++;
		}

	END_PARSING();

	return;
}


//####################################################
// Parse_MATERIAL(S_MATERIAL *pCurMtrl)
//####################################################
void CAseParser::_Parse_MATERIAL(S_MATERIAL *pCurMtrl)
{
	int	nCount = 0;
	int	nTemp=0;

	BEGIN_PARSING();

		//## MATERIAL_AMBIENT
		if( _stricmp( m_cCompBuffer, T_MATERIAL_AMBIENT ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
				&(pCurMtrl->Mtrl.Ambient.r),
				&(pCurMtrl->Mtrl.Ambient.g),
				&(pCurMtrl->Mtrl.Ambient.b) );
		}

		//## MATERIAL_DIFFUSE
		if( _stricmp( m_cCompBuffer, T_MATERIAL_DIFFUSE ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
				&(pCurMtrl->Mtrl.Diffuse.r),
				&(pCurMtrl->Mtrl.Diffuse.g),
				&(pCurMtrl->Mtrl.Diffuse.b) );
		}

		//## MATERIAL_SPECULAR
		if( _stricmp( m_cCompBuffer, T_MATERIAL_SPECULAR ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
				&(pCurMtrl->Mtrl.Specular.r),
				&(pCurMtrl->Mtrl.Specular.g),
				&(pCurMtrl->Mtrl.Specular.b) );
		}

		//## SUB MATERIAL 수
		if( _stricmp( m_cCompBuffer, T_NUMSUBMTLS ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pCurMtrl->nNumSubMtrl) );

			//## material data 메모리 할당
			if ( pCurMtrl->nNumSubMtrl > 1 )
                pCurMtrl->pSubMtrl = new S_MATERIAL[pCurMtrl->nNumSubMtrl];
			else if( pCurMtrl->nNumSubMtrl == 1 )
				pCurMtrl->pSubMtrl = new S_MATERIAL;
		}

		//## SUB MATERIAL
		if( _stricmp( m_cCompBuffer, T_SUBMATERIAL ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer,
				&(pCurMtrl->pSubMtrl[nCount].uiId) );

			_Parse_MATERIAL( &(pCurMtrl->pSubMtrl[nCount]) );

			nCount++;
		}

		//## *MAP_ { 이런 형태만 골라낸다.
		//## 그래서 TEXTURE 파일을 얻는다.
		_Check_MAP(pCurMtrl);

	END_PARSING();

	return;
}

void CAseParser::_Check_MAP(S_MATERIAL *pCurMtrl)
{
		//## *MAP_ { 이런 형태만 골라낸다.
		//## 그래서 TEXTURE 파일을 얻는다.
		char cTemp[255], cTemp2[255];

		sscanf(m_cLineBuffer,"%s%s",cTemp,cTemp2);
		if( _stricmp( cTemp2, "{" ) == 0 )
		{
			char	*pStr = NULL;
			char	*pToken = NULL;
			char	sep[] = "_";

			pToken = strtok( cTemp, sep );

			while( pToken != 0 )
			{
				pStr = pToken;
				if( _stricmp( pStr, "*MAP" )==0 )
				{
					_Parse_MAP_DIFFUSE(pCurMtrl);
					break;
				}

				pToken = strtok( 0, sep);
			}
		}
}

//####################################################
// texture
//####################################################
void CAseParser::_Parse_MAP_DIFFUSE(S_MATERIAL *pCurMtrl)
{
	BEGIN_PARSING();

		//## 현재 MATERIAL의 텍스쳐가 지정되어 있지 않고 BITMAP 구문과 일치하면
		if( pCurMtrl->pTexFile == NULL && _stricmp( m_cCompBuffer, T_BITMAP ) == 0 )
		{
			//## 파일 경로에 공백도 들어갈 수 있기때문에....
			//## "로 추출
			char *pStr = NULL;
			char sep[] = "\"";

			pStr = strtok( m_cLineBuffer, sep );
			pStr = strtok( 0, sep);

			if( strlen(pStr) > 0 )
			{
				SAFE_DELETE_ARRAY( pCurMtrl->pTexFile );
				pCurMtrl->pTexFile = new char[ strlen(pStr) + 1 ];
				strcpy( pCurMtrl->pTexFile, pStr );

				//## 텍스쳐가 있으므로 텍스쳐 관련 구문을 읽을 것이다.
				//m_bTexture = true;
			}
		}

		_Check_MAP(pCurMtrl);;

	END_PARSING();

	return;
}

//####################################################
// _Parse_GEOMOBJECT()
//####################################################
void CAseParser::_Parse_GEOMOBJECT()
{
	char	cTemp[255];
	char	*pStr;
	char	*pNodeName = NULL;

	//## GEOMOBJECT 구조체 메모리 할당
	S_GEOMOBJECT *pGeomObj = new S_GEOMOBJECT;

	//## LIST에 저장
	m_GeomObjList.push_back( pGeomObj );

	//#############################################################
	//##
	BEGIN_PARSING();

		//## NODE_NAME
		if( _stricmp( m_cCompBuffer, T_NODE_NAME ) == 0 )
		{
			if( pGeomObj->pNodeName == NULL )
			{
				sscanf( m_cLineBuffer, "%s%s", m_cCompBuffer, cTemp );
				pStr = strtok( cTemp, "\"");

				pNodeName = new char[ strlen(pStr) + 1 ];
				strcpy( pNodeName, pStr );
				pGeomObj->pNodeName = pNodeName;
			}
		}

		//## NODE_PARENT NAME
		if( _stricmp( m_cCompBuffer, T_NODE_PARENT ) == 0 )
		{

			sscanf( m_cLineBuffer, "%s%s", m_cCompBuffer, cTemp );
			pStr = strtok( cTemp, "\"");

			pNodeName = new char[ strlen(pStr) + 1 ];
			strcpy( pNodeName, pStr );
			pGeomObj->pParentNodeName = pNodeName;
		}

		//## NODE_TM
		if( _stricmp( m_cCompBuffer, T_NODE_TM ) == 0 )
			_Parse_NODE_TM( pGeomObj );

		//## MESH
		if( _stricmp( m_cCompBuffer, T_MESH ) == 0 )
		{
			_Parse_MESH( pGeomObj );

			//## tm 의 역행렬 구하고 local 좌표계로 변환
			_TransformToLocal( pGeomObj );
		}

		//## MATERIAL_REF
		if( _stricmp( m_cCompBuffer, T_MATERIAL_REF ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pGeomObj->nMtrlId) );
		}

		//## TM_ANIMATION
		if( _stricmp( m_cCompBuffer, T_TM_ANIMATION ) == 0 )
		{
			_Parse_TM_ANIMATION();
		}

	END_PARSING();
	//##
	//#############################################################


	return;
}

//####################################################
// _Parse_NODE_TM( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_NODE_TM( S_GEOMOBJECT *pGeomObj )
{
	BEGIN_PARSING();

		//## TM 행렬 ROW 0
		if( _stricmp( m_cCompBuffer, T_TM_ROW0 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._11),
												&(pGeomObj->matTm._13),
												&(pGeomObj->matTm._12));
		}

		//## TM 행렬 ROW 1
		if( _stricmp( m_cCompBuffer, T_TM_ROW1 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._31),
												&(pGeomObj->matTm._33),
												&(pGeomObj->matTm._32));
		}

		//## TM 행렬 ROW 2
		if( _stricmp( m_cCompBuffer, T_TM_ROW2 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._21),
												&(pGeomObj->matTm._23),
												&(pGeomObj->matTm._22));
		}

		//## TM 행렬 ROW 3
		if( _stricmp( m_cCompBuffer, T_TM_ROW3 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._41),
												&(pGeomObj->matTm._43),
												&(pGeomObj->matTm._42));
		}

	END_PARSING();

	return;
}

//####################################################
// _Parse_MESH( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_MESH( S_GEOMOBJECT *pGeomObj )
{
	int i=0;
	int nCount=0;

	BEGIN_PARSING();

		//## MESH_NUMVERTEX
		if( _stricmp( m_cCompBuffer, T_MESH_NUMVERTEX ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pGeomObj->nNumVertex) );
		}

		//## MESH_VERTEX_LIST
		if( _stricmp( m_cCompBuffer, T_MESH_VERTEX_LIST ) == 0 )
		{
			_Parse_MESH_VERTEX_LIST( pGeomObj );
		}

		//## MESH_NUMFACE
		if( _stricmp( m_cCompBuffer, T_MESH_NUMFACE ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pGeomObj->nNumFace) );
		}

		//## MESH_FACE_LIST
		if( _stricmp( m_cCompBuffer, T_MESH_FACE_LIST ) == 0 )
		{
			_Parse_MESH_FACE_LIST( pGeomObj );
		}


		//## MESH_NUMTVERTEX
		if( _stricmp( m_cCompBuffer, T_MESH_NUMTVERTEX ) == 0 )
		{
			//if( !m_bTexture )
			//	pGeomObj->nNumTVertex = 0;
			//else
				sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pGeomObj->nNumTVertex) );
		}

		//## MESH_TVERTLIST
		if( _stricmp( m_cCompBuffer, T_MESH_TVERTLIST ) == 0 )
		{
			_Parse_MESH_TVERTLIST( pGeomObj );
		}

		//## MESH_NUMTVFACES
		if( _stricmp( m_cCompBuffer, T_MESH_NUMTVFACES ) == 0 )
		{
			//if( !m_bTexture )
			//	pGeomObj->nNumTFace = 0;
			//else
                sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pGeomObj->nNumTFace) );
		}

		//## MESH_TFACELIST
		if( _stricmp( m_cCompBuffer, T_MESH_TFACELIST ) == 0 )
		{
			_Parse_MESH_TFACELIST( pGeomObj );
		}


		//## MESH_NORMALS
		if( _stricmp( m_cCompBuffer, T_MESH_NORMALS ) == 0 )
		{
			_Parse_MESH_NORMALS( pGeomObj );
		}


	END_PARSING();

	return;
}

//####################################################
// _Parse_MESH_VERTEX_LIST( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_MESH_VERTEX_LIST( S_GEOMOBJECT *pGeomObj )
{
	int nCount;
	float fX,fY,fZ;

	//## 정점 버퍼 메모리를 할당한다.
	pGeomObj->pvVertices = new D3DXVECTOR3[ pGeomObj->nNumVertex ];

	BEGIN_PARSING();

		//## MESH_VERTEX
		if( _stricmp( m_cCompBuffer, T_MESH_VERTEX ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d%f%f%f", m_cCompBuffer, &nCount,&fX,&fZ,&fY);

			pGeomObj->pvVertices[nCount].x = fX;
			pGeomObj->pvVertices[nCount].y = fY;
			pGeomObj->pvVertices[nCount].z = fZ;
		}

	END_PARSING();
}

//####################################################
// _Parse_MESH_FACE_LIST( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_MESH_FACE_LIST( S_GEOMOBJECT *pGeomObj )
{
	char	cSmooth[20];
	char	cMtrlID[20];
	int		nID,nI0,nI1,nI2,nTemp;
	int	nCount = 0;

	//## 면 구조체 메모리 할당
	pGeomObj->pFace = new S_MESH_FACE[ pGeomObj->nNumFace ];

	BEGIN_PARSING();

		// MESH_FACE
		if( _stricmp( m_cCompBuffer, T_MESH_FACE ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d",
				m_cCompBuffer, m_cCompBuffer,
				m_cCompBuffer, &nI0,
				m_cCompBuffer, &nI2,
				m_cCompBuffer, &nI1,
				m_cCompBuffer, &nTemp,
				m_cCompBuffer, &nTemp,
				m_cCompBuffer, &nTemp,
				m_cCompBuffer, cSmooth,
				cMtrlID, &nID);

			//## 인덱스 정보
			pGeomObj->pFace[ nCount ].Index._0 = nI0;
			pGeomObj->pFace[ nCount ].Index._1 = nI1;
			pGeomObj->pFace[ nCount ].Index._2 = nI2;

			//## smooth 정보가 가끔 생략되므로 
			if(_strcmpi( cMtrlID, T_MESH_MTLID )==0)
				pGeomObj->pFace[ nCount ].uiSubMtrlId = nID;
			
			if(_strcmpi( cSmooth, T_MESH_MTLID )==0)
				pGeomObj->pFace[ nCount ].uiSubMtrlId = atoi(cMtrlID);

			nCount++;
		}

	END_PARSING();

	return;
}

//####################################################
// _Parse_MESH_TVERTLIST( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_MESH_TVERTLIST( S_GEOMOBJECT *pGeomObj )
{
	int nCount;
	float fU,fV,fW;

	//## uv 좌표를 위한 메모리 할당
	if( pGeomObj->nNumTVertex > 0 )//m_bTexture && 
        pGeomObj->pTuv = new S_MESH_TUV[ pGeomObj->nNumTVertex ];

	BEGIN_PARSING();

		//## MESH_TVERTLIST
		if(  _stricmp( m_cCompBuffer, T_MESH_TVERT ) == 0 )//m_bTexture &&
		{
			sscanf( m_cLineBuffer, "%s%d%f%f%f", m_cCompBuffer, &nCount, &fU, &fV, &fW );
			pGeomObj->pTuv[nCount].fTu = fU;
			pGeomObj->pTuv[nCount].fTv = 1.0f - fV;
		}

	END_PARSING();

	return;
}

//####################################################
// _Parse_MESH_TFACELIST( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_MESH_TFACELIST( S_GEOMOBJECT *pGeomObj )
{
	int nCount;
	int nI0,nI1,nI2;

	BEGIN_PARSING();

		//## MESH_TFACELIST
		if( _stricmp( m_cCompBuffer, T_MESH_TFACE ) == 0 )// m_bTexture &&
		{
			sscanf( m_cLineBuffer, "%s%d%d%d%d", m_cCompBuffer, &nCount, &nI0, &nI2, &nI1 );
			pGeomObj->pFace[nCount].TexIndex._0 = nI0;
			pGeomObj->pFace[nCount].TexIndex._1 = nI1;
			pGeomObj->pFace[nCount].TexIndex._2 = nI2;
		}

	END_PARSING();

	return;
}

//####################################################
// _Parse_MESH_NORMALS( S_GEOMOBJECT *pGeomObj )
//####################################################
void CAseParser::_Parse_MESH_NORMALS( S_GEOMOBJECT *pGeomObj )
{
	int	nFaceCount;
	int nVertexIndex;
	int nCount;
	float	fX,fY,fZ;

	BEGIN_PARSING();

		//## MESH_FACENORMAL
		if( _stricmp( m_cCompBuffer, T_MESH_FACENORMAL ) == 0 )
		{
			nCount = 0;
			sscanf( m_cLineBuffer, "%s%d%f%f%f", m_cCompBuffer, &nFaceCount, &fX, &fZ, &fY );
			pGeomObj->pFace[nFaceCount].FaceNormal.vFaceNormal.x = fX;
			pGeomObj->pFace[nFaceCount].FaceNormal.vFaceNormal.y = fY;
			pGeomObj->pFace[nFaceCount].FaceNormal.vFaceNormal.z = fZ;
		}

		//## MESH_VERTEXNORMAL
		if( _stricmp( m_cCompBuffer, T_MESH_VERTEXNORMAL ) == 0 )
		{
			if( nCount == 1 )
				nCount = 2;
			else if( nCount == 3 )
				nCount = 1;

			sscanf( m_cLineBuffer, "%s%d%f%f%f", m_cCompBuffer, &nVertexIndex, &fX, &fZ, &fY );
			pGeomObj->pFace[nFaceCount].FaceNormal.vVertexNormal[nCount].x = fX;
			pGeomObj->pFace[nFaceCount].FaceNormal.vVertexNormal[nCount].y = fY;
			pGeomObj->pFace[nFaceCount].FaceNormal.vVertexNormal[nCount].z = fZ;

			nCount++;
		}

	END_PARSING();

	return;
}

//####################################################
// _Parse_TM_ANIMATION()
//####################################################
void CAseParser::_Parse_TM_ANIMATION()
{
	BEGIN_PARSING();

		if( _stricmp( m_cCompBuffer, T_CONTROL_ROT_TCB ) == 0 )
		{
			_Parse_CONTROL_ROT_TCB();
		}

		if( _stricmp( m_cCompBuffer, T_CONTROL_POS_TRACK ) == 0 )
		{
			_Parse_CONTROL_POS_TRACK();
		}

		if( _stricmp( m_cCompBuffer, T_CONTROL_ROT_TRACK ) == 0 )
		{
			_Parse_CONTROL_ROT_TRACK();
		}

	END_PARSING();

	return;
}

//####################################################
// _Parse_CONTROL_ROT_TCB()
//####################################################
void CAseParser::_Parse_CONTROL_ROT_TCB()
{
	BEGIN_PARSING();

	END_PARSING();

	return;
}

//####################################################
// _Parse_CONTROL_POS_TRACK()
//####################################################
void CAseParser::_Parse_CONTROL_POS_TRACK()
{
	BEGIN_PARSING();

	END_PARSING();

	return;
}

//####################################################
// _Parse_CONTROL_ROT_TRACK()
//####################################################
void CAseParser::_Parse_CONTROL_ROT_TRACK()
{
	BEGIN_PARSING();

	END_PARSING();

	return;
}

//####################################################
// _TransformToLocal( S_GEOMOBJECT *pGeomObj )
// 3d max의 world 좌표를 local 좌표로 변환한다.
//####################################################
void CAseParser::_TransformToLocal( S_GEOMOBJECT *pGeomObj )
{
	D3DXVECTOR3 vWorld;
	D3DXVECTOR3 vLocal;
	D3DXMATRIXA16 matInvTm;		//## tm 의 역행렬

	//## tm의 역행렬을 구한다.
	D3DXMatrixInverse( &matInvTm, 0, &(pGeomObj->matTm) );
	pGeomObj->matInvTm = matInvTm;

	//## 정점 수 만큼 loop
	for(int i = 0; i < pGeomObj->nNumVertex; i++ )
	{
		//## 객체의 world 정점 좌표들이다.
		vWorld = pGeomObj->pvVertices[i];

		//## local 좌표로 변환한다. 
		D3DXVec3TransformCoord( &vLocal, &vWorld, &matInvTm );

		//## 담는다.
		pGeomObj->pvVertices[i] = vLocal;

		if( vLocal.x > m_vMax.x )
            m_vMax.x = vLocal.x;

		if( vLocal.y > m_vMax.y )
			m_vMax.y = vLocal.y;

		if( vLocal.z > m_vMax.z )
			m_vMax.z = vLocal.z;

		if( vLocal.x < m_vMin.x )
			m_vMin.x = vLocal.x;

		if( vLocal.y < m_vMin.y )
			m_vMin.y = vLocal.y;

		if( vLocal.z < m_vMin.z )
			m_vMin.z = vLocal.z;
	}
}

bool CAseParser::_BuildTexture(LPDIRECT3DDEVICE9 pd3dDevice, 
							  char *pFilePathName,
							  LPDIRECT3DTEXTURE9 *pTexture )
{
	//## texture를 생성한다.
	if( FAILED( D3DXCreateTextureFromFileEx( pd3dDevice, pFilePathName,
											 D3DX_DEFAULT, D3DX_DEFAULT,
											 1, 0,
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											 D3DX_DEFAULT, D3DX_DEFAULT,
											 0, NULL, NULL, pTexture ) ) )
	{
		//## 경로명이 틀리다면 파일명만 가지고 현재 경로에서 찾는다.
		char	*pStr = NULL;
		char	*pToken = NULL;
		char	sep[] = "\\";

		pToken = strtok( pFilePathName, sep );

		while( pToken != 0 )
		{
			pStr = pToken;
			pToken = strtok( 0, sep);
		}

		if( FAILED( D3DXCreateTextureFromFileEx( pd3dDevice, pStr,
												 D3DX_DEFAULT, D3DX_DEFAULT,
												 1, 0,
												 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
												 D3DX_DEFAULT, D3DX_DEFAULT,
												 0, NULL, NULL, pTexture ) ) )
		{
			return false;
		}

		strcpy( pFilePathName, pStr );
	}

	return true;
}

//####################################################
// CModel 클래스로 변환하는 함수
// TEXTURE 생성을 위해
// LPDIRECT3DDEVICE9 pd3dDevice를 받는다.
//####################################################
bool CAseParser::Convert( CModel *pModel, LPDIRECT3DDEVICE9 pd3dDevice )
{
	S_GEOMOBJECT *pGeomObj;

	S_MATERIAL_MESH *pMtrlMesh;

	//## MeshObjList 포인터
	MeshObjList *pMeshObjList = NULL;

	//## GEOMOBJECT LIST iterator
	GeomObjListItor pi;

	int nCount;

	//## material loop
	for( int i = 0; i < m_nMtrlCount; i++ )
	{
		nCount = 0;

		pMtrlMesh = NULL;

		//## GEOMOBJECT LIST loop
		for( pi = m_GeomObjList.begin(); pi != m_GeomObjList.end(); ++pi )
		{
			//## 현재 GEOMOBJECT 포인터
			pGeomObj = static_cast<S_GEOMOBJECT *>(*pi);

			//## 현재 GEOMOBJECT의 MATERIAL ID 비교
			if( pGeomObj->pvVertices != NULL && m_pMtrl[i].uiId == pGeomObj->nMtrlId )
			{
				//## submaterial이 존재하면
				if( m_pMtrl[i].nNumSubMtrl > 0 )
				{
					//## submaterial 처리
					_ProcessSubMaterial( pModel, &(m_pMtrl[i]), pGeomObj, pd3dDevice );
				}
				else
				{

					//## nCount == 0 이면 CMeshObj list를 생성
					if( nCount == 0 )
					{
						pMeshObjList = NULL;
						pMtrlMesh = new S_MATERIAL_MESH;
						pMeshObjList = pModel->CreateMeshObjList();
						pMtrlMesh->pObjList = pMeshObjList;

						//## material 정보를 넘긴다.
						//## 참조 정보를 넘기지 말고 값을 넘겨야 한다.
						//## parser를 사용후 제거된다.
						pMtrlMesh->Mtrl = m_pMtrl[i].Mtrl;

						if( m_pMtrl[i].pTexFile != NULL )
						{
							pMtrlMesh->pTexFile = new char[ strlen( m_pMtrl[i].pTexFile ) + 1 ];
							strcpy( pMtrlMesh->pTexFile, m_pMtrl[i].pTexFile );

							//######################################################################
							//## texture 생성
							if( !_BuildTexture( pd3dDevice, pMtrlMesh->pTexFile, &pMtrlMesh->pTexture ) )
								::MessageBox(NULL, "텍스쳐 생성 실패! 그냥 진행합니다.","error",MB_OK);
						}
					}

					if( pMeshObjList != NULL )
						pMeshObjList->push_back( _CreateModel( pModel, pGeomObj ) );

					nCount++;
				}

			}//## if 현재 GEOMOBJECT의 MATERIAL ID 비교

		}//## for GEOMOBJECT LIST loop
		
		//## 다음 material 로 넘어가기 전에 MeshObjList 저장
		if( pMtrlMesh != NULL )
            pModel->GetMtrlMeshListPtr()->push_back( pMtrlMesh );

	}//## for material loop
	

	return true;
}

//####################################################
// SUBMATERIAL 처리
//####################################################
void CAseParser::_ProcessSubMaterial(CModel *pModel,
									 S_MATERIAL *pMtrl,
									 S_GEOMOBJECT *pGeomObj,
									 LPDIRECT3DDEVICE9 pd3dDevice )
{
	//## submaterial이 적용된 face 수를 얻기위한 임시 변수
	BYTE *pbySubIdList = new BYTE[ pGeomObj->nNumFace ];
	int *pnNumSubMtrl = new int[ pMtrl->nNumSubMtrl ];
	ZeroMemory( pnNumSubMtrl, sizeof(int)*pMtrl->nNumSubMtrl);

	BYTE byMtrlId;
	//## 모든 face를 LOOP
	for( int k = 0; k < pGeomObj->nNumFace; k++ )
	{
		byMtrlId = pGeomObj->pFace[k].uiSubMtrlId;

		//## face의 SUBMATERIAL ID와 FACE COUNT 변수와 매핑
		pnNumSubMtrl[ byMtrlId ]++;

		pbySubIdList[k] = byMtrlId;
	}

	//## face 구조체 시작 pointer를 담을 메모리 생성
	S_MESH_FACE **paNewFace =  new S_MESH_FACE*[ pMtrl->nNumSubMtrl ];

	//## 각 submaterial이 적용된 face 수 만큼 메모리 할당
	for( int loop = 0; loop < pMtrl->nNumSubMtrl; loop++ )
	{
		paNewFace[loop] = new S_MESH_FACE[ pnNumSubMtrl[ loop ] ];

	}

	//#######################################################################
	//## submaterial에 따라 덩어리 별로 분리한 할당된 S_MESH_FACE 메모리에 
	//## face 데이타를 담는다.

	//## face index의 offset을 구하기 위한 임시 카운터
	int *pnCount = new int[ pMtrl->nNumSubMtrl ];
	for( int m = 0; m < pMtrl->nNumSubMtrl; m++ )
	{
		pnCount[m] = pnNumSubMtrl[ m ];
	}

	//## 현재 face pointer
	S_MESH_FACE *pCurMeshFace;

	//#############################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## 각 submaterial 별로 할당한 메모리에 face 정보를 담는다.
	//## 한 큐에 끝내버린다.
	for( int i = 0; i < pGeomObj->nNumFace; i++ )
	{

		//## submaterial에 적용된 face 수가 0보다 크면 다음으로 진행
		if( pnCount[ pbySubIdList[i] ] > 0 )
		{
			//## 다음 메모리 시작 번지를 알 수 있도록 설정해 놓은 offset
			int nOffset = pnNumSubMtrl[ pbySubIdList[i] ] - pnCount[ pbySubIdList[i] ] ;

			//## 새로 face정보를 담을 S_MESH_FACE 포인터를 구한다.
			pCurMeshFace = paNewFace[pbySubIdList[i]] + nOffset;

			//################################################################################
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//## FACE 정보를 담는다.
			pCurMeshFace->Index._0 = pGeomObj->pFace[i].Index._0;
			pCurMeshFace->Index._1 = pGeomObj->pFace[i].Index._1;
			pCurMeshFace->Index._2 = pGeomObj->pFace[i].Index._2;

			//## texture가 있으면
			if( pGeomObj->pTuv != NULL )//pCurMeshFace->pMtrl->pSubMtrl[ pbySubIdList[i] ].pTexFile != NULL )
			{
				pCurMeshFace->TexIndex._0 = pGeomObj->pFace[i].TexIndex._0;
				pCurMeshFace->TexIndex._1 = pGeomObj->pFace[i].TexIndex._1;
				pCurMeshFace->TexIndex._2 = pGeomObj->pFace[i].TexIndex._2;
			}

			pCurMeshFace->uiSubMtrlId = pGeomObj->pFace[i].uiSubMtrlId;

			//## normal 값을 담는다.
			pCurMeshFace->FaceNormal.vFaceNormal 
					= pGeomObj->pFace[i].FaceNormal.vFaceNormal;
			pCurMeshFace->FaceNormal.vVertexNormal[0]
				= pGeomObj->pFace[i].FaceNormal.vVertexNormal[0];
			pCurMeshFace->FaceNormal.vVertexNormal[1]
				= pGeomObj->pFace[i].FaceNormal.vVertexNormal[1];
			pCurMeshFace->FaceNormal.vVertexNormal[2]
				= pGeomObj->pFace[i].FaceNormal.vVertexNormal[2];

			//## FACE 정보를 담는다.
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//################################################################################

			//## 하나씩 접근해 나가기 위해서 face 수를 감소시킨다.
			pnCount[ pbySubIdList[i] ]--;
		}
	}
	//## 각 submaterial 별로 할당한 메모리에 face 정보를 담는다.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//#############################################################################

	SAFE_DELETE_ARRAY( pGeomObj->pFace );
	SAFE_DELETE_ARRAY( pnCount );
	SAFE_DELETE_ARRAY( pbySubIdList );

	//## material 구조체에 담아보자.
	S_MATERIAL_MESH *pMtrlMesh;

	//## MeshObjList 포인터
	MeshObjList *pMeshObjList = NULL;
	//###########################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## 이제 S_GEOMOBJECT 구조체와 연결하여 _CreateModel 함수를 호출한다.
	for( int a = 0; a < pMtrl->nNumSubMtrl; a++ )
	{
		//## submaterial 이 적용된 face 수가 하나라도 있으면
		if( pnNumSubMtrl[ a ] > 0 )
		{
			//## S_MATERIAL_MESH 메모리 할당
			pMtrlMesh = new S_MATERIAL_MESH;

			//## Mesh Object List 메모리 할당
			pMeshObjList = pModel->CreateMeshObjList();

			//## MtrlMesh의 Object list에 연결
			pMtrlMesh->pObjList = pMeshObjList;

			//## material 정보를 넘긴다.
			//## 참조 정보를 넘기지 말고 값을 넘겨야 한다. 왜냐면 이 메모리들은 제거되기때문에
			//## parser를 사용후 제거된다.
			pMtrlMesh->Mtrl = pMtrl->pSubMtrl[a].Mtrl;//m_pMtrl[a].Mtrl;

			if( pMtrl->pSubMtrl[a].pTexFile != NULL )//m_bTexture != NULL &&
			{
				pMtrlMesh->pTexFile = new char[ strlen( pMtrl->pSubMtrl[a].pTexFile ) + 1 ];
				strcpy( pMtrlMesh->pTexFile, pMtrl->pSubMtrl[a].pTexFile );

				//######################################################################
				//## texture 생성
				if( !_BuildTexture( pd3dDevice, pMtrlMesh->pTexFile, &pMtrlMesh->pTexture ) )
					::MessageBox(NULL, "텍스쳐 생성 실패! 그냥 진행합니다.","error",MB_OK);
			}

			//## 현재 S_GEOMOBJECT 의 FACE 포인터에 연결
			pGeomObj->pFace = paNewFace[a];
			pGeomObj->nNumFace = pnNumSubMtrl[ a ];

			//## 정점 수가 너무 많은 경우가 있으니 face에서 적용할 정점을 추려내고
			//## face index도 정점 수에 따라 완전히 바꿀것이다.
			S_GEOMOBJECT *pNewGeomObj = _OptimizeVertices( pGeomObj );

			//## CModel 을 만든다.
			if( pMeshObjList != NULL )
				pMeshObjList->push_back( _CreateModel( pModel, pNewGeomObj ) );

			SAFE_DELETE( pNewGeomObj );
			SAFE_DELETE_ARRAY( pGeomObj->pFace );

			//## 다음 material 로 넘어가기 전에 MeshObjList 저장
			if( pMtrlMesh != NULL )
				pModel->GetMtrlMeshListPtr()->push_back( pMtrlMesh );
		}
	}

	SAFE_DELETE_ARRAY( paNewFace );
	SAFE_DELETE_ARRAY( pnNumSubMtrl );

}

//####################################################
// submaterial 처리시 정점 수 최적화
//####################################################
S_GEOMOBJECT *CAseParser::_OptimizeVertices( S_GEOMOBJECT *pGeomObj )
{

	//############################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## 새로운 S_GEOMOBJECT 구조체를 작성하기 위한 메모리를 할당하고
	//## 원본 S_GEOMOBJECT 구조체로부터 필요한 데이타를 넘겨받는다.

	//## 먼저 넘겨받은 MESH의 총 face 수를 구한다.
	int nNumFace = pGeomObj->nNumFace;


	//## 정점이 이미 등록되었는지 체크하기 위한 임시 메모리
	//## 정점 수 만큼 할당한다.
	int *pnVIndex = new int[ pGeomObj->nNumVertex ];

	//## 그리고 초기화한다. -1로
	for(int k = 0; k < pGeomObj->nNumVertex; k++ )
	{
		pnVIndex[k] = -1;
	}

	//## 여기서 새로운 S_GEOMOBJECT 구조체 메모리를 할당한다.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	S_GEOMOBJECT *pNewGeomObj = new S_GEOMOBJECT;

	//## 여기서 정점 정보를 담을 구조체 메모리를 할당한다.
	//## 정점 수는 최대 FACE * 3 으로 볼수 있다.( 텍스쳐 좌표가 모든 정점이 다 다를 경우 )
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	D3DXVECTOR3 *pVertices = new D3DXVECTOR3[ pGeomObj->nNumVertex ];

	//## FACE 정보를 담을 메모리를 할당한다.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	S_MESH_FACE *pFace = new S_MESH_FACE[ nNumFace ];

	S_MESH_TUV *pTuv = NULL;
	//## 텍스쳐가 적용되었다면 uv 좌표에 대한 메모리도 할당한다.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int *pnUvIndex = NULL;
	if( pGeomObj->pTuv != NULL )
	{
        pTuv = new S_MESH_TUV[ pGeomObj->nNumTVertex  ];

		//## uv 정점이 이미 등록되었는지 체크하기 위한 임시 메모리
		//## uv 정점 수 만큼 할당한다.
		pnUvIndex = new int[ pGeomObj->nNumTVertex ];

		//## 그리고 초기화한다. -1로
		for(int k = 0; k < pGeomObj->nNumTVertex; k++ )
		{
			pnUvIndex[k] = -1;
		}
	}


	//## 이전 메시 정보를 새 메시 구조체 메모리로 담는다.
	pNewGeomObj->matInvTm = pGeomObj->matInvTm;
	pNewGeomObj->matTm = pGeomObj->matTm;
	pNewGeomObj->nMtrlId = pGeomObj->nMtrlId;

	pNewGeomObj->nNumFace = nNumFace;
	pNewGeomObj->nNumTFace = pGeomObj->nNumTFace;

	//## nodename
	if( pGeomObj->pNodeName != NULL )
	{
		//## 노드명을 저장하기위한 메모리 할당
        pNewGeomObj->pNodeName = new char[ strlen(pGeomObj->pNodeName)+1 ];
		strcpy( pNewGeomObj->pNodeName, pGeomObj->pNodeName );
	}
	//## parentnode name
	if( pGeomObj->pParentNodeName != NULL )
	{
		//## 노드명을 저장하기위한 메모리 할당
		pNewGeomObj->pParentNodeName = new char[ strlen(pGeomObj->pParentNodeName)+1 ];
		strcpy( pNewGeomObj->pParentNodeName, pGeomObj->pParentNodeName );
	}

	//## 새 메시 정보에 tvertex 메모리와, vertex 메모리를 연결시킨다.
	pNewGeomObj->pvVertices = pVertices;
	pNewGeomObj->pFace = pFace;
	pNewGeomObj->pTuv = pTuv;

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//############################################################################

	WORD wFaceIndex, wTexIndex;

	//## 정점 수 카운터
	int nNumVertices = 0;
	//## UV 정점 수 카운터
	int nNumUvVertices = 0;

	//####################################################################################
	//## 작업을 시작한다.
	//## face 수 만큼 loop
	for( int i = 0; i < nNumFace; i++ )
	{
		//## 일단 face normal 저장
		pFace[ i ].FaceNormal.vFaceNormal = pGeomObj->pFace[i].FaceNormal.vFaceNormal;

		//## 정점이 3개니까 3번 loop
		for( int j = 0; j < 3; j++ )
		{

			//## 정점 index
			wFaceIndex = *((&(pGeomObj->pFace[i].Index._0))+j);

			//## texture가 적용되는 경우
			//if( pNewGeomObj->nNumTVertex > 0 )
				//## texture 정점 index를 받는다.
			if( pGeomObj->pTuv != NULL )
				wTexIndex = *((&(pGeomObj->pFace[i].TexIndex._0))+j);
		
			//## 각 정점 normal을 받는다.
			pFace[ i ].FaceNormal.vVertexNormal[j]
						= pGeomObj->pFace[i].FaceNormal.vVertexNormal[j];

			//## 이미 등록된 정점인지 flag 값을 통하여 체크한다.
			if( pnVIndex[ wFaceIndex ] > -1 )
			{
				//## 이미에 등록되었다면 index 버퍼에 
				//## pnVIndex배열에 저장해둔 index를 담는다.
				*((&(pFace[ i ].Index._0))+j) = pnVIndex[ wFaceIndex ];
			}
			else
			{
				//## 등록된 정점이 아니라면 순차적으로 등록한다.
				pVertices[nNumVertices] = pGeomObj->pvVertices[ wFaceIndex ];

				//## index도 등록한다.
				*((&(pFace[ i ].Index._0))+j) = nNumVertices;

				//## index 값을 저장하여 두면 -1 이 아니므로
				//## 등록된 정점임을 알 수 있다.
				pnVIndex[ wFaceIndex ] = nNumVertices;

				//## 정점 수를 센다.
				nNumVertices++;
			}

			//## uv 좌표 메모리가 비어있지 않으면 
			if( pGeomObj->pTuv != NULL )
			{
				//## 이미에 등록된 uv정점인지 flag 값을 통하여 체크한다.
				if( pnUvIndex[ wTexIndex ] > -1 )
				{
					//## 이미에 등록되었다면 index 버퍼에 
					//## pnVIndex배열에 저장해둔 index를 담는다.
					*((&(pFace[ i ].TexIndex._0))+j) = pnUvIndex[ wTexIndex ];
				}
				else
				{
					//## 등록된 uv 정점이 아니면 등록한다.
					pTuv[nNumUvVertices] = pGeomObj->pTuv[wTexIndex];

					//## uv index도 저장
					*((&(pFace[ i ].TexIndex._0))+j) = nNumUvVertices;

					//## index 값을 저장하여 두면 -1 이 아니므로
					//## 등록된 정점임을 알 수 있다.
					pnUvIndex[ wTexIndex ] = nNumUvVertices;

					//## UV 정점 수를 센다.
					nNumUvVertices++;
				}
			}
		}
	}	

	//## 최종 추려낸 정점 수가 나온다.
	pNewGeomObj->nNumVertex = nNumVertices;

	if( pGeomObj->pTuv != NULL )
		pNewGeomObj->nNumTVertex = nNumUvVertices;

	//## 메모리를 재할당하여 복사하고 이전 메모리는 제거한다.
	D3DXVECTOR3 *pNewVertices = new D3DXVECTOR3[ nNumVertices ];
	memcpy( pNewVertices, pVertices, sizeof(D3DXVECTOR3)*nNumVertices );
	pNewGeomObj->pvVertices = pNewVertices;

	SAFE_DELETE_ARRAY( pVertices );

	if( nNumUvVertices > 0 )
	{
		S_MESH_TUV *pNewUvVertices = new S_MESH_TUV[ nNumUvVertices ];
		memcpy( pNewUvVertices, pTuv, sizeof(S_MESH_TUV)*nNumUvVertices );
		pNewGeomObj->pTuv = pNewUvVertices;

		SAFE_DELETE_ARRAY( pTuv );
	}

	//## 임시 체크용 메모리를 제거한다.
	SAFE_DELETE_ARRAY( pnVIndex );
	SAFE_DELETE_ARRAY( pnUvIndex );

	//## 새 메시 포인터를 반환한다.
	return pNewGeomObj;
}


//####################################################
// CModel 생성
//####################################################
CMeshObj *CAseParser::_CreateModel( CModel *pModel, S_GEOMOBJECT *pGeomObj )
{

	CMeshObj *pMeshObj = pModel->CreateMeshObj();

	pMeshObj->SetMatrixTM( pGeomObj->matTm );
	pMeshObj->SetMatrixInverseTM( pGeomObj->matInvTm );

	//## bbox
	pMeshObj->SetBbox( &m_vMax, &m_vMin );

	bool bUseTex = ( pGeomObj->pTuv == NULL ) ? false : true;

	//## 일단 기본 버텍스 수부터 시작한다.
	int nNumVertices = pGeomObj->nNumVertex;
	int nNumFace = pGeomObj->nNumFace;

	//## vertex 최대치
	int nMaxVertices = nNumFace * 3;

	//## 만약에 vertex 최대치로 잡은 수보다 pGeomObj->nNumVertex 값이 크면
	if( pGeomObj->nNumVertex > nMaxVertices )
		nMaxVertices = pGeomObj->nNumVertex;

	//## 중복된 정점 수를 담을 메모리를 준비. 임시용이다. 반드시 제거한다.
	int *pnNumVertex = new int[ nMaxVertices ];
	ZeroMemory( pnNumVertex, sizeof(int)*nMaxVertices );	//## 초기화 모두 0로

	//## flag 메모리를 생성한다. 임시용이다. 반드시 제거한다.
	bool *pbFlag = new bool[ nMaxVertices ];

	//## 초기화 모두 false로
	for( int k = 0; k < nMaxVertices; k++ )
		pbFlag[k] = false;

	//## vertex 할당은 최대치로 한다. 임시용이다. 반드시 제거한다.
	S_CUSTOMVERTEX *pVertices;

	if( bUseTex )
		pVertices = new S_CUSTOMVERTEX[ nMaxVertices*2 ];
	else
		pVertices = new S_CUSTOMVERTEX[ nMaxVertices ];

	//## index
	S_CUSTOMINDEX *pIndices = new S_CUSTOMINDEX[ nNumFace ];

	//## 기본 정점과 같은 위치를 가지고 새로 만들어지는 정점 index
	WORD *pwIndexMapOVtx = new WORD[nMaxVertices];

	int nMapOVtxCount = 0;
	int nOrgVertices = nNumVertices;	//## 기본 정점수 백업

	//#################################################################################
	//## 정점 버퍼, 인덱스 버퍼 작성
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## face 수 만큼 loop
	WORD wFaceIndex, wTexIndex, wUpdateIndex;

	for( int i = 0; i < pGeomObj->nNumFace; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			wFaceIndex = *((&(pGeomObj->pFace[i].Index._0))+j);

			if( bUseTex )
				wTexIndex = *((&(pGeomObj->pFace[i].TexIndex._0))+j);

			//## 이미 넣어진 값이면
			if( pbFlag[ wFaceIndex ] == true )
			{
				pnNumVertex[wFaceIndex]++;
				pVertices[ wFaceIndex ].vN +=
					pGeomObj->pFace[i].FaceNormal.vVertexNormal[j];

				if( bUseTex )
				{
					//## uv 값이 같으면
					if( ( pVertices[ wFaceIndex ].fTu == pGeomObj->pTuv[ wTexIndex ].fTu ) &&
						( pVertices[ wFaceIndex ].fTv == pGeomObj->pTuv[ wTexIndex ].fTv ) )
					{
						wUpdateIndex = wFaceIndex;
					}
					else
					{
						//## uv값이 다르면
						//## 버텍스 수를 하나 증가시켜주고
						nNumVertices++;

						pwIndexMapOVtx[nMapOVtxCount++] = wFaceIndex;

						//## 값을 세팅한다.
						pVertices[ nNumVertices-1 ].vP = pVertices[ wFaceIndex ].vP;
						pVertices[ nNumVertices-1 ].fTu = pGeomObj->pTuv[ wTexIndex ].fTu;
						pVertices[ nNumVertices-1 ].fTv = pGeomObj->pTuv[ wTexIndex ].fTv;

						wUpdateIndex = nNumVertices-1;
					}

				}
				else
					wUpdateIndex = wFaceIndex;

				*((&(pIndices[ i ]._0))+j) = wUpdateIndex;

			}
			else //## if( pbFlag[ wFaceIndex ] == true )
			{
				//## 이미 넣어진 값이 아니면 즉 pFlag[] 값이 false이면
				//## 그냥 대입한다.
				pVertices[ wFaceIndex ].vP = pGeomObj->pvVertices[ wFaceIndex ];

				if( bUseTex )
				{
					pVertices[ wFaceIndex ].fTu = pGeomObj->pTuv[ wTexIndex ].fTu;
					pVertices[ wFaceIndex ].fTv = pGeomObj->pTuv[ wTexIndex ].fTv;
				}

				pVertices[ wFaceIndex ].vN =
					pGeomObj->pFace[i].FaceNormal.vVertexNormal[j];

				pnNumVertex[wFaceIndex]++;

				//## 그리고 index를 추가한다.
				*((&(pIndices[ i ]._0))+j) = wFaceIndex;

				//## 그리고 flag를 true 로 바꾼다.
				pbFlag[ wFaceIndex ] = true;

			}//## if( pbFlag[ wFaceIndex ] == true )

		}//## for( int j = 0; j < 3; j++ )

	}//## for( int i = 0; i < pGeomObj->nNumFace; i++ )

	for( int m = 0; m < nMapOVtxCount; m++ )
	{
		pVertices[nOrgVertices].vN = pVertices[ pwIndexMapOVtx[m] ].vN;
		pnNumVertex[nOrgVertices] = pnNumVertex[ pwIndexMapOVtx[m] ];

		nOrgVertices++;
	}

	for( int loop=0; loop < nNumVertices; loop++ )
	{
		pVertices[loop].vN /= (float)pnNumVertex[loop];
	}

	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## 정점 버퍼, 인덱스 버퍼 작성
	//#################################################################################

	//## 메모리를 재할당하여 복사하고 이전 메모리는 제거한다.
	S_CUSTOMVERTEX *pNewVertices = new S_CUSTOMVERTEX[ nNumVertices ];
	memcpy( pNewVertices, pVertices, sizeof(S_CUSTOMVERTEX)*nNumVertices );

	SAFE_DELETE_ARRAY( pwIndexMapOVtx );
	SAFE_DELETE_ARRAY( pnNumVertex );
	SAFE_DELETE_ARRAY( pVertices );
	SAFE_DELETE_ARRAY( pbFlag );

	//## 이제 정점 버퍼 포인터를 넘긴다.
	pMeshObj->SetVertices( pNewVertices );

	pMeshObj->SetNumVertices( nNumVertices );
	pMeshObj->SetNumFaces( nNumFace );

	pMeshObj->SetIndices( pIndices );

	return pMeshObj;
}