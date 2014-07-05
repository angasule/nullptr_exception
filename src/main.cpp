#include <stdio.h>
#include <unistd.h>

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#include <signal.h>

class NullPointerException
{
	int data;
};

void signal_handler(int signum, siginfo_t *info, void *)
{
	// info->si_addr holds the dereferenced pointer address
	if (info->si_addr == NULL) {
		// This will be thrown at the point in the code
		// where the exception was caused.
		throw NullPointerException();
	} else {
		// Now restore default behaviour for this signal,
		// and send signal to self.
		signal(signum, SIG_DFL);
		kill(getpid(), signum);
	}
}

int main(int argc, char **argv)
{
	struct sigaction act; // Signal structure

	act.sa_sigaction = signal_handler; // Set the action to our function.
	sigemptyset(&act.sa_mask); // Initialise signal set.
	act.sa_flags = SA_SIGINFO; // Our handler takes 3 params.
	sigaction(11, &act, NULL); // Set signal action to our handler.

	try {
		int *a = NULL;
		printf("%d\n", *a); // Cause Null Pointer Exception.
	}
	catch (...) {
		printf("Exception caught!\n"); // It works!
	}
	try {
		int *a = (int*)0x01;
		printf("%d\n", *a); // Cause Null Pointer Exception.
	}
	catch (...) {
		printf("Exception caught!\n"); // This should NOT be printed.
	}
}
