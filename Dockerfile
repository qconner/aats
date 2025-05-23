# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Use a home made image as the base, which includes:
# utuntu:latest
# git
# thrift
# maven
# java8
FROM ubuntu:24.04

RUN ["apt", "update"]
RUN apt -y install wget telnet git build-essential libboost-system-dev libboost-thread-dev

# Set the working directory to /src
WORKDIR /src

RUN wget http://artscene.textfiles.com/vt100/bambi.vt

# Copy git repo into the working directory
COPY . /src
#CMD tail -f /dev/null 
RUN g++ -std=c++17  -o vt100_server vt100_server.cpp -I/usr/local/include -L/usr/local/lib -lboost_system -lboost_thread
CMD ./vt100_server 8888 19200 bambi.vt
