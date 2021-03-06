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
    Copyright (C) 2015 Arb authors

******************************************************************************/

#include "acb.h"

void
acb_fprintd(FILE * file, const acb_t z, slong digits)
{
    flint_fprintf(file, "(");
    arf_fprintd(file, arb_midref(acb_realref(z)), digits);

    if (arf_sgn(arb_midref(acb_imagref(z))) < 0)
    {
        arf_t t;
        arf_init_neg_shallow(t, arb_midref(acb_imagref(z)));
        flint_fprintf(file, " - ");
        arf_fprintd(file, t, digits);
    }
    else
    {
        flint_fprintf(file, " + ");
        arf_fprintd(file, arb_midref(acb_imagref(z)), digits);
    }
    flint_fprintf(file, "j)");

    flint_fprintf(file, "  +/-  ");

    flint_fprintf(file, "(");
    mag_fprintd(file, arb_radref(acb_realref(z)), 3);
    flint_fprintf(file, ", ");
    mag_fprintd(file, arb_radref(acb_imagref(z)), 3);
    flint_fprintf(file, "j)");
}
