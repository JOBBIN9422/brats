#include <utility>
#include <SDL2/SDL.h>
#include "Robot.h"

class World
{
    private:
        std::vector<std::vector<double>> lightMap;
        std::vector<std::pair<int, int>> lightPositions;
        std::vector<Robot*> robots;

        double euclideanDist(int x1, int y1, int x2, int y2); //helper function
        double maxLightVal;

        bool attract; //are the robots attracted to light?

        //rendering
        SDL_Window* mainWindow;
        SDL_Texture* windowTexture;
        SDL_Renderer* renderer;
        uint* lightBuffer;

        //SDL events
        SDL_Event event;

    public:
        World(std::vector<std::pair<int, int>> lightPositions, bool attract);
        std::vector<std::vector<double>> getLightMap();

        void addRobot(int x, int y, int length, int width);
        void addLight(int x, int y);
        void calcLightValues();
        void updateWorld();
        void initVideo();
        void renderLightMap();
        void renderRobot(Robot* robot);
        void setRobotAttraction(bool attract);
        void handleEvents();

};
