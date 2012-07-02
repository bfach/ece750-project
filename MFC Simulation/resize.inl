// resize.inl : inlines for resize.h
//
// Written by Torben B. Haagh (TBHaagh@hotmail.com)

/////////////////////////////////////////////////////////////////////////////
// CRealRect

inline CRealRect::CRealRect()
{
}

inline CRealRect::CRealRect(const RECT& rc)
{
	(operator =)(rc);
}

inline CRealRect::CRealRect(const REALRECT& rrc)
{
	(operator =)(rrc);
}

inline CRealRect::CRealRect(REAL l, REAL t, REAL r, REAL b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

inline CRealRect & CRealRect::operator = (const RECT& rc) 
{ 
	left = (REAL) rc.left; 
	top = (REAL) rc.top; 
	right = (REAL) rc.right; 
	bottom = (REAL) rc.bottom; 
	return *this;
}

inline CRealRect & CRealRect::operator = (const REALRECT& rrc) 
{ 
	left = rrc.left; 
	top = rrc.top; 
	right = rrc.right; 
	bottom = rrc.bottom; 
	return *this;
}

inline CRealRect & CRealRect::operator += (const REALRECT& rrc) 
{ 
	left += rrc.left; 
	top += rrc.top; 
	right += rrc.right; 
	bottom += rrc.bottom; 
	return *this;
}

inline CRealRect CRealRect::operator + (const REALRECT& rrc) 
{ 
	return CRealRect(left + rrc.left, top + rrc.top, right + rrc.right, bottom += rrc.bottom); 
}

inline BOOL CRealRect::IsNormalized()
{
	return left<=right && top<=bottom;
}

inline CRealRect CRealRect::operator & (const REALRECT& rrc)
{ 
	ASSERT(IsNormalized() && ((CRealRect&) rrc).IsNormalized());

	CRealRect rrcI(max(left,rrc.left), max(top,rrc.top), 
				   min(right,rrc.right), min(bottom,rrc.bottom)); 
	
	// only intersection if resulting rect is normalized
	return (rrcI.IsNormalized()) ? rrcI : CRealRect(0,0,0,0);
}

inline bool CRealRect::operator == (const REALRECT& rrc)
{ 
	return left==rrc.left && top==rrc.top && right==rrc.right && bottom==rrc.bottom; 
}

inline bool CRealRect::operator != (const REALRECT& rrc) 
{ 
	return !operator==(rrc); 
}

inline CRealRect::operator CRect() 
{ 
	return CRect((int) left, (int) top, (int) right, (int) bottom); 
}

inline REAL CRealRect::Width() 
{ 
	return right-left; 
}

inline REAL CRealRect::Height() 
{ 
	return bottom-top; 
}

/////////////////////////////////////////////////////////////////////////////
// CResize

inline void CResize::SetMinSize(CSize& sz) 
{ 
	m_szMin = sz; 
}

inline void CResize::SetMaxSize(CSize& sz) 
{ 
	m_szMax = sz; 
}

inline BOOL CResize::HasFlag(EFlags eFlag)
{
	return (m_nFlags & eFlag) != 0;
}

inline void CResize::SetResize(const UINT nID, REAL left, REAL top, REAL right, REAL bottom)
{
	SetResize(nID, CRealRect(left, top, right, bottom));
}

inline void CResize::SetResize(SIZING arr[])
{
	for (int i=0; i<sizeof(arr); i++)
	{
		SIZING& sz = arr[i];
		SetResize(sz.id, sz.rrc.left, sz.rrc.top, sz.rrc.right, sz.rrc.bottom);
	}
}

inline void CResize::SetResize(CWnd* pwnd, const CRealRect& rrcSizing, CRect rcWindow)
{
	m_arrItems.Add(new CItem(pwnd, rrcSizing, rcWindow, FALSE));
}

inline CResize::CItem::CItem(CWnd* pwnd, const CRealRect& rrcSizing, CRect& rcWindow, BOOL bDeleteWnd) : 
	m_pwnd(pwnd),
	m_rrcSizing(rrcSizing),
	m_rrcWindow(rcWindow),
	m_bDeleteWnd(bDeleteWnd)
{

}

inline CResize::CItem::~CItem()
{
	if (m_bDeleteWnd)
	{
		m_pwnd->Detach();
		delete m_pwnd; 
	}
}
