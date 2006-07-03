#include "copyright.h"
/*==============================================================================
 * FILE: flux_two-shock.c
 *
 * PURPOSE: Computes 1D fluxes using simple two-shock Riemann solver.
 *   Currently only isothermal hydrodynamics has been implemented.  
 *
 * REFERENCES:
 *   E.F. Toro, "Riemann Solvers and numerical methods for fluid dynamics",
 *   2nd ed., Springer-Verlag, Berlin, (1999).
 *
 * CONTAINS PUBLIC FUNCTIONS:
 *   flux_two-shock()
 *============================================================================*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "athena.h"
#include "prototypes.h"

/*----------------------------------------------------------------------------*/
/* flux_two-shock:
 *   Input Arguments:
 *     Bxi = B in direction of slice at cell interface
 *     Ul,Ur = L/R-states of CONSERVED variables at cell interface
 *   Output Arguments:
 *     pFlux = pointer to fluxes of CONSERVED variables at cell interface
 */

void flux_two-shock(const Real Bxi, const Cons1D Ul, const Cons1D Ur, Flux *pF)
{
  Prim1D Wl, Wr;
  Real zl, zc, zr, dc, Vxc, tmp, pbl, pbr;
  Real sl, sr;  /* Left and right going shock velocity */
  Real al, ar;  /* HLL a_l, a_r -> min and max signal velocity */

  if(!(Ul.d > 0.0)||!(Ur.d > 0.0))
    ath_error("[flux_hllc]: Non-positive densities: dl = %e  dr = %e\n",
	      Ul.d, Ur.d);

/*--- Step 1. ------------------------------------------------------------------
 * Convert left- and right- states in conserved to primitive variables.
 */

  pbl = Cons1D_to_Prim1D(&Ul,&Wl,&Bxi);
  pbr = Cons1D_to_Prim1D(&Ur,&Wr,&Bxi);

/*--- Step 2. ------------------------------------------------------------------
 * Compute the max and min wave speeds
 */

  zl = sqrt((double)Wl.d);
  zr = sqrt((double)Wr.d);

  tmp = zl*zr*(Wl.Vx - Wr.Vx)/(2.0*Iso_csound*(zl + zr));
  zc = tmp + sqrt((double)(tmp*tmp + zl*zr));

  Vxc = (Wl.Vx*zl + Wr.Vx*zr)/(zl + zr) + Iso_csound*(zl - zr)/zc;

/* The L/R-going shock velocity */
  sl = Wl.Vx - Iso_csound*zc/zl;
  sr = Wr.Vx + Iso_csound*zc/zr;

/* Min/Max signal velocity, the HLL velocity al */
  al = zc > zl ? sl : Wl.Vx - Iso_csound;
  ar = zc > zr ? sr : Wr.Vx + Iso_csound;

  tmp       = fabs((double)ar);

/*--- Step 3. ------------------------------------------------------------------
 * For supersonic flow, return the upwind flux.
 */

  if(sr <= 0.0){
    pF->d  = Ur.Mx;
    pF->Mx = Ur.Mx*(Wr.Vx) + Wr.d*Iso_csound2;
    pF->My = Ur.My*(Wr.Vx);
    pF->Mz = Ur.Mz*(Wr.Vx);
    return;
  }

  if(sl >= 0.0){
    pF->d  = Ul.Mx;
    pF->Mx = Ul.Mx*(Wl.Vx) + Wl.d*Iso_csound2;
    pF->My = Ul.My*(Wl.Vx);
    pF->Mz = Ul.Mz*(Wl.Vx);
    return;
  }

/*--- Step 4. ------------------------------------------------------------------
 * Calculate the Interface Flux */

  dc = zc*zc;
  if(Vxc >= 0.0){
    pF->d  = dc*Vxc;
    pF->Mx = dc*Vxc*Vxc + dc*Iso_csound2;
    pF->My = dc*Vxc*Wl.Vy;
    pF->Mz = dc*Vxc*Wl.Vz;
  }
  else{
    pF->d  = dc*Vxc;
    pF->Mx = dc*Vxc*Vxc + dc*Iso_csound2;
    pF->My = dc*Vxc*Wr.Vy;
    pF->Mz = dc*Vxc*Wr.Vz;
  }

  return;
}