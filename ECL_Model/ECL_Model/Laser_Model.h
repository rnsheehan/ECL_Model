#ifndef LASER_MODEL_H
#define LASER_MODEL_H

// Declaration of the class Laser_Model
// class will be used to compute the parameters of an External Cavity Laser
// The model for an ECL LI curve is described in the paper
// Power-efficient {III-V/Silicon} external cavity {DBR} lasers, Zilkie et al, Opt. Expr., 20 (21), 2012
// R. Sheehan 16 - 10 - 2018

class ec_laser {
public:
	ec_laser(); 

	void set_params(); 
private:
	// define function f(T, a) which characterises thermal roll-off

private:
	// there's going to be a lot of parameters
	double L; // effective laser cavity length
	double Lgout; // length of grating outside cavity

	double Rg; // peak grating reflectance
	double Rr; // RSOA rear facet reflectance
	double Reff; // effective grating reflectance

	double eta; // waveguide coupling efficiency
	double etad; // differential quantum efficiency
	double etai; // internal quantum efficiency
	double etaext; // external quantum efficiency

	double alpha; // effective waveguide scattering loss
	double alphag; // grating loss

	double Pdc; // DC power supplied to RSOA
	double PsatT; // laser thermal saturation power
	double Ib; // DC current supplied to RSOA
	double Vb; // DC voltage across RSOA
	double ZT; // laser thermal impedance
};

#endif