#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



extern char **environ;   //环境变量！

int main(){
    puts("Begin!");

    execl("/bin/date", "date", "+%s", NULL);
    perror("execl()");
    exit(1);

    puts("End!");

    exit(0);


}