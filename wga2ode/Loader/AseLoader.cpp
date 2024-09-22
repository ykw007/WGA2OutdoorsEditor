#include "StdAfx.h"
#include "aseloader.h"

bool CAseLoader::Clear()
{
	m_pMtrl = 0;
	m_pMesh = 0;
	m_nNumMtrl=0;
	m_pMtrl=0;
	m_fpFile=0;
	m_pMdEntry=0;

	return true;
}

void CAseLoader::Destroy()
{
	if(m_nNumMtrl>1)
		SAFE_DELETE_ARRAY(m_pMtrl)
	else if(m_nNumMtrl==1)
		SAFE_DELETE(m_pMtrl)

	if(!m_pMeshList.empty())
		stl_wipe(m_pMeshList);

	return;
}


inline void CAseLoader::ClearBuffer()
{
	memset(m_sBuffer,0,BUFFERSIZE);
	memset(m_sHeader,0,BUFFERSIZE);
}

//##############################################################################
// ASE ���� �� ���ξ� �����鼭 �Ľ�
//##############################################################################
MODEL* CAseLoader::ReadAse(const char *pFile)
{
	//## �ι� �̻� ase ������ �д� ��쿡 ���� ����Ÿ�� �ʱ�ȭ�Ѵ�.
	Clear();

	char string[255];

	//## ������ ����.
	if( (m_fpFile = fopen(pFile,"r")) == NULL )
	{
		//## ���� ��ġ�� ������ �������� ã�´�.
		//strcpy(string,"..\\");
		//strcat(string,pFile);

		//## �ٽ� ������ ����.
		//if( (m_fpFile = fopen(string,"r")) == NULL )
			return 0;
	}
		
	if( fgets(m_sBuffer,BUFFERSIZE,m_fpFile)!=NULL && m_sBuffer[0] != '\0' )
		sscanf(m_sBuffer,"%s",m_sHeader);
	else
		return 0;

	//## ASE �������� üũ
	if( _stricmp(m_sHeader,T_ASE) != 0 )
		return 0;

	m_pMtrl = new MATERIAL;
	m_nNumMtrl = 1;

	ClearBuffer();

	//## ������ ������ ����
	while( fgets(m_sBuffer,BUFFERSIZE,m_fpFile) != NULL && m_sBuffer[0] != '\0' )
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		//## MATERIAL_LIST parsing
		if( _stricmp(m_sHeader,T_MATERIAL_LIST) == 0 )
			Parse_MATERIAL_LIST();

		//## GEOMOBJECT parsing
		if( _stricmp(m_sHeader,T_GEOMOBJECT) == 0 )
			Parse_GEOMOBJECT();

		ClearBuffer();
	}

	if( fclose(m_fpFile) != 0 )
		return 0;


	// ������ ����Ʈ ����Ÿ ����
	return MakeModelList();
}

/******************************************************************************
*
* void CAseLoader::Parse_MATERIAL_LIST();
*
* MATERIAL_LIST �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_MATERIAL_LIST()
{
	int		c=0;
	int		nTemp=0;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL &&
		m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;
		}

		// MATERIAL_COUNT ���ϱ�
		if( _stricmp(m_sHeader,T_MATERIAL_COUNT) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&nTemp);

			if( nTemp > 0 )
			{
				// ó�� ����� �ξ��� MATERIAL �޸� ����
				SAFE_DELETE(m_pMtrl)
				m_nNumMtrl = nTemp;
				m_pMtrl = new MATERIAL[nTemp];
			}
			else
				return;
		}

		// �� MATERIAL ���ϱ�
		if( _stricmp(m_sHeader,T_MATERIAL) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&nTemp);
			m_pMtrl[c].nID = nTemp;
			Parse_MATERIAL(&m_pMtrl[c]);
			c++;
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MATERIAL();
*
* MATERIAL �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_MATERIAL(MATERIAL *pCurMtrl)
{
	int		c=0;
	int		nTemp=0;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
			return;

		// MATERIAL_AMBIENT
		if( _stricmp(m_sHeader,T_MATERIAL_AMBIENT) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(pCurMtrl->Mtrl.Ambient.r),
				&(pCurMtrl->Mtrl.Ambient.g),
				&(pCurMtrl->Mtrl.Ambient.b));
		}

		// MATERIAL_DIFFUSE
		if( _stricmp(m_sHeader,T_MATERIAL_DIFFUSE) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(pCurMtrl->Mtrl.Diffuse.r),
				&(pCurMtrl->Mtrl.Diffuse.g),
				&(pCurMtrl->Mtrl.Diffuse.b));
		}

		// MATERIAL_SPECULAR
		if( _stricmp(m_sHeader,T_MATERIAL_SPECULAR) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(pCurMtrl->Mtrl.Specular.r),
				&(pCurMtrl->Mtrl.Specular.g),
				&(pCurMtrl->Mtrl.Specular.b));
		}

		// MAP DIFFUSE
		if( _stricmp(m_sHeader,T_MAP_DIFFUSE) == 0 )
		{
			Parse_MAP_DIFFUSE(pCurMtrl);
		}

		// SUB MATERIAL NUM
		if( _stricmp(m_sHeader,T_NUMSUBMTLS) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&(pCurMtrl->uiNumSubMtrl));
			// material data �޸� �Ҵ�
			if (pCurMtrl->uiNumSubMtrl > 1)
                pCurMtrl->pSubMtrl = new MATERIAL[pCurMtrl->uiNumSubMtrl];
			else if(pCurMtrl->uiNumSubMtrl == 1)
				pCurMtrl->pSubMtrl = new MATERIAL;
		}

		// SUB MATERIAL
		if( _stricmp(m_sHeader,T_SUBMATERIAL) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&nTemp);
			pCurMtrl->pSubMtrl[c].nID = nTemp;
			Parse_MATERIAL(&(pCurMtrl->pSubMtrl[c]));
			c++;
		}

		// �ʿ���� �±�
		if( _stricmp(m_sHeader,T_MAP_AMBIENT) == 0 ||
			_stricmp(m_sHeader,T_MAP_SHINESTRENGTH) == 0 ||
			_stricmp(m_sHeader,T_MAP_BUMP) == 0 ||
			_stricmp(m_sHeader,T_MAP_SPECULAR) == 0 || 
			_stricmp(m_sHeader,T_MAP_REFLECT) == 0 || 
			_stricmp(m_sHeader,T_MAP_REFRACT) == 0 )
		{
			Parse_ONLY_PASS();
		}

		ClearBuffer();
	}

	return;
}

void CAseLoader::Parse_ONLY_PASS()
{
	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
			return;

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MAP_DIFFUSE(MATERIAL *);
*
* MAP_DIFFUSE tag �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_MAP_DIFFUSE(MATERIAL *pCurMtrl)
{
	char*	pToken=0;
	char*	pTemp=0;
	char	sep[] = "\\\"\n";

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
			return;

		// BITMAP
		if( _stricmp(m_sHeader,T_BITMAP) == 0 )
		{
			pToken = strtok( m_sBuffer, sep);
			while( pToken != 0 )
			{
				pTemp = pToken;
				pToken = strtok( 0, sep);
			}

			pCurMtrl->sTexFile = new char[strlen(pTemp)+1];
			strcpy(pCurMtrl->sTexFile,pTemp);
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_GEOMOBJECT();
*
* MATERIAL_GEOMOBJECT �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_GEOMOBJECT()
{
	// �޽� ���� �޸� �Ҵ�
	if(m_pMesh!=0) m_pMesh=0;
	m_pMesh = new MESH;

	char	*pTemp=0;
	char	*pNodeName=0;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			m_pMeshList.push_back(m_pMesh);
			return;		// �Լ� ����
		}

		// NODE_NAME
		if( _stricmp(m_sHeader,T_NODE_NAME) == 0 )
		{
			if( m_pMesh->pNodeName == 0 )
			{
				sscanf(m_sBuffer,"%s%s",m_sHeader,m_sHeader);
				pTemp = strtok( m_sHeader, "\"");

				pNodeName = new char[strlen(pTemp)+1];
				strcpy(pNodeName,pTemp);

				m_pMesh->pNodeName = pNodeName;
			}
		}

		// NODE_PARENT
		if( _stricmp(m_sHeader,T_NODE_PARENT) == 0 )
		{
			sscanf(m_sBuffer,"%s%s",m_sHeader,m_sHeader);
			pTemp = strtok( m_sHeader, "\"");

			pNodeName = new char[strlen(pTemp)+1];
			strcpy(pNodeName,pTemp);

			m_pMesh->pParentName = pNodeName;
		}

		// NODE_TM
		if( _stricmp(m_sHeader,T_NODE_TM) == 0 )
			Parse_NODE_TM();

		// MESH
		if( _stricmp(m_sHeader,T_MESH) == 0 )
			Parse_MESH();

		// MATERIAL_REF
		if( _stricmp(m_sHeader,T_MATERIAL_REF) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&(m_pMesh->nMtrlRef));
		}

		// TM_ANIMATION
		if( _stricmp(m_sHeader,T_TM_ANIMATION) == 0 )
		{
			Parse_TM_ANIMATION();
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_TM_ANIMATION();
*
* TM_ANIMATION �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_TM_ANIMATION()
{
	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;		// �Լ� ����
		}

		if( _stricmp(m_sHeader,T_CONTROL_ROT_TCB) == 0 )
		{
			Parse_CONTROL_ROT_TCB();
		}

		if( _stricmp(m_sHeader,T_CONTROL_POS_TRACK) == 0 )
		{
			Parse_CONTROL_POS_TRACK();
		}

		if( _stricmp(m_sHeader,T_CONTROL_ROT_TRACK) == 0 )
		{
			Parse_CONTROL_ROT_TRACK();
		}

		ClearBuffer();
	}

	return;
}

void CAseLoader::Parse_CONTROL_ROT_TCB()
{
	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;		// �Լ� ����
		}

		ClearBuffer();
	}

	return;
}

void CAseLoader::Parse_CONTROL_POS_TRACK()
{
	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;		// �Լ� ����
		}

		ClearBuffer();
	}

	return;
}

void CAseLoader::Parse_CONTROL_ROT_TRACK()
{
	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;		// �Լ� ����
		}

		ClearBuffer();
	}

	return;
}
/******************************************************************************
*
* void CAseLoader::Parse_NODE_TM();
*
* NODE_TM �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_NODE_TM()
{
	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;		// �Լ� ����
		}

		// TM ���
		if( _stricmp(m_sHeader,T_TM_ROW0) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(m_pMesh->matTm._11),
				&(m_pMesh->matTm._13),
				&(m_pMesh->matTm._12));
		}

		if( _stricmp(m_sHeader,T_TM_ROW1) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(m_pMesh->matTm._31),
				&(m_pMesh->matTm._33),
				&(m_pMesh->matTm._32));
		}

		if( _stricmp(m_sHeader,T_TM_ROW2) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(m_pMesh->matTm._21),
				&(m_pMesh->matTm._23),
				&(m_pMesh->matTm._22));
		}

		if( _stricmp(m_sHeader,T_TM_ROW3) == 0 )
		{
			sscanf(m_sBuffer,"%s%f%f%f",m_sHeader,
				&(m_pMesh->matTm._41),
				&(m_pMesh->matTm._43),
				&(m_pMesh->matTm._42));
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MESH();
*
* MESH �Ľ�
*
******************************************************************************/
void CAseLoader::Parse_MESH()
{
	int i=0;
	int nCount=0;

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			// tm �� ����� ���ϰ� vertex local�� ��ȯ
			TransformToLocal();
			return;
		}

		// MESH_NUMVERTEX
		if( _stricmp(m_sHeader,T_MESH_NUMVERTEX) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,
				&(m_pMesh->nNumVertex));
		}

		// MESH_VERTEX_LIST
		if( _stricmp(m_sHeader,T_MESH_VERTEX_LIST) == 0 )
		{
			m_pMesh->pVertex = new S_CUSTOMVERTEX[m_pMesh->nNumVertex];
			Parse_MESH_VERTEX_LIST();
		}

		// MESH_NUMFACE
		if( _stricmp(m_sHeader,T_MESH_NUMFACE) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&(m_pMesh->nNumFace));
		}

		// MESH_FACE_LIST
		if( _stricmp(m_sHeader,T_MESH_FACE_LIST) == 0 )
		{
			// �� ����ü �޸� �Ҵ�
			m_pMesh->pFace = new FACE[m_pMesh->nNumFace];
			Parse_MESH_FACE_LIST();
		}

		// MESH_NUMTVERTEX
		if( _stricmp(m_sHeader,T_MESH_NUMTVERTEX) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&(m_pMesh->nNumTVertex));
			if( m_pMesh->nNumTVertex > 0 )
				m_pMesh->pUV = new TUV[m_pMesh->nNumTVertex];
		}

		// MESH_NORMALS
		if( _stricmp(m_sHeader,T_MESH_NORMALS) == 0 )
		{
			Parse_MESH_NORMALS();
		}

		// MESH_TVERTLIST
		if( _stricmp(m_sHeader,T_MESH_TVERTLIST) == 0 )
		{
			Parse_MESH_TVERTLIST();
		}

		// MESH_NUMTVFACES
		if( _stricmp(m_sHeader,T_MESH_NUMTVFACES) == 0 )
		{
			sscanf(m_sBuffer,"%s%d",m_sHeader,&(m_pMesh->nNumTVFace));
		}

		// MESH_TFACELIST
		if( _stricmp(m_sHeader,T_MESH_TFACELIST) == 0 )
		{
			Parse_MESH_TFACELIST();
		}

	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MESH_VERTEX_LIST();
*
* vertex list �ۼ�
*
******************************************************************************/
void CAseLoader::Parse_MESH_VERTEX_LIST()
{
	int nCount=0;
	float fX,fY,fZ;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;
		}

		// MESH_VERTEX
		if( _stricmp(m_sHeader,T_MESH_VERTEX) == 0 )
		{
			sscanf(m_sBuffer,"%s%d%f%f%f",m_sHeader,&nCount,&fX,&fZ,&fY);

			m_pMesh->pVertex[nCount].p.x = fX;
			m_pMesh->pVertex[nCount].p.y = fY;
			m_pMesh->pVertex[nCount].p.z = fZ;
		}
	}
}

/******************************************************************************
*
* void CAseLoader::Parse_MESH_FACE_LIST();
*
* face list �ۼ�
*
******************************************************************************/
void CAseLoader::Parse_MESH_FACE_LIST()
{
	char	sSmooth[20];
	char	sMtrlID[20];
	int		nID,nI0,nI1,nI2,nTemp;
	int		c=0;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;
		}

		// MESH_FACE
		if( _stricmp(m_sHeader,T_MESH_FACE) == 0 )
		{
			sscanf(m_sBuffer, "%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d",
				m_sHeader, m_sHeader,
				m_sHeader, &nI0,
                m_sHeader, &nI2,
                m_sHeader, &nI1,
                m_sHeader, &nTemp, m_sHeader, &nTemp, m_sHeader, &nTemp, m_sHeader, sSmooth,
				sMtrlID, &nID);

			m_pMesh->pFace[c].Index._0 = nI0;
			m_pMesh->pFace[c].Index._1 = nI1;
			m_pMesh->pFace[c].Index._2 = nI2;

			// smooth ������ ���� �����ǹǷ� 
			if(_strcmpi(sMtrlID,T_MESH_MTLID)==0)
				m_pMesh->pFace[c].nSubMtrlID = nID;
			
			if(_strcmpi(sSmooth,T_MESH_MTLID)==0)
				m_pMesh->pFace[c].nSubMtrlID = atoi(sMtrlID);

			// ���� �ε��� ���� ���������� �Է�
			c++;
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MESH_TVERTLIST();
*
* texture vertex list �ۼ�
*
******************************************************************************/
void CAseLoader::Parse_MESH_TVERTLIST()
{
	int nCount=0;
	float fU,fV,fW;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;
		}

		// MESH_TVERTLIST
		if( _stricmp(m_sHeader,T_MESH_TVERT) == 0 )
		{
			sscanf(m_sBuffer,"%s%d%f%f%f",m_sHeader,&nCount,&fU,&fV,&fW);
			m_pMesh->pUV[nCount].fU = fU;
			m_pMesh->pUV[nCount].fV = 1.0f - fV;
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MESH_TFACELIST();
*
* texture face list �ۼ�
*
******************************************************************************/
void CAseLoader::Parse_MESH_TFACELIST()
{
	int nCount=0,nI0,nI1,nI2;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;
		}

		// MESH_TFACELIST
		if( _stricmp(m_sHeader,T_MESH_TFACE) == 0 )
		{
			sscanf(m_sBuffer,"%s%d%d%d%d",m_sHeader,&nCount,&nI0,&nI2,&nI1);
			m_pMesh->pFace[nCount].TIndex._0 = nI0;
			m_pMesh->pFace[nCount].TIndex._1 = nI1;
			m_pMesh->pFace[nCount].TIndex._2 = nI2;
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::Parse_MESH_NORMALS();
*
* normal list �ۼ�
*
******************************************************************************/
void CAseLoader::Parse_MESH_NORMALS()
{
	int		nCount=0;
	float	fX,fY,fZ;

	ClearBuffer();

	while( fgets( m_sBuffer, BUFFERSIZE, m_fpFile )!=NULL && m_sBuffer[0]!='\0')
	{
		sscanf(m_sBuffer,"%s",m_sHeader);

		if( _stricmp(m_sHeader,"}") == 0 )
		{
			return;
		}

		// MESH_FACENORMAL
		if( _stricmp(m_sHeader,T_MESH_FACENORMAL) == 0 )
		{
			sscanf(m_sBuffer,"%s%d%f%f%f",m_sHeader,&nCount,&fX,&fZ,&fY);
			m_pMesh->pFace[nCount].vN.x = fX;
			m_pMesh->pFace[nCount].vN.y = fY;
			m_pMesh->pFace[nCount].vN.z = fZ;
		}

		// MESH_VERTEXNORMAL
		if( _stricmp(m_sHeader,T_MESH_VERTEXNORMAL) == 0 )
		{
			sscanf(m_sBuffer,"%s%d%f%f%f",m_sHeader,&nCount,&fX,&fZ,&fY);
			m_pMesh->pVertex[nCount].n.x = fX;
			m_pMesh->pVertex[nCount].n.y = fY;
			m_pMesh->pVertex[nCount].n.z = fZ;
		}

		ClearBuffer();
	}

	return;
}

/******************************************************************************
*
* void CAseLoader::TransformToLocal();
*
* TM�� ������� ���ϰ� ������ ���� ��ǥ�� ���÷� ��ȯ
*
******************************************************************************/
void CAseLoader::TransformToLocal()
{
	D3DXVECTOR3 vOrgTemp;
	D3DXVECTOR3 vTemp;
	D3DXMATRIXA16 matTemp;
	int i;

	D3DXMatrixInverse(&matTemp,0, &(m_pMesh->matTm));
	D3DXMatrixIdentity(&(m_pMesh->matInvTm));
	m_pMesh->matInvTm = matTemp;

	for( i=0; i < m_pMesh->nNumVertex; i++ )
	{
		vOrgTemp = m_pMesh->pVertex[i].p;
		D3DXVec3TransformCoord(&vTemp,&vOrgTemp,&matTemp);
		m_pMesh->pVertex[i].p=vTemp;
		if(m_pMesh->pVertex[i].n.x==0.0f
			&& m_pMesh->pVertex[i].n.y==0.0f
			&& m_pMesh->pVertex[i].n.z==0.0f)
			m_pMesh->pVertex[i].n = vTemp;
	}
}


void CAseLoader::ProcessSubMaterial(MATERIAL* pMtrl,MESH* pMesh)
{
	MODEL*		pTemp=0;
	int i,j,nBackFace=0;
	int c=0,nTemp;
	int*		pnCount=0;
	FACE*		pNewFace=0;
	FACE*		pBackFace=0;

	// �� ���� ���͸����� �� �ε��� �� ���� ���� �ӽ� ������ ����
	pnCount = new int[pMtrl->uiNumSubMtrl];
	
	for(i=0;i<pMtrl->uiNumSubMtrl;i++)
	{
		pnCount[i]=0;
	}

	for(i=0;i<pMesh->nNumFace;i++)
	{
		nTemp = pMesh->pFace[i].nSubMtrlID;
		pnCount[ nTemp ]++;
	}

	// �������� ���� memory pointer ���
	pBackFace = pMesh->pFace;
	nBackFace = pMesh->nNumFace;

	for(i=0;i<pMtrl->uiNumSubMtrl;i++)
	{
		if( pnCount[i]>0 )
		{
			if(i>0)
			{
				pMesh->pFace = pBackFace;
				pMesh->nNumFace = nBackFace;
			}

			pNewFace = new FACE[ pnCount[i] ];

			for(j=0;j<pMesh->nNumFace;j++)
			{
				if( pMesh->pFace[j].nSubMtrlID == pMtrl->pSubMtrl[i].nID)
				{
					pNewFace[c] = pMesh->pFace[j];
					pNewFace[c].Index._0 = pMesh->pFace[j].Index._0;
					pNewFace[c].Index._1 = pMesh->pFace[j].Index._2;
					pNewFace[c].Index._2 = pMesh->pFace[j].Index._1;

					pNewFace[c].TIndex._0 = pMesh->pFace[j].TIndex._0;
					pNewFace[c].TIndex._1 = pMesh->pFace[j].TIndex._1;
					pNewFace[c].TIndex._2 = pMesh->pFace[j].TIndex._2;

					c++;
				}
			}
			pMesh->pFace = pNewFace;
			pMesh->nNumFace = c;
			c=0;

			if(m_pMdEntry==0)
				MakeModel(&pTemp,&(pMtrl->pSubMtrl[i]),pMesh);
			else
			{
				for(pTemp=m_pMdEntry;pTemp->pNext!=0;pTemp=pTemp->pNext);
				MakeModel(&(pTemp->pNext),&(pMtrl->pSubMtrl[i]),pMesh);
			}
		}

	}

	// ���� face ����Ÿ �޸� ����
	SAFE_DELETE_ARRAY(pBackFace);
	SAFE_DELETE_ARRAY(pnCount);
}


MODEL*	CAseLoader::MakeModelList()
{
	MODEL*		pTemp=0;
	MESH*		pMesh=0;
	int			i;

	// material �迭 ������ ����
	for(i=0;i<m_nNumMtrl;i++)
	{
		list<MESH *>::iterator pi;
		for( pi=m_pMeshList.begin(); pi!=m_pMeshList.end(); ++pi)
		{
			pMesh = (MESH*)(*pi);
			if(pMesh->nMtrlRef==m_pMtrl[i].nID)
			{
				// submaterial �� �����ϸ�
				//if(m_pMtrl[i].uiNumSubMtrl>0)
				//{
					// submaterial ó��
				//	ProcessSubMaterial(&(m_pMtrl[i]),pMesh);
				//}
				//else
				{
					if(m_pMdEntry==0)
						MakeModel(&pTemp,&m_pMtrl[i],pMesh);
					else
					{
                        for(pTemp=m_pMdEntry;pTemp->pNext!=0;pTemp=pTemp->pNext);
						MakeModel(&(pTemp->pNext),&m_pMtrl[i],pMesh);
					}
				}
			}
		}
	}

	Destroy();
	return m_pMdEntry;
}

void CAseLoader::MakeModel(MODEL** pMd,MATERIAL* pMtrl,MESH* pMesh)
{
	int			j;
	char		string[255];
	MODEL*		pTemp=0;
	OBJECT*		pObjTemp=0;

	(*pMd) = new MODEL;
	pTemp = (*pMd);

	if(m_pMdEntry==0) m_pMdEntry = pTemp;

	if(pTemp->pObjList==0)
	{
		pObjTemp = pTemp->pObjList = new OBJECT;
	}
	else
	{
		for(pObjTemp=pTemp->pObjList;pObjTemp->pNext!=0;pObjTemp=pObjTemp->pNext);
		pObjTemp = pObjTemp->pNext = new OBJECT;
	}

	pTemp->Mtrl = pMtrl->Mtrl;
	pObjTemp->nNumFace = pMesh->nNumFace;
	pObjTemp->pVertex = new S_CUSTOMVERTEX[(pMesh->nNumFace)*3];

	pObjTemp->matTm = pMesh->matTm;
	pObjTemp->matInvTm = pMesh->matInvTm;

	for(j=0;j<(pMesh->nNumFace);j++)
	{
		pObjTemp->pVertex[j*3].p = pMesh->pVertex[pMesh->pFace[j].Index._0].p;
		pObjTemp->pVertex[j*3+1].p = pMesh->pVertex[pMesh->pFace[j].Index._1].p;
		pObjTemp->pVertex[j*3+2].p = pMesh->pVertex[pMesh->pFace[j].Index._2].p;

		pObjTemp->pVertex[j*3].n = pMesh->pVertex[pMesh->pFace[j].Index._0].n;
		pObjTemp->pVertex[j*3+1].n = pMesh->pVertex[pMesh->pFace[j].Index._1].n;
		pObjTemp->pVertex[j*3+2].n = pMesh->pVertex[pMesh->pFace[j].Index._2].n;

		if(pMesh->pUV!=0)
		{
			pObjTemp->pVertex[j*3].tu = pMesh->pUV[pMesh->pFace[j].TIndex._0].fU;
			pObjTemp->pVertex[j*3].tv = pMesh->pUV[pMesh->pFace[j].TIndex._0].fV;

			pObjTemp->pVertex[j*3+1].tu = pMesh->pUV[pMesh->pFace[j].TIndex._1].fU;
			pObjTemp->pVertex[j*3+1].tv = pMesh->pUV[pMesh->pFace[j].TIndex._1].fV;

			pObjTemp->pVertex[j*3+2].tv = pMesh->pUV[pMesh->pFace[j].TIndex._2].fU;
			pObjTemp->pVertex[j*3+2].tv = pMesh->pUV[pMesh->pFace[j].TIndex._2].fV;
		}
	}

	if(pMtrl->sTexFile!=0)
	{
		strcpy(string,".\\Data\\");
		strcat(string,pMtrl->sTexFile);
		if( FAILED( D3DXCreateTextureFromFile( CGameEnv::GetObj()->GetDevice(), string, &pTemp->pTexture ) ) )
		{
			memset(string,0,255);
			strcpy(string,"..\\Data\\");
			strcat(string,pMtrl->sTexFile);

			if( FAILED( D3DXCreateTextureFromFile( CGameEnv::GetObj()->GetDevice(), string, &pTemp->pTexture ) ) )
			{
				return ;
			}
		}
	}

	return;
}