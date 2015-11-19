#
# Makefile for ftp-client
# Jakub J. Šimon, 3rd June 2012
#
#Name of the translated executable
executable=ftp-client

#List of files
OBJ=CButtonBar.o CControl.o CError.o CFile.o CFolder.o CFTP.o CGUIItem.o CInputWindow.o CList.o CNetwork.o CPanel.o CProgress.o CQueue.o CSplash.o main.o

#Compiler name
CC=g++

#Flags
CFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb -lpthread -lncurses

#compilation

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(executable)
	doxygen Doxyfile

compile: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(executable)

CButtonBar.o: src/CButtonBar.cpp
	$(CC) $(CFLAGS) -c src/CButtonBar.cpp
CControl.o: src/CControl.cpp 
	$(CC) $(CFLAGS) -c src/CControl.cpp 
CError.o: src/CError.cpp 
	$(CC) $(CFLAGS) -c src/CError.cpp
CFile.o: src/CFile.cpp
	$(CC) $(CFLAGS) -c src/CFile.cpp
CFolder.o: src/CFolder.cpp
	$(CC) $(CFLAGS) -c src/CFolder.cpp 
CFTP.o: src/CFTP.cpp
	$(CC) $(CFLAGS) -c src/CFTP.cpp
CGUIItem.o: src/CGUIItem.cpp
	$(CC) $(CFLAGS) -c src/CGUIItem.cpp
CInputWindow.o: src/CInputWindow.cpp
	$(CC) $(CFLAGS) -c src/CInputWindow.cpp
CList.o: src/CList.cpp
	$(CC) $(CFLAGS) -c src/CList.cpp
CNetwork.o: src/CNetwork.cpp
	$(CC) $(CFLAGS) -c src/CNetwork.cpp 
CPanel.o: src/CPanel.cpp 
	$(CC) $(CFLAGS) -c src/CPanel.cpp
CProgress.o: src/CProgress.cpp
	$(CC) $(CFLAGS) -c src/CProgress.cpp
CQueue.o: src/CQueue.cpp
	$(CC) $(CFLAGS) -c src/CQueue.cpp
CSplash.o: src/CSplash.cpp 
	$(CC) $(CFLAGS) -c src/CSplash.cpp
main.o: 
	$(CC) $(CFLAGS) -c src/main.cpp

clean:
	rm *.o
	rm ftp-client

run: ftp-client
	./ftp-client

