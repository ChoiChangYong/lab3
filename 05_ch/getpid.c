#include <string.h> 
#include <unistd.h>

static int num = 0; 
static char namebuf[20]; 
static char prefix[] = "/tmp/tmp";

char *gentemp(void) { 
	int length; pid_t pid;

	pid = getpid(); /* 프로세스 식별번호를 얻는다 */ 
	/* 표준 문자열처리 루틴들 */ 
	strcpy (namebuf, prefix); 
	length = strlen(namebuf);

	/* 파일 이름에 프로세스 식별번호(pid)를 추가한다 */ 
	itoa (pid, &namebuf[length]);
	strcat (namebuf, "."); 
	length = strlen (namebuf);  
	  
	do {
		/* 접미 번호를 추가한다 */ 
		itoa (num++, &namebuf[length]); 
	} while (access(namebuf, F_OK) != -1) ; 

	return (namebuf);
}

/* itoa – 정수를 문자열로 변환한다 */ 
int itoa(int i, char *string) { 
	int power, j;
	j = i;
	for (power = 1; j >= 10; j /= 10) 
		power *= 10;


	for ( ; power > 0; power /= 10) { 
		*string++ = '0' + i/power; 
		i %= power;
	}

	*string = '\0';
}

