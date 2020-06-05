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

#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library
/*------------------------------------------------------------------------
 * main - Iterative TCP server for TIME service
 *------------------------------------------------------------------------
 */

struct can{
	SOCKET sj;
	char ips[30]; 
	char por[20];
};
//CRITICAL_SECTION cs; // �ٽ���
SOCKET	msock, ssock[5];
int k=0;//���ڱ��ssock�ĸ���    
unsigned __stdcall rec_ll(void* s){
	    //EnterCriticalSection(&cs);
		struct can* t=(struct can* )s;
		struct sockaddr_in from;
		int fromsize=sizeof(from);
		char* ipsb="�ͻ��˵�ַ��";
		char* newline="\n";
		char* porb="�ͻ��˶˿ںţ�"; 
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
			char* ttb="ʱ�䣺"; 
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
				if(j>0) printf("�ͻ�%d\n",i);
			   }
		}
		//close(ss->sj );
		free(t);
		//LeaveCriticalSection(&cs); 
};
int main(int argc, char *argv[]) 
/* argc: �����в��������� ���磺C:\> TCPServer 8080 
                     argc=2 argv[0]="TCPServer",argv[1]="8080" */
{   
    //InitializeCriticalSection(&cs); // �ٽ�����ʼ��
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	
	WSADATA wsadata; 
	char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
    int	    alen;			        /* from-address length		        */
	char	*pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */

	
	WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														// ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET

	memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(16λ)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�
	//sin_port��sin_addr�������������ֽ���NBO����һ����ӻ������ֶ��������ֽ���HBO����
	listen(msock, 5); // �ȴ��������ӵĶ��г���Ϊ5

	int ptl;
	
	HANDLE  hThread[5];
    while(!_kbhit()){ 	// ����Ƿ��а���
		//ptl=k;
	   struct can* kk=(struct can* )malloc(sizeof(struct can));
       alen = sizeof(struct sockaddr);                   // ȡ����ַ�ṹ�ĳ���
	   ssock[k]= accept(msock, (struct sockaddr *)&fsin, &alen); // ������µ��������󣬷��������׽��֣����򣬱�������fsin�����ͻ���IP��ַ�Ͷ˿ں�
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
    (void) closesocket(msock);                                 // �رռ����׽���
     WSACleanup();                                             // ж��winsock library

}

