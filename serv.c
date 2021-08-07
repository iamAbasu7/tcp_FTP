#include <sys/socket.h>  
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include<sys/stat.h>


int cli_count = 0;
int uid =1;

//char *msg="FILE ALREADY EXISTS";

/* Client structure */
typedef struct
{
	struct sockaddr_in address;
        int sockfd;
        int uid;
        char filename[1024];
} client_t; 



/* Thread Function */
void *connection_handler(void *arg)
{
	
	

   	printf("Connection established to client\n");
	client_t *cli = (client_t *)arg;
	int rb=0;
   	char rbuf[1024];
   	char filename[1024];
	char string[1024];
   	int leave_flag = 0;
   	cli_count++;
   	int z=0;

   	z=recv(cli->sockfd, string, 1024, 0);
	if(z<=0)
	{
		printf("Filename not received\n");
	//	goto S;
		
	}
	char *ptr=string+strlen(string);
	int len=strlen(string);
	while(len)
	{
		ptr--;
		len--;
		if(*ptr=='/')
		{
			ptr++;
			//filename=(char *)malloc(sizeof(ptr));
			strcpy(filename,ptr);
			break;
		}
	}
	//char str[1024];
	char *str;
	char num[3];
	char *name;
	//strcpy(cli->filename,filename);
	//strcpy(str,filename);
	
	str=strdup(filename);
	bzero(filename,1024);
	name=strdup(str);
	
	//overwrite file
	//char str1[]=cli->filename;
		//sprintf(num,"%d",uid);
	//*str=(char *)malloc((strlen(str)+1)*sizeof(char));
	FILE *file;
	char ch[7]="(copy)";
	//no=strdup(
	int i=1;
	int a=0;	
	P:if((file=fopen(str,"r"))!=NULL)
	{
	
		//remove file contents(over write)->
	//	fclose(fopen(cli->filename, "w"));
	//	fclose(file);
	   /*---------------------------------------------*/	
		//stop two clients to write at a same time on same file
		//goto S;
	  /*------------------------------------------------*/
	  	
		//if same file then rename
		bzero(str,strlen(str));
		str=strdup(name);
		//bzero(name,strlen(name));
		//strcpy(str,filename);
		strncat(str,ch,strlen(ch));


		a=i;
		i++;
		sprintf(num,"%d",a);
		//strncat(ch,num,strlen(num));
		//bzero(num,strlen(num));
		//strncat(str,ch,strlen(ch));
		//bzero(ch,strlen(ch));
		strncat(str,num,strlen(num));
	
		//memset(ch,0,strlen(ch));			
		
		goto P;
		//memset(str,0,strlen(str));
		//free(str);
	}
	char location[1024]="/home/antar/Documents/FileTransfer/Server_/";
	strncat(location,str,strlen(str));
	//strcpy(cli->filename,str);
		
        memset(rbuf,0,sizeof(rbuf));
        while ((rb = recv(cli->sockfd, rbuf, 1024,0))>0)
	{
       		FILE *out;
        	//out = fopen(cli->filename, "a");
		out=fopen(location,"a");
        	printf("%s\n", rbuf);
        	fwrite(rbuf,1,strlen(rbuf),out);
        	bzero(rbuf,1024);
        	fclose(out);
        }
	
	
       S:printf("Closing thread socket.\n");
   	close(cli->sockfd);
   	cli_count--;
  	free(cli);
	free(str);
    	return 0;
	
}

int main(int argc, char **argv)
{	

        if(argc != 2)
	{
                printf("Usage: %s <port>\n", argv[0]);
                return EXIT_FAILURE;
        }
        char *ip = "127.0.0.1";
        int port = atoi(argv[1]); 
	int option = 1;
	int listenfd = 0, connfd = 0;
  	struct sockaddr_in serv_addr; 
  	struct sockaddr_in cli_addr;
  	pthread_t tid;
	
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
  	serv_addr.sin_family = AF_INET; 
  	serv_addr.sin_addr.s_addr = inet_addr(ip);
  	serv_addr.sin_port = htons(port);

	/* Ignore pipe signals */
        signal(SIGPIPE, SIG_IGN); 
	if(setsockopt(listenfd, SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0)
	{ 
     		perror("ERROR: setsockopt failed");
    		return EXIT_FAILURE;
        }

        /* Bind */
  	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
	{
   		 perror("ERROR: Socket binding failed");
    		 return EXIT_FAILURE;
 	}

  	/* Listen */
 	if (listen(listenfd, 10) < 0) 
	{
    		perror("ERROR: Socket listening failed");
    		return EXIT_FAILURE;
        }
	while(1)
	{
		socklen_t clilen=sizeof(cli_addr);
		connfd=accept(listenfd,(struct sockaddr *)&cli_addr,&clilen);

		if(connfd<0)
       		{
                	printf("server accept fail");
                	exit(0);
        	}
        	else
        	{
                	printf("sever accept the client\n");
                
        	}


        /* Client settings */
        client_t *cli = (client_t *)malloc(sizeof(client_t));// dynamically giving memory for the client
        cli->address = cli_addr;
        cli->sockfd = connfd;
        cli->uid = uid++;
	
	/* Creating thread */
	
	

	pthread_create(&tid, NULL, &connection_handler, (void*)cli);
	
	
        }
        return EXIT_SUCCESS;
}


