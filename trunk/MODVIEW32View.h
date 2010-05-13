// MODVIEW32View.h : interface of the CMODVIEW32View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODVIEW32VIEW_H__1ECD6141_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
#define AFX_MODVIEW32VIEW_H__1ECD6141_A743_11D3_9455_00105A9D6FFE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MODVIEW32Doc.h"

#define RENDER_WIREFRAME 0
#define RENDER_SURFACE 1
#define RENDER_TEXTURED 2

#define FS_GUN_CROSSLENGTH 2
#define FS_GUN_CROSSCOLOR_R 255
#define FS_GUN_CROSSCOLOR_G 255
#define FS_GUN_CROSSCOLOR_B 127


#define D3_WireCube FS_WireCube
#define D3_BuildWireCube FS_BuildWireCube

#define QUICKRENDERMODE_NORMAL		0
#define QUICKRENDERMODE_TOQUICK		1
#define QUICKRENDERMODE_QUICK		2
#define QUICKRENDERMODE_TONORMAL	3

class CMainFrame;

class CMODVIEW32View : public CView
{
protected: // create from serialization only
	CMODVIEW32View();
	DECLARE_DYNCREATE(CMODVIEW32View)

// Attributes
public:
	CMODVIEW32Doc* GetDocument();
	CMainFrame *GetMainFrame();

// Operations
public:
	HDC m_hDC;
	HGLRC m_hGLContext;
	int m_GLPixelIndex;
	float PI;
	
	int m_QuickRenderMode;
	void QuickRenderMode_ChangeToNormal();
	void QuickRenderMode_ChangeToQuick();
	BOOL SmartRenderEngine_SkipBuild();
	int SmartRenderEngine_RenderMode();

	//OLD
		float m_xRotation;
		float m_yRotation;
		float m_xScaling;
		float m_yScaling;
		float m_zScaling;
		float m_xTranslation;
		float m_yTranslation;
		float m_zTranslation;
	//NEW
		GLfloat m_DirZ, m_DirY, m_DirX;
		GLfloat m_PosZ, m_PosY, m_PosX;
		GLfloat m_AngleX, m_AngleY, m_AngleZ;
		GLfloat m_PosIncr;
		GLfloat m_AngIncr;


	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;

	GLfloat nRange;
	int lastHeight;
	int lastWidth;
	CRect m_ClientRect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMODVIEW32View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	void RF_RenderScene();
	void RF_BuildScene();
	void FS_RotateParts(int subm);
	float m_Rotation;
	BOOL m_DoRotation;

	void CopyToClipboard();
	void OnKeyDown2(UINT nChar, UINT nRepCnt, UINT nFlags);
	void PaintErrorMessage(CString x);
	void DrawCross(FS_VPNT point, int len);
	int FS_CalcRealSubmodelNumber(int subm);
	int m_EditorFS_HighLight_Segment_2;
	int m_EditorFS_HighLight_Segment_1;
	int m_EditorFS_GPNT_FiringPoint;
	int m_EditorFS_TGUN_FiringPoint;
	int m_EditorFS_MODEL_CurLight;
	BOOL m_EditorFS_ShowModelInfo;
	BOOL m_D2_SmoothPositionChange;
	void D3_PreparePosition();
	void D2_ResetPosAngleData_StepBit();
	void D2_ResetPosAngleData_ShallBe();
	void D2_ResetPosAngleData_Current();
	BOOL D2_ParsePosAngle(short *cur,short *sha,short *stp);
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	void ResetGeometry();
	void DrawCartAxis();
	void ChangeSize();
	void ChangeSize(int w, int h);
	GLint GetZbufferConst();
	void RenderIndicator();
	HPALETTE GetOpenGLPalette(HDC hDC);
	int m_DisplaySubmodel;
	int m_DisplayTexture;
	int m_FS_DisplayThruster;
	int m_FS_DisplaySubsystem;
	int m_DisplayGun;
	BOOL m_ShowSegments;
	BOOL m_ShowGuns;
	int Axis;
	BOOL m_RenderZbuffer;
	BOOL m_SkipBuild;
	BOOL m_FastTextureLoad;
	unsigned int m_Detaillevel;
	BOOL m_ShowThruster;
	BOOL m_ShowShield;
	BOOL m_FS_ShowDockingPoints;
	BOOL m_FS_ShowThrusterGlows;
	int m_RenderMode;
	BOOL m_RenderSmooth;
	BOOL m_RenderCull;

	//CPoint m_RightDownPos;
	//BOOL m_RightButtonDown;
	//CPoint m_LeftDownPos;
	//BOOL m_LeftButtonDown;
#define MOUSEMOVEMODE_NONE		0
#define MOUSEMOVEMODE_ROTATE	1
#define MOUSEMOVEMODE_ZOOM		2
#define MOUSEMOVEMODE_PANE		3
	int m_MouseMoveMode;
	CPoint m_MouseMovePos;


	GLuint m_D2_TestModel;
	int m_D2_Position;
	D2_VMS_ANGVEC m_D2_PosAngle_ShallBe[MAX_D2_SUBMODELS];
	D2_VMS_ANGVEC m_D2_PosAngle_Current[MAX_D2_SUBMODELS];
	D2_VMS_ANGVEC m_D2_PosAngle_StepBit[MAX_D2_SUBMODELS];
	int D2_RenderScene();
	int D2_BuildScene();
	void D2_BuildWireCube(D2_VMS_VECTOR min,D2_VMS_VECTOR max);
	void D2_PreparePosition();
	void D2_SetPosition(unsigned int pos);
	
	BOOL m_RenderD3Flat;
	BOOL m_RenderD3Smooth;
	BOOL m_D3_Rotate;
	unsigned int m_D3_Position;
	GLuint  m_D3_ModelSobj[60];
	GLuint	m_D3_CrossPnts;
	BOOL    m_D3_LoadedSobj[60];
	D3_DISPLAYPOLY m_D3_Dpoly[4000];
	D3_DISPLAYSOBJ m_D3_Dsobj[MAX_D3_SOBJ];
	GLfloat m_D3_Dnorm[20000][3];
	GLfloat m_D3_Dvert[20000][3];
	GLfloat m_D3_Duv[20000][2];
	void D3_BuildPnts(unsigned int SubM);
	void D3_BuildSobj(unsigned short SobjNum);
	void D3_BuildScene();
	void D3_BuildArrays();
	void D3_CrossPnt(D3_VPNT xhair);
	void D3_CrossPntN(D3_VPNT xhair, D3_VPNT Dir);
	void D3_RenderScene();
	void D3_SetPosition(unsigned int pos);
	D3_POSANGLE m_D3_PosAngle_ShallBe[MAX_D3_SOBJ];
	D3_POSANGLE m_D3_PosAngle_Current[MAX_D3_SOBJ];
	D3_POSANGLE m_D3_PosAngle_StepBit[MAX_D3_SOBJ];
	
	void FS_BuildScene();
	void FS_SetDetailLevel(int mode);
	float FS_WireCube[25][3];
	void FS_BuildWireCube(FS_VPNT min,FS_VPNT max);

	virtual ~CMODVIEW32View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMODVIEW32View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MODVIEW32View.cpp
inline CMODVIEW32Doc* CMODVIEW32View::GetDocument()
   { return (CMODVIEW32Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODVIEW32VIEW_H__1ECD6141_A743_11D3_9455_00105A9D6FFE__INCLUDED_)
