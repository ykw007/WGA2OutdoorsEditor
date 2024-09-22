#include "StdAfx.h"
#include "landscapescenemanager.h"
#include "observer.h"

CLandscapeSceneManager::CLandscapeSceneManager(void)
{
	m_pd3dDevice = NULL;
	Clear();
}

CLandscapeSceneManager::~CLandscapeSceneManager(void)
{
	SAFE_DELETE(m_pSkybox);
	SAFE_RELEASE( m_pFogEffect );
	Destroy();
}

void CLandscapeSceneManager::Clear()
{
	m_pFogEffect = NULL;
	m_FogHandle = NULL;
	m_pTerrain = NULL;
	m_pSkybox = NULL;
	m_bRenderingSwitch = false;
	m_pTextureMng = NULL;
}

void CLandscapeSceneManager::Destroy()
{
	SAFE_DELETE(m_pTerrain);
}

//####################################################
// ���� ���� ���� ����
//####################################################
void CLandscapeSceneManager::AdjustVec3OverLand(D3DXVECTOR3 *pvPos)
{
	float fMapSize = (float)(m_pTerrain->GetEdgeSpace() );

	//## �����ȿ� ������ ���� ���� ����
	if( ( pvPos->x >= 0.0f && pvPos->z <= 0.0f ) &&
		( pvPos->x <= fMapSize && pvPos->z >= -fMapSize ) )
	{
		pvPos->y = m_pTerrain->GetHeight( pvPos->x, pvPos->z ) + 7.0f;

#ifdef _DEBUG
//	g_pObserver->Show( pvPos->y );
#endif

	}
	else
		pvPos->y = 0.0f;
}


//####################################################
// FOG
//####################################################
bool CLandscapeSceneManager::SetupFogEffect()
{
	HRESULT hr = 0;
	ID3DXBuffer* errorBuffer = 0;

	//## create effect
	hr = D3DXCreateEffectFromFile(	m_pd3dDevice, // associated device
									".\\wga2ode\\Effects\\fog.txt", // effect filename
									//".\\Effects\\fog.txt", // effect filename
									0, // no preprocessor definitions
									0, // no ID3DXInclude interface
									D3DXSHADER_DEBUG, // compile flags
									0, // don't share parameters
									&m_pFogEffect, // return effect interface pointer
									&errorBuffer); // return error messages

    // output any error messages
	if( errorBuffer )
	{
		::MessageBox(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		SAFE_RELEASE( errorBuffer );
	}

	if( FAILED(hr) )
	{
		::MessageBox(0, "D3DXCreateEffectFromFile() - FAILED", 0, 0);
		return false;
	}

	m_FogHandle = m_pFogEffect->GetTechniqueByName("Fog");

}

//####################################################
// �ʱ�ȭ
// D3D rendering device ����
//####################################################
void CLandscapeSceneManager::Init(CTextureManager *pTextureMng, LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pTextureMng = pTextureMng;

	//## ������ ����̽� ����
	if( m_pd3dDevice == NULL )
	{
        m_pd3dDevice = pd3dDevice;

		//## skybox �ʱ�ȭ
		m_pSkybox = new CSkybox();
		m_pSkybox->Init( m_pd3dDevice );

		m_pSkybox->SetTexture(".\\Data\\Source\\Terrain\\Skybox\\front.bmp", 0);
		m_pSkybox->SetTexture(".\\Data\\Source\\Terrain\\Skybox\\back.bmp", 1);
		m_pSkybox->SetTexture(".\\Data\\Source\\Terrain\\Skybox\\left.bmp", 2);
		m_pSkybox->SetTexture(".\\Data\\Source\\Terrain\\Skybox\\right.bmp", 3);
		m_pSkybox->SetTexture(".\\Data\\Source\\Terrain\\Skybox\\top.bmp", 4);
		m_pSkybox->SetTexture(".\\Data\\Source\\Terrain\\Skybox\\bottom.bmp", 5);

		//## FOG SETUP
		SetupFogEffect();

		//## ���� ��ü ������ �ʱ�ȭ
		m_ObjMng.Init( m_pd3dDevice );
	}
}

//####################################################
// CLandscapeSceneManager�� CTerrain �����ϰ�
// terrain ���� �Լ� ȣ��
//####################################################
bool CLandscapeSceneManager::CreateTerrain(int nTerrainType,
				   int nMapSize, int nNumPatchesPerSide, int nCellSpace,
				   LPCTSTR lpHeightMapPath, LPCTSTR lpTexturePath )
{


	//## terrain ����
	SAFE_DELETE(m_pTerrain);
	m_pTerrain = new CTerrain();

	if( m_pTerrain != NULL )
	{
		//## ������ device ����
		m_pTerrain->InitDevice(m_pd3dDevice);

		if( m_pTextureMng != NULL )
			m_pTerrain->SetTextureManager( m_pTextureMng );

		//## ������ ���� �ʱ�ȭ : �⺻ �� ����
		if( !m_pTerrain->Create(nTerrainType,
							nMapSize, nNumPatchesPerSide, nCellSpace,
							lpHeightMapPath, lpTexturePath ) )
		{
			return false;
		}
	}

	//## skybox ũ�� ����
	m_pSkybox->SetSize(280);

	m_bRenderingSwitch = true;

	return true;
}

//## ������
void CLandscapeSceneManager::Render()
{
	if( m_bRenderingSwitch )
	{
		//## ���� ȿ��
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		m_pFogEffect->SetTechnique( m_FogHandle );

		UINT numPasses = 0;

		m_pFogEffect->Begin(&numPasses, 0);

		for(int i = 0; i < numPasses; i++)
		{
			m_pFogEffect->Pass(i);

			m_pd3dDevice->SetFVF( FVF );

			//## SKYBOX
			m_pSkybox->Render();

			//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			m_pTerrain->Render();

			m_ObjMng.Render();
		}

		m_pFogEffect->End();
		//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	}
}