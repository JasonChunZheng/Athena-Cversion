#ifndef RSOLVERS_PROTOTYPES_H
#define RSOLVERS_PROTOTYPES_H 
#include "../copyright.h"
/*==============================================================================
 * FILE: prototypes.h
 *
 * PURPOSE: Prototypes for all public functions in the src/rsolvers
 *   directory.
 *============================================================================*/

#include <stdio.h>
#include <stdarg.h>
#include "../athena.h"
#include "../defs.h"

#include "../config.h"

/*----------------------------------------------------------------------------*/
/* esystem_*.c */

void esys_roe_iso_hyd(const Real v1, const Real v2, const Real v3,
  Real eigenvalues[],
  Real right_eigenmatrix[][4], Real left_eigenmatrix[][4]);

void esys_roe_adb_hyd(const Real v1, const Real v2, const Real v3,
  const Real h, Real eigenvalues[],
  Real right_eigenmatrix[][5], Real left_eigenmatrix[][5]);

void esys_roe_iso_mhd(const Real d, const Real v1, const Real v2,
  const Real v3, const Real b1, const Real b2, const Real b3,
  const Real x, const Real y, Real eigenvalues[],
  Real right_eigenmatrix[][6], Real left_eigenmatrix[][6]);

void esys_roe_adb_mhd(const Real d, const Real v1, const Real v2,
  const Real v3, const Real h, const Real b1, const Real b2, const Real b3,
  const Real x, const Real y, Real eigenvalues[],
  Real right_eigenmatrix[][7], Real left_eigenmatrix[][7]);

/*----------------------------------------------------------------------------*/
/* All of the Riemann solvers in this directory contain the same function name
 */

void fluxes(const Cons1D Ul, const Cons1D Ur,
            const Prim1D Wl, const Prim1D Wr,
            MHDARG(const Real Bxi,) Cons1D *pF);

#endif /* RSOLVERS_PROTOTYPES_H */