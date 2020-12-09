#include <stdio.h>
#include <stdlib.h>
#define BUFSIZE 1024

int main(int argc, char **argv)
{
	char buf[BUFSIZE];
	int n =0 ;
	if(argc < 3){
		fprintf(stderr, "Usage:%s <src_filename> <dest_filename>\n", argv[0]);
		exit(1);
	}
	FILE* fps = NULL, *fpd = NULL;
	fps = fopen(argv[1],"r");
	if(fps == NULL){
		perror("fopen():fps:");
		exit(1);
	}
	fpd = fopen(argv[2],"w");
    if(fpd == NULL){
		fclose(fps); //第二个参数必须要关闭，不然存在内存泄漏！
		perror("fopen():fpd:");
		exit(1);
	}
	while((n = fread(buf, 1, BUFSIZE, fps)) > 0)
		fwrite(buf, 1, n , fpd);
		

	fclose(fpd);
	fclose(fps);

	exit(0);

}















