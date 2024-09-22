// WGA2OutdoorsEditorView.h : iCWGA2OutdoorsEditorView Ŭ������ �������̽�
//

#include "TerrainEditingMng.h"
#include ".\WGA2ode\gameworldmanager.h"



class CWGA2OutdoorsEditorView : public CView
{

protected: // serialization������ ��������ϴ�.
	CWGA2OutdoorsEditorView();
	DECLARE_DYNCREATE(CWGA2OutdoorsEditorView)

// Ư��
public:
	CWGA2OutdoorsEditorDoc* GetDocument() const;

// �۾�
private:

	UINT					m_nFillmode;

	void OutputMouseCoordInStatusBar(POINT &ptCursor);				//## ���¹ٿ� ���콺 ��ǥ ���

public:
	CGameWorldManager		m_GameWorldMng;				//## ���� ������
	CTerrainEditingMng		m_TerrainEditingMng;		//## ���� ���� ������

	bool Init();

// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CWGA2OutdoorsEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnFillmode(UINT nID);
	afx_msg void OnUpdateFillmode(CCmdUI *pCmdUI);

	void OnExit();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnOctreeBbox();
	afx_msg void OnUpdateOctreeBbox(CCmdUI *pCmdUI);
	afx_msg void OnTerrainBbox();
	afx_msg void OnUpdateTerrainBbox(CCmdUI *pCmdUI);



	LPDIRECT3DDEVICE9	m_pd3dDevice;
};

CWGA2OutdoorsEditorView *GetViewPointer();

#ifndef _DEBUG  // WGA2OutdoorsEditorView.cpp�� ����� ����
inline CWGA2OutdoorsEditorDoc* CWGA2OutdoorsEditorView::GetDocument() const
   { return reinterpret_cast<CWGA2OutdoorsEditorDoc*>(m_pDocument); }
#endif

