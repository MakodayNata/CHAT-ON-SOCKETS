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
	// ф-ція що передає імя вузла звязку в його адресу
	if ((he=gethostbyname(argv[1])) == NULL) {
		printf("gethostbyname() error\n"); 
		exit(-1); 
	}
	printf(" gethostbyname()  +\n");
	if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){/*ф-ція створення сокета:int socket (domain, type, protocol). 
 		domain задає набір протоколів, що використовуються для звязку- для стека протоколов TCP/IP - AF_INET.
 		type задає режим взаємодії - SOCK_STREAM - з встановленням зєднання 
 		protocol задає конкретний протокол транспортного рівня (при 0 - використовується протокоз "за замовчуванням" TCP для SOCK_STREAM */
		printf("socket() error\n"); 
		exit(-1); 
	} 
	printf("client socket  +\n"); 
	server.sin_family = AF_INET; // поле визначає формат адреси (набір протоколів), що використовується
	server.sin_port = htons(PORT); // поле містись номер порта вузла звязку
	server.sin_addr = *((struct in_addr *)he->h_addr); // поле містить адресу вузла звязку
	ConSendRes (fd, server);	
	closesocket(fd); // закриття сокету
	WSACleanup();
 	return -1; 
}

void ConSendRes(int fd, struct sockaddr_in server)
{
	int  numbytes;
	char buf[MAXDATASIZE], key; 
	if(connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr))==-1) {/* ф-ція ініціювання підключення до віддаленого хосту
	int connect (fd, addr, addrlen):
	fd- дескрипрот сокета
	addr - вказує на структуру даних, що містить адресу приписаний сокету програми сервера, до якої дається запит на зєднання
	addrlen - задає розмір (у байтах)структури даних, що вказана аргументом addr*/ 
 		printf("connect() error\n"); 
		exit(-1); 
	} 
	printf("client conect  +\n"); 
	do{
		if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1) {  // ф-ція отримання даних з дескриптора сокета
			printf("recv() error\n"); 
			exit(-1); 
		} 
		buf[numbytes] = '\0'; // додаємо символ кінця рядка
		printf("Server  Message: %s\n",buf);
		printf ( "\n   Відповідь серверу:\n" );
		gets(buf);
		send (fd,buf,MAXDATASIZE,0); // ф-ція надсилання даних з дескриптора сокета	
		printf("\nПродовжити розмову? (Esc- ні)\n");
		key = getch();		
	}while (key!=27);	
}


