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

#include "arb.h"

int main()
{
    slong iter;
    flint_rand_t state;

    flint_printf("const_glaisher....");
    fflush(stdout);
    flint_randinit(state);

    for (iter = 0; iter < 250; iter++)
    {
        arb_t r, s, t;
        fmpz_t v;
        slong accuracy, prec;

        prec = 2 + n_randint(state, 2000);

        arb_init(r);
        arb_init(s);
        arb_init(t);
        fmpz_init(v);

        arb_const_glaisher(r, prec);
        arb_const_glaisher(s, prec + 100);

        if (!arb_overlaps(r, s))
        {
            flint_printf("FAIL: containment\n\n");
            flint_printf("prec = %wd\n", prec);
            flint_printf("r = "); arb_printd(r, prec / 3.33); flint_printf("\n\n");
            abort();
        }

        accuracy = arb_rel_accuracy_bits(r);

        if (accuracy < prec - 4)
        {
            flint_printf("FAIL: poor accuracy\n\n");
            flint_printf("prec = %wd\n", prec);
            flint_printf("r = "); arb_printd(r, prec / 3.33); flint_printf("\n\n");
            abort();
        }

        if (n_randint(state, 30) == 0)
        {
            flint_cleanup();
        }

        fmpz_set_str(v, "128242712910062263687534256886979172776768892732500", 10);
        arb_set_fmpz(t, v);
        mag_one(arb_radref(t));
        fmpz_ui_pow_ui(v, 10, 50);
        arb_div_fmpz(t, t, v, 170);

        if (!arb_overlaps(r, t))
        {
            flint_printf("FAIL: reference value\n\n");
            flint_printf("prec = %wd\n", prec);
            flint_printf("r = "); arb_printd(r, prec / 3.33); flint_printf("\n\n");
            abort();
        }

        arb_clear(r);
        arb_clear(s);
        arb_clear(t);
        fmpz_clear(v);
    }

    flint_randclear(state);
    flint_cleanup();
    flint_printf("PASS\n");
    return EXIT_SUCCESS;
}

