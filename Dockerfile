FROM ubuntu:16.04 

MAINTAINER yang 

RUN mkdir Blocks 

ADD Data Blocks 
ADD blocks.cpp Blocks 
ADD blocks Blocks 
ADD glut.dll Blocks 
ADD glut32.dll Blocks 
ADD makefile Blocks 
ADD blocks.exe Blocks 
ADD start.sh Blocks 
ADD virtualgl_2.5.2_amd64.deb Blocks 

RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list 
RUN apt-get clean 
RUN apt-get update 
RUN apt-get install -y apt-utils 
RUN apt-get install -y vim 
RUN echo "Start"
# install sudo for ubuntu 
RUN apt-get update && apt-get -y install sudo 
RUN apt-get update && apt-get install -y libltdl7 
RUN apt-get install -y wget 
RUN apt-get install -y lrzsz 
RUN apt-get install -y gcc 
RUN apt-get install -y curl 
RUN apt-get install -y make 
RUN apt-get install -y build-essential libgl1-mesa-dev 
RUN apt-get install -y freeglut3-dev 
RUN apt-get install -y net-tools 
RUN apt-get install -y openssl 
RUN apt-get install -y libssl-dev 
RUN apt-get install -y zlibc zlib1g-dev 
RUN ls Blocks 

WORKDIR /Blocks
RUN pwd

