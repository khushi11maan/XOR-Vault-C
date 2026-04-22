# Makefile Secure Password Manager
# Usage:make builds the program (app.exe) and clean deletes app.exe and .o files
CFLAGS = -Wall -std=c99
SRCS = main.c crypto.c list.c fileio.c admin.c user.c # All source files
OBJS = main.o crypto.o list.o fileio.o admin.o user.o # Object files generated from each .c file

TARGET = app.exe
run: $(TARGET)
 
$(TARGET): $(OBJS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJS)
	@echo "Build successful!! Now run with: ./app.exe"
 
main.o:   main.c
	gcc $(CFLAGS) -c main.c
 
crypto.o: crypto.c crypto.h
	gcc $(CFLAGS) -c crypto.c
 
list.o:   list.c list.h
	gcc $(CFLAGS) -c list.c
 
fileio.o: fileio.c fileio.h
	gcc $(CFLAGS) -c fileio.c
 
admin.o:  admin.c admin.h
	gcc $(CFLAGS) -c admin.c
 
user.o:   user.c user.h
	gcc $(CFLAGS) -c user.c
 
rmove: 
	rm *.o $(TARGET)
 
