#include "SubDialog.h"

// CDlgObjectEditor 대화 상자입니다.

class CDlgObjectEditor : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgObjectEditor)

public:
	CDlgObjectEditor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgObjectEditor();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_OBJECT_EDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
