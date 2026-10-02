//Author : Joe
#pragma once
#include "JETAlg_Std.h"
#include <memory>

namespace JET {
	namespace alg {
		/// <summary>
		/// Z = aX + bY + c
		/// </summary>
		struct JETALG_API SParam_RegressionPlane
		{
			double A;
			double B;
			double C;

			double GetAngleX();
			double GetAngleY();
			double PredictZ(int x, int y);
		};

		/// <summary>
		/// f(x) = a + bx + cx^2 + dx^3...
		/// </summary>
		struct JETALG_API SParam_RegressionPolynoial
		{			
			//把计计
			std::vector<double> Cof;
			//玡矪瞶(へ秸俱)
			double alpha = 1;

			SParam_RegressionPolynoial() = default;
			
			//Predicit(x) = f(x*alpha) = a + b(x*alpha) + c((x*alpha)^2) + ...
			inline double Predicit(double x);
					
			//把计﹃て
			std::shared_ptr<std::wstring> ParamToStr();
			//sparam = "alpha, a, b, c, d, ....."
			bool LoadFromStr(std::wstring sParam);

			//蹲把计郎
			bool SaveToFile(std::wstring filename);
			//弄把计郎
			bool LoadFromFile(std::wstring filename);

			//家干纕(dst = Predicit(src) = f(src); src籔dst琌夹
			template<typename T> void RefineMap(const T* src, T* dst, int width, int height) {
				if (Cof.size() <= 0) {
					return;
				}
				auto psrc = src;
				auto pdst = dst;
				double val = 0;
				double sx = 0;
				double tmpx = 0;
				auto totallen = height * width;
				for (int i = 0; i < totallen; i++) {
					val = Cof[0];
					sx = *psrc * alpha;
					for (int i = 1; i < Cof.size(); i++) {
						tmpx = sx;
						for (int j = 0; j < i - 1; j++) {
							tmpx *= sx;
						}
						val += Cof[i] * tmpx;
					}
					*pdst++ = val;
					psrc++;
				}
			}
		};
		extern JETALG_API SParam_RegressionPolynoial HeightDistortionCorrect;

		class JETALG_API CRegression
		{
		private:
			//LU=0, SVD=1, EIG=2, CHOLESKY=3, QR=4, NORMAL=16
			const int Default_TypeInv = 16;
			SParam_RegressionPlane m_Param_Plane;
			SParam_RegressionPlane m_Param_DeSkew;
			int m_ThreadNum_Omp = 1;

		public:
			int m_TypeInv = CRegression::Default_TypeInv;

		public:
			const char* GetVersion();

			//ㄌ沮芠诡(dataX)籔龟悔(dataY)―眔兜Α耴家家f(x) = y = a_0 + a_1*x^1 + ... + a_n*x^n;
			SParam_RegressionPolynoial Regression_Polynomial_Observed(unsigned char* dataX, unsigned char* dataY, size_t length, int times);
			SParam_RegressionPolynoial Regression_Polynomial_Observed(short* dataX, short* dataY, size_t length, int times);
			SParam_RegressionPolynoial Regression_Polynomial_Observed(float* dataX, float* dataY, size_t length, int times);
			SParam_RegressionPolynoial Regression_Polynomial_Observed(double* dataX, double* dataY, size_t length, int times);

			//莉览キ把计
			const SParam_RegressionPlane GetParam_Plane() { return this->m_Param_Plane; }
			//莉览キ把计
			const SParam_RegressionPlane GetParam_DeSkew() { return this->m_Param_DeSkew; }

			//Set MultiThread for calculation...
			void SetMultiThreadNum(int num);

			//Regression Model : z = a + bx + cx^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
			void Regression_Polynomial_X(void* dataStart, int width, int height, int type, int times = 2);
			//Regression Model : z = a + by + cy^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
			void Regression_Polynomial_Y(void* dataStart, int width, int height, int type, int times = 2);

			//Regression Model : z = a + bx + cx^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
			void Regression_Polynomial_Block_X(void* dataStart, int width, int height, int type, int times, int size);
			//Regression Model : z = a + bx + cx^2 + ..., type : equal CV_8U/CV_8S/CV_16S...
			void Regression_Polynomial_Block_Y(void* dataStart, int width, int height, int type, int times, int size);

			//psrc : unwrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_UnWrappingPhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int times);

			//psrc : wrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_BasePhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int times);
			//psrc : wrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_BasePhase(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int timesX, int timesY);

			//psrc : wrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_BasePhase_Block(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int times, int blocksize);
			//psrc : wrapping phase,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_BasePhase_Block(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, double maxPhase, int timesX, int timesY, int blocksizeX, int bloxksizeY);

			//psrc : height factor,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_HeightFactor(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int times);
			//psrc : height factor,  typeSrc, typeDst : equal CV_8U/CV_8S/CV_16S...
			void Calibrate_HeightFactor(const void* psrc, void* pdst, int width, int height, int typeSrc, int typeDst, int timesX, int timesY);

			//莉程ㄎちキ, Mask > 0 vaild, Mask = 0 skip
			void Regression_Linear_Plane(void* psrc, int width, int height, int typeSrc, void* pmask = nullptr, int typeMask = 0);
			//渡弊タ(﹍戈癟-程ㄎちキ), Mask > 0 vaild, Mask = 0 skip
			void Regression_Linear_DeSkew(void* psrc, int width, int height, int typeSrc, void* pmask = nullptr, int typeMask = 0);
		};
	}
}