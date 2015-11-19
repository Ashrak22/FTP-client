/**
 * @file main.cpp
 * @brief This is main file for the FTP client, it inits CControl (the control Class) starts login procedure and then starts the main loop.
 * @author Jakub J Simon
 *
 * @date 15/5/2012
 */

#include <cstdlib>
#include "CControl.h"
#include <iostream>
#include <iomanip>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    CControl mainLoop;
    mainLoop.init();
    mainLoop.login();
    mainLoop.run();
    return 0;
}

