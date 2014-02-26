#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>
#include <string.h>

void btrace(char *fmt, ...);
void btrace_init(char *logfile_name);
static char btrace_logfile_name[256] = "";

void btrace_init(char *logfile_name)
{
  strncpy(btrace_logfile_name, logfile_name, sizeof(btrace_logfile_name));
}

void btrace(char *fmt, ...)
{
  va_list ap;
  char time_buf[30];
  FILE *LOG;
  struct stat statb;
  struct timeval unow;
  time_t now;
  int rc;

  if (strncmp(btrace_logfile_name, "", sizeof(btrace_logfile_name)) == 0) {
    fprintf(stderr,"Set the log file name with btrace_init before using btrace\n");
    return;
  }

  if (stat(btrace_logfile_name, &statb) != 0) {
    /* ignore error if file does not exist yet, it will be created below */
    if (errno != ENOENT) {
      fprintf(stderr,"Could not stat file %s errno %d\n", btrace_logfile_name, errno);
      return;
    }
  }

  /* st_size is the number of bytes used by the file. So, 2^31-512 = 2147483136
     is the max size of a file that can be written to, in bytes. Subtracting 512 
     ensures we don't get too close to the 2G limit that the write will hang. */
  if (statb.st_size >= 2147483136) {
    fprintf(stderr,"%s is too big\n", btrace_logfile_name);
    return;
  }

  if((LOG = fopen(btrace_logfile_name, "a")) == NULL){
    fprintf(stderr,"File %s could not be opened\n", btrace_logfile_name);
    return;
  }

  rc = gettimeofday(&unow, NULL);
  now = unow.tv_sec;

#ifdef _IBM_SOURCE
  ctime_r(&now, time_buf);
#endif
#ifdef _SUN_SOURCE
  ctime_r(&now, time_buf, 30);
#endif

  time_buf[19] = '\0';
  fprintf(LOG, "%s.%03d ", time_buf, unow.tv_usec/1000);

  va_start(ap, fmt);
  vfprintf(LOG, fmt, ap);
  va_end(ap);

  fclose(LOG);
}

