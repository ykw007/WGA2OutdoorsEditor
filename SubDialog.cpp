// SubDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "SubDialog.h"


// CSubDialog ��ȭ �����Դϴ�.

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


// CSubDialog �޽��� ó�����Դϴ�.

void CSubDialog::OnCancel()
{
}

void CSubDialog::OnOK()
{
}

BOOL CSubDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
