#pragma once
#include "JETAlg_Std.h"
#include <memory>
#include <string>

namespace cv {
	class Mat;
}

namespace JET {
	namespace alg {

		enum class ESuperRes_Alg {
			ESPCN,			
			FSRCNN,
			LapSRN,
			EDSR,
		};

		//based on opencv3_4_16
		class JETALG_API CSuperResolution
		{
		public:
			//此預設方法為ESPCN，放大倍率為4倍(此函示會自動載入dll資源的模型檔.pb)
			explicit CSuperResolution();
			//ESPCN 已內嵌至dllresource.h中，其餘FSRCNN、LapSRN、EDSR需要額外讀取.pb檔
			explicit CSuperResolution(ESuperRes_Alg alg, int scale, std::string modelpath);
			~CSuperResolution();

			//src的寬、高、型態(CV_8UC1、CV_8UC3、CV_32FC3 for normal image, CV_32FC1 for point cloud)
			bool UpSample(const cv::Mat& src, cv::Mat& dst);
		
			//src的寬、高、型態(CV_8UC1、CV_8UC3、CV_32FC3 for normal image, CV_32FC1 for point cloud)   
			//dst must equal nullptr, dst記憶體須記得釋放(delete[] dst)
			//new_cols = cols*4
			//new_rows = rows*4
			//lenDst = new_cols * new_rows * channels
			bool UpSample(size_t cols, size_t rows, int cvType, unsigned char* src, unsigned char*& dst, size_t& lenDst);

			std::shared_ptr<std::string> GetErrorString();
			std::shared_ptr<std::string> GetPathModel();
			int GetScale() { return this->m_Scale; }
						
		protected:
			bool isSupportType(int ctype);
			bool loadNet(ESuperRes_Alg alg = ESuperRes_Alg::ESPCN, int scale = 4, std::string modelpath = "");
			void setErrorStr(std::string s);
			void preProcess(const cv::Mat& inpImg, cv::Mat& outImg);
			void postProcess(const cv::Mat& inpImg, const cv::Mat& origImg, cv::Mat& outImg, int scale);

		private:
			void* m_Net = nullptr;
			ESuperRes_Alg m_Alg;
			std::string m_PathModel;
			bool m_IsLoadNet;
			int m_Scale;
			float m_NormScale;
			double m_Min;
			double m_Max;
			std::string m_ErrorStr;
		};
	}
}
