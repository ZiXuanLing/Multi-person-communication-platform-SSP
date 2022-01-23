.SSP: all
all: server client	# all的含义是执行某些命令

server: 1server.c
		gcc -o $@ $^
client: 1client.c
		gcc -o $@ $^
.SSP: clean
clean:
	rm -f server client