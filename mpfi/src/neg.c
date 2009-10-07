/* neg.c -- Return interval opposite.

Copyright 1999, 2000, 2001, 2002, 2003, 2004, 2005,
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
mpfi_neg (mpfi_ptr a, mpfi_srcptr b)
{
  mpfr_t tmp;
  int inexact_left, inexact_right, inexact=0;

  mpfr_init2 (tmp, mpfi_get_prec (a));
  inexact_right = mpfr_set (tmp, &(b->left), MPFI_RNDD);
  inexact_left = mpfr_neg (&(a->left), &(b->right), MPFI_RNDD);
  inexact_right |= mpfr_neg (&(a->right), tmp, MPFI_RNDU);
  mpfr_clear (tmp);

  if (MPFI_NAN_P (a))
    MPFR_RET_NAN;

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  if (mpfi_revert_if_needed (a)) {
    /*
    fprintf (stdout, "Pb endpoints in reverse order in mpfi_neg, operand: ");
    mpfi_out_str (stdout, 10, 0, b);
    fprintf (stdout, "Pb endpoints in reverse order in mpfi_neg, result: ");
    mpfi_out_str (stdout, 10, 0, a);
    fprintf (stdout, "\n");
    */
    inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
  }

  return inexact;
}
