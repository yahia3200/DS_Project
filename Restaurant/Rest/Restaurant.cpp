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
Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;
}
void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		SimpleSimulator(MODE_INTR);
		break;
	case MODE_STEP:
		SimpleSimulator(MODE_STEP);
		break;
	case MODE_SLNT:
		SimpleSimulator(MODE_SLNT);
		break;
	
	};

}
void Restaurant::SimpleSimulator(PROG_MODE mode)
{
	int CurrentTimeStep = 1;
	LoadFile();
	//the condition of exeting the loop is to be changed 
	// with the last order served this will end 
	while (!EventsQueue.isEmpty() || Ordassigned != Finshed_orders.GetCount()) {
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);
		ExecuteEvents(CurrentTimeStep);
		ThirdStage(CurrentTimeStep);
		Middle_Stage(CurrentTimeStep);
		this->FillDrawingList();
		this->PrintInfo(CurrentTimeStep);
	
		switch (mode)
		{
		case MODE_INTR:
			pGUI->UpdateInterface();
			pGUI->waitForClick();
			break;
		case MODE_STEP:
			pGUI->UpdateInterface();
			Sleep(1000);
			break;
		default:
			break;
		}
		
		CurrentTimeStep++;
		pGUI->ResetDrawingList();

	}

	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

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
	inFile >> BO >> BreakOfNC >> BreakOfGC >> BreakOfVC;
	inFile >> AutoP;

	//Populating cooks lists.
	Cook* CookPtr;
	int i = 0;
	//Populating Normal cooks in their list
	while (i < NumOfNC)
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
	while (i < NumOfGC)
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
	while (i < NumOfVC)
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
	while (i < NumOfEvents)
	{
		inFile >> EventType;
		if (EventType == 'R')
		{
			char oType; int eTime; int oID; int oSize; double oMoney;
			inFile >> oType >> eTime >> oID >> oSize >> oMoney;
			EventPtr = new ArrivalEvent((ORD_TYPE)(CharToNum(oType)), eTime, oID, oSize, oMoney);
			EventsQueue.enqueue(EventPtr);
			Ordassigned++;
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
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


//***********************************GUI Functions*****************************

void Restaurant::FillDrawingList()
{
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
void Restaurant::PrintInfo(int currenttime) {
	pGUI->PrintSeveral("TS :  " + to_string(currenttime));
	pGUI->PrintSeveral("Waiting _VO:  " + to_string(Waiting_VO.getCount()) +
		"  Waiting _GO:  " + to_string(Waiting_GO.GetCount()) +
		"  Waiting _N0:  " + to_string(Waiting_NO.getCount()));
	pGUI->PrintSeveral("Available_VC:  " + to_string(Available_VC.GetCount()) +
		"  Available_GC:  " + to_string(Available_GC.GetCount()) +
		"  Available_NC " + to_string(Available_NC.GetCount()));
	pGUI->PrintSeveral("Finished_VO:  " + to_string(Finished_VO) +
		" Finished_GO:  " + to_string(Finished_GO) +
		" Finished_NO: " + to_string(Finished_NO));

}

//***********************************End GUI Functions*****************************


//***********************************Helper Functions*****************************

void Restaurant::inValidFormat()
{
	ofstream ErrorFile;
	ErrorFile.open("Error.txt");
	ErrorFile << "Something is wrong with the input file's format \n ";
	ErrorFile << "Refer to the document to help solve your problem: \n";
	ErrorFile.close();
	exit(1);
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
void Restaurant::ToWaitingList(Order* neworder)
{

	ORD_TYPE type = neworder->GetType();
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
void Restaurant::SetOrdAssigned(int s) {
	Ordassigned = s;
}
int Restaurant::GetOrdAssigned()
{
	return Ordassigned;
}

//***********************************End Helper Functions*****************************


//***********************************Middle Stage Functions*****************************

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
		AC->setCurrentOrder(InSRV_O); // assign the order to the cooker
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
		AC->setCurrentOrder(InSRV_O);
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
		AC->setCurrentOrder(InSRV_O);
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
			busy_cooks.enqueue(AC);
			InSRV_O->setStatus(SRV);
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
			busy_cooks.enqueue(AC);
			InSRV_O->setStatus(SRV);
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
			busy_cooks.enqueue(AC);
			InSRV_O->setStatus(SRV);
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

//***********************************End Middle Stage Functions*****************************


//***********************************Third Stage Functions*****************************

void Restaurant::ExitBusyList(Cook* &c,int currenttime) {
	if (c->getStatus() != BUSY)return;
	c->setFinishedOrders(c->getFinishedOrders() + 1);
	c->setCurrentOrder(nullptr);
	//meets the no of  orders before break 
		if (c->getFinishedOrders() % BO == 0) 
		{

		busy_cooks.dequeue();
		c->setStatus(BREAK);
		c->setEndBreakTime(currenttime + c->getBreakDuration());
		in_break.enqueue(c);
	    }
		else ToAvailableList(c);
}
void Restaurant::ExitBreakList( int currenttime) {
	if (in_break.isEmpty())return;
	Cook* c = in_break.Peek();
		while (c&&c->getEndBreakTime()==currenttime) 
		{
			ToAvailableList(c);
			c = in_break.Peek();
	    }
}
void Restaurant::ToAvailableList(Cook*& c) {
	ORD_TYPE type = c->GetType();
	COOK_STATUS stat=c->getStatus();
	if (stat == BUSY) {
		busy_cooks.dequeue();
	}
	else if (stat == BREAK) {
		in_break.dequeue();
	}
	switch (type)
	{
	case TYPE_NRM:
		Available_NC.enqueue(c);
		break;
	case TYPE_VGAN:
		Available_GC.enqueue(c);
		break;
	case TYPE_VIP:
		Available_VC.enqueue(c);
		break;
	}
	c->setStatus(AVAILABLE);

}
void Restaurant::ThirdStage(int currenttime) {
	ExitBreakList(currenttime);
	Cook * c;
	Order* ord;
	
	ord = Being_Served.Peek();
	while (ord &&ord->getFinishTime() == currenttime) {
		Finshed_orders.enqueue(ord);
		switch (ord->GetType()) {
		case TYPE_NRM:
			Finished_NO++;
			break;
		case TYPE_VGAN:
			Finished_GO++;
			break;
		case TYPE_VIP:
			Finished_VO++;
			break;
		}
		ord = Being_Served.dequeue();
		ord->setStatus(DONE);
		c = busy_cooks.Peek();
		ExitBusyList(c, currenttime);
		ord = Being_Served.Peek();
	}
}

//***********************************End Third Stage Functions*****************************