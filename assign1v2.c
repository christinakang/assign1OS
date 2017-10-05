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

        if(pipe(p1) == -1)
                {
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
                                
                                close(p1[1]);
                                close(p1[0]);
                      
                                close(p4[0]);
                                fpc3 = fopen("child3log.txt","w");
                                p3 = open("mypipe", O_RDONLY);
                                while (read(p3,inbuf, 128) >0) {
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
                                                write(p4[1],inbuf, 128);
                                        } // end else
                                     //close(p3[0]);
                                     close(p4[1]);
                                } //end while
                        //exit(0);
                        }//end child 3
                        else {
                                //continue child 2
                              close(p4[1]);
                              close(p4[0]);
                              close(p2[1]);
                              //close(p3[0]);
                                fpc2 = fopen ("child2log.txt", "w");
                                p3= open ("mypipe", O_WRONLY);
                                while(read (p2[0],inbuf, 128) > 0){
                                        sscanf (inbuf, "%d\t%s", &num, inbuf);
                                        if (num == 2) {
                                                printf ("CHILD 2 - - %s\tkeep\n", inbuf);
                                                fprintf(fpc2, " %s %s\tkeep\n", asctime(localtime(&currentTime)), inbuf);
                                                fflush(fpc2);
                                        }
                                        else {
                                                printf("CHILD 2 - - %s\tforward\n", inbuf);
                                                fprintf(fpc2, "%s %s\t forward\n", asctime(localtime(&currentTime)), inbuf);
                                                fflush(fpc2);
                                                write (p3,inbuf, 128);
                                        }
                                }// end while
                              close(p2[0]);
                              //close(p3[1]);
                              //exit(0);
                        } // end else
                } // end if
                else {
                        //child 1 process
                      close(p1[1]);
                      //close(p2[0]);
                      close(p4[1]);
                      close(p4[0]);
                        fpc1 = fopen("child1log.txt","w");
                        while (read (p1[0],inbuf, 128) > 0 ){
                                sscanf(inbuf, "%d\t%s", &num, inbuf);
                                   printf("child1 %s\n",inbuf);
                                /*if (num == 1){
                                        fprintf(fpc1, " %s %s\tkeep\n",asctime(localtime(&currentTime)), inbuf);
                                        fflush(fpc1);
                                }
                                else {
                                        printf("CHILD 1 - - %s\tforward\n", inbuf);
                                        fprintf(fpc1, " %s %s\tforward\n", asctime(localtime(&currentTime)), inbuf);
                                        fflush(fpc1);
                                        write(p2[1],inbuf, 128);
                                }*/
                        } // end while
                    close(p1[0]);
                    close(p2[1]);
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
                                //printf ("%s +parent",inbuf);
                                write(p1[1],inbuf,128);
                        }// end while

                }//end else
                fclose(fpmessage);
                close(p1[1]);

                wait(&pid1);
                wait(&pid2);

                pid3 = fork();
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
                } //end while for writing log in parent
        }// end else for parent process
}//end main
