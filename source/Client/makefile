TARGET ?= client
CC := gcc
CFLAG_O := -lpthread -o
HEADERS := -IHeaders
OBJS    := client_func.o \
            main.o \
			tcp_client.o \
			myfile.o
vpath %.c Sources/client_func \
          Sources/main \
		  Sources/tcp_client \
		  Sources/myfile

$(TARGET): $(OBJS)
	$(CC) $^ $(CFLAG_O) $@

%.o: %.c
	$(CC) -c -lpthread $(HEADERS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGET) $(OBJS) -rf