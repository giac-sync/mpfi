/* type_iiz.c -- Test functions associated with functions of the type
                 mpfi_f (mpfi_t, mpfi_t, mpz_t).

Copyright 2010
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

#include "mpfi-tests.h"

extern unsigned long line_number; /* current line */
unsigned long test_line_number;   /* start line of current test */

void
read_line_iiz (mpfi_function_ptr this, FILE* fp)
{
  test_line_number = line_number;
  /* [1] return value */
  read_exactness (fp, &(MPFI_FUN_ARG (*this, 1, i)));
  /* [2] expected value */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] mpfi_t operand */
  read_mpfi (fp, MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] mpz_t operand */
  read_mpz (fp, MPFI_FUN_ARG (*this, 4, mpz));
}

/* check function against data at different precisions and test if an
   input variable can be reused as output */

void
check_line_iiz (mpfi_function_ptr this)
{
  int inex;

  /* rename operands for better readability */
  IIZ_fun  f_IIZ    = MPFI_FUN_GET (*this, IIZ);
  mpfi_ptr got      = MPFI_FUN_ARG (*this, 0, mpfi);
  int expected_inex = MPFI_FUN_ARG (*this, 1, i);
  mpfi_ptr expected = MPFI_FUN_ARG (*this, 2, mpfi);
  mpfi_ptr op1      = MPFI_FUN_ARG (*this, 3, mpfi);
  mpz_ptr op2       = MPFI_FUN_ARG (*this, 4, mpz);

  mpfi_set_prec (got, mpfi_get_prec (expected));

  inex = f_IIZ (got, op1, op2);
  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Failed line %lu.\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = ");
    mpz_out_str (stdout, 16, op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }

  /* reuse input variable as output */
  mpfi_set (got, op1);

  inex = f_IIZ (got, got, op2);

  if (inex != expected_inex || !same_value (got, expected)) {
    printf ("Error when reusing first input argument as output (line %lu)."
            "\nop1 = ", test_line_number);
    mpfi_out_str (stdout, 16, 0, op1);
    printf ("\nop2 = ");
    mpz_out_str (stdout, 16, op2);
    printf ("\ngot      = ");
    mpfi_out_str (stdout, 16, 0, got);
    printf ("\nexpected = ");
    mpfi_out_str (stdout, 16, 0, expected);
    putchar ('\n');
    if (inex != expected_inex)
      printf ("inexact flag: got = %u, expected = %u\n",
              inex, expected_inex);

    exit (1);
  }
}

/* Check if the image of a random point chosen in the given interval is in the
   image of this interval. */
void
random_iiz (mpfi_function_ptr this)
{
  /* rename operands for better readability */
  IIZ_fun f_IIZ = MPFI_FUN_GET (*this, IIZ);
  RRZ_fun f_RRZ = MPFI_FUN_MPFR_FUNCTION (*this, IIZ);
  mpfi_ptr b    = MPFI_FUN_ARG (*this, 2, mpfi);
  mpfi_ptr a    = MPFI_FUN_ARG (*this, 3, mpfi);
  mpz_ptr  z    = MPFI_FUN_ARG (*this, 4, mpz);
  /* reuse endpoint as mpfr_t */
  mpfi_ptr i    = MPFI_FUN_ARG (*this, 0, mpfi);
  mpfr_ptr x    = &(i->left);
  mpfr_ptr y    = &(i->right);
  unsigned long n = mpfi_get_prec (a) + 17;

  random_mpz (z, n);
  random_interval (a);
  mpfi_alea (x, a);
  f_IIZ (b, a, z);
  f_RRZ (y, x, z, MPFI_RNDD);
  if (!mpfi_is_inside_fr (y, b)) {
    printf ("Error:\nthe interval b, image of (a, n), does not contain "
            "the point y, image of (x, n) where x is in a.\na = ");
    mpfi_out_str (stdout, 10, 0, a);
    printf ("\nn = ");
    mpz_out_str (stdout, 10, z); 
    printf ("\nb = ");
    mpfi_out_str (stdout, 10, 0, b);
    printf ("\nx = ");
    mpfr_out_str (stdout, 10, 0, x, MPFI_RNDU);
    printf ("\ny = ");
    mpfr_out_str (stdout, 10, 0, y, MPFI_RNDU);
    putchar ('\n');

    exit (1);
  }
}

void
set_prec_iiz (mpfi_function_ptr this, mp_prec_t prec)
{
  mpfi_set_prec (MPFI_FUN_ARG (*this, 0, mpfi), prec);
  mpfi_set_prec (MPFI_FUN_ARG (*this, 2, mpfi), prec);
  mpfi_set_prec (MPFI_FUN_ARG (*this, 3, mpfi), prec);
}

void
clear_iiz (mpfi_function_ptr this)
{
  /* [0] initial value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 0, mpfi));
  /* [1] return value (int), needs no deallocation */
  /* [2] expected value (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 2, mpfi));
  /* [3] operand (mpfi_t) */
  mpfi_clear (MPFI_FUN_ARG (*this, 3, mpfi));
  /* [4] operand (mpz_t) */
  mpz_clear (MPFI_FUN_ARG (*this, 4, mpz));

  free (MPFI_FUN_ARGS (*this));
  MPFI_FUN_ARGS (*this) = NULL;
}

/* In operands array, variables are in the same order as for data in
   '.dat' files plus one additional variable before them. */
void
mpfi_fun_init_IIZ (mpfi_function_ptr this, IIZ_fun mpfi_function,
                   RRZ_fun mpfr_function)
{
  this->type = IIZ;
  this->func.IIZ = mpfi_function;
  this->mpfr_func.IIZ = mpfr_function;

  /* init operands */
  MPFI_FUN_ARGS (*this) =
    (mpfi_fun_operand_t*) malloc (5 * sizeof (mpfi_fun_operand_t));

  /* [0] initial value (mpfi_t) */
  mpfi_init2 (MPFI_FUN_ARG (*this, 0, mpfi), 1024);
  /* [1] return value (int), needs no initialization */
  /* [2] expected value (mpfi_t) */
  mpfi_init2 (MPFI_FUN_ARG (*this, 2, mpfi), 1024);
  /* [3] first operand (mpfi_t) */
  mpfi_init2 (MPFI_FUN_ARG (*this, 3, mpfi), 1024);
  /* [4] second operand (mpz_t) */
  mpz_init (MPFI_FUN_ARG (*this, 4, mpz));

  /* init methods */
  this->set_prec    = set_prec_iiz;
  this->read_line  = read_line_iiz;
  this->check_line = check_line_iiz;
  this->random     = random_iiz;
  this->clear      = clear_iiz;
}