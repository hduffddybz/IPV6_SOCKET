#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "10002"
#define SERVADDR "fe80::200:eff:fe12:3456%wlan0"
#define MESSAGE "hi there"

int main(void)
{
	struct addrinfo hints = {0};
	struct addrinfo *res;
	int get_err;
	int sockfd;
	char buffer[1024];

	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
	
	get_err = getaddrinfo(SERVADDR, PORT, &hints, &res);
	if(get_err)
	{
		fprintf(stderr, "getaddrinfo:%s\n", gai_strerror(get_err));
		return 1;
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd < 0)
	{
		perror("socket");
		return 1;
	}
	if(sendto(sockfd, MESSAGE, sizeof(MESSAGE), 0, res->ai_addr, res->ai_addrlen) < 0)
	{
		perror("sento failed");
		exit(4);
	}
	close(sockfd);

	return 0;
}
