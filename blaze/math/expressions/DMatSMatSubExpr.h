//=================================================================================================
/*!
//  \file blaze/math/expressions/DMatSMatSubExpr.h
//  \brief Header file for the dense matrix/sparse matrix subtraction expression
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

#ifndef _BLAZE_MATH_EXPRESSIONS_DMATSMATSUBEXPR_H_
#define _BLAZE_MATH_EXPRESSIONS_DMATSMATSUBEXPR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <stdexcept>
#include <blaze/math/constraints/DenseMatrix.h>
#include <blaze/math/constraints/SparseMatrix.h>
#include <blaze/math/constraints/StorageOrder.h>
#include <blaze/math/expressions/Computation.h>
#include <blaze/math/expressions/DenseMatrix.h>
#include <blaze/math/expressions/Expression.h>
#include <blaze/math/expressions/Forward.h>
#include <blaze/math/traits/AddExprTrait.h>
#include <blaze/math/traits/SubExprTrait.h>
#include <blaze/math/traits/SubTrait.h>
#include <blaze/math/typetraits/IsColumnMajorMatrix.h>
#include <blaze/math/typetraits/IsDenseMatrix.h>
#include <blaze/math/typetraits/IsExpression.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/math/typetraits/IsSparseMatrix.h>
#include <blaze/math/typetraits/IsTemporary.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Reference.h>
#include <blaze/util/logging/FunctionTrace.h>
#include <blaze/util/SelectType.h>
#include <blaze/util/Types.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DMATSMATSUBEXPR
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Expression object for dense matrix-sparse matrix subtractions.
// \ingroup dense_matrix_expression
//
// The DMatSMatSubExpr class represents the compile time expression for subtractions between
// a dense matrix and a sparse matrix with identical storage order.
*/
template< typename MT1  // Type of the left-hand side dense matrix
        , typename MT2  // Type of the right-hand side sparse matrix
        , bool SO >     // Storage order
class DMatSMatSubExpr : public DenseMatrix< DMatSMatSubExpr<MT1,MT2,SO>, SO >
                      , private Expression
                      , private Computation
{
 private:
   //**Type definitions****************************************************************************
   typedef typename MT1::ResultType  RT1;  //!< Result type of the left-hand side dense matrix expression.
   typedef typename MT2::ResultType  RT2;  //!< Result type of the right-hand side sparse matrix expression.
   typedef typename MT1::ReturnType  RN1;  //!< Return type of the left-hand side dense matrix expression.
   typedef typename MT2::ReturnType  RN2;  //!< Return type of the right-hand side sparse matrix expression.
   //**********************************************************************************************

   //**Return type evaluation**********************************************************************
   //! Compilation switch for the selection of the subscript operator return type.
   /*! The \a returnExpr compile time constant expression is a compilation switch for the
       selection of the \a ReturnType. If either matrix operand returns a temporary vector
       or matrix, \a returnExpr will be set to \a false and the subscript operator will
       return it's result by value. Otherwise \a returnExpr will be set to \a true and
       the subscript operator may return it's result as an expression. */
   enum { returnExpr = !IsTemporary<RN1>::value && !IsTemporary<RN2>::value };

   //! Expression return type for the subscript operator.
   typedef typename SubExprTrait<RN1,RN2>::Type  ExprReturnType;
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   typedef DMatSMatSubExpr<MT1,MT2,SO>         This;           //!< Type of this DMatSMatSubExpr instance.
   typedef typename SubTrait<RT1,RT2>::Type    ResultType;     //!< Result type for expression template evaluations.
   typedef typename ResultType::OppositeType   OppositeType;   //!< Result type with opposite storage order for expression template evaluations.
   typedef typename ResultType::TransposeType  TransposeType;  //!< Transpose type for expression template evaluations.
   typedef typename ResultType::ElementType    ElementType;    //!< Resulting element type.

   //! Return type for expression template evaluations.
   typedef const typename SelectType< returnExpr, ExprReturnType, ElementType >::Type  ReturnType;

   //! Data type for composite expression templates.
   typedef const ResultType  CompositeType;

   //! Composite type of the left-hand side dense matrix expression.
   typedef typename SelectType< IsExpression<MT1>::value, const MT1, const MT1& >::Type  LeftOperand;

   //! Composite type of the right-hand side sparse matrix expression.
   typedef typename SelectType< IsExpression<MT2>::value, const MT2, const MT2& >::Type  RightOperand;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum { vectorizable = 0 };
   //**********************************************************************************************

   //**Constructor*********************************************************************************
   /*!\brief Constructor for the DMatSMatSubExpr class.
   //
   // \param lhs The left-hand side dense matrix operand of the subtraction expression.
   // \param rhs The right-hand side sparse matrix operand of the subtraction expression.
   */
   explicit inline DMatSMatSubExpr( const MT1& lhs, const MT2& rhs )
      : lhs_( lhs )  // Left-hand side dense matrix of the subtraction expression
      , rhs_( rhs )  // Right-hand side sparse matrix of the subtraction expression
   {
      BLAZE_INTERNAL_ASSERT( lhs.rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( lhs.columns() == rhs.columns(), "Invalid number of columns" );
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
      BLAZE_INTERNAL_ASSERT( j < lhs_.columns(), "Invalid column access index" );
      return lhs_(i,j) - rhs_(i,j);
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
      return lhs_.columns();
   }
   //**********************************************************************************************

   //**Left operand access*************************************************************************
   /*!\brief Returns the left-hand side dense matrix operand.
   //
   // \return The left-hand side dense matrix operand.
   */
   inline LeftOperand leftOperand() const {
      return lhs_;
   }
   //**********************************************************************************************

   //**Right operand access************************************************************************
   /*!\brief Returns the right-hand side dense matrix operand.
   //
   // \return The right-hand side dense matrix operand.
   */
   inline RightOperand rightOperand() const {
      return rhs_;
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression can alias with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case the expression can alias effect is detected, \a false otherwise.
   */
   template< typename T >
   inline bool canAlias( const T* alias ) const {
      return ( IsExpression<MT1>::value && lhs_.canAlias( alias ) ) ||
             ( rhs_.canAlias( alias ) );
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
   LeftOperand  lhs_;  //!< Left-hand side dense matrix of the subtraction expression.
   RightOperand rhs_;  //!< Right-hand side sparse matrix of the subtraction expression.
   //**********************************************************************************************

   //**Assignment to dense matrices****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense matrix-sparse matrix subtraction to a dense matrix.
   // \ingroup dense_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side subtraction expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense matrix-sparse
   // matrix subtraction expression to a dense matrix.
   */
   template< typename MT  // Type of the target dense matrix
           , bool SO2 >   // Storage order of the target dense matrix
   friend inline void assign( DenseMatrix<MT,SO2>& lhs, const DMatSMatSubExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      if( !IsExpression<MT1>::value && (~lhs).isAliased( &rhs.lhs_ ) ) {
         subAssign( ~lhs, rhs.rhs_ );
      }
      else {
         assign   ( ~lhs, rhs.lhs_ );
         subAssign( ~lhs, rhs.rhs_ );
      }
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to sparse matrices***************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense matrix-sparse matrix subtraction to a sparse matrix.
   // \ingroup dense_matrix
   //
   // \param lhs The target left-hand side sparse matrix.
   // \param rhs The right-hand side subtraction expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense matrix-sparse
   // matrix subtraction expression to a sparse matrix.
   */
   template< typename MT  // Type of the target sparse matrix
           , bool SO2 >   // Storage order of the target sparse matrix
   friend inline void assign( SparseMatrix<MT,SO2>& lhs, const DMatSMatSubExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      typedef typename SelectType< SO == SO2, ResultType, OppositeType >::Type  TmpType;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE( ResultType );
      BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE( OppositeType );
      BLAZE_CONSTRAINT_MUST_BE_MATRIX_WITH_STORAGE_ORDER( ResultType, SO );
      BLAZE_CONSTRAINT_MUST_BE_MATRIX_WITH_STORAGE_ORDER( OppositeType, !SO );
      BLAZE_CONSTRAINT_MATRICES_MUST_HAVE_SAME_STORAGE_ORDER( MT, TmpType );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( typename TmpType::CompositeType );

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      const TmpType tmp( rhs );
      assign( ~lhs, tmp );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to dense matrices*******************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Addition assignment of a dense matrix-sparse matrix subtraction to a dense matrix.
   // \ingroup dense_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side subtraction expression to be added.
   // \return void
   //
   // This function implements the performance optimized addition assignment of a dense matrix-
   // sparse matrix subtraction expression to a dense matrix.
   */
   template< typename MT  // Type of the target dense matrix
           , bool SO2 >   // Storage order of the target dense matrix
   friend inline void addAssign( DenseMatrix<MT,SO2>& lhs, const DMatSMatSubExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      addAssign( ~lhs, rhs.lhs_ );
      subAssign( ~lhs, rhs.rhs_ );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to sparse matrices******************************************************
   // No special implementation for the addition assignment to sparse matrices.
   //**********************************************************************************************

   //**Subtraction assignment to dense matrices****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Subtraction assignment of a dense matrix-sparse matrix subtraction to a dense matrix.
   // \ingroup dense_matrix
   //
   // \param lhs The target left-hand side dense matrix.
   // \param rhs The right-hand side subtraction expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized subtraction assignment of a dense matrix-
   // sparse matrix subtraction expression to a dense matrix.
   */
   template< typename MT  // Type of the target dense matrix
           , bool SO2 >   // Storage order of the target dense matrix
   friend inline void subAssign( DenseMatrix<MT,SO2>& lhs, const DMatSMatSubExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).rows()    == rhs.rows()   , "Invalid number of rows"    );
      BLAZE_INTERNAL_ASSERT( (~lhs).columns() == rhs.columns(), "Invalid number of columns" );

      subAssign( ~lhs, rhs.lhs_ );
      addAssign( ~lhs, rhs.rhs_ );
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
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE ( MT1 );
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT2 );
   BLAZE_CONSTRAINT_MATRICES_MUST_HAVE_SAME_STORAGE_ORDER( MT1, MT2 );
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
/*!\brief Subtraction operator for the subtraction of a dense matrix and a sparse matrix with
//        identical storage order (\f$ A=B+C \f$).
// \ingroup dense_matrix
//
// \param lhs The left-hand side dense matrix for the matrix subtraction.
// \param rhs The right-hand side sparse matrix to be subtracted from the dense matrix.
// \return The difference of the two matrices.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the subtraction of a dense matrix and a sparse matrix with identical
// storage order:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::CompressedMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A - B;
   \endcode

// The operator returns an expression representing a dense matrix of the higher-order element
// type of the two involved matrix element types \a T1::ElementType and \a T2::ElementType.
// Both matrix types \a T1 and \a T2 as well as the two element types \a T1::ElementType and
// \a T2::ElementType have to be supported by the SubTrait class template.\n
// In case the current sizes of the two given matrices don't match, a \a std::invalid_argument
// is thrown.
*/
template< typename T1  // Type of the left-hand side dense matrix
        , typename T2  // Type of the right-hand side sparse matrix
        , bool SO >    // Storage order
inline const DMatSMatSubExpr<T1,T2,SO>
   operator-( const DenseMatrix<T1,SO>& lhs, const SparseMatrix<T2,SO>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != (~rhs).rows() || (~lhs).columns() != (~rhs).columns() )
      throw std::invalid_argument( "Matrix sizes do not match" );

   return DMatSMatSubExpr<T1,T2,SO>( ~lhs, ~rhs );
}
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING BINARY ARITHMETIC OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition operator for the addition of a dense matrix-sparse matrix subtraction
//        expression and a dense matrix (\f$ A=(B-C)+D \f$).
// \ingroup dense_matrix
//
// \param lhs The left-hand side dense matrix-sparse matrix subtraction.
// \param rhs The right-hand side dense matrix.
// \return The sum of the two matrices.
//
// This operator implements a performance optimized treatment of the addition of a dense
// matrix-sparse matrix subtraction expression to a dense matrix.
*/
template< typename T1  // Type of the dense matrix of the left-hand side expression
        , typename T2  // Type of the sparse matrix of the left-hand side expression
        , bool SO1     // Storage order of the left-hand side expression
        , typename T3  // Type of the right-hand side dense matrix
        , bool SO2 >   // Storage order of the right-hand side dense matrix
inline const typename AddExprTrait< DMatSMatSubExpr<T1,T2,SO1>, T3 >::Type
   operator+( const DMatSMatSubExpr<T1,T2,SO1>& lhs, const DenseMatrix<T3,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   return ( lhs.leftOperand() + (~rhs) ) - lhs.rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction operator for the subtraction of a dense matrix-sparse matrix subtraction
//        expression and a dense matrix (\f$ A=(B-C)-D \f$).
// \ingroup dense_matrix
//
// \param lhs The left-hand side dense matrix-sparse matrix subtraction.
// \param rhs The right-hand side dense matrix.
// \return The difference of the two matrices.
//
// This operator implements a performance optimized treatment of the subtraction of a dense
// matrix-sparse matrix subtraction expression and a dense matrix.
*/
template< typename T1  // Type of the dense matrix of the left-hand side expression
        , typename T2  // Type of the sparse matrix of the left-hand side expression
        , bool SO1     // Storage order of the left-hand side expression
        , typename T3  // Type of the right-hand side dense matrix
        , bool SO2 >   // Storage order of the right-hand side dense matrix
inline const typename SubExprTrait< DMatSMatSubExpr<T1,T2,SO1>, T3 >::Type
   operator-( const DMatSMatSubExpr<T1,T2,SO1>& lhs, const DenseMatrix<T3,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   return ( lhs.leftOperand() - (~rhs) ) - lhs.rightOperand();
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct DMatDMatAddExprTrait< DMatSMatSubExpr<MT1,MT2,false>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsRowMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsRowMajorMatrix<MT3>::value
                              , typename DMatSMatSubExprTrait< typename DMatDMatAddExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct DMatTDMatAddExprTrait< DMatSMatSubExpr<MT1,MT2,false>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsRowMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsColumnMajorMatrix<MT3>::value
                              , typename DMatSMatSubExprTrait< typename DMatTDMatAddExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct TDMatDMatAddExprTrait< DMatSMatSubExpr<MT1,MT2,true>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsColumnMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsRowMajorMatrix<MT3>::value
                              , typename DMatTSMatSubExprTrait< typename TDMatDMatAddExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct TDMatTDMatAddExprTrait< DMatSMatSubExpr<MT1,MT2,true>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsColumnMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsColumnMajorMatrix<MT3>::value
                              , typename TDMatTSMatSubExprTrait< typename TDMatTDMatAddExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct DMatDMatSubExprTrait< DMatSMatSubExpr<MT1,MT2,false>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsRowMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsRowMajorMatrix<MT3>::value
                              , typename DMatSMatSubExprTrait< typename DMatDMatSubExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct DMatTDMatSubExprTrait< DMatSMatSubExpr<MT1,MT2,false>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsRowMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsColumnMajorMatrix<MT3>::value
                              , typename DMatSMatSubExprTrait< typename DMatTDMatSubExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct TDMatDMatSubExprTrait< DMatSMatSubExpr<MT1,MT2,true>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsColumnMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsRowMajorMatrix<MT3>::value
                              , typename DMatTSMatSubExprTrait< typename TDMatDMatSubExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT1, typename MT2, typename MT3 >
struct TDMatTDMatSubExprTrait< DMatSMatSubExpr<MT1,MT2,true>, MT3 >
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsDenseMatrix<MT1>::value  && IsColumnMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsColumnMajorMatrix<MT2>::value &&
                                IsDenseMatrix<MT3>::value  && IsColumnMajorMatrix<MT3>::value
                              , typename TDMatTSMatSubExprTrait< typename TDMatTDMatSubExprTrait<MT1,MT3>::Type, MT2 >::Type
                              , INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
