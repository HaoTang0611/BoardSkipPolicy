//----------------------------------------------------------------------------------------------//
//---------------- Version 1.0.0.4 ---- Author : Joe ----------------------- Date : 20220808 ---//
//----------------------------------------------------------------------------------------------//
/*
Visual studio 2015
Based on openCV(2.4.13.6)
*/

//Z = aX + bY + c
struct SParam_RegressionPlane
{
	double A;
	double B;
	double C;
	double GetAngleX() { return A * 180 / 3.1415926; }
	double GetAngleY() { return B * 180 / 3.1415926; }
	double PredictZ(int x, int y) { return A * x + B * y + C; }
};

class Regression_Joe
{
#pragma region Properties
private:
	//LU=0, SVD=1, EIG=2, CHOLESKY=3, QR=4, NORMAL=16
	const int Default_TypeInv = 16;
	SParam_RegressionPlane Param_Plane;
	SParam_RegressionPlane Param_DeSkew;
	int ThreadNum_Omp = 1;

public:
	int TypeInv = Regression_Joe::Default_TypeInv;
#pragma endregion

#pragma region Func
public:
	//Set MultiThread for calculation...
	void Set_MultiThreadNum(int num);
	
	//Get RegressionPlane Param
	const SParam_RegressionPlane Get_Param_Plane() { return this->Param_Plane; }
	//Get RegressionPlane Param
	const SParam_RegressionPlane Get_Param_DeSkew() { return this->Param_DeSkew; }

	//Regression Model : z = a + bx + cx^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
	void Regression_Polynomial_X(void* dataStart, int width, int height, int type, int times = 2);
	//Regression Model : z = a + by + cy^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
	void Regression_Polynomial_Y(void* dataStart, int width, int height, int type, int times = 2);
	//Regression Model : z = a + by + cy^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
	void Regression_Polynomial_Block_X(void* dataStart, int width, int height, int type, int times, int size);
	//Regression Model : z = a + by + cy^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
	void Regression_Polynomial_Block_Y(void* dataStart, int width, int height, int type, int times, int size);

	//psrc : unwrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
	void Calibrate_BasePhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int times = 2);
	//psrc : unwrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
	void Calibrate_BasePhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int timesX, int timesY);
	//psrc : unwrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
	void Calibrate_BasePhase_Block(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int times = 2, int blocksize = 64);
	//psrc : unwrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
	void Calibrate_BasePhase_Block(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int timesX, int timesY, int blocksizeX, int blocksizeY);

	//psrc : height factor,   typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
	void Calibrate_HeightFactor(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int times);
	//psrc : height factor,   typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
	void Calibrate_HeightFactor(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int timesX, int timesY);

	//獲取最佳切平面, Mask > 0 vaild, Mask = 0 skip
	void Regression_Linear_Plane(void* psrc, int width, int height, int typeSrc, void* pmask = nullptr, int typeMask = 0);
	//傾斜校正(原始資訊-最佳切平面), Mask > 0 vaild, Mask = 0 skip
	void Regression_Linear_DeSkew(void* psrc, int width, int height, int typeSrc, void* pmask = nullptr, int typeMask = 0);
#pragma endregion

};