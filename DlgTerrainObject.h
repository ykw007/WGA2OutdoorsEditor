


#include "SubDialog.h"
#include "afxcmn.h"
#include "wga2ode\model.h"

#include <list>
#include "afxwin.h"

using namespace std;

#ifndef _DLG_TERRAINOBJECT_H_
#define _DLG_TERRAINOBJECT_H_

struct S_TREE_OBJECT
{
	char	*pName;
	CModel	*pModel;

	~S_TREE_OBJECT()
	{
		SAFE_DELETE_ARRAY( pName );
		SAFE_DELETE( pModel );
	}
};

typedef list<S_TREE_OBJECT *>			lstTreeObj;
typedef lstTreeObj::iterator			lstTreeObjItor;

// CDlgTerrainObject 대화 상자입니다.


class CDlgTerrainObject : public CSubDialog
{
	DECLARE_DYNAMIC(CDlgTerrainObject)

public:
	CDlgTerrainObject(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTerrainObject();

	void Destroy();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_TERRAIN_OBJECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:

public:


	virtual BOOL OnInitDialog();
	bool InsertTerrainObject( CModel *pModel, const char *pFilePathName );
	void UpdateRadius();

	afx_msg void OnTvnSelchangedTerrainObjectLib(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedTransObject();
	afx_msg void OnBnClickedAddObject();
	afx_msg void OnBnClickedDeleteObject();
	afx_msg void OnBnClickedImportTerrainObj();
public:

	CImageList			m_ImageList;
	CTreeCtrl			m_treeTObj;
	lstTreeObj			m_lstTreeObj;
	CStatic				m_stcObjName;
	CButton				m_rdoAddObject;
};

#endif