#pragma once


// CSortableStringArray
typedef int (__cdecl *GENERICCOMPAREFN)(const void * elem1, const void * elem2);
typedef int (__cdecl *STRINGCOMPAREFN)(const CString * elem1, const CString * elem2);

class CSortableStringArray : public CStringArray
{
public:
	CSortableStringArray();
	virtual ~CSortableStringArray();
	void Sort(STRINGCOMPAREFN pfnCompare = NormalSort);
	static int __cdecl NormalSort(const CString * pstr1, const CString * pstr2);
	static int __cdecl LogicalSort(const CString * pstr1, const CString * pstr2);
	static int __cdecl ReverseSort(const CString * pstr1, const CString * pstr2);
	static int __cdecl ReverseLogicalSort(const CString * pstr1, const CString * pstr2);
protected:
	static int __cdecl StrCmpLogical(const CString* pstr1, const CString* pstr2);
};


