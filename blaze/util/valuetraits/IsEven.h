//=================================================================================================
/*!
//  \file blaze/util/valuetraits/IsEven.h
//  \brief Header file for the IsEven value trait
//
//  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_UTIL_VALUETRAITS_ISEVEN_H_
#define _BLAZE_UTIL_VALUETRAITS_ISEVEN_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/util/IntegralConstant.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Compile time check whether a compile time constant expression is even.
// \ingroup value_traits
//
// This value trait tests whether the given integral value \a N is an even value. In case the
// value is even, the \a value member enumeration is set to \a true, the nested type definition
// \a Type is \a TrueType, and the class derives from \a TrueType. Otherwise \a value is set
// to \a false, \a Type is \a FalseType, and the class derives from \a FalseType.

   \code
   blaze::IsEven<2>::value   // Evaluates to 1
   blaze::IsEven<4>::Type    // Results in TrueType
   blaze::IsEven<6>          // Is derived from TrueType
   blaze::IsEven<1>::value   // Evaluates to 0
   blaze::IsEven<3>::Type    // Results in FalseType
   blaze::IsEven<5>          // Is derived from FalseType
   \endcode
*/
template< size_t N >
struct IsEven
   : public BoolConstant< N % 2UL == 0UL >
{};
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Auxiliary variable template for the IsEven value trait.
// \ingroup value_traits
//
// The IsEven_v variable template provides a convenient shortcut to access the nested \a value
// of the IsEven class template. For instance, given the compile time constant value \a N the
// following two statements are identical:

   \code
   constexpr bool value1 = IsEven<N>::value;
   constexpr bool value2 = IsEven_v<N>;
   \endcode
*/
template< size_t N >
constexpr bool IsEven_v = IsEven<N>::value;
//*************************************************************************************************

} // namespace blaze

#endif
