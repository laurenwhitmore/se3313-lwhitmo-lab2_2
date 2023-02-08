#include <iostream>
#include "SharedObject.h"
#include "Semaphore.h"

struct MyShared{
	int threadId;
	int reportId;
	int timePassed;
};
int main(void)
{
	std::cout << "I am a reader" << std::endl;
	//make the semphores
	Semaphore writer("writer", 1 , false);
	Semaphore notifier("notifier", 1 ,false);
	Shared<MyShared> sharedMemory("sharedMemory");//create shared memory object
	while(true){
		notifier.Wait();
		writer.Wait();
		std::cout<<"Reader Thread: "<< sharedMemory -> threadId << " "<< sharedMemory->reportId <<" "<< sharedMemory->timePassed <<std::endl;
		writer.Signal();
	}
}
