#ifndef BLACKSQUARE_H
#define BLACKSQUARE_H
#include"Square.h"
#include<ctime>
#include<cstdlib>

class BlackSquare : public Square
{
public:
	int num;
	BlackSquare(int t)
	{
		num = t;
		lightOn = 0;
		bulb = false;
	}
	BlackSquare(BlackSquare* bs)
	{
		num = bs->num;
		lightOn = bs->lightOn;
		bulb = bs->bulb;
	}
	void print()
	{
		if(num>=0 && num<=4)
		cout<<num;
		else
		cout<<"x";
	}
	void setBulb(){}
	int kind()
	{
		return 1;
	}

};
#endif