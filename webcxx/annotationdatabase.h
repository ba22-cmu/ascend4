#ifndef WEBCXX_ANNOTATION_DB_H
#define WEBCXX_ANNOTATION_DB_H

#include "annotation.h"

#ifdef __EMSCRIPTEN
#include "lists.h"
list_vector(Annotation);
#endif 

class AnnotationDatabase{
private:
	const symchar *dbid;
public:
	AnnotationDatabase(const SymChar &dbid);

#ifdef __EMSCRIPTEN
	Annotation_list getNotes_list(const Type &type, const SymChar *lang
		, const SymChar *id, const SymChar *method=NULL, const int flag=0
	);

	Annotation_list getTypeRefinedNotesLang_list(const Type &type ,
		       	const SymChar *lang
	);
#endif
	std::vector<Annotation> getNotes(const Type &type, const SymChar *lang=NULL
		, const SymChar *id=NULL, const SymChar *method=NULL, const int flag=0
	);

	std::vector<Annotation> getTypeRefinedNotesLang(const Type &type
		, const SymChar *lang
	);
	const char *getNoteForVariable(const Type &type
		, const SymChar *varname
		, const SymChar *lang=NULL /* assume 'inline' if not specified */
	);
};

#endif // WEBCXX_ANNOTATION_DB_H
