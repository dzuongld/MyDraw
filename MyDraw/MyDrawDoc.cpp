// MyDrawDoc.cpp : implementation of the CMyDrawDoc class
//

#include "stdafx.h"
#include "MyDraw.h"

#include "MyDrawDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyDrawDoc

IMPLEMENT_DYNCREATE(CMyDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CMyDrawDoc, CDocument)
END_MESSAGE_MAP()


// CMyDrawDoc construction/destruction

CMyDrawDoc::CMyDrawDoc()
{
	// TODO: add one-time construction code here
	drawing = new Drawing();

}

CMyDrawDoc::~CMyDrawDoc()
{
	delete drawing;
}

BOOL CMyDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMyDrawDoc serialization

void CMyDrawDoc::Serialize(CArchive& ar)
{
	CDocument::Serialize(ar);
	if (ar.IsStoring()) {
		ar << drawing;
		
	} else {
		
		ar >> drawing;
		
	}
	
}


// CMyDrawDoc diagnostics

#ifdef _DEBUG
void CMyDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMyDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMyDrawDoc commands
