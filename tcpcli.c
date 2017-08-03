#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h>

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

	char buf[1024];
	char *ptr = buf;
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
