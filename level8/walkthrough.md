```bash
(gdb) run < <(python -c 'print ("auth " + "\x01")')
```

### Payload

We have to write a character on auth + 8 on the heap, auth is a 4bytes address so we have to write
32 char after auth in the heap, but we can't write more than 30 character because the code as a protection
with strlen(buffer1) <= 30

Do not forget that the heap is contiguous, so at the first loop we write 30 char in the heap at the address of auth, and after that we write service so we strdup 27 char in the service variable
so we are 5 character short from the target of auth + 8

In order to close the gap we re-type service in order to allocate more memory on the heap and copy another 27 char after the previous 27 char we copied with strdup

after that auth + 8 is filled with `A` so when we write login we trigger the if that pop a shell

```bash
(python -c 'print ("auth " + "A" * 29)'; cat) | ./level8
service
service
login
```
