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
#include <vector>
#include <sys/msg.h>
#include <signal.h>


using namespace std;

void signal_callback_handler(int signum) {
   cout << "Terminating with Control C " << signum << endl;
   // Terminate program
   exit(signum);
}

struct mesg_buffer {
    long mesg_type = 1;
    char mesg_text[1];
};

int main(int argc, char **argv)
{



//Adding signal handler.
signal(SIGINT, signal_callback_handler);

struct mesg_buffer parentMessage;
struct mesg_buffer childMessage;
int msgidParent;
int msgidChild;

vector<int> pids;

//Establishing shared memory
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

*secondPtr = 0;
*nanSecondPtr = 0;
*shmPID = 0;

//Setting variables for command line arguments.
 int maxNumChildren = 100;
 int concurrentChildren = 5;
 int maxTimeSeconds = 20;
 string fileName = "logfile.out";

 if(argc < 1)
 {
   cout<<"Usage: "<<argv[0]<<" [-c MaxNumber of concurrent processes] [-t MaxTime in seconds] [-l logFileName]"<<endl;
   cout<<"Defaults are as follows: MaxNumber of concurrent processes = 5 Max time 20 seconds. Default logfile name is logfile.out"<<endl;
	 return 0;
 }

 int option_index = 0;
 char *user_name = NULL;
 while (( option_index = getopt(argc, argv, "c:l:t:h")) != -1){
   switch (option_index) {
     case 'c':
       concurrentChildren = atoi(optarg);
       break;
     case 'l':
       fileName = optarg;
       break;
		 case 't':
	     maxTimeSeconds = atoi(optarg);
	     break;
		 case 'h':
	 		 cout<<"Usage: "<<argv[0]<<" [-c MaxNumber of concurrent processes] [-t MaxTime in seconds] [-l logFileName]"<<endl;
			 cout<<"Defaults are as follows: MaxNumber of concurrent processes = 5 Max time 20 seconds. Default logfile name is logfile.out"<<endl;
			 return 0;
     default:
      printf("Incorrect options.\n");
      return 1;
     } //end block for switch
   }  //end block for while


	cout<<"\nMax num concurrent children: "<<concurrentChildren<<endl;
	cout<<"Max time: "<<maxTimeSeconds<<endl;
	cout<<"Input file name: "<<fileName<<endl;

	//ifstream inFile(fileName.c_str());
  //ofstream palLog("log.out", std::ofstream::out | std::ofstream::app);
  ofsteam ofs(fileName.c_str(), std::ofstream::out | std::ofstream::app);
  ofs<<"Start of log"<<"\n";

  //palindromes.push_back(str);


	char *argvars[] = {NULL,NULL};
	string tempString;
	int counter = 0;
	int curProcessCount = 0;
	int maxProcessCount = 0;
  int terminatedProcesses = 0;
  pid_t pid;

  pid = fork();

    msgsnd(msgidParent, &parentMessage, sizeof(parentMessage), 0);
    while(1==1)
    {
      while(curProcessCount >= concurrentChildren)
			{
				wait();
			}
			//while(curProcessCount < concurrentChildren)
			//{
        if (curProcessCount < 1)
        {
          //cout<<"forkgin"<<endl;
          curProcessCount += 1;
  				maxProcessCount += 1;
  				pid = fork();
			  }

      if (pid == 0)
      {	  //Exec child;
          cout<<"Making child"<<endl;
          pids.push_back(pid);
          ofs<<"Master: Creating new child PID:"<<pid<<" at my time "<<*secondPtr<<"."<<*nanSecondPtr<<"\n";
          execvp("./user",argvars);
      }
      else if (pid > 0)
      {
        //Parent process
      }
      else
      {
          // fork failed
          perror("fork() failed!\n");
          return 1;
      }

      if(msgrcv(msgidChild, &childMessage, sizeof(childMessage), 1, 0) != -1)
      {
        cout<<"Recieved message from child."<<endl;
        *nanSecondPtr += 100000;

        if(*nanSecondPtr%1000000 == 0)
        {
          *nanSecondPtr = 0;
          *secondPtr += 1;

  	      //cout<<"Current shared pid is : "<<*shmPID<<endl;
          if(*shmPID > 0)
          {
            ofs<<"Master: Child PID:"<<*shmPID<<" is terminating at system time "<<*secondPtr<<"."<<*nanSecondPtr<<"\n";
            curProcessCount -= 1;
            terminatedProcesses += 1;
            *shmPID = 0;
          }


        }
        msgsnd(msgidParent, &parentMessage, sizeof(parentMessage), 0);
    }

    while(terminatedProcesses < maxProcessCount)
    {
      cout<<"Term processes: "<<terminatedProcesses<<" maxProcessCount: "<<maxProcessCount<<endl;
      sleep(2);
    }
    cout<<"Waited..."<<endl;

    //Detaching from shared memory
    shmdt((int *) secondPtr);
    shmdt((int *) nanSecondPtr);
    shmdt((int *) shmPID);

    //Deleting shared memory
    shmctl(shmid1, IPC_RMID, NULL);
    shmctl(shmid2, IPC_RMID, NULL);
    shmctl(shmid3, IPC_RMID, NULL);

    return 0;


  }
}
