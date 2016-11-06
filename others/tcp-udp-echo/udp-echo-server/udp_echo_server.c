#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024 // buf size

int main(int argc, char *argv[]) {
	struct sockaddr_in serv_addr;
	char recv_buf[BUFFER_SIZE];
	int port, i, ret, sock_fd;
	socklen_t addr_len;
	
	// check your input
	if (argc != 2) {
		printf("please check your input.\n");
		printf("such as ./udp_server [portnumber]");
		return -1;
	}

	// create two-way transmission socket
	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		goto err;
	
	// initialize address
	port = atoi(argv[1]);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// bind the local port
	if ((bind(sock_fd, (struct sockaddr *)&serv_addr, 
				sizeof(struct sockaddr_in))) < 0) {
		perror("bind");
		goto err;
	}
	
	while (1) {
		// recvive data  
		addr_len = sizeof(struct sockaddr_in);
		if ((ret = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, 
					(struct sockaddr *)&serv_addr, &addr_len)) < 0)
			goto err;

		printf("message from tcp_client: %s\n", recv_buf);
		printf("\n");
		
		// transform data
		for (i = 0; i < ret; i++) {			
			if (recv_buf[i] >= 'a' && recv_buf[i] <= 'z') {
					recv_buf[i] = recv_buf[i] - 32;
				}
		}
		// send back data
		if ((sendto(sock_fd, recv_buf, ret, 0, (struct sockaddr *)
					&serv_addr, sizeof(struct sockaddr_in))) < 0)
			goto err;
	}

	close(sock_fd);

	return 0;

err:
	close(sock_fd);

	return -1;	
}
