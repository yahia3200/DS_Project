#ifndef __RESTAURANT_H_\
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\LinkedList.h"
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
	Queue<Order*> Waiting_GO;	//Important: This is just for demo

	//Noran //Add a linkedlist for N orders Named Waiting_NO //NO stands for Normal Orders
	LinkedList<Order*> Waiting_NO;

	//Noran //Add a priority queue for V orders Named Waiting_VO //VO stands for VIP Orders
	//Noran //Need the priority queue implementation

	//

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	
	
	
public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	


	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//


/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

/// ================================================================================================== 



};

#endif