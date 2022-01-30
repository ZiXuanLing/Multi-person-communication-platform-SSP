.SSP:all 

# protobuf
INC_PROTOBUF = -I/usr/local/include/google/protobuf
LIB_PROTOBUF = -L/usr/local/lib/libprotobuf.a -lprotobuf
OBJ_SSP = proto/*.o

# mysql
LIB_MYSQL = -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto

all:user rela mess photo svr cln

user:UserManager.cc
	g++ -c -o user.o $^

rela:RelationManager.cc
	g++ -c -o relaI.o RelationInfo.cc
	g++ -c -o realM.o RelationManager.cc

mess:MessageManager.cc
	g++ -c -o mess.o $^

photo:PhotoManager.cc
	g++ -c -o photo.o $^

db:DbManager.cc
	g++ -c -o db.o $^

svr:main.cc UserManager.cc RelationManager.cc RelationInfo.cc MessageManager.cc PhotoManager.cc DbManager.cc 
	g++ -g -o server_ssp  $^ $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL) $(OBJ_SSP) -D _D

dbt:dbmain.cc
	g++ -c -o db.o DbManager.cc
	g++ -o dbtest db.o $(INC_PROTOBUF) $(LIB_PROTOBUF) $(LIB_MYSQL) $(OBJ_SSP) $^

cln:client.cc
	g++ -g -o client_ssp  $^ $(LIB_PROTOBUF) $(OBJ_SSP)

.SSP:clean
clean:
	rm -f *.o server_ssp client_ssp

