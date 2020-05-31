#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	                
	double totalMoney;	//Total order money

	int orderSize;  //Number of dishes

	int ArrTime, ServTime, FinishTime, WaitingTime;	//arrival, service start,  finish and waiting Time
	bool Was_promoted; //changed from normal to vip

public:
	//Order(); added a default constructure needed for class TNode (copy constructor)
	Order(int ID, ORD_TYPE r_Type);
	Order(int id, ORD_TYPE r_type, double money, int r_size, int etime);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;
	void SetType(ORD_TYPE);


	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	void setTotalMoney(double M);
	double getTotalMoney() const;

	int getOrderSize() const;

	int getArrTime() const;

	void setServTime(int T);
	int getServTime() const;

	void setFinishTime(int T);
	int getFinishTime() const;

	void setWaitingTime(int T);  
	int getWaitingTime() const;

	void set_Was_promoted(bool);
	bool get_Was_promoted()const;
	
	bool operator==(const Order&);
	bool operator>(const Order&);
	bool operator==(const int&);
};

#endif