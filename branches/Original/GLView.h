// GLView.h : interface of the CGLView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__4AC3D196_94EC_11D2_B21D_00C04FBEDB8F__INCLUDED_)
#define AFX_GLVIEW_H__4AC3D196_94EC_11D2_B21D_00C04FBEDB8F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// OpenGL headers
//#include <gl\gl.h>
//#include <gl\glu.h>

// Custom palette structure
typedef struct tagLogicalPalette {
	WORD wVersion;
	WORD wNumEntries;
	PALETTEENTRY peaEntries[256];
} LOGICALPALETTE;

class CGLView : public CView
{
protected: // create from serialization only
	CGLView();
	DECLARE_DYNCREATE(CGLView)

// Attributes
public:

	GLfloat nRange;
// Operations
public:

protected:
	void SetupPalette();
	void DrawCartAxis();
	int Axis;
	void DrawGrid();
	
	float m_xScaling;
	float m_yScaling;
	float m_zScaling;
	float m_xRotation;
	float m_yRotation;
	float m_xTranslation;
	float m_yTranslation;
	float m_zTranslation;
	CRect m_ClientRect;

	CPoint m_LeftDownPos;
	BOOL m_LeftButtonDown;
	CPoint m_RightDownPos;
	BOOL m_RightButtonDown;

	BOOL m_RBControl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLView)
	public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void RenderIndicator();
	virtual ~CGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	// OpenGL palettes and structures
    HGLRC m_hRC;
	HPALETTE m_hPalette;

	//{{AFX_MSG(CGLView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLVIEW_H__4AC3D196_94EC_11D2_B21D_00C04FBEDB8F__INCLUDED_)
