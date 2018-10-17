#ifndef ATTACH_H
#include "Attach.h"

// Declaration of the class Laser_Model
// class will be used to compute the parameters of an External Cavity Laser
// The model for an ECL LI curve is described in the paper
// Power-efficient {III-V/Silicon} external cavity {DBR} lasers, Zilkie et al, Opt. Expr., 20 (21), 2012
// R. Sheehan 16 - 10 - 2018

ec_laser::ec_laser()
{
	// Default Constructor
	Pdc = Ib = Vb = eta = etad = etai = Reff = etaext = Rprime = Rprod = RQfactor = PsatT = 0.0;
}

ec_laser::ec_laser(double &coupEff, double &intQE, lengths &theLength, reflections &theRefs, losses &theLoss, dcvals &theDC)
{
	set_params(coupEff, intQE, theLength, theRefs, theLoss, theDC); 
}

void ec_laser::set_params(double &coupEff, double &intQE, lengths &theLength, reflections &theRefs, losses &theLoss, dcvals &theDC)
{
	try {
		
		if (coupEff > 0.0 && coupEff < 1.1 && intQE > 0.0 && intQE < 1.1) {
			Lvals = theLength;
			Rvals = theRefs;
			Avals = theLoss;
			DCvals = theDC;

			eta = coupEff;

			etai = intQE; 

			Reff = template_funcs::DSQR(eta) * Rvals.get_Rg();

			Rprime = 1.0 - Rvals.get_Rg(); 

			Rprod = log(1.0 / (Rvals.get_Rr() * Reff ) ); 

			etad = Rprod / (Rprod + ( 2.0 * Lvals.get_L() * Avals.get_alpha() ) ); 

			etaext = etad * etai * exp( Avals.get_alphag() * Lvals.get_Lg() );

			RQfactor = (etaext * eta * Rprime * Rvals.get_rtRr()) / ( ( 1.0 - Reff)*Rvals.get_rtRr() + ( 1.0 - Rvals.get_Rr() )*sqrt(Reff) );
		}
		else {
			std::string reason = "Error: ec_laser::set_params(double &coupEff, lengths &theLength, quanteff &theEta, reflections &theRefs, losses &theLoss, dcvals &theDC)\n";
			reason += "Coupling Efficiency is not correctly defined\n"; 
			throw( std::runtime_error(reason) ); 
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

double ec_laser::Pout(double wavelength, double current)
{
	// Compute the Pout based on the input parameters
	// wavelength must be in units of nm

	try {
		if (current > 0.0 && wavelength > 1000.0) {
			return ( RQfactor * (1242.38 / wavelength) * ( current - DCvals.get_Ith() ) );
		}
		else {
			return 0.0; 
			std::string reason = "Error: ec_laser::Pout(double wavelength, double current)\n";
			reason += "Incorrect input parameters\n";
			reason += "Wavelength = " + template_funcs::toString(wavelength, 2) + "\n"; 
			reason += "Current = " + template_funcs::toString(current, 2) + "\n"; 
			throw(std::runtime_error(reason));
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
		return 0.0;
	}
}

double ec_laser::Pout(double wavelength, double current, double T, double gamma, double aa, double T0, double T1)
{
	// Model which includes the temperature effects

	// Compute the Pout based on the input parameters
	// wavelength must be in units of nm
	// T0, T1 are values that characterise the LI curve roll off

	try {
		if (current > 0.0 && wavelength > 1000.0) {
			return ( RQfactor * (1242.38 / wavelength) * f(T, gamma, -1.0*T1) * (current - ( DCvals.get_Ith() * f(T, gamma, T0) ) ) );
		}
		else {
			return 0.0;
			std::string reason = "Error: ec_laser::Pout(double wavelength, double current)\n";
			reason += "Incorrect input parameters\n";
			reason += "Wavelength = " + template_funcs::toString(wavelength, 2) + "\n";
			reason += "Current = " + template_funcs::toString(current, 2) + "\n";
			throw(std::runtime_error(reason));
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
		return 0.0;
	}
}

double ec_laser::f(double T, double gamma, double aa)
{
	// define function f(T, a) which characterises thermal roll-off
	// this is necessary to incorporate effects of temperature 
	// function is of the form exp{ f(T, gamma, a) }
	// gamma ia a fitting parameter that contains the effect of thermal impedance and power absorbed by the Rsoa
	// in the Zilkie paper gamma = Z_{T} ( P_{dc} - P_{out} )
	// Z_{T} is the device thermal impedance, P_{dc} is the DC power being used
	// I initially thought that P_{out} was the laser output power, but that doesn't make sense
	// I now think that this is the optical output power from the RSOA

	try {
		if (T > 0 && fabs(aa) > 0.0) {
			double arg = (T + gamma) / aa; 
			return exp(arg); 
		}
		else {
			return 0.0;
			std::string reason = "Error: ec_laser::f(double T, double gamma, double aa)\n";
			reason += "Incorrect input parameters\n";
			reason += "Temperature = " + template_funcs::toString(T, 2) + "\n";
			reason += "aa = " + template_funcs::toString(aa, 2) + "\n";
			throw(std::runtime_error(reason));
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
		return 0.0;
	}
}

// Laser parameter classes

lengths::lengths()
{
	// default constructor
	L = Lgout = 0.0;
}

lengths::lengths(double laser_length, double grating_length)
{
	set_params(laser_length, grating_length); 
}

lengths::lengths(const lengths &ll)
{
	// copy constructor
	try {
		L = ll.L; Lgout = ll.Lgout;
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

void lengths::set_params(double laser_length, double grating_length)
{
	try {
		if (laser_length > 0.0 && grating_length > 0.0) {
			L = laser_length; Lgout = grating_length; 
		}
		else {
			std::string reason = "Error: lengths::set_params(double laser_length, double grating_length)\n"; 
			reason += "Input laser lengths are not correct\n"; 
			throw std::runtime_error(reason); 
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what(); 
	}
}

reflections::reflections()
{
	// default constructor	
	Rg = Rr = rtRr = 0.0;
}

reflections::reflections(double peak_grating_ref, double rsoa_hr_ref)
{
	set_params(peak_grating_ref, rsoa_hr_ref); 
}

reflections::reflections(const reflections &rr)
{
	// Copy constructor
	try {
		Rg = rr.Rg; Rr = rr.Rr; 
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

void reflections::set_params(double peak_grating_ref, double rsoa_hr_ref)
{
	try {
		if (peak_grating_ref > 0.0 && rsoa_hr_ref > 0.0) {
			Rg = peak_grating_ref; Rr = rsoa_hr_ref; rtRr = sqrt(Rr); 
		}
		else {
			std::string reason = "Error: reflections::set_params(double peak_grating_ref, double rsoa_hr_ref)\n";
			reason += "Input laser reflections are not correct\n";
			throw std::runtime_error(reason);
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

losses::losses() 
{
	// Default Constructor
	alpha = alphag = 0.0;
}

losses::losses(double sct_loss, double gr_loss) 
{
	set_params(sct_loss, gr_loss);
}

losses::losses(const losses &ll)
{
	try {
		alpha = ll.alpha; alphag = ll.alphag; 
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

void losses::set_params(double sct_loss, double gr_loss)
{
	try {
		if (sct_loss > 0.0 && gr_loss > 0.0) {
			alpha = sct_loss; alphag = gr_loss;
		}
		else {
			std::string reason = "Error: losses::set_params(double sct_loss, double gr_loss)\n";
			reason += "Input laser losses are not correct\n";
			throw std::runtime_error(reason);
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

dcvals::dcvals()
{
	// Default Constructor
	Ith = ZT = 0.0;
}

dcvals::dcvals(double Rth, double curr_th)
{
	set_params(Rth, curr_th); 
}

dcvals::dcvals(const dcvals &dd)
{
	try {
		ZT = dd.ZT; Ith = dd.Ith; 
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

void dcvals::set_params(double Rth, double curr_th)
{
	try {
		if (Rth > 0.0 && curr_th > 0.0) {
			ZT = Rth; Ith = curr_th;
		}
		else {
			std::string reason = "Error: dcvals::set_params(double Rth, double curr_th)\n";
			reason += "Input laser dc vals are not correct\n";
			throw std::runtime_error(reason);
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

#endif // !ATTACH_H