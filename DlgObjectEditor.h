#include "SubDialog.h"

// CDlgObjectEditor ��ȭ �����Դϴ�.

class CDlgObjectEditor : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgObjectEditor)

public:
	CDlgObjectEditor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgObjectEditor();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_OBJECT_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
