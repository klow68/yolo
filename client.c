#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>


void erreur(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
	
}