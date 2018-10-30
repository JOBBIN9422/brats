#include <vector>
#include <armadillo>
#include <cmath>

class Robot
{
    private:
        arma::mat kMatrix;
        arma::ivec XYpos;

        int width, length;
        int color;
        int heading;

        std::vector<std::vector<double>>& lightMap;

        int mod(int a, int b);

    public:
        Robot(int x, int y, int length, int width, std::vector<std::vector<double>>& lightMap, bool attract);

        int getHeading();
        int getColor();
        int getLength();
        int getWidth();

        arma::ivec getPos();
        arma::ivec getSensor1Pos();
        arma::ivec getSensor2Pos();

        arma::vec readLightValues();
        arma::vec getWheelValues();

        void updateHeading();
        void move();

        void setAttraction(bool attract);
};
