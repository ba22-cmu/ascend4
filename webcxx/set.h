#ifndef WEBCXX_SET_H
#define WEBCXX_SET_H

#include <iostream>
#include <stdexcept>

#include "symchar.h"

#include "config.h"
extern "C"{
#include <ascend/general/platform.h>
#include <ascend/compiler/compiler.h>
#include <ascend/compiler/setinstval.h>
}

/**
	This C++ template defines WEBCXX_Set<long> and ASCXX_Set<SymChar>
	which can hold instance-variable sets (struct set_t) (as opposed
	to 'struct Set' which is different!).

	It has to be a template because the element type is different,
	so the element accessor functions are different.

	In python, this class can be wrapped in a more elegant way, see
	ascend.i for details.
*/

class Empty{
	// empty class for use in WEBCXX_Set<Empty>
};

template<class T>
class WEBCXX_Set{
private:
	const struct set_t *s;
public:

	WEBCXX_Set(){
		throw std::runtime_error("Can't create new WEBCXX_Set objects");
	}

	WEBCXX_Set(const struct set_t *s) : s(s){
		if(!isCorrectType()){
			throw std::runtime_error("Invalid set creation");
		}
	}

	const bool isCorrectType() const;

	unsigned long length() const{
		return Cardinality(s);
	}

	const T operator[](const unsigned long &index) const;
	inline const T at(const unsigned long &index) const{
		return (*this)[index];
	}
};

template<>
const bool WEBCXX_Set<long>::isCorrectType() const;

template<>
const bool WEBCXX_Set<SymChar>::isCorrectType() const;

template<>
const bool WEBCXX_Set<void>::isCorrectType() const;

template<class T>
std::ostream& operator<< (std::ostream& os, const WEBCXX_Set<T>& s){
	os << "[ ";
	bool first=true;
	for(int i=1; i<= s.length(); ++i){
		if(!first)os << ", ";
		else first=false;
		os << T(s[i]);
	}
	os << "]";
	return os;
}

template<>
const long WEBCXX_Set<long>::operator[](const unsigned long &i) const;

template<>
const SymChar WEBCXX_Set<SymChar>::operator[](const unsigned long &i) const;

template<>
const Empty WEBCXX_Set<Empty>::operator[](const unsigned long &i) const;

#endif
