CFLAGS+=-I../include/ -Wall -pthread
LDFLAGS += -pthread

all: server 

server: server.o thr_channel.o thr_list.o mytbf.o medialib.o

	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) 

clean: rm *.o server -rf


