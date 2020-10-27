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
#include <sys/msg.h>
#include <signal.h>
#include <time.h>


using namespace std;

struct mesg_buffer {
    long mesg_type = 1;
    char mesg_text[1];
};

int main(int argc,char *argv[])
{
  srand(time(0));

  bool keepAlive = true;
  bool expiredProc = false;

  struct mesg_buffer parentMessage;
  struct mesg_buffer childMessage;
  int msgidParent;
  int msgidChild;

  key_t secondKey = 7688233;
  key_t nanSecondKey = 3768452;
  key_t shmPidKey = 4226754;
  key_t keyParentMessage = 9446365;
  key_t keyChildMessage = 8849795;

  int shmid1 = shmget(secondKey,sizeof(int),0666|IPC_CREAT);
  int *secondPtr = (int*) shmat(shmid1,(void*)0,0);

  int shmid2 = shmget(nanSecondKey,sizeof(int),0666|IPC_CREAT);
  int *nanSecondPtr = (int*) shmat(shmid2,(void*)0,0);

  int shmid3 = shmget(shmPidKey,sizeof(int),0666|IPC_CREAT);
  int *shmPID = (int*) shmat(shmid3,(void*)0,0);

  msgidParent = msgget(keyParentMessage, 0666 | IPC_CREAT);
  msgidChild = msgget(keyChildMessage, 0666 | IPC_CREAT);

  int seconds = *secondPtr;
  int nanSeconds = *nanSecondPtr;
  int lifeSpan = abs((rand()*rand())%1000000);
  int deathNanSeconds = 0;
  int deathSeconds = 0;

  deathNanSeconds = nanSeconds + lifeSpan;

  if(deathNanSeconds >= 1000000)
  {
    deathSeconds = seconds + 1;
    deathNanSeconds = deathNanSeconds % 1000000;
  }
  else
  {
    deathSeconds = seconds;
  }

  pid_t myPid = getpid();

    while(myPid == 0)
  {
    myPid = getpid();
  }

  while(keepAlive)
  {

    if(msgrcv(msgidParent, &parentMessage, sizeof(parentMessage), 1, 0) != -1)
    {
        if(deathSeconds < *secondPtr)
        {
          expiredProc = true;
        }
        else if(deathSeconds == *secondPtr && deathNanSeconds >= *nanSecondPtr)
        {
          expiredProc = true;
        }

        cout<<"Current clock time in the child is" << seconds << "." <<nanSeconds<<endl;

        if(*shmPID == 0 && expiredProc == true)
        {
	        /*cout<<"Still my pid is: "<<myPid<<endl;
          cout<<"Child found shmpid as "<< *shmPID <<" and set it to: "<<myPid<<endl;*/
          keepAlive = false;
          *shmPID = myPid;
        }

        msgsnd(msgidChild, &childMessage, sizeof(childMessage), 0);
    }

    sleep(1);

  }



//Detaching from shared memory
shmdt((int *) secondPtr);
shmdt((int *) nanSecondPtr);
shmdt((int *) shmPID);

  exit(0);

}
