#include "Cook.h"


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

	 if (status == BUSY)
	{
		if (!this->currentOrder) return true;
		if (!C2.currentOrder) return false;
		else
		{
			Order* O1 = this->getCurrentOrder();
			Order* O2 = C2.getCurrentOrder();

			//Delete these comments before submitting the project
			//This commented status is not fully correct 
			//Because we compare the FinishTime of the orders
			//What if the FinishTimes are equal then the most prior node is C2 not this*
			//While in the order's operator overloading, the most prior node is this->Order not C2->Order
			//Because we compare with both the finish and the servece time, and if both are equal
			//Therfore the most appropriate way to compare here is by orders (order's operator overload), not the order's finish time
			
			//return (this->currentOrder->getFinishTime() < C2.currentOrder->getFinishTime());

			if (O1->getFinishTime() == O2->getFinishTime())
				return (O1->getServTime() <= O2->getServTime());
			else
				return (O1->getFinishTime() <= O2->getFinishTime());

			//To be more safe, i won't call the operator overload of order because it depends on the status of the order(and the place we change the status in)
			//i.e We can say this line insted, but to be more safe we won't: return (O1 > O2) i.e (this->getCurrentOrder() > C2->getCurrentOrder())
			
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

	// just any valid comparison
	return (speed > C2.speed);
}


