#ifndef WEBCXX_CURVE_H
#define WEBCXX_CURVE_H

#include <vector>
#include <string>

#include "plot.h"
#include "instance.h"
#include "lists.h"

class Curve : public aw_Instance{
	friend class std::vector<Curve>;
private:
	friend class Plot;
	explicit Curve(const aw_Instance &);	
public:
	Curve(); // hack -- this was previously private. not sure what this breaks.
	Curve(const Curve &);
	std::vector<double> x;
	std::vector<double> y;
#ifdef __EMSCRIPTEN__
	double* get_x();
	double* get_y();
#endif
	

	const std::string getLegend() const;

	/**
		Get the curve format string, as used by matplotlib.
		Documented here:
		http://matplotlib.sourceforge.net/api/axes_api.html#matplotlib.axes.Axes.plot

		(This value is not used by the Tcl/Tk GUI.)
	*/
	const std::string getFormat() const;
};

#endif
