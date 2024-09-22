#include "StdAfx.h"
#include "observer.h"

CObserver::CObserver()
{
}

CObserver::~CObserver()
{
}

//######################################################################
//## observer���� ����� ��Ʈ���� ����Ѵ�.
//######################################################################
bool CObserver::RegisterControl( UINT uiCtrlId, HWND hWnd )
{
	S_OBSERVER_CONTROL OsvCtrl;

	//## vector�� ������� �ʴٸ� 
	if( !m_vecCtrl.empty() )
	{
		//## loop �ϸ鼭 �ߺ��Ǵ� ctrlID�� �ִ��� �˻��Ѵ�.
		for( vecCtrlItor Itor = m_vecCtrl.begin(); Itor != m_vecCtrl.end(); ++Itor )
		{
			if( uiCtrlId == static_cast<S_OBSERVER_CONTROL>(*Itor).uiCtrlId )
			{
				//## ERROR �޽���
				::MessageBox( NULL,
					"observer control id�� �ߺ��˴ϴ�.\r\n control ����� ����մϴ�.",
					"Error",
					MB_OK );

				return false;
			}
		}
	}

	OsvCtrl.uiCtrlId = uiCtrlId;
	OsvCtrl.hWnd = hWnd;
	m_vecCtrl.push_back( OsvCtrl );

	return true;
}

//######################################################################
//## ��Ʈ�ѿ� ����Ѵ�. float �� ����
//######################################################################
void CObserver::Display( UINT uiCtrlId, float fValue, UINT uiInterval )
{
	char cTemp[255];
	sprintf( cTemp, "%f", fValue );

	_Show( uiCtrlId, cTemp );
}

//######################################################################
//## ��Ʈ�ѿ� ����Ѵ�. int ��
//######################################################################
void CObserver::Display( UINT uiCtrlId, int nValue, UINT uiInterval )
{
	char cTemp[255];
	sprintf( cTemp, "%d", nValue );

	_Show( uiCtrlId, cTemp );
}


//######################################################################
//## ��Ʈ�ѿ� ����Ѵ�. char *
//######################################################################
void CObserver::Display( UINT uiCtrlId, const char *pString, UINT uiInterval )
{
	_Show( uiCtrlId, pString );
}


void CObserver::_Show( UINT uiCtrlId, const char *pString )
{
	S_OBSERVER_CONTROL *pOsvCtrl;

	//## loop �ϸ鼭 �ߺ��Ǵ� ctrlID�� �ִ��� �˻��Ѵ�.
	for( vecCtrlItor Itor = m_vecCtrl.begin(); Itor != m_vecCtrl.end(); ++Itor )
	{
		pOsvCtrl = &(static_cast<S_OBSERVER_CONTROL>(*Itor));
		if( uiCtrlId == pOsvCtrl->uiCtrlId )
		{
			::SetWindowText( pOsvCtrl->hWnd, pString );
		}
	}
}