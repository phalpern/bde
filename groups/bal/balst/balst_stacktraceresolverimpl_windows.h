// balst_stacktraceresolverimpl_windows.h                             -*-C++-*-
#ifndef INCLUDED_BALST_STACKTRACERESOLVERIMPL_WINDOWS
#define INCLUDED_BALST_STACKTRACERESOLVERIMPL_WINDOWS

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide resolution of symbols in stack trace for Windows objects.
//
//@CLASSES:
//   balst::StackTraceResolverImpl<Windows>: symbol resolution for Windows objs
//
//@SEE_ALSO:
//
//@AUTHOR: Oleg Semenov, Bill Chapman
//
//@DESCRIPTION: This class provides a class this able to take a vector of
// 'balst::StackTraceFrame's that have only their 'address' fields set, and
// sets as many of the other fields in the stack trace frames as possible.  Elf
// objects are used on Solaris, Linux, and HPUX platforms.
//
///Usage
///-----
// This component is an implementation detail of 'baesu' and is *not* intended
// for direct client use.  It is subject to change without notice.  As such, a
// usage example is not provided.

#ifndef INCLUDED_BALSCM_VERSION
#include <balscm_version.h>
#endif

#ifndef INCLUDED_BALST_OBJECTFILEFORMAT
#include <balst_objectfileformat.h>
#endif

#ifndef INCLUDED_BALST_STACKTRACE
#include <balst_stacktrace.h>
#endif

#ifndef INCLUDED_BALST_STACKTRACEFRAME
#include <balst_stacktraceframe.h>
#endif

#ifndef INCLUDED_BSL_VECTOR
#include <bsl_vector.h>
#endif

#ifndef INCLUDED_BSLMA_ALLOCATOR
#include <bslma_allocator.h>
#endif

namespace BloombergLP {

#if defined(BALST_OBJECTFILEFORMAT_RESOLVER_WINDOWS)


namespace balst {

template <typename RESOLVER_POLICY>
class StackTraceResolverImpl;

    // ===================================================================
    // class StackTraceResolverImpl<ObjectFileFormat::Windows>
    // ===================================================================

template <>
class StackTraceResolverImpl<ObjectFileFormat::Windows> {
    // This type is for resolving symbols in Windows executables.  Resolving
    // symbols on Windows is straightforward using the dbghelp.dll library,
    // described at
    // http://msdn.microsoft.com/en-us/library/ms681412(v=VS.85).aspx

  private:
    // NOT IMPLEMENTED
    StackTraceResolverImpl();
    StackTraceResolverImpl(const StackTraceResolverImpl&);
    StackTraceResolverImpl& operator=(
                                          const StackTraceResolverImpl&);
    ~StackTraceResolverImpl();

  public:
    // CLASS METHODS
    static int resolve(StackTrace *stackTrace,
                       bool              demangle);
        // Given a specified stack trace object 'stackTrace' of stack trace
        // frames with only their 'address' fields valid, set as many other
        // fields of the frames as possible.  The 'demangle' argument is
        // ignored, demangling always happens on Windows.  Return 0 if
        // successful and a non-zero value otherwise.

    static inline
    int testFunc();
        // For testing only.  Do some random garbage and return a line number
        // from within the routine.
};

                         // ----------------------------------
                         // class StackTraceResolverImpl
                         // ----------------------------------

inline
int StackTraceResolverImpl<ObjectFileFormat::Windows>::testFunc()
{
    // Do some random garbage to generate some code, then return a line number
    // within this routine

    int line = 0, lineCopy = 0;

    for (int i = 0; true; ++i) {
        BSLS_ASSERT_OPT(line == lineCopy);

        const int loopGuard = 0x8edf0000;    // garbage with a lot of trailing
                                             // 0's.
        const int mask      = 0xa72c3dca;    // pure garbage

        enum { LINE = __LINE__ };

        for (int i = 0; !(i & loopGuard); ++i) {
            line ^= (i & mask);
        }

        // The above loop will leave the value of 'line' unchanged.  See
        // 'foilOptimizer' in the test driver.

        BSLS_ASSERT_OPT(line == lineCopy);

        if (line != 0) {
            break;
        }

        line = LINE;
        lineCopy = line;
    }

    return line;
}

}  // close package namespace

#endif

}  // close namespace BloombergLP

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2010
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
