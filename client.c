#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>

#define BUFSZ 1024

void logexit(const char *str) {
	perror(str);
	exit(EXIT_FAILURE);
}

void sigint_handler(int signun) {
    close(signun);
}

int main(int argc, char **argv) {

    char *ip = argv[1];
    int port = atoi(argv[2]);

    // printf("%s\n%i\n%s\n%i\n", ip, port, message, x);

	int s;

    // internet address, userd by sockaddr_in
	struct in_addr inaddr;
    // convert IPv4 and IPv6 adsresses from text to binary form
    inet_pton(AF_INET, ip, &inaddr);

    // IPv4 AF_INET6 sockets
	struct sockaddr_in addr;
    // Interface for sockets. It's a parameter for connect function.
	struct sockaddr *addrptr = (struct sockaddr *)&addr;
	addr.sin_family = AF_INET; // address family, AF_xxx. If it were IPv6, then it would be AF_INET6.
	addr.sin_port = htons(5152); // converts the unsigned short integer hostshort from host byte order to network byte order.
	addr.sin_addr = inaddr;

    // sockfd = socket(int socket_family, int socket_type, int protocol);
    // DÚVIDA AQUI
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == -1)
        logexit("socket");

    // int connect(int socket, const struct sockaddr *address, socklen_t address_len);
	if (connect(s, addrptr, sizeof(struct sockaddr_in)))
		logexit("connect");

	// char linha[512];

    char *message = argv[3];
    unsigned int x = atoi(argv[4]);

    if (message[strlen(message)] == '\0')
        printf("Tem"); 

    char linha[512];
	printf("mensagem> ");
	fgets(linha, 511, stdin);
	ssize_t count;
	count = send(s, message, strlen(message), 0);
	if(count != strlen(linha))
		logexit("send");

	char buf[512];
	memset(buf, 0, 512);
	unsigned total = 0;
	while(1) {
		count = recv(s, buf+total, 512, 0);
		if(count == 0) break;
		total += count;
	}

	printf("received %d bytes\n", (int)total);
	puts(buf);

	exit(EXIT_SUCCESS);
}

