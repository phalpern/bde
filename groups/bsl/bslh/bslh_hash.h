// bslh_hash.h                                                        -*-C++-*-
#ifndef INCLUDED_BSLH_HASH
#define INCLUDED_BSLH_HASH

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a 'struct' that can run any hashing algorithm on any type.
//
//@CLASSES:
//  bslh::Hash: Universal hashing functor that can apply any algorithm to types
//
//@SEE_ALSO:
//
//@DESCRIPTION: This component provides a templated 'struct', 'bslh::Hash',
// which provides hashing functionality and is a drop in replacement for
// 'bsl::hash'.  'bslh::Hash' is a wrapper that adapts the hashing algorithms
// to match the interface of 'bsl::hash'.  This component also contains
// hashAppend definitions for fundamental types, which are required to make the
// hashing algorithms in 'bslh' work. 'bslh::Hash' is a universal hashing
// functor that will hash any type that implements 'hashAppend' using the
// hashing algorithm provided as a template parameter.  For more details, see:
// https://cms.prod.bloomberg.com/team/pages/viewpage.action?title=
// Modular+Hashing&spaceKey=bde
//
///Modularity
///----------
// 'bslh::Hash' provides a modular system for hashing.  Identification of
// attributes on a type that are salient to hashing and the actual
// implementation of hashing algorithms can be decoupled. Attributes that are
// salient to hashing can be called out on a type using 'hashAppend'. Hashing
// algorithms can be written (some defaults are provided in 'bslh') to operate
// on the attributes called out by 'hashAppend'.  This prevents type creators
// from having to duplicate work or write bad hashing algorithms.
//
///'hashAppend'
///------------
// 'hashAppend' is the function that is used to pass attributes that are
// salient to hashing into a hashing algorithm. Any type being hashed must
// implement 'hashAppend'. Within this implementation, the type will call
// 'hashAppend' on each of it's attributes that are salient to hashing. If
// those attributes are themselves user defined types, they will continue
// calling 'hashAppend' on their own  attributes until fundamental types are
// reached.
//
// Within this file, 'hashAppend' has been implemented for all of the
// fundamental types. When 'hashAppend is reached on a fundamental type, the
// hashing algorithm is no longer propagated, and instead a pointer to the
// beginning of the type in memory is passed to the algorithm, along with the
// length of the type. The algorithm will then incorporate the type into its
// internal state and return a finalized hash when requested.
//
///Hashing Algorithms
///------------------
// All of the hashing algorithms in 'bslh' will implement the same interface.
// They offer an 'operator()' to pass in a pointer and a length. The algorithms
// will operate on the number of bytes specified by the length, starting at the
// location pointed to by the pointer. They will all also offer a 'computHash'
// method that will return a finalized hash.


#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLH_DEFAULTHASHALGORITHM
#include <bslh_defaulthashalgorithm.h>
#endif

#ifndef INCLUDED_BSLMF_ISBITWISEMOVEABLE
#include <bslmf_isbitwisemoveable.h>
#endif

#ifndef INCLUDED_BSLMF_ISTRIVIALLYCOPYABLE
#include <bslmf_istriviallycopyable.h>
#endif

#ifndef INCLUDED_BSLMF_ISTRIVIALLYDEFAULTCONSTRUCTIBLE
#include <bslmf_istriviallydefaultconstructible.h>
#endif

#ifndef INCLUDED_BSLS_COMPILERFEATURES
#include <bsls_compilerfeatures.h>
#endif

#ifndef INCLUDED_STDDEF_H
#include <stddef.h>  // for 'size_t'
#define INCLUDED_STDDEF_H
#endif

namespace BloombergLP {

namespace bslh {

                          // ================
                          // class bslh::Hash
                          // ================

template <class HASH_ALGORITHM = bslh::DefaultHashAlgorithm>
struct Hash {
    // This struct wraps the (template parameter) 'HASH_ALGORITHM' in an
    // interface that is a drop in replacement for 'bsl::hash'.

    // TYPES
    typedef size_t result_type;
        // Type of the hash that will be returned.

    // ACCESSORS
    template <class TYPE>
    result_type operator()(TYPE const& type) const;
        // Returns a hash generated by the parameterized 'HASH_ALGORITHM' for
        // the specified 'type'. The value returned by the 'HASH_ALGORITHM' is
        // converted to 'size_t' before returning.

};

// ACCESSORS
template <class HASH_ALGORITHM>
template <class TYPE>
typename Hash<HASH_ALGORITHM>::result_type
Hash<HASH_ALGORITHM>::operator()(TYPE const& key) const
{
    HASH_ALGORITHM hashAlg;
    hashAppend(hashAlg, key);
    return static_cast<result_type>(hashAlg.computeHash());
}

// FREE FUNCTIONS
template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, bool input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, char input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, signed char input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, unsigned char input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, wchar_t input);

#if defined BSLS_COMPILERFEATURES_SUPPORT_UNICODE_CHAR_TYPES
template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, char16_t input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, char32_t input);
#endif

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, short input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, unsigned short input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, int input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, unsigned int input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, long input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, unsigned long input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, long long input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, unsigned long long input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, float input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, double input);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, long double input);

template <class HASH_ALGORITHM, size_t N>
void hashAppend(HASH_ALGORITHM& hashAlg, const char (&input)[N]);

template <class HASH_ALGORITHM, class TYPE, size_t N>
void hashAppend(HASH_ALGORITHM& hashAlg, const TYPE (&input)[N]);

template <class HASH_ALGORITHM>
void hashAppend(HASH_ALGORITHM& hashAlg, const void *input);
    // Passes the specified 'input' to the specified 'hashAlg' to be combined
    // into the hash.

}  // close package namespace

// ============================================================================
//                  TEMPLATE AND INLINE FUNCTION DEFINITIONS
// ============================================================================

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, bool input)
{
    if (input) {
        input = 1;
    } else {
        input = 0;
    }
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, char input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, signed char input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, unsigned char input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, wchar_t input)
{
    hashAlg(&input, sizeof(input));
}

#if defined BSLS_COMPILERFEATURES_SUPPORT_UNICODE_CHAR_TYPES
template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, char16_t input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, char32_t input)
{
    hashAlg(&input, sizeof(input));
}
#endif

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, short input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, unsigned short input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, int input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, unsigned int input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, long input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, unsigned long input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, long long input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, unsigned long long input)
{
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, float input)
{
    if (input == 0){
        input = 0;
    }
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, double input)
{
    if (input == 0){
        input = 0;
    }
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, long double input)
{
    if (input == 0){
        input = 0;
    }
    hashAlg(&input, sizeof(input));
}

template <class HASH_ALGORITHM, size_t N>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, const char (&input)[N])
{
    hashAlg(&input, sizeof(char)*N);
}

template <class HASH_ALGORITHM, class TYPE, size_t N>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, const TYPE (&input)[N])
{
    for (size_t i = 0; i < N; ++i) {
        hashAppend(hashAlg, input[i]);
    }
}

template <class HASH_ALGORITHM>
inline
void bslh::hashAppend(HASH_ALGORITHM& hashAlg, const void *input)
{
    hashAlg(&input, sizeof(input));
}


// ============================================================================
//                                TYPE TRAITS
// ============================================================================

// Type traits for STL 'hash'
//: o 'bsl::hash<TYPE>' is trivially default constructible.
//: o 'bsl::hash<TYPE>' is trivially copyable.
//: o 'bsl::hash<TYPE>' is bitwise movable.

namespace bslmf {
template <class TYPE>
struct IsBitwiseMoveable<bslh::Hash<TYPE> >
    : bsl::true_type {};
}  // close package namespace


}  // close enterprise namespace

namespace bsl {
template <class TYPE>
struct is_trivially_default_constructible< ::BloombergLP::bslh::Hash<TYPE> >
: bsl::true_type
{};

template <class TYPE>
struct is_trivially_copyable< ::BloombergLP::bslh::Hash<TYPE> >
: bsl::true_type
{};
}  // close package namespace



#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Bloomberg Finance L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
