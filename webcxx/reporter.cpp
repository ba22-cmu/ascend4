#include "config.h"
#include "reporter.h"

#include <cstdio>
#include <iostream>
using namespace std;

static const int REPORTER_MAX_ERROR_MSG = ERROR_REPORTER_MAX_MSG;

Reporter::Reporter(){
	error_reporter_set_callback(NULL);
}

Reporter *Reporter::_instance;

Reporter *
Reporter::Instance(){
	if(_instance==0){
		_instance = new Reporter();
	}
	return _instance;
}

Reporter *getReporter(){
	return Reporter::Instance();
}

Reporter::~Reporter(){
	error_reporter_set_callback(NULL);
}

void
Reporter::setErrorCallback(error_reporter_callback_t callback, void *client_data){
	this->client_data = client_data;
	error_reporter_set_callback(callback);
}

/*
int
Reporter::reportError(ERROR_REPORTER_CALLBACK_ARGS){
	char msg[REPORTER_MAX_ERROR_MSG];
	vsnprintf(msg,REPORTER_MAX_ERROR_MSG,fmt,args);
	cerr << char(27) << "[32;1m" << msg << char(27) << "[0m";
	return strlen(msg) + 11; // 11 chars worth of escape codes
}
*/

