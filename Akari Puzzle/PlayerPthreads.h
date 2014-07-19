#ifndef PLAYERPTHREAD_H
#define PLAYERPTHREAD_H
#include<pthread.h>
#include"APuzzle.h"
#include"ChessBoard.h"

#pragma comment(lib, "pthreadVC2.lib")


pthread_t* threadHandles;
int threadCount;
ChessBoard chessBoard;
volatile int gResult = 0;//通常我们会在这个全局变量前加上volatile声明，来告诉编译器这个全局变量是易变的，
						 //让编译器不要对这个变量进行优化（至于编译器到底有没有按照你的要求来对volatile进行处理这个暂且不理）。
pthread_mutex_t mutex;

void changeResultCount(int r)
{
		pthread_mutex_lock(&mutex);
		gResult = r;
		pthread_mutex_unlock(&mutex);
	
};
int getResultCount()
{
		int r;	
		pthread_mutex_lock(&mutex);
		r = gResult; 
		pthread_mutex_unlock(&mutex);
		return r;
	
};
void setThreadResultCount(int & r)
{
		pthread_mutex_lock(&mutex);
		r = gResult; 
		pthread_mutex_unlock(&mutex);
	
};

//在APuzzle::findAnswer中的最深一层调用检查答案是否符合要求，通过checkRules的就打印出来，反正，回溯：1.已找到答案，继续找下一个；2.深度遍历过程中发现错了，回溯
void* findAnswerPartition(void* rank)
{
		long myRank = (long)rank;

		//ChessBoard cb = chessBoard;//深复制一个一摸一样的副本
		APuzzle ap(chessBoard);
		
		long long myN = ap.cb.col/threadCount;
		long long myFirst = myRank*myN;
		long long myLast = myFirst + myN - 1; 
		int curRow = 0;
		int i;
		for(i = myFirst; i<=myLast && !getResultCount(); i++)//当前第一行，在大家都没找到答案前，职责范围内逐个位置试
		{
				
						cout<<"("<<curRow<<","<<i<<")";
				if(ap.cb.setGrid(curRow,i))//成功放灯，即该位置符合放灯的基本要求：白格+没有灯光+四个邻居都不是0黑格+没有灯
				{						//设置成功，继续递归:找下个黑格，或者下一行	
					int k = i+1;
					while(k<ap.cb.col && ap.cb.chessboard[curRow][k]->kind()==0)
					{
						k++;
					}
					if(k<ap.cb.col)//在格子范围内
					{	
						ap.findAnswer(curRow,k);
								
					}
					else
						ap.findAnswer(curRow+1,0);
					//很样衰地，找不到解，重置
					ap.cb.resetGrid(curRow,i);
				}
						
				else //设置不成功=黑格||有灯光覆盖||有0号黑格做邻居||本来就有灯
				{	
					if(ap.cb.chessboard[curRow][i]->kind()==1)//i是黑格
					{
						if(ap.cb.setGridAroundBlackNum(curRow,i,((BlackSquare*)ap.cb.chessboard[curRow][i])->num))//成功则继续递归
						{
							if((i+1) <ap.cb.col)
								ap.findAnswer(curRow,i+1);
							else
								ap.findAnswer(curRow+1,0);
							//很样衰地，找不到解，重置
							ap.cb.resetNumGrid(curRow,i,((BlackSquare*)ap.cb.chessboard[curRow][i])->num);
							break;//回溯到此处，证明黑格不被满足，没必要试下一个，直接跳出循环返回上层调用
						}
						else
						{
							ap.cb.resetNumGrid(curRow,i,((BlackSquare*)ap.cb.chessboard[curRow][i])->num);
							break;//if通不过，证明黑格不被满足，没必要试下一个，直接跳出循环返回上层调用
						}
								
						//不成功则结束函数返回到上一层调用
					}
				//当前格不是黑格，即：有灯光覆盖||有0号黑格做邻居||有灯
					else if(ap.cb.chessboard[curRow][i]->bulb)//本来就有灯
					{
						int k = i+1;
						while(k<ap.cb.col && ap.cb.chessboard[curRow][k]->kind()==0)
						{
							k++;
						}
						if(k<ap.cb.col)
						{	
							ap.findAnswer(curRow,k);
								
						}
						else
							ap.findAnswer(curRow+1,0);
					}
							
					//有灯光覆盖||有0号黑格做邻居,实在不行了，试下一个位置吧
				}

				if(ap.result != 0)//当前线程找到解
				{
					changeResultCount(ap.result);//设置全局结果数
							
				}
						
				//if(getResultCount())//获取全局结果数，全局已有结果，则设置当前线程为"已找到解"状态，立即回溯并退出函数
				//setThreadResultCount(ap.result);
		}
		if(!getResultCount() && i >= ap.cb.col)//未找到解，并且是此行最后一个爆格时为棋盘列数
		{
			//如果不加条件 i == ap.cb.col，则多个线程在第一行寻不到可行位置的时候都会向第二行进行递归，几个线程做一模一样的事
			//在第一行分配的范围内，找不到解，只有在棋盘col处爆格的才进入，即只有一个线程负责这种情况
			if(ap.cb.chessboard[curRow][i-1]->lightOn>0 || ap.cb.checkNeighIsZero(curRow,i-1))//由于：有灯光覆盖||有0号黑格做邻居，
			{		cout<<"爆格";																	//使得一直设不了灯，无法递归就含冤爆格，很明显仍需在下一行继续寻解
				ap.findAnswer(curRow+1,0);//直接递归下一行
					
			}
		}
		//if(ap.result!=0)
		//pthread_cancel(threadHandles[(myRank+1)%2]);
		cout<<"finish : "<<rank<<endl;
		return (void*)rank;
};





class PlayerPthreads
{
public:
	
	long thread;
	

	PlayerPthreads(int threadCnt,ChessBoard chsbor)
	{
		pthread_mutex_init(&mutex,NULL);//初始化

		chessBoard = chsbor;
		threadCount = threadCnt;

		threadHandles = (pthread_t*)malloc(threadCount * sizeof(pthread_t));

		for(thread = 0; thread<threadCount; thread++)
		{
			pthread_create(&threadHandles[thread],NULL, findAnswerPartition,(void*)thread);
		}
		for(thread = 0; thread<threadCount; thread++)
		{
			pthread_join(threadHandles[thread],NULL);
		}
		free(threadHandles);//与malloc()函数配对使用，释放malloc函数申请的动态内存。
		//另：如果p 是NULL 指针，那么free 对p 无论操作多少次都不会出问题。如果p 不是NULL 指针，那么free 对p连续操作两次就会导致程序运行错误。
		
		pthread_mutex_destroy(&mutex);//销毁
	}
	~PlayerPthreads()
	{
		
	
	}




};

#endif 