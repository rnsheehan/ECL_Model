#ifndef ATTACH_H
#include "Attach.h"

// Declaration of the class Laser_Model
// class will be used to compute the parameters of an External Cavity Laser
// The model for an ECL LI curve is described in the paper
// Power-efficient {III-V/Silicon} external cavity {DBR} lasers, Zilkie et al, Opt. Expr., 20 (21), 2012
// R. Sheehan 16 - 10 - 2018

ec_laser::ec_laser()
{
	// default constructor
	L = Lgout = 0.0;
	Rg = Rr = Reff = 0.0;
	eta = etad = etai = etaext = 0.0;
	alpha = alphag = 0.0;
	Pdc = PsatT = Ib = Vb = ZT = 0.0; // laser thermal impedance
}

void ec_laser::set_params()
{
	
}

#endif // !ATTACH_H