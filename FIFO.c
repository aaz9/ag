#include<stdio.h> 
#include<unistd.h>  
#include<string.h> 
#include<stdlib.h> 

int main(void) 
{ 
	int fd1[2], nbytes=1,fd2[2],a=0; 
	pid_t pid; 
	char string[80]; 
	char readbuffer[80];
	char ch='a',ch1='\n';
	FILE *fp; 
	
	pipe(fd1);//PIPE CREATED
	pipe(fd2);//PIPE CREATED 

	/*Error in fork*/
	if((pid = fork()) == -1) 
	{ 
		perror("fork"); 
		exit(1); 
	} 

	//Child Process
	if(pid == 0) 
	{  
		close(fd1[1]);           /*closing write end of Pipe 1*/
		read(fd1[0], readbuffer, sizeof(readbuffer));     /*reading filename through Pipe 1*/
		printf("\nFilename '%s' is being read by Child Process through Pipe 1...\n",readbuffer);
		fp=fopen(readbuffer,"r");
		close(fd1[0]);         /*closing read end of Pipe 1*/
		close(fd2[0]);        /*closing read end of Pipe 2*/
		printf("\nContents of %s are being sent to Parent Process through Pipe 2...\n",readbuffer);
		while(a!=-1)
		{
			a=fscanf(fp,"%c",&ch);
			write(fd2[1], &ch, sizeof(ch));   /*writing contents of file on Pipe 2*/
		} 
		close(fd2[1]); /*closing write end of Pipe 2*/
		exit(0); 
	} 
	//Parent process
	else 
	{  
		close(fd1[0]); /*closing read end of Pipe 1*/
		printf("IN PARENT PROCESS\n" );
		printf("\nEnter name of file:");
		scanf("%s",string); 
		printf("Filename is being sent by Parent Process to Child Process through Pipe 1...\n"); 
		write(fd1[1], string, (strlen(string)+1));  /*writing filename on Pipe 1*/
		wait();
		close(fd1[1]); /*closing write end of Pipe 1*/
		close(fd2[1]);  /*closing write end of Pipe 2*/

		printf("\nContents of %s are being received by Parent Process through Pipe 2...\n\n",string);
		printf("IN PARENT PROCESS\n" );
		printf("\nReceived Message:\n");

		while(nbytes!=0) 
		{
			printf("%c",ch1);
			nbytes = read(fd2[0], &ch1, sizeof(ch1));   /*reading contents of file from Pipe 2*/
		} 
		close(fd2[0]); /*closing read end of Pipe 2*/
	} 
	return(0); 
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#include <stdio.h>
int n,nf;
int in[100];
int p[50];
int hit=0;  //for counting no-faults
int i,j,k;
int pgfaultcnt=0;  //for counting page faults

//Function for taking input from the user
void Getdata()
{
    printf("Enter the length of reference sequence:\n"); //for taking length of string from user
    scanf("%d", &n);
    printf("Enter the page reference sequence:\n");  //for taking string from user
    for(i=0; i<n; i++)
    {
        scanf("%d", &in[i]);
    }
    printf("Enter the number of page frames:\n");  //taking number of page frames from the user
    scanf("%d", &nf); 
}
//Function ends

//Function to initializing the page frames array
void initialize()
{
    pgfaultcnt=0;
    for(i=0; i<nf; i++)
        p[i]=9999;
}
//Function ends

//Function to count no-faults
int isHit(int data)
{
    hit=0;
    for(j=0; j<nf; j++)
    {
        if(p[j]==data)
        {
            hit=1;
            break;
        }
 
    }
 
    return hit;
}
//Function ends

//Function to get the index of the no-faults 
int getHitIndex(int data)
{
    int hitind;
    for(k=0; k<nf; k++)
    {
        if(p[k]==data)
        {
            hitind=k;
            break;
        }
    }
    return hitind;
}
//Function ends

//Function to display pages
void dispPages()
{
    for (k=0; k<nf; k++)
    {
        if(p[k]!=9999)
            printf(" %d",p[k]);
    }
 
}
//Function ends

//Function to display count of page faults
void dispPgFaultCnt()
{
    printf("\nTotal no of page faults:%d",pgfaultcnt);
}
//Function ends

//Function to implement FIFO Page Replacement Algorithm
void fifo()
{
    initialize();
    for(i=0; i<n; i++)
    {
        printf("\nFor %d :",in[i]);
 
        if(isHit(in[i])==0)
        {
 
            for(k=0; k<nf-1; k++)
                p[k]=p[k+1];
 
            p[k]=in[i];
            pgfaultcnt++;
            dispPages();
        }
        else
            printf("No page fault");
    }
    dispPgFaultCnt();
}
//Function ends

//Main function
int main()
{
    Getdata();
    fifo();
    return 0;
}
