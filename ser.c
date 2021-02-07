#include "socet.h" 
void BinLis(int fd, struct sockaddr_in server);

void AcSendRes (int fd, struct sockaddr_in server);
int server() 
{ 
	WSADATA wsdata; 
	WSAStartup(0x0101,&wsdata); // ������� ��� ���������� �������� wsock32.dll
	int fd, fd2; // �e��������� ������
	struct sockaddr_in server;// ���������� �������� ��� ���������� ��������� ��� ������ (���������� ��� �������)
	if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) { /*�-��� ��������� ������ int socket (domain, type, protocol). 
		domain ���� ���� ���������, �� ���������������� ��� ������- ��� ����� ���������� TCP/IP - AF_INET.
		type ���� ����� �����䳿 - SOCK_STREAM - � ������������� ������� 
		protocol ���� ���������� �������� ������������� ���� (��� 0 - ��������������� �������� "�� �������������" TCP ��� SOCK_STREAM */
		printf(" socket() error\n"); 
		exit(-1); 
	} 
	printf("server socket +\n");
	// ���������� ��������� ��� �������:
	server.sin_family = AF_INET; // ���� ������� ������ ������ (���� ���������), �� ���������������
	server.sin_port = htons(PORT);// ���� ������ ����� ����� ����� ������
	server.sin_addr.s_addr = INADDR_ANY; // ���� ������ ������ ����� ������, INADDR_ANY - ��������� ����� ���� ����������� �� ������ ����� (������) ���������� ����� �����
	BinLis (fd, server);
	AcSendRes (fd, server);	
	closesocket (fd2); // �������� ����������� ������
	WSACleanup(); 
	return -1; 
}

void BinLis( int fd,struct sockaddr_in server)
{
	if (bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1) { /* �-��� ������� �� ������ �� ������ � ����
		int bind (fd, addr, addrlen)
		fd- ���������� ������
		addr - ����� �� ��������� �����, �� ������ �������� ������( ��� TCP/IP - sockaddr_in
		addrlen - ���� ����� (� ������) ��������� �����, �� ��������� ���������� addr */
		printf ("bind() error\n"); 
		exit (-1); 
	} 
	printf ("server bind() +\n");
	if (listen (fd,BACKLOG) == -1) { 	/* �-��� ��������������� ������ ��� ������� ���������� 
		int listen (fd, n)
		fd- ���������� ������
		n- ����������� ������� ����� ������� ������ �� ������������ ������*/
		printf ("listen() error\n"); 
		exit (-1); 
	} 
	printf ("server listen() +\n"); 	
}

void AcSendRes (int fd, struct sockaddr_in server)
{
	int fd2, numbytes; 
	char buf[MAXDATASIZE], key;
	struct sockaddr_in client; // ���������� ��� �볺���
	int sin_size;
	sin_size=sizeof (struct sockaddr_in); //��� ���������� �� ������ �� ����� ���������� ���������
		if ((fd2 = accept(fd,(struct sockaddr *)&client,&sin_size)) == -1) { /*�-��� ������ ����� �� ����������
			int accept (fd, addr, p_addrlen)
			fd - ���������� ������ ����� ���� ��������� �������� ����� �� �������
			addr - ����� �� ������� �����, ����� ��� �������� ��������� � �� ��������� �����, �� ������ ������ ������ �������� �볺���, ���� ������ ����� �� �������
			p_addrlen - ����� �� ������� ����� � ������ ������ �����, �� ���� ����� (� ������) ������ �����, ���� ������� ���������� */
			printf ("accept() error\n"); 
			exit (-1); 
		} 
		printf ("accept() +\n"); 
		printf ("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); // ������ �� ������ �볺���
		send (fd2,"������� ������� �� ������ �������.\n",MAXDATASIZE,0); /* �-��� ���������� ����� �� ����������� ������ 
		int send (fd2, buf, len, flags)
		int fd2 - ���������� ������, ����� ���� ������������� ���
		len - ������� (� ������) �����, �� �����������
		char *buf - ����� �� ������� �����, �� ����������� ���, �� �����������
		int flags - �������� ��������� ���������� ������� send,  ��� 0 - ���������� �-��� write*/
		do{
			printf("\n �������� ����������� �� �볺���? (Esc- �)\n");
			key = getch();
			if ((numbytes=recv(fd2,buf,MAXDATASIZE,0)) == -1) {/* �-��� ��������� ����� � ����������� ������
				int recv (fd2, buf, len, flags)*/
	 			printf("recv() error\n"); 
			} 
			buf[numbytes] = '\0'; 
			printf("Client Message: %s\n",buf);
			printf ( "\n   ³������ �볺���:\n" );
			gets(buf);
			send (fd2,buf,MAXDATASIZE,0); 	
			printf("\n���������� �������? (Esc- �)\n");
			key = getch();		
		}while (key!=27);
			
}

