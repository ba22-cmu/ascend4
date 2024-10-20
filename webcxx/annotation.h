#ifndef ASCXX_ANNOTATION_H
#define ASCXX_ANNOTATION_H

#include "symchar.h"
#include "type.h"

#include <vector>

class Annotation{
private:
	struct Note *n;
public:
	Annotation();
	Annotation(struct Note *);
	~Annotation();
	
	const char *getId() const;
	const char *getMethod() const;
	const SymChar getType() const;
	const SymChar getLanguage() const;
	const char *getFilename() const;
	const int getLineNumber() const;
	const char *getText() const;
	const int getDataType() const;
	//const void *getUserData(const int &datatype) const;
};

#endif
