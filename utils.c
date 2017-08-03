#include <sys/types.h>
#include <errno.h>

ssize_t
readn(int fd, void * buf, size_t n)
{
	size_t n_sur = n;
	void * ptr = buf;
	while(n_sur > 0){
		ssize_t nr = read(fd, ptr, n_sur);
		if(nr < 0){
			if(errno == EINTR)
				continue;
			return nr;
		}else if (n == 0){
			break;
		}

		n_sur -= nr;
		buf += nr;
	}

	return n - n_sur;
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

	return n;
}

ssize_t
readline(int fd, void * buf, size_t maxlen)
{
	void * ptr = buf;
	size_t nread = 0;
	while(nread < maxlen){
		ssize_t n = read(fd, ptr, 1);
		if(n < 0){
			if(errno == EINTR){
				continue;
			}
			return n;
		}else if(n == 0){
			break;
		}else if(*(char *)ptr == '\n'){
			return nread + 1;
		}

		ptr += n;
		nread += n;
	}

	*ptr = '\0';
	return nread;
}
