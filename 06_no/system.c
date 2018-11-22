#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>

void rwx(file_mode){

	  if (S_ISDIR(file_mode))
			printf("d");
		else
			printf("-");

		if (file_mode & S_IRUSR)
			printf("r");
		else
			printf("-");

		if (file_mode & S_IWUSR)
			printf("w");
		else
			printf("-");

		if (file_mode & S_IXUSR)
			printf("x");
		else
			printf("-");

		if (file_mode & S_IRGRP)
			printf("r");
		else
			printf("-");

		if (file_mode & S_IWGRP)
			printf("w");
		else
			printf("-");

		if(file_mode & S_IXGRP)
			printf("x");
		else
			printf("-");

		if (file_mode & S_IROTH)
			printf("r");
		else
			printf("-");

		if (file_mode & S_IWOTH)
			printf("w");
		else
			printf("-");

		if(file_mode & S_IXOTH)
			printf("x");
		else
			printf("-");

		printf(" ");
}

void my_ls_la(){
  DIR *dirp;
  struct dirent *dentry;
  int return_stat;
  char *file_name;
  struct stat file_info;
  struct tm *tminfo;
  mode_t file_mode;
  struct passwd *my_passwd;
  struct group  *my_group;
  char buf1[80];

  if((dirp = opendir(".")) == NULL)
    exit(1);

  while( dentry = readdir(dirp)) {
    if ((return_stat = stat(dentry->d_name, &file_info)) == -1)
    {
      perror("Error : ");
      exit(0);
    }

    file_mode = file_info.st_mode;
    rwx(file_mode);//권한
    printf("%d ", file_info.st_nlink);//링크수

    my_passwd = getpwuid(file_info.st_uid);
    my_group  = getgrgid(file_info.st_gid);
    printf("%s ", my_passwd->pw_name); //소유자
    printf("%s ", my_group->gr_name); //그룹
    printf("%d ", file_info.st_size); //파일크기

    tminfo = localtime (&(file_info.st_atime));
    strftime(buf1, 80, "%m월%d일 %H:%M", tminfo);
    printf("%s ", buf1);   //시간

    if(dentry->d_ino != 0)
    printf("%s\n", dentry->d_name);	 //파일이름
  }
  closedir(dirp);
}


int main(int argc, char* argv[]){
  if(strcmp(argv[1], "ls")==0){
    if(strcmp(argv[2], "-la")==0){
      my_ls_la();
    }
  }
}
