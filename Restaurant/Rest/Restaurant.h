
#ifndef __RESTAURANT_H_/
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\LinkedList.h"
#include "..\Generic_DS\priorityQueue.h"
#include "..\Events\Event.h"



#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	//
	// TODO: Add More Data Members As Needed
	//Add a queue for G orders Named Waiting_GO //GO stands for Vegan Orders
	Queue<Order*> Waiting_GO;	

	//Add a linkedlist for N orders Named Waiting_NO //NO stands for Normal Orders
	LinkedList <Order*> Waiting_NO;
	
	//hala // will temporary create a queue for the V orders Named Waition_VO//VO for VIP orders
	//this will instatnly be changed once we have a priority queue
	//but just for the time being to test simulating function
	LinkedList<Order*> Drawing;   //hala//added to point to all orders//to be used in filldrawinglist function
    PriorityQueue<Order*> Waiting_VO;// menna//i changed it to priorityQueue

	PriorityQueue<Order*> Being_Served; // for all types of orders that are being served orders
	Queue<Order*> Finshed_orders; // for all types of orders that have been served

	Queue<Cook*> Available_NC; //Available Normal Cooks List
	Queue<Cook*> Available_GC; //Available Vegan Cooks List
	Queue<Cook*> Available_VC; //Available VIP Cooks List

	PriorityQueue<Cook*> busy_cooks; // for cooks who have been assigned to order
	PriorityQueue<Cook*> in_break; // for cooks who in break duration

	//We will need this in the output file
	int NumOfAutoPNO = 0; //Number of autopromoted normal orders
	
	//the number of cooks of different types
	int NumOfNC; //For Noraml Cooks
	int NumOfGC; //For Vegan Cooks
	int NumOfVC; //For VIP Cooks
	int NumOfEvents; //Number of Total Events
	
	int SpeedOfNC, SpeedOfGC, SpeedOfVC;

	int BO; //the number of orders a cook must prepare before taking a break
	int BreakOfNC; //the break duration (in timesteps) for normal cooks
	int BreakOfGC; //the break duration (in timesteps) for Vegan cooks
	int BreakOfVC; //the break duration (in timesteps) for VIP cooks

	int AutoP; //the number of timesteps after which an order is automatically promoted to VIP.
	int Ordassigned=0; //will need this to change the condition of ending the simulation
	int Finished_VO=0;
	int Finished_GO = 0;
	int Finished_NO = 0;
	void inValidFormat(); //an error handling function

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//Executes ALL events that should take place at current timestep
	void RunSimulation();
	void SimpleSimulator(PROG_MODE);
	void LoadFile();
	void OutFile();
	
	// GUI Functions
	void FillDrawingList();
	void PrintInfo(int);

	// Helper Functions
	int CharToNum(char Type); //This function returns the number corresponding to each type's symbol
							  //i.e N --->0
							  //i.e G --->1
							  //i.e V --->2
	void ToWaitingList(Order* neworder);//this function changes an arrival event to an order
										//it places it in the appropriate waiting list 
	Order* RemoveFromWaiting_NO(int id);
	void RemoveFromDrawing(Order*);
	void ToVIP(Order*);
	void SetOrdAssigned(int);
	int GetOrdAssigned();  //to be used inside cancel event 
	
	// Middle Stage Functions
	void Middle_Stage(int currtime);
	void increment_Waiting_Time(); //This function increments the waiting time for all the orders that haven't been assigned in this timestep
	bool Assign_To_VC(Order*, Cook* &);
	bool Assign_To_NC(Order*, Cook* &);
	bool Assign_To_GC(Order*, Cook* &);

	void ThirdStage(int currenttime);
	void ExitBusyList(Cook*&,int);
	void ExitBreakList(int);
	void ToAvailableList(Cook*&);

};

#endif