/*
 * no need for tctlapi.so and tctllog.so.
 */

#include <stdio.h>
#include <stdarg.h>

//#include "tctlapi.h"
#define TCTL_HTYPE_J3PANEL      (3)
#define TCTL_HTYPE_OPANEL       (5)
//#include "tctllog.h"
#include <syslog.h>

#if defined(__O_PANEL__)
#define	hw_type	TCTL_HTYPE_OPANEL;
#else
#define	hw_type	TCTL_HTYPE_J3PANEL;
#endif

int tctl_TermHwType()
{
	return hw_type;
}

static int log_mask = LOG_DEBUG;
int tctl_logInit(char *prog, int use_syslog, int lvl)
{
	return lvl;
}

static void vlog(int lvl, const char *fmt, va_list ap)
{
	char buf[2048];

	vsnprintf(buf, sizeof(buf), fmt, ap);
	fprintf(stderr, "%s\n", buf);
}

void tctl_logOutput(int lvl, const char *fmt, ...)
{
	va_list ap;
	int mask = log_mask;

	//if (!(mask & LOG_MASK(lvl)))
	//	return;

	va_start(ap, fmt);
	vlog(lvl, fmt, ap);
	va_end(ap);

	return;
}
