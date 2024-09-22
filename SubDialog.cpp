// SubDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "SubDialog.h"


// CSubDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSubDialog, CDialog)
CSubDialog::CSubDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
: CDialog(nIDTemplate, pParent)
{
}

CSubDialog::~CSubDialog()
{
}

void CSubDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSubDialog, CDialog)
END_MESSAGE_MAP()


// CSubDialog 메시지 처리기입니다.

void CSubDialog::OnCancel()
{
}

void CSubDialog::OnOK()
{
}

BOOL CSubDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
