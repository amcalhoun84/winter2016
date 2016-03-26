#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

main()
{
    int sig_handler = 0;

   signal(SIGKILL, sig_handler);
}
