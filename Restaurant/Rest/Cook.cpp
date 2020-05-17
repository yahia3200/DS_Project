#include "Cook.h"
#include <exception>

Cook::Cook()
{
	status = AVAILABLE;
	currentOrder = nullptr;//menna
	finishedOrders = 0;
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
	if (s <= 0)
		throw "Unvalid Speed";

	speed = s == 0 ? 1 : s;
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

void Cook::setRestperiod(int d)
{
	rst_prd = d;
}

int Cook::getRestperiod() const
{
	return rst_prd;
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

int Cook::getEndBreakTime() const
{
	return endBreakTime;
}

void Cook::setEndRestTime(int r)
{
	endRestTime = r;
}

int Cook::getEndRestTime() const
{
	return endRestTime;
}

bool Cook::operator==(const Cook& C2)
{
	return (ID == C2.ID);
}

// priority of cooks
bool Cook::operator>(const Cook& C2)
{
	Order* O1 = this->getCurrentOrder();
	Order* O2 = C2.getCurrentOrder();

	if (status == BUSY || status == INJURED)
	{
		 if (!O1 || !O2)
		 {
			 // this case should never happen
			 throw "Busy Cook Without Order";
		 } 
		else
		{
			

			if (O1->getFinishTime() == O2->getFinishTime())
				return (O1->getServTime() <= O2->getServTime());
			else
				return (O1->getFinishTime() <= O2->getFinishTime());
		
			
		}
	}

	 else if (status == BREAK) {
		 if (!this->currentOrder) return true;
		 if (!C2.currentOrder) return false;
		 else
		 {
			 return (this->getEndBreakTime() <
				 C2.getEndBreakTime());
		 }
	 }

	
	// Undefined priority
	throw "Undefined Priority";
}


