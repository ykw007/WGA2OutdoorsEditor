// WGA2OutdoorsEditorDoc.h : CWGA2OutdoorsEditorDoc 클래스의 인터페이스
//
#ifndef DOC_H
#define DOC_H

#pragma once

class CWGA2OutdoorsEditorDoc : public CDocument
{

protected: // serialization에서만 만들어집니다.
	CWGA2OutdoorsEditorDoc();
	DECLARE_DYNCREATE(CWGA2OutdoorsEditorDoc)

// 특성
public:

// 작업
public:

	void ClearGameWorld();

// 재정의
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 구현
public:
	virtual ~CWGA2OutdoorsEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:

	int	m_x;
	int m_z;

	afx_msg void OnCreateTerrain();
};



#endif