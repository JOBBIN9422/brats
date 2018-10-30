#include "World.h"
using namespace std;

World::World(vector<pair<int, int>> lightPositions, bool attract)
: lightPositions(lightPositions), maxLightVal(100), lightMap(600, vector<double>(800, 0.0)), attract(attract)
{
    this->calcLightValues();
}

double World::euclideanDist(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

vector<vector<double>> World::getLightMap()
{
    return this->lightMap;    
}


void World::addRobot(int x, int y, int length, int width)
{
    this->robots.push_back(new Robot(x, y, length, width, this->lightMap, attract));
}

void World::addLight(int x, int y)
{
    this->lightPositions.push_back(make_pair(x, y));
    this->calcLightValues();
}

void World::calcLightValues()
{
    for (int i = 0; i < 600; i++)
    {
        vector<double> currRow;

        for (int j = 0; j < 800; j++)
        {
            double currLightIntensity = 0.0;
            for (int k = 0; k < lightPositions.size(); k++)
            {
                double distance = euclideanDist(lightPositions.at(k).first, 
                lightPositions.at(k).second, j, i);
                currLightIntensity += 100 / (distance);
            }
            if (currLightIntensity > 100.0)
            {
                currLightIntensity = 100.0;
            }
            this->lightMap.at(i).at(j) = currLightIntensity;
        }
    }
}

void World::updateWorld()
{
    this->handleEvents();
    for (int i = 0; i < robots.size(); i++)
    {
        robots.at(i)->move();
    }
}

void World::initVideo()
{
    this->mainWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, 800, 600, 
                                        SDL_WINDOW_SHOWN);
    this->renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_SOFTWARE);
    this->windowTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STATIC, 800, 600);
    this->lightBuffer = new uint[600 * 800];
}

void World::renderLightMap()
{
    for (int y = 0; y < lightMap.size(); y++)
    {
        for (int x = 0; x < lightMap.at(y).size(); x++)
        {
            unsigned char lightVal = round(lightMap.at(y).at(x) / maxLightVal * 0xff);
            lightBuffer[800 * y + x] = (0xff000000 | (lightVal << 16)  | (lightVal << 8) | lightVal);
        }
    }

    for (int i = 0; i < robots.size(); i++)
    {
        this->renderRobot(robots.at(i));
    }

    //render lightmap texture to window
    SDL_UpdateTexture(windowTexture, NULL, lightBuffer, 800 * sizeof(uint));
    //SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, windowTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void World::renderRobot(Robot* robot)
{
        int randColor = robot->getColor();
        arma::ivec XYpos = robot->getPos();
        this->lightBuffer[800 * XYpos(1) + XYpos(0)] = randColor;
        this->lightBuffer[800 * XYpos(1) + (XYpos(0)+1)] = randColor;
        this->lightBuffer[800 * XYpos(1) + (XYpos(0)-1)] = randColor;
        this->lightBuffer[800 * (XYpos(1)+1) + XYpos(0)] = randColor;
        this->lightBuffer[800 * (XYpos(1)-1) + XYpos(0)] = randColor;

        arma::ivec sensor1pos = robot->getSensor1Pos();
        arma::ivec sensor2pos = robot->getSensor2Pos();

        this->lightBuffer[800 * sensor1pos(1) + sensor1pos(0)] = 0xffffffff;
        this->lightBuffer[800 * sensor2pos(1) + sensor2pos(0)] = 0xffffffff;
}

void World::setRobotAttraction(bool attract)
{
    for (int i = 0; i < this->robots.size(); i++)
    {
        this->robots.at(i)->setAttraction(attract);
    }
}

void World::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button)
            {
                case SDL_BUTTON_LEFT:
                    this->addRobot(event.button.x, event.button.y,
                                   this->robots.at(0)->getLength(),
                                   this->robots.at(0)->getWidth());
                    this->calcLightValues();
                    break;
                case SDL_BUTTON_RIGHT:
                    this->addLight(event.button.x, event.button.y);
                    break;
            }
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_SPACE:
                    this->attract = !this->attract;
                    cout << "toggling robot attraction to " << this->attract << "\n";
                    for (int i = 0; i < this->robots.size(); i++)
                    {
                        robots.at(i)->setAttraction(this->attract);
                    }
                    break;
                
                case SDLK_ESCAPE:
                    SDL_Quit();
                    break;
            }
            break;
    }
}
