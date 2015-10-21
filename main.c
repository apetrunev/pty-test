#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int main(int argc, char **argv)
{
	
	int pfd, status;
	pid_t pid;
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


	pid = fork();
	if (pid < 0) {
		fprintf(stderr, "error %s\n", strerror(errno));
		close(pfd);
		exit(EXIT_FAILURE);
		
	}	
	
	/* child process */
	if (pid == 0) {
		/* close master terminal */
		close(pfd);
		
		if (setsid() < 0) {
			fprintf(stderr, "error: setsid() faild\n")
			exit(EXIT_FAILURE);
		}

		/* open a slave terminal 
                 * now it becomes a controlling terminal for the process 
                 */
		if ((sfd = open(pname, O_RDWR)) < 0) {
			fprintf(stderr, "cannot open %s\n", pname);
			exit(EXIT_FAILURE);
		}
		
	
		if (dup2(sfd, STDIN_FILENO) != STDIN_FILENO) {
			fprintf(stderr, "error: dup2() failed\n");
			exit(EXIT_FAILURE);
		}

		if (dup2(sfd, STDOUT_FILENO) != STDOUT_FILENO) {
			fprintf(stderr, "error: dup2() failed\n");
			exit(EXIT_FAILURE);
		}	

		if (dup2(sfd, STDERR_FILENO) != STDERR_FILENO) {
			fprintf(stderr, "error: dup2() failed\n");
			exit(EXIT_FAILURE);
		}	
		
		/* no longer need */
		if (sfd > STDERR_FILENO) 
			close(sfd);
				
		exit(EXIT_SUCCESS);
	}

	/* parent */
	waitpid(pid, &status, 0);
 
	return EXIT_SUCCESS;
}
