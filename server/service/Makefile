INC = -I../include
LIB = -lmysqlcppconn
STD = -std=c++11
sv: sock_connect.o data.o main.o user.o
	g++ server.o data.o main.o user.o -o sv

user.o: ../models/user.cpp
	g++ $(STD) -c ../models/user.cpp
server.o: sock_connect.cpp ../models/user.cpp
	g++ $(STD) -c sock_connect.cpp ../models/user.cpp  -lmysqlcppconn $(INC)

data.o: databaseAccess.cpp ../models/Room.cpp ../responsibility/MySQLAccess.cpp ../responsibility/roomsDA.cpp ../responsibility/usersDA.cpp ../models/user.cpp main.cpp
	g++ $(STD) databaseAccess.cpp ../models/Room.cpp ../models/user.cpp ../responsibility/MySQLAccess.cpp ../responsibility/roomsDA.cpp ../responsibility/usersDA.cpp main.cpp -lmysqlcppconn $(INC) 

