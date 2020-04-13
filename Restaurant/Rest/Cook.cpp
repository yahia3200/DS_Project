#include "Cook.h"


Cook::Cook()
{
	status = AVAILABLE;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}

void Cook::setStatus(COOK_STATUS st)
{
	status = st;
}

COOK_STATUS Cook::getStatus() const
{
	return status;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}

void Cook::setSpeed(int s)
{
	speed = s;
}

int Cook::getSpeed() const
{
	return speed;
}

void Cook::setFinishedOrders(int orders)
{
	finishedOrders = orders;
}

int Cook::getFinishedOrders() const
{
	return finishedOrders;
}

void Cook::setBreakDuration(int d)
{
	breakDuration = d;
}

int Cook::getBreakDuration() const
{
	return breakDuration;
}

void Cook::setCurrentOrder(Order* O)
{
	currentOrder = O;
}

Order* Cook::getCurrentOrder() const
{
	return currentOrder;
}

void Cook::setEndBreakTime(int t)
{
	endBreakTime = t;
}

int Cook::getEndBreakTime()
{
	return endBreakTime;
}

bool Cook::operator==(const Cook& C2)
{
	return (ID == C2.ID);
}

// priority of cooks
bool Cook::operator>(const Cook& C2)
{
	if (status == AVAILABLE)
	{
		return (currentOrder->getFinishTime() < C2.currentOrder->getFinishTime());
	}
	else if (status == BUSY)
	{
		return (currentOrder->getFinishTime() + BREAK <
									C2.currentOrder->getFinishTime() + C2.breakDuration);
	}

	// just any valid comparison
	return (speed > C2.speed);
}


