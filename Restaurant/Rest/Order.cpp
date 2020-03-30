#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
}
Order::Order(int id, ORD_TYPE r_type, double money, int r_size, int etime) {
	ID = (id > 0 && id < 1000) ? id : 0;
	type = r_type;
	totalMoney = money;
	orderSize = r_size;
	ArrTime = etime;
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

	FinishTime = ArrTime + WaitingTime + ServTime;
}

int Order::getWaitingTime() const
{
	return WaitingTime;
}

