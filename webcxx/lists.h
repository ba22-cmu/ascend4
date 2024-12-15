#ifndef lists_h
#define lists_h
#include <vector>
#ifdef __EMSCRIPTEN__

// a wrapper to enable webidl of lists
// _list is suffix to allow kind to be namespaced
#define list_vector(KIND) \
class KIND##_list { \
private: \
	std::vector< KIND > v; \
public: \
	KIND##_list(std::vector<KIND> s) : v(s) {} \
	KIND##_list() {} \
	int get_len() const; \
	KIND get(int pos) const; \
};

#define list_vector_forward(KIND) \
class KIND; \
class KIND##_list;

#define list_vector_impl(KIND) \
int KIND##_list::get_len() const \
{ \
	return v.size(); \
} \
KIND KIND##_list::get(int pos) const \
{ \
	return v[pos]; \
}

//std::vector(std::vector(double) not yet handled
#if 0
#define list_gllist(KIND) \
class KIND##_list { \
private: \
	struct gl_list_t *gl; \
public: \
	KIND##_list(struct gl_list_t *s) : gl(s) {} \
	int get_len() const; \
	KIND get(int pos) const; \
}
#endif

typedef std::string stdstring;
list_vector(double);
list_vector(int);
list_vector(stdstring);
list_vector(unsigned);
list_vector(uint32_t);

// aw namespace on instance to prevent C clash.
#define apply_classes(m) \
	m(Annotation) \
	m(Curve) \
	m(ExtMethod) \
	m(IncidencePoint) \
	m(aw_Instance) \
	m(Method) \
	m(Module) \
	m(Relation) \
	m(Solver) \
	m(Type) \
	m(UnitsM) \
	m(Variable)

#define lists_obj_fwd
#ifdef lists_obj_fwd
apply_classes(list_vector_forward)
#endif // lists_obj_fwd
 
#define lists_obj
#ifdef lists_obj
#include "webcxx/annotation.h"
#include "webcxx/curve.h"
#include "webcxx/extmethod.h"
#include "webcxx/incidencematrix.h"
#include "webcxx/instance.h"
#include "webcxx/method.h"
#include "webcxx/module.h"
#include "webcxx/relation.h"
#include "webcxx/solver.h"
#include "webcxx/type.h"
#include "webcxx/units.h"
#include "webcxx/variable.h"
apply_classes(list_vector)
#endif // lists_obj
 
#endif // __EMSCRIPTEN__

#endif //lists_h
