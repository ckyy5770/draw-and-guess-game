//
// Created by chuilian on 3/17/18.
//
#include "../include/client.h"


int main (int argc, const char * argv[]) {
    client cli("127.0.0.1", "5555", "5556", "5557");
    cli.start();
}