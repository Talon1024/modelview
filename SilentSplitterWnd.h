// class definition
class CSilentSplitterWnd :public CSplitterWnd
{
public:
	CSilentSplitterWnd();
	virtual ~CSilentSplitterWnd();
	int HitTest(CPoint pt)const;
protected:
	DECLARE_MESSAGE_MAP()
};
