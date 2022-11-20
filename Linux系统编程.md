# Linux_C系统编程

## 一 IO

### 1.1 标准IO

**I/O:input&output,是一切实现的基础**

> stdio:标准IO
>
> sysio:系统调用IO(文件IO)
>
> 优先考虑使用标准IO,stdio:FILE类型贯穿始终



> \#include<stdio.h>
> FILE *fopen( const char *filename, const char *mode );
>
> \#include<stdio.h>
> int fclose( FILE *stream );



***/usr/include/asm-generic/errno-base.h	:存储errno的文件***

**ulimit -a:查看系统限制**

**umask**

**diff 文件1 文件2 :比较两个文件是否相同**



>补充:
>
>#include<stdio.h> or <stdlib.h> 
>void perror( const char *string );//将errno打印成错误信息
>
>#include<string.h>
>char *strerror( int errnum );
>
>#incldue<stdio.h>
>
>int fprintf( FILE *stream, const char *format [, argument ]...);



>#include<stdio.h>
>int fgetc( FILE *stream );
>
>#include<stdio.h>
>int fputc( int c, FILE *stream );



> \#include<stdio.h>
> char *fgets( char *string, int n, FILE *stream );
> 两个正常结束的条件:
> 1.读取到n-1个字符
> 2.读取到'\n'
>
> \#include<stdio.h>
> int fputs( const char *string, FILE *stream );



> \#include<stdio.h>
> size_t fread( void *buffer, size_t size, size_t count, FILE *stream );
>
> \#include<stdio.h>
> size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
>
> **fread(buf,size,memb,fp)**
> **1->数据量足够**
> **2->只有5个字节**
> **fread(buf,1,10,fp)**
> **1->		读10次,读到10个字节**
> **2->		读5次,读到五个字节**
> **fread(buf,10,1,fp)**
> **1->		读1次,读到10个字节**
> **2->		读到0次,读到字节不详**
>
> **建议将fread函数的每次读取size设置为1**



**int atoi( const char \*string ); 将字符串装换位整形串**
**int sprintf( char *buffer, const char \*format [, argument] ... ); 将多种数据类型装换为字符串输出,可以看作是 atoi 函数的逆转.**



> \#incldue<stdio.h>
> int printf( const char *format [, argument]... );
>
> \#incldue<stdio.h>
> int scanf( const char *format [,argument]... );



>\#include<stdio.h>
>int fseek( FILE *stream, long offset, int origin );
>
>\#include<stdio.h>
>long ftell( FILE *stream );



>\#include<stdio.h>
>int fflush( FILE *stream );
>
>**缓冲区的作用:		->(大多数情况是好事,合并系统调用)
>行缓冲:换行的时候刷新|满了的时候刷新|强制刷新(标准输出是这样的,因为是终端设备)
>全缓冲:满了的时候刷新|强制刷新
>无缓冲:如stderr**



**int setvbuf( FILE *stream, char *buffer, int mode, size_t size );:可以更改缓冲模式**



> #include <stdio.h>	
>
> ssize_t getline(char **lineptr, size_t *n, FILE *stream);



> **临时文件:  1.如何不冲突  2.及时销毁**
>
> #include <stdio.h>	
>
> char *tmpnam( char *string );	//	返回一个临时文件名
>
> #include <stdio.h>	
>
> FILE *tmpfile( void );:以二进制形式读写打开一个文件  (匿名文件)



### 1.2 文件IO/系统调用IO

​			**fd是在文件IO中贯穿始终的类型**

#### 1.2.1 文件描述符的概念   

​			文件描述符的实质是一个整型数,数组下标,文件描述符优先使用当前可用范围内最小的一个

#### 1.2.2 文件IO操作相关函数:

> **open**
>
> ```c
> #include <sys/types.h>
> #include <sys/stat.h>
> #include <fcntl.h>
> 
> int open(const char *pathname, int flags);
> int open(const char *pathname, int flags, mode_t mode);
> ```
>
> **close**
>
> ```c
> #include <unistd.h>
> 
> int close(int fd);
> ```
>
> **read**
>
> ```c
> #include <unistd.h>
> 
> ssize_t read(int fd, void *buf, size_t count);
> ```
>
> **write**
>
> ```c
> #include <unistd.h>
> 
> ssize_t write(int fd, const void *buf, size_t count);
> ```
>
> **lseek**
>
> ```c
> #include <sys/types.h>
> #include <unistd.h>
> 
> off_t lseek(int fd, off_t offset, int whence);
> ```

#### 1.2.3 文件IO和标准IO的区别

​			**标准IO依赖于文件IO实现,.**

​			1.响应速度: 文件IO快于标准IO

​			2.吞吐量: 标准IO吞吐量大于文件IO

​			**&面试: **如何使一个程序变快?

​			**#提醒:** 标准IO和文件IO不可混用

> fileno: 将文件流转换为文件描述符
>
> ```c
> #include <stdio.h>
> 
> int fileno(FILE *stream);
> ```
>
> fdopen: 以文件流形式打开文件描述符文件
>
> ```c
> #include <stdio.h>
> 
> FILE *fdopen(int fd, const char *mode);
> ```

#### 1.2.4 IO效率问题

​			**time 命令行**:测试命令执行时间

#### 1.2.5 文件共享

​			多个任务共同操作一个文件或者协同完成任务

​			**&面试:** 写程序删除一个文件的第10行.

> truncate|ftruncate: 将一个未打开的文件或者已打开的文件描述符的内容截断.
>
> ```c
> #include <unistd.h>
> #include <sys/types.h>
> 
> int truncate(const char *path, off_t length);
> int ftruncate(int fd, off_t length);
> ```

#### 1.2.6 原子操作

​			不可分割的操作,用于解决竞争和冲突.

#### 1.2.7 程序中的重定向

> dup
>
> ```c
> #include <unistd.h>
> 
> int dup(int oldfd);
> int dup2(int oldfd, int newfd); 
> ```

#### 1.2.8 同步

>sync
>
>```c
>#include <unistd.h>
>void sync(void);
>```
>
>fsync|fdatasync
>
>```c
>#include <unistd.h>
>
>int fsync(int fd);
>
>int fdatasync(int fd);	//只刷数据,不刷呀数据
>```

#### 1.2.9 fcntl()

​			文件描述符所变得魔术机会都与该函数相关

> fcntl
>
> ```c
> #include <unistd.h>
> #include <fcntl.h>
> 
> int fcntl(int fd, int cmd, ... /* arg */ );
> ```

#### 1.2.10 ioctl

​			设备相关的内容

> ioctl
>
> ```c
> #include <sys/ioctl.h>
> 
> int ioctl(int fd, unsigned long request, ...);
> ```

#### 1.2.11 /dev/fd/目录

​			虚目录,显示的是当前进程下的文件描述符信息

## 二 文件系统

​			**$本章目标**:能够实现类ls的demo,如 myls -l -a i

### 2.1 目录和文件

#### 2.1.1 获取文件属性

​			**$stat:** 通过文件路径获取属性,面对符号链接文件时获取的是所指向的目标文件的属性.

​			**$fstat:** 通过文件描述符获取文件属性.

​			**$lstat:** 面对符号链接文件时获取的是符号链接文件的属性.

> stat|fstat|lstat: 查看文件信息
>
> ```c
> #include <sys/types.h>
> #include <sys/stat.h>
> #include <unistd.h>
> 
> int stat(const char *pathname, struct stat *statbuf);
> int fstat(int fd, struct stat *statbuf);
> int lstat(const char *pathname, struct stat *statbuf);
> 
> /**************************************/
> struct stat {
>     dev_t     st_dev;         /* ID of device containing file */
>     ino_t     st_ino;         /* Inode number */
>     mode_t    st_mode;        /* File type and mode */
>     nlink_t   st_nlink;       /* Number of hard links */
>     uid_t     st_uid;         /* User ID of owner */
>     gid_t     st_gid;         /* Group ID of owner */
>     dev_t     st_rdev;        /* Device ID (if special file) */
>     off_t     st_size;        /* Total size, in bytes */
>     blksize_t st_blksize;     /* Block size for filesystem I/O */
>     blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */
> 
>     /* Since Linux 2.6, the kernel supports nanosecond
>                   precision for the following timestamp fields.
>                   For the details before Linux 2.6, see NOTES. */
> 
>     struct timespec st_atim;  /* Time of last access */
>     struct timespec st_mtim;  /* Time of last modification */
>     struct timespec st_ctim;  /* Time of last status change */
> 
>     #define st_atime st_atim.tv_sec      /* Backward compatibility */
>     #define st_mtime st_mtim.tv_sec
>     #define st_ctime st_ctim.tv_sec
> };
> /**************************************/
> 注意:
> 	1) st_size!=st_blksize*st_blocks
>     2) cp 命令支持空洞文件拷贝
> ```

#### 2.1.2 文件访问权限

> st_mode是一个16位的位图,用于表示文件类型,文件访问权限,及特殊权限位
>
> 文件类型(7种): dcb-lsp
>
> 文件权限: rwxrwxrwx

#### 2.1.3 umask

​			作用:防止产生权限过松的文件

> umask
>
> ```c
> #include <sys/types.h>
> #include <sys/stat.h>
> 
> mode_t umask(mode_t mask);
> ```

#### 2.1.4 文件权限的更改/管理

> chmod|fchmod
>
> ```c
> #include <sys/stat.h>
> 
> int chmod(const char *pathname, mode_t mode);
> int fchmod(int fd, mode_t mode);
> ```

#### 2.1.5 粘住位

​			t位(已经不常用)

#### 2.1.6 文件系统

​			文件系统:文件或数据的存储和管理

> FAT16|FAT32:实质是静态存储的单链表 
>
> UFS---->文件存储格式

#### 2.1.7 硬链接,符号链接

​			**$硬链接:** 类似于两个指针指向同一个文件

​			**$符号链接:** 类似于window中的快捷方式,本身不占内存

​			硬链接与目录项是同义词,且建立硬链接有限制:不能给分区建立,不能给目录建立.符号链接优点:可跨分区,可以给目录建立

> link
>
> ```c
> #include <unistd.h>
> 
> int link(const char *oldpath, const char *newpath);
> ```
>
> unlink
>
> ```c
> #include <unistd.h>
> 
> int unlink(const char *pathname);
> ```
>
> remove: 删除一个文件
>
> ```c
> #include <stdio.h>
> 
> int remove(const char *pathname);
> ```
>
> rename: 改变文件名字或位置
>
> ```c
> #include <stdio.h>
> 
> int rename(const char *oldpath, const char *newpath);
> ```

#### 2.1.8 utime

> utime: 可更改文件的最后读取时间和最后修改的时间
>
> ```c
> #include <sys/types.h>
> #include <utime.h>
> 
> int utime(const char *filename, const struct utimbuf *times);
> /**********************************/
> The utimbuf structure is:
> 
> struct utimbuf {
>     time_t actime;       /* access time */
>     time_t modtime;      /* modification time */
> };
> /********************************/
> struct timeval {
>     long tv_sec;        /* seconds */
>     long tv_usec;       /* microseconds */
> };
> ```

#### 2.1.9 目录的创建和销毁

>mkdir
>
>```c
>#include <sys/stat.h>
>#include <sys/types.h>
>
>int mkdir(const char *pathname, mode_t mode);
>```
>
>rmdir: 只能删除一个空的文件夹
>
>```c
>#include <unistd.h>
>
>int rmdir(const char *pathname);
>```

#### 2.1.10 更改当前工作路径

> chdir|fchdir
>
> ```c
> #include <unistd.h>
> 
> int chdir(const char *path);
> int fchdir(int fd);
> ```
>
> getcwd: 获取当前工作路径
>
> ```c
> #include <unistd.h>
> 
> char *getcwd(char *buf, size_t size);
> 
> char *getwd(char *buf);
> ```

#### 2.1.11 分析目录/读取目录内容

> glob: 解析模式,通配符
>
> ```c
> #include <glob.h>
> 
> int glob(const char *pattern, int flags,
>          int (*errfunc) (const char *epath, int eerrno),
>          glob_t *pglob);
> /******************************************/
> typedef struct {
>     size_t   gl_pathc;    /* Count of paths matched so far  */
>     char   **gl_pathv;    /* List of matched pathnames.  */
>     size_t   gl_offs;     /* Slots to reserve in gl_pathv.  */
> } glob_t;
> ```
>
> opendir: 打开一个目录
>
> ```c
> #include <sys/types.h>
> #include <dirent.h>
> 
> DIR *opendir(const char *name);
> DIR *fdopendir(int fd);
> ```
>
> closedir: 关闭一个目录
>
> ```c
> #include <sys/types.h>
> #include <dirent.h>
> 
> int closedir(DIR *dirp);
> ```
>
> readdir: 读取文件夹信息
>
> ```c
> #include <dirent.h>
> 
> struct dirent *readdir(DIR *dirp);
> /*****************************************/
> struct dirent {
>     ino_t          d_ino;       /* Inode number */
>     off_t          d_off;       /* Not an offset; see below */
>     unsigned short d_reclen;    /* Length of this record */
>     unsigned char  d_type;      /* Type of file; not supported
>                                               by all filesystem types */
>     char           d_name[256]; /* Null-terminated filename */
> };
> ```
>
> rewenddir
>
> seekdir
>
> telldir:

​			**du 查看文件目录所占大小**

补充:

>strchr|strrchr: 从一个字符串中寻找指定字符
>
>```c
>#include <string.h>
>
>char *strchr(const char *s, int c);
>char *strrchr(const char *s, int c);
>```

​			**&原则:** 如果一个变量的使用是完全在递归之前或之后,那么它可以被优化为静态变量或者是全局变量

### 2.2 系统数据文件和信息

#### 2.2.1 /etc/passwd

> 1.getpwuid:根据uid查询用户信息
>
> 2 getpwnam :根据用户名获取用户信息
>
> ```c
> #include <sys/types.h>
> #include <pwd.h>
> 
> struct passwd *getpwnam(const char *name);
> 
> struct passwd *getpwuid(uid_t uid);
> /**********************************/
> struct passwd {
>     char   *pw_name;       /* username */
>     char   *pw_passwd;     /* user password */
>     uid_t   pw_uid;        /* user ID */
>     gid_t   pw_gid;        /* group ID */
>     char   *pw_gecos;      /* user information */
>     char   *pw_dir;        /* home directory */
>     char   *pw_shell;      /* shell program */
> };
> ```

#### 2.2.2 /etc/group

> 1.getgrgid();根据组id查询组相关信息
>
> 2.getgrgrnam():根据组名查询组相关信息
>
> ```c
> #include <sys/types.h>
> #include <grp.h>
> 
> struct group *getgrnam(const char *name);
> 
> struct group *getgrgid(gid_t gid);
> /**********************************/
> struct group {
>     char   *gr_name;        /* group name */
>     char   *gr_passwd;      /* group password */
>     gid_t   gr_gid;         /* group ID */
>     char  **gr_mem;         /* NULL-terminated array of pointers to names of group members */
> };
> ```

#### 2.2.3 /etc/shadow

> 1.getspnam();根据用户名获取shadow信息
>
> ```c
> #include <shadow.h>
> 
> struct spwd *getspnam(const char *name);
> /*************************************/
> struct spwd {
>     char *sp_namp;     /* Login name */
>     char *sp_pwdp;     /* Encrypted password */
>     long  sp_lstchg;   /* Date of last change(measured in days since 1970-01-01 00:00:00 +0000 (UTC)) */
>     long  sp_min;      /* Min # of days between changes */
>     long  sp_max;      /* Max # of days between changes */
>     long  sp_warn;     /* # of days before password expires to warn user to change it */
>     long  sp_inact;    /* # of days after password expires until account is disabled */
>     long  sp_expire;   /* Date when account expires (measured in days since 1970-01-01 00:00:00 +0000 (UTC)) */
>     unsigned long sp_flag;  /* Reserved */
> };
> ```
>
> 2.crypt():加密方法
>
> ```c
> #include <crypt.h>
> 
> char *crypt(const char *phrase, const char *setting);
> ```
>
> 3.getpass():根据登录密码获取shadow中密码加密串
>
> ```c
> #include <unistd.h>
> 
> char *getpass(const char *prompt);
> ```

补充:

**hash: 混淆 不可逆 如果原串相同,所得串也一样,防备管理员坚守自盗**

#### 2.2.4 时间戳

​			时间戳的三种形式:

​			**&time_t**

​			**&struct tm**

​			**&char ***

![img](https://gimg2.baidu.com/image_search/src=http%3A%2F%2Fs2.51cto.com%2Fimages%2Fblog%2F202201%2F01001045_61cf2b858e8615483.png%3Fx-oss-process%3Dimage%2Fwatermark%2Csize_16%2Ctext_QDUxQ1RP5Y2a5a6i%2Ccolor_FFFFFF%2Ct_30%2Cg_se%2Cx_10%2Cy_10%2Cshadow_20%2Ctype_ZmFuZ3poZW5naGVpdGk%3D&refer=http%3A%2F%2Fs2.51cto.com&app=2002&size=f9999,10000&q=a80&n=0&g=0n&fmt=auto?sec=1669724438&t=95367e632e758a5977b899670c3920ea)

> 1.time()
>
> ```c
>#include <time.h>
> 
>time_t time(time_t *tloc);
> ```
>
> 2.gmtime();
>
> 3.localtime();
>
> ```c
>#include<time.h>
> struct tm *gmtime(const time_t *timep);
> struct tm *gmtime_r(const time_t *timep, struct tm *result);
> 
> struct tm *localtime(const time_t *timep);
> struct tm *localtime_r(const time_t *timep, struct tm *result);
> /****************************************/
> struct tm {
>     int tm_sec;    /* Seconds (0-60) */
>     int tm_min;    /* Minutes (0-59) */
>     int tm_hour;   /* Hours (0-23) */
>     int tm_mday;   /* Day of the month (1-31) */
>     int tm_mon;    /* Month (0-11) */
>     int tm_year;   /* Year - 1900 */
>     int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
>     int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
>     int tm_isdst;  /* Daylight saving time */
> };
> ```
> 
> 	4.mktime();
> 
> ```c
> #include<time.h>
> time_t mktime(struct tm *tm);
> ```
> 
> 5.strftime();
> 
> ```c
> #include <time.h>
> 
> size_t strftime(char *s, size_t max, const char *format,const struct tm *tm);
> ```

​			**tail -f 文件名:** 动态查看文件变化

### 2.3 进程环境

#### 2.3.1 main函数

​			int main(int argc,char* argv[])

#### 2.3.2 进程的终止情况

**1.正常终止:**

1. ​		从main函数返回

2. ​		调用exit: 程序正常退出,刷新缓冲,释放资源

3. ​		调用_exit或__Exit: 导致当前进程终止,不执行钩子函数,不执行IO清理

4. ​		最后一个线程从其启动例程返回

5. ​		最后一个线程调用pthread_exit()

**2.异常终止:**

1. ​		调用abort

2.  		接到一个信号并终止

3. ​		最后一个线程对其取消请求做出响应

​			**$?:** 返回终端上一条语句执行的结果

> atexit():钩子函数
>
> ```c
> #include <stdlib.h>
> 
> int atexit(void (*function)(void));
> ```

#### 2.3.3 命令行参数的分析

> getopt():
>
> ```c
> #include <unistd.h>
> 
> int getopt(int argc, char * const argv[],const char *optstring);
> 
> extern char *optarg;
> extern int optind, opterr, optopt;
> ```
>
> getopt_long
>
> ```c
> #include <getopt.h>
> 
> int getopt_long(int argc, char * const argv[],const char *optstring,const struct option *longopts, int *longindex);
> ```

#### 2.3.4 环境变量

​			**export:** 显示环境变量,格式: KEY=VALUE

> getenv():获取环境变量值
>
> ```c
> #include <stdlib.h>
> 
> char *getenv(const char *name);
> ```
>
> setenv():改变或者添加环境变量值
>
> ```c
> #include <stdlib.h>
> 
> int setenv(const char *name, const char *value, int overwrite);
> ```
>
> putenv():改变或者添加环境变量值
>
> ```c
> #include <stdlib.h>
> 
> int putenv(char *string);
> ```

#### 2.3.5 C程序的存储空间布局

 			**pmap**:一条命令

#### 2.3.6 库

​			动态库:

​			静态库:

​			手工装载库:

#### 2.3.7 函数跳转

​			goto(): 只能实现单一函数内跳转

> setjmp();可以实现跨函数安全跳转
>
> ```c
> #include <setjmp.h>
> 
> int setjmp(jmp_buf env);
> void longjmp(jmp_buf env, int val);
> ```

#### 2.3.8 资源的获取以及控制

> 1.getrlimit():
>
> 2.setrlimit():
>
> ```c
> #include <sys/time.h>
> #include <sys/resource.h>
> 
> int getrlimit(int resource, struct rlimit *rlim);
> int setrlimit(int resource, const struct rlimit *rlim);
> /**************************************/
> struct rlimit {
>     rlim_t rlim_cur;  /* Soft limit */
>     rlim_t rlim_max;  /* Hard limit (ceiling for rlim_cur) */
> };
> ```
>
> 

## 三 进程基本知识

### 3.1 进程标识符(pid)

​			1.类型: pid_t (有符号的16位数字)

​			2.**ps:** 显示当前系统进程信息

​			3.进程号是顺次向下使用的

> pid_t  getpid(void):返回进程号
>
> pid_t  getppid(void):返回当前进程的父进程
>
> ```c
> #include <sys/types.h>
> #include <unistd.h>
> 
> pid_t getpid(void);
> pid_t getppid(void);
> ```

### 3.2 进程的产生

​			1.fork():复制一个线程,子进程运行完后需要及时销毁,exit(0);

​			**fork后父子进程的区别:**

​				**1.fork的返回值不一样**

​				**2.pid不同**

​				**3.ppid也不相同**

​				**4.未决信号和文件锁不继承**

​				**5.资源利用量清0**

​			init进程:1号,是所有进程的的祖先进程

​			***调度器的调度策略来决定那个进程先运行***

​			**在fork()之前一定要先刷新缓冲流.**

> fork
>
> vfork():子进程共用父进程的数据区域,如果需要写数据,写时拷贝.
>
> ```c
>#include <sys/types.h>
> #include <unistd.h>
>
> pid_t fork(void);
>```

​			**time 命令:** 查看执行某条命令需要使用的时间

​			**命令 > /dev/null: ** 将命令执行结果输出到空设备,不显示

​			**命令 | wc -l:** 以行为单位统计命令执行的输出结果

### 3.3 进程的消亡及释放资源

> 1. wait
>
> 2. waitpid
>
> ```c
> #include <sys/types.h>
> #include <sys/wait.h>
> 
> pid_t wait(int *wstatus);
> 
> pid_t waitpid(pid_t pid, int *wstatus, int options);
> ```
>
> 3.wait3();
>
> 4.wait4();

### 3.4 exec函数族

> 1.execl();
>
> 2.execlp();
>
> 3.execle();
>
> 4.execv();
>
> 5.execvp();
>
> ```c
> #include <unistd.h>
> 
> extern char **environ;
> 
> int execl(const char *pathname, const char *arg, ...
>           /* (char  *) NULL */);
> int execlp(const char *file, const char *arg, ...
>            /* (char  *) NULL */);
> int execle(const char *pathname, const char *arg, ...
>            /*, (char *) NULL, char *const envp[] */);
> int execv(const char *pathname, char *const argv[]);
> int execvp(const char *file, char *const argv[]);
> int execvpe(const char *file, char *const argv[],
>             char *const envp[]);
> ```

​			**注意:执行execl函数之前要调用fflush()函数**

​			**字符串分割函数**

```c
#include <string.h>

char *strtok(char *str, const char *delim);
```

```c
#include <string.h>
char *strsep(char **stringp, const char *delim);
```

### 3.5 用户权限及组权限

​			**1.u+s**

​			**2.g+s**

> 1.geteuid();
>
> 2.getuid
>
> ```c
> #include <unistd.h>
> #include <sys/types.h>
> 
> uid_t getuid(void);
> uid_t geteuid(void);
> ```
>
> 3.getgid();
>
> 4.getegid();
>
> ```c
> #include <unistd.h>
> #include <sys/types.h>
> 
> gid_t getgid(void);
> gid_t getegid(void);
> ```
>
> 5.setuid();
>
> 6.setgid();
>
> ```c
> #include <sys/types.h>
> #include <unistd.h>
> 
> int setuid(uid_t uid);
> int setgid(gid_t gid);
> ```
>
> 9.setreuid();
>
> 10.setregid();
>
> ```c
> #include <sys/types.h>
> #include <unistd.h>
> 
> int setreuid(uid_t ruid, uid_t euid);
> int setregid(gid_t rgid, gid_t egid);
> ```
>
> 11.seteuid();
>
> 12.setegid();
>
> ```c
> #include <sys/types.h>
> #include <unistd.h>
> 
> int seteuid(uid_t euid);
> int setegid(gid_t egid);
> ```

### 3.6 观摩课:解释器文件

### 3.7 system();

​			**理解成 fork+exec+wait封装**

### 3.8 进程会计

> acct();
>
> ```c
> #include <unistd.h>
> 
> int acct(const char *filename);
> ```

### 3.9 进程时间

> times();
>
> ```c
> #include <sys/times.h>
> 
> clock_t times(struct tms *buf);
> /**************************/
> struct tms {
>     clock_t tms_utime;  /* user time */
>     clock_t tms_stime;  /* system time */
>     clock_t tms_cutime; /* user time of children */
>     clock_t tms_cstime; /* system time of children */
> };
> ```

### 3.10 守护进程

​			1.会话session

​			2.终端

> 3.setsid();
>
> 4.getpgrp();
>
> 5.getpgid()
>
> 6.setpgid();
>
> ```c
> #include <sys/types.h>
> #include <unistd.h>
> 
> pid_t setsid(void);
> ```

​			1.实现单实例守护进程: 锁文件 /var/run/name.pid

​			2.启动脚本文件: /etc/rc*...

### 3.11 系统日志

>syslog
>
>```c
>#include <syslog.h>
>
>void openlog(const char *ident, int option, int facility);
>void syslog(int priority, const char *format, ...);
>void closelog(void);
>```

## 四 并发(信号,线程)

### *同步异步*

​			异步事件的处理: 查询法,通知法

### 4.1 信号

#### 4.1.1 信号的概念

​			1.信号是软件中断

​			2.信号的响应依赖于中断

#### 4.1.2 signal();

>#include <signal.h>
>
>```c
>#include <signal.h>
>
>typedef void (*sighandler_t)(int);
>
>sighandler_t signal(int signum, sighandler_t handler);
>/另一种实现/
>void (*signal(int signum, void (*func)(int)))(int);
>```

​			**信号会打断阻塞的系统调用**

#### 4.1.3 信号的不可靠

​			信号的行为不可靠

#### 4.1.4 可重入函数

​			概念: 函数第一次调用还没有结束,就调用第二次.

​			为了解决信号的不可靠.

​			所有的系统调用都是可重入的,一部分库函数也是可重入的,如:memcpy

#### 4.1.5 信号的响应过程

> 信号从收到到响应有一个不可避免的延迟
>
> 思考:1.如何忽略到一个信号的?
>
> ​		2.标准信号为什么要丢失.
>
> ​		3.标准信号的响应没有严格的顺序
>
> ​		4.不能从信号处理函数中随意的往外跳

#### 4.1.6 信号的常用函数

> 1.kill
>
> ```c
> #include <sys/types.h>
> #include <signal.h>
> 
> int kill(pid_t pid, int sig);
> ```
>
> 2.raise();
>
> ```c
> #include <signal.h>
> 
> int raise(int sig);
> ```
>
> 3.alarm
>
> ```c
> #include <unistd.h>
> 
> unsigned int alarm(unsigned int seconds);
> ```
>
> 4.pause
>
> ```c
> #include <unistd.h>
> 
> int pause(void);
> ```
>
> 5.abort
>
> ```c
> #include <stdlib.h>
> 
> void abort(void);
> ```
>
> 6.system
>
> ```c
> #include <stdlib.h>
> 
> int system(const char *command);
> ```
>
> 7.sleep()
>
> ```c
> #include <unistd.h>
> 
> unsigned int sleep(unsigned int seconds);
> ```
>
> 8.nanosleep():可以用来代替sleep
>
> ```c
> #include <time.h>
> 
> int  nanosleep(const struct timespec *req, struct timespec *rem);
> /****************************/
> struct timespec {
>     time_t tv_sec;        /* seconds */
>     long   tv_nsec;       /* nanoseconds */
> };
> ```
>
> 9.usleep()
>
> ```c
> #include <unistd.h>
> 
> int usleep(useconds_t usec);
> ```
>
> 10.setitimer
>
> 例题:使用单一计时器,利用alarm或者setitimer构造一组函数,实现任意数量的计时器
>
> ```c
> #include <sys/time.h>
> 
> int getitimer(int which, struct itimerval *curr_value);
> int setitimer(int which, const struct itimerval*new_value,struct itimerval *old_value);
> /**************************/
> struct itimerval {
>     struct timeval it_interval; /* Interval for periodic timer */
>     struct timeval it_value;    /* Time until next expiration */
> };
> struct timeval {
>     time_t      tv_sec;         /* seconds */
>     suseconds_t tv_usec;        /* microseconds*/
> };
> ```

#### 4.1.7 信号集

​			**信号集类型:sigset_t**

> 1.sigemptyset()
>
> 2.sigfillset()
>
> 3.sigaddset()
>
> 4.sigdelset()
>
> 5.sigismember()
>
> ```c
> #include <signal.h>
> 
> int sigemptyset(sigset_t *set);
> 
> int sigfillset(sigset_t *set);
> 
> int sigaddset(sigset_t *set, int signum);
> 
> int sigdelset(sigset_t *set, int signum);
> 
> int sigismember(const sigset_t *set, int signum);
> ```

#### 4.1.8 信号屏蔽字/判定集的处理

> 1.sigprocmask:
>
> ```c
> #include <signal.h>
> 
> /* Prototype for the glibc wrapper function */
> int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
> 
> /* Prototype for the underlying system call */
> int rt_sigprocmask(int how, const kernel_sigset_t *set,
>                    kernel_sigset_t *oldset, size_t sigsetsize);
> 
> /* Prototype for the legacy system call (deprecated) */
> int sigprocmask(int how, const old_kernel_sigset_t *set,old_kernel_sigset_t *oldset);
> ```
>
> 2.sigpending();		几乎不用
>
> ```c
> #include <signal.h>
> 
> int sigpending(sigset_t *set);
> ```

#### 4.1.9 扩展

> 1.sigsuspend();
>
> ```c
> #include <signal.h>
> 
> int sigsuspend(const sigset_t *mask);
> ```
>
> 2.sigaction();
>
> ```c
> #include <signal.h>
> 
> int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
> /***********************************************/
> struct sigaction {
>     void     (*sa_handler)(int);
>     void     (*sa_sigaction)(int, siginfo_t *, void *);
>     sigset_t   sa_mask;
>     int        sa_flags;
>     void     (*sa_restorer)(void);
> };
> ```
>
> 3.setitimer();

#### 4.1.10 实时信号

​			**实时信号不丢失** 

### 4.2 线程

#### 4.2.1 线程的概念

​		**一个正在运行的函数**

​		**posix线程是一套标准,而不是实现**

​		openmp线程**

​		线程标识:pthread_t**

> pthread_equal();
>
> ```c
> #include <pthread.h>
> 
> int pthread_equal(pthread_t t1, pthread_t t2);
> ```
>
> pthread_self();
>
> ```c
> #include <pthread.h>
> 
> pthread_t pthread_self(void);
> ```

#### 4.2.2 线程的生命周期

##### 1.线程的创建

> pthread_create();创建一个线程
>
> ```c
> #include <pthread.h>
> 
> int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);
> ```
>
> **线程的调度取决于调度器策略**

##### 2 线程的终止

​		**3种方式:**

​			**1)线程从启动例程返回,返回值就是线程的退出码**

​			**2)线程可以被同一进程中的其他线程取消**

​			**3)线程调用pthread_exit()函数**

> pthread_join();线程收尸     ----->   wait();
>
> ```c
> #include <pthread.h>
> 
> int pthread_join(pthread_t thread, void **retval);
> ```

​	**栈的清理**

>pthread_cleanup_push();
>
>pthread_cleanup_pop(); 
>
>```c
>#include <pthread.h>
>
>void pthread_cleanup_push(void (*routine)(void *),void *arg);
>void pthread_cleanup_pop(int execute);
>```

##### 3 线程的取消 

>pthread_cancel
>
>```c
>#include <pthread.h>
>
>int pthread_cancel(pthread_t thread);
>```

​		取消有2种状态:允许和不允许

​		允许取消又分为	1)异步cancel	2)推迟cancel(默认)->推迟至cancel点

​		cancel点:POSIX定义的cancel点,都是可能引发阻塞的系统调用

> pthread_setcancelstate()	设置是否允许取消
>
> pthread_setcanceltype()	设置取消方式
>
> ```c
> #include <pthread.h>
> 
> int pthread_setcancelstate(int state, int *oldstate);
> int pthread_setcanceltype(int type, int *oldtype);
> ```
>
> pthread_testcancel()	本函数什么都不做,就是一个取消点.
>
> ```c
> #include <pthread.h>
> 
> void pthread_testcancel(void);
> ```
>
> pthread_detach()	线程分离
>
> ```c
> #include <pthread.h>
> 
> int pthread_detach(pthread_t thread);
> ```

#### 4.2.3 线程同步

​		互斥量:	pthread_mutex_t

> ​	pthread_mutex_init();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
> pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
> ```
>
> ​	pthread_mutex_destroy();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutex_destroy(pthread_mutex_t *mutex);
> int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
> pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
> ```
>
> ​	pthread_mutex_lock();
>
> ​	pthread_mutex_trylock();
>
> ​	pthread_mutex_unlock();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutex_lock(pthread_mutex_t *mutex);
> int pthread_mutex_trylock(pthread_mutex_t *mutex);
> int pthread_mutex_unlock(pthread_mutex_t *mutex);
> ```
>
> sched_yield
>
> ```c
> #include <sched.h>
> 
> int sched_yield(void);
> ```
>
> pthread_once
>
> ```c
> #include <pthread.h>
> 
> int pthread_once(pthread_once_t *once_control,void (*init_routine)(void));
> pthread_once_t once_control = PTHREAD_ONCE_INIT;
> ```

​		条件变量:	pthread_cond_t();

> ​	pthread_cond_init();
>
> ​	pthread_cond_destroy();
>
> ```c
> #include <pthread.h>
> 
> int pthread_cond_destroy(pthread_cond_t *cond);
> int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrict attr);
> pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
> ```
>
> ​	pthread_cond_broadcast();
>
> ​	pthread_cond_siganl();
>
> ```c
> #include <pthread.h>
> 
> int pthread_cond_broadcast(pthread_cond_t *cond);
> int pthread_cond_signal(pthread_cond_t *cond);
> ```

​			信号量:

​			读写锁:	读锁->共享锁	写锁->互斥锁

#### 4.2.4 线程属性

> pthread_attr_init();
>
> pthread_attr_destroy();
>
> ```c
> #include <pthread.h>
> 
> int pthread_attr_init(pthread_attr_t *attr);
> int pthread_attr_destroy(pthread_attr_t *attr);
> ```
>
> pthread_attr_setstacksize();
>
> ```c
> #include <pthread.h>
> 
> int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
> int pthread_attr_getstacksize(const pthread_attr_t *attr,size_t *stacksize);
> ```
>
> 见 man pthread_attr_init 的  see also

#### 4.2.5 线程同步的属性

​			互斥量属性:

> ​	pthread_mutexattr_init();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutexattr_init(pthread_mutexattr_t *attr);
> ```
>
> ​	pthread_mutexattr_destroy();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
> int pthread_mutexattr_init(pthread_mutexattr_t *attr);
> ```
>
> ​	pthread_mutexattr_getpshared();
>
> ​	pthread_mutexattr_setpshared();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr,int *pshared);
> int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,int pshared);
> ```
>
> ​	clone();
>
> ```c
> #define _GNU_SOURCE
> #include <sched.h>
> 
> int clone(int (*fn)(void *), void *stack, int flags, void *arg, .../* pid_t *parent_tid, void *tls, pid_t *child_tid */ );
> ```
>
> ​	pthread_mutexattr_gettype();
>
> ​	pthread_mutexattr_setype();
>
> ```c
> #include <pthread.h>
> 
> int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,int *restrict type);
> int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
> ```

​			条件变量属性:

> ​	pthread_condattr_t
>
> ​    pthread_condattr_init();
>
> ​    pthread_condattr_destroy();
>
> ```c
> #include <pthread.h>
> 
> int pthread_condattr_destroy(pthread_condattr_t *attr);
> int pthread_condattr_init(pthread_condattr_t *attr);
> ```

​			读写锁属性:

#### 4.2.5 重入

​			多线程中的IO

##### 线程与信号

> ​	pthread_sigmask();
>
> ```c
> #include <signal.h>
> 
> int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
> ```
>
> ​	sigwait();
>
> ```c
> #include <signal.h>
> 
> int sigwait(const sigset_t *set, int *sig);
> ```
>
> ​	pthread_kill();
>
> ```c
> #include <signal.h>
> 
> int pthread_kill(pthread_t thread, int sig);
> ```

##### 线程与fork

## 五 高级IO

​			非阻塞IO     ---------      阻塞IO

​			补充:有限状态机编程

### 5.1 非阻塞IO

​			简单流程:自然流程是结构化的

​			复杂流程:自然流程是非结构化的

### 5.2 IO多路转接

> select();移植性好
>
> ```c
> #include <sys/select.h>
> 
> int select(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, struct timeval *timeout);
> 
> void FD_CLR(int fd, fd_set *set);
> int  FD_ISSET(int fd, fd_set *set);
> void FD_SET(int fd, fd_set *set);
> void FD_ZERO(fd_set *set);
> ```
>
> poll();
>
> ```c
> #include <poll.h>
> 
> int poll(struct pollfd *fds, nfds_t nfds, int timeout);
> /*****************************************/
> struct pollfd {
>     int   fd;         /* file descriptor */
>     short events;     /* requested events */
>     short revents;    /* returned events */
> };
> ```
>
> epoll();
>
> ```c
> #include <sys/epoll.h>
> 
> int epoll_create(int size);
> int epoll_create1(int flags);
> ```
>
> epoll_ctl
>
> ```c
> #include <sys/epoll.h>
> 
> int  epoll_ctl(int  epfd,  int  op,  int   fd,   struct epoll_event *event);
> ```
>
> epoll_wait
>
> ```c
> #include <sys/epoll.h>
> 
> int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);
> int epoll_pwait(int epfd, struct epoll_event *events,int maxevents, int timeout,const sigset_t *sigmask);
> ```

### 5.3 其他读写相关函数

> readv();
>
> writev();
>
> ```c
> #include <sys/uio.h>
> 
> ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
> 
> ssize_t writev(int fd, const struct iovec *iov, int iovcnt);
> /*******************************************/
> struct iovec {
>     void  *iov_base;    /* Starting address */
>     size_t iov_len;     /* Number of bytes to transfe
> r */
> };
> ```
>
> readn();
>
> writen();

### 5.4 存储映射IO

> mmap();
>
> munmap()
>
> ```c
> #include <sys/mman.h>
> 
> void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
> int munmap(void *addr, size_t length);
> ```

### 5.5 文件锁

> fcntl()
>
> lockf()
>
> ```c
> #include <unistd.h>
> 
> int lockf(int fd, int cmd, off_t len);
> ```
>
> flock()

## 六 进程间通信

### 6.1 管道

> 内核提供,单工,自同步机制
>
> 1.匿名管道
>
> ​	pipe()
>
> ```c
> #include <unistd.h>
> #include<fcntl.h>
> 
> int pipe2(int pipefd[2], int flags);
> ```
>
> 2.命名管道
>
> ​	mkfifo();
>
> ```c
> #include <sys/types.h>
> #include <sys/stat.h>
> 
> int mkfifo(const char *pathname, mode_t mode);
> ```

### 6.2 XSI  -> Sysv

> IPC -> Inter-Process Communication
>
> 主动端:先发包的一方
>
> 被动端:先收包的一端(先运行)
>
> key:ftok()
>
> ```c
> #include <sys/types.h>
> #include <sys/ipc.h>
> 
> key_t  ftok(const char *pathname,int proj_id);
> ```

1.Message Queues

> ​	msgget()		//创建
>
> ​	msget()		  //操作
>
> ​	msgctl()		//控制
>
> ```c
> #include <sys/types.h>
> #include <sys/ipc.h>
> #include <sys/msg.h>
> 
> int msgget(key_t key, int msgflg);
> int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
> ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
> int msgctl(int msqid, int cmd, struct msqid_ds *buf);
> ```

2.Semaphare Arrays

> ​	semget()
>
> ​	semop();
>
> ​	semctl();
>
> ```c
> #include <sys/types.h>
> #include <sys/ipc.h>
> #include <sys/sem.h>
> 
> int semget(key_t key, int nsems, int  sem‐flg);
> int semop(int semid, struct sembuf *sops, size_t nsops);
> int semctl(int semid, int semnum, int cmd, ...);
> ```

3.Shared Memory

> ​	shmget()
>
> ​	shmop()
>
> ​	shmctl();
>
> ```c
> #include <sys/ipc.h>
> #include <sys/types.h>
> #include <sys/shm.h>
> 
> int shmget(key_t key, size_t size, int shmflg);
> void *shmat(int shmid, const void *shmaddr, int shmflg);
> int shmdt(const void *shmaddr);
> int shmctl(int shmid, int cmd, struct shmid_ds *buf);
> ```

### 6.3 网络套接字     socket

#### *6.3.1讨论:跨主机的传输要注意的问题*

​	1.字节序问题:大端,小端

​		**大端:低地址处放高字节**

​		**小端:低地址处放低字节**

​		主机字节序:host

​		网络字节序:network

​		解决:_to__:htons,htonl,ntohs,ntohl

​	2.对齐:

​		struct{

​			int i;

​			float f;

​			char ch;

​			char ch1;	

​		}

​		解决:不对齐

​	3.类型长度问题:

​		int

​		char

​		解决:int32_t,uint32_t,int64_t,int8_t,uint8_t

​	4.socket是什么?

> socket
>
> ```c
> #include <sys/types.h>          /* See NOTES */
> #include <sys/socket.h>
> 
> int socket(int domain, int type, int protocol);
> ```

**报式套接字**

1.被动端

​	1.取得socket

​	2.给socket取得地址

​	3.收/发消息

​	4.关闭socket

2.主动端

​	1.取得socket

​	2.给socket取得地址(可省略)

​	3.发/收消息

​	4.关闭socket

> socket();
>
> ```c
> #include <sys/types.h>          /* See NOTES */
> #include <sys/socket.h>
> 
> int socket(int domain, int type, int protocol);
> ```
>
> bind();
>
> ```c
> #include <sys/types.h>          /* See NOTES */
> #include <sys/socket.h>
> 
> int bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
> ```
>
> sendto();
>
> ```c
> #include <sys/types.h>
> #include <sys/socket.h>
> 
> ssize_t send(int sockfd, const void *buf, size_t len, int flags);
> 
> ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,const struct sockaddr *dest_addr, socklen_t addrlen);
> 
> ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
> ```
>
> recvfrom();
>
> ```c
> #include <sys/types.h>
> #include <sys/socket.h>
> 
> ssize_t recv(int sockfd, void *buf, size_t len, int flags);
> 
> ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,struct sockaddr *src_addr, socklen_t *addrlen);
> 
> ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
> ```
>
> inet_pton();
>
> ```c
> #include <arpa/inet.h>
> 
> int inet_pton(int af, const char *src, void *dst);
> ```
>
> inet_ntop();
>
> ```c
> #include <arpa/inet.h>
> 
> const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);
> ```
>
> 多点通讯:广播(全网广播,子网广播),多播/组播()
>
> setsockopt
>
> getsockopt
>
> ```c
> #include <sys/types.h>         
> #include <sys/socket.h>
> 
> int getsockopt(int sockfd, int level, int optname,void *optval, socklen_t *optlen);
> int setsockopt(int sockfd, int level, int optname,const void *optval, socklen_t optlen);
> ```
>
> if_nametoindex
>
> ```c
> #include <net/if.h>
> 
> unsigned int if_nametoindex(const char *ifname);
> 
> char *if_indextoname(unsigned int ifindex, char *ifname);
> ```

**流式套接字**

1.C端(主动端)

​	1.获取socket

​	2.给socket取得地址

​	3.发送链接

​	4.收/发消息

​	5.关闭

2.S端(被动端)

​	1.获取socket()

​	2.给socket取的地址

​	3.将socket设置为监听模式

​	4.接受链接

​	5.收/发消息

​	6.关闭

> listen
>
> ```c
> #include <sys/types.h>          /* See NOTES */
> #include <sys/socket.h>
> 
> int listen(int sockfd, int backlog);
> ```
>
> accept
>
> ```c
> #include <sys/types.h>          /* See NOTES */
> #include <sys/socket.h>
> 
> int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
> ```
>
> 
