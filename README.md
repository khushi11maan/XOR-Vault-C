# XOR-Vault-C
**Project - 1234**

**C-Vault: XOR Encrypted Password Manager in C**

A secure command line password manager in C featuring encrypted credential storage, role based admin controls and modular file handling with automated user vault management.

## **Overview**

C-Vault is a simple command-line password manager made in C. The project was created to understand how authentication systems, file handling, encryption, and modular programming work in real applications. It allows an admin to create and manage users while securely storing login information using XOR encryption and Base64 encoding.

This project helped in learning concepts like structures, file I/O, dynamic memory allocation, encryption basics, and working with multiple source and header files in C.

## **Features**

XOR-based encrypted password storage

Admin authentication system

Create and manage multiple users

Reset user passwords

Delete users and associated vaults

Individual vault file management

Modular code structure using header and source files

Basic credential protection using Base64 + XOR encryption

Cross-platform terminal screen clearing support

Makefile support for easy compilation

## **How Security Works**

The project uses a simple XOR-based encryption method to protect stored data. Since encrypted data may contain unreadable characters, Base64 encoding is used to convert it into printable text before saving it to files.

## **Project Structure**

├── main.c

├── admin.c

├── admin.h

├── user.c

├── user.h

├── fileio.c

├── fileio.h

├── crypto.c (also a cryptoold.c file also exists ignore it)

├── crypto.h

├── list.c

├── list.h

└── README.md

## **Compilation**

Compile the project using:

***make run***

Run the program:

***./app.exe***

Also to again start the program from scratch remove all the existing *.o files and delete .txt file for admin and user and the vaults directory or folder

## **Default Admin Credentials**

Username: admin

Password: admin123password

## **Concepts Used**

***Language: C***

File Handling

Structures

Dynamic Memory Allocation

Modular Programming

Encryption Concepts

Binary File Operations

Header files and libraries

Base64 encoding and decoding

Authentication systems

## **Limitations**

XOR encryption is not highly secure

Default Admin Credentials and admin cannot change its own password

No password hashing implemented

Command-line based project only

## **Future Improvements**

Add stronger encryption methods

Improve password security

Add hidden password input

Better error handling

Improve file organization

## **AUTHOR**

**Khushi Maan**

CS25B039
