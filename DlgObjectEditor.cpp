// DlgObjectEditor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "DlgObjectEditor.h"


// CDlgObjectEditor 대화 상자입니다.

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


// CDlgObjectEditor 메시지 처리기입니다.
