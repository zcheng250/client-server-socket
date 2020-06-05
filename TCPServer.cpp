/* TCPServer.cpp - main */

#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include<process.h>
#include <windows.h>
#include<math.h>
#include <time.h>
#include "conio.h"

#define QLEN	   5
#define	WSVERS	MAKEWORD(2, 0)

#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library
/*------------------------------------------------------------------------
 * main - Iterative TCP server for TIME service
 *------------------------------------------------------------------------
 */

struct can{
	SOCKET sj;
	char ips[30]; 
	char por[20];
};
//CRITICAL_SECTION cs; // 临界区
SOCKET	msock, ssock[5];
int k=0;//用于标记ssock的个数    
unsigned __stdcall rec_ll(void* s){
	    //EnterCriticalSection(&cs);
		struct can* t=(struct can* )s;
		struct sockaddr_in from;
		int fromsize=sizeof(from);
		char* ipsb="客户端地址：";
		char* newline="\n";
		char* porb="客户端端口号："; 
		char* en=" enter\n";
		char* iple="leave";
		char buff[200],bufl[200];
		strcpy(buff,ipsb);
		strcat(buff,t->ips );
		strcat(buff,newline);
		strcat(buff,porb);
		strcat(buff,t->por );
		strcat(buff,newline);
		strcpy(bufl,buff);
		strcat(buff,en);      //enter
		strcat(bufl,iple);    //leave
		printf("%s\n",buff);
		int j=0;
		for(j=0;j<=k;j++){
			sendto(ssock[j],buff,strlen(buff),0,(SOCKADDR* )&from,fromsize);
		}
		
		
		while(1){
		
			char buf[200];
			char bufz[5];
			int l1=recvfrom(t->sj,buf,200,0,(SOCKADDR* )&from,&fromsize);
			buf[l1]='\0';
			
			for(int h=0;h<5;h++){
				bufz[h]=buf[h];
			}
			bufz[4]='\0';
			if(strcmp(bufz,"exit")==0){
				int jjk=0;
				printf("%s\n",bufl);
				for(jjk=0;jjk<=k;jjk++){
					sendto(ssock[jjk],bufl,strlen(bufl),0,(SOCKADDR* )&from,fromsize);
				}
				break;
			}
			
			time_t now;
			time(&now);
			char* ttb="时间："; 
			char* tt=ctime(&now);
			strcat(buf,"\n");
			strcat(buf,ttb);
			strcat(buf,tt);
			
			strcat(buf,ipsb);
			strcat(buf,t->ips );
			strcat(buf,"\n");
			strcat(buf,porb);
			strcat(buf,t->por );
			strcat(buf,"\n");
			
			printf("%s",buf); 
			int i;
			for(i=0;i<=k;i++){
				int j=sendto(ssock[i],buf,strlen(buf),0,(SOCKADDR* )&from,fromsize);
				if(j>0) printf("客户%d\n",i);
			   }
		}
		//close(ss->sj );
		free(t);
		//LeaveCriticalSection(&cs); 
};
int main(int argc, char *argv[]) 
/* argc: 命令行参数个数， 例如：C:\> TCPServer 8080 
                     argc=2 argv[0]="TCPServer",argv[1]="8080" */
{   
    //InitializeCriticalSection(&cs); // 临界区初始化
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	
	WSADATA wsadata; 
	char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
    int	    alen;			        /* from-address length		        */
	char	*pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */

	
	WSAStartup(WSVERS, &wsadata);						// 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														// 返回：要监听套接字的描述符或INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));						// 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							// 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// 监听所有(接口的)IP地址。
	sin.sin_port = htons((u_short)atoi(service));		// 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(16位)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // 绑定监听的IP地址和端口号
	//sin_port和sin_addr都必须是网络字节序（NBO），一般可视化的数字都是主机字节序（HBO）。
	listen(msock, 5); // 等待建立连接的队列长度为5

	int ptl;
	
	HANDLE  hThread[5];
    while(!_kbhit()){ 	// 检测是否有按键
		//ptl=k;
	   struct can* kk=(struct can* )malloc(sizeof(struct can));
       alen = sizeof(struct sockaddr);                   // 取到地址结构的长度
	   ssock[k]= accept(msock, (struct sockaddr *)&fsin, &alen); // 如果有新的连接请求，返回连接套接字，否则，被阻塞。fsin包含客户端IP地址和端口号
	   char *ipp=inet_ntoa(fsin.sin_addr );
	   strcpy(kk->ips ,ipp);
	   u_short pp=fsin.sin_port ;
	   itoa(pp,kk->por ,10);
	   kk->sj =ssock[k];
	   hThread[k]= (HANDLE)_beginthreadex(NULL,0,&rec_ll ,(void *)kk, 0, NULL);
	   
	   
       
	   k++;
    }
     
    //DeleteCriticalSection(&cs);
	int i;
	for(i=0;i<k;i++){
		CloseHandle(hThread[i]);
	}
	for(i=0;i<k;i++){
		closesocket(ssock[k]);
	}
    (void) closesocket(msock);                                 // 关闭监听套接字
     WSACleanup();                                             // 卸载winsock library

}

