#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void usage()
{
	printf("usage: cpprogress <bufsize> <src> <dest>\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	char *buf;
	int n, m, bsize, fd_src, fd_dest;
	
	if(argc!=4) usage();
	
	bsize=atoi(argv[1]);
	if(bsize<0) usage();
	buf=malloc(bsize);
	if(!buf){
		printf("Could not malloc buffer.\n");
		usage();
	}
	
	fd_src=open(argv[2], O_RDONLY);
	if(fd_src<0){
		printf("Could not open %s\n", argv[2]);
		usage();
	}
	
	fd_dest=open(argv[3], O_WRONLY|O_TRUNC|O_CREAT, 0644);
	if(fd_dest<0){
		printf("Could not open %s\n", argv[3]);
		usage();
	}
	
	while((n=read(fd_src, buf, bsize))){
		m=write(fd_dest, buf, n);
		if(n!=m){
			printf("write error\n");
			return 2;
		}
		printf("1\n");
		fflush(0);
	}
	close(fd_src);
	close(fd_dest);
	return 0;
}