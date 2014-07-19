#ifndef PLAYERPTHREAD_H
#define PLAYERPTHREAD_H
#include<pthread.h>
#include"APuzzle.h"
#include"ChessBoard.h"

#pragma comment(lib, "pthreadVC2.lib")


pthread_t* threadHandles;
int threadCount;
ChessBoard chessBoard;
volatile int gResult = 0;//ͨ�����ǻ������ȫ�ֱ���ǰ����volatile�����������߱��������ȫ�ֱ������ױ�ģ�
						 //�ñ�������Ҫ��������������Ż������ڱ�����������û�а������Ҫ������volatile���д���������Ҳ�����
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

//��APuzzle::findAnswer�е�����һ����ü����Ƿ����Ҫ��ͨ��checkRules�ľʹ�ӡ���������������ݣ�1.���ҵ��𰸣���������һ����2.��ȱ��������з��ִ��ˣ�����
void* findAnswerPartition(void* rank)
{
		long myRank = (long)rank;

		//ChessBoard cb = chessBoard;//���һ��һ��һ���ĸ���
		APuzzle ap(chessBoard);
		
		long long myN = ap.cb.col/threadCount;
		long long myFirst = myRank*myN;
		long long myLast = myFirst + myN - 1; 
		int curRow = 0;
		int i;
		for(i = myFirst; i<=myLast && !getResultCount(); i++)//��ǰ��һ�У��ڴ�Ҷ�û�ҵ���ǰ��ְ��Χ�����λ����
		{
				
						cout<<"("<<curRow<<","<<i<<")";
				if(ap.cb.setGrid(curRow,i))//�ɹ��ŵƣ�����λ�÷��ϷŵƵĻ���Ҫ�󣺰׸�+û�еƹ�+�ĸ��ھӶ�����0�ڸ�+û�е�
				{						//���óɹ��������ݹ�:���¸��ڸ񣬻�����һ��	
					int k = i+1;
					while(k<ap.cb.col && ap.cb.chessboard[curRow][k]->kind()==0)
					{
						k++;
					}
					if(k<ap.cb.col)//�ڸ��ӷ�Χ��
					{	
						ap.findAnswer(curRow,k);
								
					}
					else
						ap.findAnswer(curRow+1,0);
					//����˥�أ��Ҳ����⣬����
					ap.cb.resetGrid(curRow,i);
				}
						
				else //���ò��ɹ�=�ڸ�||�еƹ⸲��||��0�źڸ����ھ�||�������е�
				{	
					if(ap.cb.chessboard[curRow][i]->kind()==1)//i�Ǻڸ�
					{
						if(ap.cb.setGridAroundBlackNum(curRow,i,((BlackSquare*)ap.cb.chessboard[curRow][i])->num))//�ɹ�������ݹ�
						{
							if((i+1) <ap.cb.col)
								ap.findAnswer(curRow,i+1);
							else
								ap.findAnswer(curRow+1,0);
							//����˥�أ��Ҳ����⣬����
							ap.cb.resetNumGrid(curRow,i,((BlackSquare*)ap.cb.chessboard[curRow][i])->num);
							break;//���ݵ��˴���֤���ڸ񲻱����㣬û��Ҫ����һ����ֱ������ѭ�������ϲ����
						}
						else
						{
							ap.cb.resetNumGrid(curRow,i,((BlackSquare*)ap.cb.chessboard[curRow][i])->num);
							break;//ifͨ������֤���ڸ񲻱����㣬û��Ҫ����һ����ֱ������ѭ�������ϲ����
						}
								
						//���ɹ�������������ص���һ�����
					}
				//��ǰ���Ǻڸ񣬼����еƹ⸲��||��0�źڸ����ھ�||�е�
					else if(ap.cb.chessboard[curRow][i]->bulb)//�������е�
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
							
					//�еƹ⸲��||��0�źڸ����ھ�,ʵ�ڲ����ˣ�����һ��λ�ð�
				}

				if(ap.result != 0)//��ǰ�߳��ҵ���
				{
					changeResultCount(ap.result);//����ȫ�ֽ����
							
				}
						
				//if(getResultCount())//��ȡȫ�ֽ������ȫ�����н���������õ�ǰ�߳�Ϊ"���ҵ���"״̬���������ݲ��˳�����
				//setThreadResultCount(ap.result);
		}
		if(!getResultCount() && i >= ap.cb.col)//δ�ҵ��⣬�����Ǵ������һ������ʱΪ��������
		{
			//����������� i == ap.cb.col�������߳��ڵ�һ��Ѱ��������λ�õ�ʱ�򶼻���ڶ��н��еݹ飬�����߳���һģһ������
			//�ڵ�һ�з���ķ�Χ�ڣ��Ҳ����⣬ֻ��������col������ĲŽ��룬��ֻ��һ���̸߳����������
			if(ap.cb.chessboard[curRow][i-1]->lightOn>0 || ap.cb.checkNeighIsZero(curRow,i-1))//���ڣ��еƹ⸲��||��0�źڸ����ھӣ�
			{		cout<<"����";																	//ʹ��һֱ�費�˵ƣ��޷��ݹ�ͺ�ԩ���񣬺�������������һ�м���Ѱ��
				ap.findAnswer(curRow+1,0);//ֱ�ӵݹ���һ��
					
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
		pthread_mutex_init(&mutex,NULL);//��ʼ��

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
		free(threadHandles);//��malloc()�������ʹ�ã��ͷ�malloc��������Ķ�̬�ڴ档
		//�����p ��NULL ָ�룬��ôfree ��p ���۲������ٴζ���������⡣���p ����NULL ָ�룬��ôfree ��p�����������ξͻᵼ�³������д���
		
		pthread_mutex_destroy(&mutex);//����
	}
	~PlayerPthreads()
	{
		
	
	}




};

#endif 