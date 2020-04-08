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


public:
	Order();//menna// added a default constructure needed for class TNode (copy constructor)
	Order(int ID, ORD_TYPE r_Type);
	//hala// added a constructor to facilitate creating an order object
	//etime for the arrival time
	Order(int id, ORD_TYPE r_type, double money, int r_size, int etime);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	void setTotalMoney(double M);
	double getTotalMoney() const;

	void setOrderSize(int ordsize);
	int getOrderSize() const;

	void setArrTime(int T);
	int getArrTime() const;

	void setServTime(int T);
	int getServTime() const;

	void setFinishTime(int T);
	int getFinishTime() const;

	void setWaitingTime(int T);  //Finish Time will be set also here
	int getWaitingTime() const;

};

#endif