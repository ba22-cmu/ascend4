#ifndef WEBCXX_UNITS_H
#define WEBCXX_UNITS_H

#include "config.h"
extern "C"{
#include <ascend/general/platform.h>
#include <ascend/compiler/fractions.h>
#include <ascend/compiler/compiler.h>
#include <ascend/compiler/dimen.h>
#include <ascend/compiler/units.h>
}

#include "symchar.h"

class Dimensions;

/**
	This class might be renamed Units for use in javascript
*/
class UnitsM{
private:
	const struct Units *u;
protected:
	const struct Units *getInternalType() const;

public:
	UnitsM();
	UnitsM(const struct Units *u);
	UnitsM(const char *units);

	const SymChar getName() const;
	const Dimensions getDimensions() const;
	const double getConversion() const;
	const bool operator==(const UnitsM &other) const;
};

#endif
