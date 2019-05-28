#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "config.h"

int main() {
	pid_t forkStatus;
	forkStatus = fork();

	/* Hijo... */
	if (forkStatus == 0) {
		printf("Hijo en ejecucion. Espere.\n");
		sleep(2);
		printf("Hijo terminado, saliendo..\n");

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