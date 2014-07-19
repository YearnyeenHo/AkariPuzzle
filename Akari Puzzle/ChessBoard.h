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
	Square * ** chessboard;//����ָ�������
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
			{//��ʼ��һ��
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
			
			chessboard[j] = tempRow;//����ʼ�����һ����ӵ����̣�����ѭ��ֱ��row�ж���ʼ��
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
	bool checkNeighIsZero(int i,int j)//i,j���ھ����Ƿ���0�ڸ�����еĻ����˴���Ӧ���õ��ݣ���ʹ�пհס�
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
	//��ӡ���

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

	//----���������ø�㣨�ܵ�ǰ���̵����Ӱ�죩----

	bool setGrid(int i,int j)//�Ե��еĸ��ӽ������ã���Ϸ���Ƕ����Էŵƣ�ֻ�Ǻڸ�Ϊ���壬�׸�������䡣�ŵƵ�ͬʱ���ܷ���ĸ��ӽ������á�
	{
	
		if(checkBorder(i,j))
		{	
			Square * temp = chessboard[i][j];

			if(temp->kind() == 0 && temp->lightOn == 0 && !checkNeighIsZero(i,j))//�׸�+û�еƹ�+�ĸ��ھӶ�����0�ڸ�=���Էŵơ���ȷ���������Ȳ��ܻ��ա�
			{
				temp->setBulb();//�����
				int k = 1;
				//��ֱ�������õƹ�
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
				//ˮƽ�������õƹ�
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
	//���������ֺڸ���ھӸ���
	void resetNumGrid(int i,int j,int num)
	{
		if(num<=4 && num>=0)
		{
			int counter = 0;
			if(checkBorder(i,j))
			{
				//��ͼ��˳ʱ��˳���ںڸ������ܳ������ݣ����ݵ�ǰ�������������װ����ĵƸ���
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
	{//����i��jλ�õĵ��ݣ������������������и���e�ķ��������1
		
		if(checkBorder(i,j) && chessboard[i][j]->bulb)//�еƲų���
		{
			Square * temp = chessboard[i][j];
			if(temp->kind() == 0 && temp->lightOn > 0)//�׸�+û�еƹ�=���Էŵơ���ȷ���������Ȳ��ܻ��ա�
			{
				temp->resetBulb();//�����
				int k = 1;
				//��ֱ�������õƹ�
				while((k + i)<row && chessboard[i+k][j]->kind() != 1 )
				{
					chessboard[i+k][j]->setLightOff();//����ֱ�Ϸ������кڸ���ǰ�ĸ��ӷ��������һ
					k++;
				}
				k = 1;
				while((i-k)>=0 && chessboard[i-k][j]->kind() != 1 )
				{
					chessboard[i-k][j]->setLightOff();
					k++;
				}
				k = 1;	
				//ˮƽ�������õƹ�
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

	//--������ս���Ƿ������Ϸ����--

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
	
	//----���ڸ���Χ�Ƿ��з���Ҫ��ĵ���-----
	bool checkBlackGrid(int i,int j,int num)
	{	if(checkBorder(i,j))
		{
			BlackSquare * temp = (BlackSquare * )chessboard[i][j];
			int counter = checkNeighBulbNum(i,j);
			switch(temp->num)
			{	
				

				case 0 ://���ܲ����е� 
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
						
				default:	return true;//�ڸ�û0-4���֣���ô���У�
			}
		}
		return true;
	
	}

	
	bool checkRules()//check���̵����ս��
	{
		//��������ǣ��׸񴦶��еƹ⣬�Һڸ񴦣�����0-4�����֣����úڸ�4���ھ��Ƿ񹹳���Ӧ�����ĵ�
		//�ӵ�һ����ʼ��鵽���һ��������
		//����ǰ׸��Ƿ��еƹ⣿
		//�����Ǻڸ���[0,4]���������У��������򷽸񣬼���������������
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
					if(!checkBlackGrid(i,j,num))//�����Ϻڸ�����
						return false;
				}

			}
	    }
		return true;
	
	}

//---�ںڸ���Χ����Ҫ����Ŀ�ĵƣ��������óɹ����---
bool setGridAroundBlackNum(int i,int j,int num)
{
	if(num<=4 && num>=0)
	{
		int counter = 0;
		if(checkBorder(i,j))
		{
			//��ͼ��˳ʱ��˳���ںڸ����������õ��ݣ����ݵ�ǰ�������������װ����ĵƸ���
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
	return true;//ɶҲ����
}
};
#endif 