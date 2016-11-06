#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024 // buf size

int main(int argc, char *argv[]) {
	int conn_fd, sock_fd, port, ret = 0;
	char buf[BUFFER_SIZE];
	struct sockaddr_in serv_addr;
	
	// check the arguments
	if (argc != 2) {
		printf("please check your input.\n");
		printf("such as ./tcp_client [portnumber]\n");
		return -1;
	}

	// initialize address
	port = atoi(argv[1]);
	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// create socket
	if ((conn_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;
	
	// connect
	if ((sock_fd = connect(conn_fd, (struct sockaddr *)&serv_addr,
				sizeof(struct sockaddr))) < 0) {
		perror("connect");
		goto err;
	}

	bzero(buf, sizeof(buf));
	
	// keyboard input
	if ((read(STDIN_FILENO, buf, sizeof(buf))) < 0)
		goto err;
	
	// send data
	if ((send(conn_fd, buf, sizeof(buf), 0)) < 0)
		goto err;
	
	// accept data
	memset(buf, 0, sizeof(buf));
	if ((ret = recv(conn_fd, buf, BUFFER_SIZE, 0)) < 0)
		goto err;

	printf("the Change message from tcp_server:%s", buf);
	printf("\n");
	
	close(conn_fd);
	close(sock_fd);

	return 0;

err:
	close(conn_fd);
	close(sock_fd);
	
	return -1;	
}
