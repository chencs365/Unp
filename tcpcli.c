#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h>

ssize_t writen(int fd, const void * buf, size_t n);

int
main()
{
	int clifd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(clifd < 0){
		perror("new socket failed");
		return 1;
	}

	/*
	if(bind(clifd, INADDR_ANY, 0) < 0){
		perror("bind socket failed");
		return 1;
	}
	*/

	struct sockaddr_in srvaddr;
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8080);
	if(inet_aton("127.0.0.1", &srvaddr.sin_addr) != 1){
		perror("invaild address");
		return 1;
	}


	if(connect(clifd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0){
		perror("connect to server failed");
		return 1;
	}

	/* send line */
	char buf[1024];
	gets(buf);
	writen(clifd, buf, strlen(buf) + 1);

	char buf2[1024];
	char *ptr = buf2;
	int nr = 0;
	while(1){
		int n = read(clifd, buf, sizeof(buf));
		if(n < 0){
			if(errno == EINTR){
				continue;
			}
			break;
		}else if(n == 0){
			break;
		}

		ptr += n;
		nr += n;
	}

	buf[nr] = '\0';

	printf("read %s form server\n", buf);
	exit(0);
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
