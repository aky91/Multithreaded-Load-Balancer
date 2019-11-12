/* Akash Yadav
 * 20164094
*/

#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#define BACKLOG 10
#define SERVER_NAME_LEN_MAX 255

#define SERVERPORT 6000
#define SERVER1PORT 6001
#define SERVER2PORT 6002

int clientNumber = 0;

typedef struct pthread_arg_t {
	int new_socket_fd;
	struct sockaddr_in client_address;
	/* TODO: Put arguments passed to threads here. See lines 116 and 139. */
} pthread_arg_t;

/* Thread routine to serve connection to client. */
void *pthread_routine(void *arg);

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

int main(int argc, char *argv[]) {
	int port = SERVERPORT, socket_fd, new_socket_fd;
	struct sockaddr_in address;
	pthread_attr_t pthread_attr;
	pthread_arg_t *pthread_arg;
	pthread_t pthread;
	socklen_t client_address_len;

	/*uncomment the following line if you wish to change the server port*/
	/* Get port from command line arguments or stdin. */
	// port = argc > 1 ? atoi(argv[1]) : 0;
	// if (!port) {
	// 	printf("Enter Port: ");
	// 	scanf("%d", &port);
	// }

	/* Initialise IPv4 address. */
	memset(&address, 0, sizeof address);
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	/* Create TCP socket. */
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* Bind address to socket. */
	if (bind(socket_fd, (struct sockaddr *)&address, sizeof address) == -1) {
		perror("bind");
		exit(1);
	}

	/* Listen on socket. */
	if (listen(socket_fd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	/* Assign signal handlers to signals. */
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		perror("signal");
		exit(1);
	}
	if (signal(SIGTERM, signal_handler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}
	if (signal(SIGINT, signal_handler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	/* Initialise pthread attribute to create detached threads. */
	if (pthread_attr_init(&pthread_attr) != 0) {
		perror("pthread_attr_init");
		exit(1);
	}
	if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
		perror("pthread_attr_setdetachstate");
		exit(1);
	}

	while (1) {
		/* Create pthread argument for each connection to client. */
		/* TODO: malloc'ing before accepting a connection causes only one small
		 * memory when the program exits. It can be safely ignored.
		 */
		pthread_arg = (pthread_arg_t *)malloc(sizeof *pthread_arg);
		if (!pthread_arg) {
			perror("malloc");
			continue;
		}

		/* Accept connection to client. */
		client_address_len = sizeof pthread_arg->client_address;
		new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
		if (new_socket_fd == -1) {
			perror("accept");
			free(pthread_arg);
			continue;
		}

		/* Initialise pthread argument. */
		pthread_arg->new_socket_fd = new_socket_fd;
		/* TODO: Initialise arguments passed to threads here. See lines 22 and
		 * 139.
		 */

		/* Create thread to serve connection to client. */
		if (pthread_create(&pthread, &pthread_attr, pthread_routine, (void *)pthread_arg) != 0) {
			perror("pthread_create");
			free(pthread_arg);
			continue;
		}
	}

	/* close(socket_fd);
	 * TODO: If you really want to close the socket, you would do it in
	 * signal_handler(), meaning socket_fd would need to be a global variable.
	 */
	return 0;
}

/*function to return load of server# idx */
int getLoadServer(int idx){
	
	/*initialize connection*/
	char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1\0";
	int server_port = 0, socket_fd;
	if(idx == 1) server_port = SERVER1PORT;
	else if(idx == 2) server_port = SERVER2PORT;
	struct hostent *server_host;
	struct sockaddr_in server_address;
	server_host = gethostbyname("localhost");
	memset(&server_address, 0, sizeof server_address);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

	/*make connection*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address);

	/*request load*/
	write(socket_fd, "__clients?", 100);

	/*get reply*/
	char reply[100];
	read(socket_fd, reply, 100);

	/*close connection*/
	close(socket_fd);

	/*convert string to integer*/
	int load = 0;
	sscanf(reply, "%d", &load);

	/*return load*/
	return load;
}

void sendquery(int idx, char input[], char reply[]){
	
	/*initialize connection*/
	char server_name[SERVER_NAME_LEN_MAX + 1] = "127.0.0.1\0";
	int server_port = 0, socket_fd;
	if(idx == 1) server_port = SERVER1PORT;
	else if(idx == 2) server_port = SERVER2PORT;
	struct hostent *server_host;
	struct sockaddr_in server_address;
	server_host = gethostbyname("localhost");
	memset(&server_address, 0, sizeof server_address);
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	memcpy(&server_address.sin_addr.s_addr, server_host->h_addr, server_host->h_length);

	/*make connection*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address);

	/*sent data to process*/
	write(socket_fd, input, 100);

	/*get reply from server*/
	read(socket_fd, reply, 100);
}	

void *pthread_routine(void *arg) {
	clientNumber++;
	int clientIdx = clientNumber;
	pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
	int new_socket_fd = pthread_arg->new_socket_fd;
	struct sockaddr_in client_address = pthread_arg->client_address;
	/* TODO: Get arguments passed to threads here. See lines 22 and 116. */

	free(arg);

	/* TODO: Put client interaction code here. For example, use
	 * write(new_socket_fd,,) and read(new_socket_fd,,) to send and receive
	 * messages with the client.
	 */

	printf("client#%d : connected\n", clientIdx);

	/*get input from actual client*/
	char input[100];
	read(new_socket_fd, input, 100);

	/*get load from server1*/    
	int load1 = getLoadServer(1);

	/*get load from server2 */ 
	int load2 = getLoadServer(2);

	/*display load*/
	printf("LOAD:- SERVER1: %d  SERVER2: %d\n", load1, load2);

	/*send query to the server with less load*/
	char reply[100];
	if(load1 <= load2) sendquery(1, input, reply);
	else sendquery(2, input, reply);
	
	/*send output to the client*/
	write(new_socket_fd, reply, 100);

	printf("client%d : disconnected\n", clientIdx);

	/*close connection*/
	close(new_socket_fd);

	return NULL;
}

void signal_handler(int signal_number) {
	/* TODO: Put exit cleanup code here. */
	exit(0);
}