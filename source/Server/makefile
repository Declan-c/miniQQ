TARGET ?= server
CC := gcc
CFLAG_O := -lpthread -lsqlite3 -o
HEADERS := -IHeaders
OBJS    := main.o \
		   myepoll.o \
		   tcp_server.o \
		   tpool.o \
		   server_func.o \
		   mysqlite.o \
		   user.o \
		   myfile.o
BIN_PATH := Sources/
vpath %.c $(BIN_PATH)main \
          $(BIN_PATH)myepoll \
		  $(BIN_PATH)tcp_server \
		  $(BIN_PATH)tpool \
		  $(BIN_PATH)server_func \
		  $(BIN_PATH)mysqlite \
		  $(BIN_PATH)user \
		  $(BIN_PATH)myfile

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAG_O) $@

%.o: %.c
	$(CC) -c -lpthread $(HEADERS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS) -rf