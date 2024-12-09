#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"


void find(char *path, char *filename)
{
	char buf[512];
	struct dirent de;
	struct stat st;
	int fd;

	if((fd = open(path, 0))<0)
	{
		fprintf(2, "error");
		return;
	}

	if(fstat(fd, &st) <0)
	{
		fprintf(2, "error");
		close(fd);
		return;
	}

	if(st.type == T_DIR)
	{
		strcpy(buf, path);
		char *p = buf + strlen(buf);
		if(buf[strlen(buf) - 1] != '/')
		{
		*p++='/';
		}
		
		while(read(fd, &de, sizeof(de)) == sizeof(de))
		{
			if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
			{
				continue;
			}
			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			if(strcmp(de.name, filename) == 0)
			{
				printf("%s\n", buf);
			}
			find(buf, filename);
		}
	}

	close(fd);
}
int main(int argc, char **argv)
{
  
  find(argv[1], argv[2]);
  exit(0);
}
