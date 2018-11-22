#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define BUFSIZE 128
#define QKEY (key_t)0111
#define QKEY2 (key_t)0112

struct msgq_data {
  long type;
  char text[BUFSIZE];
};

void *send(void *data)
{
  int qid, len;
  struct msgq_data send_data;

  while(1){
    send_data.type = 1;
    strcpy(send_data.text, "");

    fgets(send_data.text, sizeof(send_data.text), stdin);
    send_data.text[strlen(send_data.text)-1] = '\0';

    if ((qid = msgget(QKEY, IPC_CREAT | 0666)) == -1) {
      perror("msgget failed");
      exit(1);
    }
    if (msgsnd(qid,&send_data,strlen(send_data.text),0) == -1) {
      perror("msgsnd failed");
      exit(1);
    }
  }
}
void *receive(void *data)
{
  int qid, len;
  struct msgq_data *recv_data;

  while(1){
    recv_data = malloc(sizeof(struct msgq_data));

    if ((qid = msgget(QKEY2, IPC_CREAT | 0666)) == -1) {
      perror("msgget failed");
      exit(1);
    }
    if ((len = msgrcv(qid, recv_data, BUFSIZE, 0, 0)) == -1) {
      perror("msgrcv failed");
      exit(1);
    }

    printf("Kim: %s\n", recv_data->text);

    if (msgctl(qid, IPC_RMID, 0) == -1) {
      perror("msgctl failed");
      exit(1);
    }
  }
}

int main() {
  printf("Welcome, You are Choi\n");

  pthread_t p_thread[2];
  int thr_id;
  int status;
  char p1[] = "thread_send";   // 1번 쓰레드 이름
  char p2[] = "thread_receive";   // 2번 쓰레드 이름
  char pM[] = "thread_main";

  thr_id = pthread_create(&p_thread[0], NULL, send, (void *)p1);
  // pthread_create() 으로 성공적으로 쓰레드가 생성되면 0 이 리턴됩니다
  if (thr_id < 0)
  {
      perror("thread create error : ");
      exit(0);
  }

  thr_id = pthread_create(&p_thread[1], NULL, receive, (void *)p1);
  if (thr_id < 0)
  {
      perror("thread create error : ");
      exit(0);
  }

  // 쓰레드 종료를 기다린다.
  pthread_join(p_thread[0], (void **)&status);
  pthread_join(p_thread[1], (void **)&status);
}
