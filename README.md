# Rainfall - Binary Exploitation Project

Welcome to the Rainfall Binary Exploitation Project. This project is designed to enhance understanding of binary exploitation in the i386 system, focusing on ELF binaries. 

## Getting Started

Before starting, ensure you have a virtual machine running a 64-bit operating system. Once you have started your machine with the provided ISO (rainfall.iso) , you should receive a simple prompt with an IP address. Use the following command to SSH into the machine:

```bash
ssh level0@localhost -p 4243
```

The default login credentials are `level0:level0`.  
After logging in, your task is to read the `.pass` file in the home directory of each user (excluding level0), which contains the password for the next level.

## Project Structure

- `flag`: Contains the flag for the current level.
- `source`: Contains the exploited binary in a form understandable by any developer.
- `walkthrough`: Contains the steps taken to solve the test.
- `Resources`: Contains any resources needed to prove your results during evaluation.



Remember, the goal is to learn and grow, so take your time and enjoy the process. Good luck!
