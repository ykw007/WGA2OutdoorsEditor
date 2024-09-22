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
	//## ���� ������ �����ش�.
	((CTerrainModEditArea*)m_pEditArea)->Show();

	//## ���⼭ �ش� ��� ���� �������� �����Ѵ�.

	if( m_bUpdate )
	{
		if( !m_pEditArea->IsFixed() )
			m_bUpdateStart = true;


		//## �����ȿ� ���� ��
		int nNumMapPoint = m_pEditArea->GetSelectedMapPointCount();

		//## map point ���� ������
		S_MAPPOINT_INAREA *pMapPoint = m_pEditArea->GetSelectedMapPointsPtr();

		int c;
		float fUpdateHeight, fCurFinalHeight = 0.0f, fCurDistPow, fMaxDistPow;

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

			//## �ִ������Ʈy * cos( (pi/2)*(����Ÿ�����/�ִ�Ÿ�����) )
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

				//## speed ����
				if( m_bSerialUpdate )
					fUpdateHeight = fabs(m_faFinalHeight[ i ]) * ((float)m_nUpdateSpeed/1000.0f);
				else
					fUpdateHeight = fCurFinalHeight;

				//## �� ������ ���� update!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
				//## HEIGHTMAP ����Ÿ ���� �����ϰ� NORMAL�� ���ؾ� �Ѵ�.
				//## heightmap ����Ÿ ����
				if( c == 0 )
					m_pEditArea->GetTerrainPtr()->GetPatchBeginPtr()->AdjustHeightmap(
													(int)(pVertex->vP.x),
                    								-(int)(pVertex->vP.z),
													pVertex->vP.y	);
			}
		}


		//## ��� normal : ����صΰ� ������ map point�� ���Ը��Ѵ�.
		D3DXVECTOR3 vCopyNormal;

		//## normal�� ���� loop
		//## ���� ����ŭ loop
		for( int ni = 0; ni < nNumMapPoint; ni++ )
		{
			for( c = 0; c < 4; c++ )
			{
				S_CUSTOMVERTEX *pVertex = (pMapPoint+ni)->pMapPoint->paMapPoint[c];

				if( pVertex == NULL )
					break;

				//## normal update
				//## normal�� �� ó�� map point�� normal�� ����ؼ� ���ϰ�
				//## ������ map point�� normal�� ������� �ʰ� ���Ը��Ѵ�.
				//## �̷��� �ؾ� �������� ��Ǯ���ϴ� overhead�� ���ְ�
				//## patch ���м��� ������ �ʴ´�.
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

				//## bounding box ����
				(pMapPoint+ni)->pMapPoint->pPatch[c]->AdjustBbox();
			}
		}

		//## �������� ������Ʈ�� �ƴ� ���
		if( !m_bSerialUpdate )
		{
			m_bUpdate = false;
		}

		m_bUpdateStart = false;		//## �������ʹ� ���������� Ŭ���ϰ� �ִ� �����̴�.

	}//## if( m_bUpdate )
	else
	{
		//m_fFinalHeight = 0;
		//## �� �÷��׸� üũ�Ͽ� Ŭ�� ���۽� �� �ѹ� �ʱ�ȭ�Ѵ�.
		m_bUpdateStart = true;
	}
}