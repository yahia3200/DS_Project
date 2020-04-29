#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancelEvent.h"
#include "..\Events\PromoteEvent.h"


Restaurant::Restaurant() 
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		break;
	case MODE_STEP:
		SimpleSimulator();
	case MODE_SLNT:
		break;
	
	};

}
//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}
	
}
//***********************************Middle Stage Functions*****************************
//This function increments the waiting time for all the orders that haven't been assigned in this timestep
void Restaurant::increment_Waiting_Time()
{
	int GO_Count = 0;
	
	if (!Waiting_NO.isEmpty())
	{
		Order** NO_Array = Waiting_NO.toArray();
		for (int i = 0; i < Waiting_NO.getCount(); i++)
		{
			NO_Array[i]->setWaitingTime(NO_Array[i]->getWaitingTime() + 1);
			if (NO_Array[i]->getWaitingTime() == AutoP)
			{
				//RemoveFromWaiting_NO(NO_Array[i]->GetID());
				Waiting_NO.deleteNode(NO_Array[i]);
				NO_Array[i]->SetType(TYPE_VIP);
				ToVIP(NO_Array[i]);
				NumOfAutoPNO++;
			}
		}
		delete[] NO_Array;
	}
	if (!Waiting_VO.isEmpty())
	{
		Order** VO_Array = Waiting_VO.toArray();
		for (int i = 0; i < Waiting_VO.getCount(); i++)
		{
			VO_Array[i]->setWaitingTime(VO_Array[i]->getWaitingTime() + 1);
		}
		delete[] VO_Array;
	}
	if (!Waiting_GO.isEmpty())
	{
		Order** GO_Array = Waiting_GO.toArray(GO_Count);
		for (int i = 0; i < GO_Count; i++)
		{
			GO_Array[i]->setWaitingTime(GO_Array[i]->getWaitingTime() + 1);
		}
		delete[] GO_Array;
	}
}

bool Restaurant::Assign_To_VC(Order* InSRV_O, Cook* &AC)
{

	if (!Available_VC.isEmpty())
	{
		Available_VC.dequeue(AC); // peeking 
		AC->setStatus(BUSY); //update the cooker'status to "BUSY"
		busy_cooks.enqueue(AC); // move the cooker to busy_cooks list
		AC->setCurrentOrder(InSRV_O); // assign the order to the cooker

		InSRV_O->setStatus(SRV); //update the order status to "serve"
		return true;
	}
	return false;
}

bool Restaurant::Assign_To_NC(Order* InSRV_O, Cook* &AC)
{

	if (!Available_NC.isEmpty())
	{
		Available_NC.dequeue(AC);
		AC->setStatus(BUSY);
		busy_cooks.enqueue(AC);
		AC->setCurrentOrder(InSRV_O);
		InSRV_O->setStatus(SRV);
		return true;
	}
	return false;
}

bool Restaurant::Assign_To_GC(Order* InSRV_O, Cook* &AC)
{

	if (!Available_GC.isEmpty())
	{
		Available_GC.dequeue(AC);
		AC->setStatus(BUSY);
		busy_cooks.enqueue(AC);
		AC->setCurrentOrder(InSRV_O);
		InSRV_O->setStatus(SRV);
		return true;
	}
	return false;
}


void Restaurant::Middle_Stage(int currtime)
{
	Order* InSRV_O;// the order that will be served

	Cook* AC; // Available cooker for any type
	
	//----------------1) for vip order assignment ---------------
	while (!Waiting_VO.isEmpty())
	{
		InSRV_O = Waiting_VO.Peek();//peek the highest priority order but without deleting yet

		if (Assign_To_VC(InSRV_O, AC) || Assign_To_NC(InSRV_O, AC) || Assign_To_GC(InSRV_O, AC))
		{
			Waiting_VO.dequeue();// after confirming the order status == srv , delete it from waiting list
			int ST = ceil(float(InSRV_O->getOrderSize()) / AC->getSpeed()); //calculation of serving time
			InSRV_O->setServTime(ST);
			int FT = InSRV_O->getArrTime() + InSRV_O->getServTime() + InSRV_O->getWaitingTime();// calculation of finished time
			InSRV_O->setFinishTime(FT);
			Being_Served.enqueue(InSRV_O); //move to being served list
		}
		else
		{
			break;
		}
	}

	//+++++++++++++++2) for vegan order assignment +++++++++++++++++
	while (!Waiting_GO.isEmpty())
	{
		Waiting_GO.peekFront(InSRV_O);
		if (Assign_To_GC(InSRV_O, AC))
		{
			Order* GO; // just because of the implementation of dequeue function in class Queue
			Waiting_GO.dequeue(GO);// after confirming the order status == srv , delete it from waiting list
			int ST = ceil(float(InSRV_O->getOrderSize()) / AC->getSpeed()); //calculation of serving time
			InSRV_O->setServTime(ST);
			int FT = InSRV_O->getArrTime() + InSRV_O->getServTime() + InSRV_O->getWaitingTime();// calculation of finished time
			InSRV_O->setFinishTime(FT);
			Being_Served.enqueue(InSRV_O); //move to being served list
		}
		else
		{
			break;
		}
	}

	//+++++++++++++++3) for Normal order assignment +++++++++++++++++
	while (!Waiting_NO.isEmpty())
	{
		InSRV_O = Waiting_NO.peekHead();
		if (Assign_To_NC(InSRV_O, AC) || Assign_To_VC(InSRV_O, AC))
		{
			Waiting_NO.Remove_Head();// after confirming the order status == srv , delete it from waiting list
			int ST = ceil(float(InSRV_O->getOrderSize()) / AC->getSpeed()); //calculation of serving time
			InSRV_O->setServTime(ST);
			int FT = InSRV_O->getArrTime() + InSRV_O->getServTime() + InSRV_O->getWaitingTime();// calculation of finished time
			InSRV_O->setFinishTime(FT);
			Being_Served.enqueue(InSRV_O); //move to being served list
		}
		else
		{
			break;
		}
	}

	//this function increments the waiting time for all the orders that haven't been assigned in this timestep
	increment_Waiting_Time();
}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}


void Restaurant::FillDrawingList() {
	Queue<Order*>temp;
	Order* frnt;

	frnt = Drawing.Remove_Head();

	while (frnt)
	{
		temp.enqueue(frnt);
		pGUI->AddToDrawingList(frnt);
		frnt = Drawing.Remove_Head();
	}
	while (temp.dequeue(frnt)) {
		Drawing.insertEnd(frnt);
	}

	///////////////////////for cooks this will present each type of cook after each other/////////////////
	int size = 0;
	Cook** Cook_Array = Available_VC.toArray(size);
	Cook* pCook;
	for (int i = 0; i < size; i++)
	{
		pCook = Cook_Array[i];
		pGUI->AddToDrawingList(pCook);
	}
	Cook_Array = Available_NC.toArray(size);
	for (int i = 0; i < size; i++)
	{
		pCook = Cook_Array[i];
		pGUI->AddToDrawingList(pCook);
	}
	Cook_Array = Available_GC.toArray(size);
	for (int i = 0; i < size; i++)
	{
		pCook = Cook_Array[i];
		pGUI->AddToDrawingList(pCook);
	}
}

void Restaurant::LoadFile()
{
	ifstream inFile("InFile.txt");
	
	//Handling errors
	if (!inFile.is_open())
	{
		ofstream ErrorFile;
		ErrorFile.open("Error.txt");
		ErrorFile << "An Error Occured while opening the input file \n";
		ErrorFile << "Check the mentioned instructions to help solve your problem: \n";
		ErrorFile << "-Make sure you are placing the input file in the same directory as the Demo_Main.cpp \n";
		ErrorFile << "-The file should not be in use \n";
		ErrorFile.close();
		exit(1);
	}

	//Loading Cooks-related data from the i/p file
	inFile >> NumOfNC >> NumOfGC >> NumOfVC;
	inFile >> SpeedOfNC >> SpeedOfGC >> SpeedOfVC;
	inFile >>BO >> BreakOfNC >> BreakOfGC >> BreakOfVC;
	inFile >> AutoP;
	
	//Populating cooks lists.
	Cook* CookPtr;
	int i = 0;
	//Populating Normal cooks in their list
	while (i<NumOfNC)
	{
		CookPtr = new Cook;
		CookPtr->setID(i + 1);
		CookPtr->setType(TYPE_NRM);
		CookPtr->setSpeed(SpeedOfNC);
		CookPtr->setFinishedOrders(0);
		CookPtr->setBreakDuration(BreakOfNC);
		Available_NC.enqueue(CookPtr);
		i++;
	}
	
	i = 0;
	//Populating Vegan cooks in their list
	while (i<NumOfGC)
	{
		CookPtr = new Cook;
		CookPtr->setID(i + 1 + NumOfNC);
		CookPtr->setType(TYPE_VGAN);
		CookPtr->setSpeed(SpeedOfGC);
		CookPtr->setFinishedOrders(0);
		CookPtr->setBreakDuration(BreakOfGC);
		Available_GC.enqueue(CookPtr);
		i++;
	}

	i = 0;
	//Populating VIP cooks in their list
	while (i<NumOfVC)
	{
		CookPtr = new Cook;
		CookPtr->setID(i + 1 + NumOfNC + NumOfGC);
		CookPtr->setType(TYPE_VIP);
		CookPtr->setSpeed(SpeedOfVC);
		CookPtr->setFinishedOrders(0);
		CookPtr->setBreakDuration(BreakOfVC);
		Available_VC.enqueue(CookPtr);
		i++;
	}

	//Note that the NumOfEvents is the number of lines left in the i/p file
	inFile >> NumOfEvents;

	i = 0;
	char EventType;
	Event* EventPtr;
	while (i<NumOfEvents)
	{
		inFile >> EventType;
		if (EventType == 'R')
		{
			char oType; int eTime; int oID; int oSize; double oMoney;
			inFile >> oType >> eTime >> oID >> oSize >> oMoney;
			EventPtr = new ArrivalEvent((ORD_TYPE)(CharToNum(oType)), eTime, oID, oSize, oMoney);
			EventsQueue.enqueue(EventPtr);
		}
		else if (EventType == 'X')
		{
			int eTime; int oID;
			inFile >> eTime >> oID;
			EventPtr = new CancelEvent(eTime, oID);
			EventsQueue.enqueue(EventPtr);
		}
		else if (EventType == 'P')
		{
			int eTime; int oID; double oExtraMoney;
			inFile >> eTime >> oID >> oExtraMoney;
			EventPtr = new PromoteEvent(eTime, oID, oExtraMoney);
			EventsQueue.enqueue(EventPtr);
		}
		else
		{
			inValidFormat();
		}
		i++;
	}

	inFile.close();

}

int Restaurant::CharToNum(char Type)
{
	switch (Type)
	{
	case 'N': 
		return 0;
	case 'G': 
		return 1;
	case 'V': 
		return 2;
	default: // code to be executed if 'Type' doesn't match any valid cases
		inValidFormat();
	}
}

void Restaurant::inValidFormat()
{
	ofstream ErrorFile;
	ErrorFile.open("Error.txt");
	ErrorFile << "Something is wrong with the input file's format \n ";
	ErrorFile << "Refer to the document to help solve your problem: \n";
	ErrorFile.close();
	exit(1);
}


void Restaurant::SimpleSimulator()
{
	int CurrentTimeStep = 1;
	LoadFile();
	//the condition of exeting the loop is to be changed later
	// this is according to the changes made in phase2 
	// with the last order served this will end 
	while (!EventsQueue.isEmpty()) {
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);
		ExecuteEvents(CurrentTimeStep);

		////////////    HAAAALAAAA    /////////////////
		//Third stage function should be called before middle stage function
		//for more information call me :")

		//CHECK AL TRTEEB BTA3 L CALLS
		Middle_Stage(CurrentTimeStep);
		this->FillDrawingList();

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	
		pGUI->ResetDrawingList();

	}
	
	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

}
//////////////////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//Begin of DEMO-related functions

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");
	pGUI->waitForClick();
		
	//Just for sake of demo, generate some cooks and add them to the drawing list
	//In next phases, Cooks info should be loaded from input file
	int C_count = 12;	

	Cook *pC = new Cook[C_count];
	int cID = 1;

	for(int i=0; i<C_count; i++)
	{
		cID+= (rand()%15+1);	
		pC[i].setID(cID);
		pC[i].setType((ORD_TYPE)(rand()%TYPE_CNT));
	}	

		
	int EvTime = 0;

	int O_id = 1;
	
	//Create Random events and fill them into EventsQueue
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		O_id += (rand()%4+1);		
		int OType = rand()%TYPE_CNT;	//Randomize order type		
		EvTime += (rand()%5+1);			//Randomize event time
		pEv = new ArrivalEvent(EvTime,O_id,(ORD_TYPE)OType);
		EventsQueue.enqueue(pEv);

	}	

	// --->   In next phases, no random generation is done
	// --->       EventsQueue should be filled from actual events loaded from input file

	
	
	
	
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		//Noran//This converts the int value in 'CurrentTimeStep' into strings and save that string into 'timestep'
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		

/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks
		
		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		for(int i=0; i<C_count; i++)
			pGUI->AddToDrawingList(&pC[i]);
		
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		int size = 0;
		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);
		
		for(int i=0; i<size; i++)
		{
			pOrd = Demo_Orders_Array[i];
			pGUI->AddToDrawingList(pOrd);
		}
/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
	}

	delete []pC;


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}
////////////////

void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

/// ==> end of DEMO-related function
//////////////////////////////////////////////////////////////////////////////////////////////


//the function's responsible of choosing the right waiting list 

void Restaurant:: ToWaitingList( Order * neworder)
{

ORD_TYPE type=	neworder->GetType();
	switch (type)
	{

	case  TYPE_NRM:
		Waiting_NO.insertEnd(neworder);
		break;
	case TYPE_VGAN:
		Waiting_GO.enqueue(neworder);
		break;
	case TYPE_VIP:
		Waiting_VO.enqueue(neworder);
		break;

	}


	Drawing.insertEnd(neworder);
}

Order* Restaurant::RemoveFromWaiting_NO(int id)
{
	Order* dOrder = Waiting_NO.get_by_index(id);

	if (dOrder)
		Waiting_NO.deleteNode(dOrder);

	return dOrder;
}

void Restaurant::RemoveFromDrawing(Order* dOrder)
{
	Drawing.deleteNode(dOrder);
}

void Restaurant::ToVIP(Order* ord)
{
	Waiting_VO.enqueue(ord);
}





