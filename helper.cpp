#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <fstream>


using namespace std;

int main(int argc,char *argv[])
{

  key_t key = 13933;
  int shmid = shmget(key,sizeof(int),0666|IPC_CREAT);
  int *ptr = (int*) shmat(shmid,(void*)0,0);

	string potPal = argv[0];
  //string index = argv[1];

	int length = 0;
	int flag = 0;

	length = potPal.length();

	for(int i=0;i < length ;i++)
	{
        	if(potPal[i] != potPal[length-i-1]){
            		flag = 1;
            		break;
		}
  	}


	if(flag == 0)
	{
    ofstream PalOut("palin.out", std::ofstream::out | std::ofstream::app);
    PalOut << potPal<<"\n";
    PalOut.close();

    ofstream palLog("log.out", std::ofstream::out | std::ofstream::app);
    palLog<< "Pid: "<<getpid()<<" String: "<<potPal<<"\n";
    palLog.close();
	}
	else
	{
    ofstream noPalOut("nopalin.out", std::ofstream::out | std::ofstream::app);
    noPalOut << potPal<<"\n";
    noPalOut.close();

    ofstream noPalLog("log.out", std::ofstream::out | std::ofstream::app);
    noPalLog << "Pid: "<<getpid()<<" String: "<<potPal<<"\n";
    noPalLog.close();
	}

  exit(0);
}
