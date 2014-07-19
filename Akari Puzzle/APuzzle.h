#ifndef APUZZLE_H
#define APUZZLE_H
#include"ChessBoard.h"

class APuzzle//���
{
public:
	ChessBoard cb;
	int result;
	//���ݷ�Ѱ��
	APuzzle(ChessBoard &cb)
	{
		this->cb = cb;
		result = 0;
		cout<<"after copy"<<endl;
		//this->cb.printChess();
		cout<<endl;
	}



	//��Ȼ��ֻ��һ�֣�Ϊë����ֶ���ؽ�=����ݹ�ö���㷨�����⣬���ѵ����ǰ׳յ�ȫö������


	void findAnswer(int curRow,int i)//�ڼ��У��ڼ���λ�ã������±꣩
	{
		
		if((curRow == cb.row)) {if(cb.checkRules()){result++;if(result == 1){cb.printChess();cout<<endl;}}}
		else
		{
				for(i; i<cb.col && result == 0; i++)//��ǰ��һ�����λ����,ʵ���Ҳ���λ�ã����ص���һ�����
				{
						//if(curRow<2)
						//cout<<"("<<curRow<<","<<i<<")";
						if(cb.setGrid(curRow,i))//�ɹ��ŵƣ�����λ�÷��ϷŵƵĻ���Ҫ�󣺰׸�+û�еƹ�+�ĸ��ھӶ�����0�ڸ�+û�е�
						{						//���óɹ��������ݹ�:���¸��ڸ񣬻�����һ��	
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
							//����˥�أ��Ҳ����⣬����
							cb.resetGrid(curRow,i);
						}
						
						else //���ò��ɹ�=�ڸ�||�еƹ⸲��||��0�źڸ����ھ�||�������е�
						{	
							
							/*if(cb.chessboard[curRow][i]->kind() == 1)//�ڸ���
							{
								findAnswer(curRow,i);//�Ӵ˴�������һ�ֵݹ�
								break;//���ݵ��˴���֤���ڸ񲻱����㣬û��Ҫ����һ����ֱ������ѭ�������ϲ����
							
							}*/

							if(cb.chessboard[curRow][i]->kind()==1)//i�Ǻڸ�
							{
								if(cb.setGridAroundBlackNum(curRow,i,((BlackSquare*)cb.chessboard[curRow][i])->num))//�ɹ�������ݹ�
								{
									if((i+1) < cb.col)
										findAnswer(curRow,i+1);
									else
										findAnswer(curRow+1,0);
									//����˥�أ��Ҳ����⣬����
									cb.resetNumGrid(curRow,i,((BlackSquare*)cb.chessboard[curRow][i])->num);
									break;//���ݵ��˴���֤���ڸ񲻱����㣬û��Ҫ����һ����ֱ������ѭ�������ϲ����
								}
								else
								{
									cb.resetNumGrid(curRow,i,((BlackSquare*)cb.chessboard[curRow][i])->num);
									break;//ifͨ������֤���ڸ񲻱����㣬û��Ҫ����һ����ֱ������ѭ�������ϲ����
								}
								
								//���ɹ�������������ص���һ�����
							}
						//��ǰ���Ǻڸ񣬼����еƹ⸲��||��0�źڸ����ھ�||�е�
							else if(cb.chessboard[curRow][i]->bulb)//�������е�
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
							
							//�еƹ⸲��||��0�źڸ����ھ�,ʵ�ڲ����ˣ�����һ��λ�ð�
						}
							
				}
				
				
				if((result == 0) && (cb.chessboard[curRow][i-1]->lightOn>0 || cb.checkNeighIsZero(curRow,i-1)))//���ڣ��еƹ⸲��||��0�źڸ����ھӣ�
																							//ʹ��һֱ�費�˵ƣ��޷��ݹ�ͺ�ԩ���񣬺�������������һ�м���Ѱ��
					findAnswer(curRow+1,0);                                                 //ֱ�ӵݹ���һ��
																							//Ϊʲô˵��ԩ��ֻ���������̵�ǰ���ֵ�Լ���޷��ŵƣ������������������ǷǷ��⣬������ܽ���			
																							//Ҫ��Ϊ������ϵ�һ��������µݹ飬ֻҪ���ǷǷ����Ҫ�����ݹ飬�ڸ񲻱�������ǷǷ��⣬�������

		}
	}

};
#endif
//���һ������ڽ����̱���������Ƿ����Ҫ��