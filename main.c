#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char **argv)
{
	
	int pfd;
	char *pname;
		
	/* opent pty master */
 	if ((pfd = posix_openpt(O_RDWR | O_NOCTTY)) < 0) {
		fprintf(stderr, "error: cannot open master terminal device\n");
		exit(EXIT_FAILURE);
	}	
	/* grant access to slave pty */
	if (grantpt(pfd) < 0) {
		fprintf(stderr, "error: %s\n", strerror(errno));
		close(pfd);
		exit(EXIT_FAILURE);	
	}

	if (unlockpt(pfd) < 0) {
		fprintf(stderr, "error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	/* get name of the slave pty */ 	
	pname = ptsname(pfd);
	if (pname == NULL) {
		fprintf(stderr, "error: %s\n", strerror(errno));
		close(pfd);
		exit(EXIT_FAILURE);
	}

	close(pfd);
 
	return EXIT_SUCCESS;
}
