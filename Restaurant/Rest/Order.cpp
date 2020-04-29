#include "Order.h"

//Order::Order()
//{ 
//}

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	setWaitingTime(0);
	setFinishTime(-1);
	setServTime(-1);
}

Order::Order(int id, ORD_TYPE r_type, double money, int r_size, int etime) {
	ID = (id > 0 && id < 1000) ? id : 0;
	type = r_type;
	totalMoney = money;
	orderSize = r_size;
	ArrTime = etime;
	status = WAIT;
	
	//when the order has just arrived(created), the time it waited is initially 0
	setWaitingTime(0);   

	//-1 is an itial value indicates it hasnot been assigned to a cook yet
	setFinishTime(-1);
	setServTime(-1);
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}

ORD_TYPE Order::GetType() const
{
	return type;
}

void Order::SetType(ORD_TYPE t)
{
	type = t;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

void Order::setTotalMoney(double M)
{
	totalMoney = M;
}

double Order::getTotalMoney() const
{
	return totalMoney;
}

void Order::setOrderSize(int ordsize)
{
	orderSize = ordsize;
}

int Order::getOrderSize() const
{
	return orderSize;
}

void Order::setArrTime(int T)
{
	ArrTime = T;
}

int Order::getArrTime() const
{
	return ArrTime;
}

void Order::setServTime(int T)
{
	ServTime = T;
}

int Order::getServTime() const
{
	return ServTime;
}

void Order::setFinishTime(int T)
{
	FinishTime = T;
}

int Order::getFinishTime() const
{
	return FinishTime;
}

void Order::setWaitingTime(int T)
{
	WaitingTime = T;
}

int Order::getWaitingTime() const
{
	return WaitingTime;
}

bool Order::operator==(const Order& O2)
{
	return (ID == O2.ID);
}

// priority of orders
bool Order::operator>(const Order& O2)
{
	if (status == WAIT && type == TYPE_VIP)
	{
		// priority equation for VIB orders 
		return ((0.3 * ArrTime + 0.5 * totalMoney - 0.2 * orderSize) >
				(0.3 * O2.ArrTime + 0.5 * O2.totalMoney - 0.2 * O2.orderSize));
	}
	else if (status == SRV)
	{
	
		if (FinishTime == O2.FinishTime)
			return (ServTime < O2.ServTime);
		else
			return (FinishTime < O2.FinishTime);

	}
	
	// just any valid comparison
	return (ArrTime < O2.ArrTime);
}

bool Order::operator==(const int& id)
{
	return ID == id;
}

