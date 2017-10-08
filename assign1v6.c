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
        char tmp[128];
        int num;

        if(pipe(p1) == -1)
                {
                        printf("Couln't create pipe 1\n");
                        exit(0);
                }
        pid1 = fork();

        if(pid1 == 0) {
                //child 1 process
                if(pipe(p2)==-1){
					printf("pipe 2 failed\n");
					}					
                pid2 = fork();
                if (pid2 == 0){		
					//printf("child 2 run\n");
                        //child 2 process detection
                        if (pid3 == 0){
	                            // printf("child 3 run2\n");
                                
                                //child 3 process
                                close(p1[1]);
                                close(p1[0]); 
                                close(p4[0]);
                                fpc3 = fopen("child3log.txt","w");
                               
                                p3 = open("mypipe", O_RDONLY);
                                 printf("child 3 \n");
                                while (read(p3,inbuf, 128) >0) {
                                        printf("child 3 pipe %s",inbuf);
                                        sscanf(inbuf, "%d\t%s", &num, inbuf);
                                        if (num ==3){ // keeps the message
                                                printf("CHILD 3 - - %s\tkeep\n",inbuf);
                                                fprintf(fpc3, "%s%s\tkeep\n", asctime(localtime(&currentTime)), inbuf);
                                                fflush(fpc3);
                                        } // end if
                                        else { // forwards the message
                                                printf("CHILD 3 - - %s\tforward\n", inbuf);
                                                fprintf(fpc3, "%s%s\tforward\n", asctime(localtime(&currentTime)), inbuf);
                                                fflush(fpc3);
                                                sprintf(tmp,"%d\t%s",num,inbuf); 
                                                write(p4[1],tmp, 128);
                                        } // end else
                                     close(p4[1]);
                                     close(p3);
                                } //end while
                           // exit(0);//terminate child 3
                        }//end child 3
                       
                       
                                //continue child 2
                               // printf("child 2 run\n");
                              close(p4[1]);
                              close(p4[0]);
                              close(p2[1]);
                                fpc2 = fopen ("child2log.txt", "w");
                                
                                while(read(p2[0],inbuf, 128) > 0){
									//printf("pipe2 run\n");
                                        sscanf (inbuf, "%d\t%s", &num, inbuf);
                                       
                                        if (num == 2) {
                                                printf ("CHILD 2 - - %s\tkeep\n", inbuf);
                                                fprintf(fpc2, " %s %s\tkeep\n", asctime(localtime(&currentTime)), inbuf);
                                                fflush(fpc2);
                                        }
                                        else {
                                                printf("CHILD 2 - - %s\tforward\n", inbuf);
                                                fprintf(fpc2, "%s %s\t forward\n", asctime(localtime(&currentTime)), inbuf);
                                               p3= open ("mypipe", O_WRONLY);
                                                sprintf(tmp,"%d\t%s",num,inbuf); 
                                                write(p3,tmp,128);
                                                 fflush(fpc2);
                                                 close(p3); 
                                        }
                                        
                                      
                                }// end while
                              close(p2[0]);
                             
                              //exit(0);//terminate child 2 process
                        
                } // end if child 2 process 
                else {
                     
                        //child 1 process
                      close(p1[1]);
                      close(p2[0]);
                      close(p4[1]);
                      close(p4[0]);
                        fpc1 = fopen("child1log.txt","w");
                        while (read (p1[0],inbuf, 128) > 0 ){
                                sscanf(inbuf, "%d\t%s", &num, inbuf);
                                if (num == 1){
                                        printf("CHILD 1 - - %s\tkeep\n", inbuf);
                                        fprintf(fpc1, " %s %s\tkeep\n",asctime(localtime(&currentTime)), inbuf);
                                        fflush(fpc1);
                                }
                                else {
                                        printf("CHILD 1- - %s\tforward\n", inbuf);
                                        fprintf(fpc1, " %s %s\tforward\n", asctime(localtime(&currentTime)), inbuf);
                                        fflush(fpc1);
                                        sprintf(tmp,"%d\t%s",num,inbuf); 
                                        //printf("write in pipe 2 child 1 %s\n",tmp);                                  
                                        write(p2[1],tmp, 128);
                                }
                        } // end while
                    close(p1[0]);
                    close(p2[1]);
                      //exit(0); //terminate child 1
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
                                //printf ("%s",inbuf);
                                write(p1[1],inbuf,128);
                        }// end while

                }//end else
                fclose(fpmessage);
                close(p1[1]);

                wait(&pid1);
                wait(&pid2);

                pid3 = fork();
                mkfifo("mypipe",0666);
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
                } //end while for writing log in parent
        }// end else for parent process
}//end main

