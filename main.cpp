#include <iostream>
#include <utility>
#include "World.h"
using namespace std;

int main(int argc, char** argv)
{
    int numRobots = 1;
    int numLights = 1;
    bool attract = true;

    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (string(argv[i]) == "-r")
            {
                numRobots = atoi(argv[i + 1]);
            }

            if (string(argv[i]) == "-l")
            {
                numLights = atoi(argv[i + 1]);
            }

            if (string(argv[i]) == "--attract")
            {
                attract = true;
            }

            if (string(argv[i]) == "--avoid")
            {
                attract = false;
            }
        }
    }
    srand(time(NULL));
    vector<pair<int, int>> lightPositions;
    for (int i = 0; i < numLights; i++)
    {
        pair<int, int> light(rand() % 800, rand() % 600);
        lightPositions.push_back(light);
    } 

    World world(lightPositions, attract);
    for (int i = 0 ; i < numRobots; i++)
    {
        world.addRobot(rand() % 800, rand() % 600, 20, 20);
    }

    //initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL init error: " << SDL_GetError() << endl;
    }
    else
    {
        world.initVideo();
    }

    for (;;)
    {
        world.updateWorld();
        world.renderLightMap();
    }

}
