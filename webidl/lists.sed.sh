#! /bin/bash
/bin/rm -f lists.widl
for k in Annotation Curve ExtMethod IncidencePoint aw_Instance Method Module Relation Solver Type UnitsM Variable; do
	sed -e "s/KIND/$k/g" lists.widl.in >> lists.widl
done
