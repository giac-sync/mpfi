/* inv.c -- Inverse of an interval.

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


#include <stdio.h>
#include "mpfi.h"
#include "mpfi-impl.h"

int
mpfi_inv (mpfi_ptr a, mpfi_srcptr b)
{
  mpfr_t tmp;
  int inexact_left, inexact_right, inexact = 0;

  if ( MPFI_NAN_P (b) ) {
    mpfr_set_nan (&(a->left));
    mpfr_set_nan (&(a->right));
    MPFR_RET_NAN;
  }
  else if ( (mpfr_sgn (&(b->left)) < 0) && (mpfr_sgn (&(b->right)) > 0) ) {
    /* b has endpoints of opposite signs */
    /* b can even be [0-, 0+] */
    /* The result is [-oo, +oo] and both endpoints are exact. */
    /*MPFI_ERROR ("mpfi_inv: inversion by an interval that contains 0");*/
    mpfr_set_inf (&(a->left), -1);
    mpfr_set_inf (&(a->right), 1);
  }
  else { /* signed zeroes and Infinity are properly handled by MPFR */
      mpfr_init2 (tmp, mpfi_get_prec (a));
      inexact_right = mpfr_ui_div (tmp, 1, &(b->left), MPFI_RNDU);
      inexact_left = mpfr_ui_div (&(a->left), 1, &(b->right), MPFI_RNDD);
      inexact_right |= mpfr_set (&(a->right), tmp, MPFI_RNDU);
      if (inexact_left)
        inexact += 1;
      if (inexact_right)
        inexact += 2;
      if (mpfi_revert_if_needed (a)) {
        /* Not an error, but due to lazy programming */
	WARNING_REVERTED_ENDPOINTS (a, "mpfi_inv");
        inexact = MPFI_REVERT_INEXACT_FLAGS (inexact);
      }
      mpfr_clear (tmp);
  }

  return inexact;
}
