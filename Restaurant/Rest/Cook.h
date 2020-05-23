#pragma once

#include "..\Defs.h"
#include "Order.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	COOK_STATUS status; //status of this cook
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int normal_speed;
	int finishedOrders; //number of orders that this cook has finished 
	int breakDuration;  //duration of the break
	Order* currentOrder; //the order that being survied now by this cook
	int endBreakTime; // time step at which the break for this cook will end
	int endRestTime; // time step at which the rest for this cook will end
	int rst_prd;//Rest Period for medication
	
	//This flag is used to keep track of the cooks that has been assigned to an urgent order, so that we can know when to restore it's speed. 
	bool Had_Urgent; //true if the cook has been assigned to an urgent order // false, if it hasn't been assigned to an urgent order
	
public:
	Cook();
	virtual ~Cook();

	int GetID() const;
	ORD_TYPE GetType() const;

	void setStatus(COOK_STATUS);
	COOK_STATUS getStatus() const;

	void setID(int);
	void setType(ORD_TYPE);

	void setSpeed(int s);
	int getSpeed() const;

	void setNormalSpeed(int s);
	int getNormalSpeed();

	void setFinishedOrders(int orders);
	int getFinishedOrders() const;

	void setBreakDuration(int d);
	int getBreakDuration() const;

	void setRestperiod(int d);
	int getRestperiod() const;

	void setCurrentOrder(Order*);
	Order* getCurrentOrder() const;

	void setEndBreakTime(int);
	int getEndBreakTime()const;

	void setEndRestTime(int);
	int getEndRestTime()const;

	void setHad_Urgent(bool);
	bool getHad_Urgent()const;

	bool operator==(const Cook&) const;
	bool operator>(const Cook&) const;

};
