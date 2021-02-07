#include "socet.h" 
void ConSendRes(int fd, struct sockaddr_in server);

int client(int argc, char *argv[]) 
{ 
	WSADATA wsdata; 
	WSAStartup(0x0101,&wsdata); 
	int fd;
	struct hostent *he; 
	struct sockaddr_in server; 
	printf("client +\n");
	// �-��� �� ������ ��� ����� ������ � ���� ������
	if ((he=gethostbyname(argv[1])) == NULL) {
		printf("gethostbyname() error\n"); 
		exit(-1); 
	}
	printf(" gethostbyname()  +\n");
	if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){/*�-��� ��������� ������:int socket (domain, type, protocol). 
 		domain ���� ���� ���������, �� ���������������� ��� ������- ��� ����� ���������� TCP/IP - AF_INET.
 		type ���� ����� �����䳿 - SOCK_STREAM - � ������������� ������� 
 		protocol ���� ���������� �������� ������������� ���� (��� 0 - ��������������� �������� "�� �������������" TCP ��� SOCK_STREAM */
		printf("socket() error\n"); 
		exit(-1); 
	} 
	printf("client socket  +\n"); 
	server.sin_family = AF_INET; // ���� ������� ������ ������ (���� ���������), �� ���������������
	server.sin_port = htons(PORT); // ���� ������ ����� ����� ����� ������
	server.sin_addr = *((struct in_addr *)he->h_addr); // ���� ������ ������ ����� ������
	ConSendRes (fd, server);	
	closesocket(fd); // �������� ������
	WSACleanup();
 	return -1; 
}

void ConSendRes(int fd, struct sockaddr_in server)
{
	int  numbytes;
	char buf[MAXDATASIZE], key; 
	if(connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr))==-1) {/* �-��� ���������� ���������� �� ���������� �����
	int connect (fd, addr, addrlen):
	fd- ���������� ������
	addr - ����� �� ��������� �����, �� ������ ������ ���������� ������ �������� �������, �� ��� ������ ����� �� �������
	addrlen - ���� ����� (� ������)��������� �����, �� ������� ���������� addr*/ 
 		printf("connect() error\n"); 
		exit(-1); 
	} 
	printf("client conect  +\n"); 
	do{
		if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1) {  // �-��� ��������� ����� � ����������� ������
			printf("recv() error\n"); 
			exit(-1); 
		} 
		buf[numbytes] = '\0'; // ������ ������ ���� �����
		printf("Server  Message: %s\n",buf);
		printf ( "\n   ³������ �������:\n" );
		gets(buf);
		send (fd,buf,MAXDATASIZE,0); // �-��� ���������� ����� � ����������� ������	
		printf("\n���������� �������? (Esc- �)\n");
		key = getch();		
	}while (key!=27);	
}


