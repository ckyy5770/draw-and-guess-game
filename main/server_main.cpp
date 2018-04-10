//
// Created by chuilian on 3/17/18.
//

#include "../include/server.h"

int main (int argc, const char * argv[]) {
    server srv("127.0.0.1", "5555", "5556", "5557");
    srv.start();
}