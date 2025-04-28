#include "solvers/static_solvers.h"

int qrslv_register();
int ipopt_register();
int dopri5_register();
int lsode_register();
int ida_register();
int radau5_register();

void static_solvers_register()
{
	qrslv_register();
#ifdef HAVE_IPOPT
	ipopt_register();
#endif

#ifdef HAVE_DOPRI5
	dopri5_register(void);
#endif

#ifdef HAVE_LSODE
	lsode_register(void);
#endif

#ifdef HAVE_IDA
	ida_register(void);
#endif

#ifdef HAVE_RADAU5
	radau5_register(void);
#endif
}
