#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include<ctime>
#include<cstdlib>
#include"Square.h"
#include"BlackSquare.h"
#include"WhiteSquare.h"
#include<fstream>

class ChessBoard
{
public:
	int row,col;//n row,m col
	Square * ** chessboard;//放满指针的棋盘
	ChessBoard()
	{
		row = 0;
		col = 0;
		chessboard = NULL;
	
	}
	ChessBoard& operator= (ChessBoard cb)
	{
		int i=0,j=0;
		for(i; i<row; i++)
		{
			for(j; j<col; j++)
			delete chessboard[i][j];

			delete [] chessboard[i];
		}
		delete [] chessboard;

		row = cb.row;
		col = cb.col;
		//chessboard = cb.chessboard;
		chessboard = new Square**[row];
	
		for(int i = 0; i<cb.row; i++)
		{
			Square ** temp = new Square *[col];
			for(int j = 0; j<cb.col; j++)
			{
				if(cb.chessboard[i][j]->kind() == 0)
				{
					
					temp[j] = new WhiteSquare;//((WhiteSquare*)cb.chessboard[i][j]);
				}
				else
				{
					temp[j] = new BlackSquare(((BlackSquare*)cb.chessboard[i][j])->num);
				}
				
			}
			chessboard[i] = temp;
		}
		return *this;
	}
	ChessBoard(ChessBoard & cb)
	{
		row = cb.row;
		col = cb.col;
	
		chessboard = new Square**[row];
	
		for(int i = 0; i<cb.row; i++)
		{
			Square ** temp = new Square *[col];
			for(int j = 0; j<cb.col; j++)
			{
				if(cb.chessboard[i][j]->kind() == 0)
				{
					
					temp[j] = new WhiteSquare;//((WhiteSquare*)cb.chessboard[i][j]);
				}
				else
				{
					temp[j] = new BlackSquare(((BlackSquare*)cb.chessboard[i][j])->num);
				}
				
			}
			chessboard[i] = temp;
		}
		
		
	}
	
	ChessBoard(int n,int m)
	{	
		fstream file("data.txt",ios::in);
		row = n,col = m;
		chessboard = new Square**[row];

		srand((unsigned)time(NULL));
		
		
		
		cout<<"input an 8x8 matrix,input \"5\" means empty, or integer [0 ,4]"<<endl;
		
		for(int j = 0; j < row; j++)
		{
			Square ** tempRow = new Square *[col];
			
			for(int i = 0; i < col; i++)
			{//初始化一行
				int t;
				
				file>>t;
				cout<<t<<" ";
				if(t == 5)
					{
						
						tempRow[i] = new WhiteSquare;
					}
				else
					if(t>=0)
					{
						
						tempRow[i] = new BlackSquare(t);
					}
				
			}
			cout<<endl;
			
			chessboard[j] = tempRow;//将初始化后的一行添加到棋盘，继续循环直到row行都初始化
		}
		file.close();
		cout<<endl;
	}
	~ChessBoard()
	{
		int i=0,j=0;
		for(i; i<row; i++)
		{
			for(j; j<col; j++)
			delete chessboard[i][j];

			delete [] chessboard[i];
		}
		delete [] chessboard;
	}

	bool checkRowBorder(int i)
	{
		if(i>=0 && i<row)
			return true;
			return false;
	};
	bool checkColBorder(int j)
	{
		if(j>=0 && j<col)
			return true;
		return false;
	}
	bool checkBorder(int i,int j)
	{
		if(checkRowBorder(i) && checkColBorder(j))
			return true;
		else
			return false;
	}
	bool checkNeighIsZero(int i,int j)//i,j的邻居中是否有0黑格？如果有的话，此处不应设置灯泡，即使有空白。
	{
		int k = 1;
		if(!checkBorder(i,j))return false;
		if(checkRowBorder(i+k) && chessboard[i+k][j]->kind() ==1)
			if(((BlackSquare*)chessboard[i+k][j])->num == 0)
		    return true;
		if(checkRowBorder(i-k) && chessboard[i-k][j]->kind() ==1) 
			if(((BlackSquare*)chessboard[i-k][j])->num == 0)
		   return true;
		if(checkColBorder(j+k) && chessboard[i][j+k]->kind() ==1)
			if(((BlackSquare*)chessboard[i][j+k])->num == 0)
	       return true;
		if(checkColBorder(j-k) && chessboard[i][j-k]->kind() ==1)
			if(((BlackSquare*)chessboard[i][j-k])->num == 0)
			return true;
		return false;
	}
	//打印结果

	void printChess()
	{
		
		for(int i = 0; i<row; i++)
		{
			for(int j = 0; j<col; j++)
			{
				chessboard[i][j]->print();
				cout<<" ";
			}
			cout<<endl;
		
		}
	}

	//----按规则设置格点（受当前棋盘的情况影响）----

	bool setGrid(int i,int j)//对点中的格子进行设置，游戏中是都可以放灯，只是黑格为黑体，白格向外辐射。放灯的同时对受辐射的格子进行设置。
	{
	
		if(checkBorder(i,j))
		{	
			Square * temp = chessboard[i][j];

			if(temp->kind() == 0 && temp->lightOn == 0 && !checkNeighIsZero(i,j))//白格+没有灯光+四个邻居都不是0黑格=可以放灯【以确保了两个等不能互照】
			{
				temp->setBulb();//设个灯
				int k = 1;
				//垂直方向设置灯光
				while((k + i)<row && chessboard[i+k][j]->kind() != 1 )
				{
					chessboard[i+k][j]->setLightOn();
					k++;
				}
				k = 1;
				while((i-k)>=0 && chessboard[i-k][j]->kind() != 1 )
				{
					chessboard[i-k][j]->setLightOn();
					k++;
				}
				k = 1;	
				//水平方向设置灯光
				while((k + j)<col && chessboard[i][j+k]->kind() != 1 )
				{
					chessboard[i][j+k]->setLightOn();
					k++;
				}
				k = 1;
				while((j-k)>=0 && chessboard[i][j-k]->kind() != 1 )
				{
					chessboard[i][j-k]->setLightOn();
					k++;
				}
				return true;
			}

		}
		return false;
	}
	//重置有数字黑格的邻居格子
	void resetNumGrid(int i,int j,int num)
	{
		if(num<=4 && num>=0)
		{
			int counter = 0;
			if(checkBorder(i,j))
			{
				//试图按顺时针顺序在黑格子四周撤销灯泡，根据当前的棋盘情况，安装所需的灯个数
				if(checkColBorder(j-1) && counter<num)
				{
					if(resetGrid(i,j-1))
						counter++;
				}
				if(checkRowBorder(i+1) && counter<num)
				{
					if(resetGrid(i+1,j))
						counter++;
				}
				if(checkColBorder(j+1) && counter<num)
				{
					if(resetGrid(i,j+1))
						counter++;
				}
				if(checkRowBorder(i-1) && counter<num)
				{
					if(resetGrid(i-1,j))
						counter++;
				}
			
			}
		}
	}
	bool resetGrid(int i,int j)
	{//撤销i，j位置的灯泡，并将其辐射区域的所有格子e的辐射计数减1
		
		if(checkBorder(i,j) && chessboard[i][j]->bulb)//有灯才撤销
		{
			Square * temp = chessboard[i][j];
			if(temp->kind() == 0 && temp->lightOn > 0)//白格+没有灯光=可以放灯【以确保了两个等不能互照】
			{
				temp->resetBulb();//设个灯
				int k = 1;
				//垂直方向设置灯光
				while((k + i)<row && chessboard[i+k][j]->kind() != 1 )
				{
					chessboard[i+k][j]->setLightOff();//将垂直上方的所有黑格子前的格子辐射计数减一
					k++;
				}
				k = 1;
				while((i-k)>=0 && chessboard[i-k][j]->kind() != 1 )
				{
					chessboard[i-k][j]->setLightOff();
					k++;
				}
				k = 1;	
				//水平方向设置灯光
				while((k + j)<col && chessboard[i][j+k]->kind() != 1 )
				{
					chessboard[i][j+k]->setLightOff();
					k++;
				}
				k = 1;
				while((j-k)>=0 && chessboard[i][j-k]->kind() != 1 )
				{
					chessboard[i][j-k]->setLightOff();
					k++;
				}
			}
			return true;
		}
	return false;
	
	}

	//--检查最终结果是否符合游戏规则--

	int checkNeighBulbNum(int i,int j)
	{
		int counter = 0,k = 1;
		if(checkRowBorder(i+k) && chessboard[i+k][j]->bulb)
		    counter++;
		if(checkRowBorder(i-k) && chessboard[i-k][j]->bulb) 
		    counter++;
		if(checkColBorder(j+k) && chessboard[i][j+k]->bulb)
	        counter++;
		if(checkColBorder(j-k) && chessboard[i][j-k]->bulb)
			counter++;
		return counter;
	
	}
	
	//----检查黑格周围是否有符合要求的灯数-----
	bool checkBlackGrid(int i,int j,int num)
	{	if(checkBorder(i,j))
		{
			BlackSquare * temp = (BlackSquare * )chessboard[i][j];
			int counter = checkNeighBulbNum(i,j);
			switch(temp->num)
			{	
				

				case 0 ://四周不能有灯 
					if(counter == 0)
						return true;
					else
						return false;

				case 1 :
					if(counter == 1)
						return true;
					return false;
					
				case 2 : 
					if(counter == 2)
						return true;
					return false;
						
				case 3 : 
					if(counter == 3)
						return true;
					return false;
				case 4 : 
					if(counter == 4)
						return true;
					return false;
						
				default:	return true;//黑格没0-4数字，怎么都行！
			}
		}
		return true;
	
	}

	
	bool checkRules()//check棋盘的最终结果
	{
		//美满结局是：白格处都有灯光，且黑格处，若有0-4的数字，检查该黑格4个邻居是否构成相应个数的灯
		//从第一个开始检查到最后一个，遍历
		//如果是白格，是否有灯光？
		//若果是黑格，有[0,4]的数字吗？有，则检查邻域方格，计数，符合数字吗？
		for(int i = 0; i<row ;i++)
		{
			for(int j = 0; j<col; j++)
			{
				if(chessboard[i][j]->kind() == 0 )//White
				{
					if(chessboard[i][j]->lightOn == 0 )//White && lightOn==false
					{
						return false;
					}
				
				}
				else//Black
				{
					int num = ((BlackSquare *)chessboard[i][j])->num;
					if(!checkBlackGrid(i,j,num))//不符合黑格的情况
						return false;
				}

			}
	    }
		return true;
	
	}

//---在黑格周围设置要求数目的灯，返回设置成功与否---
bool setGridAroundBlackNum(int i,int j,int num)
{
	if(num<=4 && num>=0)
	{
		int counter = 0;
		if(checkBorder(i,j))
		{
			//试图按顺时针顺序在黑格子四周设置灯泡，根据当前的棋盘情况，安装所需的灯个数
			if(checkRowBorder(i-1) && counter<num)
			{
				if(setGrid(i-1,j))
				{
					counter++;}
				else
					if(chessboard[i-1][j]->bulb)
						counter++;
			}
			if(checkColBorder(j-1) && counter<num)
			{
				if(setGrid(i,j-1))
					counter++;
				else
					if(chessboard[i][j-1]->bulb)
						counter++;
			}
			
			if(checkColBorder(j+1) && counter<num)
			{
				if(setGrid(i,j+1))
					counter++;
				else
					if(chessboard[i][j+1]->bulb)
						counter++;
			}
			if(checkRowBorder(i+1) && counter<num)
			{
				if(setGrid(i+1,j))
					counter++;
				else
					if(chessboard[i+1][j])
						counter++;
			}
			if(counter == num)
				return true;
			else
				{
					resetNumGrid(i,j,num);
					return false;
				}
		}
	}
	return true;//啥也不干
}
};
#endif 