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

	//## ������ ����.
	if( ( m_fpFile = fopen( m_pFilePathName, "r" ) ) == NULL )
	{
		//## �ٽ� ������ ����.
		//if( (m_fpFile = fopen(string,"r")) == NULL )
		::MessageBox( NULL, "������ �� �� �����ϴ�.", "Error", MB_OK );
		bReturn = false;
	}
		
	if( fgets( m_cLineBuffer, BUFFER_SIZE, m_fpFile ) != NULL )
	{
		sscanf( m_cLineBuffer, "%s", m_cCompBuffer );
	}

	//##################################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ����!!!!!!!!!
	//## ASE �������� üũ
	if( _stricmp( m_cCompBuffer ,T_ASE ) != 0 )
	{
		::MessageBox( NULL, "ASE ������ �ƴմϴ�.", "Error", MB_OK );
		bReturn = false;
	}

	_ClearBuffer();

	//## ������ ������ ����
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

		//## MATERIAL_COUNT ���ϱ�
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

		//## �� MATERIAL ���ϱ�
		if( _stricmp( m_cCompBuffer, T_MATERIAL ) == 0 )
		{
			//## material id ����
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

		//## SUB MATERIAL ��
		if( _stricmp( m_cCompBuffer, T_NUMSUBMTLS ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%d", m_cCompBuffer, &(pCurMtrl->nNumSubMtrl) );

			//## material data �޸� �Ҵ�
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

		//## *MAP_ { �̷� ���¸� ��󳽴�.
		//## �׷��� TEXTURE ������ ��´�.
		_Check_MAP(pCurMtrl);

	END_PARSING();

	return;
}

void CAseParser::_Check_MAP(S_MATERIAL *pCurMtrl)
{
		//## *MAP_ { �̷� ���¸� ��󳽴�.
		//## �׷��� TEXTURE ������ ��´�.
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

		//## ���� MATERIAL�� �ؽ��İ� �����Ǿ� ���� �ʰ� BITMAP ������ ��ġ�ϸ�
		if( pCurMtrl->pTexFile == NULL && _stricmp( m_cCompBuffer, T_BITMAP ) == 0 )
		{
			//## ���� ��ο� ���鵵 �� �� �ֱ⶧����....
			//## "�� ����
			char *pStr = NULL;
			char sep[] = "\"";

			pStr = strtok( m_cLineBuffer, sep );
			pStr = strtok( 0, sep);

			if( strlen(pStr) > 0 )
			{
				SAFE_DELETE_ARRAY( pCurMtrl->pTexFile );
				pCurMtrl->pTexFile = new char[ strlen(pStr) + 1 ];
				strcpy( pCurMtrl->pTexFile, pStr );

				//## �ؽ��İ� �����Ƿ� �ؽ��� ���� ������ ���� ���̴�.
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

	//## GEOMOBJECT ����ü �޸� �Ҵ�
	S_GEOMOBJECT *pGeomObj = new S_GEOMOBJECT;

	//## LIST�� ����
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

			//## tm �� ����� ���ϰ� local ��ǥ��� ��ȯ
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

		//## TM ��� ROW 0
		if( _stricmp( m_cCompBuffer, T_TM_ROW0 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._11),
												&(pGeomObj->matTm._13),
												&(pGeomObj->matTm._12));
		}

		//## TM ��� ROW 1
		if( _stricmp( m_cCompBuffer, T_TM_ROW1 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._31),
												&(pGeomObj->matTm._33),
												&(pGeomObj->matTm._32));
		}

		//## TM ��� ROW 2
		if( _stricmp( m_cCompBuffer, T_TM_ROW2 ) == 0 )
		{
			sscanf( m_cLineBuffer, "%s%f%f%f", m_cCompBuffer,
												&(pGeomObj->matTm._21),
												&(pGeomObj->matTm._23),
												&(pGeomObj->matTm._22));
		}

		//## TM ��� ROW 3
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

	//## ���� ���� �޸𸮸� �Ҵ��Ѵ�.
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

	//## �� ����ü �޸� �Ҵ�
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

			//## �ε��� ����
			pGeomObj->pFace[ nCount ].Index._0 = nI0;
			pGeomObj->pFace[ nCount ].Index._1 = nI1;
			pGeomObj->pFace[ nCount ].Index._2 = nI2;

			//## smooth ������ ���� �����ǹǷ� 
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

	//## uv ��ǥ�� ���� �޸� �Ҵ�
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
// 3d max�� world ��ǥ�� local ��ǥ�� ��ȯ�Ѵ�.
//####################################################
void CAseParser::_TransformToLocal( S_GEOMOBJECT *pGeomObj )
{
	D3DXVECTOR3 vWorld;
	D3DXVECTOR3 vLocal;
	D3DXMATRIXA16 matInvTm;		//## tm �� �����

	//## tm�� ������� ���Ѵ�.
	D3DXMatrixInverse( &matInvTm, 0, &(pGeomObj->matTm) );
	pGeomObj->matInvTm = matInvTm;

	//## ���� �� ��ŭ loop
	for(int i = 0; i < pGeomObj->nNumVertex; i++ )
	{
		//## ��ü�� world ���� ��ǥ���̴�.
		vWorld = pGeomObj->pvVertices[i];

		//## local ��ǥ�� ��ȯ�Ѵ�. 
		D3DXVec3TransformCoord( &vLocal, &vWorld, &matInvTm );

		//## ��´�.
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
	//## texture�� �����Ѵ�.
	if( FAILED( D3DXCreateTextureFromFileEx( pd3dDevice, pFilePathName,
											 D3DX_DEFAULT, D3DX_DEFAULT,
											 1, 0,
											 D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
											 D3DX_DEFAULT, D3DX_DEFAULT,
											 0, NULL, NULL, pTexture ) ) )
	{
		//## ��θ��� Ʋ���ٸ� ���ϸ� ������ ���� ��ο��� ã�´�.
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
// CModel Ŭ������ ��ȯ�ϴ� �Լ�
// TEXTURE ������ ����
// LPDIRECT3DDEVICE9 pd3dDevice�� �޴´�.
//####################################################
bool CAseParser::Convert( CModel *pModel, LPDIRECT3DDEVICE9 pd3dDevice )
{
	S_GEOMOBJECT *pGeomObj;

	S_MATERIAL_MESH *pMtrlMesh;

	//## MeshObjList ������
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
			//## ���� GEOMOBJECT ������
			pGeomObj = static_cast<S_GEOMOBJECT *>(*pi);

			//## ���� GEOMOBJECT�� MATERIAL ID ��
			if( pGeomObj->pvVertices != NULL && m_pMtrl[i].uiId == pGeomObj->nMtrlId )
			{
				//## submaterial�� �����ϸ�
				if( m_pMtrl[i].nNumSubMtrl > 0 )
				{
					//## submaterial ó��
					_ProcessSubMaterial( pModel, &(m_pMtrl[i]), pGeomObj, pd3dDevice );
				}
				else
				{

					//## nCount == 0 �̸� CMeshObj list�� ����
					if( nCount == 0 )
					{
						pMeshObjList = NULL;
						pMtrlMesh = new S_MATERIAL_MESH;
						pMeshObjList = pModel->CreateMeshObjList();
						pMtrlMesh->pObjList = pMeshObjList;

						//## material ������ �ѱ��.
						//## ���� ������ �ѱ��� ���� ���� �Ѱܾ� �Ѵ�.
						//## parser�� ����� ���ŵȴ�.
						pMtrlMesh->Mtrl = m_pMtrl[i].Mtrl;

						if( m_pMtrl[i].pTexFile != NULL )
						{
							pMtrlMesh->pTexFile = new char[ strlen( m_pMtrl[i].pTexFile ) + 1 ];
							strcpy( pMtrlMesh->pTexFile, m_pMtrl[i].pTexFile );

							//######################################################################
							//## texture ����
							if( !_BuildTexture( pd3dDevice, pMtrlMesh->pTexFile, &pMtrlMesh->pTexture ) )
								::MessageBox(NULL, "�ؽ��� ���� ����! �׳� �����մϴ�.","error",MB_OK);
						}
					}

					if( pMeshObjList != NULL )
						pMeshObjList->push_back( _CreateModel( pModel, pGeomObj ) );

					nCount++;
				}

			}//## if ���� GEOMOBJECT�� MATERIAL ID ��

		}//## for GEOMOBJECT LIST loop
		
		//## ���� material �� �Ѿ�� ���� MeshObjList ����
		if( pMtrlMesh != NULL )
            pModel->GetMtrlMeshListPtr()->push_back( pMtrlMesh );

	}//## for material loop
	

	return true;
}

//####################################################
// SUBMATERIAL ó��
//####################################################
void CAseParser::_ProcessSubMaterial(CModel *pModel,
									 S_MATERIAL *pMtrl,
									 S_GEOMOBJECT *pGeomObj,
									 LPDIRECT3DDEVICE9 pd3dDevice )
{
	//## submaterial�� ����� face ���� ������� �ӽ� ����
	BYTE *pbySubIdList = new BYTE[ pGeomObj->nNumFace ];
	int *pnNumSubMtrl = new int[ pMtrl->nNumSubMtrl ];
	ZeroMemory( pnNumSubMtrl, sizeof(int)*pMtrl->nNumSubMtrl);

	BYTE byMtrlId;
	//## ��� face�� LOOP
	for( int k = 0; k < pGeomObj->nNumFace; k++ )
	{
		byMtrlId = pGeomObj->pFace[k].uiSubMtrlId;

		//## face�� SUBMATERIAL ID�� FACE COUNT ������ ����
		pnNumSubMtrl[ byMtrlId ]++;

		pbySubIdList[k] = byMtrlId;
	}

	//## face ����ü ���� pointer�� ���� �޸� ����
	S_MESH_FACE **paNewFace =  new S_MESH_FACE*[ pMtrl->nNumSubMtrl ];

	//## �� submaterial�� ����� face �� ��ŭ �޸� �Ҵ�
	for( int loop = 0; loop < pMtrl->nNumSubMtrl; loop++ )
	{
		paNewFace[loop] = new S_MESH_FACE[ pnNumSubMtrl[ loop ] ];

	}

	//#######################################################################
	//## submaterial�� ���� ��� ���� �и��� �Ҵ�� S_MESH_FACE �޸𸮿� 
	//## face ����Ÿ�� ��´�.

	//## face index�� offset�� ���ϱ� ���� �ӽ� ī����
	int *pnCount = new int[ pMtrl->nNumSubMtrl ];
	for( int m = 0; m < pMtrl->nNumSubMtrl; m++ )
	{
		pnCount[m] = pnNumSubMtrl[ m ];
	}

	//## ���� face pointer
	S_MESH_FACE *pCurMeshFace;

	//#############################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## �� submaterial ���� �Ҵ��� �޸𸮿� face ������ ��´�.
	//## �� ť�� ����������.
	for( int i = 0; i < pGeomObj->nNumFace; i++ )
	{

		//## submaterial�� ����� face ���� 0���� ũ�� �������� ����
		if( pnCount[ pbySubIdList[i] ] > 0 )
		{
			//## ���� �޸� ���� ������ �� �� �ֵ��� ������ ���� offset
			int nOffset = pnNumSubMtrl[ pbySubIdList[i] ] - pnCount[ pbySubIdList[i] ] ;

			//## ���� face������ ���� S_MESH_FACE �����͸� ���Ѵ�.
			pCurMeshFace = paNewFace[pbySubIdList[i]] + nOffset;

			//################################################################################
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//## FACE ������ ��´�.
			pCurMeshFace->Index._0 = pGeomObj->pFace[i].Index._0;
			pCurMeshFace->Index._1 = pGeomObj->pFace[i].Index._1;
			pCurMeshFace->Index._2 = pGeomObj->pFace[i].Index._2;

			//## texture�� ������
			if( pGeomObj->pTuv != NULL )//pCurMeshFace->pMtrl->pSubMtrl[ pbySubIdList[i] ].pTexFile != NULL )
			{
				pCurMeshFace->TexIndex._0 = pGeomObj->pFace[i].TexIndex._0;
				pCurMeshFace->TexIndex._1 = pGeomObj->pFace[i].TexIndex._1;
				pCurMeshFace->TexIndex._2 = pGeomObj->pFace[i].TexIndex._2;
			}

			pCurMeshFace->uiSubMtrlId = pGeomObj->pFace[i].uiSubMtrlId;

			//## normal ���� ��´�.
			pCurMeshFace->FaceNormal.vFaceNormal 
					= pGeomObj->pFace[i].FaceNormal.vFaceNormal;
			pCurMeshFace->FaceNormal.vVertexNormal[0]
				= pGeomObj->pFace[i].FaceNormal.vVertexNormal[0];
			pCurMeshFace->FaceNormal.vVertexNormal[1]
				= pGeomObj->pFace[i].FaceNormal.vVertexNormal[1];
			pCurMeshFace->FaceNormal.vVertexNormal[2]
				= pGeomObj->pFace[i].FaceNormal.vVertexNormal[2];

			//## FACE ������ ��´�.
			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//################################################################################

			//## �ϳ��� ������ ������ ���ؼ� face ���� ���ҽ�Ų��.
			pnCount[ pbySubIdList[i] ]--;
		}
	}
	//## �� submaterial ���� �Ҵ��� �޸𸮿� face ������ ��´�.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//#############################################################################

	SAFE_DELETE_ARRAY( pGeomObj->pFace );
	SAFE_DELETE_ARRAY( pnCount );
	SAFE_DELETE_ARRAY( pbySubIdList );

	//## material ����ü�� ��ƺ���.
	S_MATERIAL_MESH *pMtrlMesh;

	//## MeshObjList ������
	MeshObjList *pMeshObjList = NULL;
	//###########################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ���� S_GEOMOBJECT ����ü�� �����Ͽ� _CreateModel �Լ��� ȣ���Ѵ�.
	for( int a = 0; a < pMtrl->nNumSubMtrl; a++ )
	{
		//## submaterial �� ����� face ���� �ϳ��� ������
		if( pnNumSubMtrl[ a ] > 0 )
		{
			//## S_MATERIAL_MESH �޸� �Ҵ�
			pMtrlMesh = new S_MATERIAL_MESH;

			//## Mesh Object List �޸� �Ҵ�
			pMeshObjList = pModel->CreateMeshObjList();

			//## MtrlMesh�� Object list�� ����
			pMtrlMesh->pObjList = pMeshObjList;

			//## material ������ �ѱ��.
			//## ���� ������ �ѱ��� ���� ���� �Ѱܾ� �Ѵ�. �ֳĸ� �� �޸𸮵��� ���ŵǱ⶧����
			//## parser�� ����� ���ŵȴ�.
			pMtrlMesh->Mtrl = pMtrl->pSubMtrl[a].Mtrl;//m_pMtrl[a].Mtrl;

			if( pMtrl->pSubMtrl[a].pTexFile != NULL )//m_bTexture != NULL &&
			{
				pMtrlMesh->pTexFile = new char[ strlen( pMtrl->pSubMtrl[a].pTexFile ) + 1 ];
				strcpy( pMtrlMesh->pTexFile, pMtrl->pSubMtrl[a].pTexFile );

				//######################################################################
				//## texture ����
				if( !_BuildTexture( pd3dDevice, pMtrlMesh->pTexFile, &pMtrlMesh->pTexture ) )
					::MessageBox(NULL, "�ؽ��� ���� ����! �׳� �����մϴ�.","error",MB_OK);
			}

			//## ���� S_GEOMOBJECT �� FACE �����Ϳ� ����
			pGeomObj->pFace = paNewFace[a];
			pGeomObj->nNumFace = pnNumSubMtrl[ a ];

			//## ���� ���� �ʹ� ���� ��찡 ������ face���� ������ ������ �߷�����
			//## face index�� ���� ���� ���� ������ �ٲܰ��̴�.
			S_GEOMOBJECT *pNewGeomObj = _OptimizeVertices( pGeomObj );

			//## CModel �� �����.
			if( pMeshObjList != NULL )
				pMeshObjList->push_back( _CreateModel( pModel, pNewGeomObj ) );

			SAFE_DELETE( pNewGeomObj );
			SAFE_DELETE_ARRAY( pGeomObj->pFace );

			//## ���� material �� �Ѿ�� ���� MeshObjList ����
			if( pMtrlMesh != NULL )
				pModel->GetMtrlMeshListPtr()->push_back( pMtrlMesh );
		}
	}

	SAFE_DELETE_ARRAY( paNewFace );
	SAFE_DELETE_ARRAY( pnNumSubMtrl );

}

//####################################################
// submaterial ó���� ���� �� ����ȭ
//####################################################
S_GEOMOBJECT *CAseParser::_OptimizeVertices( S_GEOMOBJECT *pGeomObj )
{

	//############################################################################
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## ���ο� S_GEOMOBJECT ����ü�� �ۼ��ϱ� ���� �޸𸮸� �Ҵ��ϰ�
	//## ���� S_GEOMOBJECT ����ü�κ��� �ʿ��� ����Ÿ�� �Ѱܹ޴´�.

	//## ���� �Ѱܹ��� MESH�� �� face ���� ���Ѵ�.
	int nNumFace = pGeomObj->nNumFace;


	//## ������ �̹� ��ϵǾ����� üũ�ϱ� ���� �ӽ� �޸�
	//## ���� �� ��ŭ �Ҵ��Ѵ�.
	int *pnVIndex = new int[ pGeomObj->nNumVertex ];

	//## �׸��� �ʱ�ȭ�Ѵ�. -1��
	for(int k = 0; k < pGeomObj->nNumVertex; k++ )
	{
		pnVIndex[k] = -1;
	}

	//## ���⼭ ���ο� S_GEOMOBJECT ����ü �޸𸮸� �Ҵ��Ѵ�.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	S_GEOMOBJECT *pNewGeomObj = new S_GEOMOBJECT;

	//## ���⼭ ���� ������ ���� ����ü �޸𸮸� �Ҵ��Ѵ�.
	//## ���� ���� �ִ� FACE * 3 ���� ���� �ִ�.( �ؽ��� ��ǥ�� ��� ������ �� �ٸ� ��� )
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	D3DXVECTOR3 *pVertices = new D3DXVECTOR3[ pGeomObj->nNumVertex ];

	//## FACE ������ ���� �޸𸮸� �Ҵ��Ѵ�.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	S_MESH_FACE *pFace = new S_MESH_FACE[ nNumFace ];

	S_MESH_TUV *pTuv = NULL;
	//## �ؽ��İ� ����Ǿ��ٸ� uv ��ǥ�� ���� �޸𸮵� �Ҵ��Ѵ�.
	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int *pnUvIndex = NULL;
	if( pGeomObj->pTuv != NULL )
	{
        pTuv = new S_MESH_TUV[ pGeomObj->nNumTVertex  ];

		//## uv ������ �̹� ��ϵǾ����� üũ�ϱ� ���� �ӽ� �޸�
		//## uv ���� �� ��ŭ �Ҵ��Ѵ�.
		pnUvIndex = new int[ pGeomObj->nNumTVertex ];

		//## �׸��� �ʱ�ȭ�Ѵ�. -1��
		for(int k = 0; k < pGeomObj->nNumTVertex; k++ )
		{
			pnUvIndex[k] = -1;
		}
	}


	//## ���� �޽� ������ �� �޽� ����ü �޸𸮷� ��´�.
	pNewGeomObj->matInvTm = pGeomObj->matInvTm;
	pNewGeomObj->matTm = pGeomObj->matTm;
	pNewGeomObj->nMtrlId = pGeomObj->nMtrlId;

	pNewGeomObj->nNumFace = nNumFace;
	pNewGeomObj->nNumTFace = pGeomObj->nNumTFace;

	//## nodename
	if( pGeomObj->pNodeName != NULL )
	{
		//## ������ �����ϱ����� �޸� �Ҵ�
        pNewGeomObj->pNodeName = new char[ strlen(pGeomObj->pNodeName)+1 ];
		strcpy( pNewGeomObj->pNodeName, pGeomObj->pNodeName );
	}
	//## parentnode name
	if( pGeomObj->pParentNodeName != NULL )
	{
		//## ������ �����ϱ����� �޸� �Ҵ�
		pNewGeomObj->pParentNodeName = new char[ strlen(pGeomObj->pParentNodeName)+1 ];
		strcpy( pNewGeomObj->pParentNodeName, pGeomObj->pParentNodeName );
	}

	//## �� �޽� ������ tvertex �޸𸮿�, vertex �޸𸮸� �����Ų��.
	pNewGeomObj->pvVertices = pVertices;
	pNewGeomObj->pFace = pFace;
	pNewGeomObj->pTuv = pTuv;

	//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//############################################################################

	WORD wFaceIndex, wTexIndex;

	//## ���� �� ī����
	int nNumVertices = 0;
	//## UV ���� �� ī����
	int nNumUvVertices = 0;

	//####################################################################################
	//## �۾��� �����Ѵ�.
	//## face �� ��ŭ loop
	for( int i = 0; i < nNumFace; i++ )
	{
		//## �ϴ� face normal ����
		pFace[ i ].FaceNormal.vFaceNormal = pGeomObj->pFace[i].FaceNormal.vFaceNormal;

		//## ������ 3���ϱ� 3�� loop
		for( int j = 0; j < 3; j++ )
		{

			//## ���� index
			wFaceIndex = *((&(pGeomObj->pFace[i].Index._0))+j);

			//## texture�� ����Ǵ� ���
			//if( pNewGeomObj->nNumTVertex > 0 )
				//## texture ���� index�� �޴´�.
			if( pGeomObj->pTuv != NULL )
				wTexIndex = *((&(pGeomObj->pFace[i].TexIndex._0))+j);
		
			//## �� ���� normal�� �޴´�.
			pFace[ i ].FaceNormal.vVertexNormal[j]
						= pGeomObj->pFace[i].FaceNormal.vVertexNormal[j];

			//## �̹� ��ϵ� �������� flag ���� ���Ͽ� üũ�Ѵ�.
			if( pnVIndex[ wFaceIndex ] > -1 )
			{
				//## �̹̿� ��ϵǾ��ٸ� index ���ۿ� 
				//## pnVIndex�迭�� �����ص� index�� ��´�.
				*((&(pFace[ i ].Index._0))+j) = pnVIndex[ wFaceIndex ];
			}
			else
			{
				//## ��ϵ� ������ �ƴ϶�� ���������� ����Ѵ�.
				pVertices[nNumVertices] = pGeomObj->pvVertices[ wFaceIndex ];

				//## index�� ����Ѵ�.
				*((&(pFace[ i ].Index._0))+j) = nNumVertices;

				//## index ���� �����Ͽ� �θ� -1 �� �ƴϹǷ�
				//## ��ϵ� �������� �� �� �ִ�.
				pnVIndex[ wFaceIndex ] = nNumVertices;

				//## ���� ���� ����.
				nNumVertices++;
			}

			//## uv ��ǥ �޸𸮰� ������� ������ 
			if( pGeomObj->pTuv != NULL )
			{
				//## �̹̿� ��ϵ� uv�������� flag ���� ���Ͽ� üũ�Ѵ�.
				if( pnUvIndex[ wTexIndex ] > -1 )
				{
					//## �̹̿� ��ϵǾ��ٸ� index ���ۿ� 
					//## pnVIndex�迭�� �����ص� index�� ��´�.
					*((&(pFace[ i ].TexIndex._0))+j) = pnUvIndex[ wTexIndex ];
				}
				else
				{
					//## ��ϵ� uv ������ �ƴϸ� ����Ѵ�.
					pTuv[nNumUvVertices] = pGeomObj->pTuv[wTexIndex];

					//## uv index�� ����
					*((&(pFace[ i ].TexIndex._0))+j) = nNumUvVertices;

					//## index ���� �����Ͽ� �θ� -1 �� �ƴϹǷ�
					//## ��ϵ� �������� �� �� �ִ�.
					pnUvIndex[ wTexIndex ] = nNumUvVertices;

					//## UV ���� ���� ����.
					nNumUvVertices++;
				}
			}
		}
	}	

	//## ���� �߷��� ���� ���� ���´�.
	pNewGeomObj->nNumVertex = nNumVertices;

	if( pGeomObj->pTuv != NULL )
		pNewGeomObj->nNumTVertex = nNumUvVertices;

	//## �޸𸮸� ���Ҵ��Ͽ� �����ϰ� ���� �޸𸮴� �����Ѵ�.
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

	//## �ӽ� üũ�� �޸𸮸� �����Ѵ�.
	SAFE_DELETE_ARRAY( pnVIndex );
	SAFE_DELETE_ARRAY( pnUvIndex );

	//## �� �޽� �����͸� ��ȯ�Ѵ�.
	return pNewGeomObj;
}


//####################################################
// CModel ����
//####################################################
CMeshObj *CAseParser::_CreateModel( CModel *pModel, S_GEOMOBJECT *pGeomObj )
{

	CMeshObj *pMeshObj = pModel->CreateMeshObj();

	pMeshObj->SetMatrixTM( pGeomObj->matTm );
	pMeshObj->SetMatrixInverseTM( pGeomObj->matInvTm );

	//## bbox
	pMeshObj->SetBbox( &m_vMax, &m_vMin );

	bool bUseTex = ( pGeomObj->pTuv == NULL ) ? false : true;

	//## �ϴ� �⺻ ���ؽ� ������ �����Ѵ�.
	int nNumVertices = pGeomObj->nNumVertex;
	int nNumFace = pGeomObj->nNumFace;

	//## vertex �ִ�ġ
	int nMaxVertices = nNumFace * 3;

	//## ���࿡ vertex �ִ�ġ�� ���� ������ pGeomObj->nNumVertex ���� ũ��
	if( pGeomObj->nNumVertex > nMaxVertices )
		nMaxVertices = pGeomObj->nNumVertex;

	//## �ߺ��� ���� ���� ���� �޸𸮸� �غ�. �ӽÿ��̴�. �ݵ�� �����Ѵ�.
	int *pnNumVertex = new int[ nMaxVertices ];
	ZeroMemory( pnNumVertex, sizeof(int)*nMaxVertices );	//## �ʱ�ȭ ��� 0��

	//## flag �޸𸮸� �����Ѵ�. �ӽÿ��̴�. �ݵ�� �����Ѵ�.
	bool *pbFlag = new bool[ nMaxVertices ];

	//## �ʱ�ȭ ��� false��
	for( int k = 0; k < nMaxVertices; k++ )
		pbFlag[k] = false;

	//## vertex �Ҵ��� �ִ�ġ�� �Ѵ�. �ӽÿ��̴�. �ݵ�� �����Ѵ�.
	S_CUSTOMVERTEX *pVertices;

	if( bUseTex )
		pVertices = new S_CUSTOMVERTEX[ nMaxVertices*2 ];
	else
		pVertices = new S_CUSTOMVERTEX[ nMaxVertices ];

	//## index
	S_CUSTOMINDEX *pIndices = new S_CUSTOMINDEX[ nNumFace ];

	//## �⺻ ������ ���� ��ġ�� ������ ���� ��������� ���� index
	WORD *pwIndexMapOVtx = new WORD[nMaxVertices];

	int nMapOVtxCount = 0;
	int nOrgVertices = nNumVertices;	//## �⺻ ������ ���

	//#################################################################################
	//## ���� ����, �ε��� ���� �ۼ�
	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//## face �� ��ŭ loop
	WORD wFaceIndex, wTexIndex, wUpdateIndex;

	for( int i = 0; i < pGeomObj->nNumFace; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			wFaceIndex = *((&(pGeomObj->pFace[i].Index._0))+j);

			if( bUseTex )
				wTexIndex = *((&(pGeomObj->pFace[i].TexIndex._0))+j);

			//## �̹� �־��� ���̸�
			if( pbFlag[ wFaceIndex ] == true )
			{
				pnNumVertex[wFaceIndex]++;
				pVertices[ wFaceIndex ].vN +=
					pGeomObj->pFace[i].FaceNormal.vVertexNormal[j];

				if( bUseTex )
				{
					//## uv ���� ������
					if( ( pVertices[ wFaceIndex ].fTu == pGeomObj->pTuv[ wTexIndex ].fTu ) &&
						( pVertices[ wFaceIndex ].fTv == pGeomObj->pTuv[ wTexIndex ].fTv ) )
					{
						wUpdateIndex = wFaceIndex;
					}
					else
					{
						//## uv���� �ٸ���
						//## ���ؽ� ���� �ϳ� ���������ְ�
						nNumVertices++;

						pwIndexMapOVtx[nMapOVtxCount++] = wFaceIndex;

						//## ���� �����Ѵ�.
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
				//## �̹� �־��� ���� �ƴϸ� �� pFlag[] ���� false�̸�
				//## �׳� �����Ѵ�.
				pVertices[ wFaceIndex ].vP = pGeomObj->pvVertices[ wFaceIndex ];

				if( bUseTex )
				{
					pVertices[ wFaceIndex ].fTu = pGeomObj->pTuv[ wTexIndex ].fTu;
					pVertices[ wFaceIndex ].fTv = pGeomObj->pTuv[ wTexIndex ].fTv;
				}

				pVertices[ wFaceIndex ].vN =
					pGeomObj->pFace[i].FaceNormal.vVertexNormal[j];

				pnNumVertex[wFaceIndex]++;

				//## �׸��� index�� �߰��Ѵ�.
				*((&(pIndices[ i ]._0))+j) = wFaceIndex;

				//## �׸��� flag�� true �� �ٲ۴�.
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
	//## ���� ����, �ε��� ���� �ۼ�
	//#################################################################################

	//## �޸𸮸� ���Ҵ��Ͽ� �����ϰ� ���� �޸𸮴� �����Ѵ�.
	S_CUSTOMVERTEX *pNewVertices = new S_CUSTOMVERTEX[ nNumVertices ];
	memcpy( pNewVertices, pVertices, sizeof(S_CUSTOMVERTEX)*nNumVertices );

	SAFE_DELETE_ARRAY( pwIndexMapOVtx );
	SAFE_DELETE_ARRAY( pnNumVertex );
	SAFE_DELETE_ARRAY( pVertices );
	SAFE_DELETE_ARRAY( pbFlag );

	//## ���� ���� ���� �����͸� �ѱ��.
	pMeshObj->SetVertices( pNewVertices );

	pMeshObj->SetNumVertices( nNumVertices );
	pMeshObj->SetNumFaces( nNumFace );

	pMeshObj->SetIndices( pIndices );

	return pMeshObj;
}