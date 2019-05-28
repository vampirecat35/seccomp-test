#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "config.h"
#include "seccomp-bpf.h"
#include "syscall-reporter.h"

static int install_syscall_filter(void)
{
	struct sock_filter filter[] = {
		/* Validate architecture. */
		VALIDATE_ARCHITECTURE,
		/* Grab the system call number. */
		EXAMINE_SYSCALL,
		/* List allowed syscalls. */
		ALLOW_SYSCALL(rt_sigreturn),
#ifdef __NR_sigreturn
		ALLOW_SYSCALL(sigreturn),
#endif
		ALLOW_SYSCALL(exit_group),
		ALLOW_SYSCALL(exit),
		ALLOW_SYSCALL(read),
		ALLOW_SYSCALL(write),
		KILL_PROCESS,
	};
	struct sock_fprog prog = {
		.len = (unsigned short)(sizeof(filter)/sizeof(filter[0])),
		.filter = filter,
	};

	if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
		perror("prctl(NO_NEW_PRIVS)");
		goto failed;
	}
	if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
		perror("prctl(SECCOMP)");
		goto failed;
	}
	return 0;

failed:
	if (errno == EINVAL)
		fprintf(stderr, "SECCOMP_FILTER is not available. :(\n");
	return 1;
}

int main() {
	if (install_syscall_reporter())
		return 1;	
	if (install_syscall_filter())
		return 1;	
	
	pid_t forkStatus;
	forkStatus = fork();

	/* Hijo... */
	if (forkStatus == 0) {
		printf("Hijo en ejecucion. Espere.\n");
		sleep(2);
		printf("Hijo terminado, saliendo...\n");

	/* Padre... */
	} else if (forkStatus != -1) {
		printf("Proceso padre a la espera...\n");
		wait(NULL);
		printf("Padre terminado...\n");

	} else {
		perror("Error al llamar a la funcion fork()");
	}
	return 0;
}