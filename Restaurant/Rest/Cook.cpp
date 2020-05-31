#include "Cook.h"
#include <exception>

Cook::Cook()
{
	status = AVAILABLE;
	currentOrder = nullptr;
	finishedOrders = 0;
	Got_Injured = false;
	Had_Urgent = false;
}


Cook::~Cook()
{
	currentOrder = nullptr;
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
	if (s < 0)
		throw "Unvalid Speed";
	
	//If the speed was 1 and got int-divided by 2 to be 0, then we will restore it to be 1
	//i.e if the speed was 1, it will keep saturated at 1 :""
	if (s == 0)
	{ speed = 1; }

	speed = s;
}

int Cook::getSpeed() const
{
	return speed;
}

void Cook::setNormalSpeed(int s)
{
	if (s <= 0)
		throw "Unvalid Speed";

	normal_speed = s;
}

int Cook::getNormalSpeed()
{
	return normal_speed;
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

void Cook::setHad_Urgent(bool value)
{
	Had_Urgent = value;
}

bool Cook::getHad_Urgent()const
{
	return Had_Urgent;
}
void Cook::setGot_Injured(bool value) {
	Got_Injured = value;
}
bool Cook::getGot_Injured()const {
	return Got_Injured;
}

bool Cook::operator==(const Cook& C2) const
{
	return (ID == C2.ID);
}

// priority of cooks
bool Cook::operator>(const Cook& C2) const
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


