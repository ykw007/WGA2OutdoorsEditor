#ifndef _WGA2BUTTON_H_
#define _WGA2BUTTON_H_


//## Return values
#ifndef	BTNRN_OK
#define	BTNRN_OK						0
#endif
#ifndef	BTNRN_INVALIDINDEX
#define	BTNRN_INVALIDINDEX				3
#endif

// CWGA2Button

class CWGA2Button : public CButton
{
	DECLARE_DYNAMIC(CWGA2Button)

public:
	CWGA2Button();
	virtual ~CWGA2Button();

	//## 칼라 설정
	//## 안눌러지거나, 눌러졌을때 배경색, 글자색
	enum STATE_COLOR { 
		BG_NOPUSH,
		BG_PUSH,
		FONT_NOPUSH,
		FONT_PUSH,
		MAX_COLOR
	};

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	virtual void OnDrawBackground(CDC* pDC, LPCRECT pRect);
	virtual DWORD OnDrawBorder(CDC* pDC, LPCRECT pRect);

public:


protected:
	virtual void PreSubclassWindow();
	afx_msg BOOL OnClicked();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	void SetChecking(int nCheck){ m_nCheck = nCheck; Invalidate(); }
	DWORD SetDefaultColors(BOOL bRepaint = 1);
	void SetEnabled(BOOL bEnabled = TRUE)
	{
		m_nCheck =  0 ;
		m_bIsDisabled = !bEnabled; 
	} 

	void SetColor( BYTE byStateColor, COLORREF crColor, BOOL bRepaint = TRUE );


protected:
	CBrush			m_brush;			//## 브러시
	bool			m_bIsPressed;
	bool			m_bMouseOnButton;
	COLORREF		m_crColors[MAX_COLOR];
	BOOL			m_bIsDisabled;

private:
	bool			m_bIsCheckBox;					//## checkbox 버튼인가?
	bool			m_bIsFlat; 
	bool			m_bDrawBorder; 
	int				m_nCheck;

	void CancelHover();

};


#endif