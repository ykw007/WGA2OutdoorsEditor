// WGA2OutdoorsEditorView.h : iCWGA2OutdoorsEditorView 클래스의 인터페이스
//

#include "TerrainEditingMng.h"
#include ".\WGA2ode\gameworldmanager.h"



class CWGA2OutdoorsEditorView : public CView
{

protected: // serialization에서만 만들어집니다.
	CWGA2OutdoorsEditorView();
	DECLARE_DYNCREATE(CWGA2OutdoorsEditorView)

// 특성
public:
	CWGA2OutdoorsEditorDoc* GetDocument() const;

// 작업
private:

	UINT					m_nFillmode;

	void OutputMouseCoordInStatusBar(POINT &ptCursor);				//## 상태바에 마우스 좌표 출력

public:
	CGameWorldManager		m_GameWorldMng;				//## 월드 관리자
	CTerrainEditingMng		m_TerrainEditingMng;		//## 지형 편집 관리자

	bool Init();

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현
public:
	virtual ~CWGA2OutdoorsEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
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

#ifndef _DEBUG  // WGA2OutdoorsEditorView.cpp의 디버그 버전
inline CWGA2OutdoorsEditorDoc* CWGA2OutdoorsEditorView::GetDocument() const
   { return reinterpret_cast<CWGA2OutdoorsEditorDoc*>(m_pDocument); }
#endif

