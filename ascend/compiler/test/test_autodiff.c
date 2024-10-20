/*	ASCEND modelling environment
	Copyright (C) 2007 Carnegie Mellon University

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
/**
	@file
	Unit test functions reverse ad & second derivatives
	Created by: Mahesh Narayanamurthi
	Revised by: Ben Allan - Rev.1
				Mahesh Narayanamurthi - Rev. 2

	TODO Note that this test case plays a DOUBLE ROLE of both preparing
	the test files (yacas-output-*.txt) and then reading them and comparing
	values with those returned by the computer algebra system YACAS. If you 
	want to change and of the model files being tested here, you will also
	need to update the yacas output files against which the derivatives will
	be compared.

	To update the YACAS output files, use the following commands: 

pushd ~/ascend/trunk \
&& scons -j4 test ascend models solvers \
&& LD_LIBRARY_PATH=. ASC_YACAS_GEN=1 test/test compiler_autodiff \
&& cd ascend/compiler/test \
&& python yacasgen.py \
&& dirn=`pwd` \
&& cd /usr/share/yacas \
&& yacas $dirn/yacas-input-1st.txt > $dirn/yacas-output-1st.txt \
&& yacas $dirn/yacas-input-2nd.txt > $dirn/yacas-output-2nd.txt \
&& cd $dirn/../../.. && LD_LIBRARY_PATH=. test/test compiler_autodiff;
popd
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <ascend/general/env.h>
#include <ascend/general/ospath.h>
#include <ascend/general/list.h>
#include <ascend/general/ltmatrix.h>

#include <ascend/general/platform.h>
#include <ascend/utilities/ascEnvVar.h>
#include <ascend/utilities/error.h>

#include <ascend/compiler/ascCompiler.h>
#include <ascend/compiler/module.h>
#include <ascend/compiler/parser.h>
#include <ascend/compiler/library.h>
#include <ascend/compiler/symtab.h>
#include <ascend/compiler/simlist.h>
#include <ascend/compiler/instquery.h>
#include <ascend/compiler/parentchild.h>
#include <ascend/compiler/atomvalue.h>
#include <ascend/compiler/relation_io.h>
#include <ascend/compiler/reverse_ad.h>
#include <ascend/compiler/relation_util.h>
#include <ascend/compiler/mathinst.h>
#include <ascend/compiler/watchpt.h>
#include <ascend/compiler/initialize.h>
#include <ascend/compiler/name.h>
#include <ascend/compiler/visitinst.h>
#include <ascend/compiler/functype.h>
#include <ascend/compiler/safe.h>
#include <ascend/compiler/qlfdid.h>
#include <ascend/compiler/instance_io.h>
// FIXME include anything else?

#include <test/common.h>
#include <test/assertimpl.h>
#include <test/test_globals.h>

//#define AUTODIFF_DEBUG
#ifdef AUTODIFF_DEBUG
# define MSG CONSOLE_DEBUG
#else
# define MSG(ARGS...) ((void)0)
#endif

#define TOL 1e-5
#define VERY_CLOSE(a,b) (fabs(a-b)<=MAX(TOL,MAX(fabs(a*TOL),fabs(b*TOL))))

extern char ASC_TEST_PATH[PATH_MAX];

struct FileList
{
	FILE * yacas; /* Meant for YACAS */
	FILE * safeder; /* Meant for Safe Der */
	FILE * nonsafeder; /* Meant for Non-Safe Der */
};

struct TestFileList
{
	FILE * FirstDerYacas;
	FILE * SecondDerYacas;
};

struct DiffTestData
{
	FILE * outfile; /* Meant for Logging Debug Info */
	FILE * varfile; /* Meant for recording the Variable Values */
	struct FileList FirstDer;
	struct FileList SecondDer;
	int use_yacas;
	FILE * first_yacas;
	FILE * second_yacas;
	struct Instance *root;
	const char *infile;
	int d0errors;
	int d1errors;
	int d1errors_yacas;
	int d2errors_yacas;
	int numrels;
};

#define INITDTD(d, log, varfile, FirstDer, SecondDer, use_yacas, YacasInFirst, YacasInSecond ,instroot, inputname) \
if(1){\
	d.outfile = log; \
	d.varfile = varfile; \
	d.FirstDer = FirstDer;\
	d.SecondDer = SecondDer;\
	d.use_yacas = use_yacas; \
	d.first_yacas = YacasInFirst;\
	d.second_yacas = YacasInSecond;\
	d.root = instroot; \
	d.infile = inputname; \
	d.d0errors= 0; \
	d.d1errors= 0; \
	d.d1errors_yacas= 0; \
	d.d2errors_yacas= 0; \
	d.numrels = 0;\
}

#define LOG(d, ...)\
	if(d != NULL && d->outfile != NULL){ \
		fprintf(d->outfile,__VA_ARGS__); \
	}

/** Temporary Declarations */
static void AutomateDiffTest(struct Instance *inst, VOIDPTR ptr);

static void test_autodiff(void){
#define OUTENV "../ascend/compiler/test/LOG.html"
#define VARFILE "../ascend/compiler/test/varnames.txt"

#define SAFEDER_1ST "../ascend/compiler/test/derivs-safe-1st.txt"
#define SAFEDER_2ND "../ascend/compiler/test/derivs-safe-2nd.txt"
#define NONSAFEDER_1ST "../ascend/compiler/test/derivs-nonsafe-1st.txt"
#define NONSAFEDER_2ND "../ascend/compiler/test/derivs-nonsafe-2nd.txt"

#define YACAS_PREP_1ST "../ascend/compiler/test/yacas-prep-1st.txt"
#define YACAS_PREP_2ND "../ascend/compiler/test/yacas-prep-2nd.txt"
#define YACAS_OUT_1ST "../ascend/compiler/test/yacas-output-1st.txt"
#define YACAS_OUT_2ND "../ascend/compiler/test/yacas-output-2nd.txt"

#if 0
# define CASEFILE "test/reverse_ad/allmodels.a4c"
# define TYPENAME "allmodels"
#else
# define CASEFILE "test/ipopt/dummy.a4c"
# define TYPENAME "dummy"
#endif

#define USE_YACAS_ENV "ASC_YACAS_GEN"

	int status;
	int use_yacas = 0;

	//struct module_t *m;

	struct Name *name;
	enum Proc_enum pe;
	struct Instance *root;

	struct DiffTestData data;

	FILE *outfile = NULL;
	FILE *varfile = NULL;

	FILE *first_yacas = NULL;
	FILE *second_yacas = NULL;

	struct FileList FirstDer;
	struct FileList SecondDer;

	FirstDer.yacas = FirstDer.safeder = FirstDer.nonsafeder = NULL;
	SecondDer.yacas = SecondDer.safeder = SecondDer.nonsafeder = NULL;

	char env1[2*PATH_MAX];
	Asc_CompilerInit(1);
	/* set the needed environment variables so that models, solvers can be found */
	snprintf(env1,2*PATH_MAX,ASC_ENV_LIBRARY "=%s","models");
	CU_TEST(0 == Asc_PutEnv(env1));
	CU_TEST(0 == Asc_PutEnv(ASC_ENV_SOLVERS "=solvers/ipopt"));

	Asc_PutEnv(ASC_ENV_LIBRARY "=models");


	// FIXME Use the environment variables here
	MSG("ASC_TEST_PATH = '%s'",ASC_TEST_PATH);

	struct FilePath *rootfp;
	{
		struct FilePath *tmp = ospath_new(ASC_TEST_PATH);
		rootfp = ospath_getabs(tmp);
		ospath_free(tmp);
	}

#define OPENTESTFILE(FNAME,VAR,MODE) {\
		struct FilePath *tmp = ospath_new_noclean(FNAME);\
		struct FilePath *fp2 = ospath_concat(rootfp,tmp);\
		ospath_cleanup(fp2);\
		VAR = ospath_fopen(fp2,MODE);\
		char *s=ospath_str(fp2);\
		CU_ASSERT_PTR_NOT_NULL_FATAL(VAR);\
		MSG("Opened test file '%s'",s);\
		ASC_FREE(s);\
		ospath_free(tmp);\
		ospath_free(fp2);\
	}

	OPENTESTFILE(OUTENV,outfile,"w");

	/** @TODO Open the following streams only if Environment Variable is set */
	if(getenv(USE_YACAS_ENV) && atol(getenv(USE_YACAS_ENV))){
		MSG("Generating YACAS input files for verification of ASCEND's values");
		OPENTESTFILE(VARFILE,varfile,"w");
		OPENTESTFILE(YACAS_PREP_2ND,SecondDer.yacas,"w");
		OPENTESTFILE(SAFEDER_2ND,SecondDer.safeder,"w");
		OPENTESTFILE(NONSAFEDER_2ND,SecondDer.nonsafeder,"w");
		OPENTESTFILE(YACAS_PREP_1ST,FirstDer.yacas,"w");
		OPENTESTFILE(SAFEDER_1ST,FirstDer.safeder,"w");
		OPENTESTFILE(NONSAFEDER_1ST,FirstDer.nonsafeder,"w");
		use_yacas=1;
	}else{
		MSG("Testing against pre-calculated derivatives from YACAS.");
		OPENTESTFILE(YACAS_OUT_1ST,first_yacas,"r");
		OPENTESTFILE(YACAS_OUT_2ND,second_yacas,"r");
	}

	ospath_free(rootfp);

	/* load the file */
	Asc_OpenModule(CASEFILE,&status);
	CU_ASSERT_FATAL(status == 0);

	/* parse it */
	CU_ASSERT_FATAL(0 == zz_parse());

	/* find the model */
	symchar * type = AddSymbol(TYPENAME);
	CU_ASSERT_FATAL(FindType(type)!=NULL);

	/* instantiate it */
	struct Instance *sim = SimsCreateInstance(type, AddSymbol("sim1"), e_normal, NULL);
	CU_ASSERT_FATAL(sim!=NULL);

	/*root of Simulation object*/
	root = GetSimulationRoot(sim);

	/** Call on_load */
	error_reporter_tree_start();

	name = CreateIdName(AddSymbol("on_load"));
	pe = Initialize(root,name,"sim1",ASCERR,0, NULL, NULL);
	CU_TEST_FATAL(0==error_reporter_tree_has_error());
	error_reporter_tree_end();

	CU_TEST_FATAL(pe == Proc_all_ok);

	/* check for vars and rels */

	INITDTD(data, outfile, varfile, FirstDer, SecondDer,use_yacas, first_yacas, second_yacas, root, CASEFILE);
	if(data.outfile!=NULL){
		fprintf(data.outfile,"<html><head><title>LOG: Testing of Reverse Automatic Differentiation in ASCEND</title></head><body style='font-size: 9pt;font-family: serif;'>\n");
		fprintf(data.outfile,"<h2>LOG File of Test-Suite for Automatic Differentiation</h2>\n");
		fprintf(data.outfile,"<h3><u>Legend</u></h3><b>tolerance		=		%21.17g</br>\n",TOL);
		fprintf(data.outfile,"Difference(a,b)		=		a - b</br>\n");
		fprintf(data.outfile,"Error(a,b)		=		(a - b)/max(a,b) </br>\n");
		fprintf(data.outfile,"<font color='gold'>gold</font>		=	Residual Error </br>\n");
		fprintf(data.outfile,"<font color='indigo'>indigo</font>=		Gradient Error </br>\n");
		fprintf(data.outfile,"<font color='purple'>purple</font>=		Gradient Mismatch </br>\n");
		fprintf(data.outfile,"<font color='red'>red</font>=		Second Partials Mismatch</b> </center></br></br>\n");
	}

	VisitInstanceTreeTwo(root, AutomateDiffTest, 0, 0, &data);
	sim_destroy(sim);
	Asc_CompilerDestroy();

	if(data.outfile!=NULL){
		fprintf(data.outfile,"<h2>Summary</h2>\n");
		fprintf(data.outfile,"<p>No. of relations tests = %d<br>\n",data.numrels);
		fprintf(data.outfile,"<p>No. of Residual Errors = %d <br>\n",data.d0errors);
		fprintf(data.outfile,"No. of First RAD Errors = %d <br>\n",data.d1errors);
		fprintf(data.outfile,"No. of Total Errors = %d<</p>>\n",data.d0errors+data.d1errors);
		fprintf(data.outfile,"<p>No. of First YACAS Mismatches = %d<br>\n",data.d1errors_yacas);
		fprintf(data.outfile,"No. of Second YACAS Mismatches = %d<br>\n",data.d2errors_yacas);
		fprintf(data.outfile,"No. of Total Mismatches = %d</p></body></html>"
			,data.d1errors_yacas + data.d2errors_yacas
		);
	}
	MSG("TOTAL OF %d RELATIONS TESTED:",data.numrels);
	MSG("  Residual errors: %d",data.d0errors);
	MSG("  First deriv errors: %d",data.d1errors);
	MSG("  First YACAS mismatches: %d",data.d1errors_yacas);
	MSG("  Second YACAS mismatches: %d",data.d2errors_yacas);

#define OSPCLEAN(FPTR) if(FPTR!=NULL){fclose(FPTR);data.FPTR = NULL;}
	OSPCLEAN(outfile);
	if(use_yacas){
		OSPCLEAN(varfile);
		OSPCLEAN(SecondDer.yacas);
		OSPCLEAN(SecondDer.safeder);
		OSPCLEAN(SecondDer.nonsafeder);
		OSPCLEAN(FirstDer.yacas);
		OSPCLEAN(FirstDer.safeder);
		OSPCLEAN(FirstDer.nonsafeder);
	}else{
		OSPCLEAN(first_yacas);
		OSPCLEAN(second_yacas);
	}

	CU_ASSERT( 0 == (data.d0errors + data.d1errors) );
	MSG("For non-fatal errors refer ascend/compiler/test/LOG.html");
}


/*------------------------------------------------------------------------------
	Routine to conduct derivative tests on a single relation
*/
static void AutomateDiffTest(struct Instance *inst, VOIDPTR ptr){
	double residual_rev,residual_fwd;
	double *gradients_rev,*gradients_fwd,*deriv_2nd;
	float yacas_first_der = 0.0;  /* compiler complains type mismatch when using */
	float yacas_second_der = 0.0; /*double to read using fscanf(File*,"%21.17g"...) */
	unsigned long num_var,i,j;
	enum Expr_enum reltype;
	struct relation *r;
	int32 status;
	char *rname = NULL, *infix_rel = NULL, *varname;
	char buf[21];
	int nitems;
    struct RXNameData myrd = {"x",NULL,""};
	struct DiffTestData *data = (struct DiffTestData*) ptr;
	struct Instance *var_inst;
	const char *style;
#define RETURN if(rname != NULL)ASC_FREE(rname); return

	if(inst==NULL || InstanceKind(inst)!=REL_INST)return;

	if (data != NULL && data->outfile != NULL) {
		rname = WriteInstanceNameString(inst, data->root);
	}

	data->numrels++;

	/* get the relation, check type */
	r = (struct relation *)GetInstanceRelation(inst, &reltype);
	if(r == NULL){
		LOG(data, "<font color='orange'><b>! skipping instance with null struct relation:</b></font> %s</br>\n", rname);
		RETURN;
	}
	if(reltype != e_token){
		LOG(data, "<font color='orange'><b>! skipping non-token relation</b></font> %s</br>\n", rname);
		RETURN;
	}
	LOG(data, "</br><font color='green'><b> Evaluating token relation </b></font> <b> %s </b> </br>\n", rname);

	/* convert relation to YACAS code string */
	infix_rel = WriteRelationString(inst, data->root
			, (WRSNameFunc)RelationVarXName, NULL, relio_yacas, NULL
	);
	num_var = NumberVariables(r); // FIXME or should we use rel_n_incidences

	/* write out the values of each variable; log rel + var vals */
	MSG("Relation %d: '%s' = %s",data->numrels,rname,infix_rel);
	LOG(data,"</br></br><b>Relation:  %s  </br>\n",infix_rel); // Do I need to escape this??
	for(i=0; i<num_var; i++) {
		varname = RelationVarXName(r,i+1,&myrd);
		if (varname!=NULL){
			LOG(data,"%s:=",varname);
			var_inst = RelationVariable(r,i+1);
			LOG(data,"%21.17g</br>\n",RealAtomValue(var_inst));
			MSG("%s = %g",varname,RealAtomValue(var_inst));
		}
	}
	LOG(data,"</b>\n");

	gradients_rev = ASC_NEW_ARRAY(double,num_var); // or rel_n_incidences
	CU_TEST_FATAL(NULL!=gradients_rev);
	gradients_fwd = ASC_NEW_ARRAY(double,num_var); // or rel_n_incidences
	CU_TEST_FATAL(NULL!=gradients_fwd);
	deriv_2nd = ASC_NEW_ARRAY(double,num_var); // or rel_n_incidences
	CU_TEST_FATAL(NULL!=deriv_2nd);

	/* write out data files for use by yacas */
	if(data->use_yacas){
		if(data->FirstDer.yacas!=NULL && data->SecondDer.yacas!=NULL && data->varfile!=NULL){
			/** Print Variables Values First */
			for(i=0; i<num_var; i++) {
				varname = RelationVarXName(r,i+1,&myrd);
				if (varname!=NULL){
					fprintf(data->varfile,"%s==",varname);
					var_inst = RelationVariable(r,i+1);
					fprintf(data->varfile,"%-21.17g\n",RealAtomValue(var_inst));
				}
			}
			fprintf(data->varfile,"@ Relation: %s\n",rname);

			if (infix_rel!=NULL){
				for(i=0; i<num_var;i++){
					varname = RelationVarXName(r,i+1,&myrd);
					strncpy(buf,varname,20);

					// Generating Output for First Derivative Yacas Input file
					fprintf(data->FirstDer.yacas
						,"D(%s) (%s)\n"
						,buf,infix_rel
					);

					// Generating Output for Second Derivative Yacas Input file
					for (j=0;j<num_var;j++){
						varname = RelationVarXName(r,j+1,&myrd);
						fprintf(data->SecondDer.yacas
							,"D(%s) D(%s) (%s)\n"
							,buf,varname,infix_rel
						);
					}
				}
			}
			else{
				ERROR_REPORTER_HERE(ASC_PROG_FATAL,"Infix Relation is NULL");
			}
		}
	}
	ASC_FREE(infix_rel);

	/*--- non-safe evaluation routines ---*/

	/* TODO we need to sigfpe trap this code or use the safe versions. */

	RelationCalcResidGradRev(inst,&residual_rev,gradients_rev);
	RelationCalcResidGrad(inst,&residual_fwd,gradients_fwd);

	LOG(data,"<h4>Table of Values for Residuals</h4>\n");
	LOG(data,"\n<table BORDER>\n");
	LOG(data,"<tr><td>ASCEND(NONSAFE,REV)</td><td>ASCEND(NONSAFE,FWD)</td><td>Abs. Error</td></tr>\n");

	if(!VERY_CLOSE(residual_rev,residual_fwd)){
		MSG("residuals: rev=%g, fwd=%g",residual_rev, residual_fwd);
		CU_FAIL("forward and revert residuals didn't match");
		data->d0errors ++;
		style = "background-color:yellow; font-color:red";
	}else{
		style = "";
	}
	LOG(data,"<tr style=\"%s\"><td>%21.17g</td><td>%21.17g</td><td>%.4g</td></tr>\n"
			,style, residual_rev,residual_fwd, fabs(residual_rev - residual_fwd)
	);
	LOG(data,"</table>\n");

	if(data->first_yacas!=NULL){
		char s[PATH_MAX+1];
		if(NULL==fgets(s,PATH_MAX,data->first_yacas)){
			MSG("line: %s",s);
			CU_FAIL_FATAL("Expected line from " YACAS_OUT_1ST);
		}else{
			if(0!=strncmp(s,"@ Relation: ",10)){
				MSG("line (yacas-1st): %s",s);
				CU_FAIL_FATAL("Expected '@Relation:' from " YACAS_OUT_1ST);
			}
		}
	}

	LOG(data,"<h4>Table of Values for First Derivatives</h4>\n");
	LOG(data,"\n<table BORDER>\n");
	for(i=0; i<num_var; i++){
		if(data->use_yacas && data->FirstDer.nonsafeder!=NULL){
			/* Recording Reverse AD Non-Safe Derivatives to file */
			fprintf(data->FirstDer.nonsafeder,"%21.17g\n",gradients_rev[i]);

		}else if(data->first_yacas!=NULL){
			/* Benchmarking Non-Safe Gradient Errors against Yacas */
			if(!feof(data->first_yacas)){
				nitems = fscanf(data->first_yacas,"%g\n",&yacas_first_der);
				if (nitems != 1) {
					MSG("no number found for 1st derivative");
					CU_FAIL("first derivative unparsed (yacas versus reverse autodiff)");
					data->d1errors_yacas ++;
					style = "background-color:yellow; font-color:red";
				} else if(!VERY_CLOSE(yacas_first_der,gradients_rev[i])){
					MSG("dR/dx%lu_yacas = %g",i,yacas_first_der);
					MSG("dR/dx%lu_rev = %g",i,gradients_rev[i]);
					MSG("abs. error = %g",fabs(yacas_first_der - gradients_rev[i]));
					CU_FAIL("first derivative mismatch (yacas versus reverse autodiff)");
					data->d1errors_yacas ++;
					style = "background-color:yellow; font-color:red";
				} else {
					style = "";
				}
				LOG(data,"<tr><td>Column</td><td>ASCEND(nonsafe,rev)</td><td>YACAS</td><td>Abs. Error</td></tr>\n");
				LOG(data,"<tr style=\"%s\"><td>%lu</td><td>%21.17g</td><td>%21.17g</td><td>%.4g</td></tr>\n"
					,style, i,gradients_rev[i],yacas_first_der, fabs(yacas_first_der - gradients_rev[i])
				);
			}
		}

		if(!VERY_CLOSE(gradients_fwd[i],gradients_rev[i])){
			MSG("first derivs: fwd=%g, rev=%g, abs. error = %g",gradients_fwd[i],gradients_rev[i],fabs(gradients_fwd[i]-gradients_rev[i]));
			CU_FAIL("first derivs fwd vs rev not in agreement");
			data->d1errors ++;
			style = "background-color:yellow; font-color:red";
		}else{
			style = "";
		}
		LOG(data,"<tr><td>Column</td><td>ASCEND(nonsafe,rev)</td><td>ASCEND(nonsafe,fwd)</td><td>Abs. Error</td></tr>\n");
		LOG(data,"<tr style=\"%s\"><td>%lu</td><td>%21.17g</td><td>%21.17g</td><td>%.4g</td></tr>\n"
			, style, i,gradients_rev[i],gradients_fwd[i], fabs(gradients_fwd[i]-gradients_rev[i]));
	}
	LOG(data,"</table>\n");

	/*Non Safe Second Derivative Calculations*/
	if(data->use_yacas && data->SecondDer.nonsafeder!=NULL){
		fprintf(data->SecondDer.nonsafeder,"@ Relation: %s\n",rname);
	}

	if(data->second_yacas!=NULL){
		char s[PATH_MAX+1];
		char *rs;
		rs = fgets(s,PATH_MAX,data->second_yacas);
		char s2[PATH_MAX];
		snprintf(s2,PATH_MAX,"@ Relation: %s\n",rname);
		if (!rs) {
			MSG("line read failed from second-derivs yacas file");
			CU_FAIL_FATAL("Missing relation in second-derivs yacas file");
		} else if(0!=strcmp(s,s2)){
			MSG("line: <%s>",s);
			MSG("expected: <%s>",s2);
			CU_FAIL_FATAL("wrong relation read from second-derivs yacas file");
		}
	}

	LOG(data,"</br> <b> Table of Values for Second Derivatives </b> </br>\n");
	LOG(data,"\n<table BORDER>\n");
	LOG(data,"<tr><td>Row</td><td>Column</td><td>ASCEND (nonsafe)</td><td>YACAS</td><td>Abs. Error</td></tr>\n");
	for(i=0; i<num_var; i++){
		RelationCalcSecondDeriv(inst,deriv_2nd,i);
		if(data->use_yacas && data->SecondDer.nonsafeder!=NULL){
			/** @todo log calculated values and indiceshere.*/ /*FIXME*/
			for(j=0; j<num_var; j++){
				fprintf(data->SecondDer.nonsafeder,"%21.17g\n",deriv_2nd[j]); /*FIXME*/
			}
		}
		else if (data->second_yacas!=NULL){
			/* Benchmarking Non-Safe Second Derivative Errors against Yacas */
			for(j=0; j<num_var; j++){
				if(!feof(data->second_yacas)){
					nitems = fscanf(data->second_yacas,"%g\n",&yacas_second_der);
					if (nitems != 1) {
						MSG("no number found for 2nd derivative");
						CU_FAIL("second derivative yacas vs reverse AD");
						data->d1errors_yacas ++;
						style = "background-color:yellow; font-color:red";
					} else if(!VERY_CLOSE(yacas_second_der, deriv_2nd[j])){
	 					MSG("d2R/dx%ludx%lu_yacas = %g",i,j,yacas_second_der);
						MSG("d2R/dx%ludx%lu_rev = %g",i,j,deriv_2nd[j]);
						CU_FAIL("second derivative yacas vs reverse AD");
						data->d2errors_yacas ++;
						style = "background-color:yellow; font-color:red";
					} else {
						style = "";
					}
					LOG(data,"<tr style=\"%s\"><td>%lu</td><td>%lu</td><td>%21.17g</td><td>%21.17g</td><td>%.4g</td></tr>\n"
						,style, i,j,deriv_2nd[j],yacas_second_der,fabs(yacas_second_der-deriv_2nd[j])
					);
				}
			}
		}
	}

	LOG(data,"</table>\n");

	/** Testing safe evaluation routines */

	status = (int32) RelationCalcResidGradRevSafe(inst,&residual_rev,gradients_rev);
	safe_error_to_stderr( (enum safe_err *)&status );
	status = RelationCalcResidGradSafe(inst,&residual_fwd,gradients_fwd);
	safe_error_to_stderr( (enum safe_err *)&status );
	
	if(!VERY_CLOSE(residual_rev,residual_fwd)){
		CU_FAIL("Safe residuals: rev and fwd do not agree");
		data->d0errors ++;
	}

	for(i=0;i<num_var;i++) {
		if(data->use_yacas && data->FirstDer.safeder!=NULL){
			/* Recording Reverse AD Safe Derivatives to file */
			fprintf(data->FirstDer.safeder,"%21.17g\n",gradients_rev[i]);
		}
		if(!VERY_CLOSE(gradients_fwd[i],gradients_rev[i])){
			MSG("safe gradients: fwd=%.17g, rev=%.17g",gradients_fwd[i],gradients_rev[i]);
			CU_FAIL("safe first dervis: fwd and rev do not agree");
			data->d1errors ++;
		}
	}

	/*Safe Second Derivative Calculations*/
	if(data->use_yacas && data->SecondDer.safeder!=NULL){
		fprintf(data->SecondDer.safeder,"@ Relation: %s\n",rname);
	}
	for(i=0; i<num_var; i++){
		status = (int32) RelationCalcSecondDerivSafe(inst,deriv_2nd,i);
		safe_error_to_stderr( (enum safe_err *)&status );

		if(data->use_yacas && data->SecondDer.safeder!=NULL){
			/** @todo log calculated values here.*/ /*FIXME*/
			for(j=0; j<num_var; j++){
				fprintf(data->SecondDer.safeder,"%21.17g\n",deriv_2nd[j]); /*FIXME*/
			}
		}
	}

	/** End of Relation */
	LOG(data,"</br><b>@ End of Relation </b>%p{%s}</br><hr/>",r,rname);

	/** Freeing used memeory */
	if(gradients_rev)ASC_FREE(gradients_rev);
	if(gradients_fwd)ASC_FREE(gradients_fwd);
	if(deriv_2nd)ASC_FREE(deriv_2nd);
	RETURN;
#undef RETURN
}
/*===========================================================================*/
/* Registration information */

/* the list of tests */

#define TESTS(T) \
  T(autodiff)

REGISTER_TESTS_SIMPLE(compiler_autodiff, TESTS)

