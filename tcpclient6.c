#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define PORT "10002"
#define SERVADDR "fe80::200:eff:fe12:3456%wlan0"
static const char send_data[] = "This is TCP Client from Debian.";

int main(void)
{
	struct addrinfo hints = {0};
	struct addrinfo *res;
	int get_err;
	int sockfd;
	int len;
	char buffer[1024];
	

	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	
	get_err = getaddrinfo(SERVADDR, PORT, &hints, &res);
	if(get_err)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(get_err));
		return 1;
	}
	
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd < 0)
	{
		perror("socket");
		return 1;
	}
	if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("connect");
		return 1;
	}
	send(sockfd, send_data, strlen(send_data), 0);
	printf("reading message\n");
	len = read(sockfd, buffer, 1024);
	buffer[len] = '\0';
	printf("got '%s'\n", buffer);
		
	close(sockfd);
	return 0;
}
