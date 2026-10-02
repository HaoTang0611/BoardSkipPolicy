#ifndef _MOTIONDATASTRUCTURE_H_
#define _MOTIONDATASTRUCTURE_H_

#include <vector>
#include <utility>

struct PEG_Data 
{
	bool Enable;
	double StartPosition;
	double EndPosition;
	double Interval;
	double TimeWidth;

	PEG_Data()
	{
		Enable = false;
		StartPosition = 0;
		EndPosition = 0;
		Interval = 1;
		TimeWidth = 0.01;
	}

	PEG_Data& operator=(const PEG_Data &rhs)
	{
		this->Enable = rhs.Enable;
		this->StartPosition = rhs.StartPosition;
		this->EndPosition = rhs.EndPosition;
		this->Interval = rhs.Interval;
		this->TimeWidth = rhs.TimeWidth;	

		return (*this);
	}
};

struct HOME_Data
{
	double PreMovingDist;
	double OrgOffset;
	double Velocity;

	HOME_Data()
	{
		PreMovingDist = 5000;
		OrgOffset	  = 5000;
		Velocity	  = 10000;
	}

	HOME_Data& operator=(const HOME_Data &rhs)
	{
		this->PreMovingDist = rhs.PreMovingDist;
		this->OrgOffset	  = rhs.OrgOffset;
		this->Velocity	  = rhs.Velocity;
		return (*this);
	}

};

struct JOG_Data
{
	double MaxVelocity;
	double MinVelocity;
	double AccelerationTime;
	double DecelerationTime;

	JOG_Data()
	{
		MaxVelocity		= 3000;
		MinVelocity		= 1000;
		AccelerationTime= 0.15;
		DecelerationTime= 0.15;
	}

	JOG_Data& operator=(const JOG_Data &rhs)
	{
		this->MaxVelocity		= rhs.MaxVelocity;
		this->MinVelocity		= rhs.MinVelocity;
		this->AccelerationTime= rhs.AccelerationTime;
		this->DecelerationTime= rhs.DecelerationTime;
		return (*this);
	}

};



#endif
