.SSP: all
all: user server 

user: UserManager.cc
	g++ -c UserManager.cc -o user.o
server: main.cc
	g++ -o $@ *.o $^
.SSP: clean
clean:
	rm -f *.o server