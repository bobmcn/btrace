#ifndef INCLUDED_BTRACE_H
#define INCLUDED_BTRACE_H

#ifndef lint
static char RCSId_btrace_h[] = "$Id: $  $CSID:  $  $SCMId:  $";
#endif

void btrace_init(char *logfile_name);
void btrace(char *fmt, ...);
extern char btrace_logfile_name[256];

#endif
