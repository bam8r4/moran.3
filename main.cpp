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

using namespace std;

int main(int argc, char **argv)
{

 vector<string> palindromes;

//Establishing shared memory
 key_t key = 13933;
 int shmid = shmget(key,sizeof(int),0666|IPC_CREAT);
 int *ptr = (int*) shmat(shmid,(void*)0,0);

 *ptr = 9;

//Setting variables for command line arguments.
 int maxNumChildren = 4;
 int concurrentChildren = 2;
 int maxTimeSeconds = 100;
 string fileName;

 if(argc < 2)
 {
	 cout<<"Usage: "<<argv[0]<<" [-n MaxNumber of children] [-s MaxNumber of concurrent processes] [-t MaxTime in seconds] InputFile"<<endl;
	 cout<<"Defaults are as follows: MaxNumber of children = 4 Max concurrent processes = 2 Max time 100 seconds."<<endl;
	 return 0;
 }

 int option_index = 0;
 char *user_name = NULL;
 while (( option_index = getopt(argc, argv, "n:s:t:h")) != -1){
   switch (option_index) {
     case 'n':
       maxNumChildren = atoi(optarg);
       break;
     case 's':
       concurrentChildren = atoi(optarg);
       break;
		 case 't':
	     maxTimeSeconds = atoi(optarg);
	     break;
		 case 'h':
	 		 cout<<"Usage: "<<argv[0]<<" [-n MaxNumber of children] [-s MaxNumber of concurrent processes] [-t MaxTime in seconds] InputFile"<<endl;
			 cout<<"Defaults are as follows: MaxNumber of children = 4 Max concurrent processes = 2 Max time 100 seconds."<<endl;
			 return 0;
     default:
      printf("Incorrect options.\n");
      return 1;
     } //end block for switch
   }  //end block for while

	fileName = argv[argc-1];

	if(concurrentChildren > 20)
	{
		concurrentChildren = 20;
	}

	cout<<"\nMax num children: "<<maxNumChildren<<endl;
	cout<<"Max concurrent processes: "<<concurrentChildren<<endl;
	cout<<"Max time: "<<maxTimeSeconds<<endl;
	cout<<"Input file name: "<<fileName<<endl;

	ifstream inFile(fileName.c_str());
	string str;

	while(getline(inFile,str))
	{

		for (int i = 0, len = str.size(); i < len; i++)
    {
        if (ispunct(str[i]) || isspace(str[i]))
        {
            str.erase(i--, 1);
            len = str.size();
        }
    }

		palindromes.push_back(str);
	}


	char *argvars[] = {NULL,NULL};
	string tempString;
	int counter = 0;
	int curProcessCount = 0;
	int maxProcessCount = 0;
  pid_t pid;
	while(maxProcessCount <= maxNumChildren)
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
	}

		//Deleting shared memory
		shmdt((void *) ptr);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;


}
