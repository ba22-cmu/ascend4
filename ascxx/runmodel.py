import ascpy, pathlib, sys

def run_ascend_model(filen,model=None):
	import ascpy
	L = ascpy.Library()
	L.load(filen)
	if model is None:
		model = pathlib.Path(filen).stem
	T = L.findType(model)
	M = T.getSimulation('sim',True) # run default method = True
	M.solve(ascpy.Solver("QRSlv"),ascpy.SolverReporter())
	# TODO: we can add a customised solverreporter here
	# TODO: we could also extend the user interface to support setting of solver parameters etc.
	# TODO: we could also implement setting of model parameters and/or parameter sweeps.
	# TODO: we could also implement running 'self_test' methods after the model has solved.
	# TODO: examine how SlvReq can be used to allow the model file to select its own solver.
	# TODO: what about integrator models?
	# TODO: what about outputting a value or values from the model?
	# TODO: implement a custom ErrorReporter to suppress ASC_USER_NOTE and ASC_PROG_NOTE by default.
	
if __name__=="__main__":
	#print("sys.argv =",sys.argv)
	try:
		if len(sys.argv)>=2:
			filen = sys.argv[1]
			#print("FILE:",filen)
			model = None
			if len(sys.argv)==3:
				model = sys.argv[2]
				#print("MODEL:",model)
			elif len(sys.argv)>3:
				raise RuntimeError("Invalid number of command-line arguments received",sys.argv[0]);
			run_ascend_model(filen,model)
		else:
			raise RuntimeError("Invalid number of command-line arguments received",sys.argv[0]);
		sys.exit(0)
	except Exception as e:
		sys.stderr.write(f"{pathlib.Path(sys.argv[0]).name}: {str(e)}\n")
		sys.exit(1)
