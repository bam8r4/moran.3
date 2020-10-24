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

  key_t secondKey = 7688233;
  key_t nanSecondKey = 3768452;
  key_t shmPidKey = 4226754;
  key_t keyParentMessage = 9446365;
  key_t keyChildMessage = 8849795;

  int shmid = shmget(secondKey,sizeof(int),0666|IPC_CREAT);
  int *secondPtr = (int*) shmat(shmid,(void*)0,0);

  shmid = shmget(nanSecondKey,sizeof(int),0666|IPC_CREAT);
  int *nanSecondPtr = (int*) shmat(shmid,(void*)0,0);

  shmid = shmget(shmPidKey,sizeof(int),0666|IPC_CREAT);
  int *shmPID = (int*) shmat(shmid,(void*)0,0);

  int seconds = *secondPtr;
  int nanSeconds = *nanSecondPtr;
  pid_t myPid = getpid();


  for(int i = 0; i < 50; i++)
  {
    seconds = *secondPtr;
    nanSeconds = *nanSecondPtr;

    cout<<"Current clock time in the child is" << seconds << "." <<nanSeconds<<endl;

    if(*shmPID == 0)
    {
      cout<<"Child found shmpid as zero and set it to: "<<myPid<<;
      *shmPID = myPid;
    }

    sleep(1);

  }




  exit(0);
}
