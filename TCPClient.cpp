/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include<process.h>
#include <winsock2.h>
#include<windows.h>
#include<math.h>
#include<time.h>

#include <string.h>

#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary


SOCKET	sock;		  	            /* socket descriptor	    	*/
unsigned __stdcall rec_kk(void* pt){  //__stdcall��˵�����������Ǵ�������ͨ����ջ���ݵ�
	int ss=*((int* )pt);
	struct sockaddr_in from;
	int fromsize=sizeof(from);
	while(1){
	
	char buf[200];
    int l=recvfrom(sock,buf,200,0,(SOCKADDR* )&from,&fromsize);
	if(l==0) printf("û�н���\n");
	else{
		buf[l]='\0';
	printf("�յ���Ϣ��%s\n",buf);
	}
	}
}

int main(int argc, char *argv[])
{
	char	*host = "192.168.1.3";	 //172.18.147.199   /* server IP to connect         */
	char	*service = "50500";  	    /* server port to connect       */
	struct sockaddr_in sin;
	int sinsize=sizeof(sin);	            /* an Internet endpoint address	*/
	char	buf[BUFLEN+1];   		        /* buffer for one line of text	*/
	
	int	cc;			                    /* recv character count		    */

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														  //���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
    sin.sin_family = AF_INET;							  // ��������ַ��
    sin.sin_addr.s_addr = inet_addr(host);                // ������IP��ַ(32λ)
    sin.sin_port = htons((u_short)atoi(service));         // �������˿ں�  
    connect(sock, (struct sockaddr *)&sin, sizeof(sin));// ���ӵ�������
    int p1=1;
    HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0,&rec_kk, (void *)&p1, 0, NULL);//�������߳� 
	
	while(1){
	 
		char mess[200];
		scanf("%s",mess);
		
		int jj=sendto(sock,mess,strlen(mess),0,(SOCKADDR* )&sin,sinsize);
		if(jj>0) printf("������Ϣ��%s\n",mess);
		
	}
	
    getchar();
    getchar();
    getchar();
    getchar();
    
	CloseHandle(hThread1);
    closesocket(sock);                             // �رռ����׽���
    WSACleanup();                                  // ж��winsock library

    printf("���س�������...");
	getchar();										// �ȴ����ⰴ��
}
