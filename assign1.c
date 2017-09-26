
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <time.h>

char *msg = "Hello world";
int main()
{
	pid_t pid1,pid2,pid3;//process
	int p1[2],p2[2],p3[2],p4[2];//pipe
	char inbuf1[128];
	char inbuf2[128];
	char inbuf3[128];
	char inbuf4[128];
	
	if((pid1 = fork())==-1)
	exit(0); 
	
	//create pipe 1 
	if((pipe(p1)) == -1)
	exit(0);
	
	close(p1[0]);
	write(p1[1],msg,128);
	close(p1[1]);
	
	if(pid1 == 0)
	{
		printf("run if\n");
		//create process 2 
		pid2 = fork();
		pipe(p2);
		
		//read from parent pipe p1 
		close(p1[1]);
		close(p2[0]);
		while(read(p1[0],inbuf1,128)!=0)
		    {
				printf("PID1 %s\n",inbuf1);
				//write to pipe 2 
				write(p2[1],inbuf1,128);
				close(p2[1]);
		    }
		    close(p1[0]);
				exit(0);
	}//end process1 if 
		else if(pid2==0)
		{
			//create process 3 
			pid3 = fork();
			pipe(p3);
			
			close(p2[1]);
			close(p3[0]);
			
		while(read(p2[0],inbuf2,128)!=0)
		    {
				printf("PID2 %s", inbuf2);
				//write to pipe 3 
				write(p3[1],inbuf2,128);
			
		    }
							
			exit(0);
		}//end process2 if
		
		else if(pid3==0)
		{
			pipe(p4);
			close(p3[1]);
			close(p4[0]);
		while(read(p2[0],inbuf3,128)!=0)
		    {
				printf("PID3 %s", inbuf3);
				//write to pipe 3 
				write(p4[1],inbuf3,128);
		    }	
			exit(0);
		}//end process 3 if

	
	else{
		//parent process
		wait(&pid1);
		wait(&pid2);
		wait(&pid3);
		
		close(p1[0]);
		close(p4[1]);
		
		while(read(p4[0],inbuf4,128)!=0)
		    {
				printf("PID parent %s",inbuf4);
		    }	
		
	}
	
	return 0;
}

