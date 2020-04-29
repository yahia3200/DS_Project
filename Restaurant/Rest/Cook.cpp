#include "Cook.h"


Cook::Cook()
{
	status = AVAILABLE;
	currentOrder = nullptr;//menna
}


Cook::~Cook()
{
	currentOrder = nullptr;//menna
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
	// should be busy
	if (status == AVAILABLE)
	{
		if (!this->currentOrder) return true;
		if (!C2.currentOrder) return false;
		else
		{
			return (this->currentOrder->getFinishTime() < C2.currentOrder->getFinishTime());
		}
	}

	// should be Break
	else if (status == BUSY)
	{
		if (!this->currentOrder) return true;
		if (!C2.currentOrder) return false;
		else
		{
			return (this->currentOrder->getFinishTime() + BREAK <
				C2.currentOrder->getFinishTime() + C2.breakDuration);
		}
	}

	// just any valid comparison
	return (speed > C2.speed);
}


