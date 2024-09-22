#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#define DEBUG_FLAG

//## INCLUDES
#include "common.h"
#include "misc.h"
#include "singleton.h"

#include <vector>
using namespace std;


struct S_OBSERVER_CONTROL
{
	UINT			uiCtrlId;
	HWND			hWnd;
};


typedef vector<S_OBSERVER_CONTROL>				vecCtrl;
typedef vecCtrl::iterator			vecCtrlItor;

//####################################################
//####################################################
// DEBUG용
// singleton
//####################################################
//####################################################
class AFX_EXT_CLASS CObserver : public Singleton <CObserver>
{
public:
	CObserver();
	~CObserver();

	bool RegisterControl( UINT uiCtrlId, HWND hWnd );
	void Display( UINT uiCtrlId, float fValue, UINT uiInterval );
	void Display( UINT uiCtrlId, int nValue, UINT uiInterval );
	void Display( UINT uiCtrlId, const char *pString, UINT uiInterval );

private:
	void _Show( UINT uiCtrlId, const char *pString );
private:
	vecCtrl					m_vecCtrl;		//## 연결 ctrl을 등록한 vector
};

#define g_pObserver		CObserver::GetSingletonPtr()

#endif