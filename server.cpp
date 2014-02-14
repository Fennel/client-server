#include "common.h"

int main(int argc, char *args[]){
	//server message is stored in index 1
	const char *msg;
	printf("argc=%d", argc);
	if(argc > 1)
		msg = args[1];
	else
		msg = "Welcome to Echo Server!";
	
	set_debug_tag("Server");
	
	//initialize winsock
	if(start_winsock() == -1){
		print("Problem starting winsock. Client will terminate.");
		stop_winsock();
		return FALSE;
	}else
		print("Winsock initialized");
	
	//make socket
	SOCKET server;
	create_socket(server);
	print("Socket created.");
	
	//configure address
	sockaddr_in sin;
	configure_address(sin);
	
	if(bind_socket(server, sin) == -1){
		print("Problem binding socket. Server will close.");
		stop_winsock();
		return FALSE;
	}else
		print("Successfully bound server socket.");

	SOCKET client;
	char buf[100];
	SOCKET *sockets[100];
	int is_recycleable[100];
	while(1){
		if(listen_socket(server) == 1){
			//dequeue connection
			print("A waiting client exists.");
			if(accept_client(server, sin, client) == -1){
				print("Encountered a problem while trying to establish connection. Server will close.");
				stop_winsock();
				return FALSE;
			}else{
				sock_addr2string(sin, buf);
				printf("Established connection to client from %s\n", buf);
				int result = add_socket(client, sockets, is_recycleable);
				int bytes_sent;
				if(result == -1){
					printf("Server capacity full. Connection to this client will close.", buf);
					send_socket(client, (char *)"Maximum slot capacity reached. Connection will close.", bytes_sent);
					close_socket(client);
				}
			}
		}else{
			print("Encountered a problem while listening for connections. Server will close.");
			stop_winsock();
			return FALSE;
		}
		
		//loop to list of connected clients
		//read
		//is success echo
		//otherwise close that client and mark it as recycleable
		
		printf("Incoming client will receive the server message %s\n", msg);
		
		
		while(1){

			print("Waiting for client message.");
			while(1){
				//read from remote client
				int result = read_socket(buf, client);
				if(result == -1){
					print("An error occured while reading from remote client.");
					break;
				}else
					printf("Client Message: %s (%d bytes)\n", buf, sizeof buf);
				
				//echo it back
				//no error handling when sending data to client for now
				int bytes_sent;
				send_socket(client, buf, bytes_sent);
				printf("Sent %d bytes of data\n", bytes_sent);
			}
			
			if(close_socket(client))
				print("Closed connection to client");
		}
		
		break;
	}
	
	stop_winsock();
	print("Winsock stopped");

	return 0;
};