/*! \file
Copyright (c) 2003, The Regents of the University of California, through
Lawrence Berkeley National Laboratory (subject to receipt of any required 
approvals from U.S. Dept. of Energy) 

All rights reserved. 

The source code is distributed under BSD license, see the file License.txt
at the top-level directory.
*/
/*! @file mark_relax.c
 * \brief Record the rows pivoted by the relaxed supernodes.
 *
 * <pre>
 * -- SuperLU routine (version 4.0) --
 * Lawrence Berkeley National Laboratory
 * June 1, 2009
 * <\pre>
 */
#include "slu_ddefs.h"

/*! \brief
 *
 * <pre>
 * Purpose
 * =======
 *    mark_relax() - record the rows used by the relaxed supernodes.
 * </pre>
 */
int_t mark_relax(
	int_t n,		    /* order of the matrix A */
	int_t *relax_end,     /* last column in a relaxed supernode.
			     * if j-th column starts a relaxed supernode,
			     * relax_end[j] represents the last column of
			     * this supernode. */
	int_t *relax_fsupc,   /* first column in a relaxed supernode.
			     * relax_fsupc[j] represents the first column of
			     * j-th supernode. */
	int_t *xa_begin,	    /* Astore->colbeg */
	int_t *xa_end,	    /* Astore->colend */
	int_t *asub,	    /* row index of A */
	int_t *marker	    /* marker[j] is the maximum column index if j-th
			     * row belongs to a relaxed supernode. */ )
{
    register int_t jcol, kcol;
    register int_t i, j, k;

    for (i = 0; i < n && relax_fsupc[i] != EMPTY; i++)
    {
	jcol = relax_fsupc[i];	/* first column */
	kcol = relax_end[jcol]; /* last column */
	for (j = jcol; j <= kcol; j++)
	    for (k = xa_begin[j]; k < xa_end[j]; k++)
		marker[asub[k]] = jcol;
    }
    return i;
}
