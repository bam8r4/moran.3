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


using namespace std;

struct mesg_buffer {
    long mesg_type = 1;
    char mesg_text[1];
};

int main(int argc,char *argv[])
{
  struct mesg_buffer parentMessage;
  struct mesg_buffer childMessage;
  int msgidParent;
  int msgidChild;

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

  msgidParent = msgget(keyParentMessage, 0666 | IPC_CREAT);
  msgidChild = msgget(keyChildMessage, 0666 | IPC_CREAT);

  int seconds = *secondPtr;
  int nanSeconds = *nanSecondPtr;
  pid_t myPid = getpid();

  while(myPid == 0)
{  

  myPid = getpid();

}

  cout << myPid << " this is my pid"<<endl;

  for(int i = 0; i < 50; i++)
  {

    if(msgrcv(msgidParent, &parentMessage, sizeof(parentMessage), 1, 0) != -1)
    {
        seconds = *secondPtr;
        nanSeconds = *nanSecondPtr;

        cout<<"Current clock time in the child is" << seconds << "." <<nanSeconds<<endl;

        if(*shmPID == 0)
        {
	  cout<<"Still my pid is: "<<myPid<<endl;
          cout<<"Child found shmpid as "<< *shmPID <<" and set it to: "<<myPid<<endl;
          *shmPID = myPid;
        }
    }
    else
    {
      cout<<"Didnt recieve the message"<<endl;
    }

    sleep(1);

  }




  exit(0);
}
