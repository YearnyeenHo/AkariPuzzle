#ifndef WHITESQUARE_H
#define WHITESQUARE_H

#include"Square.h"

class WhiteSquare: public Square
{
	public:
		
		
		WhiteSquare()
		{
			lightOn = 0;
			bulb = false;
		}
		WhiteSquare(WhiteSquare* ws)
		{
			lightOn = ws->lightOn;
			bulb = ws->bulb;
		}
		void print()
		{
			if(bulb)
				cout<<"@";//you
			else
				if(lightOn>0)
				cout<<"+";//代表有灯光
				else
					cout<<"-";
		}
		void setBulb()
		{
			bulb = true;
			setLightOn();
		}

		void resetBulb()
		{
			bulb = false;
			setLightOff();
		}

		void setLightOn()
		{
			lightOn++;
		}
		void setLightOff()
		{
			if(lightOn>0)
			lightOn--;
		
		}
		int kind()
		{
			return 0;
		}
};
#endif