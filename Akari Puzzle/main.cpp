#include"chessboard.h"
#include"APuzzle.h"
#include"PlayerPthreads.h"
#include<sys/timeb.h>
int main()
{
	struct timeb t1,t2;
	int n;
	ChessBoard cb(10,10);
	cb.printChess();
	cout<<"wait"<<endl;
	ftime(&t1);//��ʱ��
	
	/*
	���Ǵ��а�ģ�ʹ��ǰ�ȰѲ��а��ע�͵�~
	APuzzle ap(cb);
	ap.findAnswer(0,0);
	*/


	
	PlayerPthreads pp(2,cb);/*���а�*/
	
	
	ftime(&t2);//��ʱ��
	printf("Msecond : %ld \n",(t2.time - t1.time)*1000+t2.millitm - t1.millitm );
	cout<<"YEA";
	//ap.cb.printChess();
	cin>>n;
	return 0;
}