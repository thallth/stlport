/*
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999 
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted 
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef _STLP_INTERNAL_STDEXCEPT
#define _STLP_INTERNAL_STDEXCEPT

#if !defined (_STLP_USE_NATIVE_STDEXCEPT) || defined (_STLP_USE_OWN_NAMESPACE)

#  ifndef _STLP_INTERNAL_EXCEPTION
#    include <stl/_exception.h>
#  endif

#  if defined(_STLP_USE_EXCEPTIONS) || \
    !(defined(_MIPS_SIM) && defined(_ABIO32) && (_MIPS_SIM == _ABIO32))

#    ifndef _STLP_INTERNAL_CSTRING
#      include <stl/_cstring.h>
#    endif

#    ifndef _STLP_INTERNAL_ALLOC_H
#      include <stl/_alloc.h>
#    endif

#    ifndef _STLP_STRING_FWD_H
#      include <stl/_string_fwd.h>
#    endif

#    ifndef _STLP_USE_NO_IOSTREAMS
#      define _STLP_OWN_STDEXCEPT 1
#    endif

_STLP_BEGIN_NAMESPACE

/* We disable the 4275 warning for
 *  - WinCE where there are only static version of the native C++ runtime.
 *  - The MSVC compilers when the STLport user wants to make an STLport dll linked to
 *    the static C++ native runtime. In this case the std::exception base class is no more
 *    exported from native dll but is used as a base class for the exported __Named_exception
 *    class.
 */
#    if defined (_STLP_WCE_NET) || \
        defined (_STLP_USE_DYNAMIC_LIB) && defined (_STLP_USING_CROSS_NATIVE_RUNTIME_LIB)
#      define _STLP_DO_WARNING_POP
#      pragma warning (push)
#      pragma warning (disable: 4275) // Non dll interface class 'exception' used as base 
                                      // for dll-interface class '__Named_exception'
#    endif

#    if !defined (_STLP_NO_EXCEPTION_HEADER)
#      if !defined (_STLP_EXCEPTION_BASE) && !defined (_STLP_BROKEN_EXCEPTION_CLASS) && \
           defined (_STLP_USE_NAMESPACES) &&  defined (_STLP_USE_OWN_NAMESPACE)
using _STLP_VENDOR_EXCEPT_STD::exception;
#      endif
#    endif
#    define _STLP_EXCEPTION_BASE exception

class _STLP_CLASS_DECLSPEC __Named_exception : public _STLP_EXCEPTION_BASE {
public:
  __Named_exception(const string& __str) 
#    ifndef _STLP_USE_NO_IOSTREAMS
    ;
  const char* what() const _STLP_NOTHROW_INHERENTLY;
  ~__Named_exception() _STLP_NOTHROW_INHERENTLY;
#    else
  {
    strncpy(_M_name, __get_c_string(__str), _S_bufsize);
    _M_name[_S_bufsize - 1] = '\0';
  }
  const char* what() const _STLP_NOTHROW_INHERENTLY { return _M_name; }
#    endif

private:
  enum { _S_bufsize = 256 };
  char _M_name[_S_bufsize];
};

#    if defined (_STLP_DO_WARNING_POP)
#      pragma warning (pop)
#      undef _STLP_DO_WARNING_POP
#    endif

class _STLP_CLASS_DECLSPEC logic_error : public __Named_exception {
public:
  logic_error(const string& __s) : __Named_exception(__s) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~logic_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC runtime_error : public __Named_exception {
public:
  runtime_error(const string& __s) : __Named_exception(__s) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~runtime_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC domain_error : public logic_error {
public:
  domain_error(const string& __arg) : logic_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~domain_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC invalid_argument : public logic_error {
public:
  invalid_argument(const string& __arg) : logic_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~invalid_argument() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC length_error : public logic_error {
public:
  length_error(const string& __arg) : logic_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~length_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC out_of_range : public logic_error {
public:
  out_of_range(const string& __arg) : logic_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~out_of_range() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC range_error : public runtime_error {
public:
  range_error(const string& __arg) : runtime_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~range_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC overflow_error : public runtime_error {
public:
  overflow_error(const string& __arg) : runtime_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~overflow_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

class _STLP_CLASS_DECLSPEC underflow_error : public runtime_error {
public:
  underflow_error(const string& __arg) : runtime_error(__arg) {}
#    ifndef _STLP_USE_NO_IOSTREAMS
  ~underflow_error() _STLP_NOTHROW_INHERENTLY;
#    endif
};

_STLP_END_NAMESPACE

#  endif /* Not o32, and no exceptions */
#endif /* _STLP_STDEXCEPT_SEEN */

#endif /* _STLP_INTERNAL_STDEXCEPT */