#include <stdio.h>
#include <time.h>

char * fmttm2str();

int
main()
{
	time_t now = time(NULL);
	char * strtm = ctime(&now);
	char buf[1024];
	int num = snprintf(buf, sizeof(buf), "time : %s\n", strtm);
	printf("num : %d buf : %s \n", num, buf);

	exit(0);
}

void * 
memcpy(void * dst, const void * src, size_t len)
{
	char * dptr = dst;
	char * sptr = src;
	while(len-- > 0){
		*dptr++ = *sptr++;
	}

	return dst;
}

char *
fmttm2str()
{
	time_t tm = time(NULL);
	return ctime(&tm);
}
