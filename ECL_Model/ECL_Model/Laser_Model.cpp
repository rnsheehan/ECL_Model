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
	eta = Reff = etaext = 0.0;
}

ec_laser::ec_laser(double &coupEff, lengths &theLength, quanteff &theEta, reflections &theRefs, losses &theLoss, dcvals &theDC)
{
	set_params(coupEff, theLength, theEta, theRefs, theLoss, theDC); 
}

void ec_laser::set_params(double &coupEff, lengths &theLength, quanteff &theEta, reflections &theRefs, losses &theLoss, dcvals &theDC)
{
	try {
		
		if (coupEff > 0.0 && coupEff < 1.1) {
			Lvals = theLength;
			Qvals = theEta;
			Rvals = theRefs;
			Avals = theLoss;
			DCvals = theDC;

			eta = coupEff;
			Reff = template_funcs::DSQR(eta) * Rvals.get_Rg();
			etaext = Qvals.get_etad() * Qvals.get_etai() * exp(Avals.get_alphag() * Lvals.get_Lg());
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
	Rg = Rr = 0.0;
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
			Rg = peak_grating_ref; Rr = rsoa_hr_ref;
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

quanteff::quanteff()
{
	// default constructor	
	etad = etai = 0.0;
}

quanteff::quanteff(double diff_qe, double int_qe)
{
	set_params(diff_qe, int_qe);
}

quanteff::quanteff(const quanteff &qq)
{
	// copy constructor

	try {
		etad = qq.etad; etai = qq.etai; 
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

void quanteff::set_params(double diff_qe, double int_qe)
{
	try {
		if (diff_qe > 0.0 && int_qe > 0.0 && diff_qe < 1.0 && int_qe < 1.0) {
			etad = diff_qe; etai = int_qe;
		}
		else {
			std::string reason = "Error: quanteff::set_params(double diff_qe, double int_qe)s\n";
			reason += "Input laser quantum efficiencies are not correct\n";
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
	Pdc = PsatT = Ib = Vb = ZT = 0.0;
}

dcvals::dcvals(double curr_val, double volt_val, double Rth, double curr_th)
{
	set_params(curr_val, volt_val, Rth, curr_th); 
}

dcvals::dcvals(const dcvals &dd)
{
	try {
		Ib = dd.Ib; Vb = dd.Vb; ZT = dd.ZT; Ib = dd.Ib; 
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

void dcvals::set_params(double curr_val, double volt_val, double Rth, double curr_th)
{
	try {
		if (curr_val > 0.0 && volt_val > 0.0 && Rth > 0.0 && curr_th > 0.0) {
			Ib = curr_val; Vb = volt_val; ZT = Rth; Ib = curr_th;
		}
		else {
			std::string reason = "Error: dcvals::set_params(double curr_val, double volt_val, double Rth, double curr_th)\n";
			reason += "Input laser dc vals are not correct\n";
			throw std::runtime_error(reason);
		}
	}
	catch (std::runtime_error &e) {
		std::cerr << e.what();
	}
}

#endif // !ATTACH_H