//=================================================================================================
/*!
//  \file blaze/math/expressions/SMatTSMatMultExpr.h
//  \brief Header file for the sparse matrix/transpose sparse matrix multiplication expression
//
//  Copyright (C) 2011 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. This library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as published by the
//  Free Software Foundation; either version 3, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with a special
//  exception for linking and compiling against the Blaze library, the so-called "runtime
//  exception"; see the file COPYING. If not, see http://www.gnu.org/licenses/.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_EXPRESSIONS_SMATTSMATMULTEXPR_H_
#define _BLAZE_MATH_EXPRESSIONS_SMATTSMATMULTEXPR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <stdexcept>
#include <boost/type_traits/remove_reference.hpp>
#include <blaze/math/constraints/SparseMatrix.h>
#include <blaze/math/constraints/StorageOrder.h>
#include <blaze/math/expressions/Computation.h>
#include <blaze/math/expressions/Expression.h>
#include <blaze/math/expressions/Forward.h>
#include <blaze/math/expressions/SparseMatrix.h>
#include <blaze/math/traits/MultTrait.h>
#include <blaze/math/traits/SMatDVecMultExprTrait.h>
#include <blaze/math/traits/SMatSVecMultExprTrait.h>
#include <blaze/math/traits/TDVecSMatMultExprTrait.h>
#include <blaze/math/traits/TDVecTSMatMultExprTrait.h>
#include <blaze/math/traits/TSMatDVecMultExprTrait.h>
#include <blaze/math/traits/TSMatSVecMultExprTrait.h>
#include <blaze/math/traits/TSVecSMatMultExprTrait.h>
#include <blaze/math/traits/TSVecTSMatMultExprTrait.h>
#include <blaze/math/typetraits/IsColumnMajorMatrix.h>
#include <blaze/math/typetraits/IsComputation.h>
#include <blaze/math/typetraits/IsDenseVector.h>
#include <blaze/math/typetraits/IsExpression.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/math/typetraits/IsSparseMatrix.h>
#include <blaze/math/typetraits/IsSparseVector.h>
#include <blaze/math/typetraits/IsTransposeVector.h>
#include <blaze/math/typetraits/RequiresEvaluation.h>
#include <blaze/util/Assert.h>
#include <blaze/util/InvalidType.h>
#include <blaze/util/logging/FunctionTrace.h>
#include <blaze/util/SelectType.h>
#include <blaze/util/Types.h>


namespace blaze {

//=================================================================================================
//
//  CLASS SMATTSMATMULTEXPR
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Expression object for sparse matrix-transpose sparse matrix multiplications.
// \ingroup sparse_matrix_expression
//
// The SMatTSMatMultExpr class represents the compile time expression for multiplications between
// a row-major and a column-major sparse matrix.
*/
template< typename MT1    // Type of the left-hand side sparse matrix
        , typename MT2 >  // Type of the right-hand side sparse matrix
class SMatTSMatMultExpr : public SparseMatrix< SMatTSMatMultExpr<MT1,MT2>, false >
                        , private Expression
                        , private Computation
{
 private:
   //**Type definitions****************************************************************************
   typedef typename MT1::ResultType     RT1;  //!< Result type of the left-hand side sparse matrix expression.
   typedef typename MT2::ResultType     RT2;  //!< Result type of the right-hand side sparse matrix expression.
   typedef typename MT1::CompositeType  CT1;  //!< Composite type of the left-hand side sparse matrix expression.
   typedef typename MT2::CompositeType  CT2;  //!< Composite type of the right-hand side sparse matrix expression.
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   typedef SMatTSMatMultExpr<MT1,MT2>          This;           //!< Type of this SMatTSMatMultExpr instance.
   typedef typename MultTrait<RT1,RT2>::Type   ResultType;     //!< Result type for expression template evaluations.
   typedef typename ResultType::OppositeType   OppositeType;   //!< Result type with opposite storage order for expression template evaluations.
   typedef typename ResultType::TransposeType  TransposeType;  //!< Transpose type for expression template evaluations.
   typedef typename ResultType::ElementType    ElementType;    //!< Resulting element type.
   typedef const ElementType                   ReturnType;     //!< Return type for expression template evaluations.
   typedef const ResultType                    CompositeType;  //!< Data type for composite expression templates.

   //! Composite type of the left-hand side sparse matrix expression.
   typedef typename SelectType< IsExpression<MT1>::value, const MT1, const MT1& >::Type  LeftOperand;

   //! Composite type of the right-hand side sparse matrix expression.
   typedef typename SelectType< IsExpression<MT2>::value, const MT2, const MT2& >::Type  RightOperand;
   //**********************************************************************************************

   //**Constructor*********************************************************************************
   /*!\brief Constructor for the SMatTSMatMultExpr class.
   //
   // \param lhs The left-hand side sparse matrix operand of the multiplication expression.
   // \param rhs The right-hand side sparse matrix operand of the multiplication expression.
   */
   explicit inline SMatTSMatMultExpr( const MT1& lhs, const MT2& rhs )
      : lhs_( lhs )  // Left-hand side sparse matrix of the multiplication expression
      , rhs_( rhs )  // Right-hand side sparse matrix of the multiplication expression
   {
      BLAZE_INTERNAL_ASSERT( lhs.columns() == rhs.rows(), "Invalid matrix sizes" );
   }
   //**********************************************************************************************

   //**Access operator*****************************************************************************
   /*!\brief 2D-access to the matrix elements.
   //
   // \param i Access index for the row. The index has to be in the range \f$[0..M-1]\f$.
   // \param j Access index for the column. The index has to be in the range \f$[0..N-1]\f$.
   // \return The resulting value.
   */
   inline ReturnType operator()( size_t i, size_t j ) const {
      BLAZE_INTERNAL_ASSERT( i < lhs_.rows()   , "Invalid row access index"    );
      BLAZE_INTERNAL_ASSERT( j < rhs_.columns(), "Invalid column access index" );

      typedef typename boost::remove_reference<CT1>::type::ConstIterator  LeftIterator;
      typedef typename boost::remove_reference<CT2>::type::ConstIterator  RightIterator;

      ElementType tmp = ElementType();

      // Early exit
      if( lhs_.columns() == 0UL )
         return tmp;

      // Fast computation in case both the left-hand side and the right-hand side
      // sparse matrices directly provide iterators
      if( !RequiresEvaluation<MT1>::value && !RequiresEvaluation<MT2>::value )
      {
         // Evaluation of the left-hand side sparse matrix operand
         CT1 A( lhs_ );

         const LeftIterator lend( A.end(i) );
         LeftIterator lelem( A.begin(i) );

         // Evaluation of the right-hand side sparse matrix operand
         CT2 B( rhs_ );

         const RightIterator rend( B.end(j) );
         RightIterator relem( B.begin(j) );

         // Early exit in case row i or column j are empty
         if( lelem == lend || relem == rend )
            return tmp;

         // Calculating element (i,j)
         while( true ) {
            if( lelem->index() < relem->index() ) {
               ++lelem;
               if( lelem == lend ) break;
            }
            else if( relem->index() < lelem->index() ) {
               ++relem;
               if( relem == rend ) break;
            }
            else {
               tmp = lelem->value() * relem->value();
               ++lelem;
               ++relem;
               break;
            }
         }

         if( lelem != lend && relem != rend )
         {
            while( true ) {
               if( lelem->index() < relem->index() ) {
                  ++lelem;
                  if( lelem == lend ) break;
               }
               else if( relem->index() < lelem->index() ) {
                  ++relem;
                  if( relem == rend ) break;
               }
               else {
                  tmp += lelem->value() * relem->value();
                  ++lelem;
                  if( lelem == lend ) break;
                  ++relem;
                  if( relem == rend ) break;
               }
            }
         }
      }

      // Optimized computation in case the left-hand side sparse matrix directly provides iterators
      else if( !RequiresEvaluation<MT1>::value )
      {
         // Evaluation of the left-hand side sparse matrix operand
         CT1 A( lhs_ );

         const LeftIterator end( A.end(i) );
         LeftIterator element( A.begin(i) );

         // Early exit in case row i is empty
         if( element == end )
            return tmp;

         // Calculating element (i,j)
         tmp = element->value() * rhs_(element->index(),j);
         ++element;
         for( ; element!=end; ++element )
            tmp += element->value() * rhs_(element->index(),j);
      }

      // Optimized computation in case the right-hand side sparse matrix directly provides iterators
      else if( !RequiresEvaluation<MT2>::value )
      {
         // Evaluation of the right-hand side sparse matrix operand
         CT2 B( rhs_ );

         const RightIterator end( B.end(j) );
         RightIterator element( B.begin(j) );

         // Early exit in case row i is empty
         if( element == end )
            return tmp;

         // Calculating element (i,j)
         tmp = lhs_(i,element->index()) * element->value();
         ++element;
         for( ; element!=end; ++element )
            tmp += lhs_(i,element->index()) * element->value();
      }

      // Default computation in case both sparse matrices don't provide iterators
      else {
         tmp = lhs_(i,0UL) * rhs_(0UL,j);
         for( size_t k=1UL; k<lhs_.columns(); ++k ) {
            tmp += lhs_(i,k) * rhs_(k,j);
         }
      }

      return tmp;
   }
   //**********************************************************************************************

   //**Rows function*******************************************************************************
   /*!\brief Returns the current number of rows of the matrix.
   //
   // \return The number of rows of the matrix.
   */
   inline size_t rows() const {
      return lhs_.rows();
   }
   //**********************************************************************************************

   //**Columns function****************************************************************************
   /*!\brief Returns the current number of columns of the matrix.
   //
   // \return The number of columns of the matrix.
   */
   inline size_t columns() const {
      return rhs_.columns();
   }
   //**********************************************************************************************

   //**NonZeros function***************************************************************************
   /*!\brief Returns the number of non-zero elements in the sparse matrix.
   //
   // \return The number of non-zero elements in the sparse matrix.
   */
   inline size_t nonZeros() const {
      return 0UL;
   }
   //**********************************************************************************************

   //**NonZeros function***************************************************************************
   /*!\brief Returns the number of non-zero elements in the specified row.
   //
   // \param i The index of the row.
   // \return The number of non-zero elements of row \a i.
   */
   inline size_t nonZeros( size_t i ) const {
      return 0UL;
   }
   //**********************************************************************************************

   //**Left operand access*************************************************************************
   /*!\brief Returns the left-hand side sparse matrix operand.
   //
   // \return The left-hand side sparse matrix operand.
   */
   inline LeftOperand leftOperand() const {
      return lhs_;
   }
   //**********************************************************************************************

   //**Right operand access************************************************************************
   /*!\brief Returns the right-hand side transpose sparse matrix operand.
   //
   // \return The right-hand side transpose sparse matrix operand.
   */
   inline RightOperand rightOperand() const {
      return rhs_;
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression can alias with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case the expression can alias, \a false otherwise.
   */
   template< typename T >
   inline bool canAlias( const T* alias ) const {
      return ( lhs_.isAliased( alias ) || rhs_.isAliased( alias ) );
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression is aliased with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case an alias effect is detected, \a false otherwise.
   */
   template< typename T >
   inline bool isAliased( const T* alias ) const {
      return ( lhs_.isAliased( alias ) || rhs_.isAliased( alias ) );
   }
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   LeftOperand  lhs_;  //!< Left-hand side sparse matrix of the multiplication expression.
   RightOperand rhs_;  //!< Right-hand side sparse matrix of the multiplication expression.
   //**********************************************************************************************

   //**Assignment to row-major dense matrices******************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a sparse matrix-transpose sparse matrix multiplication to a row-major
   //        dense matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side multiplication expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a sparse matrix-transpose
   // sparse matrix multiplication expression to a row-major dense matrix.
   */
   template< typename MT >  // Type of the target dense matrix
   friend inline void assign( DenseMatrix<MT,false>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( typename MT2::OppositeType );

      const typename MT2::OppositeType tmp( rhs.rhs_ );
      assign( ~lhs, rhs.lhs_ * tmp );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to column-major dense matrices***************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a sparse matrix-transpose sparse matrix multiplication to a
   //        column-major dense matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side multiplication expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a sparse matrix-transpose
   // sparse matrix multiplication expression to a column-major dense matrix.
   */
   template< typename MT >  // Type of the target dense matrix
   friend inline void assign( DenseMatrix<MT,true>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( typename MT1::OppositeType );

      const typename MT1::OppositeType tmp( rhs.lhs_ );
      assign( ~lhs, tmp * rhs.rhs_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to row-major sparse matrices*****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a sparse matrix-transpose sparse matrix multiplication to a row-major
   //        sparse matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side sparse matrix.
   // \param rhs The right-hand side multiplication expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a sparse matrix-transpose
   // sparse matrix multiplication expression to a row-major sparse matrix.
   */
   template< typename MT >  // Type of the target sparse matrix
   friend inline void assign( SparseMatrix<MT,false>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( typename MT2::OppositeType );

      const typename MT2::OppositeType tmp( rhs.rhs_ );
      assign( ~lhs, rhs.lhs_ * tmp );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to column-major sparse matrices**************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a sparse matrix-transpose sparse matrix multiplication to a
   //        column-major sparse matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side sparse matrix.
   // \param rhs The right-hand side multiplication expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a sparse matrix-transpose
   // sparse matrix multiplication expression to a column-major sparse matrix.
   */
   template< typename MT >  // Type of the target sparse matrix
   friend inline void assign( SparseMatrix<MT,true>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( typename MT1::OppositeType );

      const typename MT1::OppositeType tmp( rhs.lhs_ );
      assign( ~lhs, tmp * rhs.rhs_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to row-major dense matrices*********************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Addition assignment of a sparse matrix-transpose sparse matrix multiplication to a
   //        row-major dense matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side multiplication expression to be added.
   // \return void
   //
   // This function implements the performance optimized addition assignment of a sparse matrix-
   // transpose sparse matrix multiplication expression to a row-major dense matrix.
   */
   template< typename MT >  // Type of the target dense matrix
   friend inline void addAssign( DenseMatrix<MT,false>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( typename MT2::OppositeType );

      const typename MT2::OppositeType tmp( rhs.rhs_ );
      addAssign( ~lhs, rhs.lhs_ * tmp );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to column-major dense matrices******************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Addition assignment of a sparse matrix-transpose sparse matrix multiplication to a
   //        column-major dense matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side multiplication expression to be added.
   // \return void
   //
   // This function implements the performance optimized addition assignment of a sparse matrix-
   // transpose sparse matrix multiplication expression to a column-major dense matrix.
   */
   template< typename MT >  // Type of the target dense matrix
   friend inline void addAssign( DenseMatrix<MT,true>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( typename MT1::OppositeType );

      const typename MT1::OppositeType tmp( rhs.lhs_ );
      addAssign( ~lhs, tmp * rhs.rhs_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to sparse matrices******************************************************
   // No special implementation for the addition assignment to sparse matrices.
   //**********************************************************************************************

   //**Subtraction assignment to row-major dense matrices******************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Subtraction assignment of a sparse matrix-transpose sparse matrix multiplication
   //        to a row-major dense matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side multiplication expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized subtraction assignment of a sparse matrix-
   // transpose sparse matrix multiplication expression to a row-major dense matrix.
   */
   template< typename MT >  // Type of the target dense matrix
   friend inline void subAssign( DenseMatrix<MT,false>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( typename MT2::OppositeType );

      const typename MT2::OppositeType tmp( rhs.rhs_ );
      subAssign( ~lhs, rhs.lhs_ * tmp );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Subtraction assignment to column-major dense matrices***************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Subtraction assignment of a sparse matrix-transpose sparse matrix multiplication
   //        to a column-major dense matrix.
   // \ingroup sparse_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side multiplication expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized subtraction assignment of a sparse matrix-
   // transpose sparse matrix multiplication expression to a column-major dense matrix.
   */
   template< typename MT >  // Type of the target dense matrix
   friend inline void subAssign( DenseMatrix<MT,true>& lhs, const SMatTSMatMultExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( typename MT1::OppositeType );

      const typename MT1::OppositeType tmp( rhs.lhs_ );
      subAssign( ~lhs, tmp * rhs.rhs_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Subtraction assignment to sparse matrices***************************************************
   // No special implementation for the subtraction assignment to sparse matrices.
   //**********************************************************************************************

   //**Multiplication assignment to dense matrices*************************************************
   // No special implementation for the multiplication assignment to dense matrices.
   //**********************************************************************************************

   //**Multiplication assignment to sparse matrices************************************************
   // No special implementation for the multiplication assignment to sparse matrices.
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT1 );
   BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( MT1 );
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT2 );
   BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( MT2 );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL BINARY ARITHMETIC OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Multiplication operator for the multiplication of a row-major sparse matrix and a
//        column-major sparse matrix (\f$ A=B*C \f$).
// \ingroup sparse_matrix
//
// \param lhs The left-hand side sparse matrix for the matrix multiplication.
// \param rhs The right-hand side sparse matrix for the matrix multiplication.
// \return The product of the two matrices.
// \exception std::invalid_argument Matrix sizes do not match
//
// This operator represents the multiplication of a row-major sparse matrix and a column-major
// sparse matrix:

   \code
   using blaze::rowMajor;
   using blaze::columnMajor;

   blaze::CompressedMatrix<double,rowMajor> A, C;
   blaze::CompressedMatrix<double,columnMajor> B;
   // ... Resizing and initialization
   C = A * B;
   \endcode

// The operator returns an expression representing a sparse matrix of the higher-order element
// type of the two involved matrix element types \a T1::ElementType and \a T2::ElementType.
// Both matrix types \a T1 and \a T2 as well as the two element types \a T1::ElementType and
// \a T2::ElementType have to be supported by the MultTrait class template.\n
// In case the current number of rows and columns of the two given matrices don't match, a
// \a std::invalid_argument is thrown.
*/
template< typename T1    // Type of the left-hand side sparse matrix
        , typename T2 >  // Type of the right-hand side sparse matrix
inline const SMatTSMatMultExpr<T1,T2>
   operator*( const SparseMatrix<T1,false>& lhs, const SparseMatrix<T2,true>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).columns() != (~rhs).rows() )
      throw std::invalid_argument( "Matrix sizes do not match" );

   return SMatTSMatMultExpr<T1,T2>( ~lhs, ~rhs );
}
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename VT >
struct SMatDVecMultExprTrait< SMatTSMatMultExpr<MT1,MT2>, VT >
{
 public:
   //**********************************************************************************************
   typedef typename SelectType< IsSparseMatrix<MT1>::value && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value &&
                                IsDenseVector<VT>::value   && !IsTransposeVector<VT>::value
                              , typename SMatDVecMultExprTrait< MT1, typename TSMatDVecMultExprTrait<MT2,VT>::Type >::Type
                              , INVALID_TYPE >::Type  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename VT >
struct SMatSVecMultExprTrait< SMatTSMatMultExpr<MT1,MT2>, VT >
{
 public:
   //**********************************************************************************************
   typedef typename SelectType< IsSparseMatrix<MT1>::value && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value &&
                                IsSparseVector<VT>::value  && !IsTransposeVector<VT>::value
                              , typename SMatSVecMultExprTrait< MT1, typename TSMatSVecMultExprTrait<MT2,VT>::Type >::Type
                              , INVALID_TYPE >::Type  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename VT, typename MT1, typename MT2 >
struct TDVecSMatMultExprTrait< VT, SMatTSMatMultExpr<MT1,MT2> >
{
 public:
   //**********************************************************************************************
   typedef typename SelectType< IsDenseVector<VT>::value   && IsTransposeVector<VT>::value &&
                                IsSparseMatrix<MT1>::value && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value
                              , typename TDVecTSMatMultExprTrait< typename TDVecSMatMultExprTrait<VT,MT1>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename VT, typename MT1, typename MT2 >
struct TSVecSMatMultExprTrait< VT, SMatTSMatMultExpr<MT1,MT2> >
{
 public:
   //**********************************************************************************************
   typedef typename SelectType< IsSparseVector<VT>::value  && IsTransposeVector<VT>::value &&
                                IsSparseMatrix<MT1>::value && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value
                              , typename TDVecTSMatMultExprTrait< typename TDVecSMatMultExprTrait<VT,MT1>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
