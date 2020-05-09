

#mysever
FROM ubuntu:16.04

#Run lib_file 
RUN  apt-get update -y
RUN  apt-get install -y libmysqlcppconn-dev  
RUN  apt-get install -y g++ 
RUN  apt-get install -y vim 
#Move
WORKDIR ./myServer
COPY ./server .

RUN chmod a+x ./service/*

CMD ./service/server


