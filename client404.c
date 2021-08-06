#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
char filename[1024];
//char *filename

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') { //if there is a '\n or enter given by the user
      arr[i] = '\0'; //it means your msg is end ('\0' the string termination)
      break;
    }
  }
}


void  INThandler(int sig)
{


     signal(sig, SIG_IGN); //catch the ctrl+c command from user
     exit(0);
     
}


int main(int argc, char **argv){
        char str[100];
        if(argc != 2){
                printf("Usage: %s <port>\n", argv[0]);
                return EXIT_FAILURE;
        }
	int sockfd=0;
	size_t len=0;
	char sbuf1[1024];
        char *ip = "127.0.0.1";
        int port = atoi(argv[1]);

        signal(SIGINT, INThandler);
	struct sockaddr_in servAddr;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(ip);
  servAddr.sin_port = htons(port);
//Connect to the server
int err=connect(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr));
if(err<0)
{
    perror("Failure to connect to the server");
    exit(1);
}

else
{
	printf("Accepted\n");


g: printf("Enter the Filename: ");
 signal(SIGINT,INThandler);
 fgets(filename,1024,stdin);
 //scanf("%m[^\n]",&filename);
 str_trim_lf(filename,strlen(filename));
 if(strlen(filename)>1024 || strlen(filename)<1)
{
	printf("Please Enter a valid file name between 1 to 1024 characters\n");

	goto g;
	//free(filename);
//	return EXIT_FAILURE;
}


FILE *in;
in = fopen(filename, "r");
if(in==NULL)
{
	printf("File Dosen't Exist, Please enter an existing filename\n");

	goto g;
	//free(filename);
}
send(sockfd,filename,1024,0);
/*if((read(sockfd,sbuf1,sizeof(sbuf1)))>0)
          {
                  printf("%s\n",sbuf1);
		  memset(sbuf1,0,sizeof(sbuf1)+1);

                  goto A;
          }
*/
int n=0;
 //Client begins to write and read from the server
  memset(sbuf1,0,sizeof(sbuf1)+1);
  printf("Client: transferring file\n");
  while((n = fread(sbuf1,1,1024,in)) != 0){
	  
     printf("File contents: %s \n", sbuf1);
      send(sockfd, sbuf1, 1024,0);
      bzero(sbuf1,1024);
  }
  fclose(in);
 // free(filename);

 printf("File transfer complete. Closing socket.\n");
 //Close socket
 close(sockfd);
 return 0;
}
}

