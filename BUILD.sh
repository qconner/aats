#!/bin/bash
set -x
#g++ -std=c++11 -stdlib=libc++ -o vt100 vt100.cpp -I/usr/local/include -L/usr/local/lib -lboost_system -lboost_thread-mt
g++ -std=c++11 -stdlib=libc++ -o vt100_server vt100_server.cpp -I/usr/local/include -L/usr/local/lib -lboost_system -lboost_thread-mt

