#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int main()
{
        pid_t pid1,pid2,pid3; //process
        int p1[2],p2[2],p3,p4[2]; //pipe
        FILE *fpmessage;
        FILE *fpparent;
        FILE *fpc1;
        FILE *fpc2;
        FILE *fpc3;
        time_t currentTime;

        char inbuf[128];
        int num;

        if(pipe(p1) == -1){
            	printf("Couln't create pipe 1\n");
            	exit(0);
        }
        pid1 = fork();

        if(pid1 == 0) {
            	//child 1 process
            	pipe(p2);
            	pid2 = fork();
            	if (pid2 == 0){
                		//child 2 process detection
                		if (pid3 == 0){
                    			//child 3 process

                		}//end child 3
                		else {
                   				//continue child 2

                    		exit(0);
                		}
            	}
            	else {
                //child 1 process

                exit(0); //terminate child 1
            	}

        }
	else {
                //parent process
                fpmessage = fopen("message.txt","r");
                if (fpmessage == NULL) {
                        printf ("File can't open\n");
                        exit(0);
                }
                else {
                        close(p1[0]);
                        while (!feof(fpmessage)){
                                fgets(inbuf,128,fpmessage);
                                printf ("%s",inbuf);
                                write(p1[1],inbuf,128);
                        }// end while
                }//end else
                fclose(fpmessage);
                close(p1[1]);

                wait(&pid1);
                wait(&pid2);

                pid3 = fork ();
                mkfifo("pipe3",0660);
                if (pipe(p4) == -1){
                        printf("Can't create pipe 4 \n");
                        exit(0);
                }
                wait (&pid3);
                close(p4[1]);
                while (read(p4[0],inbuf,128)!= 0)
                {
                        printf ("%s\n", inbuf);
                        sscanf (inbuf,"%d\t%s", &num, inbuf);
                        printf("%d\n",num);
                        printf("%s\n",inbuf);
                        if (num == 4)
                        {
                                fpparent =fopen("parentlog.txt","w");
                                if (fpparent == NULL){
                                        printf ("File fail to create");
                                        exit(0);
                                }
                                currentTime = time (NULL);
                                fprintf(fpparent, "%s\t%s\tkeep\n",asctime(localtime(&currentTime)),inbuf);
                                fflush(fpparent);
                        }
                        else {
                                printf ("%s\n",inbuf);
                                printf ("Message left error");
                        }
		} //end while fpr writing log in parent
        }// end else for parent process
} end main


