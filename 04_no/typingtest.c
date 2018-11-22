#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define PASSWORDSIZE 12
#define SIZE 3

int main(void) {
  int fd;
  int nread, cnt=0, errcnt=0;
  char ch;
  char* text[SIZE] = {"What time is it now?",
                       "It is a quarter past nine by my watch.",
                       "God bless you"};
  int i=0;
  time_t start_time, end_time;
  float res_time;
  int typing_count = 0;

  struct termios init_attr, new_attr;

  fd = open(ttyname(fileno(stdin)), O_RDWR);
  tcgetattr(fd, &init_attr);
  new_attr = init_attr;
  new_attr.c_lflag &= ~ICANON;
  new_attr.c_lflag &= ~ECHO;

  /* ~(ECHO | ECHOE | ECHOK | ECHONL); */
  new_attr.c_cc[VMIN] = 1;
  new_attr.c_cc[VTIME] = 0;

  if (tcsetattr(fd, TCSANOW, &new_attr) != 0) {
    fprintf(stderr, "터미널 속성을 설정할 수 없음.\n");
  }

  printf("다음 문장을 그대로 입력하세요.\n");

  time(&start_time);

  for(i=0; i<SIZE; i++){
    cnt = 0;
    printf("%s\n", text[i]);
    while ((nread=read(fd, &ch, 1)) > 0 && ch != '\n') {
      if (ch == text[i][cnt++])
        write(fd, &ch, 1);
      else {
        write(fd, "*", 1); errcnt++;
      }
      typing_count++;
    }
    printf("\n");
  }
  printf("\n타이핑 오류의 횟수 : %d\n", errcnt);

  time(&end_time);
  res_time = (float)(end_time-start_time);

  printf("올바른 타이핑 횟수 : %d회\n", typing_count);
  printf("총 걸린 시간 : %d초\n", res_time);
  printf("평균 분당 타자수 : %.2f타\n", (float)((60*(typing_count-errcnt))/res_time));

  tcsetattr(fd, TCSANOW, &init_attr); close(fd);
}
