/*****************************************************************************/
//
// 	Camera 2D Unit Type Define
//
//	Copyright (c) 2018, JET Tech.  All rights reserved.
//
//  Version 1.0
//
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" WITHOUT WARRANTY OF ANY KIND. NO WARRANTIES, EITHER EXPRESS
//  OR IMPLIED, ARE MADE WITH RESPECT TO THE SOFTWARE, INCLUDING, BUT 
//  NOT LIMITED TO, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS 
//  FOR A PARTICULAR PURPOSE, TITLE OR NON-INFRINGEMENT, OR ANY OTHER 
//  WARRANTIES THAT MAY ARISE FROM USAGE OF TRADE OR COURSE OF DEALING. 
//  THE COPYRIGHT HOLDERS AND CONTRIBUTORS DO NOT WARRANT, GUARANTEE, OR 
//  MAKE ANY REPRESENTATIONS REGARDING THE USE OF OR THE RESULTS OF THE 
//  USE OF THE SOFTWARE IN TERMS OF CORRECTNESS, ACCURACY, RELIABILITY, 
//  OR OTHERWISE AND DO NOT WARRANT THAT THE OPERATION OF THE SOFTWARE 
//  WILL BE UNINTERRUPTED OR ERROR FREE.  THE ENTIRE RISK AS TO THE 
//  PERFORMANCE OF THE SOFTWARE IS WITH YOU. IN NO EVENT SHALL THE 
//  COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
//  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
//  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
//  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
//  THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF ADVISED OF THE 
//  POSSIBILITY OF SUCH DAMAGE.
//
/*****************************************************************************/

#ifndef _C2D_2D_UNIT_TYPE_DEF_FILE_H_
#define _C2D_2D_UNIT_TYPE_DEF_FILE_H_


/*****************************************************************************
Definitions and types 
*****************************************************************************/
#define C2D_MAX_2D_CAMERA_UNIT   5

enum Camera2DType
{
	C2T_COAXLINK_Q12A_180F,
	C2T_TOTAL,
};

enum CameraGrabMode
{	
	CGM_TRIGGER = 0,
	CGM_FREE_RUN,
};

enum CameraTriggerMode
{	
	CTM_EXTERNAL = 0,
	CTM_INTERNAL,
};

enum CameraBayerPattern
{
	CBP_MONO,
	CBP_COLOR,
	CBP_BGGR,
	CBP_RGGB,
};

class CCameraInfo
{
	void Swap(const CCameraInfo &other)
	{
		m_Width			= other.m_Width;
		m_Height		= other.m_Height;
		m_FPS			= other.m_FPS;
		m_PixelByte		= other.m_PixelByte;
		m_ExposureTime	= other.m_ExposureTime;
		m_GrabMode		= other.m_GrabMode;
		m_TriggerMode	= other.m_TriggerMode;
		m_PayLoadSize	= other.m_PayLoadSize;
		m_BayerPattern	= other.m_BayerPattern;
	}

public:
	CCameraInfo()
	{
		m_PixelByte = 1;
		m_ExposureTime = 3000;
		m_GrabMode = CGM_TRIGGER;
		m_TriggerMode = CTM_EXTERNAL;
		m_Width = m_Height = 0;
		m_PayLoadSize = 0;
		m_BayerPattern = CBP_MONO;
	}

	~CCameraInfo()
	{
	}

	CCameraInfo(const CCameraInfo &other)
	{
		Swap(other);
	}

	CCameraInfo &operator=(const CCameraInfo &rhs)
	{
		Swap(CCameraInfo(rhs) );
		return *this;
	}

	unsigned int		m_Width;
	unsigned int		m_Height;
	unsigned int		m_FPS;
	unsigned char		m_PixelByte;
	double				m_ExposureTime;
	CameraGrabMode		m_GrabMode;
	CameraTriggerMode	m_TriggerMode;
	size_t				m_PayLoadSize;
	CameraBayerPattern	m_BayerPattern;
};

const int g_knModuleNameLen = 256;

typedef int (*FrameCallback)(unsigned char *pFrame, unsigned int BuffSize, int W, int H, short PixelSize);

/*****************************************************************************
Handle Codes
*****************************************************************************/
typedef long C2D_HANDLE;

#define C2D_HANDLE_MASK				0x0FFFFFFF
#define C2D_HANDLE_MIX				0x10000000

/*****************************************************************************
Error Codes
*****************************************************************************/
typedef int C2D_ERROR;

enum MouErrorCode
{
	C2D_ERR_SUCCESS =	 0,
	C2D_ERR_FAILED,
	C2D_ERR_NOT_IMPLEMENTED,
	C2D_ERR_DEVICE_NOT_OPEN,
	C2D_ERR_INVALID_HANDLE,
	C2D_ERR_INITIALIZE_FAILED,
	C2D_ERR_COMMAND_INVALID,
	C2D_ERR_COMMAND_PARAMETER_INVALID,
};

#endif