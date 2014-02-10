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

	if(listen_socket(server) == -1){
		print("Error listening to bound socket. Server will close.");
		stop_winsock();
		return FALSE;
	}else
		print("Listening OK");		
	
	print("Waiting for client connection...");
	printf("Incoming client will receive the server message %s\n", msg);
	SOCKET client;
	char buf[100];
	while(1){
		if(accept_client(server, sin, client) == -1){
			print("Encountered a problem while accepting a connection. Server will close.");
			stop_winsock();
			return FALSE;
		}else
			print("Incoming client connection accepted");
			
		while(1){
			//read from remote server
			int result = read_socket(buf, client);
			if(result == -1)
				print("An error occured while reading from remote client.");
				break;
			else
				printf("Client Message: %s (%d bytes)\n", buf, sizeof buf);
		}
			
		//sample data. strcpy :)
		char send_buffer[100];
		strcpy(send_buffer, msg);
		
		//no error handling when sending data to client for now
		int bytes_sent;
		send_client(client, send_buffer, bytes_sent);
		printf("Sent %d bytes of data\n", bytes_sent);
		
		if(close_socket(client))
			print("Closed connection to client");
	}
	
	stop_winsock();
	print("Winsock stopped");

	return 0;
};