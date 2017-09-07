//=================================================================================================
/*!
//  \file blaze/math/views/row/Dense.h
//  \brief RowImpl specialization for dense matrices
//
//  Copyright (C) 2012-2017 Klaus Iglberger - All Rights Reserved
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

#ifndef _BLAZE_MATH_VIEWS_ROW_DENSE_H_
#define _BLAZE_MATH_VIEWS_ROW_DENSE_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <algorithm>
#include <iterator>
#include <blaze/math/Aliases.h>
#include <blaze/math/constraints/ColumnMajorMatrix.h>
#include <blaze/math/constraints/Computation.h>
#include <blaze/math/constraints/DenseMatrix.h>
#include <blaze/math/constraints/DenseVector.h>
#include <blaze/math/constraints/RequiresEvaluation.h>
#include <blaze/math/constraints/RowMajorMatrix.h>
#include <blaze/math/constraints/RowVector.h>
#include <blaze/math/constraints/Symmetric.h>
#include <blaze/math/constraints/TransExpr.h>
#include <blaze/math/constraints/UniTriangular.h>
#include <blaze/math/Exception.h>
#include <blaze/math/expressions/DenseVector.h>
#include <blaze/math/expressions/View.h>
#include <blaze/math/InitializerList.h>
#include <blaze/math/shims/Clear.h>
#include <blaze/math/shims/IsDefault.h>
#include <blaze/math/SIMD.h>
#include <blaze/math/traits/CrossTrait.h>
#include <blaze/math/traits/RowTrait.h>
#include <blaze/math/typetraits/HasSIMDAdd.h>
#include <blaze/math/typetraits/HasSIMDDiv.h>
#include <blaze/math/typetraits/HasSIMDMult.h>
#include <blaze/math/typetraits/HasSIMDSub.h>
#include <blaze/math/typetraits/IsExpression.h>
#include <blaze/math/typetraits/IsLower.h>
#include <blaze/math/typetraits/IsPadded.h>
#include <blaze/math/typetraits/IsRestricted.h>
#include <blaze/math/typetraits/IsSIMDCombinable.h>
#include <blaze/math/typetraits/IsSparseVector.h>
#include <blaze/math/typetraits/IsStrictlyLower.h>
#include <blaze/math/typetraits/IsStrictlyUpper.h>
#include <blaze/math/typetraits/IsUniLower.h>
#include <blaze/math/typetraits/IsUniUpper.h>
#include <blaze/math/typetraits/IsUpper.h>
#include <blaze/math/views/row/BaseTemplate.h>
#include <blaze/math/views/row/RowData.h>
#include <blaze/system/CacheSize.h>
#include <blaze/system/Inline.h>
#include <blaze/system/Optimizations.h>
#include <blaze/system/Thresholds.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Pointer.h>
#include <blaze/util/constraints/Reference.h>
#include <blaze/util/constraints/Vectorizable.h>
#include <blaze/util/DisableIf.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/mpl/If.h>
#include <blaze/util/mpl/Not.h>
#include <blaze/util/mpl/Or.h>
#include <blaze/util/Template.h>
#include <blaze/util/Types.h>
#include <blaze/util/typetraits/IsConst.h>
#include <blaze/util/typetraits/IsNumeric.h>
#include <blaze/util/typetraits/IsReference.h>
#include <blaze/util/typetraits/RemoveReference.h>


namespace blaze {

//=================================================================================================
//
//  CLASS TEMPLATE SPECIALIZATION FOR ROW-MAJOR DENSE MATRICES
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of RowImpl for rows on row-major dense matrices.
// \ingroup row
//
// This specialization of RowImpl adapts the class template to the requirements of row-major
// dense matrices.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
class RowImpl<MT,true,true,SF,RAs...>
   : public View< DenseVector< RowImpl<MT,true,true,SF,RAs...>, true > >
   , private RowData<MT,RAs...>
{
 private:
   //**Type definitions****************************************************************************
   using DataType = RowData<MT,RAs...>;  //!< The type of the RowData base class.
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   //! Type of this RowImpl instance.
   using This = RowImpl<MT,true,true,SF,RAs...>;

   using BaseType      = DenseVector<This,true>;      //!< Base type of this RowImpl instance.
   using ResultType    = RowTrait_<MT>;               //!< Result type for expression template evaluations.
   using TransposeType = TransposeType_<ResultType>;  //!< Transpose type for expression template evaluations.
   using ElementType   = ElementType_<MT>;            //!< Type of the row elements.
   using SIMDType      = SIMDTrait_<ElementType>;     //!< SIMD type of the row elements.
   using ReturnType    = ReturnType_<MT>;             //!< Return type for expression template evaluations
   using CompositeType = const RowImpl&;              //!< Data type for composite expression templates.

   //! Reference to a constant row value.
   using ConstReference = ConstReference_<MT>;

   //! Reference to a non-constant row value.
   using Reference = If_< IsConst<MT>, ConstReference, Reference_<MT> >;

   //! Pointer to a constant row value.
   using ConstPointer = const ElementType*;

   //! Pointer to a non-constant row value.
   using Pointer = If_< Or< IsConst<MT>, Not< HasMutableDataAccess<MT> > >, ConstPointer, ElementType* >;

   //! Iterator over constant elements.
   using ConstIterator = ConstIterator_<MT>;

   //! Iterator over non-constant elements.
   using Iterator = If_< IsConst<MT>, ConstIterator, Iterator_<MT> >;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum : bool { simdEnabled = MT::simdEnabled };

   //! Compilation switch for the expression template assignment strategy.
   enum : bool { smpAssignable = MT::smpAssignable };
   //**********************************************************************************************

   //**Constructors********************************************************************************
   /*!\name Constructors */
   //@{
   explicit inline RowImpl( MT& matrix );
   explicit inline RowImpl( MT& matrix, size_t index );
   // No explicitly declared copy constructor.
   //@}
   //**********************************************************************************************

   //**Destructor**********************************************************************************
   // No explicitly declared destructor.
   //**********************************************************************************************

   //**Data access functions***********************************************************************
   /*!\name Data access functions */
   //@{
   inline Reference      operator[]( size_t index );
   inline ConstReference operator[]( size_t index ) const;
   inline Reference      at( size_t index );
   inline ConstReference at( size_t index ) const;
   inline Pointer        data  () noexcept;
   inline ConstPointer   data  () const noexcept;
   inline Iterator       begin ();
   inline ConstIterator  begin () const;
   inline ConstIterator  cbegin() const;
   inline Iterator       end   ();
   inline ConstIterator  end   () const;
   inline ConstIterator  cend  () const;
   //@}
   //**********************************************************************************************

   //**Assignment operators************************************************************************
   /*!\name Assignment operators */
   //@{
   inline RowImpl& operator=( const ElementType& rhs );
   inline RowImpl& operator=( initializer_list<ElementType> list );
   inline RowImpl& operator=( const RowImpl& rhs );

   template< typename VT > inline RowImpl& operator= ( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator+=( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator-=( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator*=( const DenseVector<VT,true>&  rhs );
   template< typename VT > inline RowImpl& operator*=( const SparseVector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator/=( const DenseVector<VT,true>&  rhs );
   template< typename VT > inline RowImpl& operator%=( const Vector<VT,true>& rhs );

   template< typename Other >
   inline EnableIf_< IsNumeric<Other>, RowImpl >& operator*=( Other rhs );

   template< typename Other >
   inline EnableIf_< IsNumeric<Other>, RowImpl >& operator/=( Other rhs );
   //@}
   //**********************************************************************************************

   //**Utility functions***************************************************************************
   /*!\name Utility functions */
   //@{
   using DataType::operand;
   using DataType::row;

   inline size_t size() const noexcept;
   inline size_t spacing() const noexcept;
   inline size_t capacity() const noexcept;
   inline size_t nonZeros() const;
   inline void   reset();
   //@}
   //**********************************************************************************************

   //**Numeric functions***************************************************************************
   /*!\name Numeric functions */
   //@{
   template< typename Other > inline RowImpl& scale( const Other& scalar );
   //@}
   //**********************************************************************************************

 private:
   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedAddAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDAdd< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedSubAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDSub< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedMultAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDMult< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedDivAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDDiv< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**SIMD properties*****************************************************************************
   //! The number of elements packed within a single SIMD element.
   enum : size_t { SIMDSIZE = SIMDTrait<ElementType>::size };
   //**********************************************************************************************

 public:
   //**Expression template evaluation functions****************************************************
   /*!\name Expression template evaluation functions */
   //@{
   template< typename Other >
   inline bool canAlias( const Other* alias ) const noexcept;

   template< typename MT2, bool SO2, bool SF2, size_t... RAs2 >
   inline bool canAlias( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept;

   template< typename Other >
   inline bool isAliased( const Other* alias ) const noexcept;

   template< typename MT2, bool SO2, bool SF2, size_t... RAs2 >
   inline bool isAliased( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept;

   inline bool isAligned   () const noexcept;
   inline bool canSMPAssign() const noexcept;

   BLAZE_ALWAYS_INLINE SIMDType load ( size_t index ) const noexcept;
   BLAZE_ALWAYS_INLINE SIMDType loada( size_t index ) const noexcept;
   BLAZE_ALWAYS_INLINE SIMDType loadu( size_t index ) const noexcept;

   BLAZE_ALWAYS_INLINE void store ( size_t index, const SIMDType& value ) noexcept;
   BLAZE_ALWAYS_INLINE void storea( size_t index, const SIMDType& value ) noexcept;
   BLAZE_ALWAYS_INLINE void storeu( size_t index, const SIMDType& value ) noexcept;
   BLAZE_ALWAYS_INLINE void stream( size_t index, const SIMDType& value ) noexcept;

   template< typename VT >
   inline DisableIf_< VectorizedAssign<VT> > assign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedAssign<VT> > assign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void assign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedAddAssign<VT> > addAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedAddAssign<VT> > addAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void addAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedSubAssign<VT> > subAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedSubAssign<VT> > subAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void subAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedMultAssign<VT> > multAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedMultAssign<VT> > multAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void multAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedDivAssign<VT> > divAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedDivAssign<VT> > divAssign( const DenseVector<VT,true>& rhs );
   //@}
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   using DataType::matrix_;
   //**********************************************************************************************

   //**Friend declarations*************************************************************************
   template< typename MT2, bool SO2, bool DF2, bool SF2, size_t... RAs2 > friend class RowImpl;
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE    ( MT );
   BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_COMPUTATION_TYPE ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_TRANSEXPR_TYPE   ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_POINTER_TYPE     ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_REFERENCE_TYPE   ( MT );
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  CONSTRUCTOR
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for rows with a compile time index.
//
// \param matrix The matrix containing the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,true,true,SF,RAs...>::RowImpl( MT& matrix )
   : DataType( matrix )  // Base class initialization
{}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for rows with a runtime index.
//
// \param matrix The matrix containing the row.
// \param index The index of the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,true,true,SF,RAs...>::RowImpl( MT& matrix, size_t index )
   : DataType( matrix, index )  // Base class initialization
{}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DATA ACCESS FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access index.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::Reference
   RowImpl<MT,true,true,SF,RAs...>::operator[]( size_t index )
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(row(),index);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access index.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstReference
   RowImpl<MT,true,true,SF,RAs...>::operator[]( size_t index ) const
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return const_cast<const MT&>( matrix_ )(row(),index);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checked access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid row access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access index.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::Reference
   RowImpl<MT,true,true,SF,RAs...>::at( size_t index )
{
   if( index >= size() ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   return (*this)[index];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checked access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid row access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access index.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstReference
   RowImpl<MT,true,true,SF,RAs...>::at( size_t index ) const
{
   if( index >= size() ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   return (*this)[index];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
//
// This function returns a pointer to the internal storage of the dense row. Note that in case
// of a column-major matrix you can NOT assume that the row elements lie adjacent to each other!
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::Pointer
   RowImpl<MT,true,true,SF,RAs...>::data() noexcept
{
   return matrix_.data( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
//
// This function returns a pointer to the internal storage of the dense row. Note that in case
// of a column-major matrix you can NOT assume that the row elements lie adjacent to each other!
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstPointer
   RowImpl<MT,true,true,SF,RAs...>::data() const noexcept
{
   return matrix_.data( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::Iterator
   RowImpl<MT,true,true,SF,RAs...>::begin()
{
   return matrix_.begin( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstIterator
   RowImpl<MT,true,true,SF,RAs...>::begin() const
{
   return matrix_.cbegin( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstIterator
   RowImpl<MT,true,true,SF,RAs...>::cbegin() const
{
   return matrix_.cbegin( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::Iterator
   RowImpl<MT,true,true,SF,RAs...>::end()
{
   return matrix_.end( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstIterator
   RowImpl<MT,true,true,SF,RAs...>::end() const
{
   return matrix_.cend( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,true,true,SF,RAs...>::ConstIterator
   RowImpl<MT,true,true,SF,RAs...>::cend() const
{
   return matrix_.cend( row() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ASSIGNMENT OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Homogenous assignment to all row elements.
//
// \param rhs Scalar value to be assigned to all row elements.
// \return Reference to the assigned row.
//
// This function homogeneously assigns the given value to all elements of the row. Note that in
// case the underlying dense matrix is a lower/upper matrix only lower/upper and diagonal elements
// of the underlying matrix are modified.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator=( const ElementType& rhs )
{
   const size_t jbegin( ( IsUpper<MT>::value )
                        ?( ( IsUniUpper<MT>::value || IsStrictlyUpper<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t jend  ( ( IsLower<MT>::value )
                        ?( ( IsUniLower<MT>::value || IsStrictlyLower<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t j=jbegin; j<jend; ++j )
      matrix_(row(),j) = rhs;

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief List assignment to all row elements.
//
// \param list The initializer list.
// \exception std::invalid_argument Invalid assignment to row.
//
// This assignment operator offers the option to directly assign to all elements of the dense
// row by means of an initializer list. The row elements are assigned the values from the given
// initializer list. Missing values are reset to their default state. Note that in case the size
// of the initializer list exceeds the size of the row, a \a std::invalid_argument exception is
// thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator=( initializer_list<ElementType> list )
{
   if( list.size() > size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to row" );
   }

   std::fill( std::copy( list.begin(), list.end(), begin() ), end(), ElementType() );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Copy assignment operator for Row.
//
// \param rhs Dense row to be copied.
// \return Reference to the assigned row.
// \exception std::invalid_argument Row sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two rows don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator=( const RowImpl& rhs )
{
   if( &rhs == this ) return *this;

   if( size() != rhs.size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Row sizes do not match" );
   }

   if( !tryAssign( matrix_, rhs, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsExpression<MT>::value && rhs.canAlias( &matrix_ ) ) {
      const ResultType tmp( rhs );
      smpAssign( left, tmp );
   }
   else {
      smpAssign( left, rhs );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Assignment operator for different vectors.
//
// \param rhs Vector to be assigned.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpAssign( left, tmp );
   }
   else {
      if( IsSparseVector<VT>::value )
         reset();
      smpAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition assignment operator for the addition of a vector (\f$ \vec{a}+=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be added to the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator+=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryAddAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpAddAssign( left, tmp );
   }
   else {
      smpAddAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction assignment operator for the subtraction of a vector (\f$ \vec{a}-=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be subtracted from the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator-=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !trySubAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpSubAssign( left, tmp );
   }
   else {
      smpSubAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a dense vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side dense vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator*=( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryMultAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpMultAssign( left, tmp );
   }
   else {
      smpMultAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a sparse vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side sparse vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator*=( const SparseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( ResultType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   const ResultType right( *this * (~rhs) );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   smpAssign( left, right );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a dense vector (\f$ \vec{a}/=\vec{b} \f$).
//
// \param rhs The right-hand side dense vector divisor.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator/=( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryDivAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpDivAssign( left, tmp );
   }
   else {
      smpDivAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Cross product assignment operator for the multiplication of a vector
//        (\f$ \vec{a}\times=\vec{b} \f$).
//
// \param rhs The right-hand side vector for the cross product.
// \return Reference to the assigned row.
// \exception std::invalid_argument Invalid vector size for cross product.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current size of any of the two vectors is not equal to 3, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::operator%=( const Vector<VT,true>& rhs )
{
   using blaze::assign;

   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   using CrossType = CrossTrait_< ResultType, ResultType_<VT> >;

   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( CrossType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( CrossType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( CrossType );

   if( size() != 3UL || (~rhs).size() != 3UL ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid vector size for cross product" );
   }

   const CrossType right( *this % (~rhs) );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   assign( left, right );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication between a dense row and
//        a scalar value (\f$ \vec{a}*=s \f$).
//
// \param rhs The right-hand side scalar value for the multiplication.
// \return Reference to the vector.
//
// This operator cannot be used for rows on lower or upper unitriangular matrices. The attempt
// to scale such a row results in a compilation error!
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<Other>, RowImpl<MT,true,true,SF,RAs...> >&
   RowImpl<MT,true,true,SF,RAs...>::operator*=( Other rhs )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   return operator=( (*this) * rhs );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a dense row by a scalar value
//        (\f$ \vec{a}/=s \f$).
//
// \param rhs The right-hand side scalar value for the division.
// \return Reference to the vector.
//
// This operator cannot be used for rows on lower or upper unitriangular matrices. The attempt
// to scale such a row results in a compilation error!
//
// \note A division by zero is only checked by an user assert.
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<Other>, RowImpl<MT,true,true,SF,RAs...> >&
   RowImpl<MT,true,true,SF,RAs...>::operator/=( Other rhs )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   BLAZE_USER_ASSERT( rhs != Other(0), "Division by zero detected" );

   return operator=( (*this) / rhs );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the current size/dimension of the row.
//
// \return The size of the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,true,true,SF,RAs...>::size() const noexcept
{
   return matrix_.columns();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the minimum capacity of the row.
//
// \return The minimum capacity of the row.
//
// This function returns the minimum capacity of the row, which corresponds to the current size
// plus padding.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,true,true,SF,RAs...>::spacing() const noexcept
{
   return matrix_.spacing();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the maximum capacity of the dense row.
//
// \return The maximum capacity of the dense row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,true,true,SF,RAs...>::capacity() const noexcept
{
   return matrix_.capacity( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the number of non-zero elements in the row.
//
// \return The number of non-zero elements in the row.
//
// Note that the number of non-zero elements is always less than or equal to the current number
// of columns of the matrix containing the row.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,true,true,SF,RAs...>::nonZeros() const
{
   return matrix_.nonZeros( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Reset to the default initial values.
//
// \return void
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline void RowImpl<MT,true,true,SF,RAs...>::reset()
{
   matrix_.reset( row() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  NUMERIC FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Scaling of the row by the scalar value \a scalar (\f$ \vec{a}=\vec{b}*s \f$).
//
// \param scalar The scalar value for the row scaling.
// \return Reference to the dense row.
//
// This function scales the row by applying the given scalar value \a scalar to each element
// of the row. For built-in and \c complex data types it has the same effect as using the
// multiplication assignment operator. Note that the function cannot be used to scale a row
// on a lower or upper unitriangular matrix. The attempt to scale such a row results in a
// compile time error!
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the scalar value
inline RowImpl<MT,true,true,SF,RAs...>&
   RowImpl<MT,true,true,SF,RAs...>::scale( const Other& scalar )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   const size_t jbegin( ( IsUpper<MT>::value )
                        ?( ( IsStrictlyUpper<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t jend  ( ( IsLower<MT>::value )
                        ?( ( IsStrictlyLower<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t j=jbegin; j<jend; ++j ) {
      matrix_(row(),j) *= scalar;
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TEMPLATE EVALUATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the foreign expression
inline bool RowImpl<MT,true,true,SF,RAs...>::canAlias( const Other* alias ) const noexcept
{
   return matrix_.isAliased( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given dense row \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename MT2      // Data type of the foreign dense row
        , bool SO2          // Storage order of the foreign dense row
        , bool SF2          // Symmetry flag of the foreign dense row
        , size_t... RAs2 >  // Compile time row arguments of the foreign dense row
inline bool
   RowImpl<MT,true,true,SF,RAs...>::canAlias( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept
{
   return matrix_.isAliased( &alias->matrix_ ) && ( row() == alias->row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the foreign expression
inline bool RowImpl<MT,true,true,SF,RAs...>::isAliased( const Other* alias ) const noexcept
{
   return matrix_.isAliased( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given dense row \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , bool SF           // Symmetry flag
        , size_t... RAs >   // Compile time row arguments
template< typename MT2      // Data type of the foreign dense row
        , bool SO2          // Storage order of the foreign dense row
        , bool SF2          // Symmetry flag of the foreign dense row
        , size_t... RAs2 >  // Compile time row arguments of the foreign dense row
inline bool
   RowImpl<MT,true,true,SF,RAs...>::isAliased( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept
{
   return matrix_.isAliased( &alias->matrix_ ) && ( row() == alias->row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is properly aligned in memory.
//
// \return \a true in case the dense row is aligned, \a false if not.
//
// This function returns whether the dense row is guaranteed to be properly aligned in memory,
// i.e. whether the beginning and the end of the dense row are guaranteed to conform to the
// alignment restrictions of the element type \a Type.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline bool RowImpl<MT,true,true,SF,RAs...>::isAligned() const noexcept
{
   return matrix_.isAligned();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can be used in SMP assignments.
//
// \return \a true in case the dense row can be used in SMP assignments, \a false if not.
//
// This function returns whether the dense row can be used in SMP assignments. In contrast to
// the \a smpAssignable member enumeration, which is based solely on compile time information,
// this function additionally provides runtime information (as for instance the current size
// of the dense row).
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline bool RowImpl<MT,true,true,SF,RAs...>::canSMPAssign() const noexcept
{
   return ( size() > SMP_DVECASSIGN_THRESHOLD );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Load of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return The loaded SIMD element.
//
// This function performs a load of a specific SIMD element of the dense row. The index
// must be smaller than the number of matrix columns. This function must \b NOT be called
// explicitly! It is used internally for the performance optimized evaluation of expression
// templates. Calling this function explicitly might result in erroneous results and/or in
// compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE typename RowImpl<MT,true,true,SF,RAs...>::SIMDType
   RowImpl<MT,true,true,SF,RAs...>::load( size_t index ) const noexcept
{
   return matrix_.load( row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Aligned load of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return The loaded SIMD element.
//
// This function performs an aligned load of a specific SIMD element of the dense row.
// The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE typename RowImpl<MT,true,true,SF,RAs...>::SIMDType
   RowImpl<MT,true,true,SF,RAs...>::loada( size_t index ) const noexcept
{
   return matrix_.loada( row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Unaligned load of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return The loaded SIMD element.
//
// This function performs an unaligned load of a specific SIMD element of the dense row.
// The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE typename RowImpl<MT,true,true,SF,RAs...>::SIMDType
   RowImpl<MT,true,true,SF,RAs...>::loadu( size_t index ) const noexcept
{
   return matrix_.loadu( row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs a store a specific SIMD element of the dense row. The index
// must be smaller than the number of matrix columns. This function must \b NOT be called
// explicitly! It is used internally for the performance optimized evaluation of expression
// templates. Calling this function explicitly might result in erroneous results and/or in
// compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,true,true,SF,RAs...>::store( size_t index, const SIMDType& value ) noexcept
{
   matrix_.store( row(), index, value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Aligned store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs an aligned store a specific SIMD element of the dense row. The
// index must be smaller than the number of matrix columns. This function must \b NOT be
// called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,true,true,SF,RAs...>::storea( size_t index, const SIMDType& value ) noexcept
{
   matrix_.storea( row(), index, value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Unligned store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs an unaligned store a specific SIMD element of the dense row.
// The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,true,true,SF,RAs...>::storeu( size_t index, const SIMDType& value ) noexcept
{
   matrix_.storeu( row(), index, value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Aligned, non-temporal store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs an aligned, non-temporal store a specific SIMD element of the dense
// row. The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,true,true,SF,RAs...>::stream( size_t index, const SIMDType& value ) noexcept
{
   matrix_.stream( row(), index, value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) = (~rhs)[j    ];
      matrix_(row(),j+1UL) = (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) = (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t columns( size() );

   const size_t jpos( ( remainder )?( columns & size_t(-SIMDSIZE) ):( columns ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( columns - ( columns % (SIMDSIZE) ) ) == jpos, "Invalid end calculation" );

   size_t j( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   if( useStreaming && columns > ( cacheSize/( sizeof(ElementType) * 3UL ) ) && !(~rhs).isAliased( &matrix_ ) )
   {
      for( ; j<jpos; j+=SIMDSIZE ) {
         left.stream( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      }
      for( ; remainder && j<columns; ++j ) {
         *left = *right; ++left; ++right;
      }
   }
   else
   {
      for( ; (j+SIMDSIZE*3UL) < jpos; j+=SIMDSIZE*4UL ) {
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      }
      for( ; j<jpos; j+=SIMDSIZE ) {
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      }
      for( ; remainder && j<columns; ++j ) {
         *left = *right; ++left; ++right;
      }
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,true,true,SF,RAs...>::assign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) = element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedAddAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) += (~rhs)[j    ];
      matrix_(row(),j+1UL) += (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) += (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedAddAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t columns( size() );

   const size_t jpos( ( remainder )?( columns & size_t(-SIMDSIZE) ):( columns ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( columns - ( columns % (SIMDSIZE) ) ) == jpos, "Invalid end calculation" );

   size_t j( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (j+SIMDSIZE*3UL) < jpos; j+=SIMDSIZE*4UL ) {
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; j<jpos; j+=SIMDSIZE ) {
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; remainder && j<columns; ++j ) {
      *left += *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,true,true,SF,RAs...>::addAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) += element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedSubAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) -= (~rhs)[j    ];
      matrix_(row(),j+1UL) -= (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) -= (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedSubAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t columns( size() );

   const size_t jpos( ( remainder )?( columns & size_t(-SIMDSIZE) ):( columns ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( columns - ( columns % (SIMDSIZE) ) ) == jpos, "Invalid end calculation" );

   size_t j( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (j+SIMDSIZE*3UL) < jpos; j+=SIMDSIZE*4UL ) {
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; j<jpos; j+=SIMDSIZE ) {
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; remainder && j<columns; ++j ) {
      *left -= *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,true,true,SF,RAs...>::subAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) -= element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedMultAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) *= (~rhs)[j    ];
      matrix_(row(),j+1UL) *= (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) *= (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedMultAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t columns( size() );

   const size_t jpos( ( remainder )?( columns & size_t(-SIMDSIZE) ):( columns ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( columns - ( columns % (SIMDSIZE) ) ) == jpos, "Invalid end calculation" );

   size_t j( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (j+SIMDSIZE*3UL) < jpos; j+=SIMDSIZE*4UL ) {
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; j<jpos; j+=SIMDSIZE ) {
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; remainder && j<columns; ++j ) {
      *left *= *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,true,true,SF,RAs...>::multAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const ResultType tmp( serial( *this ) );

   reset();

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) = tmp[element->index()] * element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the division assignment of a dense vector.
//
// \param rhs The right-hand side dense vector divisor.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedDivAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::divAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) /= (~rhs)[j    ];
      matrix_(row(),j+1UL) /= (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) /= (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the division assignment of a dense vector.
//
// \param rhs The right-hand side dense vector divisor.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,true,true,SF,RAs...>::BLAZE_TEMPLATE VectorizedDivAssign<VT> >
   RowImpl<MT,true,true,SF,RAs...>::divAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t columns( size() );

   const size_t jpos( columns & size_t(-SIMDSIZE) );
   BLAZE_INTERNAL_ASSERT( ( columns - ( columns % (SIMDSIZE) ) ) == jpos, "Invalid end calculation" );

   size_t j( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (j+SIMDSIZE*3UL) < jpos; j+=SIMDSIZE*4UL ) {
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; j<jpos; j+=SIMDSIZE ) {
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; j<columns; ++j ) {
      *left /= *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************








//=================================================================================================
//
//  CLASS TEMPLATE SPECIALIZATION FOR GENERAL COLUMN-MAJOR MATRICES
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of RowImpl for general column-major dense matrices.
// \ingroup row
//
// This specialization of RowImpl adapts the class template to the requirements of general
// column-major dense matrices.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
class RowImpl<MT,false,true,false,RAs...>
   : public View< DenseVector< RowImpl<MT,false,true,false,RAs...>, true > >
   , private RowData<MT,RAs...>
{
 private:
   //**Type definitions****************************************************************************
   using DataType = RowData<MT,RAs...>;  //!< The type of the RowData base class.
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   //! Type of this RowImpl instance.
   using This = RowImpl<MT,false,true,false,RAs...>;

   using BaseType      = DenseVector<This,true>;      //!< Base type of this RowImpl instance.
   using ResultType    = RowTrait_<MT>;               //!< Result type for expression template evaluations.
   using TransposeType = TransposeType_<ResultType>;  //!< Transpose type for expression template evaluations.
   using ElementType   = ElementType_<MT>;            //!< Type of the row elements.
   using ReturnType    = ElementType_<MT>;            //!< Return type for expression template evaluations
   using CompositeType = const RowImpl&;              //!< Data type for composite expression templates.

   //! Reference to a constant row value.
   using ConstReference = ConstReference_<MT>;

   //! Reference to a non-constant row value.
   using Reference = If_< IsConst<MT>, ConstReference, Reference_<MT> >;

   //! Pointer to a constant row value.
   using ConstPointer = const ElementType*;

   //! Pointer to a non-constant row value.
   using Pointer = If_< Or< IsConst<MT>, Not< HasMutableDataAccess<MT> > >, ConstPointer, ElementType* >;
   //**********************************************************************************************

   //**RowIterator class definition****************************************************************
   /*!\brief Iterator over the elements of the dense row.
   */
   template< typename MatrixType >  // Type of the dense matrix
   class RowIterator
   {
    public:
      //**Type definitions*************************************************************************
      //! Return type for the access to the value of a dense element.
      using Reference = If_< IsConst<MatrixType>, ConstReference_<MatrixType>, Reference_<MatrixType> >;

      using IteratorCategory = std::random_access_iterator_tag;  //!< The iterator category.
      using ValueType        = RemoveReference_<Reference>;      //!< Type of the underlying elements.
      using PointerType      = ValueType*;                       //!< Pointer return type.
      using ReferenceType    = Reference;                        //!< Reference return type.
      using DifferenceType   = ptrdiff_t;                        //!< Difference between two iterators.

      // STL iterator requirements
      using iterator_category = IteratorCategory;  //!< The iterator category.
      using value_type        = ValueType;         //!< Type of the underlying elements.
      using pointer           = PointerType;       //!< Pointer return type.
      using reference         = ReferenceType;     //!< Reference return type.
      using difference_type   = DifferenceType;    //!< Difference between two iterators.
      //*******************************************************************************************

      //**Constructor******************************************************************************
      /*!\brief Default constructor of the RowIterator class.
      */
      inline RowIterator() noexcept
         : matrix_( nullptr )  // The dense matrix containing the row.
         , row_   ( 0UL )      // The current row index.
         , column_( 0UL )      // The current column index.
      {}
      //*******************************************************************************************

      //**Constructor******************************************************************************
      /*!\brief Constructor of the RowIterator class.
      //
      // \param matrix The matrix containing the row.
      // \param row The row index.
      // \param column The column index.
      */
      inline RowIterator( MatrixType& matrix, size_t row, size_t column ) noexcept
         : matrix_( &matrix )  // The dense matrix containing the row.
         , row_   ( row     )  // The current row index.
         , column_( column  )  // The current column index.
      {}
      //*******************************************************************************************

      //**Constructor******************************************************************************
      /*!\brief Conversion constructor from different RowIterator instances.
      //
      // \param it The row iterator to be copied.
      */
      template< typename MatrixType2 >
      inline RowIterator( const RowIterator<MatrixType2>& it ) noexcept
         : matrix_( it.matrix_ )  // The dense matrix containing the row.
         , row_   ( it.row_    )  // The current row index.
         , column_( it.column_ )  // The current column index.
      {}
      //*******************************************************************************************

      //**Addition assignment operator*************************************************************
      /*!\brief Addition assignment operator.
      //
      // \param inc The increment of the iterator.
      // \return The incremented iterator.
      */
      inline RowIterator& operator+=( size_t inc ) noexcept {
         column_ += inc;
         return *this;
      }
      //*******************************************************************************************

      //**Subtraction assignment operator**********************************************************
      /*!\brief Subtraction assignment operator.
      //
      // \param dec The decrement of the iterator.
      // \return The decremented iterator.
      */
      inline RowIterator& operator-=( size_t dec ) noexcept {
         column_ -= dec;
         return *this;
      }
      //*******************************************************************************************

      //**Prefix increment operator****************************************************************
      /*!\brief Pre-increment operator.
      //
      // \return Reference to the incremented iterator.
      */
      inline RowIterator& operator++() noexcept {
         ++column_;
         return *this;
      }
      //*******************************************************************************************

      //**Postfix increment operator***************************************************************
      /*!\brief Post-increment operator.
      //
      // \return The previous position of the iterator.
      */
      inline const RowIterator operator++( int ) noexcept {
         const RowIterator tmp( *this );
         ++(*this);
         return tmp;
      }
      //*******************************************************************************************

      //**Prefix decrement operator****************************************************************
      /*!\brief Pre-decrement operator.
      //
      // \return Reference to the decremented iterator.
      */
      inline RowIterator& operator--() noexcept {
         --column_;
         return *this;
      }
      //*******************************************************************************************

      //**Postfix decrement operator***************************************************************
      /*!\brief Post-decrement operator.
      //
      // \return The previous position of the iterator.
      */
      inline const RowIterator operator--( int ) noexcept {
         const RowIterator tmp( *this );
         --(*this);
         return tmp;
      }
      //*******************************************************************************************

      //**Subscript operator***********************************************************************
      /*!\brief Direct access to the dense row elements.
      //
      // \param index Access index.
      // \return Reference to the accessed value.
      */
      inline ReferenceType operator[]( size_t index ) const {
         return (*matrix_)(row_,column_+index);
      }
      //*******************************************************************************************

      //**Element access operator******************************************************************
      /*!\brief Direct access to the dense row element at the current iterator position.
      //
      // \return Reference to the current value.
      */
      inline ReferenceType operator*() const {
         return (*matrix_)(row_,column_);
      }
      //*******************************************************************************************

      //**Element access operator******************************************************************
      /*!\brief Direct access to the dense row element at the current iterator position.
      //
      // \return Pointer to the dense row element at the current iterator position.
      */
      inline PointerType operator->() const {
         return &(*matrix_)(row_,column_);
      }
      //*******************************************************************************************

      //**Equality operator************************************************************************
      /*!\brief Equality comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side row iterator.
      // \return \a true if the iterators refer to the same element, \a false if not.
      */
      template< typename MatrixType2 >
      inline bool operator==( const RowIterator<MatrixType2>& rhs ) const noexcept {
         return column_ == rhs.column_;
      }
      //*******************************************************************************************

      //**Inequality operator**********************************************************************
      /*!\brief Inequality comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side row iterator.
      // \return \a true if the iterators don't refer to the same element, \a false if they do.
      */
      template< typename MatrixType2 >
      inline bool operator!=( const RowIterator<MatrixType2>& rhs ) const noexcept {
         return !( *this == rhs );
      }
      //*******************************************************************************************

      //**Less-than operator***********************************************************************
      /*!\brief Less-than comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side row iterator.
      // \return \a true if the left-hand side iterator is smaller, \a false if not.
      */
      template< typename MatrixType2 >
      inline bool operator<( const RowIterator<MatrixType2>& rhs ) const noexcept {
         return column_ < rhs.column_;
      }
      //*******************************************************************************************

      //**Greater-than operator********************************************************************
      /*!\brief Greater-than comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side row iterator.
      // \return \a true if the left-hand side iterator is greater, \a false if not.
      */
      template< typename MatrixType2 >
      inline bool operator>( const RowIterator<MatrixType2>& rhs ) const noexcept {
         return column_ > rhs.column_;
      }
      //*******************************************************************************************

      //**Less-or-equal-than operator**************************************************************
      /*!\brief Less-than comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side row iterator.
      // \return \a true if the left-hand side iterator is smaller or equal, \a false if not.
      */
      template< typename MatrixType2 >
      inline bool operator<=( const RowIterator<MatrixType2>& rhs ) const noexcept {
         return column_ <= rhs.column_;
      }
      //*******************************************************************************************

      //**Greater-or-equal-than operator***********************************************************
      /*!\brief Greater-than comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side row iterator.
      // \return \a true if the left-hand side iterator is greater or equal, \a false if not.
      */
      template< typename MatrixType2 >
      inline bool operator>=( const RowIterator<MatrixType2>& rhs ) const noexcept {
         return column_ >= rhs.column_;
      }
      //*******************************************************************************************

      //**Subtraction operator*********************************************************************
      /*!\brief Calculating the number of elements between two row iterators.
      //
      // \param rhs The right-hand side row iterator.
      // \return The number of elements between the two row iterators.
      */
      inline DifferenceType operator-( const RowIterator& rhs ) const noexcept {
         return column_ - rhs.column_;
      }
      //*******************************************************************************************

      //**Addition operator************************************************************************
      /*!\brief Addition between a RowIterator and an integral value.
      //
      // \param it The iterator to be incremented.
      // \param inc The number of elements the iterator is incremented.
      // \return The incremented iterator.
      */
      friend inline const RowIterator operator+( const RowIterator& it, size_t inc ) noexcept {
         return RowIterator( *it.matrix_, it.row_, it.column_+inc );
      }
      //*******************************************************************************************

      //**Addition operator************************************************************************
      /*!\brief Addition between an integral value and a RowIterator.
      //
      // \param inc The number of elements the iterator is incremented.
      // \param it The iterator to be incremented.
      // \return The incremented iterator.
      */
      friend inline const RowIterator operator+( size_t inc, const RowIterator& it ) noexcept {
         return RowIterator( *it.matrix_, it.row_, it.column_+inc );
      }
      //*******************************************************************************************

      //**Subtraction operator*********************************************************************
      /*!\brief Subtraction between a RowIterator and an integral value.
      //
      // \param it The iterator to be decremented.
      // \param inc The number of elements the iterator is decremented.
      // \return The decremented iterator.
      */
      friend inline const RowIterator operator-( const RowIterator& it, size_t dec ) noexcept {
         return RowIterator( *it.matrix_, it.row_, it.column_-dec );
      }
      //*******************************************************************************************

    private:
      //**Member variables*************************************************************************
      MatrixType* matrix_;  //!< The dense matrix containing the row.
      size_t      row_;     //!< The current row index.
      size_t      column_;  //!< The current column index.
      //*******************************************************************************************

      //**Friend declarations**********************************************************************
      template< typename MatrixType2 > friend class RowIterator;
      //*******************************************************************************************
   };
   //**********************************************************************************************

   //**Type definitions****************************************************************************
   //! Iterator over constant elements.
   using ConstIterator = RowIterator<const MT>;

   //! Iterator over non-constant elements.
   using Iterator = If_< IsConst<MT>, ConstIterator, RowIterator<MT> >;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum : bool { simdEnabled = false };

   //! Compilation switch for the expression template assignment strategy.
   enum : bool { smpAssignable = MT::smpAssignable };
   //**********************************************************************************************

   //**Constructors********************************************************************************
   /*!\name Constructors */
   //@{
   explicit inline RowImpl( MT& matrix );
   explicit inline RowImpl( MT& matrix, size_t index );
   // No explicitly declared copy constructor.
   //@}
   //**********************************************************************************************

   //**Destructor**********************************************************************************
   // No explicitly declared destructor.
   //**********************************************************************************************

   //**Data access functions***********************************************************************
   /*!\name Data access functions */
   //@{
   inline Reference      operator[]( size_t index );
   inline ConstReference operator[]( size_t index ) const;
   inline Reference      at( size_t index );
   inline ConstReference at( size_t index ) const;
   inline Pointer        data  () noexcept;
   inline ConstPointer   data  () const noexcept;
   inline Iterator       begin ();
   inline ConstIterator  begin () const;
   inline ConstIterator  cbegin() const;
   inline Iterator       end   ();
   inline ConstIterator  end   () const;
   inline ConstIterator  cend  () const;
   //@}
   //**********************************************************************************************

   //**Assignment operators************************************************************************
   /*!\name Assignment operators */
   //@{
   inline RowImpl& operator=( const ElementType& rhs );
   inline RowImpl& operator=( initializer_list<ElementType> list );
   inline RowImpl& operator=( const RowImpl& rhs );

   template< typename VT > inline RowImpl& operator= ( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator+=( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator-=( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator*=( const DenseVector<VT,true>&  rhs );
   template< typename VT > inline RowImpl& operator*=( const SparseVector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator/=( const DenseVector<VT,true>&  rhs );
   template< typename VT > inline RowImpl& operator%=( const Vector<VT,true>& rhs );

   template< typename Other >
   inline EnableIf_< IsNumeric<Other>, RowImpl >& operator*=( Other rhs );

   template< typename Other >
   inline EnableIf_< IsNumeric<Other>, RowImpl >& operator/=( Other rhs );
   //@}
   //**********************************************************************************************

   //**Utility functions***************************************************************************
   /*!\name Utility functions */
   //@{
   using DataType::operand;
   using DataType::row;

   inline size_t size() const noexcept;
   inline size_t spacing() const noexcept;
   inline size_t capacity() const noexcept;
   inline size_t nonZeros() const;
   inline void   reset();
   //@}
   //**********************************************************************************************

   //**Numeric functions***************************************************************************
   /*!\name Numeric functions */
   //@{
   template< typename Other > inline RowImpl& scale( const Other& scalar );
   //@}
   //**********************************************************************************************

   //**Expression template evaluation functions****************************************************
   /*!\name Expression template evaluation functions */
   //@{
   template< typename Other >
   inline bool canAlias( const Other* alias ) const noexcept;

   template< typename MT2, bool SO2, bool SF2, size_t... RAs2 >
   inline bool canAlias( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept;

   template< typename Other >
   inline bool isAliased( const Other* alias ) const noexcept;

   template< typename MT2, bool SO2, bool SF2, size_t... RAs2 >
   inline bool isAliased( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept;

   inline bool isAligned   () const noexcept;
   inline bool canSMPAssign() const noexcept;

   template< typename VT > inline void assign    ( const DenseVector <VT,true>& rhs );
   template< typename VT > inline void assign    ( const SparseVector<VT,true>& rhs );
   template< typename VT > inline void addAssign ( const DenseVector <VT,true>& rhs );
   template< typename VT > inline void addAssign ( const SparseVector<VT,true>& rhs );
   template< typename VT > inline void subAssign ( const DenseVector <VT,true>& rhs );
   template< typename VT > inline void subAssign ( const SparseVector<VT,true>& rhs );
   template< typename VT > inline void multAssign( const DenseVector <VT,true>& rhs );
   template< typename VT > inline void multAssign( const SparseVector<VT,true>& rhs );
   template< typename VT > inline void divAssign ( const DenseVector <VT,true>& rhs );
   //@}
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   using DataType::matrix_;
   //**********************************************************************************************

   //**Friend declarations*************************************************************************
   template< typename MT2, bool SO2, bool DF2, bool SF2, size_t... RAs2 > friend class RowImpl;
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE        ( MT );
   BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_SYMMETRIC_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_COMPUTATION_TYPE     ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_TRANSEXPR_TYPE       ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_POINTER_TYPE         ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_REFERENCE_TYPE       ( MT );
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  CONSTRUCTOR
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for rows with a compile time index.
//
// \param matrix The matrix containing the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,false,RAs...>::RowImpl( MT& matrix )
   : DataType( matrix )  // Base class initialization
{}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for rows with a runtime index.
//
// \param matrix The matrix containing the row.
// \param index The index of the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,false,RAs...>::RowImpl( MT& matrix, size_t index )
   : DataType( matrix, index )  // Base class initialization
{}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DATA ACCESS FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::Reference
   RowImpl<MT,false,true,false,RAs...>::operator[]( size_t index )
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(row(),index);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstReference
   RowImpl<MT,false,true,false,RAs...>::operator[]( size_t index ) const
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return const_cast<const MT&>( matrix_ )(row(),index);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checked access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid row access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::Reference
   RowImpl<MT,false,true,false,RAs...>::at( size_t index )
{
   if( index >= size() ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   return (*this)[index];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checked access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid row access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstReference
   RowImpl<MT,false,true,false,RAs...>::at( size_t index ) const
{
   if( index >= size() ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   return (*this)[index];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
//
// This function returns a pointer to the internal storage of the dense row. Note that in case
// of a column-major matrix you can NOT assume that the row elements lie adjacent to each other!
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::Pointer
   RowImpl<MT,false,true,false,RAs...>::data() noexcept
{
   return matrix_.data() + row();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
//
// This function returns a pointer to the internal storage of the dense row. Note that in case
// of a column-major matrix you can NOT assume that the row elements lie adjacent to each other!
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstPointer
   RowImpl<MT,false,true,false,RAs...>::data() const noexcept
{
   return matrix_.data() + row();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::Iterator
   RowImpl<MT,false,true,false,RAs...>::begin()
{
   return Iterator( matrix_, row(), 0UL );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstIterator
   RowImpl<MT,false,true,false,RAs...>::begin() const
{
   return ConstIterator( matrix_, row(), 0UL );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstIterator
   RowImpl<MT,false,true,false,RAs...>::cbegin() const
{
   return ConstIterator( matrix_, row(), 0UL );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::Iterator
   RowImpl<MT,false,true,false,RAs...>::end()
{
   return Iterator( matrix_, row(), size() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstIterator
   RowImpl<MT,false,true,false,RAs...>::end() const
{
   return ConstIterator( matrix_, row(), size() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,false,RAs...>::ConstIterator
   RowImpl<MT,false,true,false,RAs...>::cend() const
{
   return ConstIterator( matrix_, row(), size() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ASSIGNMENT OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Homogenous assignment to all row elements.
//
// \param rhs Scalar value to be assigned to all row elements.
// \return Reference to the assigned row.
//
// This function homogeneously assigns the given value to all elements of the row. Note that in
// case the underlying dense matrix is a lower/upper matrix only lower/upper and diagonal elements
// of the underlying matrix are modified.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator=( const ElementType& rhs )
{
   const size_t jbegin( ( IsUpper<MT>::value )
                        ?( ( IsUniUpper<MT>::value || IsStrictlyUpper<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t jend  ( ( IsLower<MT>::value )
                        ?( ( IsUniLower<MT>::value || IsStrictlyLower<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t j=jbegin; j<jend; ++j )
      matrix_(row(),j) = rhs;

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief List assignment to all row elements.
//
// \param list The initializer list.
// \exception std::invalid_argument Invalid assignment to row.
//
// This assignment operator offers the option to directly assign to all elements of the dense
// row by means of an initializer list. The row elements are assigned the values from the given
// initializer list. Missing values are reset to their default state. Note that in case the size
// of the initializer list exceeds the size of the row, a \a std::invalid_argument exception is
// thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator=( initializer_list<ElementType> list )
{
   if( list.size() > size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to row" );
   }

   std::fill( std::copy( list.begin(), list.end(), begin() ), end(), ElementType() );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Copy assignment operator for Row.
//
// \param rhs Dense row to be copied.
// \return Reference to the assigned row.
// \exception std::invalid_argument Row sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two rows don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator=( const RowImpl& rhs )
{
   if( &rhs == this ) return *this;

   if( size() != rhs.size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Row sizes do not match" );
   }

   if( !tryAssign( matrix_, rhs, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsExpression<MT>::value && rhs.canAlias( &matrix_ ) ) {
      const ResultType tmp( rhs );
      smpAssign( left, tmp );
   }
   else {
      smpAssign( left, rhs );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Assignment operator for different vectors.
//
// \param rhs Vector to be assigned.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( ResultType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType tmp( right );
      smpAssign( left, tmp );
   }
   else {
      if( IsSparseVector<VT>::value )
         reset();
      smpAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition assignment operator for the addition of a vector (\f$ \vec{a}+=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be added to the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator+=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryAddAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpAddAssign( left, tmp );
   }
   else {
      smpAddAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction assignment operator for the subtraction of a vector (\f$ \vec{a}-=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be subtracted from the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator-=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !trySubAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpSubAssign( left, tmp );
   }
   else {
      smpSubAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a dense vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side dense vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator*=( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryMultAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpMultAssign( left, tmp );
   }
   else {
      smpMultAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a sparse vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side sparse vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator*=( const SparseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( ResultType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   const ResultType right( *this * (~rhs) );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   smpAssign( left, right );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a dense vector (\f$ \vec{a}/=\vec{b} \f$).
//
// \param rhs The right-hand side dense vector divisor.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator/=( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryDivAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpDivAssign( left, tmp );
   }
   else {
      smpDivAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Cross product assignment operator for the multiplication of a vector
//        (\f$ \vec{a}\times=\vec{b} \f$).
//
// \param rhs The right-hand side vector for the cross product.
// \return Reference to the assigned row.
// \exception std::invalid_argument Invalid vector size for cross product.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current size of any of the two vectors is not equal to 3, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::operator%=( const Vector<VT,true>& rhs )
{
   using blaze::assign;

   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   using CrossType = CrossTrait_< ResultType, ResultType_<VT> >;

   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( CrossType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( CrossType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( CrossType );

   if( size() != 3UL || (~rhs).size() != 3UL ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid vector size for cross product" );
   }

   const CrossType right( *this % (~rhs) );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   assign( left, right );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication between a dense row and
//        a scalar value (\f$ \vec{a}*=s \f$).
//
// \param rhs The right-hand side scalar value for the multiplication.
// \return Reference to the vector.
//
// This operator cannot be used for rows on lower or upper unitriangular matrices. The attempt
// to scale such a row results in a compilation error!
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<Other>, RowImpl<MT,false,true,false,RAs...> >&
   RowImpl<MT,false,true,false,RAs...>::operator*=( Other rhs )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   return operator=( (*this) * rhs );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a dense row by a scalar value
//        (\f$ \vec{a}/=s \f$).
//
// \param rhs The right-hand side scalar value for the division.
// \return Reference to the vector.
//
// This operator cannot be used for rows on lower or upper unitriangular matrices. The attempt
// to scale such a row results in a compilation error!
//
// \note A division by zero is only checked by an user assert.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<Other>, RowImpl<MT,false,true,false,RAs...> >&
   RowImpl<MT,false,true,false,RAs...>::operator/=( Other rhs )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   BLAZE_USER_ASSERT( rhs != Other(0), "Division by zero detected" );

   return operator=( (*this) / rhs );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the current size/dimension of the row.
//
// \return The size of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,false,RAs...>::size() const noexcept
{
   return matrix_.columns();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the minimum capacity of the row.
//
// \return The minimum capacity of the row.
//
// This function returns the minimum capacity of the row, which corresponds to the current size
// plus padding.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,false,RAs...>::spacing() const noexcept
{
   return matrix_.spacing();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the maximum capacity of the dense row.
//
// \return The maximum capacity of the dense row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,false,RAs...>::capacity() const noexcept
{
   return matrix_.columns();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the number of non-zero elements in the row.
//
// \return The number of non-zero elements in the row.
//
// Note that the number of non-zero elements is always less than or equal to the current number
// of columns of the matrix containing the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,false,RAs...>::nonZeros() const
{
   const size_t columns( size() );
   size_t nonzeros( 0UL );

   for( size_t j=0UL; j<columns; ++j )
      if( !isDefault( matrix_(row(),j) ) )
         ++nonzeros;

   return nonzeros;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Reset to the default initial values.
//
// \return void
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline void RowImpl<MT,false,true,false,RAs...>::reset()
{
   using blaze::clear;

   const size_t jbegin( ( IsUpper<MT>::value )
                        ?( ( IsUniUpper<MT>::value || IsStrictlyUpper<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t jend  ( ( IsLower<MT>::value )
                        ?( ( IsUniLower<MT>::value || IsStrictlyLower<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t j=jbegin; j<jend; ++j )
      clear( matrix_(row(),j) );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  NUMERIC FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Scaling of the row by the scalar value \a scalar (\f$ \vec{a}=\vec{b}*s \f$).
//
// \param scalar The scalar value for the row scaling.
// \return Reference to the dense row.
//
// This function scales the row by applying the given scalar value \a scalar to each element
// of the row. For built-in and \c complex data types it has the same effect as using the
// multiplication assignment operator. Note that the function cannot be used to scale a row
// on a lower or upper unitriangular matrix. The attempt to scale such a row results in a
// compile time error!
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the scalar value
inline RowImpl<MT,false,true,false,RAs...>&
   RowImpl<MT,false,true,false,RAs...>::scale( const Other& scalar )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   const size_t jbegin( ( IsUpper<MT>::value )
                        ?( ( IsStrictlyUpper<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t jend  ( ( IsLower<MT>::value )
                        ?( ( IsStrictlyLower<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t j=jbegin; j<jend; ++j ) {
      matrix_(row(),j) *= scalar;
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TEMPLATE EVALUATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the foreign expression
inline bool RowImpl<MT,false,true,false,RAs...>::canAlias( const Other* alias ) const noexcept
{
   return matrix_.isAliased( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given dense row \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename MT2      // Data type of the foreign dense row
        , bool SO2          // Storage order of the foreign dense row
        , bool SF2          // Symmetry flag of the foreign dense row
        , size_t... RAs2 >  // Compile time row arguments of the foreign dense row
inline bool
   RowImpl<MT,false,true,false,RAs...>::canAlias( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept
{
   return matrix_.isAliased( &alias->matrix_ ) && ( row() == alias->row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the foreign expression
inline bool RowImpl<MT,false,true,false,RAs...>::isAliased( const Other* alias ) const noexcept
{
   return matrix_.isAliased( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given dense row \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename MT2      // Data type of the foreign dense row
        , bool SO2          // Storage order of the foreign dense row
        , bool SF2          // Symmetry flag of the foreign dense row
        , size_t... RAs2 >  // Compile time row arguments of the foreign dense row
inline bool
   RowImpl<MT,false,true,false,RAs...>::isAliased( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept
{
   return matrix_.isAliased( &alias->matrix_ ) && ( row() == alias->row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is properly aligned in memory.
//
// \return \a true in case the dense row is aligned, \a false if not.
//
// This function returns whether the dense row is guaranteed to be properly aligned in memory,
// i.e. whether the beginning and the end of the dense row are guaranteed to conform to the
// alignment restrictions of the element type \a Type.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline bool RowImpl<MT,false,true,false,RAs...>::isAligned() const noexcept
{
   return false;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can be used in SMP assignments.
//
// \return \a true in case the dense row can be used in SMP assignments, \a false if not.
//
// This function returns whether the dense row can be used in SMP assignments. In contrast to
// the \a smpAssignable member enumeration, which is based solely on compile time information,
// this function additionally provides runtime information (as for instance the current size
// of the dense row).
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline bool RowImpl<MT,false,true,false,RAs...>::canSMPAssign() const noexcept
{
   return ( size() > SMP_DVECASSIGN_THRESHOLD );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline void RowImpl<MT,false,true,false,RAs...>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) = (~rhs)[j    ];
      matrix_(row(),j+1UL) = (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) = (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,false,RAs...>::assign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) = element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline void RowImpl<MT,false,true,false,RAs...>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) += (~rhs)[j    ];
      matrix_(row(),j+1UL) += (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) += (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,false,RAs...>::addAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) += element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline void RowImpl<MT,false,true,false,RAs...>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) -= (~rhs)[j    ];
      matrix_(row(),j+1UL) -= (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) -= (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,false,RAs...>::subAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) -= element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline void RowImpl<MT,false,true,false,RAs...>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) *= (~rhs)[j    ];
      matrix_(row(),j+1UL) *= (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) *= (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,false,RAs...>::multAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const ResultType tmp( serial( *this ) );

   reset();

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row(),element->index()) = tmp[element->index()] * element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the division assignment of a dense vector.
//
// \param rhs The right-hand side dense vector divisor.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline void RowImpl<MT,false,true,false,RAs...>::divAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jpos( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jpos; j+=2UL ) {
      matrix_(row(),j    ) /= (~rhs)[j    ];
      matrix_(row(),j+1UL) /= (~rhs)[j+1UL];
   }
   if( jpos < (~rhs).size() )
      matrix_(row(),jpos) /= (~rhs)[jpos];
}
/*! \endcond */
//*************************************************************************************************








//=================================================================================================
//
//  CLASS TEMPLATE SPECIALIZATION FOR SYMMETRIC COLUMN-MAJOR DENSE MATRICES
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of RowImpl for symmetric column-major dense matrices.
// \ingroup row
//
// This specialization of RowImpl adapts the class template to the requirements of symmetric
// column-major dense matrices.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
class RowImpl<MT,false,true,true,RAs...>
   : public View< DenseVector< RowImpl<MT,false,true,true,RAs...>, true > >
   , private RowData<MT,RAs...>
{
 private:
   //**Type definitions****************************************************************************
   using DataType = RowData<MT,RAs...>;  //!< The type of the RowData base class.
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   //! Type of this RowImpl instance.
   using This = RowImpl<MT,false,true,true,RAs...>;

   using BaseType      = DenseVector<This,true>;      //!< Base type of this RowImpl instance.
   using ResultType    = RowTrait_<MT>;               //!< Result type for expression template evaluations.
   using TransposeType = TransposeType_<ResultType>;  //!< Transpose type for expression template evaluations.
   using ElementType   = ElementType_<MT>;            //!< Type of the row elements.
   using SIMDType      = SIMDTrait_<ElementType>;     //!< SIMD type of the row elements.
   using ReturnType    = ElementType_<MT>;            //!< Return type for expression template evaluations
   using CompositeType = const RowImpl&;              //!< Data type for composite expression templates.

   //! Reference to a constant row value.
   using ConstReference = ConstReference_<MT>;

   //! Reference to a non-constant row value.
   using Reference = If_< IsConst<MT>, ConstReference, Reference_<MT> >;

   //! Pointer to a constant row value.
   using ConstPointer = const ElementType*;

   //! Pointer to a non-constant row value.
   using Pointer = If_< Or< IsConst<MT>, Not< HasMutableDataAccess<MT> > >, ConstPointer, ElementType* >;

   //! Iterator over constant elements.
   using ConstIterator = ConstIterator_<MT>;

   //! Iterator over non-constant elements.
   using Iterator = If_< IsConst<MT>, ConstIterator, Iterator_<MT> >;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum : bool { simdEnabled = MT::simdEnabled };

   //! Compilation switch for the expression template assignment strategy.
   enum : bool { smpAssignable = MT::smpAssignable };
   //**********************************************************************************************

   //**Constructors********************************************************************************
   /*!\name Constructors */
   //@{
   explicit inline RowImpl( MT& matrix );
   explicit inline RowImpl( MT& matrix, size_t index );
   // No explicitly declared copy constructor.
   //@}
   //**********************************************************************************************

   //**Destructor**********************************************************************************
   // No explicitly declared destructor.
   //**********************************************************************************************

   //**Data access functions***********************************************************************
   /*!\name Data access functions */
   //@{
   inline Reference      operator[]( size_t index );
   inline ConstReference operator[]( size_t index ) const;
   inline Reference      at( size_t index );
   inline ConstReference at( size_t index ) const;
   inline Pointer        data  () noexcept;
   inline ConstPointer   data  () const noexcept;
   inline Iterator       begin ();
   inline ConstIterator  begin () const;
   inline ConstIterator  cbegin() const;
   inline Iterator       end   ();
   inline ConstIterator  end   () const;
   inline ConstIterator  cend  () const;
   //@}
   //**********************************************************************************************

   //**Assignment operators************************************************************************
   /*!\name Assignment operators */
   //@{
   inline RowImpl& operator=( const ElementType& rhs );
   inline RowImpl& operator=( initializer_list<ElementType> list );
   inline RowImpl& operator=( const RowImpl& rhs );

   template< typename VT > inline RowImpl& operator= ( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator+=( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator-=( const Vector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator*=( const DenseVector<VT,true>&  rhs );
   template< typename VT > inline RowImpl& operator*=( const SparseVector<VT,true>& rhs );
   template< typename VT > inline RowImpl& operator/=( const DenseVector<VT,true>&  rhs );
   template< typename VT > inline RowImpl& operator%=( const Vector<VT,true>& rhs );

   template< typename Other >
   inline EnableIf_< IsNumeric<Other>, RowImpl >& operator*=( Other rhs );

   template< typename Other >
   inline EnableIf_< IsNumeric<Other>, RowImpl >& operator/=( Other rhs );
   //@}
   //**********************************************************************************************

   //**Utility functions***************************************************************************
   /*!\name Utility functions */
   //@{
   using DataType::operand;
   using DataType::row;

   inline size_t size() const noexcept;
   inline size_t spacing() const noexcept;
   inline size_t capacity() const noexcept;
   inline size_t nonZeros() const;
   inline void   reset();
   //@}
   //**********************************************************************************************

   //**Numeric functions***************************************************************************
   /*!\name Numeric functions */
   //@{
   template< typename Other > inline RowImpl& scale( const Other& scalar );
   //@}
   //**********************************************************************************************

 private:
   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedAddAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDAdd< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedSubAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDSub< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedMultAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDMult< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**********************************************************************************************
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedDivAssign {
      enum : bool { value = useOptimizedKernels &&
                            simdEnabled && VT::simdEnabled &&
                            IsSIMDCombinable< ElementType, ElementType_<VT> >::value &&
                            HasSIMDDiv< ElementType, ElementType_<VT> >::value };
   };
   //**********************************************************************************************

   //**SIMD properties*****************************************************************************
   //! The number of elements packed within a single SIMD element.
   enum : size_t { SIMDSIZE = SIMDTrait<ElementType>::size };
   //**********************************************************************************************

 public:
   //**Expression template evaluation functions****************************************************
   /*!\name Expression template evaluation functions */
   //@{
   template< typename Other >
   inline bool canAlias( const Other* alias ) const noexcept;

   template< typename MT2, bool SO2, bool SF2, size_t... RAs2 >
   inline bool canAlias( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept;

   template< typename Other >
   inline bool isAliased( const Other* alias ) const noexcept;

   template< typename MT2, bool SO2, bool SF2, size_t... RAs2 >
   inline bool isAliased( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept;

   inline bool isAligned   () const noexcept;
   inline bool canSMPAssign() const noexcept;

   BLAZE_ALWAYS_INLINE SIMDType load ( size_t index ) const noexcept;
   BLAZE_ALWAYS_INLINE SIMDType loada( size_t index ) const noexcept;
   BLAZE_ALWAYS_INLINE SIMDType loadu( size_t index ) const noexcept;

   BLAZE_ALWAYS_INLINE void store ( size_t index, const SIMDType& value ) noexcept;
   BLAZE_ALWAYS_INLINE void storea( size_t index, const SIMDType& value ) noexcept;
   BLAZE_ALWAYS_INLINE void storeu( size_t index, const SIMDType& value ) noexcept;
   BLAZE_ALWAYS_INLINE void stream( size_t index, const SIMDType& value ) noexcept;

   template< typename VT >
   inline DisableIf_< VectorizedAssign<VT> > assign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedAssign<VT> > assign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void assign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedAddAssign<VT> > addAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedAddAssign<VT> > addAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void addAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedSubAssign<VT> > subAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedSubAssign<VT> > subAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void subAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedMultAssign<VT> > multAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedMultAssign<VT> > multAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void multAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline DisableIf_< VectorizedDivAssign<VT> > divAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline EnableIf_< VectorizedDivAssign<VT> > divAssign( const DenseVector<VT,true>& rhs );
   //@}
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   using DataType::matrix_;
   //**********************************************************************************************

   //**Friend declarations*************************************************************************
   template< typename MT2, bool SO2, bool DF2, bool SF2, size_t... RAs2 > friend class RowImpl;
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE       ( MT );
   BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_BE_SYMMETRIC_MATRIX_TYPE   ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_COMPUTATION_TYPE    ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_TRANSEXPR_TYPE      ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_POINTER_TYPE        ( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_REFERENCE_TYPE      ( MT );
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  CONSTRUCTOR
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for rows with a compile time index.
//
// \param matrix The matrix containing the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,true,RAs...>::RowImpl( MT& matrix )
   : DataType( matrix )  // Base class initialization
{}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for rows with a runtime index.
//
// \param matrix The matrix containing the row.
// \param index The index of the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,true,RAs...>::RowImpl( MT& matrix, size_t index )
   : DataType( matrix, index )  // Base class initialization
{}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DATA ACCESS FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::Reference
   RowImpl<MT,false,true,true,RAs...>::operator[]( size_t index )
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(index,row());
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstReference
   RowImpl<MT,false,true,true,RAs...>::operator[]( size_t index ) const
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return const_cast<const MT&>( matrix_ )(index,row());
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checked access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid row access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::Reference
   RowImpl<MT,false,true,true,RAs...>::at( size_t index )
{
   if( index >= size() ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   return (*this)[index];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checked access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid row access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access index.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstReference
   RowImpl<MT,false,true,true,RAs...>::at( size_t index ) const
{
   if( index >= size() ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   return (*this)[index];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
//
// This function returns a pointer to the internal storage of the dense row. Note that in case
// of a column-major matrix you can NOT assume that the row elements lie adjacent to each other!
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::Pointer
   RowImpl<MT,false,true,true,RAs...>::data() noexcept
{
   return matrix_.data( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
//
// This function returns a pointer to the internal storage of the dense row. Note that in case
// of a column-major matrix you can NOT assume that the row elements lie adjacent to each other!
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstPointer
   RowImpl<MT,false,true,true,RAs...>::data() const noexcept
{
   return matrix_.data( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::Iterator
   RowImpl<MT,false,true,true,RAs...>::begin()
{
   return matrix_.begin( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstIterator
   RowImpl<MT,false,true,true,RAs...>::begin() const
{
   return matrix_.cbegin( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstIterator
   RowImpl<MT,false,true,true,RAs...>::cbegin() const
{
   return matrix_.cbegin( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::Iterator
   RowImpl<MT,false,true,true,RAs...>::end()
{
   return matrix_.end( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstIterator
   RowImpl<MT,false,true,true,RAs...>::end() const
{
   return matrix_.cend( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline typename RowImpl<MT,false,true,true,RAs...>::ConstIterator
   RowImpl<MT,false,true,true,RAs...>::cend() const
{
   return matrix_.cend( row() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ASSIGNMENT OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Homogenous assignment to all row elements.
//
// \param rhs Scalar value to be assigned to all row elements.
// \return Reference to the assigned row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator=( const ElementType& rhs )
{
   const size_t ibegin( ( IsLower<MT>::value )
                        ?( ( IsUniLower<MT>::value || IsStrictlyLower<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t iend  ( ( IsUpper<MT>::value )
                        ?( ( IsUniUpper<MT>::value || IsStrictlyUpper<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t i=ibegin; i<iend; ++i )
      matrix_(i,row()) = rhs;

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief List assignment to all row elements.
//
// \param list The initializer list.
// \exception std::invalid_argument Invalid assignment to row.
//
// This assignment operator offers the option to directly assign to all elements of the dense
// row by means of an initializer list. The row elements are assigned the values from the given
// initializer list. Missing values are reset to their default state. Note that in case the size
// of the initializer list exceeds the size of the row, a \a std::invalid_argument exception is
// thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator=( initializer_list<ElementType> list )
{
   if( list.size() > size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to row" );
   }

   std::fill( std::copy( list.begin(), list.end(), begin() ), end(), ElementType() );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Copy assignment operator for Row.
//
// \param rhs Dense row to be copied.
// \return Reference to the assigned row.
// \exception std::invalid_argument Row sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two rows don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator=( const RowImpl& rhs )
{
   if( &rhs == this ) return *this;

   if( size() != rhs.size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Row sizes do not match" );
   }

   if( !tryAssign( matrix_, rhs, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsExpression<MT>::value && rhs.canAlias( &matrix_ ) ) {
      const ResultType tmp( rhs );
      smpAssign( left, tmp );
   }
   else {
      smpAssign( left, rhs );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Assignment operator for different vectors.
//
// \param rhs Vector to be assigned.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpAssign( left, tmp );
   }
   else {
      if( IsSparseVector<VT>::value )
         reset();
      smpAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition assignment operator for the addition of a vector (\f$ \vec{a}+=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be added to the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator+=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryAddAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpAddAssign( left, tmp );
   }
   else {
      smpAddAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction assignment operator for the subtraction of a vector (\f$ \vec{a}-=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be subtracted from the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown. Also, if the underlying matrix \a MT is a lower or upper triangular matrix and the
// assignment would violate its lower or upper property, respectively, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator-=( const Vector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !trySubAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpSubAssign( left, tmp );
   }
   else {
      smpSubAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a dense vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side dense vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator*=( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryMultAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpMultAssign( left, tmp );
   }
   else {
      smpMultAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a sparse vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side sparse vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator*=( const SparseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( ResultType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   const ResultType right( *this * (~rhs) );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   smpAssign( left, right );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a dense vector (\f$ \vec{a}/=\vec{b} \f$).
//
// \param rhs The right-hand side dense vector divisor.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator/=( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   if( size() != (~rhs).size() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Vector sizes do not match" );
   }

   using Right = If_< IsRestricted<MT>, CompositeType_<VT>, const VT& >;
   Right right( ~rhs );

   if( !tryDivAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   if( IsReference<Right>::value && right.canAlias( &matrix_ ) ) {
      const ResultType_<VT> tmp( right );
      smpDivAssign( left, tmp );
   }
   else {
      smpDivAssign( left, right );
   }

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Cross product assignment operator for the multiplication of a vector
//        (\f$ \vec{a}\times=\vec{b} \f$).
//
// \param rhs The right-hand side vector for the cross product.
// \return Reference to the assigned row.
// \exception std::invalid_argument Invalid vector size for cross product.
// \exception std::invalid_argument Invalid assignment to restricted matrix.
//
// In case the current size of any of the two vectors is not equal to 3, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side vector
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::operator%=( const Vector<VT,true>& rhs )
{
   using blaze::assign;

   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( ResultType_<VT> );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( ResultType_<VT> );

   using CrossType = CrossTrait_< ResultType, ResultType_<VT> >;

   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE  ( CrossType );
   BLAZE_CONSTRAINT_MUST_BE_ROW_VECTOR_TYPE    ( CrossType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( CrossType );

   if( size() != 3UL || (~rhs).size() != 3UL ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid vector size for cross product" );
   }

   const CrossType right( *this % (~rhs) );

   if( !tryAssign( matrix_, right, row(), 0UL ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment to restricted matrix" );
   }

   decltype(auto) left( derestrict( *this ) );

   assign( left, right );

   BLAZE_INTERNAL_ASSERT( isIntact( matrix_ ), "Invariant violation detected" );

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication between a dense row and
//        a scalar value (\f$ \vec{a}*=s \f$).
//
// \param rhs The right-hand side scalar value for the multiplication.
// \return Reference to the vector.
//
// This operator cannot be used for rows on lower or upper unitriangular matrices. The attempt
// to scale such a row results in a compilation error!
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<Other>, RowImpl<MT,false,true,true,RAs...> >&
   RowImpl<MT,false,true,true,RAs...>::operator*=( Other rhs )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   return operator=( (*this) * rhs );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a dense row by a scalar value
//        (\f$ \vec{a}/=s \f$).
//
// \param rhs The right-hand side scalar value for the division.
// \return Reference to the vector.
//
// This operator cannot be used for rows on lower or upper unitriangular matrices. The attempt
// to scale such a row results in a compilation error!
//
// \note A division by zero is only checked by an user assert.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<Other>, RowImpl<MT,false,true,true,RAs...> >&
   RowImpl<MT,false,true,true,RAs...>::operator/=( Other rhs )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   BLAZE_USER_ASSERT( rhs != Other(0), "Division by zero detected" );

   return operator=( (*this) / rhs );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the current size/dimension of the row.
//
// \return The size of the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,true,RAs...>::size() const noexcept
{
   return matrix_.columns();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the minimum capacity of the row.
//
// \return The minimum capacity of the row.
//
// This function returns the minimum capacity of the row, which corresponds to the current size
// plus padding.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,true,RAs...>::spacing() const noexcept
{
   return matrix_.spacing();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the maximum capacity of the dense row.
//
// \return The maximum capacity of the dense row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,true,RAs...>::capacity() const noexcept
{
   return matrix_.capacity( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the number of non-zero elements in the row.
//
// \return The number of non-zero elements in the row.
//
// Note that the number of non-zero elements is always less than or equal to the current number
// of columns of the matrix containing the row.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline size_t RowImpl<MT,false,true,true,RAs...>::nonZeros() const
{
   return matrix_.nonZeros( row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Reset to the default initial values.
//
// \return void
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline void RowImpl<MT,false,true,true,RAs...>::reset()
{
   matrix_.reset( row() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  NUMERIC FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Scaling of the row by the scalar value \a scalar (\f$ \vec{a}=\vec{b}*s \f$).
//
// \param scalar The scalar value for the row scaling.
// \return Reference to the dense row.
//
// This function scales the row by applying the given scalar value \a scalar to each element
// of the row. For built-in and \c complex data types it has the same effect as using the
// multiplication assignment operator. Note that the function cannot be used to scale a row
// on a lower or upper unitriangular matrix. The attempt to scale such a row results in a
// compile time error!
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the scalar value
inline RowImpl<MT,false,true,true,RAs...>&
   RowImpl<MT,false,true,true,RAs...>::scale( const Other& scalar )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   const size_t ibegin( ( IsLower<MT>::value )
                        ?( ( IsStrictlyLower<MT>::value )
                           ?( row()+1UL )
                           :( row() ) )
                        :( 0UL ) );
   const size_t iend  ( ( IsUpper<MT>::value )
                        ?( ( IsStrictlyUpper<MT>::value )
                           ?( row() )
                           :( row()+1UL ) )
                        :( size() ) );

   for( size_t i=ibegin; i<iend; ++i ) {
      matrix_(i,row()) *= scalar;
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TEMPLATE EVALUATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the foreign expression
inline bool RowImpl<MT,false,true,true,RAs...>::canAlias( const Other* alias ) const noexcept
{
   return matrix_.isAliased( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given dense row \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename MT2      // Data type of the foreign dense row
        , bool SO2          // Storage order of the foreign dense row
        , bool SF2          // Symmetry flag of the foreign dense row
        , size_t... RAs2 >  // Compile time row arguments of the foreign dense row
inline bool
   RowImpl<MT,false,true,true,RAs...>::canAlias( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept
{
   return matrix_.isAliased( &alias->matrix_ ) && ( row() == alias->row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename Other >  // Data type of the foreign expression
inline bool RowImpl<MT,false,true,true,RAs...>::isAliased( const Other* alias ) const noexcept
{
   return matrix_.isAliased( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given dense row \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , size_t... RAs >   // Compile time row arguments
template< typename MT2      // Data type of the foreign dense row
        , bool SO2          // Storage order of the foreign dense row
        , bool SF2          // Symmetry flag of the foreign dense row
        , size_t... RAs2 >  // Compile time row arguments of the foreign dense row
inline bool
   RowImpl<MT,false,true,true,RAs...>::isAliased( const RowImpl<MT2,SO2,true,SF2,RAs2...>* alias ) const noexcept
{
   return matrix_.isAliased( &alias->matrix_ ) && ( row() == alias->row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is properly aligned in memory.
//
// \return \a true in case the dense row is aligned, \a false if not.
//
// This function returns whether the dense row is guaranteed to be properly aligned in memory,
// i.e. whether the beginning and the end of the dense row are guaranteed to conform to the
// alignment restrictions of the element type \a Type.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline bool RowImpl<MT,false,true,true,RAs...>::isAligned() const noexcept
{
   return matrix_.isAligned();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can be used in SMP assignments.
//
// \return \a true in case the dense row can be used in SMP assignments, \a false if not.
//
// This function returns whether the dense row can be used in SMP assignments. In contrast to
// the \a smpAssignable member enumeration, which is based solely on compile time information,
// this function additionally provides runtime information (as for instance the current size
// of the dense row).
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
inline bool RowImpl<MT,false,true,true,RAs...>::canSMPAssign() const noexcept
{
   return ( size() > SMP_DVECASSIGN_THRESHOLD );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Load of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return The loaded SIMD element.
//
// This function performs a load of a specific SIMD element of the dense row. The index
// must be smaller than the number of matrix columns. This function must \b NOT be called
// explicitly! It is used internally for the performance optimized evaluation of expression
// templates. Calling this function explicitly might result in erroneous results and/or in
// compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE typename RowImpl<MT,false,true,true,RAs...>::SIMDType
   RowImpl<MT,false,true,true,RAs...>::load( size_t index ) const noexcept
{
   return matrix_.load( index, row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Aligned load of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return The loaded SIMD element.
//
// This function performs an aligned load of a specific SIMD element of the dense row.
// The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE typename RowImpl<MT,false,true,true,RAs...>::SIMDType
   RowImpl<MT,false,true,true,RAs...>::loada( size_t index ) const noexcept
{
   return matrix_.loada( index, row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Unaligned load of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return The loaded SIMD element.
//
// This function performs an unaligned load of a specific SIMD element of the dense row.
// The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE typename RowImpl<MT,false,true,true,RAs...>::SIMDType
   RowImpl<MT,false,true,true,RAs...>::loadu( size_t index ) const noexcept
{
   return matrix_.loadu( index, row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs a store a specific SIMD element of the dense row. The index
// must be smaller than the number of matrix columns. This function must \b NOT be called
// explicitly! It is used internally for the performance optimized evaluation of expression
// templates. Calling this function explicitly might result in erroneous results and/or in
// compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,false,true,true,RAs...>::store( size_t index, const SIMDType& value ) noexcept
{
   matrix_.store( index, row(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Aligned store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs an aligned store a specific SIMD element of the dense row. The
// index must be smaller than the number of matrix columns. This function must \b NOT be
// called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,false,true,true,RAs...>::storea( size_t index, const SIMDType& value ) noexcept
{
   matrix_.storea( index, row(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Unligned store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs an unaligned store a specific SIMD element of the dense row.
// The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,false,true,true,RAs...>::storeu( size_t index, const SIMDType& value ) noexcept
{
   matrix_.storeu( index, row(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Aligned, non-temporal store of a SIMD element of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \param value The SIMD element to be stored.
// \return void
//
// This function performs an aligned, non-temporal store a specific SIMD element of the dense
// row. The index must be smaller than the number of matrix columns. This function must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
BLAZE_ALWAYS_INLINE void
   RowImpl<MT,false,true,true,RAs...>::stream( size_t index, const SIMDType& value ) noexcept
{
   matrix_.stream( index, row(), value );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t ipos( (~rhs).size() & size_t(-2) );
   for( size_t i=0UL; i<ipos; i+=2UL ) {
      matrix_(i    ,row()) = (~rhs)[i    ];
      matrix_(i+1UL,row()) = (~rhs)[i+1UL];
   }
   if( ipos < (~rhs).size() )
      matrix_(ipos,row()) = (~rhs)[ipos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t rows( size() );

   const size_t ipos( ( remainder )?( rows & size_t(-SIMDSIZE) ):( rows ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( rows - ( rows % (SIMDSIZE) ) ) == ipos, "Invalid end calculation" );

   size_t i( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   if( useStreaming && rows > ( cacheSize/( sizeof(ElementType) * 3UL ) ) && !(~rhs).isAliased( &matrix_ ) )
   {
      for( ; i<ipos; i+=SIMDSIZE ) {
         left.stream( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      }
      for( ; remainder && i<rows; ++i ) {
         *left = *right; ++left; ++right;
      }
   }
   else
   {
      for( ; (i+SIMDSIZE*3UL) < ipos; i+=SIMDSIZE*4UL ) {
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      }
      for( ; i<ipos; i+=SIMDSIZE ) {
         left.store( right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      }
      for( ; remainder && i<rows; ++i ) {
         *left = *right; ++left; ++right;
      }
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,true,RAs...>::assign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(element->index(),row()) = element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedAddAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t ipos( (~rhs).size() & size_t(-2) );
   for( size_t i=0UL; i<ipos; i+=2UL ) {
      matrix_(i    ,row()) += (~rhs)[i    ];
      matrix_(i+1UL,row()) += (~rhs)[i+1UL];
   }
   if( ipos < (~rhs).size() )
      matrix_(ipos,row()) += (~rhs)[ipos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedAddAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t rows( size() );

   const size_t ipos( ( remainder )?( rows & size_t(-SIMDSIZE) ):( rows ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( rows - ( rows % (SIMDSIZE) ) ) == ipos, "Invalid end calculation" );

   size_t i( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (i+SIMDSIZE*3UL) < ipos; i+=SIMDSIZE*4UL ) {
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; i<ipos; i+=SIMDSIZE ) {
      left.store( left.load() + right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; remainder && i<rows; ++i ) {
      *left += *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,true,RAs...>::addAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(element->index(),row()) += element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedSubAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t ipos( (~rhs).size() & size_t(-2) );
   for( size_t i=0UL; i<ipos; i+=2UL ) {
      matrix_(i    ,row()) -= (~rhs)[i    ];
      matrix_(i+1UL,row()) -= (~rhs)[i+1UL];
   }
   if( ipos < (~rhs).size() )
      matrix_(ipos,row()) -= (~rhs)[ipos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedSubAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t rows( size() );

   const size_t ipos( ( remainder )?( rows & size_t(-SIMDSIZE) ):( rows ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( rows - ( rows % (SIMDSIZE) ) ) == ipos, "Invalid end calculation" );

   size_t i( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (i+SIMDSIZE*3UL) < ipos; i+=SIMDSIZE*4UL ) {
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; i<ipos; i+=SIMDSIZE ) {
      left.store( left.load() - right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; remainder && i<rows; ++i ) {
      *left -= *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,true,RAs...>::subAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(element->index(),row()) -= element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedMultAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t ipos( (~rhs).size() & size_t(-2) );
   for( size_t i=0UL; i<ipos; i+=2UL ) {
      matrix_(i    ,row()) *= (~rhs)[i    ];
      matrix_(i+1UL,row()) *= (~rhs)[i+1UL];
   }
   if( ipos < (~rhs).size() )
      matrix_(ipos,row()) *= (~rhs)[ipos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedMultAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   constexpr bool remainder( !IsPadded<MT>::value || !IsPadded<VT>::value );

   const size_t rows( size() );

   const size_t ipos( ( remainder )?( rows & size_t(-SIMDSIZE) ):( rows ) );
   BLAZE_INTERNAL_ASSERT( !remainder || ( rows - ( rows % (SIMDSIZE) ) ) == ipos, "Invalid end calculation" );

   size_t i( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (i+SIMDSIZE*3UL) < ipos; i+=SIMDSIZE*4UL ) {
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; i<ipos; i+=SIMDSIZE ) {
      left.store( left.load() * right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; remainder && i<rows; ++i ) {
      *left *= *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side sparse vector
inline void RowImpl<MT,false,true,true,RAs...>::multAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const ResultType tmp( serial( *this ) );

   reset();

   for( ConstIterator_<VT> element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(element->index(),row()) = tmp[element->index()] * element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the division assignment of a dense vector.
//
// \param rhs The right-hand side dense vector divisor.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline DisableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedDivAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::divAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t ipos( (~rhs).size() & size_t(-2) );
   for( size_t i=0UL; i<ipos; i+=2UL ) {
      matrix_(i    ,row()) /= (~rhs)[i    ];
      matrix_(i+1UL,row()) /= (~rhs)[i+1UL];
   }
   if( ipos < (~rhs).size() )
      matrix_(ipos,row()) /= (~rhs)[ipos];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief SIMD optimized implementation of the division assignment of a dense vector.
//
// \param rhs The right-hand side dense vector divisor.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT      // Type of the dense matrix
        , size_t... RAs >  // Compile time row arguments
template< typename VT >    // Type of the right-hand side dense vector
inline EnableIf_< typename RowImpl<MT,false,true,true,RAs...>::BLAZE_TEMPLATE VectorizedDivAssign<VT> >
   RowImpl<MT,false,true,true,RAs...>::divAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t rows( size() );

   const size_t ipos( rows & size_t(-SIMDSIZE) );
   BLAZE_INTERNAL_ASSERT( ( rows - ( rows % (SIMDSIZE) ) ) == ipos, "Invalid end calculation" );

   size_t i( 0UL );
   Iterator left( begin() );
   ConstIterator_<VT> right( (~rhs).begin() );

   for( ; (i+SIMDSIZE*3UL) < ipos; i+=SIMDSIZE*4UL ) {
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; i<ipos; i+=SIMDSIZE ) {
      left.store( left.load() / right.load() ); left += SIMDSIZE; right += SIMDSIZE;
   }
   for( ; i<rows; ++i ) {
      *left /= *right; ++left; ++right;
   }
}
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
