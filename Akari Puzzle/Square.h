#ifndef SQUARE_H
#define SQUARE_H
#include<iostream>
using namespace std;
class Square
{
public:
	int lightOn;
	bool bulb;
	Square(){}
	virtual int kind(){return -1;}
	virtual void setLightOn(){}
	virtual void setLightOff(){}
	virtual void setBulb(){}
	virtual void resetBulb(){}
	virtual ~Square(){}
	virtual void print(){}
};
#endif