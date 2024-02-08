(python -c 'print ("auth " + "\x01")'; cat) | ./level8


```bash
(gdb) run < <(python -c 'print ("auth " + "\x01")')
```