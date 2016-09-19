// GNU General Public License Agreement
// Copyright (C) 2004-2010 CodeCogs, Zyba Ltd, Broadwood, Holford, TA5 1DU, England.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by CodeCogs. 
// You must retain a copy of this licence in all copies. 
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
// ---------------------------------------------------------------------------------
//! Calculates array of Bernoulli numbers using an infinite series

#ifndef MATHS_NUMBERTHEORY_BERNOULLI_B_H
#define MATHS_NUMBERTHEORY_BERNOULLI_B_H

#include <math.h>
#include <float.h>

namespace Maths
{

namespace NumberTheory
{

//! Calculates array of Bernoulli numbers using an infinite series

void bernoulli_B( int iMax, double* dB )
{
  dB[0] = 1.0;
  dB[1] = -0.5;

  //! odd index elements
  //!
  for( int j = 3; j <= iMax; j += 2 )
    dB[j] = 0.0;

  //! computer dependent value
  //! ultimate calculation accuracy
  //
  const double eps = DBL_EPSILON;  //! minimal eps such that 1.0 + eps != 1.0
  const double TwoPi = 6.2831853071795860;
  double dCoeff = 2.0 / (TwoPi * TwoPi);
  double d2 = 2.0;

  //! Bernoulli numbers with even index
  //
  for( int n = 1; n <= iMax/2; n++ )
  {
    double   g1 = 1.0,      //!< single terms initialization
    g2 = 1.0;

    for( int j = 0; j < n; j++ )
    {
      g1 *= 4.0;      //!< 2^2n
      g2 *= 9.0;                        //!< 3^2n
    }

    double   S1 = 1.0 - 1.0/g1,          //!< partial sums initialization
    S2 = S1 + 1.0/g2,  S3;

    //! Aitken transformation to speed up convergence
    //
    double   T1 = S1 + 1.0/(g2 + g1),   //!< transformed partial sum
    T2;

    long r = 4;
    double s = -1.0;      //!< current term sign
    int nSuccess = 0;

    while( !nSuccess )
    {
      // r^(2n)
      //
      double r2 = double(r*r);
      double g3 = 1.0;

      for( int j = 0; j < n; j++ )
        g3 *= r2;

      //! next partial sum and Aitken transformed
      //
      S3 = S2 + s/g3;
      T2 = S2 + s/(g3 + g2);

      if( fabs(T2-T1) > eps*fabs(T2) )   //!< continue with next term
      {
        g2 = g3;
        S2 = S3;
        T1 = T2;
        s = -s;
        r++;
      }
      else        //!< convergence
      {
        nSuccess = 1;
      }
    }
    d2 /= 4.0;    //!< 2^(1-2n)
    dB[2*n] = 2.0 * dCoeff / (1.0-d2) * T2;
    dCoeff *= -double((2*n+1)*(2*n+2)) / (TwoPi * TwoPi);
  }
}

}

}

#endif

