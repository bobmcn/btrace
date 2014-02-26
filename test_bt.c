#include <btrace.h>

int main(int argc, char *argv[])
{
  int i;
  btrace_init("test.log");

  btrace("%s started\n", argv[0]);
  for (i=0; i<10; i++)
    btrace("%d\n", i);

  return(0);
}
