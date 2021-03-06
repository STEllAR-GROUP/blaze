//=================================================================================================
/*!
//  \file blaze/math/simd/Storeu.h
//  \brief Header file for the SIMD unaligned store functionality
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

#ifndef _BLAZE_MATH_SIMD_STOREU_H_
#define _BLAZE_MATH_SIMD_STOREU_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/simd/BasicTypes.h>
#include <blaze/system/Inline.h>
#include <blaze/system/Vectorization.h>
#include <blaze/util/Complex.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/StaticAssert.h>
#include <blaze/util/typetraits/HasSize.h>
#include <blaze/util/typetraits/IsIntegral.h>


namespace blaze {

//=================================================================================================
//
//  8-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Unaligned store of a vector of 1-byte integral values.
// \ingroup simd
//
// \param address The target address.
// \param value The 1-byte integral vector to be stored.
// \return void
//
// This function stores a vector of 1-byte integral values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,1UL> >
   storeu( T1* address, const SIMDi8<T2>& value ) noexcept
{
#if BLAZE_AVX512BW_MODE
   _mm512_storeu_si512( address, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Unaligned store of a vector of 1-byte integral complex values.
// \ingroup simd
//
// \param address The target address.
// \param value The 1-byte integral complex vector to be stored.
// \return void
//
// This function stores a vector of 1-byte integral complex values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,1UL> >
   storeu( complex<T1>* address, const SIMDci8<T2>& value ) noexcept
{
   BLAZE_STATIC_ASSERT( sizeof( complex<T1> ) == 2UL*sizeof( T1 ) );

#if BLAZE_AVX512BW_MODE
   _mm512_storeu_si512( address, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************




//=================================================================================================
//
//  16-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Unaligned store of a vector of 2-byte integral values.
// \ingroup simd
//
// \param address The target address.
// \param value The 2-byte integral vector to be stored.
// \return void
//
// This function stores a vector of 2-byte integral values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,2UL> >
   storeu( T1* address, const SIMDi16<T2>& value ) noexcept
{
#if BLAZE_AVX512BW_MODE
   _mm512_storeu_si512( address, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Unaligned store of a vector of 2-byte integral complex values.
// \ingroup simd
//
// \param address The target address.
// \param value The 2-byte integral complex vector to be stored.
// \return void
//
// This function stores a vector of 2-byte integral complex values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,2UL> >
   storeu( complex<T1>* address, const SIMDci16<T2>& value ) noexcept
{
   BLAZE_STATIC_ASSERT( sizeof( complex<T1> ) == 2UL*sizeof( T1 ) );

#if BLAZE_AVX512BW_MODE
   _mm512_storeu_si512( address, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************




//=================================================================================================
//
//  32-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Unaligned store of a vector of 4-byte integral values.
// \ingroup simd
//
// \param address The target address.
// \param value The 4-byte integral vector to be stored.
// \return void
//
// This function stores a vector of 4-byte integral values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,4UL> >
   storeu( T1* address, const SIMDi32<T2>& value ) noexcept
{
#if BLAZE_AVX512F_MODE
   _mm512_mask_storeu_epi32( address, 0xFFFF, (~value).value );
#elif BLAZE_MIC_MODE
   _mm512_packstorelo_epi32( address     , (~value).value );
   _mm512_packstorehi_epi32( address+16UL, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Unaligned store of a vector of 4-byte integral complex values.
// \ingroup simd
//
// \param address The target address.
// \param value The 4-byte integral complex vector to be stored.
// \return void
//
// This function stores a vector of 4-byte integral complex values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,4UL> >
   storeu( complex<T1>* address, const SIMDci32<T2>& value ) noexcept
{
   BLAZE_STATIC_ASSERT( sizeof( complex<T1> ) == 2UL*sizeof( T1 ) );

#if BLAZE_AVX512F_MODE
   _mm512_mask_storeu_epi32( address, 0xFFFF, (~value).value );
#elif BLAZE_MIC_MODE
   _mm512_packstorelo_epi32( address    , (~value).value );
   _mm512_packstorehi_epi32( address+8UL, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************




//=================================================================================================
//
//  64-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Unaligned store of a vector of 8-byte integral values.
// \ingroup simd
//
// \param address The target address.
// \param value The 8-byte integral vector to be stored.
// \return void
//
// This function stores a vector of 8-byte integral values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,8UL> >
   storeu( T1* address, const SIMDi64<T2>& value ) noexcept
{
#if BLAZE_AVX512F_MODE
   _mm512_mask_storeu_epi64( address, 0xFF, (~value).value );
#elif BLAZE_MIC_MODE
   _mm512_packstorelo_epi64( address    , (~value).value );
   _mm512_packstorehi_epi64( address+8UL, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Unaligned store of a vector of 8-byte integral complex values.
// \ingroup simd
//
// \param address The target address.
// \param value The 8-byte integral complex vector to be stored.
// \return void
//
// This function stores a vector of 8-byte integral complex values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
template< typename T1    // Type of the integral value
        , typename T2 >  // Type of the SIMD data type
BLAZE_ALWAYS_INLINE EnableIf_t< IsIntegral_v<T1> && HasSize_v<T1,8UL> >
   storeu( complex<T1>* address, const SIMDci64<T2>& value ) noexcept
{
   BLAZE_STATIC_ASSERT( sizeof( complex<T1> ) == 2UL*sizeof( T1 ) );

#if BLAZE_AVX512F_MODE
   _mm512_mask_storeu_epi64( address, 0xFF, (~value).value );
#elif BLAZE_MIC_MODE
   _mm512_packstorelo_epi64( address    , (~value).value );
   _mm512_packstorehi_epi64( address+4UL, (~value).value );
#elif BLAZE_AVX2_MODE
   _mm256_storeu_si256( reinterpret_cast<__m256i*>( address ), (~value).value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_si128( reinterpret_cast<__m128i*>( address ), (~value).value );
#else
   *address = (~value).value;
#endif
}
//*************************************************************************************************




//=================================================================================================
//
//  32-BIT FLOATING POINT SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Unaligned store of a vector of 'float' values.
// \ingroup simd
//
// \param address The target address.
// \param value The 'float' vector to be stored.
// \return void
//
// This function stores a vector of 'float' values. In contrast to the according \c storea()
// function, the given address is not required to be properly aligned.
*/
template< typename T >  // Type of the operand
BLAZE_ALWAYS_INLINE void storeu( float* address, const SIMDf32<T>& value ) noexcept
{
#if BLAZE_AVX512F_MODE
   _mm512_storeu_ps( address, (~value).value );
#elif BLAZE_MIC_MODE
   const SIMDfloat tmp( (~value).eval().value );
   _mm512_packstorelo_ps( address     , tmp.value );
   _mm512_packstorehi_ps( address+16UL, tmp.value );
#elif BLAZE_AVX_MODE
   _mm256_storeu_ps( address, (~value).eval().value );
#elif BLAZE_SSE_MODE
   _mm_storeu_ps( address, (~value).eval().value );
#else
   *address = (~value).eval().value;
#endif
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Unaligned store of a vector of 'complex<float>' values.
// \ingroup simd
//
// \param address The target address.
// \param value The 'complex<float>' vector to be stored.
// \return void
//
// This function stores a vector of 'complex<float>' values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
BLAZE_ALWAYS_INLINE void storeu( complex<float>* address, const SIMDcfloat& value ) noexcept
{
   BLAZE_STATIC_ASSERT( sizeof( complex<float> ) == 2UL*sizeof( float ) );

#if BLAZE_AVX512F_MODE
   _mm512_storeu_ps( address, (~value).value );
#elif BLAZE_MIC_MODE
   _mm512_packstorelo_ps( reinterpret_cast<float*>( address     ), value.value );
   _mm512_packstorehi_ps( reinterpret_cast<float*>( address+8UL ), value.value );
#elif BLAZE_AVX_MODE
   _mm256_storeu_ps( reinterpret_cast<float*>( address ), value.value );
#elif BLAZE_SSE_MODE
   _mm_storeu_ps( reinterpret_cast<float*>( address ), value.value );
#else
   *address = value.value;
#endif
}
//*************************************************************************************************




//=================================================================================================
//
//  64-BIT FLOATING POINT SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Unaligned store of a vector of 'double' values.
// \ingroup simd
//
// \param address The target address.
// \param value The 'double' vector to be stored.
// \return void
//
// This function stores a vector of 'double' values. In contrast to the according \c storea()
// function, the given address is not required to be properly aligned.
*/
template< typename T >  // Type of the operand
BLAZE_ALWAYS_INLINE void storeu( double* address, const SIMDf64<T>& value ) noexcept
{
#if BLAZE_AVX512F_MODE
   _mm512_storeu_pd( address, (~value).value );
#elif BLAZE_MIC_MODE
   const SIMDdouble tmp( (~value).eval().value );
   _mm512_packstorelo_pd( address    , tmp.value );
   _mm512_packstorehi_pd( address+8UL, tmp.value );
#elif BLAZE_AVX_MODE
   _mm256_storeu_pd( address, (~value).eval().value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_pd( address, (~value).eval().value );
#else
   *address = (~value).eval().value;
#endif
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Unaligned store of a vector of 'complex<double>' values.
// \ingroup simd
//
// \param address The target address.
// \param value The 'complex<double>' vector to be stored.
// \return void
//
// This function stores a vector of 'complex<double>' values. In contrast to the according
// \c storea() function, the given address is not required to be properly aligned.
*/
BLAZE_ALWAYS_INLINE void storeu( complex<double>* address, const SIMDcdouble& value ) noexcept
{
   BLAZE_STATIC_ASSERT( sizeof( complex<double> ) == 2UL*sizeof( double ) );

#if BLAZE_AVX512F_MODE
   _mm512_storeu_pd( address, (~value).value );
#elif BLAZE_MIC_MODE
   _mm512_packstorelo_pd( reinterpret_cast<double*>( address     ), value.value );
   _mm512_packstorehi_pd( reinterpret_cast<double*>( address+4UL ), value.value );
#elif BLAZE_AVX_MODE
   _mm256_storeu_pd( reinterpret_cast<double*>( address ), value.value );
#elif BLAZE_SSE2_MODE
   _mm_storeu_pd( reinterpret_cast<double*>( address ), value.value );
#else
   *address = value.value;
#endif
}
//*************************************************************************************************

} // namespace blaze

#endif
