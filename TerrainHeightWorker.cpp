#include "StdAfx.h"
#include "terrainheightworker.h"

#include "terrainmodeditarea.h"
#include "wga2ode\patch.h"

CTerrainHeightWorker::CTerrainHeightWorker(void)
{
	m_uiHeightAdjustment = UP;
	m_fSineHeight = 1.0f;
	m_bSerialUpdate = true;
	m_nUpdateType = 1;
	m_bUpdate = false;
	m_bUpdateStart = false;
}

CTerrainHeightWorker::~CTerrainHeightWorker(void)
{
}

void CTerrainHeightWorker::Update()
{
	//## 편집 영역을 보여준다.
	((CTerrainModEditArea*)m_pEditArea)->Show();

	//## 여기서 해당 제어에 따라 정점들을 수정한다.

	if( m_bUpdate )
	{
		if( !m_pEditArea->IsFixed() )
			m_bUpdateStart = true;


		//## 영역안에 정점 수
		int nNumMapPoint = m_pEditArea->GetSelectedMapPointCount();

		//## map point 시작 포인터
		S_MAPPOINT_INAREA *pMapPoint = m_pEditArea->GetSelectedMapPointsPtr();

		int c;
		float fUpdateHeight, fCurFinalHeight = 0.0f, fCurDistPow, fMaxDistPow;

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

			//## 최대업데이트y * cos( (pi/2)*(현재거리제곱/최대거리제곱) )
			if( m_nUpdateType )
			{
				fCurFinalHeight = (float)m_nLimitHeight * 
					(float)cos( m_fSineHeight*(3.14f/2.0f)*(fCurDistPow/fMaxDistPow) );
			}
			else
				fCurFinalHeight = (float)m_nLimitHeight;

			for( c = 0; c < 4; c++ )
			{
				S_CUSTOMVERTEX *pVertex = (pMapPoint+i)->pMapPoint->paMapPoint[c];

				if( pVertex == NULL )
					break;

				if( m_bUpdateStart && c == 0 )
					m_faFinalHeight[ i ] = pVertex->vP.y + fCurFinalHeight ;

				//## speed 적용
				if( m_bSerialUpdate )
					fUpdateHeight = fabs(m_faFinalHeight[ i ]) * ((float)m_nUpdateSpeed/1000.0f);
				else
					fUpdateHeight = fCurFinalHeight;

				//## 각 정점의 높이 update!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				switch( m_uiHeightAdjustment )
				{
				case UP:
					if( m_faFinalHeight[ i ] > pVertex->vP.y )
						pVertex->vP.y += fUpdateHeight;
					break;
				case DOWN:
					pVertex->vP.y -= fUpdateHeight;
					break;
				case FLAT:
					fUpdateHeight = 0.0f;
					pVertex->vP.y = 0.0f;
					break;
				}

				//## >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				//## HEIGHTMAP 데이타 먼저 수정하고 NORMAL을 구해야 한다.
				//## heightmap 데이타 수정
				if( c == 0 )
					m_pEditArea->GetTerrainPtr()->GetPatchBeginPtr()->AdjustHeightmap(
													(int)(pVertex->vP.x),
                    								-(int)(pVertex->vP.z),
													pVertex->vP.y	);
			}
		}


		//## 백업 normal : 백업해두고 나머지 map point는 대입만한다.
		D3DXVECTOR3 vCopyNormal;

		//## normal을 위한 loop
		//## 정점 수만큼 loop
		for( int ni = 0; ni < nNumMapPoint; ni++ )
		{
			for( c = 0; c < 4; c++ )
			{
				S_CUSTOMVERTEX *pVertex = (pMapPoint+ni)->pMapPoint->paMapPoint[c];

				if( pVertex == NULL )
					break;

				//## normal update
				//## normal은 맨 처음 map point의 normal만 계산해서 구하고
				//## 나머지 map point의 normal은 계산하지 않고 대입만한다.
				//## 이렇게 해야 계산과정을 되풀이하는 overhead를 없애고
				//## patch 구분선이 보이지 않는다.
				if( c == 0 )
				{
					m_pEditArea->GetTerrainPtr()->GetPatchBeginPtr()->BuildNormal(
																	(int)(pVertex->vP.x),
                    												-(int)(pVertex->vP.z),
																	&(pVertex->vN) );
					vCopyNormal = pVertex->vN;
				}
				else
					pVertex->vN = vCopyNormal;

				//## bounding box 조정
				(pMapPoint+ni)->pMapPoint->pPatch[c]->AdjustBbox();
			}
		}

		//## 연속적인 업데이트가 아닐 경우
		if( !m_bSerialUpdate )
		{
			m_bUpdate = false;
		}

		m_bUpdateStart = false;		//## 이제부터는 연속적으로 클릭하고 있는 상태이다.

	}//## if( m_bUpdate )
	else
	{
		//m_fFinalHeight = 0;
		//## 이 플래그를 체크하여 클릭 시작시 딱 한번 초기화한다.
		m_bUpdateStart = true;
	}
}