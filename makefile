echo : tcpechosrv.o 
	cc -o echo tcpechosrv.o 
tcpechosrv.o :
	cc -c tcpechosrv.c
clean :
	rm echo tcpechosrv.o
