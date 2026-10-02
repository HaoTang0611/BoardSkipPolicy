#ifndef _CalibrationDef_H_
#define _CalibrationDef_H_
//-------------------------------------------------------------------------------------//
enum CALIBRATION_MODE
{
	CALIBRATION_STOP = 0,

	CALIBRATION_FOV_WIDTH_START,	
	CALIBRATION_FOV_WIDTH_END,

	CALIBRATION_FOV_HEIGHT_START,	
	CALIBRATION_FOV_HEIGHT_END,

	CALIBRATION_CAMERA_ALIGN_HOR_START,
	CALIBRATION_CAMERA_ALIGN_HOR_END,

	CALIBRATION_CAMERA_ALIGN_VER_START,	
	CALIBRATION_CAMERA_ALIGN_VER_END,

	CALIBRATION_CAMERA_ALIGN_HOR_3D_START,
	CALIBRATION_CAMERA_ALIGN_HOR_3D_END,

	CALIBRATION_CAMERA_ALIGN_VER_3D_START,	
	CALIBRATION_CAMERA_ALIGN_VER_3D_END,

	CALIBRATION_IMAGE_RESOLUTION,
	
	CALIBRATION_IMAGE_FOCUS_AUTO_100,
	CALIBRATION_IMAGE_FOCUS_AUTO_10,
	CALIBRATION_IMAGE_FOCUS_AUTO,	

	CALIBRATION_2D_LIGHT_ALIGN,
	CALIBRATION_3D_CAST_ALIGN,
	CALIBRATION_3D_CAST_FOCUS,

	CALIBRATION_2D_LIGHT_CURRENT,	
	CALIBRATION_3D_CAST_CURRENT,
	CALIBRATION_3D_CAST_CURRENT_RED,
	CALIBRATION_3D_CAST_CURRENT_GRN,
	CALIBRATION_3D_CAST_CURRENT_BLU,

	CALIBRATION_PATTERN_ZERO_PLANE,//相平面

	CALIBRATION_PATTERN_HEIGHT_FACTOR_DOT,//階高塊規
	CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV,//動Z軸
	CALIBRATION_PATTERN_HEIGHT_FACTOR_FOV_Z,//動Z軸

	CALIBRATION_PATTERN_HEIGHT_FACTOR_MULTI_FOV,//動Z軸	
	
	CALIBRATION_PATTERN_PHASE_MEASURE, 

	CALIBRATION_3D_MODEL_TEST,		
	CALIBRATION_VERIFY_ZERO_PLANE,//相平面驗證

	CALIBRATION_EXCEPTION = 99999999
};
//-------------------------------------------------------------------------------------//
//相位高度校正
/*
typedef struct tagPhaseFactorGrid
{
	RECT       m_RectLevel;
	RECT       m_RectTarget;	
	double     m_PosX, m_PosY, m_PosZ;
	double     m_ImgX, m_ImgY, m_ImgZ;
	double     m_Factor;
	tagPhaseFactorGrid()
	{
		m_RectLevel.left = m_RectLevel.right = m_RectLevel.top = m_RectLevel.bottom = 0;
		m_RectTarget.left = m_RectTarget.right = m_RectTarget.top = m_RectTarget.bottom = 0;
		m_PosX = m_PosY = m_PosZ = 0;
		m_ImgX = m_ImgY = m_ImgZ = 0;
		m_Factor = 1.00;
	}
} TPhaseFactorGrid, *PPhaseFactorGrid;
*/
//-------------------------------------------------------------------------------------//
#endif//CalibrationDef