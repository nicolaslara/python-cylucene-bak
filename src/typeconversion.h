#ifndef _CYLUCENE_TYPECONF
#define _CYLUCENE_TYPECONF

#include <string>
#include <CLucene.h>


inline TCHAR *wstring_to_tchar(std::wstring s)
{
  /* CLucene uses TCHAR, which does not necessarily have to be the
     same type that wstring exposes. Thus, we can't simply use
     wstring's c_str function. We need to careful when using this
     function, then, as it produces a memory leak if not handled
     properly. */
  return STRDUP_WtoT(s.c_str());
}

inline std::wstring tchar_to_wstring(const TCHAR *s)
{
  return std::wstring(s);
}

#endif
