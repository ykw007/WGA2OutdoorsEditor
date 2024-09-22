#include "SubDialog.h"
#include "afxwin.h"


// CDlgbarObserver 대화 상자입니다.

class CDlgbarObserver : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgbarObserver)

public:
	CDlgbarObserver(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgbarObserver();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLGBAR_OBSERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CEdit m_ctrlEditRecord;
	CStatic m_StcFps;
	CStatic m_stcNumTerrainFaces;
	CStatic m_stcNumTerrainObstacleFaces;
};
