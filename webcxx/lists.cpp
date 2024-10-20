#define lists_all
#include "annotation.h"
#include "curve.h"
#include "extmethod.h"
#include "incidencematrix.h"
#include "instance.h"
#include "method.h"
#include "module.h"
#include "relation.h"
#include "solver.h"
#include "type.h"
#include "units.h"
#include "variable.h"
#include <string>
#include "lists.h"

list_vector_impl(double);
list_vector_impl(int);
list_vector_impl(stdstring);
list_vector_impl(unsigned);

apply_classes(list_vector_impl)

