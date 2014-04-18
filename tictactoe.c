#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7890

int w,h;

void printGrid(int grid[][w]);
void sendGrid(int grid[][w], int fd);
void sendClr(int fd);

void fatal(char * message)
{
	char error_message[100];

	strcpy(error_message, "[!!] Fatal Error ");
	strncat(error_message, message, 83);
	perror(error_message);
	exit(-1);


}

// Dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length) {
unsigned char byte;
unsigned int i, j;
for(i=0; i < length; i++) {
byte = data_buffer[i];
printf("%02x ", data_buffer[i]); // Display byte in hex.
if(((i%16)==15) || (i==length-1)) {
for(j=0; j < 15-(i%16); j++)
printf(" ");
printf("| ");
for(j=(i-(i%16)); j <= i; j++) { // Display printable bytes from line.
byte = data_buffer[j];
if((byte > 31) && (byte < 127)) // Outside printable char range
printf("%c", byte);
else
printf(".");
}
printf("\n"); // End of the dump line (each line is 16 bytes)
} // End if
} // End for
}

int main()
{
	int sockfd, new_sockfd;
	struct sockaddr_in host_addr, client_addr;
	socklen_t sin_size;
	int recv_length = 1, yes = 1;
	char buffer[1024];

	if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)))
		fatal("setting socket option SO_REUSEADDR");

	host_addr.sin_family = AF_INET; /* Why can it be accessed as sin_family? */
	host_addr.sin_port = htons(PORT);
	host_addr.sin_addr.s_addr = 0;
	memset(&(host_addr.sin_zero),'\0',8);

	if(bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr))==-1)
		fatal("binding to socket");

	if(listen(sockfd,5) == -1)
		fatal("listening on socket");

	while(1){	//Accept loop
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			fatal("accepting connection");
		printf("server: got connection from %s port %d\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		
		int grid[3][3] = {};
		w = 3;
		h = 3;
		int cnt = 0;

		while(cnt < 9){
		system("clear");
		printf("Your turn\n");
		printGrid(grid);
		
		int d,i,j;

		int err = 0;
		do{
		if(err)
		{
			printf("Occupied.\n");
			getchar();
			err = 0;
		}
		printf("Enter a number (1-9): ");
		scanf("%d",&d);
		d--;
		i = d/3;
		j = d%3;

		
		if(grid[i][j])
		{
				
			err = 1;

		}

		}while(d<0 || d>8 || grid[i][j]);

		d = -1;

		system("clear");
		
		grid[i][j] = 1;
		cnt++;
		if(cnt == 9)
			break;
		printGrid(grid);

		sendClr(new_sockfd);
		send(new_sockfd,"Your turn\n",10,0);
		sendGrid(grid,new_sockfd);
		do{
		

		int recv_length = recv(new_sockfd, buffer, 1024, 0);
		//printf("Received %c\n",buffer[i]);
		d = buffer[0] - '1';
		i = d/3;
		j = d%3;
		//printf("Looking at i = %d j = %d\n",i,j);

		if(grid[i][j])
			send(new_sockfd,"Occupied.\n",10,0);

		d = -1;

		}while(buffer[0] <'1' || buffer[0] > '9' || grid[i][j]);


		grid[i][j] = 2;	
		sendClr(new_sockfd);
		sendGrid(grid, new_sockfd);
		cnt++;
		}
		char end_message[]="Game ended!\n";
		printf("%s",end_message);
		sendClr(new_sockfd);
		sendGrid(grid, new_sockfd);
		send(new_sockfd,end_message,strlen(end_message),0);
		close(new_sockfd);

	}

}

void printGrid(int grid[][w])
{
	int i, j;
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			if(grid[i][j] == 0)
			{
				printf(". ");
			}
			else if(grid[i][j] == 1)
			{
				printf("X ");
			}
			else if(grid[i][j] == 2)
			{
				printf("O ");
			}

		}
		printf("\n");

	}
}

void sendGrid(int grid[][w], int fd)
{
	

	int i, j;
	for(i=0; i<h; i++)
	{
		for(j=0; j<w; j++)
		{
			if(grid[i][j] == 0)
			{
				send(fd,". ",2,0);
			}
			else if(grid[i][j] == 1)
			{
				send(fd,"X ",2,0);
			}
			else if(grid[i][j] == 2)
			{
				send(fd,"O ",2,0);
			}

		}
		send(fd,"\n",1,0);

	}
	char str[] = "Enter a number: (1-9): ";
	send(fd,str,strlen(str),0);
}

void sendClr(int fd)
{
	int i;
	for(i=0; i<25; i++)
		send(fd,"\n",1,0);

}

