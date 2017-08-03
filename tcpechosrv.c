#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>

void sigchld_cb(int signo);
ssize_t writen(int fd, const void * buf, size_t n);
void echo_line(int fd);

int
main()
{
	int srvfd;
	struct sockaddr_in srvaddr;
   	struct sockaddr	cliaddr;
	int clilen;
	srvfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(srvfd < 0){
		perror("new socket failed");
		return 1;
	}

	memset(&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8080);
	if(inet_aton("127.0.0.1", &srvaddr.sin_addr) != 1){
		perror("invalid address");
		return 1;
	}

	/* bind addr */
	if(bind(srvfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0){
		perror("bind sock failed");
		return 1;
	}

	/* listen */
	if(listen(srvfd, 5) < 0){
		perror("listen sock failed");
		return 1;
	}

	while(1){
		int clifd = accept(srvfd, NULL, NULL);
		if(clifd < 0){
			perror("accept failed");
			continue;
		}
		signal(SIGCHLD, sigchld_cb);

		switch(fork()){
			case 0:
				{
					close(srvfd);
					echo_line(clifd);
					close(clifd);
					exit(0);
					break;
				}
			case -1:
				printf("fork failed\n");
				break;
			default:
				close(clifd);
				break;
		}

		waitpid(-1, NULL, WNOHANG);
	}

	exit(0);
}

void
sigchld_cb(int signo)
{
	if(signo == SIGCHLD){
		pid_t pid = waitpid(-1, NULL, WNOHANG);
		if(pid){
			printf("child process : %d terminated\n", pid);
		}
	}
}

void
echo_line(int fd)
{
	char buf[1024];
	int n;
	while(n = read(fd, buf, sizeof(buf))){
		if(n < 0){
			if(errno == EINTR){
				continue;
			}
			break;
		}else if(n == 0){
			break;
		}
		writen(fd, buf, n);
	}
}

ssize_t
writen(int fd, const void * buf, size_t n)
{
	size_t nleft = n;
	void * ptr = buf;

	while(nleft > 0){
		size_t nw = write(fd, ptr, nleft);
		if(nw < 0){
			if(errno == EINTR){
				continue;
			}
			return nw;
		}

		nleft -= nw;
		ptr += nw;
	}

	return n - nleft;
}
