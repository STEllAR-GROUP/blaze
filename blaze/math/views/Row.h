//=================================================================================================
/*!
//  \file blaze/math/views/Row.h
//  \brief Header file for the implementation of the Row view
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

#ifndef _BLAZE_MATH_VIEWS_ROW_H_
#define _BLAZE_MATH_VIEWS_ROW_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/expressions/DeclExpr.h>
#include <blaze/math/expressions/MatEvalExpr.h>
#include <blaze/math/expressions/MatMapExpr.h>
#include <blaze/math/expressions/MatMatAddExpr.h>
#include <blaze/math/expressions/MatMatMapExpr.h>
#include <blaze/math/expressions/MatMatMultExpr.h>
#include <blaze/math/expressions/MatMatSubExpr.h>
#include <blaze/math/expressions/Matrix.h>
#include <blaze/math/expressions/MatScalarDivExpr.h>
#include <blaze/math/expressions/MatScalarMultExpr.h>
#include <blaze/math/expressions/MatSerialExpr.h>
#include <blaze/math/expressions/MatTransExpr.h>
#include <blaze/math/expressions/SchurExpr.h>
#include <blaze/math/expressions/VecTVecMultExpr.h>
#include <blaze/math/shims/IsDefault.h>
#include <blaze/math/traits/RowTrait.h>
#include <blaze/math/traits/SubvectorTrait.h>
#include <blaze/math/typetraits/HasConstDataAccess.h>
#include <blaze/math/typetraits/HasMutableDataAccess.h>
#include <blaze/math/typetraits/IsAligned.h>
#include <blaze/math/typetraits/IsOpposedView.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/math/typetraits/IsSubmatrix.h>
#include <blaze/math/typetraits/IsSymmetric.h>
#include <blaze/math/views/row/BaseTemplate.h>
#include <blaze/math/views/row/Dense.h>
#include <blaze/math/views/row/Sparse.h>
#include <blaze/util/DisableIf.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/FunctionTrace.h>
#include <blaze/util/IntegralConstant.h>
#include <blaze/util/mpl/And.h>
#include <blaze/util/mpl/Not.h>
#include <blaze/util/mpl/Or.h>
#include <blaze/util/TrueType.h>
#include <blaze/util/Types.h>


namespace blaze {

//=================================================================================================
//
//  GLOBAL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given matrix.
// \ingroup row
//
// \param matrix The matrix containing the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given matrix.

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> D;
   blaze::CompressedMatrix<double,rowMajor> S;
   // ... Resizing and initialization

   // Creating a view on the 3rd row of the dense matrix D
   auto row3 = row<3UL>( D );

   // Creating a view on the 4th row of the sparse matrix S
   auto row4 = row<4UL>( S );
   \endcode

// In case the row is not properly specified (i.e. if the specified index is greater than or equal
// to the total number of the rows in the given matrix) a \a std::invalid_argument exception is
// thrown.
*/
template< size_t I     // Row index
        , typename MT  // Type of the matrix
        , bool SO >    // Storage order
inline Row<MT,I> row( Matrix<MT,SO>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return Row<MT,I>( ~matrix );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given constant matrix.
// \ingroup row
//
// \param matrix The constant matrix containing the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given constant
// matrix.

   \code
   using blaze::rowMajor;

   const blaze::DynamicMatrix<double,rowMajor> D( ... );
   const blaze::CompressedMatrix<double,rowMajor> S( ... );

   // Creating a view on the 3rd row of the dense matrix D
   auto row3 = row<3UL>( D );

   // Creating a view on the 4th row of the sparse matrix S
   auto row4 = row<4UL>( S );
   \endcode

// In case the row is not properly specified (i.e. if the specified index is greater than or equal
// to the total number of the rows in the given matrix) a \a std::invalid_argument exception is
// thrown.
*/
template< size_t I     // Row index
        , typename MT  // Type of the matrix
        , bool SO >    // Storage order
inline const Row<const MT,I> row( const Matrix<MT,SO>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return Row<const MT,I>( ~matrix );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given temporary matrix.
// \ingroup row
//
// \param matrix The temporary matrix containing the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given temporary
// matrix. In case the row is not properly specified (i.e. if the specified index is greater
// than or equal to the total number of the rows in the given matrix) a \a std::invalid_argument
// exception is thrown.
*/
template< size_t I     // Row index
        , typename MT  // Type of the matrix
        , bool SO >    // Storage order
inline Row<MT,I> row( Matrix<MT,SO>&& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return Row<MT,I>( ~matrix );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given matrix.
// \ingroup row
//
// \param matrix The matrix containing the row.
// \param index The index of the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given matrix.

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> D;
   blaze::CompressedMatrix<double,rowMajor> S;
   // ... Resizing and initialization

   // Creating a view on the 3rd row of the dense matrix D
   auto row3 = row( D, 3UL );

   // Creating a view on the 4th row of the sparse matrix S
   auto row4 = row( S, 4UL );
   \endcode

// In case the row is not properly specified (i.e. if the specified index is greater than or equal
// to the total number of the rows in the given matrix) a \a std::invalid_argument exception is
// thrown.
*/
template< typename MT  // Type of the matrix
        , bool SO >    // Storage order
inline Row<MT> row( Matrix<MT,SO>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return Row<MT>( ~matrix, index );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given constant matrix.
// \ingroup row
//
// \param matrix The constant matrix containing the row.
// \param index The index of the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given constant
// matrix.

   \code
   using blaze::rowMajor;

   const blaze::DynamicMatrix<double,rowMajor> D( ... );
   const blaze::CompressedMatrix<double,rowMajor> S( ... );

   // Creating a view on the 3rd row of the dense matrix D
   auto row3 = row( D, 3UL );

   // Creating a view on the 4th row of the sparse matrix S
   auto row4 = row( S, 4UL );
   \endcode

// In case the row is not properly specified (i.e. if the specified index is greater than or equal
// to the total number of the rows in the given matrix) a \a std::invalid_argument exception is
// thrown.
*/
template< typename MT  // Type of the matrix
        , bool SO >    // Storage order
inline const Row<const MT> row( const Matrix<MT,SO>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return Row<const MT>( ~matrix, index );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given temporary matrix.
// \ingroup row
//
// \param matrix The temporary matrix containing the row.
// \param index The index of the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given temporary
// matrix. In case the row is not properly specified (i.e. if the specified index is greater
// than or equal to the total number of the rows in the given matrix) a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT  // Type of the matrix
        , bool SO >    // Storage order
inline Row<MT> row( Matrix<MT,SO>&& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return Row<MT>( ~matrix, index );
}
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL RESTRUCTURING FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/matrix addition.
// \ingroup row
//
// \param matrix The constant matrix/matrix addition.
// \return View on the specified row of the addition.
//
// This function returns an expression representing the specified row of the given matrix/matrix
// addition.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatAddExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).leftOperand() ) + row<I>( (~matrix).rightOperand() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/matrix addition.
// \ingroup row
//
// \param matrix The constant matrix/matrix addition.
// \param index The index of the row.
// \return View on the specified row of the addition.
//
// This function returns an expression representing the specified row of the given matrix/matrix
// addition.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatAddExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).leftOperand(), index ) + row( (~matrix).rightOperand(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/matrix subtraction.
// \ingroup row
//
// \param matrix The constant matrix/matrix subtraction.
// \return View on the specified row of the subtraction.
//
// This function returns an expression representing the specified row of the given matrix/matrix
// subtraction.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatSubExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).leftOperand() ) - row<I>( (~matrix).rightOperand() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/matrix subtraction.
// \ingroup row
//
// \param matrix The constant matrix/matrix subtraction.
// \param index The index of the row.
// \return View on the specified row of the subtraction.
//
// This function returns an expression representing the specified row of the given matrix/matrix
// subtraction.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatSubExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).leftOperand(), index ) - row( (~matrix).rightOperand(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given Schur product.
// \ingroup row
//
// \param matrix The constant Schur product.
// \return View on the specified row of the Schur product.
//
// This function returns an expression representing the specified row of the given Schur product.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const SchurExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).leftOperand() ) * row<I>( (~matrix).rightOperand() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given Schur product.
// \ingroup row
//
// \param matrix The constant Schur product.
// \param index The index of the row.
// \return View on the specified row of the Schur product.
//
// This function returns an expression representing the specified row of the given Schur product.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const SchurExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).leftOperand(), index ) * row( (~matrix).rightOperand(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/matrix multiplication.
// \ingroup row
//
// \param matrix The constant matrix/matrix multiplication.
// \return View on the specified row of the multiplication.
//
// This function returns an expression representing the specified row of the given matrix/matrix
// multiplication.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatMultExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).leftOperand() ) * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/matrix multiplication.
// \ingroup row
//
// \param matrix The constant matrix/matrix multiplication.
// \param index The index of the row.
// \return View on the specified row of the multiplication.
//
// This function returns an expression representing the specified row of the given matrix/matrix
// multiplication.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatMultExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).leftOperand(), index ) * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given outer product.
// \ingroup row
//
// \param matrix The constant outer product.
// \return View on the specified row of the outer product.
//
// This function returns an expression representing the specified row of the given outer product.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const VecTVecMultExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return (~matrix).leftOperand()[I] * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given outer product.
// \ingroup row
//
// \param matrix The constant outer product.
// \param index The index of the row.
// \return View on the specified row of the outer product.
//
// This function returns an expression representing the specified row of the given outer product.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const VecTVecMultExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return (~matrix).leftOperand()[index] * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/scalar multiplication.
// \ingroup row
//
// \param matrix The constant matrix/scalar multiplication.
// \return View on the specified row of the multiplication.
//
// This function returns an expression representing the specified row of the given matrix/scalar
// multiplication.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatScalarMultExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).leftOperand() ) * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/scalar multiplication.
// \ingroup row
//
// \param matrix The constant matrix/scalar multiplication.
// \param index The index of the row.
// \return View on the specified row of the multiplication.
//
// This function returns an expression representing the specified row of the given matrix/scalar
// multiplication.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatScalarMultExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).leftOperand(), index ) * (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/scalar division.
// \ingroup row
//
// \param matrix The constant matrix/scalar division.
// \return View on the specified row of the division.
//
// This function returns an expression representing the specified row of the given matrix/scalar
// division.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatScalarDivExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).leftOperand() ) / (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix/scalar division.
// \ingroup row
//
// \param matrix The constant matrix/scalar division.
// \param index The index of the row.
// \return View on the specified row of the division.
//
// This function returns an expression representing the specified row of the given matrix/scalar
// division.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatScalarDivExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).leftOperand(), index ) / (~matrix).rightOperand();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given unary matrix map operation.
// \ingroup row
//
// \param matrix The constant unary matrix map operation.
// \return View on the specified row of the unary map operation.
//
// This function returns an expression representing the specified row of the given unary matrix
// map operation.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMapExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return map( row<I>( (~matrix).operand() ), (~matrix).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given unary matrix map operation.
// \ingroup row
//
// \param matrix The constant unary matrix map operation.
// \param index The index of the row.
// \return View on the specified row of the unary map operation.
//
// This function returns an expression representing the specified row of the given unary matrix
// map operation.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMapExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return map( row( (~matrix).operand(), index ), (~matrix).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given binary matrix map operation.
// \ingroup row
//
// \param matrix The constant binary matrix map operation.
// \return View on the specified row of the binary map operation.
//
// This function returns an expression representing the specified row of the given binary matrix
// map operation.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatMapExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return map( row<I>( (~matrix).leftOperand() ),
               row<I>( (~matrix).rightOperand() ),
               (~matrix).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given binary matrix map operation.
// \ingroup row
//
// \param matrix The constant binary matrix map operation.
// \param index The index of the row.
// \return View on the specified row of the binary map operation.
//
// This function returns an expression representing the specified row of the given binary matrix
// map operation.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatMatMapExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return map( row( (~matrix).leftOperand() , index ),
               row( (~matrix).rightOperand(), index ),
               (~matrix).operation() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix evaluation operation.
// \ingroup row
//
// \param matrix The constant matrix evaluation operation.
// \return View on the specified row of the evaluation operation.
//
// This function returns an expression representing the specified row of the given matrix
// evaluation operation.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatEvalExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return eval( row<I>( (~matrix).operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix evaluation operation.
// \ingroup row
//
// \param matrix The constant matrix evaluation operation.
// \param index The index of the row.
// \return View on the specified row of the evaluation operation.
//
// This function returns an expression representing the specified row of the given matrix
// evaluation operation.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatEvalExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return eval( row( (~matrix).operand(), index ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix serialization operation.
// \ingroup row
//
// \param matrix The constant matrix serialization operation.
// \return View on the specified row of the serialization operation.
//
// This function returns an expression representing the specified row of the given matrix
// serialization operation.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatSerialExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return serial( row<I>( (~matrix).operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix serialization operation.
// \ingroup row
//
// \param matrix The constant matrix serialization operation.
// \param index The index of the row.
// \return View on the specified row of the serialization operation.
//
// This function returns an expression representing the specified row of the given matrix
// serialization operation.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatSerialExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return serial( row( (~matrix).operand(), index ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix declaration operation.
// \ingroup row
//
// \param matrix The constant matrix declaration operation.
// \return View on the specified row of the declaration operation.
//
// This function returns an expression representing the specified row of the given matrix
// declaration operation.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const DeclExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return row<I>( (~matrix).operand() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix declaration operation.
// \ingroup row
//
// \param matrix The constant matrix declaration operation.
// \param index The index of the row.
// \return View on the specified row of the declaration operation.
//
// This function returns an expression representing the specified row of the given matrix
// declaration operation.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const DeclExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return row( (~matrix).operand(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix transpose operation.
// \ingroup row
//
// \param matrix The constant matrix transpose operation.
// \return View on the specified row of the transpose operation.
//
// This function returns an expression representing the specified row of the given matrix
// transpose operation.
*/
template< size_t I       // Row index
        , typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatTransExpr<MT>& matrix )
{
   BLAZE_FUNCTION_TRACE;

   return trans( column<I>( (~matrix).operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Creating a view on a specific row of the given matrix transpose operation.
// \ingroup row
//
// \param matrix The constant matrix transpose operation.
// \param index The index of the row.
// \return View on the specified row of the transpose operation.
//
// This function returns an expression representing the specified row of the given matrix
// transpose operation.
*/
template< typename MT >  // Matrix base type of the expression
inline decltype(auto) row( const MatTransExpr<MT>& matrix, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return trans( column( (~matrix).operand(), index ) );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ROW OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Resetting the given row.
// \ingroup row
//
// \param row The row to be resetted.
// \return void
*/
template< typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool DF          // Density flag
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline void reset( RowImpl<MT,SO,DF,SF,RAs...>& row )
{
   row.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Resetting the given temporary row.
// \ingroup row
//
// \param row The temporary row to be resetted.
// \return void
*/
template< typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool DF          // Density flag
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline void reset( RowImpl<MT,SO,DF,SF,RAs...>&& row )
{
   row.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Clearing the given row.
// \ingroup row
//
// \param row The row to be cleared.
// \return void
//
// Clearing a row is equivalent to resetting it via the reset() function.
*/
template< typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool DF          // Density flag
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline void clear( RowImpl<MT,SO,DF,SF,RAs...>& row )
{
   row.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Clearing the given temporary row.
// \ingroup row
//
// \param row The temporary row to be cleared.
// \return void
//
// Clearing a row is equivalent to resetting it via the reset() function.
*/
template< typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool DF          // Density flag
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline void clear( RowImpl<MT,SO,DF,SF,RAs...>&& row )
{
   row.reset();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the given dense row is in default state.
// \ingroup row
//
// \param row The dense row to be tested for its default state.
// \return \a true in case the given dense row is component-wise zero, \a false otherwise.
//
// This function checks whether the dense row is in default state. For instance, in case the
// row is instantiated for a built-in integral or floating point data type, the function returns
// \a true in case all row elements are 0 and \a false in case any row element is not 0. The
// following example demonstrates the use of the \a isDefault function:

   \code
   blaze::DynamicMatrix<int,rowMajor> A;
   // ... Resizing and initialization
   if( isDefault( row( A, 0UL ) ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isDefault<relaxed>( row( A, 0UL ) ) ) { ... }
   \endcode
*/
template< bool RF          // Relaxation flag
        , typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline bool isDefault( const RowImpl<MT,SO,true,SF,RAs...>& row )
{
   using blaze::isDefault;

   for( size_t i=0UL; i<row.size(); ++i )
      if( !isDefault<RF>( row[i] ) ) return false;
   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the given sparse row is in default state.
// \ingroup row
//
// \param row The sparse row to be tested for its default state.
// \return \a true in case the given row is component-wise zero, \a false otherwise.
//
// This function checks whether the sparse row is in default state. For instance, in case the
// row is instantiated for a built-in integral or floating point data type, the function returns
// \a true in case all row elements are 0 and \a false in case any row element is not 0. The
// following example demonstrates the use of the \a isDefault function:

   \code
   blaze::CompressedMatrix<int,rowMajor> A;
   // ... Resizing and initialization
   if( isDefault( row( A, 0UL ) ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isDefault<relaxed>( row( A, 0UL ) ) ) { ... }
   \endcode
*/
template< bool RF          // Relaxation flag
        , typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline bool isDefault( const RowImpl<MT,SO,false,SF,RAs...>& row )
{
   using blaze::isDefault;

   for( const auto& element : row )
      if( !isDefault<RF>( element.value() ) ) return false;
   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the invariants of the given row are intact.
// \ingroup row
//
// \param row The row to be tested.
// \return \a true in case the given row's invariants are intact, \a false otherwise.
//
// This function checks whether the invariants of the row are intact, i.e. if its state is valid.
// In case the invariants are intact, the function returns \a true, else it will return \a false.
// The following example demonstrates the use of the \a isIntact() function:

   \code
   blaze::DynamicMatrix<int,rowMajor> A;
   // ... Resizing and initialization
   if( isIntact( row( A, 0UL ) ) ) { ... }
   \endcode
*/
template< typename MT      // Type of the matrix
        , bool SO          // Storage order
        , bool DF          // Density flag
        , bool SF          // Symmetry flag
        , size_t... RAs >  // Compile time row arguments
inline bool isIntact( const RowImpl<MT,SO,DF,SF,RAs...>& row ) noexcept
{
   return ( row.row() < row.operand().rows() &&
            isIntact( row.operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Backend of the isSame() function for two regular rows.
// \ingroup row
//
// \param a The first row to be tested for its state.
// \param b The second row to be tested for its state.
// \return \a true in case the two rows share a state, \a false otherwise.
//
// This backend implementation of the isSame() function handles the special case of two regular
// rows. In case both rows represent the same observable state, the function returns \a true,
// otherwise it returns \a false.
*/
template< typename MT1      // Type of the matrix of the left-hand side row
        , bool SO           // Storage order
        , bool DF           // Density flag
        , bool SF1          // Symmetry flag of the left-hand side row
        , size_t... RAs1    // Compile time row arguments of the left-hand side row
        , typename MT2      // Type of the matrix of the right-hand side row
        , bool SF2          // Symmetry flag of the right-hand side row
        , size_t... RAs2 >  // Compile time row arguments of the right-hand side row
inline DisableIf_< Or< IsSubmatrix<MT1>, IsSubmatrix<MT2> >, bool >
   isSame_backend( const RowImpl<MT1,SO,DF,SF1,RAs1...>& a,
                   const RowImpl<MT2,SO,DF,SF2,RAs2...>& b ) noexcept
{
   return ( isSame( a.operand(), b.operand() ) && ( a.row() == b.row() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Backend of the isSame() function for the left row being a row on a submatrix.
// \ingroup row
//
// \param a The first row to be tested for its state.
// \param b The second row to be tested for its state.
// \return \a true in case the two rows share a state, \a false otherwise.
//
// This backend implementation of the isSame() function handles the special case of the left row
// being a row on a submatrix. In case both rows represent the same observable state, the function
// returns \a true, otherwise it returns \a false.
*/
template< typename MT1      // Type of the submatrix of the left-hand side row
        , bool SO           // Storage order
        , bool DF           // Density flag
        , bool SF1          // Symmetry flag of the left-hand side row
        , size_t... RAs1    // Compile time row arguments of the left-hand side row
        , typename MT2      // Type of the matrix of the right-hand side row
        , bool SF2          // Symmetry flag of the right-hand side row
        , size_t... RAs2 >  // Compile time row arguments of the right-hand side row
inline EnableIf_< And< IsSubmatrix<MT1>, Not< IsSubmatrix<MT2> > >, bool >
   isSame_backend( const RowImpl<MT1,SO,DF,SF1,RAs1...>& a,
                   const RowImpl<MT2,SO,DF,SF2,RAs2...>& b ) noexcept
{
   return ( isSame( a.operand().operand(), b.operand() ) &&
            ( a.size() == b.size() ) &&
            ( a.row() + a.operand().row() == b.row() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Backend of the isSame() function for the right row being a row on a submatrix.
// \ingroup row
//
// \param a The first row to be tested for its state.
// \param b The second row to be tested for its state.
// \return \a true in case the two rows share a state, \a false otherwise.
//
// This backend implementation of the isSame() function handles the special case of the right row
// being a row on a submatrix. In case both rows represent the same observable state, the function
// returns \a true, otherwise it returns \a false.
*/
template< typename MT1      // Type of the matrix of the left-hand side row
        , bool SO           // Storage order
        , bool DF           // Density flag
        , bool SF1          // Symmetry flag of the left-hand side row
        , size_t... RAs1    // Compile time row arguments of the left-hand side row
        , typename MT2      // Type of the submatrix of the right-hand side row
        , bool SF2          // Symmetry flag of the right-hand side row
        , size_t... RAs2 >  // Compile time row arguments of the right-hand side row
inline EnableIf_< And< Not< IsSubmatrix<MT1> >, IsSubmatrix<MT2> >, bool >
   isSame_backend( const RowImpl<MT1,SO,DF,SF1,RAs1...>& a,
                   const RowImpl<MT2,SO,DF,SF2,RAs2...>& b ) noexcept
{
   return ( isSame( a.operand(), b.operand().operand() ) &&
            ( a.size() == b.size() ) &&
            ( a.row() == b.row() + b.operand().row() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Backend of the isSame() function for two rows on submatrices.
// \ingroup row
//
// \param a The first row to be tested for its state.
// \param b The second row to be tested for its state.
// \return \a true in case the two rows share a state, \a false otherwise.
//
// This backend implementation of the isSame() function handles the special case of both rows
// being rows on submatrices. In case both rows represent the same observable state, the function
// returns \a true, otherwise it returns \a false.
*/
template< typename MT1      // Type of the submatrix of the left-hand side row
        , bool SO           // Storage order
        , bool DF           // Density flag
        , bool SF1          // Symmetry flag of the left-hand side row
        , size_t... RAs1    // Compile time row arguments of the left-hand side row
        , typename MT2      // Type of the submatrix of the right-hand side row
        , bool SF2          // Symmetry flag of the right-hand side row
        , size_t... RAs2 >  // Compile time row arguments of the right-hand side row
inline EnableIf_< And< IsSubmatrix<MT1>, IsSubmatrix<MT2> >, bool >
   isSame_backend( const RowImpl<MT1,SO,DF,SF1,RAs1...>& a,
                   const RowImpl<MT2,SO,DF,SF2,RAs2...>& b ) noexcept
{
   return ( isSame( a.operand().operand(), b.operand().operand() ) &&
            ( a.size() == b.size() ) &&
            ( a.row() + a.operand().row() == b.row() + b.operand().row() ) &&
            ( a.operand().column() == b.operand().column() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the two given rows represent the same observable state.
// \ingroup row
//
// \param a The first row to be tested for its state.
// \param b The second row to be tested for its state.
// \return \a true in case the two rows share a state, \a false otherwise.
//
// This overload of the isSame() function tests if the two given rows refer to exactly the same
// range of the same matrix. In case both rows represent the same observable state, the function
// returns \a true, otherwise it returns \a false.
*/
template< typename MT1      // Type of the matrix of the left-hand side row
        , bool SO           // Storage order
        , bool DF           // Density flag
        , bool SF1          // Symmetry flag of the left-hand side row
        , size_t... RAs1    // Compile time row arguments of the left-hand side row
        , typename MT2      // Type of the matrix of the right-hand side row
        , bool SF2          // Symmetry flag of the right-hand side row
        , size_t... RAs2 >  // Compile time row arguments of the right-hand side row
inline bool isSame( const RowImpl<MT1,SO,DF,SF1,RAs1...>& a,
                    const RowImpl<MT2,SO,DF,SF2,RAs2...>& b ) noexcept
{
   return isSame_backend( a, b );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the assignment of a vector to a row.
// \ingroup row
//
// \param lhs The target left-hand side row.
// \param rhs The right-hand side vector to be assigned.
// \param index The index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the matrix
        , bool SO        // Storage order
        , bool DF        // Density flag
        , bool SF        // Symmetry flag
        , size_t... RAs  // Compile time row arguments
        , typename VT >  // Type of the right-hand side vector
inline bool tryAssign( const RowImpl<MT,SO,DF,SF,RAs...>& lhs,
                       const Vector<VT,true>& rhs, size_t index )
{
   BLAZE_INTERNAL_ASSERT( index <= lhs.size(), "Invalid vector access index" );
   BLAZE_INTERNAL_ASSERT( (~rhs).size() <= lhs.size() - index, "Invalid vector size" );

   return tryAssign( lhs.operand(), ~rhs, lhs.row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the addition assignment of a vector to a row.
// \ingroup row
//
// \param lhs The target left-hand side row.
// \param rhs The right-hand side vector to be added.
// \param index The index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the matrix
        , bool SO        // Storage order
        , bool DF        // Density flag
        , bool SF        // Symmetry flag
        , size_t... RAs  // Compile time row arguments
        , typename VT >  // Type of the right-hand side vector
inline bool tryAddAssign( const RowImpl<MT,SO,DF,SF,RAs...>& lhs,
                          const Vector<VT,true>& rhs, size_t index )
{
   BLAZE_INTERNAL_ASSERT( index <= lhs.size(), "Invalid vector access index" );
   BLAZE_INTERNAL_ASSERT( (~rhs).size() <= lhs.size() - index, "Invalid vector size" );

   return tryAddAssign( lhs.operand(), ~rhs, lhs.row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the subtraction assignment of a vector to a row.
// \ingroup row
//
// \param lhs The target left-hand side row.
// \param rhs The right-hand side vector to be subtracted.
// \param index The index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the matrix
        , bool SO        // Storage order
        , bool DF        // Density flag
        , bool SF        // Symmetry flag
        , size_t... RAs  // Compile time row arguments
        , typename VT >  // Type of the right-hand side vector
inline bool trySubAssign( const RowImpl<MT,SO,DF,SF,RAs...>& lhs,
                          const Vector<VT,true>& rhs, size_t index )
{
   BLAZE_INTERNAL_ASSERT( index <= lhs.size(), "Invalid vector access index" );
   BLAZE_INTERNAL_ASSERT( (~rhs).size() <= lhs.size() - index, "Invalid vector size" );

   return trySubAssign( lhs.operand(), ~rhs, lhs.row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the multiplication assignment of a vector to a row.
// \ingroup row
//
// \param lhs The target left-hand side row.
// \param rhs The right-hand side vector to be multiplied.
// \param index The index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the matrix
        , bool SO        // Storage order
        , bool DF        // Density flag
        , bool SF        // Symmetry flag
        , size_t... RAs  // Compile time row arguments
        , typename VT >  // Type of the right-hand side vector
inline bool tryMultAssign( const RowImpl<MT,SO,DF,SF,RAs...>& lhs,
                           const Vector<VT,true>& rhs, size_t index )
{
   BLAZE_INTERNAL_ASSERT( index <= lhs.size(), "Invalid vector access index" );
   BLAZE_INTERNAL_ASSERT( (~rhs).size() <= lhs.size() - index, "Invalid vector size" );

   return tryMultAssign( lhs.operand(), ~rhs, lhs.row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Predict invariant violations by the division assignment of a vector to a row.
// \ingroup row
//
// \param lhs The target left-hand side row.
// \param rhs The right-hand side vector divisor.
// \param index The index of the first element to be modified.
// \return \a true in case the assignment would be successful, \a false if not.
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the matrix
        , bool SO        // Storage order
        , bool DF        // Density flag
        , bool SF        // Symmetry flag
        , size_t... RAs  // Compile time row arguments
        , typename VT >  // Type of the right-hand side vector
inline bool tryDivAssign( const RowImpl<MT,SO,DF,SF,RAs...>& lhs,
                          const Vector<VT,true>& rhs, size_t index )
{
   BLAZE_INTERNAL_ASSERT( index <= lhs.size(), "Invalid vector access index" );
   BLAZE_INTERNAL_ASSERT( (~rhs).size() <= lhs.size() - index, "Invalid vector size" );

   return tryDivAssign( lhs.operand(), ~rhs, lhs.row(), index );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given row.
// \ingroup row
//
// \param r The row to be derestricted.
// \return Row without access restrictions.
//
// This function removes all restrictions on the data access to the given row. It returns a row
// object that does provide the same interface but does not have any restrictions on the data
// access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT  // Type of the matrix
        , bool SO      // Storage order
        , bool DF      // Density flag
        , bool SF      // Symmetry flag
        , size_t I >   // Row index
inline decltype(auto) derestrict( RowImpl<MT,SO,DF,SF,I>& r )
{
   return row<I>( derestrict( r.operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given temporary row.
// \ingroup row
//
// \param r The temporary row to be derestricted.
// \return Row without access restrictions.
//
// This function removes all restrictions on the data access to the given temporary row. It
// returns a row object that does provide the same interface but does not have any restrictions
// on the data access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT  // Type of the matrix
        , bool SO      // Storage order
        , bool DF      // Density flag
        , bool SF      // Symmetry flag
        , size_t I >   // Row index
inline decltype(auto) derestrict( RowImpl<MT,SO,DF,SF,I>&& r )
{
   return row<I>( derestrict( r.operand() ) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given row.
// \ingroup row
//
// \param r The row to be derestricted.
// \return Row without access restrictions.
//
// This function removes all restrictions on the data access to the given row. It returns a row
// object that does provide the same interface but does not have any restrictions on the data
// access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT  // Type of the matrix
        , bool SO      // Storage order
        , bool DF      // Density flag
        , bool SF >    // Symmetry flag
inline decltype(auto) derestrict( RowImpl<MT,SO,DF,SF>& r )
{
   return row( derestrict( r.operand() ), r.row() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Removal of all restrictions on the data access to the given temporary row.
// \ingroup row
//
// \param r The temporary row to be derestricted.
// \return Row without access restrictions.
//
// This function removes all restrictions on the data access to the given temporary row. It
// returns a row object that does provide the same interface but does not have any restrictions
// on the data access.\n
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in the violation of invariants, erroneous results and/or in compilation errors.
*/
template< typename MT  // Type of the matrix
        , bool SO      // Storage order
        , bool DF      // Density flag
        , bool SF >    // Symmetry flag
inline decltype(auto) derestrict( RowImpl<MT,SO,DF,SF>&& r )
{
   return row( derestrict( r.operand() ), r.row() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISRESTRICTED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, bool SF, size_t... RAs >
struct IsRestricted< RowImpl<MT,SO,DF,SF,RAs...> >
   : public BoolConstant< IsRestricted<MT>::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  HASCONSTDATAACCESS SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool SF, size_t... RAs >
struct HasConstDataAccess< RowImpl<MT,SO,true,SF,RAs...> >
   : public BoolConstant< HasConstDataAccess<MT>::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  HASMUTABLEDATAACCESS SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool SF, size_t... RAs >
struct HasMutableDataAccess< RowImpl<MT,SO,true,SF,RAs...> >
   : public BoolConstant< HasMutableDataAccess<MT>::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISALIGNED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool SF, size_t... RAs >
struct IsAligned< RowImpl<MT,SO,true,SF,RAs...> >
   : public BoolConstant< And< IsAligned<MT>, Or< IsRowMajorMatrix<MT>, IsSymmetric<MT> > >::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISPADDED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool SF, size_t... RAs >
struct IsPadded< RowImpl<MT,SO,true,SF,RAs...> >
   : public BoolConstant< And< IsPadded<MT>, Or< IsRowMajorMatrix<MT>, IsSymmetric<MT> > >::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISOPPOSEDVIEW SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool DF, size_t... RAs >
struct IsOpposedView< RowImpl<MT,false,DF,false,RAs...> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  SUBVECTORTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO, bool DF, bool SF, size_t... RAs, size_t... SAs >
struct SubvectorTrait< RowImpl<MT,SO,DF,SF,RAs...>, SAs... >
{
   using Type = SubvectorTrait_< ResultType_< RowImpl<MT,SO,DF,SF,RAs...> >, SAs... >;
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
