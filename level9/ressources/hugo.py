from pwn import p32
import sys

# Check if an argument was passed
if len(sys.argv) !=  2:
    print("Usage: python script.py <hex_string>")
    sys.exit(1)

# Get the input string from the command line arguments
input_string = sys.argv[1]

# Ensure the string is a valid hex number
try:
    int(input_string,  16)
except ValueError:
    print("Invalid hex string.")
    sys.exit(1)

# Pack the integer into a  32-bit string using p32
packed_value = p32(int(input_string,  16))

# Format the packed value as a series of hex escape sequences
formatted_output = ''.join(r'\x{:02x}'.format(b) for b in packed_value)

# Print the formatted string
print(formatted_output)
