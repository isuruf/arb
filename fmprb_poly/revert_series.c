/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

void
_fmprb_poly_revert_series(fmprb_ptr Qinv,
    fmprb_srcptr Q, long Qlen, long n, long prec)
{
    _fmprb_poly_revert_series_lagrange_fast(Qinv, Q, Qlen, n, prec);
}

void
fmprb_poly_revert_series(fmprb_poly_t Qinv,
                                    const fmprb_poly_t Q, long n, long prec)
{
    long Qlen = Q->length;

    if (Qlen < 2 || !fmprb_is_zero(Q->coeffs)
                 || fmprb_contains_zero(Q->coeffs + 1))
    {
        printf("Exception (fmprb_poly_revert_series). Input must \n"
               "have zero constant term and nonzero coefficient of x^1.\n");
        abort();
    }

    if (Qinv != Q)
    {
        fmprb_poly_fit_length(Qinv, n);
        _fmprb_poly_revert_series(Qinv->coeffs, Q->coeffs, Qlen, n, prec);
    }
    else
    {
        fmprb_poly_t t;
        fmprb_poly_init2(t, n);
        _fmprb_poly_revert_series(t->coeffs, Q->coeffs, Qlen, n, prec);
        fmprb_poly_swap(Qinv, t);
        fmprb_poly_clear(t);
    }

    _fmprb_poly_set_length(Qinv, n);
    _fmprb_poly_normalise(Qinv);
}

