/*	ASCEND modelling environment
	Copyright (C) 2006 Carnegie Mellon University

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WEBCXX_INSTANCE_H
#define WEBCXX_INSTANCE_H

#include "symchar.h"
#include "type.h"
#include "set.h"
#include "dimensions.h"

class Plot;

#include "config.h"
extern "C"{
#include <ascend/general/platform.h>
#include <ascend/utilities/error.h>
#include <ascend/compiler/expr_types.h>
#include <ascend/compiler/instance_enum.h>
#include <ascend/compiler/atomvalue.h>
#include <ascend/compiler/plot.h>
}

#include <string>
#include <vector>
#ifdef __EMSCRIPTEN__
#include "lists.h"
list_vector(aw_Instance);
#endif


typedef enum{
	webcxx_INST_STATUS_UNKNOWN=0,
	webcxx_VAR_FIXED,
	webcxx_VAR_UNSOLVED,
	webcxx_VAR_ACTIVE,
	webcxx_VAR_SOLVED,
	webcxx_REL_INACTIVE
} InstanceStatus;

/**
	This class has to be called 'Instanc' in C++ to avoid a name clash
	with C. Maybe coulda done it with namespaces but didn't know how.

	This class may be renamed back to 'Instance' by SWIG, so use 'Instance'
	when you're in javascript.

	The Right Way to implement this class would be as a base class
	with lots of diffent subclasses for the different atom types.
	Maybe even multiple inheritance.

	But until the underlying C code is ported to C++ or modularised in
	some other way, it's not going to be worth the effort. We discussed
	this in the mailing list.
*/
class aw_Instance{
protected:
	struct Instance *i;
	SymChar name;
	std::vector<aw_Instance> children;
	void setName(SymChar);
	static SymChar fixedsym;
	static SymChar solvervarsym;
public:
	aw_Instance();
	aw_Instance(Instance *i);
	aw_Instance(Instance *i, const SymChar &name);
	aw_Instance(const aw_Instance &parent, const unsigned long &childnum);
	aw_Instance(const aw_Instance&);
	~aw_Instance();
	std::vector<aw_Instance> &getChildren();
	aw_Instance getChild(const SymChar &) const;
	aw_Instance getChild(const long &) const;
	const enum inst_t getKind() const;
	const std::string getKindStr() const;
	const Type getType() const;
	const bool isAtom() const;
	const bool isFixed() const;
	const bool isIncluded() const;

	const bool isFund() const;
	const bool isConst() const;
	const bool isCompound() const;
	const bool isRelation() const;
	const bool isLogicalRelation() const;
	const bool isWhen() const;
	const bool isSet() const;
	const bool isSetInt() const;
	const bool isSetString() const;
	const bool isSetEmpty() const; // set of of type 'empty', NB not same as SetInt::length()==0
	const bool isArray() const;
	const bool isDefined() const;
	const bool isChildless() const;
	const bool isBool() const;
	const bool isInt() const;
	const bool isSymbol() const;
	const bool isReal() const;
	const bool isAssigned() const;
	const bool isModel() const;
	const SymChar &getName() const;
	const double getRealValue() const;
	const bool isDimensionless() const;
	const Dimensions getDimensions() const;
	const bool getBoolValue() const;
	const long getIntValue() const;
	const SymChar getSymbolValue() const;

	const std::string getWhenAsString(const aw_Instance &relative_to) const;
	const std::string getLogrelAsString(const aw_Instance &relative_to) const;
	const std::string getValueAsString() const;
	const std::string getRelationAsString(const aw_Instance &relative_to) const;
	Plot getPlot() const;

	const bool isPlottable() const;

	void setFixed(const bool &val=true);
	void setIncluded(const bool &val=true);
	void setBoolValue(const bool&, const unsigned &depth=0);
	void setIntValue(const long&, const unsigned &depth=0);
	void setRealValue(const double&, const unsigned &depth=0);
	void setRealValueWithUnits(double, const char *, const unsigned &depth=0);
	void setSymbolValue(const SymChar &);

	template<class T>
	const WEBCXX_Set<T> getSetValue() const{
		if(!isSet()){
			ERROR_REPORTER_NOLINE(ASC_USER_ERROR,"Variable '%s' is not set-valued",getName().toString());
			return WEBCXX_Set<T>();
		}
		if(!isConst() && !isDefined()){
			ERROR_REPORTER_NOLINE(ASC_USER_ERROR,"Variable '%s' is not defined",getName().toString());
			return WEBCXX_Set<T>();
		}
		return WEBCXX_Set<T>(SetAtomList(i));
	}

	const enum set_kind getSetType() const;
	void write(const char *fname) const;
	Instance *getInternalType() const;

	void setStatus(const InstanceStatus &); ///< make this one private, just for friend Simulation?
	const InstanceStatus getStatus() const;

	void setLowerBound(const double &);
	void setUpperBound(const double &);
	void setNominal(const double &);
	const double getLowerBound() const;
	const double  getUpperBound() const;
	const double  getNominal() const;

	const std::vector<aw_Instance> getClique() const;
	const std::vector<std::string> getAliases() const;

	const double getResidual() const;
	const bool getLogicalResidual() const;
#ifdef __EMSCRIPTEN__
	aw_Instance_list getChildren_list();
	aw_Instance_list getClique_list() const;
	list_stdstring getAliases_list() const;
#endif
};

#endif

