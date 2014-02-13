#pragma once


// CSortableStringArray
typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);
typedef int (__cdecl *STRINGCOMPAREFN)(const CString * elem1, const CString * elem2);

class CSortableStringArray : public CStringArray
{
public:
	CSortableStringArray();
	virtual ~CSortableStringArray();
	void Sort(STRINGCOMPAREFN pfnCompare = Compare);
	static int __cdecl Logical(const CString * pstr1, const CString * pstr2);
	static int __cdecl Compare(const CString * pstr1, const CString * pstr2);
	static int __cdecl Reverse(const CString * pstr1, const CString * pstr2);
	static int __cdecl ReverseLogical(const CString * pstr1, const CString * pstr2);
protected:
	static int __cdecl StrCmpLogical(const CString* pstr1, const CString* pstr2);
};


