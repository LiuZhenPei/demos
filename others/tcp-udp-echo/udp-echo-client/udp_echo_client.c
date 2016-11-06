#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024 // buf size

int main(int argc, char *argv[]) {
	int conn_fd, port;
	char buf[BUFFER_SIZE];
	socklen_t addr_len;
	struct sockaddr_in serv_addr;
	
	// check the arguments
	if (argc != 2) {
		printf("please check your input.\n");
		printf("such as ./udp_client [portnumber]\n");
		return -1;
	}

	// initialize address
	port = atoi(argv[1]);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// create udp socked
	if ((conn_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		goto err;

	bzero(buf, sizeof(buf));	
	
	// input from keyboard
	if ((read(STDIN_FILENO, buf, sizeof(buf))) < 0)
		goto err;
	
	// send data
	if ((sendto(conn_fd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr, 
				sizeof(struct sockaddr_in))) < 0) {
		perror("sendto");
		goto err;
	}
	
	// receive data
	bzero(buf, sizeof(buf));
	addr_len = sizeof(struct sockaddr_in);
	if ((recvfrom(conn_fd, buf, sizeof(buf), 0, (struct sockaddr *)&serv_addr,
				&addr_len)) < 0) {
		perror("recvfrom");
		goto err;
	}

	printf("back data: %s\n", buf);
	
	close(conn_fd);

	return 0;

err:
	close(conn_fd);
	
	return -1;	
}
