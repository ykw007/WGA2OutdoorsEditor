#pragma once


// CSubDialog 대화 상자입니다.

class CSubDialog : public CDialog
{
	DECLARE_DYNAMIC(CSubDialog)

public:
	CSubDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSubDialog();

// 대화 상자 데이터입니다.
	enum { IDD = 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	virtual void OnCancel();
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
