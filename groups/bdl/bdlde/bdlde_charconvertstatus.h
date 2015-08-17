// bdlde_charconvertstatus.h                                          -*-C++-*-
#ifndef INCLUDED_BDLDE_CHARCONVERTSTATUS
#define INCLUDED_BDLDE_CHARCONVERTSTATUS

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Enumerate masks for interpreting status from charconvert functions.
//
//@CLASSES:
//  bdlde::CharConvertStatus: namespace for bit-wise mask of charconvert status
//
//@SEE_ALSO: bdlde_charconvertutf16, bdlde_charconvertucs2
//
//@AUTHOR: Bill Chapman (bchapman2)
//
//@DESCRIPTION: This component provides a namespace for the 'enum' type
// 'bdlde::CharConvertStatus::Enum', which enumerates the set of bit-wise masks
// that can be used to interpret return values from translation functions in
// components 'bdlde_charconvertutf16' and 'bdlde_charconvertucs2'.
//
///Enumerators
///-----------
//..
//  Name                      Description
//  -----------------------   ----------------------------------------
//  BDEDE_INVALID_CHARS_BIT   Invalid characters or sequences of characters
//                            were encountered in the input.
//  BDEDE_OUT_OF_SPACE_BIT    The space provided for the output was
//                            insufficient for the translation.
//..
//
///Usage
///-----
// TBD

#ifndef INCLUDED_BDLSCM_VERSION
#include <bdlscm_version.h>
#endif

#ifndef INCLUDED_BSL_IOSFWD
#include <bsl_iosfwd.h>
#endif

namespace BloombergLP {

namespace bdlde {
                     // ==============================
                     // struct CharConvertStatus
                     // ==============================

struct CharConvertStatus {
    // This 'struct' provides a namespace for enumerating the set of mask
    // codes that can be used to interpret 'int' return values from translation
    // functions in 'bdede'.
    //
    // This class:
    //: o supports a complete set of *enumeration* operations
    //:   o except for 'bdex' serialization
    //: o is 'const' *thread-safe*
    // For terminology see 'bsldoc_glossary'.

  public:
    // TYPES
    enum Enum {
        k_INVALID_CHARS_BIT = 0x1,      // Invalid characters or sequences
                                            // of characters were encountered
                                            // in the input.
        k_OUT_OF_SPACE_BIT  = 0x2       // The space provided for the
                                            // output was insufficient for the
                                            // translation.
#ifndef BDE_OMIT_INTERNAL_DEPRECATED
      , BDEDE_INVALID_CHARS_BIT = k_INVALID_CHARS_BIT
      , BDEDE_OUT_OF_SPACE_BIT = k_OUT_OF_SPACE_BIT
#endif  // BDE_OMIT_INTERNAL_DEPRECATED
    };

  public:
    // CLASS METHODS
    static bsl::ostream& print(
                            bsl::ostream&                 stream,
                            CharConvertStatus::Enum value,
                            int                           level          = 0,
                            int                           spacesPerLevel = 4);
        // Write the string representation of the specified enumeration 'value'
        // to the specified output 'stream', and return a reference to
        // 'stream'.  Optionally specify an initial indentation 'level', whose
        // absolute value is incremented recursively for nested objects.  If
        // 'level' is specified, optionally specify 'spacesPerLevel', whose
        // absolute value indicates the number of spaces per indentation level
        // for this and all of its nested objects.  If 'level' is negative,
        // suppress indentation of the first line.  If 'spacesPerLevel' is
        // negative, format the entire output on one line, suppressing all but
        // the initial indentation (as governed by 'level').  See 'toAscii' for
        // what constitutes the string representation of a
        // 'CharConvertStatus::Enum' value.

    static const char *toAscii(CharConvertStatus::Enum value);
        // Return the non-modifiable string representation corresponding to the
        // specified enumeration 'value', if it exists, and a unique (error)
        // string otherwise.  The string representation of 'value' matches its
        // corresponding enumerator name with the "BDEDE_" prefix elided.  For
        // example:
        //..
        //  bsl::cout << CharConvertStatus::toAscii(
        //                    CharConvertStatus::BDEDE_OUT_OF_SPACE_BIT);
        //..
        // will print the following on standard output:
        //..
        //  OUT_OF_SPACE_BIT
        //..
        // Note that specifying a 'value' that does not match any of the
        // enumerators will result in a string representation that is distinct
        // from any of those corresponding to the enumerators, but is otherwise
        // unspecified.
};

// FREE OPERATORS
bsl::ostream& operator<<(bsl::ostream&                 stream,
                         CharConvertStatus::Enum value);
}  // close package namespace
    // Write the string representation of the specified enumeration 'value' to
    // the specified output 'stream' in a single-line format, and return a
    // reference to 'stream'.  See 'toAscii' for what constitutes the string
    // representation of a 'bdlde::CharConvertStatus::Enum' value.  Note that
    // this method has the same behavior as
    //..
    //  bdlde::CharConvertStatus::print(stream, value, 0, -1);
    //..

// ============================================================================
//                      INLINE FUNCTION DEFINITIONS
// ============================================================================

                     // ------------------------------
                     // struct bdlde::CharConvertStatus
                     // ------------------------------

// FREE OPERATORS
inline
bsl::ostream& bdlde::operator<<(bsl::ostream&                 stream,
                         CharConvertStatus::Enum value)
{
    return CharConvertStatus::print(stream, value, 0, -1);
}

}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
