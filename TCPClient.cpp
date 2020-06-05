/* TCPClient.cpp */

#include <stdlib.h>
#include <stdio.h>
#include<process.h>
#include <winsock2.h>
#include<windows.h>
#include<math.h>
#include<time.h>

#include <string.h>

#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary


SOCKET	sock;		  	            /* socket descriptor	    	*/
unsigned __stdcall rec_kk(void* pt){  //__stdcall是说明函数参数是从右向左通过堆栈传递的
	int ss=*((int* )pt);
	struct sockaddr_in from;
	int fromsize=sizeof(from);
	while(1){
	
	char buf[200];
    int l=recvfrom(sock,buf,200,0,(SOCKADDR* )&from,&fromsize);
	if(l==0) printf("没有接收\n");
	else{
		buf[l]='\0';
	printf("收到消息：%s\n",buf);
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
	WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														  //返回：要监听套接字的描述符或INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							  // 因特网地址簇
    sin.sin_addr.s_addr = inet_addr(host);                // 服务器IP地址(32位)
    sin.sin_port = htons((u_short)atoi(service));         // 服务器端口号  
    connect(sock, (struct sockaddr *)&sin, sizeof(sin));// 连接到服务器
    int p1=1;
    HANDLE hThread1 = (HANDLE)_beginthreadex(NULL, 0,&rec_kk, (void *)&p1, 0, NULL);//创建子线程 
	
	while(1){
	 
		char mess[200];
		scanf("%s",mess);
		
		int jj=sendto(sock,mess,strlen(mess),0,(SOCKADDR* )&sin,sinsize);
		if(jj>0) printf("发送消息：%s\n",mess);
		
	}
	
    getchar();
    getchar();
    getchar();
    getchar();
    
	CloseHandle(hThread1);
    closesocket(sock);                             // 关闭监听套接字
    WSACleanup();                                  // 卸载winsock library

    printf("按回车键继续...");
	getchar();										// 等待任意按键
}
