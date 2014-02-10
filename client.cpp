#include "common.h"

int main(){
	set_debug_tag("Client");
	
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
	
	if(connect_server(server, sin) == -1){
		print("Problem connecting to server. Program will close.");
		stop_winsock();
		return FALSE;
	}else
		print("Connected to host server.");
		
	char line[256];
	int i;
	if (fgets(line, sizeof(line), stdin)) {
		if (1 == sscanf(line, "%d", &i)) {
			int bytes_sent;
			send_socket(server, line, bytes_sent);
		}
	}
	
	//read from remote server
	char buf[100];
	int result = read_server(buf, server);
	if(result == -1)
		print("An error occured while reading from remote server.");
	else
		printf("Server Message: %s (%d bytes)\n", buf, sizeof buf);
	
	if(close_socket(server))
		print("Closed connection to server");
		
	stop_winsock();
	print("Winsock stopped");
	
};