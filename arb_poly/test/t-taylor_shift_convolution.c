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

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("taylor_shift_convolution....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 2000; iter++)
    {
        slong prec1, prec2;
        arb_poly_t f, g;
        arb_t c, d, e;

        prec1 = 2 + n_randint(state, 500);
        prec2 = 2 + n_randint(state, 500);

        arb_poly_init(f);
        arb_poly_init(g);

        arb_init(c);
        arb_init(d);
        arb_init(e);

        arb_poly_randtest(f, state, 1 + n_randint(state, 40), 1 + n_randint(state, 500), 10);
        arb_poly_randtest(g, state, 1 + n_randint(state, 20), 1 + n_randint(state, 500), 10);

        if (n_randint(state, 2))
            arb_set_si(c, n_randint(state, 5) - 2);
        else
            arb_randtest(c, state, 1 + n_randint(state, 500), 1 + n_randint(state, 100));

        if (n_randint(state, 2))
            arb_set_si(d, n_randint(state, 5) - 2);
        else
            arb_randtest(d, state, 1 + n_randint(state, 500), 1 + n_randint(state, 100));

        arb_add(e, c, d, prec1);

        /* check f(x+c)(x+d) = f(x+c+d) */
        arb_poly_taylor_shift_convolution(g, f, e, prec2);
        arb_poly_taylor_shift_convolution(f, f, c, prec1);
        arb_poly_taylor_shift_convolution(f, f, d, prec1);

        if (!arb_poly_overlaps(f, g))
        {
            flint_printf("FAIL\n\n");

            flint_printf("c = "); arb_printd(c, 15); flint_printf("\n\n");
            flint_printf("d = "); arb_printd(d, 15); flint_printf("\n\n");

            flint_printf("f = "); arb_poly_printd(f, 15); flint_printf("\n\n");
            flint_printf("g = "); arb_poly_printd(g, 15); flint_printf("\n\n");

            abort();
        }

        arb_poly_clear(f);
        arb_poly_clear(g);

        arb_clear(c);
        arb_clear(d);
        arb_clear(e);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
