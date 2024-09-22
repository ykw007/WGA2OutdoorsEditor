// WGA2OutdoorsEditorDoc.h : CWGA2OutdoorsEditorDoc Ŭ������ �������̽�
//
#ifndef DOC_H
#define DOC_H

#pragma once

class CWGA2OutdoorsEditorDoc : public CDocument
{

protected: // serialization������ ��������ϴ�.
	CWGA2OutdoorsEditorDoc();
	DECLARE_DYNCREATE(CWGA2OutdoorsEditorDoc)

// Ư��
public:

// �۾�
public:

	void ClearGameWorld();

// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CWGA2OutdoorsEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:

	int	m_x;
	int m_z;

	afx_msg void OnCreateTerrain();
};



#endif