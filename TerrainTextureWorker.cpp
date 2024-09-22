#include "StdAfx.h"
#include "terraintextureworker.h"
#include "wga2ode\texturemanager.h"

#define TEX2 \
"D:\\My_Projects\\Terrain_Map_Tool\\WGA2OutdoorsEditor_dll_v\\Data\\Source\\Terrain\\Textures\\grass.jpg"

#define FTEX \
"D:\\My_Projects\\Terrain_Map_Tool\\WGA2OutdoorsEditor_dll_v\\Data\\f.bmp"

CTerrainTextureWorker::CTerrainTextureWorker(void)
{
}

CTerrainTextureWorker::~CTerrainTextureWorker(void)
{
}

void CTerrainTextureWorker::Update()
{
	//## ���� ������ �����ش�.
	m_pEditArea->Show();

	//## ���⼭ �ش� ��� ���� �������� �����Ѵ�.
	if( GetAsyncKeyState( VK_RBUTTON ) != 0 )
	{
		//## �����ȿ� ���� ��
		int nNumMapPoint = m_pEditArea->GetSelectedMapPointCount();

		//## map point ���� ������
		S_MAPPOINT_INAREA *pMapPoint = m_pEditArea->GetSelectedMapPointsPtr();

		int c;
		float fCurDistPow, fMaxDistPow;

		//## ���� ����ŭ loop
		for( int i = 0; i < nNumMapPoint; i++ )
		{
			//## �ó�� �����
			//## �Ÿ��� 0�� �������� y���� ����
			//## �ִ�Ÿ��� �������� y���� 0�� ���������.

			//## �ִ�Ÿ��� i�� 0�϶� map point�� �Ÿ��̴�.
			if( i == 0 )
                fMaxDistPow = (pMapPoint+i)->fDistPow;

			//## ���� map point �Ÿ�����
			fCurDistPow = (pMapPoint+i)->fDistPow;

			for( c = 0; c < 4; c++ )
			{
				S_CUSTOMVERTEX *pVertex = (pMapPoint+i)->pMapPoint->paMapPoint[c];

				if( pVertex == NULL )
					break;


			}//## for( c = 0; c < 4; c++ )

		}//## for( int i = 0; i < nNumMapPoint; i++ )

	}

}