.SSP: all
all: user rela mess photo server 

user: UserManager.cc
	g++ -c UserManager.cc -o user.o
rela: RelationManager.cc
	g++ -c RelationInfo.cc -o relai.o
	g++ -c RelationManager.cc -o relam.o
mess: MessageManager.cc
	g++ -c MessageManager.cc -o mess.o
photo: PhotoManager.cc
	g++ -c PhotoManager.cc -o photo.o
db: DbManager.cc
	g++ -c DbManager.cc -o db.o
server: main.cc
	g++ -o $@ *.o $^
dbt: dbmain.cc
	g++ -c DbManager.cc -o db.o
	g++ db.o dbmain.cc `mysql_config --cflags --libs` -o dbtest -g
.SSP: clean
clean:
	rm -f *.o server