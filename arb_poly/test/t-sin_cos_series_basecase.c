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

    Copyright (C) 2012, 2013 Fredrik Johansson

******************************************************************************/

#include "arb_poly.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("sin_cos_series_basecase....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        slong m, n, qbits, rbits1, rbits2;
        fmpq_poly_t A, B;
        arb_poly_t a, b, c, d, e;
        int times_pi;

        qbits = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);

        m = 1 + n_randint(state, 30);
        n = 1 + n_randint(state, 30);
        times_pi = n_randint(state, 2);

        fmpq_poly_init(A);
        fmpq_poly_init(B);
        arb_poly_init(a);
        arb_poly_init(b);
        arb_poly_init(c);
        arb_poly_init(d);
        arb_poly_init(e);

        fmpq_poly_randtest(A, state, m, qbits);
        arb_poly_set_fmpq_poly(a, A, rbits1);

        arb_poly_sin_cos_series_basecase(b, c, a, n, rbits2, times_pi);

        /* Check sin(x)^2 + cos(x)^2 = 1 */
        arb_poly_mullow(d, b, b, n, rbits2);
        arb_poly_mullow(e, c, c, n, rbits2);
        arb_poly_add(d, d, e, rbits2);

        fmpq_poly_one(B);
        if (!arb_poly_contains_fmpq_poly(d, B))
        {
            flint_printf("FAIL\n\n");
            flint_printf("bits2 = %wd\n", rbits2);

            flint_printf("A = "); fmpq_poly_print(A); flint_printf("\n\n");
            flint_printf("a = "); arb_poly_printd(a, 15); flint_printf("\n\n");
            flint_printf("b = "); arb_poly_printd(b, 15); flint_printf("\n\n");
            flint_printf("c = "); arb_poly_printd(c, 15); flint_printf("\n\n");
            flint_printf("d = "); arb_poly_printd(d, 15); flint_printf("\n\n");

            abort();
        }

        arb_poly_set(d, a);
        arb_poly_sin_cos_series_basecase(d, c, d, n, rbits2, times_pi);
        if (!arb_poly_equal(b, d))
        {
            flint_printf("FAIL (aliasing 1)\n\n");
            abort();
        }

        arb_poly_set(d, a);
        arb_poly_sin_cos_series_basecase(b, d, d, n, rbits2, times_pi);
        if (!arb_poly_equal(c, d))
        {
            flint_printf("FAIL (aliasing 2)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);
        arb_poly_clear(a);
        arb_poly_clear(b);
        arb_poly_clear(c);
        arb_poly_clear(d);
        arb_poly_clear(e);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

