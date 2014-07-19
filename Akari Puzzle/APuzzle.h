#ifndef APUZZLE_H
#define APUZZLE_H
#include"ChessBoard.h"

class APuzzle//玩家
{
public:
	ChessBoard cb;
	int result;
	//回溯法寻解
	APuzzle(ChessBoard &cb)
	{
		this->cb = cb;
		result = 0;
		cout<<"after copy"<<endl;
		//this->cb.printChess();
		cout<<endl;
	}



	//既然答案只有一种，为毛会出现多次重解=这个递归枚举算法有问题，这难道不是白痴的全枚举了吗？


	void findAnswer(int curRow,int i)//第几行，第几个位置（都是下标）
	{
		
		if((curRow == cb.row)) {if(cb.checkRules()){result++;if(result == 1){cb.printChess();cout<<endl;}}}
		else
		{
				for(i; i<cb.col && result == 0; i++)//当前这一行逐个位置试,实在找不到位置，返回到上一层调用
				{
						//if(curRow<2)
						//cout<<"("<<curRow<<","<<i<<")";
						if(cb.setGrid(curRow,i))//成功放灯，即该位置符合放灯的基本要求：白格+没有灯光+四个邻居都不是0黑格+没有灯
						{						//设置成功，继续递归:找下个黑格，或者下一行	
							int k = i+1;
							while(k<cb.col && cb.chessboard[curRow][k]->kind()==0)
							{
								k++;
							}
							if(k<cb.col)
							{	
								findAnswer(curRow,k);
								
							}
							else
								findAnswer(curRow+1,0);
							//很样衰地，找不到解，重置
							cb.resetGrid(curRow,i);
						}
						
						else //设置不成功=黑格||有灯光覆盖||有0号黑格做邻居||本来就有灯
						{	
							
							/*if(cb.chessboard[curRow][i]->kind() == 1)//黑格吗？
							{
								findAnswer(curRow,i);//从此处进入下一轮递归
								break;//回溯到此处，证明黑格不被满足，没必要试下一个，直接跳出循环返回上层调用
							
							}*/

							if(cb.chessboard[curRow][i]->kind()==1)//i是黑格
							{
								if(cb.setGridAroundBlackNum(curRow,i,((BlackSquare*)cb.chessboard[curRow][i])->num))//成功则继续递归
								{
									if((i+1) < cb.col)
										findAnswer(curRow,i+1);
									else
										findAnswer(curRow+1,0);
									//很样衰地，找不到解，重置
									cb.resetNumGrid(curRow,i,((BlackSquare*)cb.chessboard[curRow][i])->num);
									break;//回溯到此处，证明黑格不被满足，没必要试下一个，直接跳出循环返回上层调用
								}
								else
								{
									cb.resetNumGrid(curRow,i,((BlackSquare*)cb.chessboard[curRow][i])->num);
									break;//if通不过，证明黑格不被满足，没必要试下一个，直接跳出循环返回上层调用
								}
								
								//不成功则结束函数返回到上一层调用
							}
						//当前格不是黑格，即：有灯光覆盖||有0号黑格做邻居||有灯
							else if(cb.chessboard[curRow][i]->bulb)//本来就有灯
							{
								int k = i+1;
								while(k<cb.col && cb.chessboard[curRow][k]->kind()==0)
								{
									k++;
								}
								if(k<cb.col)
								{	
									findAnswer(curRow,k);
								
								}
								else
									findAnswer(curRow+1,0);
							}
							
							//有灯光覆盖||有0号黑格做邻居,实在不行了，试下一个位置吧
						}
							
				}
				
				
				if((result == 0) && (cb.chessboard[curRow][i-1]->lightOn>0 || cb.checkNeighIsZero(curRow,i-1)))//由于：有灯光覆盖||有0号黑格做邻居，
																							//使得一直设不了灯，无法递归就含冤爆格，很明显仍需在下一行继续寻解
					findAnswer(curRow+1,0);                                                 //直接递归下一行
																							//为什么说含冤？只是由于棋盘当前布局的约束无法放灯，但不代表这种情形是非法解，因而不能结束			
																							//要作为解答数上的一个结点往下递归，只要不是非法解就要继续递归，黑格不被满足就是非法解，必须回溯

		}
	}

};
#endif
//填好一种情况在将棋盘遍历，检查是否符合要求。