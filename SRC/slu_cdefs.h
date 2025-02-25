/*! \file
Copyright (c) 2003, The Regents of the University of California, through
Lawrence Berkeley National Laboratory (subject to receipt of any required 
approvals from U.S. Dept. of Energy) 

All rights reserved. 

The source code is distributed under BSD license, see the file License.txt
at the top-level directory.
*/

/*! @file slu_cdefs.h
 * \brief Header file for real operations
 * 
 * <pre> 
 * -- SuperLU routine (version 4.1) --
 * Univ. of California Berkeley, Xerox Palo Alto Research Center,
 * and Lawrence Berkeley National Lab.
 * November, 2010
 * 
 * Global data structures used in LU factorization -
 * 
 *   nsuper: #supernodes = nsuper + 1, numbered [0, nsuper].
 *   (xsup,supno): supno[i] is the supernode no to which i belongs;
 *	xsup(s) points to the beginning of the s-th supernode.
 *	e.g.   supno 0 1 2 2 3 3 3 4 4 4 4 4   (n=12)
 *	        xsup 0 1 2 4 7 12
 *	Note: dfs will be performed on supernode rep. relative to the new 
 *	      row pivoting ordering
 *
 *   (xlsub,lsub): lsub[*] contains the compressed subscript of
 *	rectangular supernodes; xlsub[j] points to the starting
 *	location of the j-th column in lsub[*]. Note that xlsub 
 *	is indexed by column.
 *	Storage: original row subscripts
 *
 *      During the course of sparse LU factorization, we also use
 *	(xlsub,lsub) for the purpose of symmetric pruning. For each
 *	supernode {s,s+1,...,t=s+r} with first column s and last
 *	column t, the subscript set
 *		lsub[j], j=xlsub[s], .., xlsub[s+1]-1
 *	is the structure of column s (i.e. structure of this supernode).
 *	It is used for the storage of numerical values.
 *	Furthermore,
 *		lsub[j], j=xlsub[t], .., xlsub[t+1]-1
 *	is the structure of the last column t of this supernode.
 *	It is for the purpose of symmetric pruning. Therefore, the
 *	structural subscripts can be rearranged without making physical
 *	interchanges among the numerical values.
 *
 *	However, if the supernode has only one column, then we
 *	only keep one set of subscripts. For any subscript interchange
 *	performed, similar interchange must be done on the numerical
 *	values.
 *
 *	The last column structures (for pruning) will be removed
 *	after the numercial LU factorization phase.
 *
 *   (xlusup,lusup): lusup[*] contains the numerical values of the
 *	rectangular supernodes; xlusup[j] points to the starting
 *	location of the j-th column in storage vector lusup[*]
 *	Note: xlusup is indexed by column.
 *	Each rectangular supernode is stored by column-major
 *	scheme, consistent with Fortran 2-dim array storage.
 *
 *   (xusub,ucol,usub): ucol[*] stores the numerical values of
 *	U-columns outside the rectangular supernodes. The row
 *	subscript of nonzero ucol[k] is stored in usub[k].
 *	xusub[i] points to the starting location of column i in ucol.
 *	Storage: new row subscripts; that is subscripts of PA.
 * </pre>
 */
#ifndef __SUPERLU_cSP_DEFS /* allow multiple inclusions */
#define __SUPERLU_cSP_DEFS

/*
 * File name:		csp_defs.h
 * Purpose:             Sparse matrix types and function prototypes
 * History:
 */

#ifdef _CRAY
#include <fortran.h>
#endif

/* Define my integer type int_t */
#ifdef _LONGINT
typedef long long int int_t;
#define IFMT "%lld"
#else
typedef int int_t; /* default */
#define IFMT "%8d"
#endif

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "slu_Cnames.h"
#include "supermatrix.h"
#include "slu_util.h"
#include "slu_scomplex.h"


/* -------- Prototypes -------- */

#ifdef __cplusplus
extern "C" {
#endif

/*! \brief Driver routines */
extern void
cgssv(superlu_options_t *, SuperMatrix *, int_t *, int_t *, SuperMatrix *,
      SuperMatrix *, SuperMatrix *, SuperLUStat_t *, int_t *);
extern void
cgssvx(superlu_options_t *, SuperMatrix *, int_t *, int_t *, int_t *,
       char *, float *, float *, SuperMatrix *, SuperMatrix *,
       void *, int_t, SuperMatrix *, SuperMatrix *,
       float *, float *, float *, float *,
       GlobalLU_t *, mem_usage_t *, SuperLUStat_t *, int_t *);
    /* ILU */
extern void
cgsisv(superlu_options_t *, SuperMatrix *, int *, int *, SuperMatrix *,
      SuperMatrix *, SuperMatrix *, SuperLUStat_t *, int *);
extern void
cgsisx(superlu_options_t *, SuperMatrix *, int_t *, int_t *, int_t *,
       char *, float *, float *, SuperMatrix *, SuperMatrix *,
       void *, int_t, SuperMatrix *, SuperMatrix *, float *, float *,
       GlobalLU_t *, mem_usage_t *, SuperLUStat_t *, int_t *);


/*! \brief Supernodal LU factor related */
extern void
cCreate_CompCol_Matrix(SuperMatrix *, int_t, int_t, int_t, complex *,
		       int_t *, int_t *, Stype_t, Dtype_t, Mtype_t);
extern void
cCreate_CompRow_Matrix(SuperMatrix *, int_t, int_t, int_t, complex *,
		       int_t *, int_t *, Stype_t, Dtype_t, Mtype_t);
extern void
cCopy_CompCol_Matrix(SuperMatrix *, SuperMatrix *);
extern void
cCreate_Dense_Matrix(SuperMatrix *, int_t, int_t, complex *, int_t,
		     Stype_t, Dtype_t, Mtype_t);
extern void
cCreate_SuperNode_Matrix(SuperMatrix *, int_t, int_t, int_t, complex *, 
		         int_t *, int_t *, int_t *, int_t *, int_t *,
			 Stype_t, Dtype_t, Mtype_t);
extern void
cCopy_Dense_Matrix(int_t, int_t, complex *, int_t, complex *, int_t);

extern void    countnz (const int_t, int_t *, int_t *, int_t *, GlobalLU_t *);
extern void    ilu_countnz (const int_t, int_t *, int_t *, GlobalLU_t *);
extern void    fixupL (const int_t, const int_t *, GlobalLU_t *);

extern void    callocateA (int_t, int_t, complex **, int_t **, int_t **);
extern void    cgstrf (superlu_options_t*, SuperMatrix*,
                       int_t, int_t, int_t*, void *, int_t, int_t *, int_t *, 
                       SuperMatrix *, SuperMatrix *, GlobalLU_t *,
		       SuperLUStat_t*, int_t *);
extern int_t     csnode_dfs (const int_t, const int_t, const int_t *, const int_t *,
			     const int_t *, int_t *, int_t *, GlobalLU_t *);
extern int_t     csnode_bmod (const int_t, const int_t, const int_t, complex *,
                              complex *, GlobalLU_t *, SuperLUStat_t*);
extern void    cpanel_dfs (const int_t, const int_t, const int_t, SuperMatrix *,
			   int_t *, int_t *, complex *, int_t *, int_t *, int_t *,
			   int_t *, int_t *, int_t *, int_t *, GlobalLU_t *);
extern void    cpanel_bmod (const int_t, const int_t, const int_t, const int_t,
                           complex *, complex *, int_t *, int_t *,
			   GlobalLU_t *, SuperLUStat_t*);
extern int_t     ccolumn_dfs (const int_t, const int_t, int_t *, int_t *, int_t *, int_t *,
			   int_t *, int_t *, int_t *, int_t *, int_t *, GlobalLU_t *);
extern int_t     ccolumn_bmod (const int_t, const int_t, complex *,
			   complex *, int_t *, int_t *, int_t,
                           GlobalLU_t *, SuperLUStat_t*);
extern int_t     ccopy_to_ucol (int_t, int_t, int_t *, int_t *, int_t *,
                              complex *, GlobalLU_t *);         
extern int_t     cpivotL (const int_t, const double, int_t *, int_t *, 
                         int_t *, int_t *, int_t *, GlobalLU_t *, SuperLUStat_t*);
extern void    cpruneL (const int_t, const int_t *, const int_t, const int_t,
			  const int_t *, const int_t *, int_t *, GlobalLU_t *);
extern void    creadmt (int *, int *, int *, complex **, int **, int **);
extern void    cGenXtrue (int, int, complex *, int);
extern void    cFillRHS (trans_t, int, complex *, int, SuperMatrix *,
			  SuperMatrix *);
extern void    cgstrs (trans_t, SuperMatrix *, SuperMatrix *, int_t *, int_t *,
                        SuperMatrix *, SuperLUStat_t*, int_t *);
/* ILU */
extern void    cgsitrf (superlu_options_t*, SuperMatrix*, int_t, int_t, int_t*,
		        void *, int_t, int_t *, int_t *, SuperMatrix *, SuperMatrix *,
                        GlobalLU_t *, SuperLUStat_t*, int_t *);
extern int     cldperm(int_t, int_t, int_t, int_t [], int_t [], complex [],
                        int_t [],	float [], float []);
extern int_t     ilu_csnode_dfs (const int_t, const int_t, const int_t *, const int_t *,
			       const int_t *, int_t *, GlobalLU_t *);
extern void    ilu_cpanel_dfs (const int_t, const int_t, const int_t, SuperMatrix *,
			       int_t *, int_t *, complex *, float *, int_t *, int_t *,
			       int_t *, int_t *, int_t *, int_t *, GlobalLU_t *);
extern int_t     ilu_ccolumn_dfs (const int_t, const int_t, int_t *, int_t *, int_t *,
				int_t *, int_t *, int_t *, int_t *, int_t *,
				GlobalLU_t *);
extern int_t     ilu_ccopy_to_ucol (int_t, int_t, int_t *, int_t *, int_t *,
                                  complex *, int_t, milu_t, double, int_t,
                                  complex *, int_t *, GlobalLU_t *, float *);
extern int_t     ilu_cpivotL (const int_t, const double, int_t *, int_t *, int_t, int_t *,
			    int_t *, int_t *, int_t *, double, milu_t,
                            complex, GlobalLU_t *, SuperLUStat_t*);
extern int_t     ilu_cdrop_row (superlu_options_t *, int_t, int_t, double,
                              int_t, int_t *, double *, GlobalLU_t *, 
                              float *, float *, int_t);


/*! \brief Driver related */

extern void    cgsequ (SuperMatrix *, float *, float *, float *,
			float *, float *, int_t *);
extern void    claqgs (SuperMatrix *, float *, float *, float,
                        float, float, char *);
extern void    cgscon (char *, SuperMatrix *, SuperMatrix *, 
		         float, float *, SuperLUStat_t*, int_t *);
extern float   cPivotGrowth(int_t, SuperMatrix *, int_t *, 
                            SuperMatrix *, SuperMatrix *);
extern void    cgsrfs (trans_t, SuperMatrix *, SuperMatrix *,
                       SuperMatrix *, int_t *, int_t *, char *, float *, 
                       float *, SuperMatrix *, SuperMatrix *,
                       float *, float *, SuperLUStat_t*, int_t *);

extern int     sp_ctrsv (char *, char *, char *, SuperMatrix *,
			SuperMatrix *, complex *, SuperLUStat_t*, int *);
extern int     sp_cgemv (char *, complex, SuperMatrix *, complex *,
			int, complex, complex *, int);

extern int     sp_cgemm (char *, char *, int, int, int, complex,
			SuperMatrix *, complex *, int, complex, 
			complex *, int);
extern         float smach(char *);   /* from C99 standard, in float.h */

/*! \brief Memory-related */
extern int_t     cLUMemInit (fact_t, void *, int_t, int_t, int_t, int_t, int_t,
                            float, SuperMatrix *, SuperMatrix *,
                            GlobalLU_t *, int_t **, complex **);
extern void    cSetRWork (int_t, int_t, complex *, complex **, complex **);
extern void    cLUWorkFree (int_t *, complex *, GlobalLU_t *);
extern int_t     cLUMemXpand (int_t, int_t, MemType, int_t *, GlobalLU_t *);

extern complex  *complexMalloc(int_t);
extern complex  *complexCalloc(int_t);
extern float  *floatMalloc(int_t);
extern float  *floatCalloc(int_t);
extern int_t     cmemory_usage(const int_t, const int_t, const int_t, const int_t);
extern int_t     cQuerySpace (SuperMatrix *, SuperMatrix *, mem_usage_t *);
extern int_t     ilu_cQuerySpace (SuperMatrix *, SuperMatrix *, mem_usage_t *);

/*! \brief Auxiliary routines */
extern void    creadhb(FILE *, int_t *, int_t *, int_t *, complex **, int_t **, int_t **);
extern void    creadrb(int_t *, int_t *, int_t *, complex **, int_t **, int_t **);
extern void    creadtriple(int_t *, int_t *, int_t *, complex **, int_t **, int_t **);
extern void    creadMM(FILE *, int_t *, int_t *, int_t *, complex **, int_t **, int_t **);
extern void    cCompRow_to_CompCol(int_t, int_t, int_t, complex*, int_t*, int_t*,
		                   complex **, int_t **, int_t **);
extern void    cfill (complex *, int, complex);
extern void    cinf_norm_error (int, SuperMatrix *, complex *);
extern float  sqselect(int, float *, int);


/*! \brief Routines for debugging */
extern void    cPrint_CompCol_Matrix(char *, SuperMatrix *);
extern void    cPrint_SuperNode_Matrix(char *, SuperMatrix *);
extern void    cPrint_Dense_Matrix(char *, SuperMatrix *);
extern void    cprint_lu_col(char *, int, int, int *, GlobalLU_t *);
extern int     print_double_vec(char *, int, double *);
extern void    ccheck_tempv(int, complex *);

/*! \brief BLAS */

extern int cgemm_(const char*, const char*, const int*, const int*, const int*,
                  const complex*, const complex*, const int*, const complex*,
		  const int*, const complex*, complex*, const int*);
extern int ctrsv_(char*, char*, char*, int*, complex*, int*,
                  complex*, int*);
extern int ctrsm_(char*, char*, char*, char*, int*, int*,
                  complex*, complex*, int*, complex*, int*);
extern int cgemv_(char *, int *, int *, complex *, complex *a, int *,
                  complex *, int *, complex *, complex *, int *);

extern void cusolve(int, int, complex*, complex*);
extern void clsolve(int, int, complex*, complex*);
extern void cmatvec(int, int, int, complex*, complex*, complex*);

#ifdef __cplusplus
  }
#endif

#endif /* __SUPERLU_cSP_DEFS */

