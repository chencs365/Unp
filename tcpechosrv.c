#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/wait.h>

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

		time_t now = time(NULL);
		char * strtm = ctime(&now);
		char buf[1024] = {0};
		int num = snprintf(buf, sizeof(buf), "time : %s\n", strtm);
		printf("num : %d\n", num);
		int n = write(clifd, buf, sizeof(buf));
		printf("%d byte write\n", n);
		close(clifd);
	}

	exit(0);
}

