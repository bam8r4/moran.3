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

	cout<<"I am the one child process that should run."<<endl;


  exit(0);
}
