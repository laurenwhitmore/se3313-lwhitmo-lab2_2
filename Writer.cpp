#include <iostream>
#include <vector>
#include "SharedObject.h"
#include "thread.h"
#include "Semaphore.h"

struct MyShared{
	int threadId;
	int reportId;
	int timePassed;
};
//create semaphores 
Semaphore writer("writer", 1, true);
Semaphore notifier("notifier", 1, true);
class WriterThread : public Thread{
	public:
		int threadId;
		int reportNo;
		int delay;
		bool flag;
		
		WriterThread(int in, int delay):Thread(8*1000){
			this->threadId = in; //or whatever you want/need here
			this->delay=delay;
			this->reportNo = 0;//report is the number of times that a thread has reported
		}

		virtual long ThreadMain(void) override{
			//declare shared memory var so this thread can access it
			Shared<MyShared> sharedMemory ("sharedMemory");
			while(true)
			{
				writer.Wait();//call the wait to wait to block/wait
				//write to shared memory
				 sharedMemory -> threadId=threadId;
				 sharedMemory -> reportId=++reportNo;
				 sharedMemory -> timePassed=delay;

				 //signal that done 
				 writer.Signal();
				 notifier.Signal();
				 sleep(delay);//sleep for the length of the seconds in delay
				if(flag){//Exit loop to end the thread
					break;
				}
			}
			return 0;
		}
};

int main(void)
{
	std::cout << "I am a Writer" << std::endl;
	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMamory
	char selection; //used to store the user input
	int threadNo=0;
	std::vector<WriterThread*> threads;

	while(true){
		std:: cout<<"Would you like to create a writer thread?(y/n)"<< std::endl;
		std::cin>>selection; //store in selction
		if(selection!='y' && selection != 'n') {//if not y or no repeat command
			std::cout<<"Please enter y or n!"<< std::endl;
			std::cin>>selection;
		}
		if(selection == 'n') break;

		std:: string delay; //store the thread delay

		std::cout<<"What is the delay time for this thread?"<< std::endl; //ask for the thread delay
		std:: cin>>delay; //assign to the delay variable 
		threadNo++;//increment the id
		//call the writer thread constructor
		WriterThread * thread1= new WriterThread(threadNo, stoi(delay));//stoi converts delay to int
		threads.push_back(thread1);
	}
	for(WriterThread* threadPoint : threads){
		threadPoint -> flag = true;
	}
	
	return 0;
}
