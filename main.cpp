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


using namespace std;

struct mesg_buffer {
    long mesg_type = 1;
    char mesg_text[1];
};

int main(int argc, char **argv)
{

struct mesg_buffer parentMessage;
struct mesg_buffer childMessage;
int msgidParent;
int msgidChild;

vector<string> palindromes;

//Establishing shared memory
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

*secondPtr = 0;
*nanSecondPtr = 0;
*shmPID = 0;

//Setting variables for command line arguments.
 int maxNumChildren = 4;
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

	ifstream inFile(fileName.c_str());

  //palindromes.push_back(str);


	char *argvars[] = {NULL,NULL};
	string tempString;
	int counter = 0;
	int curProcessCount = 0;
	int maxProcessCount = 0;
pid_t pid;

pid = fork();

if (pid == 0)
{	  //Make child;
    cout<<"Inside"<<endl;
    //tempString = palindromes[maxProcessCount-1];
    //argvars[0] = (char *)tempString.c_str();
    msgsnd(msgidParent, &parentMessage, sizeof(parentMessage), 0);
    execvp("./user",argvars);

}
else if (pid > 0)
{
  //  cout<<"I am the parent"<<endl;
}
else
{
    // fork failed
    perror("fork() failed!\n");
    return 1;
}


  /*while(maxProcessCount <= maxNumChildren)
	{
		  while(curProcessCount >= concurrentChildren)
			{
				wait();

			}
			if(curProcessCount < concurrentChildren)
			{
				concurrentChildren += 1;
				maxProcessCount += 1;
				pid = fork();
			}

	    if (pid == 0)
	    {	  //Make child;
				  tempString = palindromes[maxProcessCount-1];
				  argvars[0] = (char *)tempString.c_str();
					//argvars[1] = (char *)(maxProcessCount-1);

				  execvp("./palin",argvars);

	    }
	    else if (pid > 0)
	    {
	        // parent process
	    }
	    else
	    {
	        // fork failed
	        perror("fork() failed!\n");
	        return 1;
	    }
	}*/
    for(int i = 0; i < 20; i++)
    {
      *nanSecondPtr += 500;

      if(*nanSecondPtr%1000 == 0)
      {
        *nanSecondPtr = 0;
        *secondPtr += 1;

        if(*shmPID > 0)
        {
          cout<<"Child has set the shared pid to: "<<*shmPID<<endl;
          *shmPID = 0;
        }
      }

      sleep(2);
    }

    wait();
    cout<<"Waited..."<<endl;
    cout<<"After child my shared memory is "<<*ptr<<endl;
		//Deleting shared memory
		shmdt((void *) ptr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;


}
