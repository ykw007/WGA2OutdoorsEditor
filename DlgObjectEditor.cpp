// DlgObjectEditor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgObjectEditor.h"


// CDlgObjectEditor ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgObjectEditor, CDialog)
CDlgObjectEditor::CDlgObjectEditor(CWnd* pParent /*=NULL*/)
	: CSubDialog(CDlgObjectEditor::IDD, pParent)
{
}

CDlgObjectEditor::~CDlgObjectEditor()
{
}

void CDlgObjectEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgObjectEditor, CDialog)
END_MESSAGE_MAP()


// CDlgObjectEditor �޽��� ó�����Դϴ�.
