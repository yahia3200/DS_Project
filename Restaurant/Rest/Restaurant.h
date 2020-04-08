
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
	//Noran //Add a queue for G orders Named Waiting_GO //GO stands for Vegan Orders
	Queue<Order*> Waiting_GO;	

	//Noran //Add a linkedlist for N orders Named Waiting_NO //NO stands for Normal Orders
	LinkedList <Order*> Waiting_NO;

	//Noran //Add a priority queue for V orders Named Waiting_VO //VO stands for VIP Orders
	//Noran //Need the priority queue implementation
	
	//hala // will temporary create a queue for the V orders Named Waition_VO//VO for VIP orders
	//this will instatnly be changed once we have a priority queue
	//but just for the time being to test simulating function

	Queue<Order*> Waiting_VO;// menna//i changed it to priorityQueue

	Queue<Cook*> Available_NC; //Available Normal Cooks List
	Queue<Cook*> Available_GC; //Available Vegan Cooks List
	Queue<Cook*> Available_VC; //Available VIP Cooks List

	//Noran//We will need this in the output file
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
	
	void inValidFormat(); //an error handling object

	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	
	
	
public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	ORD_TYPE PriorityDrawing(int& VoTime, int& NoTime, int& GoTime);

	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//
	void LoadFile();
	int CharToNum(char Type); //This function returns the number corresponding to each type's symbol
	//i.e N --->0
	//i.e G --->1
	//i.e V --->2
	void SimpleSimulator();
	//hala //not yet sure if i needed to make it private or not
	//this function changes an arrival event to an order 
	//it places it in the appropriate waiting list 
	void ToWaitingList (Order * neworder);

/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 



};

#endif