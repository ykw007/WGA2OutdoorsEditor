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
	//## 편집 영역을 보여준다.
	m_pEditArea->Show();

	//## 여기서 해당 제어에 따라 정점들을 수정한다.
	if( GetAsyncKeyState( VK_RBUTTON ) != 0 )
	{
		//## 영역안에 정점 수
		int nNumMapPoint = m_pEditArea->GetSelectedMapPointCount();

		//## map point 시작 포인터
		S_MAPPOINT_INAREA *pMapPoint = m_pEditArea->GetSelectedMapPointsPtr();

		int c;
		float fCurDistPow, fMaxDistPow;

		//## 정점 수만큼 loop
		for( int i = 0; i < nNumMapPoint; i++ )
		{
			//## 곡선처럼 만들기
			//## 거리가 0에 가까울수록 y값이 높고
			//## 최대거리에 가까울수록 y값은 0에 가까워진다.

			//## 최대거리는 i가 0일때 map point의 거리이다.
			if( i == 0 )
                fMaxDistPow = (pMapPoint+i)->fDistPow;

			//## 현재 map point 거리제곱
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