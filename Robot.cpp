#include <vector>
#include "Robot.h"
using namespace std;

Robot::Robot(int x, int y, int length, int width, vector<vector<double>>& lightMap, bool attract) 
: length(length), width(width), heading(0), 
kMatrix(2,2, arma::fill::zeros), lightMap(lightMap)
{
    this->XYpos = {x, y};
    this->setAttraction(attract);
    this->color = (0xff000000) | (rand() % 0xffffff & 0xffffff);
}

int Robot::getLength()
{
    return this->length;
}

int Robot::getWidth()
{
    return this->width;
}

int Robot::getColor()
{
    return this->color;
}
int Robot::getHeading()
{
    return this->heading;
}

arma::ivec Robot::getPos()
{
    return this->XYpos;
}

arma::ivec Robot::getSensor1Pos()
{
    //calculate XY components of vector with width / 2 magnitude
    int widthVecX = round((this->width / 2) * cos(this->heading * 3.141593 / 180.0));
    int widthVecY = round((this->width / 2) * sin(this->heading * 3.141593 / 180.0));

    int lengthVecX = round((this->length / 2) * cos(this->heading * 3.141593 / 180.0));
    int lengthVecY = round((this->length / 2) * sin(this->heading * 3.141593 / 180.0));
    
    //rotate counter-clockwise by 90 degrees
    arma::imat rotateMatrix(2,2, arma::fill::zeros);
    rotateMatrix(0, 0) = 0;
    rotateMatrix(0, 1) = -1;
    rotateMatrix(1, 0) = 1;
    rotateMatrix(1, 1) = 0;

    arma::ivec componentVec = {widthVecX, widthVecY};
    arma::ivec lengthVec = {lengthVecX, lengthVecY};
    arma::ivec sensorPos = rotateMatrix * componentVec + this->XYpos + lengthVec;

    sensorPos(0) = mod(sensorPos(0), this->lightMap.at(0).size());
    sensorPos(1) = mod(sensorPos(1), this->lightMap.size());

    return sensorPos;
}

arma::ivec Robot::getSensor2Pos()
{
    //calculate XY components of vector with width / 2 magnitude
    int widthVecX = round((this->width / 2) * cos(this->heading * 3.141593 / 180.0));
    int widthVecY = round((this->width / 2) * sin(this->heading * 3.141593 / 180.0));

    int lengthVecX = round((this->length / 2) * cos(this->heading * 3.141593 / 180.0));
    int lengthVecY = round((this->length / 2) * sin(this->heading * 3.141593 / 180.0));
    
    //rotate counter-clockwise by 90 degrees
    arma::imat rotateMatrix(2,2, arma::fill::zeros);
    rotateMatrix(0, 0) = 0;
    rotateMatrix(0, 1) = 1;
    rotateMatrix(1, 0) = -1;
    rotateMatrix(1, 1) = 0;

    arma::ivec componentVec = {widthVecX, widthVecY};
    arma::ivec lengthVec = {lengthVecX, lengthVecY};
    arma::ivec sensorPos = rotateMatrix * componentVec + this->XYpos + lengthVec;

    sensorPos(0) = mod(sensorPos(0), this->lightMap.at(0).size());
    sensorPos(1) = mod(sensorPos(1), this->lightMap.size());

    return sensorPos;
}

arma::vec Robot::readLightValues()
{
    //get sensor positions
    arma::ivec sensor1pos = this->getSensor1Pos();
    arma::ivec sensor2pos = this->getSensor2Pos();
    
    double lightVal1 = this->lightMap.at(sensor1pos(1)).at(sensor1pos(0));
    double lightVal2 = this->lightMap.at(sensor2pos(1)).at(sensor2pos(0));

    arma::vec sensorReadings = {lightVal1, lightVal2};
    return sensorReadings;
}

arma::vec Robot::getWheelValues()
{
    //calculate wheel strength (k-matrix * sensor reading vector)
    arma::vec sensorReadings = this->readLightValues();
    arma::vec wheelValues = this->kMatrix * sensorReadings;
    return wheelValues;
}

void Robot::updateHeading()
{
    arma::vec wheelValues = this->getWheelValues();

    double difference = (wheelValues(0) - wheelValues(1));

    this->heading -= (difference * 10);
    this->heading = mod(this->heading, 360);
}

void Robot::move()
{
    this->updateHeading();

    arma::vec wheelValues = this->getWheelValues();
    double magnitude = 5;
    if (magnitude > 5)
    {
        magnitude = 5;
    }

    int deltaX = magnitude * cos(this->heading * 3.141593 / 180.0);
    int deltaY = magnitude * sin(this->heading * 3.141593 / 180.0);

    this->XYpos(0) += deltaX;
    this->XYpos(1) += deltaY;

    this->XYpos(0) = mod(this->XYpos(0), this->lightMap.at(0).size());
    this->XYpos(1) = mod(this->XYpos(1), this->lightMap.size());
}

int Robot::mod(int a, int b)
{
    return ((a % b) + b) % b;
}

void Robot::setAttraction(bool attract)
{
    if (attract)
    {
        this->kMatrix(0, 0) = 0;
        this->kMatrix(0, 1) = 1;
        this->kMatrix(1, 0) = 1;
        this->kMatrix(1, 1) = 0;
    }
    else
    {
        this->kMatrix(0, 0) = 1;
        this->kMatrix(0, 1) = 0;
        this->kMatrix(1, 0) = 0;
        this->kMatrix(1, 1) = 1;
    }
}
