#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024 // buf size
#define LISTENQ 1 // the max quote

int main(int argc, char *argv[]) {
	struct sockaddr_in serv_addr;
	char recv_buf[BUFFER_SIZE];
	int sock_fd, conn_fd, ret, i, port, optval = 1;
	
	// check the arguments
	if (argc != 2) {
		printf("please check your input.\n");
		printf("such as ./tcp_client portnumber");
		return -1;
	}

	// create socket
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		return -1;

	// setting bind port crycled
	if ((setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, 
				sizeof(int))) < 0) {
		perror("setsockopt");
		goto err;
	}

	// initized address
	port = atoi(argv[1]);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// bind the port
	if ((bind(sock_fd, (struct sockaddr *)&serv_addr, 
		sizeof(struct sockaddr_in))) < 0) {
		perror("bind");
		goto err;
	}
	
	// transform the socket into listen socket
	if ((listen(sock_fd, LISTENQ)) < 0) {
		perror("listen");
		goto err;
	}

	socklen_t cli_len = sizeof(struct sockaddr_in);
	while (1) {
		// accept the asking
		if ((conn_fd = accept(sock_fd, (struct sockaddr *)&serv_addr, 
					&cli_len))< 0) 
			return -1;

		// accept the data
		if ((ret = recv(conn_fd, recv_buf, BUFFER_SIZE, 0)) < 0)
			return -1;

		printf("message from tcp_client: %s", recv_buf);
		printf("\n");
		
		// transform the data
		for (i = 0; i < ret; i++) {
			if (recv_buf[i] >= 'a' && recv_buf[i] <= 'z') {
				recv_buf[i] = recv_buf[i] - 32;
			}
		}
		
		// send data
		if ((send(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0)
			return -1;

		close(conn_fd);
	}

	close(sock_fd);

	return 0;

err:	
	close(sock_fd);
	
	return -1;
}	
