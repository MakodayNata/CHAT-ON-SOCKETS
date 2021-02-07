#include "socet.h" 
void BinLis(int fd, struct sockaddr_in server);

void AcSendRes (int fd, struct sockaddr_in server);
int server() 
{ 
	WSADATA wsdata; 
	WSAStartup(0x0101,&wsdata); // функція для підключення бібліотеки wsock32.dll
	int fd, fd2; // дeскриптори сокета
	struct sockaddr_in server;// оголошення структур для заповнення параметрів для сокета (інформація для сервера)
	if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ) { /*ф-ція створення сокета int socket (domain, type, protocol). 
		domain задає набір протоколів, що використовуються для звязку- для стека протоколов TCP/IP - AF_INET.
		type задає режим взаємодії - SOCK_STREAM - з встановленням зєднання 
		protocol задає конкретний протокол транспортного рівня (при 0 - використовується протокоз "за замовчуванням" TCP для SOCK_STREAM */
		printf(" socket() error\n"); 
		exit(-1); 
	} 
	printf("server socket +\n");
	// заповнюємо параметри для сервера:
	server.sin_family = AF_INET; // поле визначає формат адреси (набір протоколів), що використовується
	server.sin_port = htons(PORT);// поле містись номер порта вузла звязку
	server.sin_addr.s_addr = INADDR_ANY; // поле містить адресу вузла звязку, INADDR_ANY - системний визов буде привязувати до сокета номер (адресу) локального вузла звязк
	BinLis (fd, server);
	AcSendRes (fd, server);	
	closesocket (fd2); // закриття дискриптора сокета
	WSACleanup(); 
	return -1; 
}

void BinLis( int fd,struct sockaddr_in server)
{
	if (bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1) { /* ф-ція привязує до сокета ІР адресу і порт
		int bind (fd, addr, addrlen)
		fd- дискрипрот сокета
		addr - вказує на структуру даних, що містить локальну адресу( для TCP/IP - sockaddr_in
		addrlen - задає розмір (в байтах) структури доних, що вказується аргументом addr */
		printf ("bind() error\n"); 
		exit (-1); 
	} 
	printf ("server bind() +\n");
	if (listen (fd,BACKLOG) == -1) { 	/* ф-ція прослуховування сокетів для вхідних повідомлень 
		int listen (fd, n)
		fd- дискрипрот сокета
		n- максимальна довжина черги вхідних запитів на встановлення звязку*/
		printf ("listen() error\n"); 
		exit (-1); 
	} 
	printf ("server listen() +\n"); 	
}

void AcSendRes (int fd, struct sockaddr_in server)
{
	int fd2, numbytes; 
	char buf[MAXDATASIZE], key;
	struct sockaddr_in client; // інформація для клієнта
	int sin_size;
	sin_size=sizeof (struct sockaddr_in); //для визначення ІР адреси та порту віддаленого компютера
		if ((fd2 = accept(fd,(struct sockaddr *)&client,&sin_size)) == -1) { /*ф-ція приймає запит на підключення
			int accept (fd, addr, p_addrlen)
			fd - дискриптор сокета через який програмам отримала запит на зєднання
			addr - вказує на область памяті, розмір якої дозволяє розмістити в ній структуру даних, що містить адресу сокету програми клієнта, який зробив запит на зєднання
			p_addrlen - вказує на область памяті у вигляді цілого числа, що задає розмір (у байтах) області памяті, який вказано аргументом */
			printf ("accept() error\n"); 
			exit (-1); 
		} 
		printf ("accept() +\n"); 
		printf ("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); // показує ІР адресу клієнта
		send (fd2,"Ласково просимо до нашого серверу.\n",MAXDATASIZE,0); /* ф-ція надсилання даних до дескриптора сокета 
		int send (fd2, buf, len, flags)
		int fd2 - дескрипрот сокета, через який відправляються дані
		len - довжина (в байтах) даних, що передаються
		char *buf - вказує на область памяті, де зберігаються дані, що передаються
		int flags - модифікує виконання системного виклику send,  при 0 - аналогічний ф-ції write*/
		do{
			printf("\n Отримати повідомлення від клієнта? (Esc- ні)\n");
			key = getch();
			if ((numbytes=recv(fd2,buf,MAXDATASIZE,0)) == -1) {/* ф-ція ортимання даних з дескрипрота сокета
				int recv (fd2, buf, len, flags)*/
	 			printf("recv() error\n"); 
			} 
			buf[numbytes] = '\0'; 
			printf("Client Message: %s\n",buf);
			printf ( "\n   Відповідь клієнту:\n" );
			gets(buf);
			send (fd2,buf,MAXDATASIZE,0); 	
			printf("\nПродовжити розмову? (Esc- ні)\n");
			key = getch();		
		}while (key!=27);
			
}

