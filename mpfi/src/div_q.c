/* div_q.c -- Divide an interval by a rational number.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2010
                     Spaces project, Inria Lorraine
                     and Salsa project, INRIA Rocquencourt,
                     and Arenaire project, Inria Rhone-Alpes, France
                     and Lab. ANO, USTL (Univ. of Lille),  France

This file is part of the MPFI Library.

The MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPFI Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
MA 02110-1301, USA. */


#include "mpfi.h"
#include "mpfi-impl.h"

int
mpfi_div_q (mpfi_ptr a, mpfi_srcptr b, mpq_srcptr c)
{
  mpfi_t tmp;
  int inexact_set, inexact_div, inexact=0;

  mpfi_init2 (tmp, mpfi_get_prec (a));
  inexact_set = mpfi_set_q (tmp, c);
  inexact_div = mpfi_div (a, b, tmp);
  MPFI_CLEAR (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if (MPFI_LEFT_IS_INEXACT (inexact_div)
      || (inexact_set && !mpfr_inf_p (&a->left) && !mpfr_zero_p (&a->left))) {
    /* the first condition MPFI_LEFT_IS_INEXACT (inexact_div) handles, among
       others, overflow and underflow cases.
       if a->left = infinity in non-overflow case, then a->left is the
       quotient of an infinite endpoint of b with c, thus it is exact even if
       tmp is not exact.
       if a->left = 0 in non-underflow case, then a->left is the quotient of a
       zero endpoint of b with c, thus it is exact. */
    inexact += 1;
  }
  if (MPFI_RIGHT_IS_INEXACT (inexact_div)
      ||(inexact_set && !mpfr_inf_p (&a->right) && !mpfr_zero_p (&a->right))){
    inexact += 2;
  }

  return inexact;
}
