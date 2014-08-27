/*
  Example IPv6 UDP server.
  Copyright (C) 2010 Russell Bradford

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, 
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (http://www.gnu.org/copyleft/gpl.html)
  for more details. 
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 12345
#define MESSAGE "hello"

int main(void)
{
  int sock;
  socklen_t clilen;
  struct sockaddr_in6 server_addr, client_addr;
  char buffer[1024];
  char addrbuf[INET6_ADDRSTRLEN];

  /* create a DGRAM (UDP) socket in the INET6 (IPv6) protocol */
  sock = socket(PF_INET6, SOCK_DGRAM, 0);

  if (sock < 0) {
    perror("creating socket");
    exit(1);
  }

#ifdef V6ONLY
  // setting this means the socket only accepts connections from v6;
  // unset, it accepts v6 and v4 (mapped address) connections
  { int opt = 1;
    if (setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
      perror("setting option IPV6_V6ONLY");
      exit(1);
    }
  }
#endif

  /* create server address: this will say where we will be willing to
     accept datagrams from */

  /* clear it out */
  memset(&server_addr, 0, sizeof(server_addr));

  /* it is an INET6 address */
  server_addr.sin6_family = AF_INET6;

  /* the client IP address, in network byte order */
  /* in this example we accept datagrams from ANYwhere */
  server_addr.sin6_addr = in6addr_any;

  /* the port we are going to listen on, in network byte order */
  server_addr.sin6_port = htons(PORT);

  /* associate the socket with the address and port */
  if (bind(sock, (struct sockaddr *)&server_addr,
	   sizeof(server_addr)) < 0) {
    perror("bind failed");
    exit(2);
  }

  while (1) {

    /* now wait until we get a datagram */
    printf("waiting for a datagram...\n");
    clilen = sizeof(client_addr);
    if (recvfrom(sock, buffer, 1024, 0,
		 (struct sockaddr *)&client_addr,
		 &clilen) < 0) {
      perror("recvfrom failed");
      exit(4);
    }

    /* now client_addr contains the address of the client */
    printf("got '%s' from %s\n", buffer,
	   inet_ntop(AF_INET6, &client_addr.sin6_addr, addrbuf,
		     INET6_ADDRSTRLEN));

    printf("sending message back\n");

    if (sendto(sock, MESSAGE, sizeof(MESSAGE), 0,
               (struct sockaddr *)&client_addr,
	       sizeof(client_addr)) < 0) {
      perror("sendto failed");
      exit(5);
    }

  }
  
  return 0;
}
