#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>

const char *tag;
void set_debug_tag(const char *tag){
	::tag = tag;
}
void print(const char* msg){
	printf("%s: %s\n", tag, msg);
}
void add_socket(SOCKET &sock, SOCKET sockets[], int is_recycleable){
	int id = 0;
	for(id = 1; id < 100; id++){
		if(!is_recycleable[id]){	//free slot
			is_recycleable[id] = 1;
			sock[id] = &sock;
			return 1;
		}
	}
	return -1;	//out of slot, max slots used, inform client, and close connection
}
int sock_addr2string(sockaddr_in &sin, char str[]){
	int size = snprintf(str, 100, "%d.%d.%d.%d",  int(sin.sin_addr.s_addr&0xFF), int((sin.sin_addr.s_addr&0xFF00)>>8), int((sin.sin_addr.s_addr&0xFF0000)>>16),int((sin.sin_addr.s_addr&0xFF000000)>>24));
	//printf("size=%d", size);
	str[size] = '\0';
	return size;
}
int start_winsock(){
	//startup winsock
	print("starting winsock..");
	
	WSADATA wsaData;
	WORD version;
	int error;

	//initialize version as word
	version = MAKEWORD( 2, 0 );
	
	//startup winsock
	error = WSAStartup(version, &wsaData);

	//error occurs
	if (error != 0)
		return -1;

	//wrong version
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
		return -1;
	return 1;
}
void stop_winsock(){
	WSACleanup();
}
void press_any_key(){
	print("Press any key to continue");
    _getch();
}
int read_socket(char recv_buf[], SOCKET &server){
	int recv_code = SOCKET_ERROR;
	
	while(recv_code == SOCKET_ERROR){
		recv_code = recv(server, recv_buf, 100, 0);
		if (recv_code == 0 || recv_code == WSAECONNRESET)
			return -1;
		if (recv_code < 1)
			return -1;
	}
	recv_buf[recv_code] = '\0';
	return 1;
}
int close_socket(SOCKET &client){
	closesocket(client);
	return 1;
}

int configure_address(sockaddr_in &sin){
	memset(&sin, 0, sizeof(sin));
	
	sin.sin_family = AF_INET;
	//sin.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_port = htons(10000);
	return 1;
}

void create_socket(SOCKET &server){
	server = socket(AF_INET, SOCK_STREAM, 0);
}

int connect_server(SOCKET &server, sockaddr_in &sin){
	if(connect(server, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR)
		return -1;
	return 1;
}

int bind_socket(SOCKET &server, sockaddr_in &sin){
	if (bind(server, (const sockaddr *)&sin, sizeof sin) == SOCKET_ERROR )
		return -1;
	return 1;
}

int listen_socket(SOCKET &server){
	int code;
	code = listen(server, SOMAXCONN);
	printf("code=%d\n", code);
	if(code == SOCKET_ERROR )
		return -1;
	if(code < 1)
		return -1;
	return 1;
}

int accept_client(SOCKET &server, sockaddr_in &sin, SOCKET &client){
	//accept must take a POINTER to an int. in other words can't pass sizeof sin directly
	int length = sizeof sin;
	client = accept(server, (sockaddr *)&sin, &length);
	if(client == SOCKET_ERROR)
		return -1;
	return 1;
}
int send_socket(SOCKET &client, char send_buf[], int &code){
	code = send(client, send_buf, strlen(send_buf), 0);
	return 1;
}


	
	//find host
	//no use unless using DNS
	/*
	hostent *host;
	host = gethostbyname( "async5-5.remote.ualberta.ca" );
	print("Host obtained.");
	print("Connecting to host...");
	*/