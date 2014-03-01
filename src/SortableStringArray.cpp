// SortableStringArray.cpp : 実装ファイル
//

#include "stdafx.h"
#include "uniq.h"
#include "SortableStringArray.h"

#include "shlwapi.h"

// CSortableStringArray

CSortableStringArray::CSortableStringArray()
{
}

CSortableStringArray::~CSortableStringArray()
{
}


// CSortableStringArray メッセージ ハンドラ
int CSortableStringArray::NormalSort(const CString * pstr1, const CString * pstr2)
{
	ASSERT(pstr1);
	ASSERT(pstr2);
	return pstr1->Compare(*pstr2);
}

int CSortableStringArray::ReverseSort(const CString * pstr1, const CString * pstr2)
{
	ASSERT(pstr1);
	ASSERT(pstr2);
	return pstr2->Compare(*pstr1);
}

int CSortableStringArray::StrCmpLogical(const CString* pstr1, const CString* pstr2)
{
#ifdef UNICODE
	return StrCmpLogicalW((LPCWSTR)*pstr1, (LPCWSTR)*pstr2);
#else
	int ret;
	wchar_t *pwc1 = new wchar_t [ pstr1->GetLength() + 1 * sizeof(wchar_t) ];
	wchar_t *pwc2 = new wchar_t [ pstr2->GetLength() + 1 * sizeof(wchar_t) ];
	size_t s1 = mbstowcs( pwc1, *pstr1, pstr1->GetLength() + 1 );
	size_t s2 = mbstowcs( pwc2, *pstr2, pstr2->GetLength() + 1 );

	ret = StrCmpLogicalW(pwc1,pwc2);

	delete [] pwc1; 
	delete [] pwc2; 

	return ret;
#endif
}


int CSortableStringArray::LogicalSort(const CString * pstr1, const CString * pstr2)
{
	ASSERT(pstr1);
	ASSERT(pstr2);
	
	return StrCmpLogical(pstr1, pstr2);
}


int CSortableStringArray::ReverseLogicalSort(const CString * pstr1, const CString * pstr2)
{
	ASSERT(pstr1);
	ASSERT(pstr2);
	return StrCmpLogical(pstr2, pstr1);
}



void CSortableStringArray::Sort(STRINGCOMPAREFN pfnCompare /*= CSortedStringArray::Compare */)
{
	CString * prgstr = GetData();
	qsort(prgstr,GetSize(),sizeof(CString),(GENERICCOMPAREFN)pfnCompare);
}
