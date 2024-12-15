/*	ASCEND modelling environment
	Copyright (C) 2006-2010 Carnegie Mellon University

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
#ifndef WEBCXX_REPORTER_H
#define WEBCXX_REPORTER_H

#include "config.h"

extern "C"{
#include <ascend/general/platform.h>
#include <ascend/utilities/error.h>
}


/**
	This class provides C++ abstraction of the error.h error callback
	interface.

	Initially, it's trying to just handling the conveying of error
	messages back to python, but it could be used to pass back
	all sorts of other 'messages' eventually.

	Maybe raising alerts, notifying of the progress of big tasks, etc.

	The client_data pointer allows callback context to be set.
*/
class Reporter{
private:
	void *client_data;
	Reporter(); // This class will be a singleton
	~Reporter();
	static Reporter *_instance;

public:
	static ASC_EXPORT Reporter * Instance();
	void setErrorCallback(error_reporter_callback_t, void *client_data=NULL);
};

Reporter *getReporter();

#endif // WEBCXX_REPORTER_H
