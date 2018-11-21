#ifndef LASER_MODEL_H
#define LASER_MODEL_H

// Declaration of the class Laser_Model
// class will be used to compute the parameters of an External Cavity Laser
// The model for an ECL LI curve is described in the paper
// Power-efficient {III-V/Silicon} external cavity {DBR} lasers, Zilkie et al, Opt. Expr., 20 (21), 2012
// R. Sheehan 16 - 10 - 2018

// Going to group the various parameters into their own objects since there's too many parameters for a single class

// Length parameters

class lengths {
public:
	lengths(); // default constructor
	lengths(double laser_length, double grating_length);
	lengths(const lengths &ll); // copy constructor

	void set_params(double laser_length, double grating_length);

	// getters
	inline double get_L() { return L;  }
	inline double get_Lg() { return Lgout; }

private:
	double L; // effective laser cavity length
	double Lgout; // length of grating outside cavity
};

// Reflection Parameters

class reflections {
public:
	reflections();
	reflections(double peak_grating_ref, double rsoa_hr_ref);
	reflections(const reflections &rr); 

	void set_params(double peak_grating_ref, double rsoa_hr_ref);

	// getters
	inline double get_Rg() { return Rg;  }
	inline double get_Rr() { return Rr;  }
	inline double get_rtRr() { return rtRr;  }

private:
	double Rg; // peak grating reflectance
	double Rr; // RSOA rear facet reflectance
	double rtRr; // square root of Rr
};

// Scattering and loss parameters

class losses {
public:
	losses();
	losses(double sct_loss, double gr_loss);
	losses(const losses &ll);

	void set_params(double sct_loss, double gr_loss);

	inline double get_alpha() { return alpha; }
	inline double get_alphag() { return alphag;  }

private:
	double alpha; // effective waveguide scattering loss
	double alphag; // grating loss
};

// DC Laser Parameters

class dcvals {
public:
	dcvals(); 
	dcvals(double Rth, double curr_th);
	dcvals(const dcvals &dd); 

	void set_params(double Rth, double curr_th); 

	inline double get_Zt() { return ZT;  }
	inline double get_Ith() { return Ith;  }

private:
	
	double ZT; // laser thermal impedance
	double Ith; // laser threshold current
	
}; 

// Model for the ECL LI curve

class ec_laser {
public:
	ec_laser(); 
	ec_laser(double &coupEff, double &intQE, lengths &theLength, reflections &theRefs, losses &theLoss, dcvals &theDC);

	void set_params(double &coupEff, double &intQE, lengths &theLength, reflections &theRefs, losses &theLoss, dcvals &theDC);

	double Pout(double wavelength, double current); 

	double Pout(double wavelength, double current, double T, double gamma, double aa, double T0, double T1);

private:	
	double f(double T, double gamma, double aa); 

private:
	// there's going to be a lot of parameters
	double eta; // waveguide coupling efficiency
	double etad; // differential quantum efficiency
	double etai; // internal quantum efficiency
	double etaext; // external quantum efficiency
	double Reff; // effective grating reflectance
	double Rprime; // transmission parameter account for coupling efficiency
	double Rprod; // reflection coefficient product
	double RQfactor; // Combination of reflection coefficients and quantum efficiency
	double PsatT; // laser thermal saturation power
	double Ib; // DC current supplied to RSOA
	double Vb; // DC voltage across RSOA
	double Pdc; // DC power supplied to RSOA

	lengths Lvals; // cavity and grating lengths

	reflections Rvals; // reflection coefficients

	losses Avals; // scattering and loss coefficients

	dcvals DCvals; // dc parameters for the laser
};

#endif