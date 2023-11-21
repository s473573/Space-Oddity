//
//  main.cpp
//  space oddity
//
//  Created by Edward Nobody on 17.11.2023.
//

#include "Game.h"
using namespace std;
using namespace so;

int
main(int argc, char** argv)
{
    try {
        if (GAME.init())
            GAME.loop();
        return 0;
    } catch (const exception &err) {
        printf("An exception has occured: %s", err.what());
    }
}
