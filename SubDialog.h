#pragma once


// CSubDialog ��ȭ �����Դϴ�.

class CSubDialog : public CDialog
{
	DECLARE_DYNAMIC(CSubDialog)

public:
	CSubDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSubDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = 0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	virtual void OnCancel();
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
};
