#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#ifndef SC_EXENAME
#define SC_EXENAME "JETSmartChart.exe"
#endif // !SC_WINDOWNAME

#ifndef SC_WINDOWNAME
#define SC_WINDOWNAME "JETSmartChart"
#endif // !SC_WINDOWNAME

#ifndef SC_DRAGLINE_MAXIMAM
#define SC_DRAGLINE_MAXIMAM 10
#endif

#ifndef SC_STRINGLENGTH_MAXIMUM
#define SC_STRINGLENGTH_MAXIMUM 64
#endif

#ifndef SC_DATALENGTH_MAXIMUM
#define SC_DATALENGTH_MAXIMUM 10000
#endif

#ifndef SC_CHAR
#define SC_CHAR wchar_t
#endif

#ifndef SC_T
#define SC_T(str) L##str
#endif

#ifndef SC_STRCPY
#define SC_STRCPY wcscpy_s
#endif

#ifndef DEFAULT_FONTNAME
#define DEFAULT_FONTNAME SC_T("Microsoft Sans Serif")
#endif // _DEFAULT_FONTNAME

enum class ELabelProviderType
{
	ValueX,
	ValueY,
	ValueXY,
	Text,

	_End,
};

enum class EDragLineType
{
	Normal,
	Pair_Same,
	Pair_Inv,

	_End,
};

/* smart chart supporting data */
enum class ESendData_Agent {
	Unknown					= 0,
	Chart,
	Title,
	Legend,
	PrimaryAxisY,
	PrimaryAxisX,
	SecondaryAxisY,
	SecondaryAxisX,
	DragLine,

	Serie_Point				= 100,
	Serie_Line,
	Serie_Bar,

	UI_ShowSerieLabel		= 200,
};
struct SGFunc {
protected:
	void SetText(SC_CHAR* dst, const SC_CHAR* src) {
		SC_STRCPY(dst, SC_STRINGLENGTH_MAXIMUM, src);
	}
};
struct SSendData_Base : public SGFunc {
	ESendData_Agent		_Type = ESendData_Agent::Unknown;
};
struct SSendData_DragLine : public SSendData_Base {
private:
	EDragLineType	m_DragLineType = EDragLineType::Normal;
public:
	bool			m_bEnableDrag = true;
	bool			m_bDragDirection_Ver = false;		//Hor 只的是左右拉  | <---> |
	bool			m_bUseSecondary = false;
	SC_CHAR			m_sName[SC_STRINGLENGTH_MAXIMUM];
	COLORREF		color = RGB(0, 0, 0);
	double			m_dPosition = 0;
	unsigned int	m_iLineWidth = 3;
	int				m_iNDotNum = 0;

	SC_CHAR			m_sName_paired[SC_STRINGLENGTH_MAXIMUM];
	COLORREF		color_paired = RGB(0, 0, 0);
	double			m_dPosition_paired = 0;

	explicit SSendData_DragLine(const SC_CHAR* name, bool dragDirection_Ver = false, bool useSecondary = false) {
		_Type = ESendData_Agent::DragLine;
		SetText(m_sName, name);
		m_bDragDirection_Ver = dragDirection_Ver;
		m_bUseSecondary = useSecondary;
	}

	EDragLineType	GetDragLineType() { return m_DragLineType; }
	void DisablePaired() {
		this->m_DragLineType = EDragLineType::Normal;
	}
	void PairLine_Same(const SC_CHAR* name_paired, COLORREF c, double pos) {
		this->m_DragLineType = EDragLineType::Pair_Same;
		SetText(m_sName_paired, name_paired);
		this->color_paired = c;
		this->m_dPosition_paired = pos;
	}
	void PairLine_Inv(const SC_CHAR* name_paired, COLORREF c, double pos) {
		this->m_DragLineType = EDragLineType::Pair_Inv;
		SetText(m_sName_paired, name_paired);
		this->color_paired = c;
		this->m_dPosition_paired = pos;
	}
	void SetName(const SC_CHAR* name) {
		this->SetText(this->m_sName, name);
	}
};

struct SSerieData {
	typedef char TC;
private:
	double			bin_Size = 1.0;
	double			dMax = DBL_MIN;
	double			dMin = DBL_MAX;
public:
	double			data_PtrX[SC_DATALENGTH_MAXIMUM];
	double			data_PtrY[SC_DATALENGTH_MAXIMUM];
	unsigned int	data_Count = 0;

	double GetBinSize() { return this->bin_Size; }

	void SetData(SSerieData& data) {
		memcpy(data_PtrX, data.data_PtrX, sizeof(double) * data_Count);
		memcpy(data_PtrY, data.data_PtrY, sizeof(double) * data_Count);
		data_Count = data.data_Count;
	}
	double getMax() { return dMax; }
	double getMin() { return dMin; }

	/* 設置繪製Serie的數據，將原始數據 pdataX & pdataY 複製並儲存在結構子 (data_PtrX & data_PtrY )中
	pdataX : data of AxisX(Bottom / Top)
	pdataY : data of AxisY(Left / Right)
	count  : 上限值為SC_DATALENGTH_MAXIMUM
	數據範圍會自動計算(min<=>max)
	*/
	template<typename T> void SetData(T* pdataX, T* pdataY, size_t count) {
		data_Count = count;
		int i = 0;
		data_PtrX[i] = pdataX[i];
		data_PtrY[i] = pdataY[i];
		double min = pdataX[i];
		double max = pdataX[i];
		for (i = 1; i < count; i++) {
			data_PtrX[i] = pdataX[i];
			data_PtrY[i] = pdataY[i];
			if (min > pdataX[i]) { min = pdataX[i]; }
			if (max < pdataX[i]) { max = pdataX[i]; }
		}
		dMin = min;
		dMax = max;
	}
	/* Automatic Statistics (不保存原始數據pdata，只儲存分佈數據)
	pdata   : 要統計的數據
	count   : 總數據量
	binsize : 一組資料的大小，如果countBin超出limit(SC_DATALENGTH_MAXIMUM)則會自動計算符合的binsize
	數據範圍會自動計算(min<=>max)
	*/	
	template<typename T> void SetData(T* pdata, size_t count, double binsize = 1.0) {		
		double min = dMin;
		double max = dMax;
		for (int i = 0; i < count; i++) {
			if (min > pdata[i]) { min = pdata[i]; }
			if (max < pdata[i]) { max = pdata[i]; }
		}
		auto countBin = (long long)(max - min + 1.5);
		double rangeBin = 1.0;
		if (binsize > 0 && binsize != 1.0) {
			countBin = countBin / binsize;
			rangeBin = binsize;
		}
		if (countBin > SC_DATALENGTH_MAXIMUM) {
			rangeBin = ceil(countBin * 10.0 / SC_DATALENGTH_MAXIMUM) * 0.1;
			countBin = (long)(countBin / rangeBin + 1.5);
		}
		std::memset(data_PtrX, 0, sizeof(double) * countBin);
		std::memset(data_PtrY, 0, sizeof(double) * countBin);
		for (int i = 0; i < count; i++) {
			size_t idBin = (pdata[i] - min) / rangeBin;
			data_PtrY[idBin]++;
		}
		for (int i = 0; i < countBin; i++) {
			data_PtrX[i] = i * rangeBin + min;
		}
		dMin = min;
		dMax = max;
		bin_Size = rangeBin;
		data_Count = countBin;
	}
	/* Automatic Statistics (不保存原始數據pdata，只儲存分佈數據)
	pdata     : 要統計的數據
	count     : 總數據量
	min & max : 於min <=> max 之間進行統計(自動算出binsize)
	!!! 超出範圍數據將直接忽略 !!!
	*/
	template<typename T> void SetData(T* pdata, size_t count, double min, double max) {
		auto countBin = (long long)(max - min + 1.5);
		double rangeBin = 1.0;
		if (countBin > SC_DATALENGTH_MAXIMUM) {
			rangeBin = ceil(countBin * 10.0 / SC_DATALENGTH_MAXIMUM) * 0.1;
			countBin = (long)(countBin / rangeBin + 1.5);
		}
		std::memset(data_PtrX, 0, sizeof(double) * countBin);
		std::memset(data_PtrY, 0, sizeof(double) * countBin);
		for (int i = 0; i < count; i++) {
			size_t idBin = (pdata[i] - min) / rangeBin;
			data_PtrY[idBin]++;
		}
		for (int i = 0; i < countBin; i++) {
			data_PtrX[i] = i * rangeBin + min;
		}
		dMin = min;
		dMax = max;
		bin_Size = rangeBin;
		data_Count = countBin;
	}
	/* 累加資料(統計值方圖)
	pdata   : 要累加統計的數據
	count   : 總數據量
	會依循此結構的binsize進行統計，此外如果數據超出原有範圍(min<=>max)時會自動更新範圍
	*/
	template <typename T> void Statistic(T* pdata, size_t count) {
		double min = dMin;
		double max = dMax;
		for (int i = 0; i < count; i++) {
			if (min > pdata[i]) { min = pdata[i]; }
			if (max < pdata[i]) { max = pdata[i]; }
		}
		double rangeBin = bin_Size;
		long long countBin = (long long)(max - min + 1.5) / rangeBin;
		if (countBin > SC_DATALENGTH_MAXIMUM) {
			rangeBin = ceil(countBin * 10.0 / SC_DATALENGTH_MAXIMUM) * 0.1;
			countBin = (long)(countBin / rangeBin + 1.5);
		}
		if (data_Count != countBin) {
			std::vector<double> tmpx(data_Count);
			std::vector<double> tmpy(data_Count);
			memcpy(tmpx.data(), data_PtrX, sizeof(double) * data_Count);
			memcpy(tmpy.data(), data_PtrY, sizeof(double) * data_Count);
			memset(data_PtrX, 0, sizeof(double) * countBin);
			memset(data_PtrY, 0, sizeof(double) * countBin);
			for (int i = 0; i < count; i++) {
				size_t idBin = (pdata[i] - min) / rangeBin;	//x
				if (idBin < countBin && idBin >= 0) {
					data_PtrY[idBin]++;
				}
			}
			for (int i = 0; i < tmpx.size(); i++) {
				size_t idBin = (tmpx[i] - min) / rangeBin;
				data_PtrY[idBin] += tmpy[i];
			}
			for (int i = 0; i < countBin; i++) {
				data_PtrX[i] = i * rangeBin + min;
			}
		}
		else {
			for (int i = 0; i < count; i++) {
				size_t idBin = (pdata[i] - min) / rangeBin;	//x
				data_PtrY[idBin]++;
			}
		}
		dMin = min;
		dMax = max;
		bin_Size = rangeBin;
		data_Count = countBin;
	}

//	bool SaveFile(const SC_CHAR* filename) {
//		if (data_Count <= 0) { return false; }
//
//#pragma region wchar to char (out: vc)
//		int len = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
//		if (len == ERROR_NO_UNICODE_TRANSLATION) { return false; }
//		std::vector<char> vc(len);
//		int res = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, vc.data(), len, NULL, NULL);
//#pragma endregion
//
//#pragma region check extension(out: fname)
//		std::string extension = ".jsc";
//		std::string fname = vc.data();
//		bool exist = false;
//		int di = fname.length() - 1;
//		for (int i = di; i > 0; i--) {
//			if (fname[i] == '.') {
//				di = i;
//				exist = true;
//				break;
//			}
//		}
//		if (!exist) {
//			if (extension[0] != '.') {
//				fname.append(".");
//			}
//			fname.append(extension);
//		}
//#pragma endregion
//		
//		std::fstream fp(fname, std::ios::out | std::ios::binary);
//		this->SaveFile(&fp);
//		fp.close();
//		return true;
//	}
//	bool LoadFile(const SC_CHAR* filename) {
//
//#pragma region wchar to char (out: vc)
//		int len = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
//		if (len == ERROR_NO_UNICODE_TRANSLATION) { return false; }
//		std::vector<char> vc(len);
//		int res = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, vc.data(), len, NULL, NULL);
//#pragma endregion
//
//#pragma region check extension(out: fname)
//		std::string extension = ".jsc";
//		std::string fname = vc.data();
//		bool exist = false;
//		int di = fname.length() - 1;
//		for (int i = di; i > 0; i--) {
//			if (fname[i] == '.') {
//				di = i;
//				exist = true;
//				break;
//			}
//		}
//		if (!exist) {
//			if (extension[0] != '.') {
//				fname.append(".");
//			}
//			fname.append(extension);
//		}
//#pragma endregion
//
//		std::fstream fp(fname, std::ios::in | std::ios::binary);
//		this->LoadFile(&fp);
//		fp.close();
//		return true;
//	}
//	/* 將目前Serie數據儲存下來
//	*/
//	template <typename T> bool UpdateFile(const SC_CHAR* filename, T* pdata, size_t count) {
//		bool res = LoadFile(filename);
//		if (res) { Statistic<T>(pdata, count); }
//		else { SetData<T>(pdata, count); }
//		return SaveFile(filename);
//	}

protected:
	bool SaveFile(std::fstream* fp) {
		if (fp != nullptr && !fp->is_open()) {
			return false;
		}
		fp->write((TC*)&dMin, sizeof(dMin));
		fp->write((TC*)&dMax, sizeof(dMax));
		fp->write((TC*)&bin_Size, sizeof(bin_Size));
		fp->write((TC*)&data_Count, sizeof(data_Count));
		fp->write((TC*)data_PtrX, sizeof(double) * data_Count);
		fp->write((TC*)data_PtrY, sizeof(double) * data_Count);
		return true;
	}
	bool LoadFile(std::fstream* fp) {
		if (fp != nullptr && !fp->is_open()) {
			return false;
		}
		fp->read((TC*)&dMin, sizeof(dMin));
		fp->read((TC*)&dMax, sizeof(dMax));
		fp->read((TC*)&bin_Size, sizeof(bin_Size));
		fp->read((TC*)&data_Count, sizeof(data_Count));
		fp->read((TC*)data_PtrX, sizeof(double) * data_Count);
		fp->read((TC*)data_PtrY, sizeof(double) * data_Count);
		return true;
	}
};
struct SSendData_Serie_Base : public SSerieData, public SSendData_Base {
	ELabelProviderType	m_ShowType = ELabelProviderType::ValueY;
	bool			m_bUseSecondaryX = false;
	bool			m_bUseSecondaryY = false;
	bool			m_bIsVisible = true;
	bool			m_bShadow = false;
	int				m_iShadowDepth = 2;
	int				m_iNDotNum = 0;
	SC_CHAR			m_sSerieName[SC_STRINGLENGTH_MAXIMUM];
	COLORREF		color_SerieColor = RGB(0, 0, 0);
	COLORREF		color_ShadowColor = RGB(150, 150, 150);

	void SetName(const SC_CHAR* name) {
		SC_STRCPY(m_sSerieName, name);
	}

	bool SaveFile(const SC_CHAR* filename) {
		if (data_Count <= 0) { return false; }

#pragma region wchar to char (out: vc)
		int len = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		if (len == ERROR_NO_UNICODE_TRANSLATION) { return false; }
		std::vector<char> vc(len);
		int res = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, vc.data(), len, NULL, NULL);
#pragma endregion

#pragma region check extension(out: fname)
		std::string extension = ".jsc";
		std::string fname = vc.data();
		bool exist = false;
		size_t di = fname.length() - 1;
		for (size_t i = di; i > 0; i--) {
			if (fname[i] == '.') {
				di = i;
				exist = true;
				break;
			}
		}
		if (!exist) {
			if (extension[0] != '.') {
				fname.append(".");
			}
			fname.append(extension);
		}
#pragma endregion

		std::fstream fp(fname, std::ios::out | std::ios::binary);
		this->SaveFile(&fp);
		SSerieData::SaveFile(&fp);
		fp.close();
		return true;
	}
	bool LoadFile(const SC_CHAR* filename) {

#pragma region wchar to char (out: vc)
		int len = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		if (len == ERROR_NO_UNICODE_TRANSLATION) { return false; }
		std::vector<char> vc(len);
		int res = ::WideCharToMultiByte(CP_ACP, 0, filename, -1, vc.data(), len, NULL, NULL);
#pragma endregion

#pragma region check extension(out: fname)
		std::string extension = ".jsc";
		std::string fname = vc.data();
		bool exist = false;
		size_t di = fname.length() - 1;
		for (size_t i = di; i > 0; i--) {
			if (fname[i] == '.') {
				di = i;
				exist = true;
				break;
			}
		}
		if (!exist) {
			if (extension[0] != '.') {
				fname.append(".");
			}
			fname.append(extension);
		}
#pragma endregion

		std::fstream fp(fname, std::ios::in | std::ios::binary);
		this->LoadFile(&fp);
		SSerieData::LoadFile(&fp);
		fp.close();
		return true;
	}
	/* 將目前Serie數據儲存下來	*/
	template <typename T> bool UpdateFile(const SC_CHAR* filename, T* pdata, size_t count) {
		bool res = this->LoadFile(filename);
		if (res) { Statistic<T>(pdata, count); }
		else { SetData<T>(pdata, count); }
		return this->SaveFile(filename);
	}	

protected:
	/* Point */
	int				m_iPointType = 0;	//0:Ellipse, 1:Rectangle, 2:Triangle
	int				m_iPointSize = 5;

	/* Line */
	int				m_iLineWidth = 1;
	int				m_iPenStyle = PS_SOLID;
	bool			m_bSmooth = false;

	/* Bar */
	bool			m_bHorizontal = false;
	bool			m_bAutoBaseLine = false;
	bool			m_bGradient = false;
	bool			m_bStacked = false;
	unsigned int	m_iGroupId = 0;
	unsigned int	m_iGradientType = 2;	//0: Hor,  1:Ver,  2: HorDouble, 3: VerDouble
	int				m_iBarWidth = 5;
	int				m_iBorderWidth = 1;
	double			m_dBaseLine = 0;
	COLORREF		color_GradientColor = RGB(255, 255, 255);
	COLORREF		color_BorderColor = RGB(0, 0, 0);

	bool SaveFile(std::fstream* fp) {
		if (fp != nullptr && !fp->is_open()) {
			return false;
		}
		fp->write((TC*)&_Type, sizeof(_Type));
		fp->write((TC*)&m_ShowType, sizeof(m_ShowType));
		fp->write((TC*)&m_bUseSecondaryX, sizeof(m_bUseSecondaryX));
		fp->write((TC*)&m_bUseSecondaryY, sizeof(m_bUseSecondaryY));
		fp->write((TC*)&m_bIsVisible, sizeof(m_bIsVisible));
		fp->write((TC*)&m_bShadow, sizeof(m_bShadow));
		fp->write((TC*)&m_iShadowDepth, sizeof(m_iShadowDepth));
		fp->write((TC*)&m_iNDotNum, sizeof(m_iNDotNum));
		fp->write((TC*)m_sSerieName, sizeof(SC_CHAR) * SC_STRINGLENGTH_MAXIMUM);
		fp->write((TC*)&color_SerieColor, sizeof(color_SerieColor));
		fp->write((TC*)&color_ShadowColor, sizeof(color_ShadowColor));

		/* Point */
		fp->write((TC*)&m_iPointType, sizeof(m_iPointType));
		fp->write((TC*)&m_iPointSize, sizeof(m_iPointSize));

		/* Line */
		fp->write((TC*)&m_iLineWidth, sizeof(m_iLineWidth));
		fp->write((TC*)&m_iPenStyle, sizeof(m_iPenStyle));
		fp->write((TC*)&m_bSmooth, sizeof(m_bSmooth));

		/* Bar */
		fp->write((TC*)&m_bHorizontal, sizeof(m_bHorizontal));
		fp->write((TC*)&m_bAutoBaseLine, sizeof(m_bAutoBaseLine));
		fp->write((TC*)&m_bGradient, sizeof(m_bGradient));
		fp->write((TC*)&m_bStacked, sizeof(m_bStacked));
		fp->write((TC*)&m_iGroupId, sizeof(m_iGroupId));
		fp->write((TC*)&m_iGradientType, sizeof(m_iGradientType));
		fp->write((TC*)&m_iBarWidth, sizeof(m_iBarWidth));
		fp->write((TC*)&m_iBorderWidth, sizeof(m_iBorderWidth));
		fp->write((TC*)&m_dBaseLine, sizeof(m_dBaseLine));
		fp->write((TC*)&color_GradientColor, sizeof(color_GradientColor));
		fp->write((TC*)&color_BorderColor, sizeof(color_BorderColor));
		return true;
	}
	bool LoadFile(std::fstream* fp) {
		if (fp != nullptr && !fp->is_open()) {
			return false;
		}
		fp->read((TC*)&_Type, sizeof(_Type));
		fp->read((TC*)&m_ShowType, sizeof(m_ShowType));
		fp->read((TC*)&m_bUseSecondaryX, sizeof(m_bUseSecondaryX));
		fp->read((TC*)&m_bUseSecondaryY, sizeof(m_bUseSecondaryY));
		fp->read((TC*)&m_bIsVisible, sizeof(m_bIsVisible));
		fp->read((TC*)&m_bShadow, sizeof(m_bShadow));
		fp->read((TC*)&m_iShadowDepth, sizeof(m_iShadowDepth));
		fp->read((TC*)&m_iNDotNum, sizeof(m_iNDotNum));
		fp->read((TC*)m_sSerieName, sizeof(SC_CHAR) * SC_STRINGLENGTH_MAXIMUM);
		fp->read((TC*)&color_SerieColor, sizeof(color_SerieColor));
		fp->read((TC*)&color_ShadowColor, sizeof(color_ShadowColor));

		/* Point */
		fp->read((TC*)&m_iPointType, sizeof(m_iPointType));
		fp->read((TC*)&m_iPointSize, sizeof(m_iPointSize));

		/* Line */
		fp->read((TC*)&m_iLineWidth, sizeof(m_iLineWidth));
		fp->read((TC*)&m_iPenStyle, sizeof(m_iPenStyle));
		fp->read((TC*)&m_bSmooth, sizeof(m_bSmooth));

		/* Bar */
		fp->read((TC*)&m_bHorizontal, sizeof(m_bHorizontal));
		fp->read((TC*)&m_bAutoBaseLine, sizeof(m_bAutoBaseLine));
		fp->read((TC*)&m_bGradient, sizeof(m_bGradient));
		fp->read((TC*)&m_bStacked, sizeof(m_bStacked));
		fp->read((TC*)&m_iGroupId, sizeof(m_iGroupId));
		fp->read((TC*)&m_iGradientType, sizeof(m_iGradientType));
		fp->read((TC*)&m_iBarWidth, sizeof(m_iBarWidth));
		fp->read((TC*)&m_iBorderWidth, sizeof(m_iBorderWidth));
		fp->read((TC*)&m_dBaseLine, sizeof(m_dBaseLine));
		fp->read((TC*)&color_GradientColor, sizeof(color_GradientColor));
		fp->read((TC*)&color_BorderColor, sizeof(color_BorderColor));
		return true;
	}

	/* Not include serieData */
	void SetParam(SSendData_Serie_Base* p) {
		this->_Type = p->_Type;
		this->m_ShowType = p->m_ShowType;

		this->m_bUseSecondaryX = p->m_bUseSecondaryX;
		this->m_bUseSecondaryY = p->m_bUseSecondaryY;
		this->m_bIsVisible = p->m_bIsVisible;
		this->m_bShadow = p->m_bShadow;
		this->m_iShadowDepth = p->m_iShadowDepth;
		this->m_iNDotNum = p->m_iNDotNum;
		this->SetName(p->m_sSerieName);
		this->color_SerieColor = p->color_SerieColor;
		this->color_ShadowColor = p->color_ShadowColor;

		/* Point */
		this->m_iPointType = p->m_iPointType;
		this->m_iPointSize = p->m_iPointSize;

		/* Line */
		this->m_iLineWidth = p->m_iLineWidth;
		this->m_iPenStyle = p->m_iPenStyle;
		this->m_bSmooth = p->m_bSmooth;

		/* Bar */
		this->m_bHorizontal = p->m_bHorizontal;
		this->m_bAutoBaseLine = p->m_bAutoBaseLine;
		this->m_bGradient = p->m_bGradient;
		this->m_bStacked = p->m_bStacked;
		this->m_iGroupId = p->m_iGroupId;
		this->m_iGradientType = p->m_iGradientType;	//0: Hor,  1:Ver,  2: HorDouble, 3: VerDouble
		this->m_iBarWidth = p->m_iBarWidth;
		this->m_iBorderWidth = p->m_iBorderWidth;
		this->m_dBaseLine = p->m_dBaseLine;
		this->color_GradientColor = p->color_GradientColor;
		this->color_BorderColor = p->color_BorderColor;
	}
};
struct SSendData_Serie_Point : public SSendData_Serie_Base {
	//0:Ellipse, 1:Rectangle, 2:Triangle
	int* getPointType() { return &m_iPointType; }
	int* getPointSize() { return &m_iPointSize; }

	explicit SSendData_Serie_Point(const SC_CHAR* name, bool useSecondaryX = false, bool useSecondaryY = false) {
		this->_Type = ESendData_Agent::Serie_Point;
		SC_STRCPY(m_sSerieName, name);
		this->m_bUseSecondaryX = useSecondaryX;
		this->m_bUseSecondaryY = useSecondaryY;
	}	
};
struct SSendData_Serie_Line : public SSendData_Serie_Base {
	int* getLineWidth() { return &m_iLineWidth; }
	int* getPenStyle() { return &m_iPenStyle; }
	bool* getSmooth() { return &m_bSmooth; }

	explicit SSendData_Serie_Line(const SC_CHAR* name, bool useSecondaryX = false, bool useSecondaryY = false) {
		this->_Type = ESendData_Agent::Serie_Line;
		SC_STRCPY(m_sSerieName, name);
		this->m_bUseSecondaryX = useSecondaryX;
		this->m_bUseSecondaryY = useSecondaryY;
	}
};
struct SSendData_Serie_Bar : public SSendData_Serie_Base {
	bool* getHorizontal() { return &m_bHorizontal; }
	bool* getAutoBaseLine() { return &m_bAutoBaseLine; }
	bool* getGradient() { return &m_bGradient; }
	bool* getStacked() { return &m_bStacked; }
	unsigned int* getGroupId() { return &m_iGroupId; }
	unsigned int* getGradientType() { return &m_iGradientType; }
	int* getBarWidth() { return &m_iBarWidth; }
	int* getBorderWidth() { return &m_iBorderWidth; }
	double* getBaseLine() { return &m_dBaseLine; }
	COLORREF* getGradientColor() { return &color_GradientColor; }
	COLORREF* getBorderColor() { return &color_BorderColor; }

	explicit SSendData_Serie_Bar(const SC_CHAR* name, bool useSecondaryX = false, bool useSecondaryY = false) {
		this->_Type = ESendData_Agent::Serie_Bar;
		SC_STRCPY(m_sSerieName, name);
		this->m_bUseSecondaryX = useSecondaryX;
		this->m_bUseSecondaryY = useSecondaryY;
		m_iShadowDepth = 3;
	}
};
struct SSendData_Axis : public SSendData_Base {
	bool			m_bVisibled = false;
	bool			m_bAutomatic = true;
	bool			m_bScrollBarEnabled = false;
	bool			m_bScrollBarAutoHide = true;
	bool			m_bDisCreteEnabled = false;
	bool			m_bInverted = false;
	bool			m_bPanZoomEnabled = true;
	bool			m_bGrid = false;	//dash line
	bool			m_bMarginSizeAuto = true;
	int				m_iFontSize = 80;
	int				m_iLabel_FontSize = 100;	//point size
	int				m_iMarginSize = 0;
	double			m_dZoomLimit = 1;
	double			m_dMin = 0;
	double			m_dMax = 0;
	SC_CHAR			m_sLabel_Name[SC_STRINGLENGTH_MAXIMUM];
	COLORREF		color = RGB(0, 0, 0);
	COLORREF		color_Grid = RGB(128, 128, 128);
	COLORREF		color_Label_Font = RGB(0, 0, 0);
	SSendData_Axis(ESendData_Agent dType = ESendData_Agent::PrimaryAxisY, bool isVisible = false) {
		_Type = dType;
		this->m_bVisibled = isVisible;
		SC_STRCPY(m_sLabel_Name, SC_T(""));
	}
};
struct SSendData_Legend : public SSendData_Base {
	bool			m_bVisibled = false;
	bool			m_bDockEnabled = true;
	bool			m_bHorizontalMode = false;
	bool			m_bTransparent = true;
	bool			m_bShadowEnabled = false;
	int				m_iLabel_FontSize = 100;	//point size
	unsigned int	m_iDockSide = 0;	//0: Right,  1:Left,  2:Top,  3:Bottom
	SC_CHAR			m_sFontName[SC_STRINGLENGTH_MAXIMUM];	//ex: "Arial Black"
	COLORREF		color = RGB(255, 255, 255);
	COLORREF		color_Label_Font = RGB(0, 0, 0);
	COLORREF		color_Shadow = RGB(0, 0, 0);
	SSendData_Legend() {
		this->_Type = ESendData_Agent::Legend;
		SC_STRCPY(m_sFontName, SC_T("Times New Roman"));
	}
};
struct SSendData_Title : public SSendData_Base {
	bool			m_bIsVisible = true;
	int				m_iFontSize = 120;
	SC_CHAR			m_sTitleName[SC_STRINGLENGTH_MAXIMUM];
	SC_CHAR			m_sFontName[SC_STRINGLENGTH_MAXIMUM];	//ex: "Arial Black"
	COLORREF		color_TextColor = RGB(0, 0, 0);
	SSendData_Title() {
		this->_Type = ESendData_Agent::Title;
		SC_STRCPY(m_sTitleName, SC_T(""));
		SC_STRCPY(m_sFontName, SC_T("Arial"));
	}
};
struct SSendData_Chart : public SSendData_Base {
	/* Global */
	SC_CHAR			m_sFontName[SC_STRINGLENGTH_MAXIMUM];  //ex: "Arial Black"

	/* Chart */
	bool			m_EnableCrossLine = false;
	bool			m_bPanEnabled = false;
	bool			m_bZoomEnabled = false;
	//bool			m_bShowMouseCursor = true;
	bool			m_bBackGroundColorGradientEnalbed = false;
	unsigned int	m_iBackGroundColorGradientType = 1;	//0: Hor,  1:Ver,  2: HorDouble, 3: VerDouble
	COLORREF		color_background = RGB(240, 240, 240);
	COLORREF		color_background_gradient1 = RGB(240, 240, 0);
	COLORREF		color_background_gradient2 = RGB(0, 240, 240);
	COLORREF		color_border = RGB(0, 0, 0);
	COLORREF		color_zoomrect = RGB(255, 0, 0);
	COLORREF		color_crossline = RGB(255, 0, 0);
	COLORREF		color_dragline = RGB(255, 0, 0);
	COLORREF		color_dragingline = RGB(255, 255, 0);
	COLORREF		color_ballonlabel_arrow = RGB(255, 0, 0);
	COLORREF		color_ballonlabel_background = RGB(255, 255, 0);

	/* Title */
	SSendData_Title	m_stTitle;

	/* Legend */
	SSendData_Legend	m_stLegend;

	/* Axis */
	//0: PrimaryY, 1: PrimaryX, 2:SecondaryY, 3:SecondaryX
	SSendData_Axis		m_stAxis[4] = { SSendData_Axis(ESendData_Agent::PrimaryAxisY),
									SSendData_Axis(ESendData_Agent::PrimaryAxisX),
									SSendData_Axis(ESendData_Agent::SecondaryAxisY),
									SSendData_Axis(ESendData_Agent::SecondaryAxisX) };

	SSendData_Chart() {
		this->_Type = ESendData_Agent::Chart;
		SC_STRCPY(m_sFontName, DEFAULT_FONTNAME);
	}
};

struct SSendData_ShowSerieLabel : public SSendData_Base {
	ELabelProviderType	m_ShowType = ELabelProviderType::ValueY;
	SC_CHAR				m_sSerieName[SC_STRINGLENGTH_MAXIMUM];
	SC_CHAR				m_sText[SC_STRINGLENGTH_MAXIMUM];  //ex: "C35"
	unsigned int		m_iPointID;
	unsigned int		m_iNDotNum = 0;
	COLORREF			color_ArrowLine = RGB(255, 0, 0);
	COLORREF			color_Background = RGB(255, 255, 0);

	//litmit_length of text = SC_STRINGLENGTH_MAXIMUM
	explicit SSendData_ShowSerieLabel(const SC_CHAR* serieName, unsigned int pointID, const SC_CHAR* text = SC_T("Text")) {
		_Type = ESendData_Agent::UI_ShowSerieLabel;
		this->SetText(m_sSerieName, serieName);
		this->SetText(m_sText, text);
		this->m_iPointID = pointID;
	}
};

/* the LPARAM, For smartchart, to differentiate whether the wm_message is for sending or for replying */
enum class EWM_MsgType {
	Command = 0x01,
	Ask = 0x10,
};
/* the wm_message, which is for the user controlling the smartchart */
enum class EMsg_SmartChart_Command {
	/* For sending struct pointer data(based on ESendData_Agent) */
	RegisterMainHWnd = WM_USER + 1,
	RefreshCtrl,
	ResetChart,
	ClearCrossLine,
	ClearAllDragLines,
	ClearAllSeries,
	ClearAllSeriesLabel,
	UndoPanZoom,
};
/* the wm_message, which is for the user asking the smartchart to reply with data that the user wants */
enum class EMsg_SmartChart_Ask {
	PlotAreaRect = WM_USER + 1,
	DragLine,
};

/* For user, easily controlling smartchart based on wm_message */
class CSmartChart_Agent
{
public:

	/* static func */
	/* create file from resource. ex : resourceID = IDR_EXE_SMARTCHART...,  resourceType = L"EXE" */
	static bool CreateFileFromResource(int resourceID, const char* resourceType = "EXE", const char* lpFileName = SC_EXENAME, bool isOverwrited = false) {
		HRSRC		hRes = ::FindResourceA(NULL, MAKEINTRESOURCEA(resourceID), resourceType);
		HGLOBAL		hMem = ::LoadResource(NULL, hRes);
		DWORD		dwSize = ::SizeofResource(NULL, hRes);
		if (isExist(lpFileName)) {
			if (isOverwrited) {
				HANDLE		hFile = ::CreateFileA(lpFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
				if (hFile == INVALID_HANDLE_VALUE) {
					::CloseHandle(hFile);
					return false;
				}
				DWORD dwWrite = 0;
				::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
				::CloseHandle(hFile);
			}
		}
		else {
			HANDLE hFile = ::CreateFileA(lpFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
			if (hFile == INVALID_HANDLE_VALUE) {
				::CloseHandle(hFile);
				return false;
			}
			DWORD dwWrite = 0;
			::WriteFile(hFile, hMem, dwSize, &dwWrite, NULL);
			::CloseHandle(hFile);
		}
		return true;
	}
	//尋找已開啟的SmartChart視窗
	static HWND FindSCmartChart() {
		return FindWindowExA(NULL, NULL, NULL, SC_WINDOWNAME);
	}
	//開啟SmartChart新視窗
	static HWND CreateSmartChart(LPCSTR exeSChart = SC_EXENAME) {
		WinExec(exeSChart, SW_HIDE);
		HWND hWnd = NULL;
		int cnt = 100;
		do {
			Sleep(0);
			hWnd = FindSCmartChart();
			cnt--;
		} while (hWnd == NULL && cnt > 0);
		return hWnd;
	}
	//開啟SmartChart並內嵌至指定區域
	static HWND CreateEmbeddedSmartChart(HWND parentHWnd, CRect visibleRegion, LPCSTR exeSChart = SC_EXENAME) {
		WinExec(exeSChart, SW_HIDE);
		HWND hWnd = NULL;
		int cnt = 100;
		do {
			Sleep(0);
			hWnd = FindSCmartChart();
			cnt--;
		} while (hWnd == NULL && cnt > 0);
		if (hWnd != NULL) {
			EmbeddedChildWnd(hWnd, parentHWnd, visibleRegion);
		}
		return hWnd;
	}
	static void EmbeddedChildWnd(HWND childHWnd, HWND parentHWnd, CRect visibleRegion) {
		auto lStyle = ::GetWindowLong(childHWnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
		::SetWindowLong(childHWnd, GWL_STYLE, lStyle);
		CRect rect;
		::SetParent(childHWnd, parentHWnd);
		::MoveWindow(childHWnd, visibleRegion.left, visibleRegion.top, visibleRegion.Width(), visibleRegion.Height(), TRUE);
	}

	template<typename T> LRESULT SendData(T* data) {
		COPYDATASTRUCT pdata = PackData(data);
		LRESULT r = SendMessage(this->m_hWnd, WM_COPYDATA, (WPARAM)this->m_hWnd, (LPARAM)&pdata);
		//delete pdata;
		return r;
	}

	/* func */
	explicit CSmartChart_Agent(HWND hWnd_SChart, HWND hWnd_Owner) {
		this->m_hWnd = hWnd_SChart;
		this->RegisterMainHWnd(hWnd_Owner);
	}
	virtual ~CSmartChart_Agent() {

	}

	HWND GetHWnd() {
		return this->m_hWnd;
	}

	void EmbeddedChildWnd(HWND parentHWnd, CRect visibleRegion) {
		EmbeddedChildWnd(this->m_hWnd, parentHWnd, visibleRegion);
	}

	LRESULT RegisterMainHWnd(HWND hWnd) {
		return SendMsg(EMsg_SmartChart_Command::RegisterMainHWnd, reinterpret_cast<WPARAM>(hWnd));
	}
	LRESULT	ResetChart() {
		return SendMsg(EMsg_SmartChart_Command::ResetChart);
	}
	LRESULT ClearCrossLine() {
		return SendMsg(EMsg_SmartChart_Command::ClearCrossLine);
	}
	LRESULT ClearAllDragLines() {
		return SendMsg(EMsg_SmartChart_Command::ClearAllDragLines);
	}
	LRESULT ClearAllSeries() {
		return SendMsg(EMsg_SmartChart_Command::ClearAllSeries);
	}
	LRESULT ClearAllSeriesLabel() {
		return SendMsg(EMsg_SmartChart_Command::ClearAllSeriesLabel);
	}
	LRESULT UndoPanZoom() {
		return SendMsg(EMsg_SmartChart_Command::UndoPanZoom);
	}

	LRESULT DataCallBack(EMsg_SmartChart_Ask msg) {
		return PostMsg(msg);
	}
protected:
	template<typename T> static COPYDATASTRUCT PackData(T* data) {
		COPYDATASTRUCT pData;
		pData.dwData = (ULONG_PTR)data->_Type;
		pData.cbData = sizeof(T);
		pData.lpData = data;
		return pData;
	}
	LRESULT SendMsg(EMsg_SmartChart_Command msg, WPARAM wParam = NULL, EWM_MsgType type = EWM_MsgType::Command) {
		return SendMessage(this->m_hWnd, (UINT)msg, wParam, (LPARAM)type);
	}
	LRESULT PostMsg(EMsg_SmartChart_Ask msg, WPARAM wParam = NULL, EWM_MsgType type = EWM_MsgType::Ask) {
		return PostMessage(this->m_hWnd, (UINT)msg, wParam, (LPARAM)type);
	}

private:
	static bool isExist(const char* filename) {
		std::ifstream f(filename);
		bool isExisted = f.good();
		f.close();
		return isExisted;
	}

	HWND m_hWnd;
};

/* For user, to dynamic unpack data */
enum class EReplyData_CallBack {
	Unknown,
	PlotAreaRect,
	DragLine,
};

struct SReplyData_Base : public SGFunc {
	EReplyData_CallBack _Type = EReplyData_CallBack::Unknown;
};
/* Chart繪圖區(不含Title、Legend、Axis) */
struct SReplyData_PlotAreaRect : public SReplyData_Base {
	CRect m_PlotAreaRect;

	SReplyData_PlotAreaRect() {
		_Type = EReplyData_CallBack::PlotAreaRect;
	}
};
/* DragLine的Axis數值 */
struct SReplyData_DragLine : public SReplyData_Base {
	double m_vDragline[SC_DRAGLINE_MAXIMAM];
	SC_CHAR m_vNames[SC_DRAGLINE_MAXIMAM][SC_STRINGLENGTH_MAXIMUM];

	SReplyData_DragLine() {
		_Type = EReplyData_CallBack::DragLine;
		for (int i = 0; i < SC_DRAGLINE_MAXIMAM; i++) {
			m_vDragline[i] = NAN;
			SetText(m_vNames[i], L"");
		}
	}
	void SetName(int index, const SC_CHAR* name) {
		this->SetText(m_vNames[index], name);
	}
	double GetValue(const SC_CHAR* name) {
		int idx = -1;
		double res = NAN;
		for (int i = 0; i < SC_DRAGLINE_MAXIMAM; i++) {
			if (std::wcscmp(name, m_vNames[i]) == 0) {
				idx = i;
				res = m_vDragline[i];
				break;
			}
		}
		return res;
	}
};

/* for smartchart, to reply packing data */
class CSmartChart_DataCallBack {
public:
	void SetCallBackWnd(HWND hWnd) {
		this->m_hWnd = hWnd;
	}

	template<typename T> LRESULT SendData(T* data) {
		COPYDATASTRUCT pdata = PackData(data);
		LRESULT r = SendMessage(this->m_hWnd, WM_COPYDATA, reinterpret_cast<WPARAM>(this->m_hWnd), (LPARAM)&pdata);
		return r;
	}

protected:
	template<typename T> static COPYDATASTRUCT PackData(T* data) {
		COPYDATASTRUCT pData;
		pData.dwData = (ULONG_PTR)data->_Type;
		pData.cbData = sizeof(T);
		pData.lpData = data;
		return pData;
	}

private:
	HWND m_hWnd;
};