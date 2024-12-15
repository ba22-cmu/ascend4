#ifndef WEBCXX_INCIDENCEMATRIX_H
#define WEBCXX_INCIDENCEMATRIX_H

#include <vector>

#include "variable.h"
#include "relation.h"
#include "simulation.h"
#include "solverstatus.h"

#include "config.h"

extern "C"{
#include <ascend/system/incidence.h>
}

typedef enum{
	IM_NULL=0, IM_ACTIVE_FIXED, IM_ACTIVE_FREE, IM_DORMANT_FIXED, IM_DORMANT_FREE
} IncidencePointType;

typedef enum{
	IM_CONVERGED=0, IM_OVER_ITER, IM_OVER_TIME, IM_DIVERGED, IM_NOT_YET_ATTEMPTED
} BlockStatusType;

class IncidencePoint{
public:
	IncidencePoint(const int&row, const int&col, const IncidencePointType &type);
	IncidencePoint(const IncidencePoint &);
	IncidencePoint();

	int row;
	int col;
	IncidencePointType type;
};

#ifdef __EMSCRIPTEN__
list_vector(IncidencePoint);
list_vector(Variable);
list_vector(Relation);
#endif
/**
	Special class for plotting incidence matrices using matplotlib

	GOAL: facilitate use of pylab 'spy2' function, but hopefully add extra
	stuff for viewing blocks and fixed/free incidences, solved/active/unsolved
	variables, etc.

	This is going to be like a C++ified copy of MtxProc.c
*/
class IncidenceMatrix{

private:
	Simulation &sim;
	slv_system_t sys;

	std::vector<IncidencePoint> data;
	incidence_vars_t i;
	bool is_built;

	void buildPlotData();
public:
	explicit IncidenceMatrix(Simulation &sim);
	~IncidenceMatrix();

	const std::vector<IncidencePoint> &getIncidenceData();
	const int &getNumRows() const;
	const int &getNumCols() const;
	const Variable getVariable(const int &row) const;
	const Relation getRelation(const int &col) const;
	const int getBlockRow(const int & row) const;
	const std::vector<Variable> getBlockVars(const int &block);
	const std::vector<Relation> getBlockRels(const int &block);
	const std::vector<int> getBlockLocation(const int &block) const;
	const BlockStatusType getBlockStatus(const int &block) const;
	const int getNumBlocks();

#ifdef __EMSCRIPTEN__
	IncidencePoint_list getIncidenceData_list();
	Variable_list getBlockVars_list(const int &block);
	Relation_list getBlockRels_list(const int &block);
	int* getBlockLocation_list(const int &block) const;
#endif
};

#endif // WEBCXX_INCIDENCEMATRIX_H
