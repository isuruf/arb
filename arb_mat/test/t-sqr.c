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

    Copyright (C) 2015 Arb authors

******************************************************************************/

#include "arb_mat.h"


int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("sqr....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        slong n, qbits1, rbits1, rbits2;
        fmpq_mat_t A, B;
        arb_mat_t a, b, c;

        qbits1 = 2 + n_randint(state, 200);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);

        n = n_randint(state, 10);

        fmpq_mat_init(A, n, n);
        fmpq_mat_init(B, n, n);

        arb_mat_init(a, n, n);
        arb_mat_init(b, n, n);
        arb_mat_init(c, n, n);

        fmpq_mat_randtest(A, state, qbits1);
        fmpq_mat_mul(B, A, A);

        arb_mat_set_fmpq_mat(a, A, rbits1);
        arb_mat_sqr(b, a, rbits2);

        if (!arb_mat_contains_fmpq_mat(b, B))
        {
            flint_printf("FAIL\n\n");
            flint_printf("n = %wd, bits2 = %wd\n", n, rbits2);

            flint_printf("A = "); fmpq_mat_print(A); flint_printf("\n\n");
            flint_printf("B = "); fmpq_mat_print(B); flint_printf("\n\n");

            flint_printf("a = "); arb_mat_printd(a, 15); flint_printf("\n\n");
            flint_printf("b = "); arb_mat_printd(b, 15); flint_printf("\n\n");

            abort();
        }

        /* test aliasing */
        arb_mat_set(c, a);
        arb_mat_sqr(c, c, rbits2);
        if (!arb_mat_equal(c, b))
        {
            flint_printf("FAIL (aliasing)\n\n");
            abort();
        }

        fmpq_mat_clear(A);
        fmpq_mat_clear(B);

        arb_mat_clear(a);
        arb_mat_clear(b);
        arb_mat_clear(c);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}
