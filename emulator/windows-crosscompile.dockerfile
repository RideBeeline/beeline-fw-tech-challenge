FROM ubuntu:20.04

RUN apt-get update -y && apt-get -y install mingw-w64 build-essential srecord git