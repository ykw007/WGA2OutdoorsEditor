// WGA2Button.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WGA2OutdoorsEditor.h"
#include "WGA2Button.h"


// CWGA2Button

IMPLEMENT_DYNAMIC(CWGA2Button, CButton)
CWGA2Button::CWGA2Button()
: m_bIsCheckBox(false)
, m_bIsPressed(false)
, m_nCheck(0)
{
	//m_brush.CreateSolidBrush(RGB(0,0,255));
	
	m_bIsFlat = TRUE;			//## flat button type
	m_bDrawBorder = TRUE;		//## 외곽선
	m_bMouseOnButton = FALSE;
	m_bIsDisabled		= FALSE;

	SetDefaultColors(FALSE);
}

CWGA2Button::~CWGA2Button()
{
}


BEGIN_MESSAGE_MAP(CWGA2Button, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CWGA2Button 메시지 처리기입니다.


HBRUSH CWGA2Button::CtlColor(CDC* pDC, UINT nCtlColor)
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
	//pDC->SetTextColor(RGB(255,0,0));
	//pDC->SetBkColor(RGB(0,255,0));
	//return (HBRUSH)m_brush;
}

void CWGA2Button::PreSubclassWindow()
{
	//## 버튼 스타일 얻어냄
	UINT uiBS = GetButtonStyle();

	if( uiBS & BS_CHECKBOX ) m_bIsCheckBox = true;

	SetButtonStyle( uiBS | BS_OWNERDRAW );

	CButton::PreSubclassWindow();
}

DWORD CWGA2Button::SetDefaultColors(BOOL bRepaint)
{
	m_crColors[BG_PUSH]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[FONT_PUSH]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BG_NOPUSH]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[FONT_NOPUSH]	= ::GetSysColor(COLOR_BTNTEXT);

	if (bRepaint == TRUE)
		Invalidate();

	return BTNRN_OK;
}


//####################################################
// 버튼 칼라 설정
//####################################################
void CWGA2Button::SetColor(BYTE byStateColor, COLORREF crColor, BOOL bRepaint)
{
	//## 칼라 설정
	m_crColors[byStateColor] = crColor;

	//## 다시 그린다.
	if (bRepaint == TRUE)
		Invalidate();
}

//####################################################
// 버튼 클릭시 동작
//####################################################
BOOL CWGA2Button::OnClicked()
{
	//## 체크 박스이면
	if ( m_bIsCheckBox )
	{
		//## 체크 설정
		m_nCheck = !m_nCheck;

		//## 버튼 업데이트
		Invalidate();
	}

	return FALSE;
}


//####################################################
// 버튼 그리기
//####################################################
void CWGA2Button::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle( lpDrawItemStruct->hDC );
	CPen	*pOldPen;

	if( m_bIsCheckBox )
	{
		m_bIsPressed = ( lpDrawItemStruct->itemState & ODS_SELECTED ) || ( m_nCheck != 0 );
	}

	CRect itemRect = lpDrawItemStruct->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	OnDrawBackground(pDC, &itemRect);

	//## 눌려졌을 때 버튼을 그린다
	if (m_bIsPressed)
	{
		if (m_bIsFlat == TRUE)
		{
			if (m_bDrawBorder)
				OnDrawBorder(pDC, &itemRect);
		}
		else   
		{
			CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
			pDC->FrameRect(&itemRect, &brBtnShadow);
		}

	}
	else // ...else draw non pressed button
	{
		CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
		CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
		CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
		CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

		/*
		if (m_bIsFlat == TRUE)
		{
			if (m_bMouseOnButton && m_bDrawBorder)
				OnDrawBorder(pDC, &itemRect);
		}
		else
			*/
		{
			// Draw top-left borders
			// White line
			pOldPen = pDC->SelectObject(&penBtnHiLight);
			pDC->MoveTo(itemRect.left, itemRect.bottom-1);
			pDC->LineTo(itemRect.left, itemRect.top);
			pDC->LineTo(itemRect.right, itemRect.top);
			// Light gray line
			pDC->SelectObject(pen3DLight);
			pDC->MoveTo(itemRect.left+1, itemRect.bottom-1);
			pDC->LineTo(itemRect.left+1, itemRect.top+1);
			pDC->LineTo(itemRect.right, itemRect.top+1);
			// Draw bottom-right borders
			// Black line
			pDC->SelectObject(pen3DDKShadow);
			pDC->MoveTo(itemRect.left, itemRect.bottom-1);
			pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
			pDC->LineTo(itemRect.right-1, itemRect.top-1);
			// Dark gray line
			pDC->SelectObject(penBtnShadow);
			pDC->MoveTo(itemRect.left+1, itemRect.bottom-2);
			pDC->LineTo(itemRect.right-2, itemRect.bottom-2);
			pDC->LineTo(itemRect.right-2, itemRect.top);
			//
			pDC->SelectObject(pOldPen);

		}
	}

	//## 버튼에 글자를 넣는다.
	CString sTitle;
	GetWindowText(sTitle);


	CRect captionRect = itemRect;

	// Write the button title (if any)
	if (sTitle.IsEmpty() == FALSE)
	{
		// Draw the button's title
		// If button is pressed then "press" title also
		if (m_bIsPressed && m_bIsCheckBox == FALSE)
			captionRect.OffsetRect(1, 1);
    
		// Center text
		CRect centerRect = captionRect;
		pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);

		pDC->SetBkMode(TRANSPARENT);

		//## 사용할 수 없다면
		if (m_bIsDisabled)
		{
			//## 사용할 수 없는 버튼 모양을 그린다.
			captionRect.OffsetRect(1, 1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
			captionRect.OffsetRect(-1, -1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
		else
		{
			if (m_bMouseOnButton || m_bIsPressed) 
			{
				pDC->SetTextColor(m_crColors[FONT_PUSH]);
				pDC->SetBkColor(m_crColors[BG_PUSH]);
			} // if
			else 
			{
				pDC->SetTextColor(m_crColors[FONT_NOPUSH]);
				pDC->SetBkColor(m_crColors[BG_NOPUSH]);
			} // else
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		} // if
	} // if
}


//####################################################
// 배경색을 출력한다.
//####################################################
void CWGA2Button::OnDrawBackground(CDC* pDC, LPCRECT pRect)
{

	COLORREF	crColor;

	//## 버튼 위에 마우스가 올라갔거나 버튼이 눌려진 상태인 경우 버튼 배경 색 설정
//	if (m_bMouseOnButton || m_bIsPressed)
	//## 버튼이 눌려진 상태만 버튼 배경 색 설정
	if ( m_bIsPressed )
		crColor = m_crColors[BG_PUSH];
	else
		crColor = m_crColors[BG_NOPUSH];

	//## 설정된 색으로 배경 채우기
	CBrush		brBackground(crColor);
	pDC->FillRect(pRect, &brBackground);
}

DWORD CWGA2Button::OnDrawBorder(CDC* pDC, LPCRECT pRect)
{
	if (m_bIsPressed)
		pDC->Draw3dRect(pRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
	else
		pDC->Draw3dRect(pRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));

	return BTNRN_OK;
}

void CWGA2Button::OnMouseMove(UINT nFlags, CPoint point)
{
	CWnd*	pWnd;		// Active window
	CWnd*	pParent;	// Window that owns the button

	CButton::OnMouseMove(nFlags, point);

	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE)
		return;

	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	pWnd = GetActiveWindow();
	pParent = GetOwner();

	if ((GetCapture() != this) && 
		(
#ifndef ST_LIKEIE
		pWnd != NULL && 
#endif
		pParent != NULL)) 
	{
		m_bMouseOnButton = TRUE;
		//SetFocus();	// Thanks Ralph!
		SetCapture();
		Invalidate();
	} // if
	else
	{
		/*
		CRect rc;
		GetClientRect(&rc);
		if (!rc.PtInRect(point))
		{
		*/

		POINT p2 = point;
		ClientToScreen(&p2);
		CWnd* wndUnderMouse = WindowFromPoint(p2);
//		if (wndUnderMouse != this)
		if (wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd)
		{
			// Redraw only if mouse goes out
			if (m_bMouseOnButton == TRUE)
			{
				m_bMouseOnButton = FALSE;
				Invalidate();
			} // if
			// If user is NOT pressing left button then release capture!
			if (!(nFlags & MK_LBUTTON)) ReleaseCapture();
		} // if
	}
}

void CWGA2Button::CancelHover()
{
	// If our button is not flat then do nothing
	if (m_bIsFlat == FALSE) return;

	if (m_bMouseOnButton == TRUE)
	{
		m_bMouseOnButton = FALSE;
		Invalidate();
	} // if
}

void CWGA2Button::OnKillFocus(CWnd* pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	CancelHover();
}

#undef ST_LIKEIE