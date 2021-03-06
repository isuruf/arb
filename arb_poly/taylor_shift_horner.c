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

    Copyright (C) 2016 Fredrik Johansson

******************************************************************************/

#include "arb_poly.h"

void
_arb_poly_taylor_shift_horner(arb_ptr poly, const arb_t c, slong n, slong prec)
{
    slong i, j;

    if (arb_is_one(c))
    {
        for (i = n - 2; i >= 0; i--)
            for (j = i; j < n - 1; j++)
                arb_add(poly + j, poly + j, poly + j + 1, prec);
    }
    else if (arb_equal_si(c, -1))
    {
        for (i = n - 2; i >= 0; i--)
            for (j = i; j < n - 1; j++)
                arb_sub(poly + j, poly + j, poly + j + 1, prec);
    }
    else if (!arb_is_zero(c))
    {
        for (i = n - 2; i >= 0; i--)
            for (j = i; j < n - 1; j++)
                arb_addmul(poly + j, poly + j + 1, c, prec);
    }
}

void
arb_poly_taylor_shift_horner(arb_poly_t g, const arb_poly_t f,
    const arb_t c, slong prec)
{
    if (f != g)
        arb_poly_set_round(g, f, prec);

    _arb_poly_taylor_shift_horner(g->coeffs, c, g->length, prec);
}

