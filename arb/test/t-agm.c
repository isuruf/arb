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

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "arb.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("agm....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 100000; iter++)
    {
        arb_t a, b, c;
        fmpq_t q, r;
        mpfr_t t, u;
        slong prec = 2 + n_randint(state, 200);

        arb_init(a);
        arb_init(b);
        arb_init(c);
        fmpq_init(q);
        fmpq_init(r);
        mpfr_init2(t, prec + 100);
        mpfr_init2(u, prec + 100);

        arb_randtest(a, state, 1 + n_randint(state, 200), 3);
        arb_randtest(b, state, 1 + n_randint(state, 200), 3);
        arb_randtest(c, state, 1 + n_randint(state, 200), 3);

        arb_agm(c, a, b, prec);

        if (arb_equal(a, b))
        {
            if (!arb_contains(c, a))
            {
                flint_printf("FAIL: containment (identity)\n\n");
                flint_printf("a = "); arb_print(a); flint_printf("\n\n");
                flint_printf("b = "); arb_print(b); flint_printf("\n\n");
                flint_printf("c = "); arb_print(c); flint_printf("\n\n");
                abort();
            }
        }
        else
        {
            arb_get_rand_fmpq(q, state, a, 1 + n_randint(state, 200));
            arb_get_rand_fmpq(r, state, b, 1 + n_randint(state, 200));
            fmpq_get_mpfr(t, q, MPFR_RNDN);
            fmpq_get_mpfr(u, r, MPFR_RNDN);
            mpfr_agm(t, t, u, MPFR_RNDN);

            if (!arb_contains_mpfr(c, t))
            {
                flint_printf("FAIL: containment\n\n");
                flint_printf("a = "); arb_print(a); flint_printf("\n\n");
                flint_printf("b = "); arb_print(b); flint_printf("\n\n");
                flint_printf("c = "); arb_print(c); flint_printf("\n\n");
                abort();
            }
        }

        arb_clear(a);
        arb_clear(b);
        arb_clear(c);
        fmpq_clear(q);
        fmpq_clear(r);
        mpfr_clear(t);
        mpfr_clear(u);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

