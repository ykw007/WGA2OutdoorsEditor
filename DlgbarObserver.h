#include "SubDialog.h"
#include "afxwin.h"


// CDlgbarObserver ��ȭ �����Դϴ�.

class CDlgbarObserver : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgbarObserver)

public:
	CDlgbarObserver(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgbarObserver();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGBAR_OBSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CEdit m_ctrlEditRecord;
	CStatic m_StcFps;
	CStatic m_stcNumTerrainFaces;
	CStatic m_stcNumTerrainObstacleFaces;
};
