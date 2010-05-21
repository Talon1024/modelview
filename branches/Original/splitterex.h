////////////////////////////////////////////////////////////////////////////
//
// splitex.h
// (c) 1997, Oleg G. Galkin

class CSplitterWndEx : public CSplitterWnd
{
protected:
     int m_nHidedCol;  // hided column number, -1 if all columns are shown

public:
     CSplitterWndEx();

    void ShowColumn();
    void HideColumn(int colHide);

// ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CSplitterWndEx)
     //}}AFX_VIRTUAL

// Generated message map functions
protected:
     //{{AFX_MSG(CSplitterWndEx)
      // NOTE - the ClassWizard will add and remove member functions here.
     //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

