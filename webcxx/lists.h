#ifndef lists_h
#define lists_h
#include <vector>
#ifdef __EMSCRIPTEN__

// a wrapper to enable webidl of lists
#define list_vector(KIND) \
class list_##KIND { \
private: \
	std::vector< KIND > v; \
public: \
	list_##KIND(std::vector<KIND> s) : v(s) {} \
	list_##KIND() {} \
	int get_len() const; \
	KIND get(int pos) const; \
};

#define list_vector_impl(KIND) \
int list_##KIND::get_len() const \
{ \
	return v.size(); \
} \
KIND list_##KIND::get(int pos) const \
{ \
	return v[pos]; \
}

//std::vector(std::vector(double) not yet handled
#if 0
#define list_gllist(KIND) \
class list_##KIND { \
private: \
	struct gl_list_t *gl; \
public: \
	list_KIND(struct gl_list_t *s) : gl(s) {} \
	int get_len() const; \
	KIND get(int pos) const; \
}
#endif

typedef std::string stdstring;
list_vector(double);
list_vector(int);
list_vector(stdstring);
list_vector(unsigned);


#define apply_classes(m) \
	m(Annotation) \
	m(Curve) \
	m(ExtMethod) \
	m(IncidencePoint) \
	m(Instanc) \
	m(Method) \
	m(Module) \
	m(Relation) \
	m(Solver) \
	m(Type) \
	m(UnitsM) \
	m(Variable)

#ifdef lists_obj
apply_classes(list_vector)
#endif
#endif // __EMSCRIPTEN__

#endif //lists_h
