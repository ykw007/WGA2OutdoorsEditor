#include "StdAfx.h"
#include "observer.h"

CObserver::CObserver()
{
}

CObserver::~CObserver()
{
}

//######################################################################
//## observer에서 사용할 컨트롤을 등록한다.
//######################################################################
bool CObserver::RegisterControl( UINT uiCtrlId, HWND hWnd )
{
	S_OBSERVER_CONTROL OsvCtrl;

	//## vector가 비어있지 않다면 
	if( !m_vecCtrl.empty() )
	{
		//## loop 하면서 중복되는 ctrlID가 있는지 검사한다.
		for( vecCtrlItor Itor = m_vecCtrl.begin(); Itor != m_vecCtrl.end(); ++Itor )
		{
			if( uiCtrlId == static_cast<S_OBSERVER_CONTROL>(*Itor).uiCtrlId )
			{
				//## ERROR 메시지
				::MessageBox( NULL,
					"observer control id가 중복됩니다.\r\n control 등록을 취소합니다.",
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
//## 컨트롤에 출력한다. float 값 전용
//######################################################################
void CObserver::Display( UINT uiCtrlId, float fValue, UINT uiInterval )
{
	char cTemp[255];
	sprintf( cTemp, "%f", fValue );

	_Show( uiCtrlId, cTemp );
}

//######################################################################
//## 컨트롤에 출력한다. int 값
//######################################################################
void CObserver::Display( UINT uiCtrlId, int nValue, UINT uiInterval )
{
	char cTemp[255];
	sprintf( cTemp, "%d", nValue );

	_Show( uiCtrlId, cTemp );
}


//######################################################################
//## 컨트롤에 출력한다. char *
//######################################################################
void CObserver::Display( UINT uiCtrlId, const char *pString, UINT uiInterval )
{
	_Show( uiCtrlId, pString );
}


void CObserver::_Show( UINT uiCtrlId, const char *pString )
{
	S_OBSERVER_CONTROL *pOsvCtrl;

	//## loop 하면서 중복되는 ctrlID가 있는지 검사한다.
	for( vecCtrlItor Itor = m_vecCtrl.begin(); Itor != m_vecCtrl.end(); ++Itor )
	{
		pOsvCtrl = &(static_cast<S_OBSERVER_CONTROL>(*Itor));
		if( uiCtrlId == pOsvCtrl->uiCtrlId )
		{
			::SetWindowText( pOsvCtrl->hWnd, pString );
		}
	}
}