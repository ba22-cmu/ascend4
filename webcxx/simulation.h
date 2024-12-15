#ifndef WEBCXX_SIMULATION_H
#define WEBCXX_SIMULATION_H

#include <string>
#include <vector>
#include <map>

#include "symchar.h"
#include "type.h"
#include "instance.h"
#include "variable.h"
#include "relation.h"

#include <cstdio>

#include "config.h"
extern "C"{
#include <ascend/compiler/createinst.h>
#include <ascend/system/slv_types.h>
}

class Solver;
class SolverParameters;
class SolverStatus;
class IncidenceMatrix;
class SolverReporter;
class Matrix;
class SolverHooks;

/**
	A class to contain singularity information as returned by the DOF
	function slvDOF_structsing.
*/
class SingularityInfo{
public:
	bool isSingular() const;
	std::vector<Relation> rels; /**< relations involved in the singularity */
	std::vector<Variable> vars; /**< variables involved in the singularity */
	std::vector<Variable> freeablevars; /**< vars that should be freed */
#ifdef __EMSCRIPTEN__
	Relation_list get_rels_list(); /**< relations involved in the singularity */
	Variable_list get_vars_list(); /**< variables involved in the singularity */
	Variable_list get_freeablevars_list(); /**< vars that should be freed */
#endif

};

enum StructuralStatus{
	WEBCXX_DOF_UNDERSPECIFIED=1,
	WEBCXX_DOF_SQUARE=2, /* = everything's ok */
	WEBCXX_DOF_OVERSPECIFIED=4,
	WEBCXX_DOF_STRUCT_SINGULAR=3
};

/**
	@TODO This class is for *Simulation* instances.

	Handle instantiating, running initialisation functions, solving
	and outputing results of solutions.

	In ASCEND C-code, a simulation is a special type of Instance. It
	has a 'simulation root' instance which often needs to be used for
	solving, inspecting, etc, rather than the simulation instance itself.

	The Simulation can be exported to an Integrator (for time-stepping)
	or a Solver (for steady-state solutions).

	At present the architecture is a bit muddy wrt to way that Solvers and
	Integrators 'act on' the Simulation. We need to work on improving the
	delimitation of solver and integrator, and keeping better track of the 
	state of the Simulation (has it been 'built', etc).
*/
class Simulation : public aw_Instance{
	friend class IncidenceMatrix;
	friend class SolverStatus;
	friend class Integrator;
	friend class System;
private:
	aw_Instance simroot;
	slv_system_t sys;
	bool is_built;
	SingularityInfo *sing; /// will be used to store this iff singularity found
	int activeblock;
	SolverHooks *solverhooks;
protected:
	slv_system_t getSystem();
	aw_Instance getRoot();
public:
	explicit Simulation(Instance *i, const SymChar &name);
	Simulation(const Simulation &);
	~Simulation();

	aw_Instance &getModel();

	void runDefaultMethod();
	void run(const Method &method);
	void run(const Method &method, aw_Instance &model);


	// checks on the instance tree
	void checkInstance(const int &level=5);
	void checkTokens();
	void checkStructure();
	void checkStatistics();
	void checkDimensions();

	void build();

	// checks of solver lists/structure
	enum StructuralStatus checkDoF() const;
	bool checkStructuralSingularity();
	const SingularityInfo &getSingularityInfo() const;

	void solve(Solver s, SolverReporter &reporter);
	void presolve(Solver s);
	const int iterate();
	void postsolve(SolverStatus status);
	SolverStatus getStatus();

	std::vector<Variable> getFixableVariables();
	std::vector<Variable> getVariablesNearBounds(const double &epsilon=1e-4);
	std::vector<Variable> getVariablesFarFromNominals(const double &bignum);
	std::vector<Variable> getFixedVariables();
	std::vector<Variable> getallVariables();
	Matrix getMatrix();

	void write(const char *fname,const char *type=NULL) const;

	void setSolver(Solver &s);
	const Solver getSolver() const;

	SolverParameters getParameters() const;
	void setParameters(SolverParameters &);

	IncidenceMatrix getIncidenceMatrix();

	const std::string getInstanceName(const aw_Instance &) const;

	void processVarStatus();
	const int getNumVars();

	const int getActiveBlock() const;

	std::vector<Variable> getFreeableVariables();
	
	void setSolverHooks(SolverHooks *H);
	SolverHooks *getSolverHooks() const;
#ifdef __EMSCRIPTEN__
	Variable_list getFixableVariables_list();
	Variable_list getVariablesNearBounds_list(const double &epsilon=1e-4);
	Variable_list getVariablesFarFromNominals_list(const double &bignum);
	Variable_list getFixedVariables_list();
	Variable_list getallVariables_list();
	Variable_list getFreeableVariables_list();
#endif
};

#endif
