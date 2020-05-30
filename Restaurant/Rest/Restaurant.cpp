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
		
	switch (mode)	
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
	//the condition of exiting the loop is to be changed 
	// with the last order served this will end 
	while (!EventsQueue.isEmpty() || Ordassigned != Finshed_orders.GetCount()) {
		ExecuteEvents(CurrentTimeStep);
		ThirdStage(CurrentTimeStep);
		InjureACook(CurrentTimeStep);
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
	OutFile();
	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

}
void Restaurant::LoadFile()
{
	ifstream inFile("InFile.txt");
	
	//UnComment this in case you want "random sequence" to be generated for each time you run the program
	//srand(time(NULL));
	
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
	inFile >> SN_min >> SN_max >> SG_min >> SG_max >> SV_min >> SV_max;
	inFile >> BO >> BN_min >> BN_max >> BG_min >> BG_max >> BV_min >> BV_max;
	inFile >> InjProp >> RstPrd;
	inFile >> AutoP >> VIP_WT;

	//Populating cooks lists.
	Cook* CookPtr;
	int i = 0;
	//Populating Normal cooks in their list
	while (i < NumOfNC)
	{
		CookPtr = new Cook;
		CookPtr->setID(i + 1);
		CookPtr->setType(TYPE_NRM);
		CookPtr->setSpeed(( rand() % (SN_max - SN_min + 1) ) + SN_min);
		CookPtr->setNormalSpeed(CookPtr->getSpeed());
		CookPtr->setBreakDuration((rand() % (BN_max - BN_min + 1)) + BN_min);
		CookPtr->setRestperiod(RstPrd);
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
		CookPtr->setSpeed((rand() % (SG_max - SG_min + 1)) + SG_min);
		CookPtr->setNormalSpeed(CookPtr->getSpeed());
		CookPtr->setBreakDuration((rand() % (BG_max - BG_min + 1)) + BG_min);
		CookPtr->setRestperiod(RstPrd);
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
		CookPtr->setSpeed((rand() % (SV_max - SV_min + 1)) + SV_min);
		CookPtr->setNormalSpeed(CookPtr->getSpeed());
		CookPtr->setBreakDuration((rand() % (BV_max - BV_min + 1)) + BV_min);
		CookPtr->setRestperiod(RstPrd);
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

void Restaurant::OutFile()
{
	ofstream out;
	out.open("Output.txt");
	out << "FT ID AT  WT ST \n";
	Order* top;
	float total_wait_time = 0 , total_wait_ord=0 , total_serv = 0;
	int NO = 0, VO = 0, GO = 0 , Injured_cooks = 0;
	int total_ord = Finshed_orders.GetCount();
	while (!Finshed_orders.isEmpty()) {
		Finshed_orders.dequeue(top);
		if (top->getWaitingTime() != 0) { total_wait_ord++; }
		total_wait_time = total_wait_time + top->getWaitingTime();
		total_serv = total_serv + top->getServTime();
		switch (top->GetType()) {
		case TYPE_NRM:
			NO++;
			break;
		case TYPE_VGAN:
			GO++;
			break;
		case TYPE_VIP:
			VO++;
			break;
		default: 
			break;
		}

		out << top->getFinishTime() << " " << top->GetID() << " " << top->getArrTime() << " ";
		out << top->getWaitingTime() << " " << top->getServTime() << endl;
		delete top;
		top = nullptr;
	}
                ////////////////////////////////////////////////////////////////////////////////
	Cook* frnt;
	while (Available_GC.dequeue(frnt)) {
		if (frnt->getGot_Injured() == true) { Injured_cooks++; }
		delete frnt;
		frnt = nullptr;
	}
	while (Available_NC.dequeue(frnt)) {
		if (frnt->getGot_Injured() == true) { Injured_cooks++; }
		delete frnt;
		frnt = nullptr;
	}
	while (Available_VC.dequeue(frnt)) {
		if (frnt->getGot_Injured() == true) { Injured_cooks++; }
		delete frnt;
		frnt = nullptr;
	}
	while (frnt=in_break.dequeue()) {
		if (frnt->getGot_Injured() == true) { Injured_cooks++; }
		delete frnt;
		frnt = nullptr;
	}
	while (in_rest.dequeue(frnt)) {
		if (frnt->getGot_Injured() == true) { Injured_cooks++; }
		delete frnt;
		frnt = nullptr;
	}
	
	out << "Orders:" << NO + GO + VO << " [Norm:" << NO << ", Veg : " << GO << ", VIP :" << VO << "] " << endl;
	out << "Cooks:" << NumOfNC + NumOfGC + NumOfVC << "  [Norm:" << NumOfNC << ", Veg:" << NumOfGC << ", VIP:" << NumOfVC << " ,injured:" << Injured_cooks <<" ]" << endl;
	out << "Avg Wait =" << total_wait_time / total_wait_ord << " , Avg Serv =" << total_serv / total_ord << endl;
	out << "Urgent Orders: "<< NumOfUrgentVO<<",  Auto-Promoted :" << (float(NumOfAutoPNO) / float(NumOfAutoPNO + NO))*100<<"%"<< endl;
	out.close();
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
	Cook* top;
	string cook_type;
	char ord_type;
	while (Assigned_cook.dequeue(top)) {
		if (top->GetType() == TYPE_VGAN) { cook_type = "GC"; }
		else if (top->GetType() == TYPE_NRM) { cook_type = "NC"; }
		else { cook_type = "VC"; }
		if (top->getCurrentOrder()->GetType() == TYPE_VGAN) { ord_type = 'G'; }
		else if (top->getCurrentOrder()->GetType() == TYPE_NRM) { ord_type = 'N'; }
		else { ord_type = 'V'; }

		pGUI->PrintSeveral(cook_type+ to_string(top->GetID()) + " ( "
			+ord_type +"O"+ to_string(top->getCurrentOrder()->GetID())
			+ " )");
	}

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
		Waiting_VO.insertSorted(neworder);
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
	Waiting_VO.insertSorted(ord);
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
	int Urgent_Count = 0;
	
	if (!Waiting_Urgent.isEmpty())
	{
		Order** Urgent_Array = Waiting_Urgent.toArray(Urgent_Count);
		for (int i = 0; i < Urgent_Count; i++)
		{
			Urgent_Array[i]->setWaitingTime(Urgent_Array[i]->getWaitingTime() + 1);
		}
		delete[] Urgent_Array;
	}
	
	if (!Waiting_VO.isEmpty())
	{
		Order** VO_Array = Waiting_VO.toArray();
		int VO_Count = Waiting_VO.getCount();
		for (int i = 0; i < VO_Count; i++)
		{
			VO_Array[i]->setWaitingTime(VO_Array[i]->getWaitingTime() + 1);
			//The time step after this will be the one were the VIP order cannot wait any longer and should be assigned
			
			//VO_Array[i]->getWaitingTime() == VIP_WT , This line is the condition for the VIPs to become urgent
			//( VO_Array[i]->getWaitingTime() - AutoP) == VIP_WT , This line is the condition for the Normal who have been autopromoted to become urgent
			if (VO_Array[i]->getWaitingTime() == VIP_WT || ( VO_Array[i]->getWaitingTime() - AutoP) == VIP_WT)
			{
				//It's time to be an URGENT order
				Waiting_VO.deleteNode(VO_Array[i]);
				Waiting_Urgent.enqueue(VO_Array[i]);
				NumOfUrgentVO++;
			}
		}
		delete[] VO_Array;
	}
	
	if (!Waiting_NO.isEmpty())
	{
		Order** NO_Array = Waiting_NO.toArray();
		//We have to store the Waiting_NO.getCount() in a variable first before using it inside the for's condition
		//Because if we face an auto promoted order the Waiting_NO.getCount() will decrease by 1,
		//and the for loop will exited earlier than it should do
		int NO_Count = Waiting_NO.getCount();
		for (int i = 0; i < NO_Count; i++)
		{
			NO_Array[i]->setWaitingTime(NO_Array[i]->getWaitingTime() + 1);
			if (NO_Array[i]->getWaitingTime() == AutoP)
			{
				Waiting_NO.deleteNode(NO_Array[i]);
				NO_Array[i]->SetType(TYPE_VIP);
				ToVIP(NO_Array[i]);
				NumOfAutoPNO++;
			}
		}
		delete[] NO_Array;
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

bool Restaurant::Assign_To_InBreak_Cook(Order* InSRV_O, Cook* &AC)
{

	if (!in_break.isEmpty())
	{
		AC = in_break.dequeue(); // peeking 
		AC->setStatus(BUSY); //update the cooker'status to "BUSY"
		AC->setCurrentOrder(InSRV_O); // assign the order to the cooker
		Assigned_cook.enqueue(AC);
		return true;
	}
	return false;
}

bool Restaurant::Assign_To_InRest_Cook(Order* InSRV_O, Cook* &AC)
{

	if (!in_rest.isEmpty())
	{
		//His Speed is already set to its half
		in_rest.dequeue(AC); // peeking 
		AC->setStatus(BUSY); //update the cooker'status to "BUSY"
		AC->setCurrentOrder(InSRV_O); // assign the order to the cooker
		Assigned_cook.enqueue(AC);
		AC->setHad_Urgent(true);
		return true;
	}
	return false;
}

bool Restaurant::Assign_To_VC(Order* InSRV_O, Cook* &AC)
{

	if (!Available_VC.isEmpty())
	{
		Available_VC.dequeue(AC); // peeking 
		AC->setStatus(BUSY); //update the cooker'status to "BUSY"
		AC->setCurrentOrder(InSRV_O); // assign the order to the cooker
		Assigned_cook.enqueue(AC);
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
		Assigned_cook.enqueue(AC);
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
		Assigned_cook.enqueue(AC);
		return true;
	}
	return false;
}

//****************************injury ************************

void Restaurant::InjureACook(int currtime)
{
	float R;// Random number expresses the possibility of injury

	Cook* injuredC;
	Order* injuredC_Ord;

	R = float(rand() % 11) / 10; //Number between (0->10) is generated then divided by 10 (0, 0.1, 0.2, 0.3, ..... 0.9, 1)

	//The cook will be injured if he isn't an injured cook already
	//The cook will be injured if the generated random no. is < = the no. read from the input_file
	//The cook won't be injured if he was taken from his rest and serving an urgent order
	if (R <= InjProp && !busy_cooks.isEmpty() && busy_cooks.Peek()->getStatus() != INJURED && !((busy_cooks.Peek())->getHad_Urgent()))
	{
		//the dequeue and the enqueue are made to enter the cook at the correct position after injury
		//the same will be done with the order assigned to this injured cook (to reEnter the order in its correct position after changing the FT)


		injuredC = busy_cooks.dequeue();

		injuredC_Ord = Being_Served.dequeue(); 
		
		int old_true_component_of_SRV_time = currtime - injuredC_Ord->getArrTime() - injuredC_Ord->getWaitingTime(); //The order is being cooked for about
		int OldSpeed = injuredC->getSpeed(); //The old speed for the cook before injury
		int Finished_Dishes = old_true_component_of_SRV_time * OldSpeed; //Number of finished dishes untill this time step but not including it 
		int UNFinished_Dishes = (injuredC_Ord->getOrderSize()) - Finished_Dishes;

		injuredC->setSpeed(float(injuredC->getSpeed()) / 2); //a problem with the speed's int division
		int NewSpeed = injuredC->getSpeed();
		int new_component_of_SRV_time = ceil(float(UNFinished_Dishes) / NewSpeed); //calculation of the new part of the service time
		
		injuredC_Ord->setServTime(old_true_component_of_SRV_time + new_component_of_SRV_time);
		int FT = injuredC_Ord->getArrTime() + injuredC_Ord->getServTime() + injuredC_Ord->getWaitingTime();// calculation of finished time
		injuredC_Ord->setFinishTime(FT);
		injuredC->setGot_Injured(true);
		//Note that I must enqueue it to the busy cooks with a status of BUSY, inorder to insert it based on the least finised time
		busy_cooks.enqueue(injuredC);
		Being_Served.enqueue(injuredC_Ord);
		//This is why we changed the status after the enqueue
		injuredC->setStatus(INJURED);
		
		
	}
	//*******************************************************************
}

void Restaurant::Middle_Stage(int currtime)
{
	
	Order* InSRV_O;// the order that will be served

	Cook* AC; // Available cooker for any type
	
	///////////////////////////////////////// URGENTS ASSIGNMENT ////////////////////////////////

	while (!Waiting_Urgent.isEmpty())
	{
		Waiting_Urgent.peekFront(InSRV_O);
		if (Assign_To_VC(InSRV_O, AC) || Assign_To_NC(InSRV_O, AC) || Assign_To_GC(InSRV_O, AC) || Assign_To_InBreak_Cook(InSRV_O, AC) || Assign_To_InRest_Cook(InSRV_O, AC))
		{
			Order* temp; // just because of the implementation of dequeue function in class Queue
			Waiting_Urgent.dequeue(temp);
			int ST = ceil(float(InSRV_O->getOrderSize()) / AC->getSpeed()); //calculation of serving time
			InSRV_O->setServTime(ST);
			int FT = InSRV_O->getArrTime() + InSRV_O->getServTime() + InSRV_O->getWaitingTime();// calculation of finished time
			InSRV_O->setFinishTime(FT);
			InSRV_O->setStatus(SRV);
			busy_cooks.enqueue(AC);
			Being_Served.enqueue(InSRV_O); //move to being served list
		}
		else
		{
			break;
		}	
	}

	//----------------1) for vip order assignment ---------------
	while (!Waiting_VO.isEmpty())
	{
		InSRV_O = Waiting_VO.peekHead();//peek the highest priority order but without deleting yet

		if (Assign_To_VC(InSRV_O, AC) || Assign_To_NC(InSRV_O, AC) || Assign_To_GC(InSRV_O, AC))
		{
			Waiting_VO.Remove_Head();// after confirming the order status == srv , delete it from waiting list
			int ST = ceil(float(InSRV_O->getOrderSize()) / AC->getSpeed()); //calculation of serving time
			InSRV_O->setServTime(ST);
			int FT = InSRV_O->getArrTime() + InSRV_O->getServTime() + InSRV_O->getWaitingTime();// calculation of finished time
			InSRV_O->setFinishTime(FT);
			InSRV_O->setStatus(SRV);
			busy_cooks.enqueue(AC);
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
			InSRV_O->setStatus(SRV);
			busy_cooks.enqueue(AC);
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
			InSRV_O->setStatus(SRV);
			busy_cooks.enqueue(AC);
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

void Restaurant::ExitBusyList(Cook* &c,int currenttime) 
{
	
	c->setFinishedOrders(c->getFinishedOrders() + 1);
	
	//When an injured cook finishes his order
	if (c->getStatus() == INJURED)
	{
		//Inorder to dequeue the correct cook, the cook should have a status of busy
		c->setStatus(BUSY); //al str da mlo4 lzma
		busy_cooks.dequeue();
		c->setCurrentOrder(nullptr);
		//We should make his's status INJURED so that he can exit the in_rest list correctly
		c->setStatus(INJURED); //al str da mlo4 lzma
		in_rest.enqueue(c);
		c->setEndRestTime(currenttime + (c->getRestperiod()));
	}
	else if (c->getFinishedOrders() % BO == 0) //meets the no of  orders before break 
	{
		busy_cooks.dequeue();
		c->setCurrentOrder(nullptr);
		c->setStatus(BREAK);
		c->setEndBreakTime(currenttime + c->getBreakDuration());
		in_break.enqueue(c);
		if (c->getHad_Urgent())
		{
			//We should restore his original Speed
			c->setSpeed(c->getNormalSpeed());
			c->setHad_Urgent(false);
		}
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

void Restaurant::ExitRestList(int currenttime) 
{
	if (in_rest.isEmpty())return;
	Cook* c;
	
	while (in_rest.peekFront(c) && c->getEndRestTime() == currenttime)
	{
		c->setSpeed(c->getNormalSpeed());
		ToAvailableList(c);
	}
}

void Restaurant::ToAvailableList(Cook*& c) {
	ORD_TYPE type = c->GetType();
	COOK_STATUS stat =c->getStatus();
	if (stat == BUSY) {
		busy_cooks.dequeue();
	}
	else if (stat == BREAK) {
		in_break.dequeue();
	}
	else if (stat == INJURED)
	{
		in_rest.dequeue(c);
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
	c->setCurrentOrder(nullptr); 
}
void Restaurant::ThirdStage(int currenttime) {
	ExitBreakList(currenttime);
	ExitRestList(currenttime);
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
		c = busy_cooks.Peek();
		ExitBusyList(c, currenttime);
		ord->setStatus(DONE);
		ord = Being_Served.Peek();
	}
}

//***********************************End Third Stage Functions*****************************