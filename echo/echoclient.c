#include "csapp.h"

int main(int argc, char **argv)
// argc는 argument count. 명령줄 인수의 개수
// argv는 argument vector. 명령줄 인수를 저장하는 문자열 배열
{
  int clientfd;
  char *host, *port, buf[MAXLINE];
  rio_t rio;

  if (argc != 3)//만약 호스트 또는 포트를 입력하지 않으면 에러 메시지가 출력되고 프로그램이 종료된다.
  {
    fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
    exit(0);
  }
  /*argv[0]은 프로그램 이름(실행경로), argv[1]은 호스트, argv[2]는 포트*/
  host = argv[1];
  port = argv[2];

  clientfd = Open_clientfd(host, port);//클라이언트 소켓을 열어서 리턴받은 소켓 식별자를 할당한다.
  Rio_readinitb(&rio, clientfd);//rio 구조체를 초기화한다.

/*반복적으로 사용자의 입력을 받아 버퍼에 저장 한다.
Fgets 함수는 문자열을 한 줄씩 읽는 함수이다.
매개변수로 읽을 위치, 최대치, 읽어올 파일 포인터를 가리킨다*/
  while (Fgets(buf, MAXLINE, stdin) != NULL)
  {
    Rio_writen(clientfd, buf, strlen(buf)); //소켓 식별자를 통해 버퍼에 저장된 내용을 소켓 식별자에 쓰기 작업을 수행한다.
    Rio_readlineb(&rio, buf, MAXLINE);//rio 소켓에서 데이터를 읽어 온다. 읽어온 데이터는 버퍼에 저장한다.지정된 소켓으로부터 한 줄씩 데이터를 읽어 온다.
    Fputs(buf, stdout);//버퍼에 저장된 문자열을 표준 출력 스트림(stdout)으로 출력한다.
  }
  Close(clientfd); //루프가 종료한 후에 클라이언트는 식별자를 닫는다
  exit(0);
}