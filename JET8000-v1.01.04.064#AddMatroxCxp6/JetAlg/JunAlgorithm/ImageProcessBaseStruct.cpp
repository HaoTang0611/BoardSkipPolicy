
#include "stdafx.h"
#include "ImageProcessBaseStruct.h"


namespace JET {
	namespace alg {
		using namespace std;

#pragma region Save Load

		bool FindEdgeParameterToConfig(const string& strAppName, const SFindEdge_Parameter& sParam, vector<ConfigData>& vtsConfig)
		{
			string strTxt;
			if (strAppName.empty() || !sParam.Check(strTxt)) {
				return false;
			}

			int nCount = vtsConfig.size();
			int nId = 0;
			if (nCount <= 0) {
				vtsConfig.resize(1);
				nId = 0;
			}
			else {
				nId = nCount;
				vtsConfig.resize(nCount + 1);
			}

			vtsConfig[nId].AppName = strAppName;
			vtsConfig[nId].AddKeyValue("ColorTransformMode", to_string(sParam.nColorTransformMode), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ColorThresholdMode", to_string(sParam.nColorThresholdMode), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("FilterMode", to_string(sParam.nFilterMode), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("MedianSize", to_string(sParam.nMedianSize), DATATYPE_INTEGER);

			// EnhanceMode
			if (sParam.sEnhanceMode.nMode > 0 && sParam.sEnhanceMode.nMode <= 2) {
				vtsConfig[nId].AddKeyValue("EnhanceMode", to_string(sParam.sEnhanceMode.nMode), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("EnhanceMode_Kmean_GroupNumber", to_string(sParam.sEnhanceMode.sKmean.nGroupNumber), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("EnhanceMode_Kmean_DarkId", to_string(sParam.sEnhanceMode.sKmean.nDarkId), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("EnhanceMode_Kmean_LightId", to_string(sParam.sEnhanceMode.sKmean.nLightId), DATATYPE_INTEGER);
			}

			// HSV Extraction
			if (sParam.nCount_HsvExtraction > 0) {
				if (sParam.nCount_HsvExtraction == sParam.vtsHsvExtraction.size()) {
					vtsConfig[nId].AddKeyValue("Count_HsvExtraction", to_string(sParam.nCount_HsvExtraction), DATATYPE_INTEGER);
					for (int k = 0; k < sParam.nCount_HsvExtraction; ++k) {
						string strId = to_string(k + 1);
						vtsConfig[nId].AddKeyValue("HsvExtraction" + strId + "_MeanH", to_string(sParam.vtsHsvExtraction[k].nMean_H), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("HsvExtraction" + strId + "_RangeH", to_string(sParam.vtsHsvExtraction[k].nRange_H), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("HsvExtraction" + strId + "_MinS", to_string(sParam.vtsHsvExtraction[k].nMin_S), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("HsvExtraction" + strId + "_MaxS", to_string(sParam.vtsHsvExtraction[k].nMax_S), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("HsvExtraction" + strId + "_MinV", to_string(sParam.vtsHsvExtraction[k].nMin_V), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("HsvExtraction" + strId + "_MaxV", to_string(sParam.vtsHsvExtraction[k].nMax_V), DATATYPE_INTEGER);
					}
				}
			}

			// RGB Extraction
			if (sParam.nCount_ColorExtraction > 0) {
				if (sParam.nCount_ColorExtraction == sParam.vtsColorExtraction.size()) {
					vtsConfig[nId].AddKeyValue("Count_ColorExtraction", to_string(sParam.nCount_ColorExtraction), DATATYPE_INTEGER);
					for (int k = 0; k < sParam.nCount_ColorExtraction; ++k) {
						string strId = to_string(k + 1);
						vtsConfig[nId].AddKeyValue("ColorExtraction" + strId + "_R", to_string(sParam.vtsColorExtraction[k].nValue_R), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ColorExtraction" + strId + "_G", to_string(sParam.vtsColorExtraction[k].nValue_G), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ColorExtraction" + strId + "_B", to_string(sParam.vtsColorExtraction[k].nValue_B), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("ColorExtraction" + strId + "_Rang", to_string(sParam.vtsColorExtraction[k].nRange), DATATYPE_INTEGER);
					}
				}
			}

			// GrayExtraction
			if (sParam.nCount_GrayExtraction > 0) {
				if (sParam.nCount_GrayExtraction == sParam.vtsGrayExtraction.size()) {
					vtsConfig[nId].AddKeyValue("Count_GrayExtraction", to_string(sParam.nCount_GrayExtraction), DATATYPE_INTEGER);
					for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
						string strId = to_string(k + 1);
						vtsConfig[nId].AddKeyValue("GrayExtraction" + strId + "_Gary", to_string(sParam.vtsGrayExtraction[k].nValue_Gray), DATATYPE_INTEGER);
						vtsConfig[nId].AddKeyValue("GrayExtraction" + strId + "_Rang", to_string(sParam.vtsGrayExtraction[k].nRange), DATATYPE_INTEGER);
					}
				}
			}

			// DeleteMode
			if (sParam.sDeleteMode.nMode > 0 && sParam.sDeleteMode.nMode <= 4) {
				vtsConfig[nId].AddKeyValue("DeleteMode", to_string(sParam.sDeleteMode.nMode), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("DeleteMode_MaxCount", to_string(sParam.sDeleteMode.nMaxCount), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("DeleteMode_ROI_Width_Std", to_string(sParam.sDeleteMode.nROI_Width_Std), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("DeleteMode_ROI_Width_Range", to_string(sParam.sDeleteMode.nROI_Width_Range), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("DeleteMode_ROI_Height_Std", to_string(sParam.sDeleteMode.nROI_Height_Std), DATATYPE_INTEGER);
				vtsConfig[nId].AddKeyValue("DeleteMode_ROI_Height_Range", to_string(sParam.sDeleteMode.nROI_Height_Range), DATATYPE_INTEGER);

				// SDeleteROI
				if (sParam.sDeleteMode.sROI.nCount > 0) {
					if (sParam.sDeleteMode.sROI.nCount == sParam.sDeleteMode.sROI.vtrectROI.size()) {
						vtsConfig[nId].AddKeyValue("DeleteMode_RoiCount", to_string(sParam.sDeleteMode.sROI.nCount), DATATYPE_INTEGER);
						for (int k = 0; k < sParam.sDeleteMode.sROI.nCount; ++k) {
							string strId = to_string(k + 1);
							vtsConfig[nId].AddKeyValue("DeleteROI" + strId + "_L", to_string(sParam.sDeleteMode.sROI.vtrectROI[k].left), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("DeleteROI" + strId + "_T", to_string(sParam.sDeleteMode.sROI.vtrectROI[k].top), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("DeleteROI" + strId + "_R", to_string(sParam.sDeleteMode.sROI.vtrectROI[k].right), DATATYPE_INTEGER);
							vtsConfig[nId].AddKeyValue("DeleteROI" + strId + "_B", to_string(sParam.sDeleteMode.sROI.vtrectROI[k].bottom), DATATYPE_INTEGER);
						}
					}
				}
			}

			vtsConfig[nId].AddKeyValue("OpenX", to_string(sParam.nOpenX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("OpenY", to_string(sParam.nOpenY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ConnectX", to_string(sParam.nConnectX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ConnectY", to_string(sParam.nConnectY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("DilateX", to_string(sParam.nDilateX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("DilateY", to_string(sParam.nDilateY), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ErosionX", to_string(sParam.nErosionX), DATATYPE_INTEGER);
			vtsConfig[nId].AddKeyValue("ErosionY", to_string(sParam.nErosionY), DATATYPE_INTEGER);

			return true;
		}

		bool FindEdgeConfigToParameter(const string& strAppName, const std::vector<ConfigData>& vtsConfig, SFindEdge_Parameter& sParam)
		{
			if (strAppName.empty() || vtsConfig.size() < 1) {
				return false;
			}

			ConfigFile sConfigfile;
			string tmp;

			// nColorTransformMode
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ColorTransformMode", tmp))
				sParam.nColorTransformMode = sConfigfile.parseInt(tmp);
			else
				sParam.nColorTransformMode = 1;

			// nColorThresholdMode
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ColorThresholdMode", tmp))
				sParam.nColorThresholdMode = sConfigfile.parseInt(tmp);
			else
				sParam.nColorThresholdMode = 1;

			// nFilterMode
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "FilterMode", tmp))
				sParam.nFilterMode = sConfigfile.parseInt(tmp);
			else
				sParam.nFilterMode = 1;

			// nMedianSize
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "MedianSize", tmp))
				sParam.nMedianSize = sConfigfile.parseInt(tmp);
			else
				sParam.nMedianSize = 1;

			// EnhanceMode
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "EnhanceMode", tmp))
			{
				sParam.sEnhanceMode.nMode = sConfigfile.parseInt(tmp);

				if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "EnhanceMode_Kmean_GroupNumber", tmp))
					sParam.sEnhanceMode.sKmean.nGroupNumber = sConfigfile.parseInt(tmp);
				else
					sParam.sEnhanceMode.sKmean.nGroupNumber = 2;

				if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "EnhanceMode_Kmean_DarkId", tmp))
					sParam.sEnhanceMode.sKmean.nDarkId = sConfigfile.parseInt(tmp);
				else
					sParam.sEnhanceMode.sKmean.nDarkId = 0;

				if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "EnhanceMode_Kmean_LightId", tmp))
					sParam.sEnhanceMode.sKmean.nLightId = sConfigfile.parseInt(tmp);
				else
					sParam.sEnhanceMode.sKmean.nLightId = 1;
			}
			else
				sParam.sEnhanceMode.nMode = 0;

			// ColorExtraction
			int nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "Count_ColorExtraction", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.nCount_ColorExtraction = nCount;
				sParam.vtsColorExtraction.resize(sParam.nCount_ColorExtraction);
				for (int k = 0; k < sParam.nCount_ColorExtraction; ++k) {
					string strId = to_string(k + 1);
					// R
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ColorExtraction" + strId + "_R", tmp))
						sParam.vtsColorExtraction[k].nValue_R = sConfigfile.parseInt(tmp);
					else
						sParam.vtsColorExtraction[k].nValue_R = 0;

					// G
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ColorExtraction" + strId + "_G", tmp))
						sParam.vtsColorExtraction[k].nValue_G = sConfigfile.parseInt(tmp);
					else
						sParam.vtsColorExtraction[k].nValue_G = 0;

					// B
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ColorExtraction" + strId + "_B", tmp))
						sParam.vtsColorExtraction[k].nValue_B = sConfigfile.parseInt(tmp);
					else
						sParam.vtsColorExtraction[k].nValue_B = 0;

					// Range
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ColorExtraction" + strId + "_Rang", tmp))
						sParam.vtsColorExtraction[k].nRange = sConfigfile.parseInt(tmp);
					else
						sParam.vtsColorExtraction[k].nRange = 1;
				}
			}



			// HSV Extraction
			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "Count_HsvExtraction", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.nCount_HsvExtraction = nCount;
				sParam.vtsHsvExtraction.resize(sParam.nCount_HsvExtraction);
				for (int k = 0; k < sParam.nCount_HsvExtraction; ++k) {
					string strId = to_string(k + 1);
					// H
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "HsvExtraction" + strId + "_MeanH", tmp))
						sParam.vtsHsvExtraction[k].nMean_H = sConfigfile.parseInt(tmp);
					else
						sParam.vtsHsvExtraction[k].nMean_H = 0;

					// H
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "HsvExtraction" + strId + "_RangeH", tmp))
						sParam.vtsHsvExtraction[k].nRange_H = sConfigfile.parseInt(tmp);
					else
						sParam.vtsHsvExtraction[k].nRange_H = 10;

					// S
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "HsvExtraction" + strId + "_MinS", tmp))
						sParam.vtsHsvExtraction[k].nMin_S = sConfigfile.parseInt(tmp);
					else
						sParam.vtsHsvExtraction[k].nMin_S = 30;

					// S
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "HsvExtraction" + strId + "_MaxS", tmp))
						sParam.vtsHsvExtraction[k].nMax_S = sConfigfile.parseInt(tmp);
					else
						sParam.vtsHsvExtraction[k].nMax_S = 250;

					// V
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "HsvExtraction" + strId + "_MinV", tmp))
						sParam.vtsHsvExtraction[k].nMin_V = sConfigfile.parseInt(tmp);
					else
						sParam.vtsHsvExtraction[k].nMin_V = 30;

					// V
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "HsvExtraction" + strId + "_MaxV", tmp))
						sParam.vtsHsvExtraction[k].nMax_V = sConfigfile.parseInt(tmp);
					else
						sParam.vtsHsvExtraction[k].nMax_V = 250;
				}
			}



			// Count_GrayExtraction
			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "Count_GrayExtraction", tmp))
				nCount = sConfigfile.parseInt(tmp);

			if (nCount > 0) {
				sParam.nCount_GrayExtraction = nCount;
				sParam.vtsGrayExtraction.resize(sParam.nCount_GrayExtraction);
				for (int k = 0; k < sParam.nCount_GrayExtraction; ++k) {
					string strId = to_string(k + 1);
					// Gray
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "GrayExtraction" + strId + "_Gary", tmp))
						sParam.vtsGrayExtraction[k].nValue_Gray = sConfigfile.parseInt(tmp);
					else
						sParam.vtsGrayExtraction[k].nValue_Gray = 0;

					// Range
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "GrayExtraction" + strId + "_Rang", tmp))
						sParam.vtsGrayExtraction[k].nRange = sConfigfile.parseInt(tmp);
					else
						sParam.vtsGrayExtraction[k].nRange = 1;
				}
			}


			// DeleteMode
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode", tmp))
				sParam.sDeleteMode.nMode = sConfigfile.parseInt(tmp);
			else
				sParam.sDeleteMode.nMode = 0;

			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode_MaxCount", tmp))
				sParam.sDeleteMode.nMaxCount = sConfigfile.parseInt(tmp);
			else
				sParam.sDeleteMode.nMaxCount = 1;

			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode_ROI_Width_Std", tmp))
				sParam.sDeleteMode.nROI_Width_Std = sConfigfile.parseInt(tmp);
			else
				sParam.sDeleteMode.nROI_Width_Std = 0;

			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode_ROI_Width_Range", tmp))
				sParam.sDeleteMode.nROI_Width_Range = sConfigfile.parseInt(tmp);
			else
				sParam.sDeleteMode.nROI_Width_Range = 0;

			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode_ROI_Height_Std", tmp))
				sParam.sDeleteMode.nROI_Height_Std = sConfigfile.parseInt(tmp);
			else
				sParam.sDeleteMode.nROI_Height_Std = 0;

			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode_ROI_Height_Range", tmp))
				sParam.sDeleteMode.nROI_Height_Range = sConfigfile.parseInt(tmp);
			else
				sParam.sDeleteMode.nROI_Height_Range = 0;

			nCount = 0;
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteMode_RoiCount", tmp))
				nCount = sConfigfile.parseInt(tmp);

			// SDeleteROI
			if (nCount > 0) {
				sParam.sDeleteMode.sROI.nCount = nCount;
				sParam.sDeleteMode.sROI.vtrectROI.resize(sParam.sDeleteMode.sROI.nCount);
				for (int k = 0; k < sParam.sDeleteMode.sROI.nCount; ++k) {
					string strId = to_string(k + 1);
					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteROI" + strId + "_L", tmp))
						sParam.sDeleteMode.sROI.vtrectROI[k].left = sConfigfile.parseInt(tmp);
					else
						sParam.sDeleteMode.sROI.vtrectROI[k].left = 0;

					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteROI" + strId + "_T", tmp))
						sParam.sDeleteMode.sROI.vtrectROI[k].top = sConfigfile.parseInt(tmp);
					else
						sParam.sDeleteMode.sROI.vtrectROI[k].top = 0;

					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteROI" + strId + "_R", tmp))
						sParam.sDeleteMode.sROI.vtrectROI[k].right = sConfigfile.parseInt(tmp);
					else
						sParam.sDeleteMode.sROI.vtrectROI[k].right = 0;

					if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DeleteROI" + strId + "_B", tmp))
						sParam.sDeleteMode.sROI.vtrectROI[k].bottom = sConfigfile.parseInt(tmp);
					else
						sParam.sDeleteMode.sROI.vtrectROI[k].bottom = 0;
				}
			}

			// OpenX
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "OpenX", tmp))
				sParam.nOpenX = sConfigfile.parseInt(tmp);
			else
				sParam.nOpenX = 1;

			// OpenY
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "OpenY", tmp))
				sParam.nOpenY = sConfigfile.parseInt(tmp);
			else
				sParam.nOpenY = 1;

			// ConnectX
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ConnectX", tmp))
				sParam.nConnectX = sConfigfile.parseInt(tmp);
			else
				sParam.nConnectX = 1;

			// ConnectY
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ConnectY", tmp))
				sParam.nConnectY = sConfigfile.parseInt(tmp);
			else
				sParam.nConnectY = 1;

			// DilateX
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DilateX", tmp))
				sParam.nDilateX = sConfigfile.parseInt(tmp);
			else
				sParam.nDilateX = 1;

			// DilateY
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "DilateY", tmp))
				sParam.nDilateY = sConfigfile.parseInt(tmp);
			else
				sParam.nDilateY = 1;

			// ErosionX
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ErosionX", tmp))
				sParam.nErosionX = sConfigfile.parseInt(tmp);
			else
				sParam.nErosionX = 1;

			// ErosionY
			if (sConfigfile.FindConfigValue(vtsConfig, strAppName, "ErosionY", tmp))
				sParam.nErosionY = sConfigfile.parseInt(tmp);
			else
				sParam.nErosionY = 1;

			return true;
		}

#pragma endregion
	}
}