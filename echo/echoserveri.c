#include "csapp.h"

void echo(int connfd);

int main(int argc, char **argv)
{
  int listenfd, connfd; //듣기 식별자, 연결 식별자
  socklen_t clientlen; //클라이언트 소켓 주소 길이
  struct sockaddr_storage clientaddr; //클라이언트 주소 정보 저장하는 구조체
  char client_hostname[MAXLINE], client_port[MAXLINE]; //클라이언트 호스트네임과 포트

  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }

  listenfd = Open_listenfd(argv[1]); //듣기 소켓을 열어 듣기 식별자를 반환한다.
  while (1)
  {
    clientlen = sizeof(struct sockaddr_storage); //클라이언트 소켓 주소 구조체 길이를 반환한다.
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
// 듣기 식별자를 통해 연결 요청이 들어오면 해당 클라이언트 주소와 길이를 바탕으로 새로운 연결 소켓을 생성하고 연결 식별자를 반환한다.
// SA*는 struct sockaddr 형식의 포인터 식별자이다.
// 네트워크 프로그래밍에서 일반적으로 사용되는 구조체이다.
    Getnameinfo((SA *)&clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);
// SA *로 캐스팅된 클라이언트 구조체에서 호스트 이름과 포트 번호를 얻는다.
// clientlen, client_hostname, client_port는 변수 포인터이다.
// 이 함수를 실행하면 클라이언트 주소 구조체에서(IP에서) 호스트 이름과 포트 번호를 client_hostname과 client_port 변수에 각각 문자열로 저장한다.
    printf("Conneted to (%s, %s)\n", client_hostname, client_port);
    echo(connfd);
    Close(connfd);
  }
  exit(0);
}

void echo(int connfd)
{
  size_t n;
  char buf[MAXLINE];
  rio_t rio;

  Rio_readinitb(&rio, connfd); //듣기 식별자를 연결시켜서 구조체를 초기화한다.
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) //버퍼에 들어온 문자열의 길이를 반환해서 0이 아닐때
  {
    printf("server received %d bytes\n", (int)n);
    Rio_writen(connfd, buf, n); //연결 식별자를 통해 클라이언트에 버퍼의 내용을 전송한다.
  }
}