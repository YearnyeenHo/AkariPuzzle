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
	ftime(&t1);//算时间
	
	/*
	这是串行版的，使用前先把并行版的注释掉~
	APuzzle ap(cb);
	ap.findAnswer(0,0);
	*/


	
	PlayerPthreads pp(2,cb);/*并行版*/
	
	
	ftime(&t2);//算时间
	printf("Msecond : %ld \n",(t2.time - t1.time)*1000+t2.millitm - t1.millitm );
	cout<<"YEA";
	//ap.cb.printChess();
	cin>>n;
	return 0;
}