/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "arb.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("submul....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        arb_t a, b, c;
        fmpq_t x, y, z;

        arb_init(a, n_randint(state, 200));
        arb_init(b, n_randint(state, 200));
        arb_init(c, n_randint(state, 200));

        fmpq_init(x);
        fmpq_init(y);
        fmpq_init(z);

        arb_randtest(a, state, 10);
        arb_randtest(b, state, 10);
        arb_randtest(c, state, 10);

        arb_get_rand_fmpq(x, state, a);
        arb_get_rand_fmpq(y, state, b);
        arb_get_rand_fmpq(z, state, c);

        arb_submul(c, a, b);
        fmpq_submul(z, x, y);

        if (!arb_contains_fmpq(c, z))
        {
            printf("FAIL: containment\n\n");
            printf("a = "); arb_debug(a); printf("\n\n");
            printf("x = "); fmpq_print(x); printf("\n\n");
            printf("b = "); arb_debug(b); printf("\n\n");
            printf("y = "); fmpq_print(y); printf("\n\n");
            printf("c = "); arb_debug(c); printf("\n\n");
            printf("z = "); fmpq_print(z); printf("\n\n");
            abort();
        }

        arb_clear(a);
        arb_clear(b);
        arb_clear(c);

        fmpq_clear(x);
        fmpq_clear(y);
        fmpq_clear(z);
    }

    /* aliasing of c and a */
    for (iter = 0; iter < 10000; iter++)
    {
        arb_t a, b;
        fmpq_t x, y;

        arb_init(a, n_randint(state, 200));
        arb_init(b, n_randint(state, 200));

        fmpq_init(x);
        fmpq_init(y);

        arb_randtest(a, state, 10);
        arb_randtest(b, state, 10);

        arb_get_rand_fmpq(x, state, a);
        arb_get_rand_fmpq(y, state, b);

        arb_submul(a, a, b);
        fmpq_submul(x, x, y);

        if (!arb_contains_fmpq(a, x))
        {
            printf("FAIL: aliasing (c, a)\n\n");
            printf("a = "); arb_debug(a); printf("\n\n");
            printf("x = "); fmpq_print(x); printf("\n\n");
            printf("b = "); arb_debug(b); printf("\n\n");
            printf("y = "); fmpq_print(y); printf("\n\n");
            abort();
        }

        arb_clear(a);
        arb_clear(b);

        fmpq_clear(x);
        fmpq_clear(y);
    }

    /* aliasing of c and b */
    for (iter = 0; iter < 10000; iter++)
    {
        arb_t a, b;
        fmpq_t x, y;

        arb_init(a, n_randint(state, 200));
        arb_init(b, n_randint(state, 200));

        fmpq_init(x);
        fmpq_init(y);

        arb_randtest(a, state, 10);
        arb_randtest(b, state, 10);

        arb_get_rand_fmpq(x, state, a);
        arb_get_rand_fmpq(y, state, b);

        arb_submul(b, a, b);
        fmpq_submul(y, x, y);

        if (!arb_contains_fmpq(b, y))
        {
            printf("FAIL: aliasing (c, b)\n\n");
            printf("a = "); arb_debug(a); printf("\n\n");
            printf("x = "); fmpq_print(x); printf("\n\n");
            printf("b = "); arb_debug(b); printf("\n\n");
            printf("y = "); fmpq_print(y); printf("\n\n");
            abort();
        }

        arb_clear(a);
        arb_clear(b);

        fmpq_clear(x);
        fmpq_clear(y);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}