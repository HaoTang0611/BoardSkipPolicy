// EditModelView.cpp : implementation file
//
//-------------------------------------------------------------------------------------//
#include "stdafx.h"
#include "jet8000.h"
#include "EditModelView.h"
//-------------------------------------------------------------------------------------//
#include "InputBoxWnd.h"
#include "ModelAddAllWnd.h"
#include "ModelPropertyWnd.h"
#include "DefectListWnd.h"
#include "InputListWnd.h"
#include "ModelUpdateToGroupWnd.h"
#include "SmartChartStaticWnd.h"
#include "JetAlg\\JETAlg.h"
#include "WndAlgPropertyDef.h"
#include "ComponentAgentListWnd.h"
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//-------------------------------------------------------------------------------------//
#define LAND_COUNT_MODE_1D      1
#define LAND_COUNT_MODE_2D      2
#define LAND_COUNT_MODE_ARRAY   3
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelView
//-------------------------------------------------------------------------------------//
IMPLEMENT_DYNCREATE(CEditModelView, CView)
//-------------------------------------------------------------------------------------//
CEditModelView::CEditModelView()
{	
	m_BkColor = 0x000000;
	m_ImageZoom = 1.0;
	m_ImageOffset.x = m_ImageOffset.y = 0;
	m_DrawAddRect = false;
	m_KeepImageOffset = false;
	m_MousePosMode = CURSOR_POS_NONE;	
	//m_ManiMode = MANIPULATE_MODEL_EDIT;	
	m_FrameResolution.x = 10;
	m_FrameResolution.y = 10;
	m_FOVPosStage.x = m_FOVPosStage.y = 0;

	PreInitUniFrameBuffer();	

	::memset(&m_ImageWndRect, 0x00, sizeof(m_ImageWndRect));
	::memset(&m_MousePosLast, 0x00, sizeof(m_MousePosLast));
	::memset(&m_MousePosFirst, 0x00, sizeof(m_MousePosFirst));
	::memset(&m_MousePosCurrent, 0x00, sizeof(m_MousePosCurrent));
	::memset(&m_MousePosImageWnd, 0x00, sizeof(m_MousePosImageWnd));	

	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;
	m_ShowImagePtr = NULL;

	m_ModelPtr = NULL;
	m_ProjectPtr = NULL;	
	m_FovRatio = 1.0;
	m_ChangeComponentSelected = false;	
	m_ShowPopupMenu = true;
	m_UpdateTestMapTickCount = 0;
}
//-------------------------------------------------------------------------------------//
CEditModelView::~CEditModelView()
{
}
//-------------------------------------------------------------------------------------//
BEGIN_MESSAGE_MAP(CEditModelView, CView)
	//{{AFX_MSG_MAP(CEditModelView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_CONTEXTMENU()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(MENU_MODEL_EDIT_SWITCH_ONLINE_VIEW, OnModelEditSwitchOnlineView)	
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_SWITCH_ONLINE_VIEW, OnUpdateModelEditSwitchOnlineView)
	ON_COMMAND(MENU_MODEL_EDIT_SWITCH_EDIT_MAIN, OnModelEditSwitchEditMain)	
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_SWITCH_EDIT_MAIN, OnUpdateModelEditSwitchEditMain)
	ON_COMMAND(ID_SHOW_FRAME_01, OnShowFrame01)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_01, OnUpdateShowFrame01)	
	ON_COMMAND(ID_SHOW_FRAME_02, OnShowFrame02)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_02, OnUpdateShowFrame02)	
	ON_COMMAND(ID_SHOW_FRAME_03, OnShowFrame03)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_03, OnUpdateShowFrame03)	
	ON_COMMAND(ID_SHOW_FRAME_04, OnShowFrame04)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_04, OnUpdateShowFrame04)
	ON_COMMAND(ID_SHOW_FRAME_05, OnShowFrame05)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_05, OnUpdateShowFrame05)	
	ON_COMMAND(ID_SHOW_FRAME_06, OnShowFrame06)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_06, OnUpdateShowFrame06)	
	ON_COMMAND(ID_SHOW_FRAME_07, OnShowFrame07)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_07, OnUpdateShowFrame07)	
	ON_COMMAND(ID_SHOW_FRAME_08, OnShowFrame08)	
	ON_UPDATE_COMMAND_UI(ID_SHOW_FRAME_08, OnUpdateShowFrame08)
	ON_COMMAND(ID_EDIT_MODEL_ADD, OnEditModelAdd)	
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODEL_ADD, OnUpdateEditModelAdd)
	ON_COMMAND(ID_EDIT_MODEL_EDIT, OnEditModelEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODEL_EDIT, OnUpdateEditModelEdit)
	ON_COMMAND(ID_EDIT_MODEL_SELECT, OnEditModelSelect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODEL_SELECT, OnUpdateEditModelSelect)
	ON_COMMAND(ID_EDIT_MODEL_AUTO_ADD, OnEditModelAutoAdd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MODEL_AUTO_ADD, OnUpdateEditModelAutoAdd)
	ON_COMMAND(MENU_MODEL_ADD_LAND_ELECTRODE, OnModelAddLandElectrode)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_ADD_LAND_ELECTRODE, OnUpdateModelAddLandElectrode)
	ON_COMMAND(MENU_MODEL_ADD_LAND_IC_LEAD, OnModelAddLandICLead)	
	ON_UPDATE_COMMAND_UI(MENU_MODEL_ADD_LAND_IC_LEAD, OnUpdateModelAddLandICLead)
	ON_COMMAND(MENU_MODEL_ADD_LAND_CON_LEAD, OnModelAddLandConLead)	
	ON_UPDATE_COMMAND_UI(MENU_MODEL_ADD_LAND_CON_LEAD, OnUpdateModelAddLandConLead)
	ON_COMMAND(MENU_MODEL_ADD_LAND_PAD, OnModelAddLandPad)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_ADD_LAND_PAD, OnUpdateModelAddLandPad)	
	ON_COMMAND(MENU_MODEL_EDIT_LAND_COUNT, OnModelEditLandCount)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_COUNT, OnUpdateModelEditLandCount)
	ON_COMMAND(MENU_MODEL_EDIT_LAND_COUNT_2D, OnModelEditLandCount2D)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_COUNT_2D, OnUpdateModelEditLandCount2D)
	ON_COMMAND(MENU_MODEL_EDIT_LAND_ARRAY, OnModelEditLandArray)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_ARRAY, OnUpdateModelEditLandArray)
	ON_COMMAND(MENU_MODEL_EDIT_LAND_PITCH, OnModelEditLandPitch)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_PITCH, OnUpdateModelEditLandPitch)
	ON_COMMAND(MENU_MODEL_EDIT_LAND_ALIGN, OnModelEditLandAlign)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_ALIGN, OnUpdateModelEditLandAlign)
	ON_COMMAND(MENU_MODEL_EDIT_LAND_INCLUDE_PAD_ALIGN, OnModelEditLandIncludePadAlign)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_INCLUDE_PAD_ALIGN, OnUpdateModelEditLandIncludePadAlign)
	ON_COMMAND(MENU_MODEL_EDIT_LAND_INCLUDE_PART_ALIGN, OnModelEditLandIncludePartAlign)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_LAND_INCLUDE_PART_ALIGN, OnUpdateModelEditLandIncludePartAlign)
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_PASTE, OnModelEditClonePaste)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_PASTE, OnUpdateModelEditClonePaste)
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_ROTATE_090, OnModelEditCloneRotate090)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_ROTATE_090, OnUpdateModelEditCloneRotate090)	
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_ROTATE_180, OnModelEditCloneRotate180)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_ROTATE_180, OnUpdateModelEditCloneRotate180)
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_ROTATE_270, OnModelEditCloneRotate270)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_ROTATE_270, OnUpdateModelEditCloneRotate270)	
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_MIRROR_X_POS, OnModelEditCloneMirrorXPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_MIRROR_X_POS, OnUpdateModelEditCloneMirrorXPos)
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_MIRROR_Y_POS, OnModelEditCloneMirrorYPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_MIRROR_Y_POS, OnUpdateModelEditCloneMirrorYPos)	
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_DIAGONAL, OnModelEditCloneDiagonal)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_DIAGONAL, OnUpdateModelEditCloneDiagonal)		
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_CORNER_4, OnModelEditCloneCorner4)	
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_CORNER_4, OnUpdateModelEditCloneCorner4)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_SELECT, OnModelEditDeleteSelect)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_SELECT, OnUpdateModelEditDeleteSelect)	
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_OTHERS, OnModelEditDeleteOthers)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_OTHERS, OnUpdateModelEditDeleteOthers)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_GROUP, OnModelEditDeleteGroup)	
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_GROUP, OnUpdateModelEditDeleteGroup)	
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_ALL, OnModelEditDeleteAll)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_ALL, OnUpdateModelEditDeleteAll)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_ALL_WND, OnModelEditDeleteAllWnd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_ALL_WND, OnUpdateModelEditDeleteAllWnd)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_DERIVATIVE, OnModelEditDeleteDerivative)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_DERIVATIVE, OnUpdateModelEditDeleteDerivative)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_COMPONENT, OnModelEditDeleteComponent)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_COMPONENT, OnUpdateModelEditDeleteComponent)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE_GROUP_LIBRARY, OnModelEditDeleteGroupLibrary)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE_GROUP_LIBRARY, OnUpdateModelEditDeleteGroupLibrary)
	ON_COMMAND(MENU_MODEL_EDIT_MODIFY_POS, OnModelEditModifyPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MODIFY_POS, OnUpdateModelEditModifyPos)
	ON_COMMAND(MENU_MODEL_EDIT_MODIFY_SIZE, OnModelEditModifySize)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MODIFY_SIZE, OnUpdateModelEditModifySize)
	ON_COMMAND(MENU_MODEL_EDIT_ROTATE_090, OnModelEditRotate090)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ROTATE_090, OnUpdateModelEditRotate090)
	ON_COMMAND(MENU_MODEL_EDIT_ROTATE_180, OnModelEditRotate180)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ROTATE_180, OnUpdateModelEditRotate180)
	ON_COMMAND(MENU_MODEL_EDIT_ROTATE_270, OnModelEditRotate270)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ROTATE_270, OnUpdateModelEditRotate270)
	ON_COMMAND(MENU_MODEL_EDIT_MIRROR_X_POS, OnModelEditMirrorXPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MIRROR_X_POS, OnUpdateModelEditMirrorXPos)	
	ON_COMMAND(MENU_MODEL_EDIT_MIRROR_Y_POS, OnModelEditMirrorYPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MIRROR_Y_POS, OnUpdateModelEditMirrorYPos)	
	ON_COMMAND(MENU_MODEL_EDIT_ALIGN_CENTER, OnModelEditAlignCenterPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ALIGN_CENTER, OnUpdateModelEditAlignCenterPos)	
	ON_COMMAND(MENU_MODEL_EDIT_ALIGN_CENTER_U, OnModelEditAlignCenterPosU)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ALIGN_CENTER_U, OnUpdateModelEditAlignCenterPosU)	
	ON_COMMAND(MENU_MODEL_EDIT_ALIGN_CENTER_V, OnModelEditAlignCenterPosV)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ALIGN_CENTER_V, OnUpdateModelEditAlignCenterPosV)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_RECTANGLE, OnModelEditWndShapeRectangle)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_RECTANGLE, OnUpdateModelEditWndShapeRectangle)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_ROUND_RECT, OnModelEditWndShapeRoundRect)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_ROUND_RECT, OnUpdateModelEditWndShapeRoundRect)	
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_ELLIPSE, OnModelEditWndShapeEllipse)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_ELLIPSE, OnUpdateModelEditWndShapeEllipse)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_CAPSULE, OnModelEditWndShapeCapsule)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_CAPSULE, OnUpdateModelEditWndShapeCapsule)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_BULLET, OnModelEditWndShapeBullet)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_BULLET, OnUpdateModelEditWndShapeBullet)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_HALF_ROUND_RECT, OnModelEditWndShapeHalfRoundRect)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_HALF_ROUND_RECT, OnUpdateModelEditWndShapeHalfRoundRect)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_T_SHAPE, OnModelEditWndShapeTShape)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_T_SHAPE, OnUpdateModelEditWndShapeTShape)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_PARAM, OnModelEditWndShapeParam)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_PARAM, OnUpdateModelEditWndShapeParam)
	ON_COMMAND(MENU_MODEL_EDIT_WND_SHAPE_PARAM_2, OnModelEditWndShapeParam2)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_WND_SHAPE_PARAM_2, OnUpdateModelEditWndShapeParam2)
	ON_COMMAND(MENU_MODEL_EDIT_GROUP_ID, OnModelEditGroupID)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_GROUP_ID, OnUpdateModelEditGroupID)	
	ON_COMMAND(MENU_MODEL_EDIT_BAND_ID, OnModelEditBandID)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_BAND_ID, OnUpdateModelEditBandID)	
	ON_COMMAND(MENU_MODEL_EDIT_ADD_ALL_WND, OnModelEditAddAllWnd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ADD_ALL_WND, OnUpdateModelEditAddAllWnd)
	ON_COMMAND(MENU_MODEL_EDIT_ADD_ALL_WND_V2, OnModelEditAddAllWndv2)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ADD_ALL_WND_V2, OnUpdateModelEditAddAllWndv2)
	ON_COMMAND(MENU_MODEL_EDIT_ADD_ALL_WND_GROUP, OnModelEditAddAllWndGroup)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ADD_ALL_WND_GROUP, OnUpdateModelEditAddAllWndGroup)
	ON_COMMAND(ID_TUNE_ALIGN_FIDUCIAL, OnTuneAlignFiducial)
	ON_UPDATE_COMMAND_UI(ID_TUNE_ALIGN_FIDUCIAL, OnUpdateTuneAlignFiducial)		
	ON_COMMAND(ID_TUNE_INSPECTION, OnTuneInspection)
	ON_UPDATE_COMMAND_UI(ID_TUNE_INSPECTION, OnUpdateTuneInspection)
	ON_COMMAND(ID_TUNE_SELECTED_COMPONENT, OnTuneSelectedComponent)
	ON_UPDATE_COMMAND_UI(ID_TUNE_SELECTED_COMPONENT, OnUpdateTuneSelectedComponent)
	ON_COMMAND(ID_TUNE_SELECTED_MODEL, OnTuneSelectedModel)
	ON_UPDATE_COMMAND_UI(ID_TUNE_SELECTED_MODEL, OnUpdateTuneSelectedModel)
	ON_COMMAND(MENU_MODEL_EDIT_UPDATE_TO_LIBRARY, OnModelEditUpdateToLibrary)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_UPDATE_TO_LIBRARY, OnUpdateModelEditUpdateToLibrary)
	ON_COMMAND(MENU_MODEL_EDIT_ARRANGE_MODEL, OnModelEditArrangeModel)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ARRANGE_MODEL, OnUpdateModelEditArrangeModel)
	ON_COMMAND(MENU_MODEL_EDIT_SHOW_MODEL_ACTIVED_LINE, OnModelEditShowModelActiveLine)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_SHOW_MODEL_ACTIVED_LINE, OnUpdateModelEditShowModelActiveLine)
	ON_COMMAND(MENU_MODEL_EDIT_UPDATE_FROM_LIBRARY, OnModelEditUpdateFromLibrary)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_UPDATE_FROM_LIBRARY, OnUpdateModelEditUpdateFromLibrary)
	ON_COMMAND(MENU_MODEL_EDIT_UPDATE_TO_LIBRARY_GROUP, OnModelEditUpdateToLibraryGroup)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_UPDATE_TO_LIBRARY_GROUP, OnUpdateModelEditUpdateToLibraryGroup)
	ON_COMMAND(MENU_MODEL_EDIT_SAVE_DEFAULT_MODEL, OnModelEditSaveDefaultModel)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_SAVE_DEFAULT_MODEL, OnUpdateModelEditSaveDefaultModel)
	ON_COMMAND(MENU_MODEL_EDIT_PROJECT_SAVE, OnModelEditProjecSave)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_PROJECT_SAVE, OnUpdateModelEditProjecSave)	
	ON_COMMAND(MENU_MODEL_EDIT_SHOW_AGENT_LIST, OnModelEditShowAgentList)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_SHOW_AGENT_LIST, OnUpdateModelEditShowAgentList)
	ON_COMMAND(MENU_MODEL_INSPECT_COMPONENT, OnModelInspectComponent)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_INSPECT_COMPONENT, OnUpdateModelInspectComponent)	
	ON_COMMAND(ID_SHOW_MODEL_EDIT_MODE, OnShowModelEditMode)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_EDIT_MODE, OnUpdateShowModelEditMode)
	ON_COMMAND(ID_SHOW_MODEL_RESULT_MODE, OnShowModelResultMode)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_RESULT_MODE, OnUpdateShowModelResultMode)
	ON_COMMAND(ID_SHOW_MODEL_LANDS, OnShowModelLands)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_LANDS, OnUpdateShowModelLands)
	ON_COMMAND(ID_HIDE_MODEL_LANDS, OnHideModelLands)
	ON_UPDATE_COMMAND_UI(ID_HIDE_MODEL_LANDS, OnUpdateHideModelLands)
	ON_COMMAND(ID_SHOW_ALL_COMPONENTS, OnShowAllComponents)
	ON_UPDATE_COMMAND_UI(ID_SHOW_ALL_COMPONENTS, OnUpdateShowAllComponents)	
	ON_COMMAND(ID_SHOW_MODEL_WND_INDEX, OnShowModelWndIndex)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_WND_INDEX, OnUpdateShowModelWndIndex)	
	ON_COMMAND(ID_SHOW_MODEL_LAND_INDEX, OnShowModelLandIndex)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_LAND_INDEX, OnUpdateShowModelLandIndex)	
	ON_COMMAND(MENU_MODEL_EDIT_CREATE_BK_IMAGE, OnEditModelBkImage)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CREATE_BK_IMAGE, OnUpdateEditModelBkImage)
	ON_COMMAND(MENU_MODEL_EDIT_CREATE_BK_IMAGE_ALL, OnEditModelBkImageAll)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CREATE_BK_IMAGE_ALL, OnUpdateEditModelBkImageAll)
	ON_COMMAND(MENU_MODEL_EDIT_CLONE_ARRAY_PASTE, OnModelEditCloneArrayPaste)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE_ARRAY_PASTE, OnUpdateModelEditCloneArrayPaste)
	ON_COMMAND(ID_SHOW_MODEL_ACTIVED_LINE, OnShowModelActivedLine)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_ACTIVED_LINE, OnUpdateShowModelActivedLine)
	ON_COMMAND(MENU_MODEL_LINK_LAND_WND_POS, OnModelLinkLandWndPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_LINK_LAND_WND_POS, OnUpdateModelLinkLandWndPos)
	ON_COMMAND(MENU_MODEL_LINK_LAND_WND_SIZE, OnModelLinkLandWndSize)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_LINK_LAND_WND_SIZE, OnUpdateModelLinkLandWndSize)
	ON_COMMAND(MENU_MODEL_LINK_LAND_POS, OnModelLinkLandPos)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_LINK_LAND_POS, OnUpdateModelLinkLandPos)
	ON_COMMAND(MENU_MODEL_LINK_LAND_SIZE, OnModelLinkLandSize)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_LINK_LAND_SIZE, OnUpdateModelLinkLandSize)	
	ON_COMMAND(ID_TUNE_REPEATE_INSPECTION, OnTuneRepeateInspection)
	ON_UPDATE_COMMAND_UI(ID_TUNE_REPEATE_INSPECTION, OnUpdateTuneRepeateInspection)
	ON_COMMAND(ID_TUNE_SELECTED_PART_NUMBER, OnTuneSelectedPartNumber)
	ON_UPDATE_COMMAND_UI(ID_TUNE_SELECTED_PART_NUMBER, OnUpdateTuneSelectedPartNumber)
	ON_COMMAND(ID_TUNE_SELECTED_MODEL_GROUP, OnTuneSelectedModelGroup)
	ON_UPDATE_COMMAND_UI(ID_TUNE_SELECTED_MODEL_GROUP, OnUpdateTuneSelectedModelGroup)
	ON_COMMAND(ID_TUNE_INSPECTION_GROUP, OnTuneInspectionGroup)
	ON_UPDATE_COMMAND_UI(ID_TUNE_INSPECTION_GROUP, OnUpdateTuneInspectionGroup)
	ON_COMMAND(MENU_MODEL_EDIT_MODIFY, OnModelEditModify)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MODIFY, OnUpdateModelEditModify)
	ON_COMMAND(MENU_MODEL_EDIT_CLONE, OnModelEditClone)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_CLONE, OnUpdateModelEditClone)
	ON_COMMAND(MENU_MODEL_EDIT_DELETE, OnModelEditDelete)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DELETE, OnUpdateModelEditDelete)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_ADD, OnModelEditMaskBoxAdd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_ADD, OnUpdateModelEditMaskBoxAdd)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_ROTATE_090, OnModelEditMaskBoxRotate090)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_ROTATE_090, OnUpdateModelEditMaskBoxRotate090)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_ROTATE_180, OnModelEditMaskBoxRotate180)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_ROTATE_180, OnUpdateModelEditMaskBoxRotate180)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_ROTATE_270, OnModelEditMaskBoxRotate270)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_ROTATE_270, OnUpdateModelEditMaskBoxRotate270)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_RECTANGLE, OnModelEditMaskBoxShapeRectangle)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_RECTANGLE, OnUpdateModelEditMaskBoxShapeRectangle)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_ROUND_RECT, OnModelEditMaskBoxShapeRoundRect)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_ROUND_RECT, OnUpdateModelEditMaskBoxShapeRoundRect)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_ELLIPSE, OnModelEditMaskBoxShapeEllipse)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_ELLIPSE, OnUpdateModelEditMaskBoxShapeEllipse)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_CAPSULE, OnModelEditMaskBoxShapeCapsule)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_CAPSULE, OnUpdateModelEditMaskBoxShapeCapsule)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_BULLET, OnModelEditMaskBoxShapeBullet)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_BULLET, OnUpdateModelEditMaskBoxShapeBullet)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_HALF_ROUND_RECT, OnModelEditMaskBoxShapeHalfRoundRect)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_HALF_ROUND_RECT, OnUpdateModelEditMaskBoxShapeHalfRoundRect)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_T_SHAPE, OnModelEditMaskBoxShapeTShape)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_T_SHAPE, OnUpdateModelEditMaskBoxShapeTShape)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_PARAM, OnModelEditMaskBoxShapeParam)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_PARAM, OnUpdateModelEditMaskBoxShapeParam)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_SHAPE_PARAM_2, OnModelEditMaskBoxShapeParam2)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_SHAPE_PARAM_2, OnUpdateModelEditMaskBoxShapeParam2)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_ERASE, OnModelEditMaskBoxErase)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_ERASE, OnUpdateModelEditMaskBoxErase)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_DELETE, OnModelEditMaskBoxDelete)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_DELETE, OnUpdateModelEditMaskBoxDelete)
	ON_COMMAND(MENU_MODEL_EDIT_MASK_BOX_CLEAR, OnModelEditMaskBoxClear)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_MASK_BOX_CLEAR, OnUpdateModelEditMaskBoxClear)	
	ON_COMMAND(MENU_MODEL_EDIT_GLOBAL_CLONE_WND, OnModelEditGlobalCloneWnd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_GLOBAL_CLONE_WND, OnUpdateModelEditGlobalCloneWnd)	
	ON_COMMAND(MENU_MODEL_EDIT_GLOBAL_COPY_WND, OnModelEditGlobalPasteWnd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_GLOBAL_COPY_WND, OnUpdateModelEditGlobalPasteWnd)	
	ON_COMMAND(MENU_MODEL_EDIT_SHOW_LIBRARY_WND, OnModelEditShowLibraryWnd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_SHOW_LIBRARY_WND, OnUpdateModelEditShowLibraryWnd)
	ON_COMMAND(MENU_MODEL_EDIT_PROPERTY_WND, OnModelEditPropertyWnd)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_PROPERTY_WND, OnUpdateModelEditPropertyWnd)	
	ON_COMMAND(MENU_MODEL_EDIT_ENABLE_ALL_WND, OnModelEditEnableAllWnds)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_ENABLE_ALL_WND, OnUpdateModelEditEnableAllWnds)	
	ON_COMMAND(MENU_MODEL_EDIT_DISABLE_ALL_WND, OnModelEditDisableAllWnds)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_EDIT_DISABLE_ALL_WND, OnUpdateModelEditDisableAllWnds)
	ON_COMMAND(MENU_MODEL_AUTO_ADD_VER, OnModelAutoAddLandVer)
	ON_COMMAND(MENU_MODEL_AUTO_ADD_HOR, OnModelAutoAddLandHor)
	ON_COMMAND(MENU_MODEL_AUTO_ADD_AROUND, OnModelAutoAddLandBoth)
	ON_UPDATE_COMMAND_UI(MENU_MODEL_AUTO_ADD_VER, OnUpdateModelAddLandElectrode)
	ON_COMMAND(ID_ONLINE_TUNING_SMART_CHART, OnModelSmartChart)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_TUNING_SMART_CHART, OnUpdateModelSmartChart)
	ON_COMMAND(ID_ONLINE_TUNING_DATA_STATIC, OnModelDataStatic)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_TUNING_DATA_STATIC, OnUpdateModelSmartChart)
	ON_COMMAND(ID_ONLINE_TUNING_SCAN_STATIC, OnScanDataStatic)
	ON_UPDATE_COMMAND_UI(ID_ONLINE_TUNING_SCAN_STATIC, OnUpdateModelSmartChart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelView drawing
//-------------------------------------------------------------------------------------//
void CEditModelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CEditModelView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelView diagnostics
//-------------------------------------------------------------------------------------//
#ifdef _DEBUG
void CEditModelView::AssertValid() const
{
	CView::AssertValid();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG
//-------------------------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////////
// CEditModelView message handlers
//-------------------------------------------------------------------------------------//
void CEditModelView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
//	ResizeParentToFit();//會讓主視窗調整成FormView的尺寸

	// TODO: Add your specialized code here and/or call the base class
	SwitchMultiLanguage();
	CWnd::GetClientRect(&m_ImageWndRect);
	m_ImageWndMapDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC.CreateMemDC(this, m_BkColor);
	m_ImageWndMemDC2.CreateMemDC(this, m_BkColor);
	
	SetShowPopupMenu(true);
	AOIDataCollect.SetShowFdList(true);	
	AOIDataCollect.SetShowMarkList(false);
	AOIDataCollect.SetShowPanelList(true);	
	AOIDataCollect.SetShowBoardList(true);	
	AOIDataCollect.SetShowBarcodeList(false);
	AOIDataCollect.SetShowComponentList(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_COMPONENT);	
	//AOIDataCollect.SwitchProjectTaskMode(PROJECT_TASK_NORMAL);

	//CEditModelView::BuildModel();	
	SwitchProject();	
	RestoreViewParam();
	if ( ExecMoveToComponentStage() == false )
	{	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	CloseProject();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if ( CWnd::GetSafeHwnd() == NULL ) { return; }	
	//if ( m_ImageWnd.GetSafeHwnd() != NULL )
	if ( CWnd::GetSafeHwnd() != NULL )
	{
		RECT  WndRect={0};
		const int Margin = 4;		
		CWnd::GetClientRect(&m_ImageWndRect);
		m_ImageWndMapDC.CreateMemDC(this, m_BkColor);
		m_ImageWndMemDC.CreateMemDC(this, m_BkColor);
		m_ImageWndMemDC2.CreateMemDC(this, m_BkColor);
		CreateBKImage();
		CreateMapImage();
		RedrawWnd();
	}
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	return CView::OnEraseBkgnd(pDC);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CView::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if ( TRUE == bShow )
	{				
		AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
		AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_EDIT);		
	}
	else
	{
		if ( AOIDataCollect.GetOfflineMode() == true )
		{	CalcFovPosition();	}
		BackupViewParam();
		CloseProject();	
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if ( NULL == pWnd ) { return; }
	const UINT CtrlID = pWnd->GetDlgCtrlID();
	POINT dPos, LastPos;
	//不要用m_MousePosLast, 因為可能收不到OnMouseMove	
	if ( GetShowPopupMenu() == false )
	{
		SetShowPopupMenu(true);
		return;
	}	

	LastPos = point;
	CWnd::ScreenToClient(&LastPos);
	dPos.x = LastPos.x - m_MousePosFirst.x;
	dPos.y = LastPos.y - m_MousePosFirst.y;

	if ( ::abs(dPos.x)>2 || ::abs(dPos.y)>2 )
	{	return; }
	
	AOIDataCollect.CancelGatherColorMode();
	const bool bGetLockUIWnd = GetLockUIWnd();
	const bool SwitchFrameMode = AOIDataCollect.CheckSwitchFrameMode();		
	if ( false == bGetLockUIWnd )
	{
		if ( true==SwitchFrameMode )
		{	SwitchFrameImage();	 }
		else
		{	ExecPopupMenu_Edit(point);	}
	}
	
	/*
	switch ( CtrlID )
	{
	case 0:
		
		break;
	default:
		//::AfxMessageBox(_T("CEditModelView::OnContextMenu"));
		break;
	}
	*/
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	UINT ControlID = pWnd->GetDlgCtrlID();	

	CURSOR_POS_MODE OldCursorMode = m_MousePosMode;
	CURSOR_POS_MODE CursorMode = CheckCursorPosMode(m_MousePosImageWnd);
	m_MousePosMode = CursorMode;
	
	//if ( CursorMode != OldCursorMode )
	//{	CEditModelView::RedrawWnd();	}
	if ( CURSOR_POS_NONE == CursorMode )
	{	return CView::OnSetCursor(pWnd, nHitTest, message);	}

	JetAPI::UpdateCursor(CursorMode); 
	return TRUE;
	//return CView::OnSetCursor(pWnd, nHitTest, message);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT pt2={0};
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;		
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);	

	m_DrawAddRect = true;	
	m_ChangeComponentSelected = false;		
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_EDIT == ManiMode )
	{
		if ( CURSOR_POS_NONE == m_MousePosMode )
		{	m_ChangeComponentSelected = ExecModelComponentSelect();	 }
	}	

	CheckActiveObjFocus(m_ActiveObj, m_ActiveBox);
	CView::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecSaveLogLButtonUp()
{
	if ( NULL == m_ActiveObj.BoxPtr ) { return true; }
	TActiveObj ActiveObj;			
	CheckActiveObjFocus(ActiveObj);	
	if ( m_ActiveObj.BoxPtr != ActiveObj.BoxPtr )
	{
		m_ActiveObj = TActiveObj();
		return true; 
	}

	double dPx=0, dPy=0;
	TREGION4D ModifyRgn;
	//DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	CAOIBox   *BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ActiveObj.BoxPtr);	
	CAOIWnd   *WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ActiveObj.WndPtr);
	CAOILand  *LandPtr = DYNAMIC_DOWNCAST(CAOILand, ActiveObj.LandPtr);
	CAOIModel *ModelPtr = DYNAMIC_DOWNCAST(CAOIModel, ActiveObj.ModelPtr);
	CAOIWndRoi *WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ActiveObj.WndRoiPtr);
	CAOIWndMask *WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ActiveObj.WndMaskPtr);
	if ( NULL == BoxPtr ) { return true; }

	switch ( m_MousePosMode )
	{
	case CURSOR_POS_INNER://Pos
		dPx = BoxPtr->GetBoxPosX()-m_ActiveBox.GetBoxPosX();
		dPy = BoxPtr->GetBoxPosY()-m_ActiveBox.GetBoxPosY();
		LogOperCtrl.SaveLogModelModifyPos(ModelPtr, BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dPx, dPy);
		break;
	case CURSOR_POS_LEFT:
	case CURSOR_POS_RIGHT:
	case CURSOR_POS_TOP:
	case CURSOR_POS_BOTTOM:
	case CURSOR_POS_LEFT_TOP:
	case CURSOR_POS_LEFT_BOTTOM:
	case CURSOR_POS_RIGHT_TOP:
	case CURSOR_POS_RIGHT_BOTTOM://Size
		ModifyRgn.minX = 0;
		ModifyRgn.minY = 0;
		ModifyRgn.maxX = BoxPtr->GetBoxSizeX()-m_ActiveBox.GetBoxSizeX();
		ModifyRgn.maxY = BoxPtr->GetBoxSizeY()-m_ActiveBox.GetBoxSizeY();
		LogOperCtrl.SaveLogModelModifySize(ModelPtr, BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, ModifyRgn);
		break;
	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);	
	MANIPULATE_MODEL_MODE ManiMode = CEditModelView::GetManiModelMode();
	CString               str;
	if ( MANIPULATE_MODEL_ADD == ManiMode )
	{
		POINT dPt;
		dPt.x = ::abs(m_MousePosLast.x-m_MousePosFirst.x);
		dPt.y = ::abs(m_MousePosLast.y-m_MousePosFirst.y);
		CAOIModel *ModelPtr = GetModelPtr();
		if ( NULL != ModelPtr )
		{
			if ( dPt.x>2 && dPt.y>2 )
			{	
				LAND_TYPE LandType = ModelPtr->GetModelLandTypeMaster();
				CAOIBox  *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
				CAOIWnd  *WndPtr = ModelPtr->GetModelWndActived();
				CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
				const int LandTypeCount = ModelPtr->GetModelSupportLandTypeCount();				
				if ( NULL==LandPtr && BoxPtr->GetBoxSelected()==false && NULL==WndPtr )
				{				
					if ( 1 == LandTypeCount )
					{	ExecModelAddLand(LandType);	}
					else if ( 0==LandTypeCount || 1<LandTypeCount )
					{
						POINT MenuPt = point;
						CWnd::ClientToScreen(&MenuPt);
						ExecPopupMenu_Add(MenuPt); 
					}
				}
				else
				{	ExecModelAddWnd();	}
				m_DrawAddRect = false;
			}
			else
			{
				m_DrawAddRect = false;
				ExecModelRegionSelect();
				RedrawWnd();
			}
		}
	}
	else if ( MANIPULATE_MODEL_SELECT==ManiMode || MANIPULATE_MODEL_EDIT==ManiMode )
	{
		m_DrawAddRect = false;
		if ( false==m_ChangeComponentSelected && CURSOR_POS_NONE==m_MousePosMode )
		{	
			ExecModelRegionSelect();
			ExecModelWndInspection(true);
			//UpdateImageByAlgParam();
		}
		else if ( CURSOR_POS_NONE!=m_MousePosMode )
		{
			ExecSaveLogLButtonUp();
			ExecModelWndInspection(true);
			UpdateImageByAlgParam();			
		}
		CEditModelView::RedrawWnd();
	}
	else if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
	{
		m_DrawAddRect = false;
		const bool CombineColorMode = AOIDataCollect.CheckCombineColorMode();
		ExecGatherColorFilter(CombineColorMode);		
		if ( false == CombineColorMode ) 		
		{	AOIDataCollect.CancelGatherColorMode();	 }
		RedrawWnd();
	}
	else if (MANIPULATE_MODEL_AUTO_ADD == ManiMode)
	{
		CAOIModel *ModelPtr = GetModelPtr();
		const bool bEnableAutoAddICLead = true;
		if (ModelPtr->GetModelType() == MODEL_TYPE_LEAD_COMPONENT || ModelPtr->GetModelType() == MODEL_TYPE_JLEAD_PLCC || ModelPtr->GetModelType() == MODEL_TYPE_TRANSISTOR)
		{
			if (true == bEnableAutoAddICLead)
			{
				POINT MenuPt = point;
				CWnd::ClientToScreen(&MenuPt);
				ExecPopupMenu_Auto(MenuPt);
				m_DrawAddRect = false;
			}
		}
		SwitchManiModelMode(AOIDataCollect.GetManipulateModelModeDefault());
	}
	CView::OnLButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	POINT pt2={0};
	m_DrawAddRect = false;
	if ( PtInControlWnd(point, MODELEDIT_IMAGE_WND, pt2) == false )
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}	
	CWnd::SetFocus();
	CWnd::SetCapture();
	m_MousePosImageWnd = m_MousePosCurrent = m_MousePosFirst = m_MousePosLast = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	CView::OnRButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	::ReleaseCapture();		
	m_MousePosImageWnd = m_MousePosCurrent = point;		
	if ( AOIDataCollect.CancelManipulateMainMode() == true ) 
	{	SetShowPopupMenu(false); }
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);		
	if ( CheckMousePosMoved() )//20241209
	{	AdjustCurrentFrames();	}	
	CView::OnRButtonUp(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SWITCH_TO_EDIT_MAIN_VIEW, (LPARAM)(this));
	CView::OnRButtonDblClk(nFlags, point);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	POINT dPoint;
	m_MousePosLast = m_MousePosCurrent;	
	m_MousePosImageWnd = m_MousePosCurrent = point;
	//CWnd::MapWindowPoints(&m_ImageWnd, &m_MousePosImageWnd, 1);
	dPoint.x = point.x - m_MousePosLast.x;
	dPoint.y = point.y - m_MousePosLast.y;
	if ( this != CWnd::GetCapture() )
	{
		RedrawWnd();
		CView::OnMouseMove(nFlags, point);
		return;
	}

	//JetAPI::UpdateCursor();
	if ( nFlags & MK_LBUTTON )//滑鼠左鍵
	{
		bool Modify = false;
		MANIPULATE_MODEL_MODE ManiMode = CEditModelView::GetManiModelMode();
		if ( MANIPULATE_MODEL_ADD == ManiMode )
		{	Modify = true;	}
		else 
		{
			switch ( m_MousePosMode )
			{
			case CURSOR_POS_INNER:
				Modify = ExecModifyActiveObjPos();
				break;
			case CURSOR_POS_LEFT:
			case CURSOR_POS_RIGHT:
			case CURSOR_POS_TOP:
			case CURSOR_POS_BOTTOM:
			case CURSOR_POS_LEFT_TOP:
			case CURSOR_POS_LEFT_BOTTOM:
			case CURSOR_POS_RIGHT_TOP:
			case CURSOR_POS_RIGHT_BOTTOM:
				Modify = ExecModifyActiveObjSize(m_MousePosMode);
				break;
			}
		}
		if ( MANIPULATE_MODEL_SELECT == ManiMode )
		{	Modify = true;	}
		if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode )
		{	Modify = true;	}

		Modify = true;
		if ( true == Modify )
		{	CEditModelView::RedrawWnd();	}
	}
	else if (nFlags & MK_RBUTTON )//滑鼠右鍵
	{
		this->m_ImageOffset.x += dPoint.x;
		this->m_ImageOffset.y += dPoint.y;
		CreateBKImage();
		RedrawWnd();
	}	
	
	CView::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	double NextImageZoom = m_ImageZoom;
	if ( zDelta > 0 ) 
	{	NextImageZoom *= 1.1;	}
	else
	{	NextImageZoom /= 1.1;	}
	const bool   OfflineMode = AOIDataCollect.GetOfflineMode();
	const bool   ShowProjectMapMode = GetShowProjectMapMode();
	NextImageZoom = AOIDataCollect.AdjustImageZoom(NextImageZoom);			
	ImageAPI.CalcImageWndZoom(m_ImageZoom, NextImageZoom, m_ImageOffset);
	m_ImageZoom = NextImageZoom;

	if ( true==OfflineMode && false==ShowProjectMapMode )
	{
		MASK_PTR   MaskPtr=NULL;
		SPACE_PTR  SpacePtr=NULL;
		IMAGE_PTR  ImagePtr=NULL;
		IMAGE_SIZE ImageW=0;
		IMAGE_SIZE ImageH=0;
		IMAGE_SIZE ImageStep=0;
		IMAGE_SIZE BitCount=0;
		if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) 
		{	return TRUE; }

		//確認是否重新補圖
		if ( NULL!=ImagePtr && m_ImageZoom>1 )
		{			
			const int RealWndW = m_ImageWndRect.right-m_ImageWndRect.left;
			const int RealWndH = m_ImageWndRect.bottom-m_ImageWndRect.top;
			const int WndSizeW = (int)(ImageW/m_ImageZoom);
			const int WndSizeH = (int)(ImageH/m_ImageZoom);
			IMAGE_SIZE BasicImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
			IMAGE_SIZE BasicImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);	

			if ( WndSizeW<RealWndW || WndSizeH<RealWndH )
			{	
				double Ratio = 1;
				double RatioW = (double)(ImageW);
				double RatioH = (double)(ImageH);
				double dBImageW = (double)(BasicImageW);
				double dBImageH = (double)(BasicImageH);
				RatioW = RatioW/dBImageW;
				RatioH = RatioH/dBImageH;
				//每次擴增0.5個FOV
				RatioW = RatioW+0.5;
				RatioH = RatioH+0.5;
				Ratio = MAX(RatioW, RatioH);
				if ( Ratio < 1.0 ) 
				{	Ratio = 1.0; }
				FillCurrentFrames(Ratio);
				BuildShowImageBuffer();
			}		
		}
	}
	//POINT point = pt;
	//CWnd::ScreenToClient(&point);
	//CalcCursorInfo(point);
	//CalcImageWndLBtn();
	//DrawImage();
	CreateBKImage();
	RedrawWnd();
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::PtInControlWnd(const POINT &pt, UINT ID, POINT &pt2)
{
	pt2 = pt;
	if ( ::PtInRect(&m_ImageWndRect, pt) == FALSE )
	{	return false; }
	return true;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditModelView::GetFrameImageW_2() const
{
	return m_ShowImageW;
}
//-------------------------------------------------------------------------------------//
inline IMAGE_SIZE CEditModelView::GetFrameImageH_2() const
{
	return m_ShowImageH;
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditModelView::GetImageW() const
{
	return GetFrameImageW_2();
}
//-------------------------------------------------------------------------------------//
IMAGE_SIZE CEditModelView::GetImageH() const
{
	return GetFrameImageH_2();
}
//-------------------------------------------------------------------------------------//
const TPOINT2D& CEditModelView::GetImageResolution() const
{
	return m_FrameResolution;
}
//-------------------------------------------------------------------------------------//
const TREGION4D& CEditModelView::GetImageStageRgn() const
{
	return m_FrameStageRgn;
}
//-------------------------------------------------------------------------------------//
const TPOINT2D CEditModelView::GetImageStageRgnCp() const
{
	TPOINT2D ImageStageRgnCp;
	const TREGION4D &ImageStageRgn = GetImageStageRgn();
	ImageStageRgnCp.x = ImageStageRgn.GetCpX();
	ImageStageRgnCp.y = ImageStageRgn.GetCpY();
	return ImageStageRgnCp;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::GetShowProjectMapMode() const
{
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CheckMousePosMoved() const//確認滑鼠移動過
{
	const int dx = m_MousePosLast.x-m_MousePosFirst.x;
	const int dy = m_MousePosLast.y-m_MousePosFirst.y;
	if ( abs(dx) > 10 || abs(dy) > 10 )
	{	return true; }
	return false;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::RedrawWnd()
{
	CClientDC dc(this);
	HDC hDC = dc.GetSafeHdc();
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	HDC hMemDC2 = m_ImageWndMemDC2.GetSafeHdc();	
	if ( NULL==hMemDC || NULL==hMemDC2 || NULL==hDC ) 
	{	return; }

	CString str;
	CString str2;
	CString strOnline;
	size_t  SelectedComponentCount = 0;
	RECT    WndRect = m_ImageWndRect;	
	bool    MultiDistrictMode = false;	
	CAOIProject *ProjectPtr = GetActiveProject();		
	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	ONLINE_STATE_MODE OnlineStateGUI = AOIDataCollect.GetOnlineStateMode_GUI();

	if ( NULL != ProjectPtr )
	{	
		strOnline = AOIDataDefine.GetOnlineStateText(OnlineStateGUI);		
		MultiDistrictMode = ProjectPtr->GetProjectMultiDistrictMode();
		SelectedComponentCount = ProjectPtr->GetProjectComponentSelectedCount();	
	}
	if ( ONLINE_STATE_INSPECTION_STOP != OnlineStateGUI )
	{	
		HDC hMapDC = m_ImageWndMapDC.GetSafeHdc();
		if ( NULL != hMapDC )
		{	::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMapDC, 0, 0, SRCCOPY ); }
		::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	
		AOIDataCollect.DrawEditViewInspection(hMemDC2, WndRect, OnlineStateGUI, ProjectPtr, m_MapZoom);	
	}
	else
	{
		int TextPosX=8;
		int TextPosY=8;
		const int FontH = 16;
		::BitBlt(hMemDC2, 0, 0, WndRect.right, WndRect.bottom, hMemDC, 0, 0, SRCCOPY );
		::IntersectClipRect(hMemDC2, WndRect.left, WndRect.top, WndRect.right, WndRect.bottom);	

		::SetTextColor(hMemDC2, 0x00FF00);
		switch ( ManiMode )
		{
		case MANIPULATE_MODEL_ADD:
		case MANIPULATE_MODEL_AUTO_ADD:
			str = _T("Add Mode");
			str.Format(_T("Add Mode [%d] [%s]"), SelectedComponentCount, strOnline);			
			break;
		case MANIPULATE_MODEL_EDIT:
			str = _T("Edit Mode");
			str.Format(_T("Edit Mode [%d] [%s]"), SelectedComponentCount, strOnline);			
			break;
		case MANIPULATE_MODEL_SELECT:
			str = _T("Select Mode");
			str.Format(_T("Select Mode [%d] [%s]"), SelectedComponentCount, strOnline);			
			break;
		case MANIPULATE_MODEL_GATHER_COLOR:
			str = _T("Gather Color");
			str.Format(_T("Gather Color [%d] [%s]"), SelectedComponentCount, strOnline);			
			break;
		default:
			str = _T("");
			break;
		}	
		if ( str.GetLength() > 0 )
		{			
			if ( true == MultiDistrictMode )
			{
				str2 = str;				
				DISTRICT_ID DistrictID = ProjectPtr->GetProjectActDistrictID();
				CString strDistrictID = AOIDataDefine.GetDistrictIDText(DistrictID);
				str.Format(_T("[%s] %s"), strDistrictID, str2);	
			}
			::TextOut(hMemDC2, TextPosX, TextPosY, str, str.GetLength());
			TextPosY += FontH;
		}
		DrawBoxInfo(hMemDC2);
		DrawComponent(hMemDC2);		
		DrawModel(hMemDC2);	
		DrawObjectList(hMemDC2);
		DrawAddRect(hMemDC2);
		DrawModelActivedLine(hMemDC2);	
		DrawTempModel(hMemDC2);
		//DrawObjectList(hMemDC2);
		DrawCrosshair(hMemDC2);
	}
	::BitBlt(hDC, 0, 0, WndRect.right, WndRect.bottom, hMemDC2, 0, 0, SRCCOPY );	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CreateBKImage()
{
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::CreateBKImage Start"));	
	HDC hMemDC = m_ImageWndMemDC.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush); 
		::DeleteObject(hBrush);	hBrush = NULL;	
	}	
	DrawImage(hMemDC);
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::CreateBKImage End"));
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CreateMapImage(bool bTestMap)
{
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::CreateMapImage Start"));	
	HDC hMemDC = m_ImageWndMapDC.GetSafeHdc();
	if ( NULL == hMemDC ) { return; }	
	HBRUSH hBrush = ::CreateSolidBrush(m_BkColor);
	if ( NULL != hBrush )
	{
		::FillRect(hMemDC, &m_ImageWndRect, hBrush); 
		::DeleteObject(hBrush);	hBrush = NULL;	
	}	
	DrawProjectMap(hMemDC, bTestMap);
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::CreateMapImage End"));	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CreateTestMapImage()
{
	CAOIProject *ProjectPtr=GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	DWORD TestMapTickCount=ProjectPtr->GetProjectTestMapTickCount(0);
	if ( TestMapTickCount < m_UpdateTestMapTickCount ) { return; }
	m_UpdateTestMapTickCount=GetTickCount();
	CreateMapImage(true);
	m_UpdateTestMapTickCount=GetTickCount();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawModel(HDC hDC)
{	
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	const bool bDrawTempModel = AOIDataCollect.GetShowModelPreViewMode();
	if ( true == bDrawTempModel ) { return ; }

	TMODEL_DRAW_PARAM DrawParam;
	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const BOOL bShowFocusLine = AOIDataCollect.GetDrawModelActivedLine();
	if ( FALSE == bShowFocusLine ) { return; }
	
	ModelPtr->GetModelAttachedPosStage(ComponentStagePos);

	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;	
	const double StageOffsetX = (StageCpx-ComponentStagePos.x);	
	const double StageOffsetY = (StageCpy-ComponentStagePos.y);

	//Cad座標與影像座標為固定方位, 因此先將機台偏差改成Cad偏差, 再來處理
	StageOffset.x = StageOffsetX;
	StageOffset.y = StageOffsetY;
	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);

	const double ImageOffsetX =  CadOffset.x/ImageRes.x;
	const double ImageOffsetY = -CadOffset.y/ImageRes.y;
	const double ViewOffsetX = ImageOffsetX/m_ImageZoom;
	const double ViewOffsetY = ImageOffsetY/m_ImageZoom;

	GetModelDrawParam(DrawParam);
	DrawParam.ShowEditLine = true;
	DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
	DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);

	ModelPtr->DrawModel(hDC, DrawModelMode, DrawParam);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawImage(HDC hDC)
{	
	if ( NULL == m_ShowImagePtr ) { return; }
	const int BltMode = AOIDataCollect.GetStretchBltMode(m_ImageZoom);
	if ( ImageAPI.DrawImageToDC(hDC, m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ImageWndRect, m_ImageOffset, m_ImageZoom, 0x00000, BltMode) == false )
	{	return ; }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawProjectMap(HDC hDC, bool bTestMap)
{
	if ( NULL == hDC ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TPOINT2D    Offset;
	bool        bForce = false;
	double      ImageZoom = 1.0;
	IMAGE_SIZE  MapW = 0;
	IMAGE_SIZE  MapH = 0;
	IMAGE_SIZE  MapStep = 0;
	IMAGE_SIZE  BitCount = 0;
	IMAGE_PTR   MapPtr = NULL;
	RECT        WndRect = m_ImageWndRect;
	const int MapIndex = m_ImageIndex;//ProjectPtr->GetProjectMapIndex();	
	if ( true == bTestMap ) { bForce = true; }
	if ( ProjectPtr->GetProjectMapPtr(MapIndex, MapW, MapH, MapStep, BitCount, MapPtr) == false )
	{	return ; }

	ProjectPtr->CreateProjectMapShowPtr(MapIndex, MapPtr, bForce, bTestMap);
	ImageAPI.CalcImageWndFitZoom(MapW, MapH, WndRect, 1.0, ImageZoom);//計算影像視窗縮放參數	
	ImageAPI.DrawImageToDC(hDC, MapW, MapH, MapStep, BitCount, MapPtr, WndRect, Offset, ImageZoom, m_BkColor);
	m_MapZoom = ImageZoom;
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawAddRect(HDC hDC)
{
	if ( false == m_DrawAddRect ) { return; }
	if ( CURSOR_POS_NONE != m_MousePosMode ) { return; }
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return; }

	HPEN hPen    = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);
	if ( false == IsExceptionAngle )
	{
		::MoveToEx(hDC, pt1.x, pt1.y, NULL);
		::LineTo(hDC, pt2.x, pt1.y);
		::LineTo(hDC, pt2.x, pt2.y);
		::LineTo(hDC, pt1.x, pt2.y);
		::LineTo(hDC, pt1.x, pt1.y);
	}
	else
	{		
		TPOINT2D Cp, dPt1, dPt2;		
		TPOINT2D CornerPoint[4];
		const double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);
		dPt1 = pt1;
		dPt2 = pt2;		
		Cp.x = (dPt1.x+dPt2.x)*0.5;
		Cp.y = (dPt1.y+dPt2.y)*0.5;		
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt1);
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt2);
		CornerPoint[0].x = dPt1.x;	CornerPoint[0].y = dPt1.y;
		CornerPoint[1].x = dPt2.x;	CornerPoint[1].y = dPt1.y;
		CornerPoint[2].x = dPt2.x;	CornerPoint[2].y = dPt2.y;
		CornerPoint[3].x = dPt1.x;	CornerPoint[3].y = dPt2.y;		
		JetAPI::RotateCornerPos(ImageAngle, Cp.x, Cp.y, CornerPoint);
		ImageAPI.DrawPolyLine(hDC, CornerPoint, 4);
	}

	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen);	hPen = NULL;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawBoxInfo(HDC hDC)
{
	size_t      i = 0;		
	TActiveObj  *ObjPtr = NULL;			
	const size_t NObjects = this->m_ActiveObjList.size();	
	CString  str, str2, strPixel;
	//Draw Curpos	
	int      IR=0, IG=0, IB=0, IV=0;
	TSIZE2D  StageSize;
	TPOINT2D WndPt = m_MousePosImageWnd;
	TPOINT2D ImagePt, StagePt;
	TPOINT2D ImagePt1, StagePt1;
	TPOINT2D ImagePt2, StagePt2;	

	int SpaceIndex = -1;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{
		SpaceIndex = ProjectPtr->GetProjectMapIndex3D();
		m_ImageIndex = ProjectPtr->GetProjectMapIndex(); 
	}
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false )
	{	return;		}

	IMAGE_SIZE SpaceW = 0;
	IMAGE_SIZE SpaceH = 0;
	IMAGE_SIZE SpaceStep = 0;
	IMAGE_SIZE SpaceBitCount = 0;
	IMAGE_PTR  SpaceImagePtr = NULL;
	MASK_PTR   SpaceMaskPtr=NULL;
	SPACE_PTR  SpaceSpacePtr=NULL;
	if ( SpaceIndex != m_ImageIndex )
	{	GetFrameImage(SpaceIndex, SpaceW, SpaceH, SpaceStep, SpaceBitCount, SpaceImagePtr, SpaceSpacePtr, SpaceMaskPtr);	}

	const int nImageW = (int)(ImageW);
	const int nImageH = (int)(ImageH);
	const int nImageStep = (int)(ImageStep);
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_MousePosFirst, ImagePt1);	
	MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, m_MousePosLast, ImagePt2);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt, StageCp, StagePt);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StageCp, StagePt1);	
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StageCp, StagePt2);
	StageSize.cx = fabs(StagePt1.x-StagePt2.x);
	StageSize.cy = fabs(StagePt1.y-StagePt2.y);
	str.Format(_T("First(%d, %d), Last(%d, %d), WndPos(%.0f, %.0f), ImagePos(%.0f, %.0f), StageSize(%.0f, %.0f)"), m_MousePosFirst.x, m_MousePosFirst.y, m_MousePosLast.x, m_MousePosLast.y, WndPt.x, WndPt.y, ImagePt.x, ImagePt.y, StageSize.cx, StageSize.cy);

	//Draw Color Value
	if ( ImagePtr==NULL || ImagePt.x<0 || ImagePt.y<0 || ImagePt.x>=nImageW || ImagePt.y>=nImageH )
	{	strPixel = _T("");	}
	else
	{
		int index = 0;
		double    SpaceHeight=0;
		const int nImageX=JetAPI::Floor(ImagePt.x);
		const int nImageY=JetAPI::Floor(ImagePt.y);		
		const double BaseHeight = AOIDataCollect.GetSpaceBaseHeight();
		if ( NULL != SpaceSpacePtr )
		{
			index = (nImageY*SpaceStep)+nImageX;
			SpaceHeight = SpaceSpacePtr[index]-BaseHeight;
		}
		switch ( BitCount )
		{
		case 8:	
			index = (nImageY*nImageStep)+nImageX;
			if ( NULL == SpacePtr )
			{	
				if ( NULL == SpaceSpacePtr )
				{	strPixel.Format(_T("Gray=(%d)"), ImagePtr[index]); }
				else
				{	strPixel.Format(_T("Gray=(%d), Height=%.0fum"), ImagePtr[index], SpaceHeight);	}
			}
			else
			{	strPixel.Format(_T("Height=%.0fum"), SpacePtr[index]-BaseHeight);	}
			break;
		case 24:
			index = (nImageY*nImageStep)+(nImageX*3);
			ImageAPI.RGBConvertToRGBV(ImagePtr[index+2], ImagePtr[index+1], ImagePtr[index], IR, IG, IB, IV);
			if ( NULL == SpaceSpacePtr )
			{	strPixel.Format(_T("RGB=(%d, %d, %d), RGBV=(%d, %d, %d, %d)"), ImagePtr[index+2], ImagePtr[index+1], ImagePtr[index], IR, IG, IB, IV);	}
			else
			{	strPixel.Format(_T("RGB=(%d, %d, %d), RGBV=(%d, %d, %d, %d), Height=%.0fum"), ImagePtr[index+2], ImagePtr[index+1], ImagePtr[index], IR, IG, IB, IV, SpaceHeight);	}
			break;
		default:			
			break;
		}		
		//str = str+CString(_T(", "))+strPixel;
	}	
#ifdef _DEBUG
	::TextOut(hDC, 8, 28, str, str.GetLength());	
#endif//_DEBUG

	str.Format(_T("Roi Size(%.0f, %.0f)"), StageSize.cx, StageSize.cy);
	::TextOut(hDC, m_ImageWndRect.right-164, m_ImageWndRect.bottom-24, str, str.GetLength());

	if ( strPixel.GetLength() > 0 ) 
	{	::TextOut(hDC, 8, m_ImageWndRect.bottom-24, strPixel, strPixel.GetLength());	}
	
	if ( NULL != ProjectPtr )
	{		
		str = ProjectPtr->GetProjectMapIndexName();
		//::TextOut(hDC, m_ImageWndRect.right-96, m_ImageWndRect.bottom-24, str, str.GetLength());
		::TextOut(hDC, m_ImageWndRect.right-96, 8, str, str.GetLength());		
	}
	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }
		if ( false == ObjPtr->GetFocused() ) { continue; }
		break;
	}
	if ( NObjects == i ) { return; }

	CAOIBox *BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
	if ( NULL == BoxPtr ) { return; }

	TPOINT2D Pos;
	TSIZE2D  Size;
	TPOINT2D CornerPos[4];
	BoxPtr->GetBoxPos(Pos);
	BoxPtr->GetBoxSize(Size);
	BoxPtr->GetBoxCornerPosStage(CornerPos);

	::SetTextColor(hDC, 0x8FFFFF);
#ifdef _DEBUG
	str.Format(_T("Pos(%.0f, %.0f), Size(%.0f, %.0f), Stage( [%.0f, %.0f], [%.0f, %.0f], [%.0f, %.0f], [%.0f, %.0f] )"), Pos.x, Pos.y, Size.cx, Size.cy, CornerPos[0].x, CornerPos[0].y, CornerPos[1].x, CornerPos[1].y, CornerPos[2].x, CornerPos[2].y, CornerPos[3].x, CornerPos[3].y);
	::TextOut(hDC, 8, 48, str, str.GetLength());
#endif//_DEBUG		
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawComponent(HDC hDC)
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return; }	

	CString      str;
	size_t       i = 0;		
	POINT        Pt={0}, CornerPos[4];
	RECT         Rect={0};
	bool         ComponentSelected = false;
	const RECT   WndRect = m_ImageWndRect;
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];	
	TPOINT2D     ImageStagePos;
	TSIZE2D      FdPatExtend, FdRoiExtend;	
	TREGION4D    ObjStageRgn, ObjImageRgn;
	BOOL         ShowComponentName = TRUE;
	CAOIFd      *FdPtr = NULL;	
	CAOIModel   *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = NULL;		
	TMODEL_DRAW_PARAM DrawParam;
	TPOINT2D ComponentStagePos;
	TPOINT2D StageOffset, CadOffset;
	DISTRICT_ID  DistrictID = Project->GetProjectActDistrictID();
	
	const IMAGE_SIZE   ImageW = GetImageW();
	const IMAGE_SIZE   ImageH = GetImageH();
	const TPOINT2D     StageCp = GetImageStageRgnCp();
	const TPOINT2D    &ImageRes = GetImageResolution();
	const TREGION4D   &ImageStageRgn = GetImageStageRgn();
	
	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;	
	const BOOL bShowFocusLine = AOIDataCollect.GetDrawModelActivedLine();
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const DRAW_COMPONENT_MODE  DrawComponentMode = AOIDataCollect.GetDrawComponentMode();//顯示零件模式	
	const size_t FdCount = Project->GetProjectFdCount();	
	const size_t ComponentCount = Project->GetProjectComponentCount();
	const TSystemParameter &SystemParam = AOIDataCollect.GetSystemParameter();
	const COLORREF  clr1 = SystemParam.m_ComponentColor1;
	const COLORREF  clr2 = SystemParam.m_ComponentColor2;
	const COLORREF  clrOK = SystemParam.m_InspectedResultOKColor;
	const COLORREF  clrNG = SystemParam.m_InspectedResultNGColor;
	const COLORREF  clrText = SystemParam.m_ComponentTextColor;
	const COLORREF  clrSelected = SystemParam.m_ComponentSelectedColor;	

	HPEN hPen  = ::CreatePen(PS_SOLID, 1, clr1);	
	HPEN hPen2 = ::CreatePen(PS_SOLID, 1, clr2);
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	COLORREF clrTextOld = ::SetTextColor(hDC, clrText);
	int BKMode = ::SetBkMode(hDC, TRANSPARENT);		

	GetModelDrawParam(DrawParam);	
	DrawParam.ShowEditLine = true;

	ImageStagePos.x = ImageStageRgn.GetCpX();
	ImageStagePos.y = ImageStageRgn.GetCpY();
	for ( i=0; i<FdCount; i++ )
	{
		FdPtr = Project->GetProjectFdPtr(i, false);
		if ( NULL == FdPtr ) { continue; }
		if ( DistrictID != FdPtr->GetFdDistrictID() ) { continue; }
		if ( FdPtr->GetFdDeleted() == true ) { continue; }		

		StagePos.x = FdPtr->GetFdStagePosX();
		StagePos.y = FdPtr->GetFdStagePosY();
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }

		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }

		FdPtr->GetFdBodyStageCornerPos(StgCornerPos);		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
		{	continue; }		

		::SelectObject(hDC, hPen);	
		ImageAPI.DrawPolyLine(hDC, CornerPos, 4);
		str.Format(_T("Fd-%d"), FdPtr->GetFdIndex_Panel()+1);		
		::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());

		//Roi Extend Range;		
		FdPtr->GetFdRoiStageCornerPos(StgCornerPos);
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
		{	continue; }		
		::SelectObject(hDC, hPen2);	
		ImageAPI.DrawPolyLine(hDC, CornerPos, 4);
	}	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;		
	::DeleteObject(hPen2); hPen2 = NULL;
	
	hPen = ::CreatePen(PS_SOLID, 1, 0x00FFFF);	
	hOldPen = (HPEN)(::SelectObject(hDC, hPen));	
	for ( i=0; i<ComponentCount; i++ )
	{	
		ComponentPtr = Project->GetProjectComponentPtr(i, false);
		if ( NULL == ComponentPtr ) { continue; }
		if ( DistrictID != ComponentPtr->GetComponentDistrictID() ) { continue; }
		if ( ComponentPtr->GetComponentDeleted() == true ) { continue; }
		if ( ComponentPtr->CheckComponentIsAgent() == true ) { continue; }
		ComponentPtr = ComponentPtr->GetComponentResultPtr();

		if ( AOIDataCollect.CheckComponentTypeVisible(ComponentPtr->GetComponentType()) == false )
		{	continue; }
		ComponentSelected = ComponentPtr->GetComponentSelected();
	//	if ( ComponentPtr->GetComponentSelected() == false ) { continue; }
		if ( DRAW_COMPONENT_FOCUSED == DrawComponentMode )
		{
			if ( FALSE == bShowFocusLine ) { continue; }
			if ( false == ComponentSelected ) { continue; }
		}		
		if ( true==ComponentSelected && DRAW_MODEL_RESULT==DrawModelMode )
		{
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			if ( ModelPtr == m_ModelPtr )
			{	continue;	}			
		}		
		StagePos.x = ComponentPtr->GetComponentStagePosX();
		StagePos.y = ComponentPtr->GetComponentStagePosY();
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{
			StageOffset.x = ComponentPtr->GetComponentStageOffsetX();
			StageOffset.y = ComponentPtr->GetComponentStageOffsetY();
			StagePos.x += StageOffset.x;
			StagePos.y += StageOffset.y;
		}		
		if ( StagePos.x<ImageStageRgn.minX || StagePos.x>ImageStageRgn.maxX ) { continue; }
		if ( StagePos.y<ImageStageRgn.minY || StagePos.y>ImageStageRgn.maxY ) { continue; }
		AOIDataCollect.MapStagePtToCamera(ImageW, ImageH, ImageRes, StagePos, ImageStagePos, ImagePos);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImagePos, WndPt);

		JetAPI::Point2DToPoint(WndPt, Pt);
		if ( Pt.x<0 || Pt.y<0 || Pt.x>WndRect.right || Pt.y>WndRect.bottom ) { continue; }
		
		ComponentPtr->GetComponentBodyStageCornerPos(StgCornerPos);				
		//ComponentPtr->GetComponentRoiStageCornerPos(StgCornerPos);
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{
			StgCornerPos[0].x += StageOffset.x;	StgCornerPos[0].y += StageOffset.y;
			StgCornerPos[1].x += StageOffset.x;	StgCornerPos[1].y += StageOffset.y;
			StgCornerPos[2].x += StageOffset.x;	StgCornerPos[2].y += StageOffset.y;
			StgCornerPos[3].x += StageOffset.x;	StgCornerPos[3].y += StageOffset.y;
		}
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, ImageStagePos, ImgCornerPos);		
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, m_ImageOffset, m_ImageZoom, ImgCornerPos[3], WndCornerPos[3]);

		JetAPI::CornerPt2DToCornerPt(WndCornerPos, CornerPos);		
		if ( JetAPI::CheckCornerInRect(CornerPos, WndRect) == false ) 
		{	continue; }		
		ImageAPI.DrawPolyLine(hDC, CornerPos, 4);

		if ( ShowComponentName )
		{
			str = ComponentPtr->GetComponentName();
			//::TextOut(hDC, Rect.left, Rect.top, str, str.GetLength());
			::TextOut(hDC, Pt.x, Pt.y, str, str.GetLength());
		}
		if ( false == ComponentSelected )
		{
			ModelPtr = ComponentPtr->GetComponentModelPtr();
			if ( NULL != ModelPtr )
			{				
				ModelPtr->GetModelAttachedPosStage(ComponentStagePos);
				const double StageOffsetX = (StageCpx-ComponentStagePos.x);	
				const double StageOffsetY = (StageCpy-ComponentStagePos.y);

				//Cad座標與影像座標為固定方位, 因此先將機台偏差改成Cad偏差, 再來處理
				StageOffset.x = StageOffsetX;
				StageOffset.y = StageOffsetY;
				AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);

				const double ImageOffsetX =  CadOffset.x/ImageRes.x;
				const double ImageOffsetY = -CadOffset.y/ImageRes.y;
				const double ViewOffsetX = ImageOffsetX/m_ImageZoom;
				const double ViewOffsetY = ImageOffsetY/m_ImageZoom;
				DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
				DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);
				
				DrawParam.ShowWndBox = false;
				DrawParam.ShowEditLine = false;
				ModelPtr->DrawModel(hDC, DrawModelMode, DrawParam);
			}
		}		
	}
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;	
	
	::SetBkMode(hDC, BKMode);
	::SetTextColor(hDC, clrTextOld);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawTempModel(HDC hDC)
{
	const bool bDrawTempModel = AOIDataCollect.GetShowModelPreViewMode();
	if ( false == bDrawTempModel ) { return ; }
	CAOIModel *TempModelPtr = AOIDataCollect.GetModelPreViewPtr();
	if ( NULL == TempModelPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }

	CAOIModel *AttachModelPtr = TempModelPtr->CloneModelObj();
	if ( NULL == AttachModelPtr ) { return; }
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();

	TPOINT2D     CadOffset;
	TPOINT2D     StageOffset;	
	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;
	const double ComponentAngle = ComponentPtr->GetComponentAngle();
	const double ComponentCadPosX = ComponentPtr->GetComponentCadPosX();
	const double ComponentCadPosY = ComponentPtr->GetComponentCadPosY();
	const double ComponentStagePosX = ComponentPtr->GetComponentStagePosX();
	const double ComponentStagePosY = ComponentPtr->GetComponentStagePosY();

	AttachModelPtr->RotateModel(ComponentAngle, 0, 0);

	AttachModelPtr->SetModelAttachedAngle(ComponentAngle);
	AttachModelPtr->SetModelAttachedPosCad(ComponentCadPosX, ComponentCadPosY);
	AttachModelPtr->SetModelAttachedPosStage(ComponentStagePosX, ComponentStagePosY);
	
	const double StageOffsetX = (StageCpx-ComponentStagePosX);	
	const double StageOffsetY = (StageCpy-ComponentStagePosY);

	//Cad座標與影像座標為固定方位, 因此先將機台偏差改成Cad偏差, 再來處理
	StageOffset.x = StageOffsetX;
	StageOffset.y = StageOffsetY;
	AOIDataCollect.MapStageOffsetPtToCad(StageOffset, CadOffset);

	const double ImageOffsetX =  CadOffset.x/ImageRes.x;
	const double ImageOffsetY = -CadOffset.y/ImageRes.y;
	const double ViewOffsetX = ImageOffsetX/m_ImageZoom;
	const double ViewOffsetY = ImageOffsetY/m_ImageZoom;

	TMODEL_DRAW_PARAM DrawParam;
	GetModelDrawParam(DrawParam);				
	DrawParam.ShowWndBox = false;
	DrawParam.ShowEditLine = false;
	DrawParam.ViewCP.x = -JetAPI::Floor(ViewOffsetX);
	DrawParam.ViewCP.y = -JetAPI::Floor(ViewOffsetY);
	AttachModelPtr->DrawModel(hDC, DRAW_MODEL_EDIT, DrawParam);

	AOIObjManager.DestroyModelObj(AttachModelPtr);
	AttachModelPtr = NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawCrosshair(HDC hDC)//十字線
{
	const POINT &Pt=m_MousePosCurrent;	
	const RECT &WndRect=m_ImageWndRect;
	MANIPULATE_MODEL_MODE ManiMode=AOIDataCollect.GetManipulateModelMode();
	if ( MANIPULATE_MODEL_ADD != ManiMode && MANIPULATE_MODEL_AUTO_ADD != ManiMode) { return ; }
	ImageAPI.DrawCrosshair(hDC, Pt, WndRect, 0xA0A0A0);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawObjectList(HDC hDC)
{
	return ;
	size_t      i = 0;		
	TActiveObj  *ObjPtr = NULL;
	RECT         nRect={0};
	RECT         nRect2={0};
	RECT         nRectI={0};
	RECT         nRectO={0};
	SIZE         szGrid={0};
	double       ImageZoom = m_ImageZoom;
	TRECT4D      dImageRect, dWndRect;
	TPOINT2D     ImageCornerPts[4], WndCornerPts[4];
	TPOINT2D     ImageOffset=m_ImageOffset;
	bool         bFocused = true;
	bool         bShowGridLine=true;
	const size_t NObjects = this->m_ActiveObjList.size();
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	

	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();
	szGrid.cx = (int)(szGrid.cx/m_ImageZoom);
	szGrid.cy = (int)(szGrid.cy/m_ImageZoom);
	if ( szGrid.cx < 1 ) { szGrid.cx = 1; }
	if ( szGrid.cy < 1 ) { szGrid.cy = 1; }

	HPEN hPen    = ::CreatePen(PS_SOLID, 1, 0x00FF00);
	HPEN hPenSel = ::CreatePen(PS_SOLID, 1, 0x0000FF);
	HPEN hPenGrid = ::CreatePen(PS_DOT, 1, 0xFFFFFF);
	HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);

	ImageOffset.x =  m_ImageOffset.x;
	ImageOffset.y =  m_ImageOffset.y;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }

		bFocused = ObjPtr->GetFocused();
		if ( true == bFocused )
		{	::SelectObject(hDC, hPenSel);	}
		else
		{	::SelectObject(hDC, hPen);	}
		
		if ( false == ObjPtr->IsExceptionAngle )
		{
			dImageRect   = ObjPtr->Rect;
			//dImageRect.top    = m_ImageH-ObjPtr->Rect.bottom;
			//dImageRect.bottom = m_ImageH-ObjPtr->Rect.top;
			ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, dImageRect, dWndRect);
			JetAPI::Rect4DToRect(dWndRect, nRect);		
			ImageAPI.DrawRectLine(hDC, nRect);

			if ( true==bShowGridLine && true==bFocused )
			{
				nRectO = nRectI = nRect;				
				::InflateRect(&nRectI, -szGrid.cx, -szGrid.cy);
				::InflateRect(&nRectO, szGrid.cx, szGrid.cy);

				::SelectObject(hDC, hPenGrid);

				//Left Top
				nRect2.left= nRectO.left;
				nRect2.top = nRectO.top;
				nRect2.right= nRectI.left;
				nRect2.bottom= nRectI.top;
				ImageAPI.DrawRectLine(hDC, nRect2);
				
				//Right Top
				nRect2.left= nRectI.right;
				nRect2.top = nRectO.top;
				nRect2.right= nRectO.right;
				nRect2.bottom= nRectI.top;
				ImageAPI.DrawRectLine(hDC, nRect2);

				//Left Bottom
				nRect2.left= nRectO.left;
				nRect2.top = nRectI.bottom;
				nRect2.right= nRectI.left;
				nRect2.bottom= nRectO.bottom;
				ImageAPI.DrawRectLine(hDC, nRect2);

				//Right Bottom
				nRect2.left= nRectI.right;
				nRect2.top = nRectI.bottom;
				nRect2.right= nRectO.right;
				nRect2.bottom= nRectO.bottom;
				ImageAPI.DrawRectLine(hDC, nRect2);
			}
		}
		else
		{
			ImageCornerPts[0] = ObjPtr->CornerPts[0];
			ImageCornerPts[1] = ObjPtr->CornerPts[1];
			ImageCornerPts[2] = ObjPtr->CornerPts[2];
			ImageCornerPts[3] = ObjPtr->CornerPts[3];
			
			//ImageCornerPts[0].y = m_ImageH-ObjPtr->CornerPts[0].y;
			//ImageCornerPts[1].y = m_ImageH-ObjPtr->CornerPts[1].y;
			//ImageCornerPts[2].y = m_ImageH-ObjPtr->CornerPts[2].y;
			//ImageCornerPts[3].y = m_ImageH-ObjPtr->CornerPts[3].y;
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[0], WndCornerPts[0]);
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[1], WndCornerPts[1]);
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[2], WndCornerPts[2]);
			ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImageCornerPts[3], WndCornerPts[3]);
			ImageAPI.DrawPolyLine(hDC, WndCornerPts, 4);
		}
	}
	
	::SelectObject(hDC, hOldPen);
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPenSel); hPenSel = NULL;
	::DeleteObject(hPenGrid); hPenGrid = NULL;
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::DrawModelActivedLine(HDC hDC)//繪製選取交線
{
	BOOL bShowFocusLine = AOIDataCollect.GetDrawModelActivedLine();
	//if ( FALSE == bShowFocusLine ) { return ; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }
	if ( ComponentPtr->GetComponentDeleted() == true ) { return; }
	CAOIModel   *ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }	

	CString      str;
	size_t       i = 0;		
	POINT        Pt={0}, CornerPos[4];
	RECT         CornerRect={0};
	TRECT4D      ImgCornerRect4d;
	TRECT4D      WndCornerRect4d;
	bool         ComponentSelected = false;
	const RECT   WndRect = m_ImageWndRect;
	TPOINT2D     StagePos, ImagePos, WndPt;
	TPOINT2D     StgCornerPos[4], ImgCornerPos[4], WndCornerPos[4];			
	double       ImageZoom = m_ImageZoom;	
	
	const IMAGE_SIZE   ImageW = GetImageW();
	const IMAGE_SIZE   ImageH = GetImageH();	
	const TPOINT2D     StageCp = GetImageStageRgnCp();
	const TPOINT2D    &ImageRes = GetImageResolution();
	const TPOINT2D    &ImageOffset = m_ImageOffset;
	const TREGION4D   &ImageStageRgn = GetImageStageRgn();	
	
	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;	
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const double ComponentAngle = ComponentPtr->GetComponentAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	CAOIWnd  *WndPtr  = ModelPtr->GetModelWndActived();
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();

	CAOIBox  *BoxPtr = NULL;
	if ( FALSE == bShowFocusLine )
	{	ModelPtr->GetModelTotalCornerPtsStage(StgCornerPos);	}
	else
	{
		if ( NULL != WndPtr )
		{	BoxPtr = WndPtr->GetWndBoxPtr(); }
		else if ( NULL != LandPtr )
		{	BoxPtr = LandPtr->GetLandBoxPtr();	}
		else
		{	BoxPtr = ModelPtr->GetModelBodyBoxPtr(); }
		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{	BoxPtr->GetBoxCornerPosStageRes(StgCornerPos);	}
		else
		{	BoxPtr->GetBoxCornerPosStage(StgCornerPos); }
	}
	AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, StgCornerPos, StageCp, ImgCornerPos);//機台4端點對應到影像四點
	JetAPI::PointsToRect(ImgCornerPos, 4, ImgCornerRect4d);
	
	HPEN hPen  = ::CreatePen(PS_SOLID, 3, 0x0000FF);	//PS_DASH
	HPEN hPenW = ::CreatePen(PS_SOLID, 5, 0xFFFFFF);	//PS_DASH
	HPEN hOldPen = (HPEN)(::SelectObject(hDC, hPen));

	if ( false == IsExceptionAngle )
	{	
		ImageAPI.MapImageRectToWndRect_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerRect4d, WndCornerRect4d);
		JetAPI::Rect4DToRect(WndCornerRect4d, CornerRect);
	}
	else
	{	
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[0], WndCornerPos[0]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[1], WndCornerPos[1]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[2], WndCornerPos[2]);
		ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, m_ImageWndRect, ImageOffset, ImageZoom, ImgCornerPos[3], WndCornerPos[3]);
		JetAPI::PointsToRect(WndCornerPos, 4, WndCornerRect4d);
		JetAPI::Rect4DToRect(WndCornerRect4d, CornerRect);
		ImageAPI.DrawRectLine(hDC, CornerRect);//斜角度補上外框
	}	
	const int nCornerCpX = (CornerRect.left+CornerRect.right)/2;
	const int nCornerCpY = (CornerRect.top+CornerRect.bottom)/2;	

	::SelectObject(hDC, hPenW);
	if ( FALSE == bShowFocusLine )
	{	ImageAPI.DrawRectLine(hDC, CornerRect);	}
	::MoveToEx(hDC, WndRect.left, nCornerCpY, NULL);
	::LineTo(hDC, CornerRect.left, nCornerCpY);
	::MoveToEx(hDC, WndRect.right, nCornerCpY, NULL);
	::LineTo(hDC, CornerRect.right, nCornerCpY);

	::MoveToEx(hDC, nCornerCpX, WndRect.top, NULL);
	::LineTo(hDC, nCornerCpX, CornerRect.top);
	::MoveToEx(hDC, nCornerCpX, WndRect.bottom, NULL);
	::LineTo(hDC, nCornerCpX, CornerRect.bottom);

	::SelectObject(hDC, hPen);
	if ( FALSE == bShowFocusLine )
	{	ImageAPI.DrawRectLine(hDC, CornerRect);	}
	::MoveToEx(hDC, WndRect.left, nCornerCpY, NULL);
	::LineTo(hDC, CornerRect.left, nCornerCpY);
	::MoveToEx(hDC, WndRect.right, nCornerCpY, NULL);
	::LineTo(hDC, CornerRect.right, nCornerCpY);

	::MoveToEx(hDC, nCornerCpX, WndRect.top, NULL);
	::LineTo(hDC, nCornerCpX, CornerRect.top);
	::MoveToEx(hDC, nCornerCpX, WndRect.bottom, NULL);
	::LineTo(hDC, nCornerCpX, CornerRect.bottom);	

	::SelectObject(hDC, hOldPen);	
	::DeleteObject(hPen); hPen = NULL;
	::DeleteObject(hPenW); hPenW = NULL;	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::PostMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.PostMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SendMessageToMainFrameWnd(UINT message, WPARAM wParam, LPARAM lParam)
{
	AOIDataCollect.SendMainFrameWndMessage(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CloseProject()
{
	ResetModel();
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
	m_ProjectPtr = NULL;
	m_MapZoom = 1.00;
	m_ImageZoom = 1.00;		
	ResetImageOffset();	
	m_FrameResolution.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	m_FrameResolution.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	CreateBKImage();
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
inline CAOIProject* CEditModelView::GetActiveProject()
{
	return m_ProjectPtr;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SwitchProject()
{	
	CloseProject();
	CAOIProject *ProjectPtr = AOIDataCollect.GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}	
	m_ProjectPtr = ProjectPtr;	
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SetProjectActiveFdIndex(-1);
	ProjectPtr->SetProjectActiveBarcodeIndex(-1);
	m_ImageIndex = ProjectPtr->GetProjectMapIndex();
	CreateMapImage();

	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();
	if ( NULL != pComponent )
	{
		pComponent->SetComponentSelected(true);
		CAOIModel *ModelPtr = pComponent->GetComponentModelPtr();
		if ( NULL != ModelPtr )
		{	ModelPtr->SetModelBodyBoxActived(true);	}
	}
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SwitchFrameImage()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	int i=0;
	int MaxFrameCount = GetMaxFrameCount();
	const int MaxFrames = GetMaxFrameCount();
	DRAW_IMAGE_MODE DrawingImageMode = AOIDataCollect.GetDrawingImageMode();
	if ( DRAW_IAMGE_BY_ALG != DrawingImageMode )	
	{	m_ImageIndex = ProjectPtr->GetProjectMapIndexNext(m_ImageIndex);	}
	ProjectPtr->SetProjectMapIndex(m_ImageIndex);
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	//PostMessageToMainFrameWnd(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::UpdateFrameImage()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	m_ImageIndex = ProjectPtr->GetProjectMapIndex();	
	if ( false == OfflineMode )
	{
		double PosX=0, PosY=0, PosZ=0;			
		if ( AOIDataCollect.GetStagePos(PosX, PosY, PosZ) == false )
		{
			JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
			return;
		}
		SetKeepImageOffset(true);
		ExecGrabFov(PosX, PosY, PosZ);
		return ;
	}
	FillCurrentFrames(m_FovRatio);
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	//PostMessageToMainFrameWnd(MSG_EDIT_VIEW_3D_WND, WPARAM_UPDATE_3D_DATA, NULL);
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SwitchMultiLanguage()
{
	/*
	size_t  i = 0;	
	UINT    WndID = 0;
	CString WndKey;
	CWnd   *pWnd = NULL;
	CString LabelText, NewLabelText;
	CString Section=_T("IDD_EDIT_MODEL_VIEW");	
	//---------------------------------------------------------------------------------//
	WndID = IDD_EDIT_MODEL_VIEW;
	WndKey = _T("IDD_EDIT_MODEL_VIEW");
	this->GetWindowText(LabelText);
	AOIDataCollect.GetUILanguageString(Section, WndKey, LabelText, NewLabelText);	
	this->SetWindowText(NewLabelText);
	//---------------------------------------------------------------------------------//	
	*/
}
//-------------------------------------------------------------------------------------//
CString CEditModelView::LoadMultiLanguageString(LPCTSTR KeyName, LPCTSTR Default)
{
	CString NewLabelText;
	LPCTSTR Section=_T("IDD_EDIT_MODEL_VIEW");	
	//---------------------------------------------------------------------------------//			
	AOIDataCollect.GetUILanguageString(Section, KeyName, Default, NewLabelText);	
	return NewLabelText;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelAddWnd()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->GetModelEditMode() == false ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false ) { return false; }
	
	TPOINT2D  ModelStageCp;
	TREGION4D RgnWndBox;
	TREGION4D RgnImgBox;
	TREGION4D RgnStgBox;
	TREGION4D RgnCadBox;
	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;
	CAOIWnd   *WndPtr = ModelPtr->GetModelWndActived();
	CAOILand  *LandPtr = ModelPtr->GetModelLandActivted();	
	MODEL_TYPE ModelType = ModelPtr->GetModelType();
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();
	const unsigned int FrameIndex = ProjectPtr->GetProjectMapIndex();
	const unsigned int FrameUniqueID = ProjectPtr->GetProjectFrameUniqueID(FrameIndex, true);
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);		

	unsigned int DefaultIndex = -1;
	unsigned int DefaultUniqueID = FRAME_UNIQUE_ID_DEFAULT;
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultIndex, DefaultUniqueID);	

	CDefectListWnd DefectListWnd;
	if ( NULL==LandPtr && NULL!=WndPtr ) 
	{	LandPtr = WndPtr->GetWndLandPtr(); }
	DefectListWnd.SetLandPtr(LandPtr);	
	if ( DefectListWnd.DoModal() == IDCANCEL ) 
	{	return false; }
	const WND_DEFECT_ID WndDefectID = DefectListWnd.GetDefectID();
	const ALG_TYPE      AlgType = DefectListWnd.GetAlgorithm();	
	const WND_FOLLOW_MODE WndFollowMode = AOIDataDefine.GetWndFollowModeByWndDefectID(WndDefectID);
	const bool bWndRoiDeleted = CAOIModel::CheckModelWndRoiCanBeDeleted(AlgType);
	int   DefaultWndRoiCount = CAOIModel::ObtainModelAlgDefaultWndRoiCount(AlgType);
	bool  bAutoAdd = CAOIModel::CheckModelWndRoiAutoAdd(AlgType, LandPtr);
	if ( DefaultWndRoiCount > 0 ) 
	{		
		if ( true == bAutoAdd )
		{	DefaultWndRoiCount = 0;	}
		else if ( true == bWndRoiDeleted )
		{
			CInputBoxWnd InputBox;
			CString      strCaption, strLabel, strValue;
			strCaption = _T("Input Wnd Roi Count");
			strCaption = LoadMultiLanguageString(strCaption, strCaption);
			strLabel = _T("Number:");
			strLabel = LoadMultiLanguageString(strLabel, strLabel);
			strValue.Format(_T("%d"), DefaultWndRoiCount); 
			InputBox.SetParam1(strCaption, strLabel, strValue);
			if ( InputBox.DoModal() == IDCANCEL ) 
			{	return false; }
			DefaultWndRoiCount =  ::_ttoi(InputBox.m_DataEdit1);
			if ( DefaultWndRoiCount < 0 ) { DefaultWndRoiCount = 0; }
		}
	}

	ModelPtr->GetModelAttachedPosStage(ModelStageCp);
	if ( false == IsExceptionAngle )
	{
		RgnWndBox.minX = MIN(pt1.x, pt2.x);
		RgnWndBox.maxX = MAX(pt1.x, pt2.x);
		RgnWndBox.minY = MIN(pt1.y, pt2.y);
		RgnWndBox.maxY = MAX(pt1.y, pt2.y);
		ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
		RgnStgBox.minX -= ModelStageCp.x;
		RgnStgBox.minY -= ModelStageCp.y;
		RgnStgBox.maxX -= ModelStageCp.x;
		RgnStgBox.maxY -= ModelStageCp.y;		
		AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);
	}
	else
	{
		TPOINT2D Cp, dPt1, dPt2;		
		TPOINT2D CornerPoint[4];
		const double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);
		dPt1 = pt1;
		dPt2 = pt2;		
		Cp.x = (dPt1.x+dPt2.x)*0.5;
		Cp.y = (dPt1.y+dPt2.y)*0.5;		
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt1);
		JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dPt2);		

		RgnWndBox.minX = MIN(dPt1.x, dPt2.x);
		RgnWndBox.maxX = MAX(dPt1.x, dPt2.x);
		RgnWndBox.minY = MIN(dPt1.y, dPt2.y);
		RgnWndBox.maxY = MAX(dPt1.y, dPt2.y);

		ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
		RgnStgBox.minX -= ModelStageCp.x;
		RgnStgBox.minY -= ModelStageCp.y;
		RgnStgBox.maxX -= ModelStageCp.x;
		RgnStgBox.maxY -= ModelStageCp.y;
		AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);
	}
	const double CadX = RgnCadBox.GetCpX();
	const double CadY = RgnCadBox.GetCpY();
	const double CadW = RgnCadBox.GetWidth();
	const double CadH = RgnCadBox.GetHeight();
	WndPtr = ModelPtr->CreateModelWnd(WndDefectID, RgnCadBox, LandPtr);
	if ( NULL == WndPtr )
	{
		JetAPI::ShowMessageBox(_T("Error, Create Model Wnd Ptr Fault"));
		return false;
	}	
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
	AlgParam.ChangeAlgType(AlgType, true);//順序不要顛倒
	WndPtr->SetWndAlgImageFrameIndex(FrameIndex);
	WndPtr->SetWndAlgImageFrameUniqueID(FrameUniqueID);
	WndPtr->UpdateWndFrameUniqueID(DefaultIndex, DefaultUniqueID, FrameIndexMapList);
	WndPtr->ChangeWndDefectID(ModelType, WndDefectID);
	WndPtr->SetWndFollowMode(WndFollowMode);
	WndPtr->UpdateWndExtendBox();
	WndPtr->BuildWndRoiWndListDefault(DefaultWndRoiCount);
	if (false == WndPtr->UpdateWndParamRoiDefault(DefaultWndRoiCount)) {
		CString ErrorStr = _T("Please enter a number between 2 to 5.");
		JetAPI::ShowMessageBox(ErrorStr);
		return false;
	}
	const bool WndRgnLinkAuto = WndPtr->GetWndRgnLinkAuto();

	ModelPtr->UnSelectModel();	
	if ( NULL != LandPtr )
	{
		const bool    bLinkMode = CheckIsLinkMode();	
		if ( false == bLinkMode )
		{	
			ModelPtr->AddModelWndPtr(WndPtr, false);
			LandPtr->AddLandWndPtr(WndPtr);
		}
		else
		{
			WndPtr->SetWndSelected(false);
			const size_t LandWndCount = LandPtr->GetLandWndCount();
			ModelPtr->AddModelWndToOtherLand(LandPtr, WndPtr, WndPtr->GetWndBandID()); 
			AOIObjManager.DestroyWndObj(WndPtr);
			WndPtr = LandPtr->GetLandWndPtr(LandWndCount, true);
			if ( NULL != WndPtr )
			{	WndPtr->SetWndSelected(true);	}
		}
	}	
	else
	{	ModelPtr->AddModelWndPtr(WndPtr, false);	}
	
	if ( true == WndRgnLinkAuto )
	{	ModelPtr->UpdateModelWndRgnByLinkMode();	}

	ModelPtr->SetModelWndActived(WndPtr);
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelBodyToComponent();

	WndPtr = ModelPtr->GetModelWndActived();	
	ProjectPtr->SetProjectActiveModelWnd(WndPtr);
	LogOperCtrl.SaveLogModelWndSelectedCreate(ModelPtr);	
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelModeDefault();
	SwitchManiModelMode(ManiMode);
	UpdateModelStats();
	RedrawWnd();	
	
	if ( NULL != WndPtr )
	{
		bAutoAdd = CAOIModel::CheckModelWndRoiAutoAdd(AlgType, LandPtr);
		if ( false == bAutoAdd )
		{	ExecModelWndInspection(false);}
		SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_NULL);//要先切換喔
		if ( true == bAutoAdd ) 
		{	SendMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_EXEC_WND_ROI_ADD_AUTO, (LPARAM)(WndPtr)); }		
	}	
	else
	{	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_NULL);	}	 
	//SendOutUpdatePartList(MSG_MODE_NONE, MSG_MODE_UPDATE);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelAddLand(LAND_TYPE LandType)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->GetModelEditMode() == false ) { return true; }
	if ( AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false ) { return false; }
	
	TPOINT2D  ModelStageCp;
	TREGION4D RgnWndBox;
	TREGION4D RgnImgBox;
	TREGION4D RgnStgBox;
	TREGION4D RgnCadBox;
	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;	
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D   StageCp = GetImageStageRgnCp();
	const TPOINT2D  &ImageRes = GetImageResolution();
	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	
	//CEditModelView轉成ImageWnd
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);	
	ModelPtr->GetModelAttachedPosStage(ModelStageCp);
	if ( false == IsExceptionAngle )
	{
		RgnWndBox.minX = MIN(pt1.x, pt2.x);
		RgnWndBox.maxX = MAX(pt1.x, pt2.x);
		RgnWndBox.minY = MIN(pt1.y, pt2.y);
		RgnWndBox.maxY = MAX(pt1.y, pt2.y);
		ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);	
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, StageCp, RgnStgBox);
		//相對於模組機台座標
		RgnStgBox.minX -= ModelStageCp.x;
		RgnStgBox.minY -= ModelStageCp.y;
		RgnStgBox.maxX -= ModelStageCp.x;
		RgnStgBox.maxY -= ModelStageCp.y;
		AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);
	}
	else
	{		
		TPOINT2D Cp, dPt1, dPt2;
		TPOINT2D WndPt1, WndPt2;
		TPOINT2D CadPt1, CadPt2;
		TPOINT2D ImagePt1, ImagePt2;
		TPOINT2D StagePt1, StagePt2;
		TREGION4D StageRgn;
		TPOINT2D CornerPoint[4];		
		const double ImageAngle = JetAPI::MapCadAngleToImageAngle(ComponentAngle);

		//視窗轉成影像
		WndPt1 = pt1;
		WndPt2 = pt2;
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);	
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);	

		//影像轉成機台
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, StageCp, StagePt1);
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, StageCp, StagePt2);

		//相對於模組機台座標		
		dPt1.x = StagePt1.x-ModelStageCp.x;
		dPt1.y = StagePt1.y-ModelStageCp.y;
		dPt2.x = StagePt2.x-ModelStageCp.x;
		dPt2.y = StagePt2.y-ModelStageCp.y;

		//機台轉成Cad座標		
		AOIDataCollect.MapStageOffsetPtToCad(dPt1, CadPt1);
		AOIDataCollect.MapStageOffsetPtToCad(dPt2, CadPt2);		
		Cp.x = 0;
		Cp.y = 0;
		JetAPI::RotatePos(-ComponentAngle, Cp.x, Cp.y, CadPt1);
		JetAPI::RotatePos(-ComponentAngle, Cp.x, Cp.y, CadPt2);
		JetAPI::PointsToRegion(CadPt1, CadPt2, RgnCadBox);
	}

	CString str;
	CString LandTypeTxt = AOIDataDefine.GetLandTypeText(LandType);
	CAOILand *LandPtr = ModelPtr->CreateModelLand(ModelType, RgnCadBox, LandType);
	if ( NULL == LandPtr )
	{
		str.Format(_T("Error, Create Model Land Ptr Fault [%s]"), LandTypeTxt);
		JetAPI::ShowMessageBox(str);
		return false;
	}

	ModelPtr->UnSelectModel();
	ModelPtr->AddModelLandPtr(LandPtr, false);
	ModelPtr->UpdateModelChipLeadRgnFromBody();
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelBodyToComponent();
	LogOperCtrl.SaveLogModelLandSelectedCreate(ModelPtr);	
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelModeDefault();
	SwitchManiModelMode(ManiMode);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelAutoAddLand(int direc)
{
	const char fnName[] = "CEditModelView::ExecModelAutoAddLand";
	CAOIModel *ModelPtr = GetModelPtr();
	if (NULL == ModelPtr) { return true; }
	if (ModelPtr->GetModelEditMode() == false) { return true; }
	if (AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false) { return false; }

	LAND_TYPE LandType;
	//if (ModelType == MODEL_TYPE_LEAD_COMPONENT || ModelType == MODEL_TYPE_TRANSISTOR || MODEL_TYPE_JLEAD_PLCC == ModelType)
	if (ModelPtr->GetModelType() == MODEL_TYPE_LEAD_COMPONENT)
	{
		LandType = LAND_TYPE_IC_LEAD;
	}
	else if (ModelPtr->GetModelType() == MODEL_TYPE_JLEAD_PLCC || ModelPtr->GetModelType() == MODEL_TYPE_TRANSISTOR)
	{
		LandType = LAND_TYPE_ELECTRODE;
	}

	ModelPtr->CalcModelTotalRegionAll();

	RECT roiRect;
	CreateModelROI(m_UniFrameList, m_FOVPosStage, m_FrameResolution, ModelPtr, roiRect);

	const int nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> UniFrameList;
	if (BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, true, bNoFilter) == false)
	{
		ModelPtr->AnalysisModelProperty(UniFrameList);
		JetAPI::ClearUniFrameList(UniFrameList);
	}
	//cv::Size sz((int)(m_UniFrameList[2].ImageW), (int)(m_UniFrameList[2].ImageH));
	//cv::Mat M(sz, CV_8UC3, m_UniFrameList[2].ImagePtr, m_UniFrameList[2].ImageStep * sizeof(uchar));

	CString str;
	TPOINT2D  ImageRes;
	TPOINT2D  ModelStageCp;
	TREGION4D RgnWndBox;
	TREGION4D RgnImgBox;
	TREGION4D RgnStgBox;
	TREGION4D RgnCadBox;
	POINT pt1 = m_MousePosLast;
	POINT pt2 = m_MousePosFirst;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();
	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	int angleindex = 0;//判斷箭頭方向與焊盤方向之index
	if (ComponentAngle == 0 || ComponentAngle == 180)
	{//引腳方向, 1 = 垂直, 2 = 水平。  箭頭水平+direc=2 會是水平，箭頭水平+direc=1 是垂直
		angleindex = direc;
	}
	else if (ComponentAngle == 90 || ComponentAngle == 270)
	{//箭頭垂直+direc=2 會是垂直，箭頭水平+direc=1 是水平
		if (direc == 1)
			angleindex = 2;
		else if (direc == 2)
			angleindex = 1;
	}

	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	ImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	//CEditModelView轉成ImageWnd
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, pt2);
	ModelPtr->GetModelAttachedPosStage(ModelStageCp);
	
	if (false == IsExceptionAngle)
	{
		RgnWndBox.minX = MIN(pt1.x, pt2.x);
		RgnWndBox.maxX = MAX(pt1.x, pt2.x);
		RgnWndBox.minY = MIN(pt1.y, pt2.y);
		RgnWndBox.maxY = MAX(pt1.y, pt2.y);
		ImageAPI.MapWndRgnToImageRgn_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, RgnWndBox, RgnImgBox);
		AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, RgnImgBox, m_FOVPosStage, RgnStgBox);

		CAOIComponent* ComponePtr = m_ModelPtr->GetModelComponentPtr();

		//相對於模組機台座標
		RgnStgBox.minX -= ModelStageCp.x;
		RgnStgBox.minY -= ModelStageCp.y;
		RgnStgBox.maxX -= ModelStageCp.x;
		RgnStgBox.maxY -= ModelStageCp.y;
		AOIDataCollect.MapStageOffsetRgnToCad(RgnStgBox, RgnCadBox);

		TREGION4D  ModelRgn;
		TPOINT2D Scale;
		RECT rect, rect2;
		ModelPtr->GetModelTotalRegionStage(ModelRgn);
		//ModelPtr->GetModelTotalRegion(ModelRgn);
		ModelPtr->GetModelImageScale(Scale);
		CAOIBox *body = ModelPtr->GetModelBodyBoxPtr();

		if (Scale.x == 0.01) //有時BuildModelUniFrameList->CreateModelUniFrameList沒被建立
		{
			Scale.x = 1 / m_FrameResolution.x;
		}
		if (Scale.y == 0.01)
		{
			Scale.y = 1 / m_FrameResolution.y;
		}

		//與model的rect比較位置
		double minx = ((ModelRgn.minX + ModelRgn.maxX) / 2) + RgnCadBox.minX; //RgnCadBox座標轉換到5120的中間
		double miny =((ModelRgn.minY + ModelRgn.maxY) / 2) + RgnCadBox.maxY;  //座標轉換
		
		int W = (RgnCadBox.maxX - RgnCadBox.minX) * Scale.x;//換成pixel
		int h = (RgnCadBox.maxY - RgnCadBox.minY) * Scale.y;
		int left = (minx - ModelRgn.minX) * Scale.x;	//左上座標偏差 scale成pix
		int top = (ModelRgn.maxY - miny) * Scale.y;
		//roiRect是model的rect
		rect.left = roiRect.left + left;
		rect.top = roiRect.top + top;
		rect.right = rect.left + W;
		rect.bottom = rect.top + h;
		rect2 = rect;

		RECT bodyRect;
		int bodyW = body->GetBoxSizeX()* Scale.x;
		int bodyH = body->GetBoxSizeY()* Scale.y;
		int bodyCposX = (roiRect.left + roiRect.right) / 2;
		int bodyCposY = (roiRect.bottom + roiRect.top) / 2;
		bodyRect.left = bodyCposX - bodyW / 2;
		bodyRect.top = bodyCposY - bodyH / 2;
		bodyRect.right = bodyRect.left + bodyW;
		bodyRect.bottom = bodyRect.top + bodyH;
		bodyRect.left = bodyRect.left - rect.left;//換成rect座標
		bodyRect.top = bodyRect.top - rect.top;
		bodyRect.right = bodyRect.right - rect.left;
		bodyRect.bottom = bodyRect.bottom - rect.top;

		//cv::Mat roi, roi2;
		//roi = M(cv::Rect(bodyRect.left, bodyRect.top, bodyW, bodyH));
		//roi2 = M(cv::Rect(roiRect.left, roiRect.top, roiRect.right- roiRect.left-1, roiRect.bottom- roiRect.top-1));
		//cv::imwrite("roiFinal.bmp", roi);
		//cv::imwrite("roiFinal2.bmp", roi2);

		JET::alg::CHeightDetection hd;
		JET::alg::SAutoCreateROIParam roiparam;
		JET::alg::SAutoCreateROIResult roiResult;
		JET::alg::SProcessMode sMode;

		roiparam.nType = 1;
		if(direc == 3)
			roiparam.nDirection = 3;// 引腳方向, 1=垂直, 2=水平, 3=垂直水平都有
		else
		{
			roiparam.nDirection = angleindex;
		}
		IMAGE_PTR image0 = NULL;
		IMAGE_PTR image1 = NULL;
		IMAGE_PTR image2 = NULL;
		IMAGE_SIZE MaskStep = W * 3;//JetAPI::GetBMPImagePixelsPerLine(W, 24, 4);
		//IMAGE_SIZE MaskStep2 = JetAPI::GetBMPImagePixelsPerLine(W, 24, 4);
		if (!ImageAPI.ExtractRoiImage(m_UniFrameList[0].ImageW, m_UniFrameList[0].ImageH, m_UniFrameList[0].ImageStep, m_UniFrameList[0].BitCount, m_UniFrameList[0].ImagePtr, rect, MaskStep, image0, false) ||
			!ImageAPI.ExtractRoiImage(m_UniFrameList[1].ImageW, m_UniFrameList[1].ImageH, m_UniFrameList[1].ImageStep, m_UniFrameList[1].BitCount, m_UniFrameList[1].ImagePtr, rect, MaskStep, image1, false) ||
			!ImageAPI.ExtractRoiImage(m_UniFrameList[2].ImageW, m_UniFrameList[2].ImageH, m_UniFrameList[2].ImageStep, m_UniFrameList[2].BitCount, m_UniFrameList[2].ImagePtr, rect, MaskStep, image2, false))
		{
			JetMemory.free_func(image0);
			JetMemory.free_func(image1);
			JetMemory.free_func(image2);
			JetAPI::ClearUniFrameList(UniFrameList);
			str.Format(_T("Error, ROI Fault"));
			JetAPI::ShowMessageBox(str);
			return false;
		}

		//cv::Size sz2((int)(W), (int)(h));
		//cv::Mat M0(sz2, CV_8UC3, image0, MaskStep);
		//cv::Mat M1(sz2, CV_8UC3, image1, MaskStep);
		//cv::Mat M2(sz2, CV_8UC3, image2, MaskStep);
		//cv::imwrite("M0.bmp", M0);
		//cv::imwrite("M1.bmp", M1);
		//cv::imwrite("M2.bmp", M2);

		roiparam.nImageH = h;
		roiparam.nImageW = W;
		roiparam.rectPartRange = bodyRect;
		roiparam.vtpu8Image[0] = image0;
		roiparam.vtpu8Image[1] = image1;
		roiparam.vtpu8Image[2] = image2;

		//string v = hd.GetVersion();
		//原本參數
		//roiparam.nImageH = m_UniFrameList[0].ImageH;
		//roiparam.nImageW = m_UniFrameList[0].ImageW;
		//roiparam.rectPartRange = rect;
		//roiparam.vtpu8Image[0] = m_UniFrameList[0].ImagePtr;
		//roiparam.vtpu8Image[1] = m_UniFrameList[1].ImagePtr;
		//roiparam.vtpu8Image[2] = m_UniFrameList[2].ImagePtr;

		//bool is = true;
		//hd.SetSaveImage(is);
		//CString s0 = "E:\\Jet";
		//string s = "E:\\Jet\\";
		//JetAPI::ClearFolder(s0);
		//bool isSave = hd.SetSavePathName(s);
		bool r;
		try
		{
			r = hd.AutoCreateROI(roiparam, roiResult);
		}
		catch (const std::exception&)
		{
			JetMemory.free_func(image0);
			JetMemory.free_func(image1);
			JetMemory.free_func(image2);
			JetAPI::ClearUniFrameList(UniFrameList);
			str.Format(_T("Error, Auto Create ROI Fault"));
			JetAPI::ShowMessageBox(str);
			return false;
		}
		
		
		if (r)
		{
			//cv::Mat result;
			//hd.ShowAutoCreateROIResult_2D(result);
		}
		else 
		{
			JetMemory.free_func(image0);
			JetMemory.free_func(image1);
			JetMemory.free_func(image2);
			JetAPI::ClearUniFrameList(UniFrameList);
			str.Format(_T("Error, Auto Create Land Fault"));
			JetAPI::ShowMessageBox(str);
			return false;
		}

		CString LandTypeTxt;
		CAOILand *LandPtr;
		const int LanGroupID = ModelPtr->GetModelLandFreeGroupID();
		//body = ModelPtr->GetModelBodyBoxPtr();

		int centerXOfPart = (roiResult.rtPart.right + roiResult.rtPart.left) / 2;
		int centerYOfPart = (roiResult.rtPart.bottom + roiResult.rtPart.top) / 2;
		for (int i = 0; i < roiResult.vtnCount_Pad.size(); i++)
		{
			for (int j = 0; j < roiResult.vtnCount_Pad[i]; j++)
			{
				RECT padRect = roiResult.vt2rtPad[i][j];
				RECT leadRect = roiResult.vt2rtLead[i][j];
				//cv::Mat roi2 = roi(cv::Rect(padRect.left, padRect.top, padRect.right - padRect.left-1, padRect.bottom - padRect.top-1));
				//cv::imshow("1", roi2);

				//padRect.left = padRect.left + left;//從小ROI位置移到中roi位置
				//padRect.right = padRect.right + left;
				//padRect.top = padRect.top + top;
				//padRect.bottom = padRect.bottom + top;

				padRect.left = padRect.left - centerXOfPart;//Rect移動到center
				padRect.right = padRect.right - centerXOfPart;
				padRect.top = -(padRect.top - centerYOfPart);
				padRect.bottom = -(padRect.bottom - centerYOfPart);

				padRect.left = padRect.left / Scale.x;//單位變成um
				padRect.right = padRect.right / Scale.x;
				padRect.top = padRect.top / Scale.y;
				padRect.bottom = padRect.bottom / Scale.y;
				//
				//leadRect.left = leadRect.left + left;//從ROI位置移到原圖
				//leadRect.right = leadRect.right + left;
				//leadRect.top = leadRect.top + top;
				//leadRect.bottom = leadRect.bottom + top;

				leadRect.left = leadRect.left - centerXOfPart;//從ROI位置移到原圖
				leadRect.right = leadRect.right - centerXOfPart;
				leadRect.top = -(leadRect.top - centerYOfPart);
				leadRect.bottom = -(leadRect.bottom - centerYOfPart);

				leadRect.left = leadRect.left / Scale.x;//單位變成um
				leadRect.right = leadRect.right / Scale.x;
				leadRect.top = leadRect.top / Scale.y;
				leadRect.bottom = leadRect.bottom / Scale.y;

				//leadRect以零件中心當原點
				LandTypeTxt = AOIDataDefine.GetLandTypeText(LandType);
				if (LandType == LAND_TYPE_IC_LEAD)
				{
					LandPtr = ModelPtr->CreateModelLand2(ModelType, LanGroupID, padRect, leadRect, LandType);
				}
				else
				{
					LandPtr = ModelPtr->CreateModelLand(ModelType, LanGroupID, padRect, leadRect, LandType);
				}


				if (NULL == LandPtr)
				{
					JetMemory.free_func(image0);
					JetMemory.free_func(image1);
					JetMemory.free_func(image2);
					JetAPI::ClearUniFrameList(UniFrameList);
					str.Format(_T("Error, Create Model Land Ptr Fault [%s]"), LandTypeTxt);
					JetAPI::ShowMessageBox(str);
					return false;
				}

				ModelPtr->AddModelLandPtr(LandPtr, false);
			}
		}

		//str = _T("Do you want to reposion the body?");
		//str = LoadMultiLanguageString(str, str);
		//if (IDNO != JetAPI::ShowMessageBox(str, MB_YESNO))
		//{
		//	TREGION4D boxStageRgn;
		//	rect2.left = rect2.left + roiResult.rtPart.left;
		//	rect2.top = rect2.top + roiResult.rtPart.top;
		//	rect2.right = rect2.left + (roiResult.rtPart.right - roiResult.rtPart.left);
		//	rect2.bottom = rect2.top + (roiResult.rtPart.bottom - roiResult.rtPart.top);

		//	if (ComponentAngle == 0 || ComponentAngle == 180)
		//	{
		//		ComponePtr->SetComponentRoiSizeW((roiResult.rtPart.right - roiResult.rtPart.left) / Scale.x);
		//		ComponePtr->SetComponentRoiSizeH((roiResult.rtPart.bottom - roiResult.rtPart.top) / Scale.y);
		//		ComponePtr->SetComponentBodySizeW((roiResult.rtPart.right - roiResult.rtPart.left) / Scale.x);
		//		ComponePtr->SetComponentBodySizeH((roiResult.rtPart.bottom - roiResult.rtPart.top) / Scale.y);
		//	}
		//	else
		//	{
		//		ComponePtr->SetComponentRoiSizeW((roiResult.rtPart.bottom - roiResult.rtPart.top) / Scale.y);
		//		ComponePtr->SetComponentRoiSizeH((roiResult.rtPart.right - roiResult.rtPart.left) / Scale.x);
		//		ComponePtr->SetComponentBodySizeW((roiResult.rtPart.bottom - roiResult.rtPart.top) / Scale.y);
		//		ComponePtr->SetComponentBodySizeH((roiResult.rtPart.right - roiResult.rtPart.left) / Scale.x);
		//	}
		//	AOIDataCollect.MapCameraRegionToStage(ImageW, ImageH, ImageRes, rect2, m_FOVPosStage, boxStageRgn);

		//	double y = ModelStageCp.y - boxStageRgn.GetCpY();
		//	double x = ModelStageCp.x - boxStageRgn.GetCpX();
		//	ModelPtr->MoveModel(x, -y);
		//}
		ComponePtr->CalcComponentCadCornerPos();
		ComponePtr->LayoutComponentStageCornerPos();
		ComponePtr->UpdateComponentParamToModel(false);
		//


		ModelPtr->UnSelectModel();

		ModelPtr->CalcModelTotalRegionAll();
		ModelPtr->UpdateModelBodyToComponent();
		LogOperCtrl.SaveLogModelLandSelectedCreate(ModelPtr);		
		MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelModeDefault();
		SwitchManiModelMode(ManiMode);
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();

		JetMemory.free_func(image0);
		JetMemory.free_func(image1);
		JetMemory.free_func(image2);
		JetAPI::ClearUniFrameList(UniFrameList);
		return true;
	}
	else
	{
	}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelRegionSelect()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }

	size_t       i=0, j=0;
	TActiveObj  *ObjPtr = NULL;	
	TActiveObj  *ObjPtr2 = NULL;	
	bool         bNeedResetSel=true;
	const size_t NObjects = this->m_ActiveObjList.size();	
	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	const CAOIWnd *LastWndPtr = ModelPtr->GetModelWndActived();
	const bool MultiSelectMode = AOIDataCollect.CheckMultiSelectMode();		
	
	//改成點到後才清除
	bNeedResetSel = true;
	if ( true == MultiSelectMode )
	{	bNeedResetSel = false;	}
	else
	{	
		if ( MANIPULATE_MODEL_EDIT != ManiMode )
		{
			ModelPtr->UnSelectModel();
			ModelPtr->SetModelWndActived(NULL);
			ModelPtr->SetModelLandActived(NULL);
			ProjectPtr->SetProjectActiveModelWnd(NULL);
			ProjectPtr->SetProjectActiveModelLand(NULL);
			ResetActiveObjPosSelect();
			bNeedResetSel = false;
		}
	}	
	
	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       ImageAngle = 0;		
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *MaskWndPtr = NULL;
	CAlgParam   *AlgParamPtr=NULL;
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;		
	POINT        Pt1, Pt2;
	TPOINT2D     Cp, dP, dP1, dP2;
	TPOINT2D     WnddPt, WndPt1, WndPt2;	
	TPOINT2D     ImagePt, ImagePt1, ImagePt2;
	TPOINT2D     CornerPoint[4];	
	POINT        ImagePoint;	
	double       ZoomScale = m_ImageZoom;		
	bool         IsPickMode = true;
	UUID         uuidBox, uuidWnd, uuidLand;	
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	
	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();	
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, Pt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, Pt2);
	WndPt1 = Pt1;
	WndPt2 = Pt2;
	WnddPt.x = ::abs(WndPt1.x-WndPt2.x);
	WnddPt.y = ::abs(WndPt1.y-WndPt2.y);
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);
	ImagePt.x = (ImagePt1.x+ImagePt2.x)*0.5;
	ImagePt.y = (ImagePt1.y+ImagePt2.y)*0.5;	
	if ( WnddPt.x<2 || WnddPt.y<2 )
	{	IsPickMode = true;	}
	else
	{	IsPickMode = false;	}

	Cp  = ImagePt;
	Cp.x = Cp.y = 0;
	bool bSelectedObj=false;
	if ( false == bNeedResetSel )
	{	ResetActiveObjPosFocus();	}
	else
	{	
		ModelPtr->UnSelectModel();
		ModelPtr->SetModelWndActived(NULL);
		ModelPtr->SetModelLandActived(NULL);
		ProjectPtr->SetProjectActiveModelWnd(NULL);
		ProjectPtr->SetProjectActiveModelLand(NULL);
		ResetActiveObjPosSelect();
	}
	if ( true == IsPickMode )
	{	
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			//if ( false == ObjPtr->GetEditabled() ) { continue; }
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			if ( ObjPtr->IsExceptionAngle == false )
			{	
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				dP = ImagePt;
				ImageAngle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);				
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];
				JetAPI::RotateCornerPos(-ImageAngle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dP);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);
				JetAPI::Point2DToPoint(dP, ImagePoint);
				JetAPI::Rect4DToRect(dRect, Rect);				
			}
			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	
				bSelectedObj = true;
				ObjPtr->SetFocused(true);			
				ObjPtr->SetSelected(true);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	
					BoxPtr->SetBoxActived(true);
					BoxPtr->SetBoxSelected(true);
					uuidBox = BoxPtr->GetObjUuid();
					ProjectPtr->SetProjectActiveModelBox(BoxPtr);		

					WndPtr = (CAOIWnd*)(ObjPtr->WndPtr);
					ProjectPtr->SetProjectActiveModelWnd(WndPtr);
					if ( NULL != WndPtr )
					{					
						uuidWnd = WndPtr->GetObjUuid();	
						WndPtr->SetWndSelected(true);
						ModelPtr->SetModelWndActived(WndPtr);

						AlgParamPtr = WndPtr->GetWndAlgParamPtr();
						WndRoiPtr = (CAOIWndRoi*)(ObjPtr->WndRoiPtr);
						MaskWndPtr = (CAOIWndMask*)(ObjPtr->WndMaskPtr);
						WndPtr->SetWndRoiWndActived(WndRoiPtr);
						WndPtr->SetWndMaskWndActived(MaskWndPtr);
						if ( NULL == WndRoiPtr ) 
						{	AlgParamPtr->SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}
						else
						{
							if ( WndRoiPtr->GetWndRoiBinaryParamEnabled() == true ) 
							{	AlgParamPtr->SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ROI_IMAGE);	}
							else
							{	AlgParamPtr->SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}
						}
					}

					LandPtr = (CAOILand*)(ObjPtr->LandPtr);
					if ( NULL != LandPtr )
					{					
						//LandPtr->GetLandBoxPtr()->SetBoxActived(true);
						//LandPtr->GetLandBoxPtr()->SetBoxSelected(true);						
					}
					ProjectPtr->SetProjectActiveModelLand(LandPtr); 
					PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);					
				}
				break; 
			}			
		}		
	}
	else//框選
	{
		RECT  SelRect={0};		
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			//if ( false == ObjPtr->GetEditabled() ) { continue; }

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			if ( ObjPtr->IsExceptionAngle == false )
			{			
				SelRect.left   = JetAPI::Floor(MIN(ImagePt1.x, ImagePt2.x));
				SelRect.top    = JetAPI::Floor(MIN(ImagePt1.y, ImagePt2.y));
				SelRect.right  = JetAPI::Floor(MAX(ImagePt1.x, ImagePt2.x));
				SelRect.bottom = JetAPI::Floor(MAX(ImagePt1.y, ImagePt2.y));
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);
			}
			else
			{	
				dP1 = ImagePt1;
				dP2 = ImagePt2;				
				ImageAngle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);								
				JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dP1);
				JetAPI::RotatePos(-ImageAngle, Cp.x, Cp.y, dP2);		
				SelRect.left   = JetAPI::Floor(MIN(dP1.x, dP2.x));
				SelRect.top    = JetAPI::Floor(MIN(dP1.y, dP2.y));
				SelRect.right  = JetAPI::Floor(MAX(dP1.x, dP2.x));
				SelRect.bottom = JetAPI::Floor(MAX(dP1.y, dP2.y));
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);

				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];
				JetAPI::RotateCornerPos(-ImageAngle, Cp.x, Cp.y, CornerPoint);				
				JetAPI::PointsToRect(CornerPoint, 4, dRect);
				JetAPI::Rect4DToRect(dRect, Rect);
				
			}			
			if ( JetAPI::RectInRect(Rect, SelRect) == false ) { continue; }
			bSelectedObj = true;
			ObjPtr->SetSelected(true);
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			if ( NULL != BoxPtr )
			{	BoxPtr->SetBoxSelected(true);	}
		}

		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			//if ( false == ObjPtr->GetEditabled() ) { continue; }
			//if ( false == ObjPtr->GetFocused() ) { continue; }
			if ( false == ObjPtr->GetSelected() ) { continue; }
			ObjPtr->SetFocused(true);
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			uuidBox = BoxPtr->GetObjUuid();
			ProjectPtr->SetProjectActiveModelBox(BoxPtr);			

			WndPtr = (CAOIWnd*)(ObjPtr->WndPtr);
			ProjectPtr->SetProjectActiveModelWnd(WndPtr);
			if ( NULL != WndPtr )
			{	
				uuidWnd = WndPtr->GetObjUuid();	
				WndPtr->SetWndSelected(true);
				ModelPtr->SetModelWndActived(WndPtr);				

				AlgParamPtr = WndPtr->GetWndAlgParamPtr();
				WndRoiPtr = (CAOIWndRoi*)(ObjPtr->WndRoiPtr);
				MaskWndPtr = (CAOIWndMask*)(ObjPtr->WndMaskPtr);
				WndPtr->SetWndRoiWndActived(WndRoiPtr);
				WndPtr->SetWndMaskWndActived(MaskWndPtr);
				if ( NULL == WndRoiPtr ) 
				{	AlgParamPtr->SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}
				else
				{
					if ( WndRoiPtr->GetWndRoiBinaryParamEnabled() == true ) 
					{	AlgParamPtr->SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ROI_IMAGE);	}
					else
					{	AlgParamPtr->SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);	}
				}
			}
				
			LandPtr = (CAOILand*)(ObjPtr->LandPtr);
			if ( NULL != LandPtr )
			{	
				uuidLand = LandPtr->GetObjUuid();
				LandPtr->GetLandBoxPtr()->SetBoxActived(true);
				//LandPtr->GetLandBoxPtr()->SetBoxSelected(true);				
			}
			ProjectPtr->SetProjectActiveModelLand(LandPtr); 
			PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
			break;
		}
	}
	if ( false==bSelectedObj && true==bNeedResetSel )
	{	
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( NULL == ObjPtr->BoxPtr ) { continue; }
			if ( NULL != ObjPtr->WndPtr ) { continue; }
			if ( NULL != ObjPtr->LandPtr ) { continue; }
			if ( NULL == ObjPtr->ModelPtr ) { continue; }
			if ( NULL != ObjPtr->WndRoiPtr ) { continue; }
			if ( NULL != ObjPtr->WndMaskPtr ) { continue; }
			ObjPtr->SetFocused(true);
			ObjPtr->SetSelected(true);
			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			BoxPtr->SetBoxActived(true);
			BoxPtr->SetBoxSelected(true);				
			break;
		}
	}

	DRAW_MODEL_MODE DrawModelMode=GetDrawModelMode();
	if ( DRAW_MODEL_RESULT==DrawModelMode && true==bSelectedObj && true==IsPickMode )
	{
		CAOIWnd *SelectedWnd=ModelPtr->GetModelWndActived();
		if ( LastWndPtr == SelectedWnd )
		{	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);	}
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelComponentSelect()
{	
	CAOIProject *ProjectPtr = m_ProjectPtr;
	if ( NULL == ProjectPtr ) { return false; }	

	TREGION4D Rgn;	
	TPOINT2D  WndPt, ImagePt, StagePt;	
	bool       bResultMode = false;
	std::vector<CAOIComponent*> ComponentList;
	const DRAW_COMPONENT_MODE DrawComponentMode = AOIDataCollect.GetDrawComponentMode();
	if ( DRAW_COMPONENT_FOCUSED == DrawComponentMode ) { return false; }
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D   StageCp = GetImageStageRgnCp();	
	const TPOINT2D  &ImageRes = GetImageResolution();

	StagePt = ImagePt = WndPt = m_MousePosImageWnd;
	CEditModelView::MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);		
	AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt, StageCp, StagePt);
	Rgn.minX = Rgn.maxX = StagePt.x;
	Rgn.minY = Rgn.maxY = StagePt.y;
	ProjectPtr->SelectProjectComponentsByStage(Rgn, bResultMode, ComponentList);
	const size_t SelCount = ComponentList.size();
	if ( 0 == SelCount ) { return false; }		
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	CAOIComponent *pComponent = ComponentList[0];
	CAOIModel   *BeforeModelPtr = GetModelPtr();

	ProjectPtr->SelectProjectAllComponents(false);
	pComponent->SetComponentSelected(true);	
	if ( NULL != BeforeModelPtr )
	{	
		CAOIComponent *BeforeComponentPtr = BeforeModelPtr->GetModelComponentPtr();
		if ( BeforeComponentPtr == pComponent )
		{
			ProjectPtr->SyncProjectCompoenntSelectedToModel();
			RedrawWnd();			
			return false; 
		}
		if ( DRAW_MODEL_TEMP != DrawModelMode )
		{	
			AOIDataCollect.CloseActiveModel(BeforeModelPtr);	
			BeforeComponentPtr->SetComponentSelected(false);
		}
	}	
	pComponent->SetComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(pComponent);
	ProjectPtr->SyncProjectCompoenntSelectedToModel();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelModeDefault();	

	CAOIModel *ModelPtr = pComponent->GetComponentModelPtr();	
	ModelPtr->UnSelectModel();
	ModelPtr->SetModelBodyBoxActived(true);	
	SetModel(ModelPtr);		
	//m_Model = *ModelPtr;
	UpdateModelStats();	
	SwitchManiModelMode(ManiMode);	
	RedrawWnd();

	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::UpdateModelStats()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	CAOIModel *ModelPtr = GetModelPtr();	
	BuildActiveObjList(ModelPtr, false);
	if ( NULL != ModelPtr )
	{
		const bool bClone=false;
		const int  nAlign = 4;		
		const bool bNoFilter=false;
		std::vector<TUNI_FRAME> UniFrameList;
		if ( ModelPtr->GetModelUnsetState() == true )
		{
			m_ImageIndex = 0;
			if ( NULL != ProjectPtr )
			{	ProjectPtr->SetProjectMapIndex(m_ImageIndex);	}
		}
		BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, bClone, bNoFilter);
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(UniFrameList); }
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::UpdateActiveObjList()
{
	CString     str;
	size_t      i=0;
	TRECT4D     Rect;	
	TPOINT2D    CornPoint[4];
	TPOINT2D    CornerPoint[4];	
	CAOIBox    *BoxPtr   = NULL;
	TActiveObj *ObjPtr = NULL;
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	const size_t NObjects = this->m_ActiveObjList.size();	
	const DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( NULL == ObjPtr ) { continue; }
		if ( NULL == ObjPtr->BoxPtr ) { continue; }
		BoxPtr = (CAOIBox*)(ObjPtr->BoxPtr);

		if ( DRAW_MODEL_RESULT == DrawModelMode )
		{	BoxPtr->GetBoxCornerPosStageRes(CornerPoint);	}
		else
		{	BoxPtr->GetBoxCornerPosStage(CornerPoint); }		
		AOIDataCollect.MapStageCornerToCamera(ImageW, ImageH, ImageRes, CornerPoint, StageCp, CornPoint);//機台4端點對應到影像四點
		JetAPI::PointsToRect(CornPoint, 4, Rect);
		ObjPtr->Rect       = Rect;
		ObjPtr->CornerPts[0] = CornPoint[0];
		ObjPtr->CornerPts[1] = CornPoint[1];
		ObjPtr->CornerPts[2] = CornPoint[2];
		ObjPtr->CornerPts[3] = CornPoint[3];		
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::BuildActiveObjList(CAOIModel *ModelPtr, bool ActiveOnly)
{
	bool CheckAddObj = false;
//	if ( AOIDataCollect.GetIsPressVRKey(m_MultiSelKey) == false )
//	{	CheckAddObj = false;}
//	else
//	{	CheckAddObj = true;	}
	this->m_ActiveObjList.clear();
	if ( NULL == ModelPtr ) { return; }

	CString      str;	
	size_t       i=0, j=0, k=0, s=0;
	size_t       LandCount = 0;	
	size_t       WndCount  = 0;
	size_t       BoxWndCount = 0;
	size_t       WndRoiCount = 0;
	size_t       MaskWndCount = 0;
	CAOIBox     *BoxPtr   = NULL;
	CAOIWnd     *WndPtr   = NULL;
	CAOILand    *LandPtr  = NULL;			
	CAOIWndRoi  *WndRoiPtr  = NULL;
	CAOIWndMask *MaskWndPtr  = NULL;
	TActiveObj   ActiveObj;
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	bool ModelActiveOnly = true;	
	bool AddModelRect    = true;
	bool bBoxWndSelected = false;
	bool bWndBoxWndSelected = false;
	bool bLandBoxWndSelected = false;	
	bool bWndRoiBoxSelected = false;
	bool bMaskBoxSelected = false;

	ActiveObj.ModelPtr = ModelPtr;	
	ModelActiveOnly = TRUE;
	
	ActiveObj = TActiveObj();
	ActiveObj.ComponentAngle = ComponentAngle;
	ActiveObj.IsExceptionAngle = IsExceptionAngle;	
	ActiveObj.WndPtr     = NULL;
	ActiveObj.LandPtr    = NULL;
	ActiveObj.BoxPtr     = NULL;
	ActiveObj.WndRoiPtr  = NULL;
	ActiveObj.WndMaskPtr = NULL;
	ActiveObj.SetEditabled(true);
	ActiveObj.PassObj    = false;	

	//if ( false == ActiveOnly ) 
	{		
		WndCount = ModelPtr->GetModelWndCount();			
		for ( j=0; j<WndCount; j++ )
		{
			WndPtr = ModelPtr->GetModelWndPtr(j, false);				
			if ( NULL == WndPtr ) { continue; }
			
			BoxPtr = WndPtr->GetWndBoxPtr();			
			if ( NULL == BoxPtr ) { continue; }
			//if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
			if ( BoxPtr->GetBoxVisibled() == false ) 
			{ 
				BoxPtr->SetBoxSelected(false);
				continue; 
			}			
			if ( true == ActiveOnly )
			{
				if ( BoxPtr->GetBoxSelected() == false ) { continue; }
			}
			//子框
			bWndRoiBoxSelected = false;
			WndRoiCount = WndPtr->GetWndRoiWndCount();
			for ( k=0; k<WndRoiCount; k++ )
			{
				WndRoiPtr = WndPtr->GetWndRoiWndPtr(k, false);
				if ( NULL == WndRoiPtr ) { continue; }
				BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) 
				{	bWndRoiBoxSelected = true; }

				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = WndPtr;					
				ActiveObj.LandPtr    = NULL;
				ActiveObj.WndRoiPtr  = WndRoiPtr;
				ActiveObj.WndMaskPtr = NULL;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}
			//遮罩框
			bMaskBoxSelected = false;
			MaskWndCount = WndPtr->GetWndMaskWndCount();
			for ( k=0; k<MaskWndCount; k++ )
			{
				MaskWndPtr = WndPtr->GetWndMaskWndPtr(k, false);
				if ( NULL == MaskWndPtr ) { continue; }
				BoxPtr = MaskWndPtr->GetWndMaskBoxPtr();
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxSelected() == true ) 
				{	bMaskBoxSelected = true; }

				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = WndPtr;					
				ActiveObj.LandPtr    = NULL;
				ActiveObj.WndRoiPtr  = NULL;
				ActiveObj.WndMaskPtr = MaskWndPtr;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}			

			BoxPtr = WndPtr->GetWndBoxPtr();	
			ModelActiveOnly = false;
			BoxPtr = WndPtr->GetWndBoxPtr();	
			ActiveObj.ModelPtr   = ModelPtr;
			ActiveObj.WndPtr     = WndPtr;					
			ActiveObj.LandPtr    = NULL;
			ActiveObj.WndRoiPtr  = NULL;
			ActiveObj.WndMaskPtr = NULL;
			ActiveObj.BoxPtr     = BoxPtr;			
			ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
			ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
			ActiveObj.SetFocused(BoxPtr->GetBoxActived());
			if ( true==bWndRoiBoxSelected || true==bMaskBoxSelected)
			{	
				ActiveObj.SetSelected(false);
				ActiveObj.SetFocused(false);
			}
			this->AddActiveObject(ActiveObj, CheckAddObj);			
		}
	}	

	//if ( false == ActiveOnly ) 
	{
		LandCount = ModelPtr->GetModelLandCount();
		for ( j=0; j<LandCount; j++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(j, false);			
			if ( NULL == LandPtr ) { continue; }
			for ( k=0; k<5; k++ )
			{				
				switch ( k )
				{				
				case 0:	BoxPtr = LandPtr->GetLandLeadBoxPtr();	break;
				case 1:	BoxPtr = LandPtr->GetLandPadBoxPtr();	break;
				case 2:	BoxPtr = LandPtr->GetLandLeadShoulderBoxPtr();	break;
				case 3:	BoxPtr = LandPtr->GetLandLeadTipBoxPtr();	break;
				case 4:	BoxPtr = LandPtr->GetLandBodyEdgeBoxPtr();	break;
				default:	BoxPtr = NULL;	break;
				}				
				if ( NULL == BoxPtr ) { continue; }
				if ( BoxPtr->GetBoxEnabled() == false ) { continue; }
				if ( BoxPtr->GetBoxVisibled() == false ) { continue; }				
				if ( true == ActiveOnly )
				{
					if ( BoxPtr->GetBoxSelected() == false ) { continue; }
				}
				ModelActiveOnly = false;
				ActiveObj.ModelPtr   = ModelPtr;
				ActiveObj.WndPtr     = NULL;
				ActiveObj.LandPtr    = LandPtr;
				ActiveObj.WndRoiPtr  = NULL;
				ActiveObj.WndMaskPtr = NULL;
				ActiveObj.BoxPtr     = BoxPtr;				
				ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
				ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
				ActiveObj.SetFocused(BoxPtr->GetBoxActived());
				this->AddActiveObject(ActiveObj, CheckAddObj);
			}
		}
	}
	
	AddModelRect    = true;
	if ( true == ActiveOnly )
	{
		if ( (ModelPtr->GetModelBodyBox().GetBoxSelected()==false) || (false==ModelActiveOnly) )
		{	AddModelRect = false;	}
	}		

	if ( true == AddModelRect )
	{
		BoxPtr = ModelPtr->GetModelBodyBoxPtr();
		ActiveObj.ModelPtr   = ModelPtr;		
		ActiveObj.LandPtr    = NULL;
		ActiveObj.BoxPtr     = BoxPtr;		
		ActiveObj.WndPtr     = NULL;
		ActiveObj.WndRoiPtr  = NULL;
		ActiveObj.WndMaskPtr = NULL;
		ActiveObj.ComponentAngle = ComponentAngle;		
		ActiveObj.SetEditabled(BoxPtr->GetBoxEditabled());
		ActiveObj.SetSelected(BoxPtr->GetBoxSelected());
		ActiveObj.SetFocused(BoxPtr->GetBoxActived());
		this->AddActiveObject(ActiveObj, CheckAddObj);
	}	
	UpdateActiveObjList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::AddActiveObject(const TActiveObj &ActiveObj, bool Check)
{
	if ( Check == true ) 
	{
		size_t i = 0;
		TActiveObj   *ActiveObjPtr = NULL;
		size_t size = this->m_ActiveObjList.size();

		if ( size > 0 ) 
		{
			for ( i=0; i<size; i++ )
			{
				ActiveObjPtr = &(m_ActiveObjList[i]);
				if ( ActiveObjPtr->ModelPtr != ActiveObj.ModelPtr ) 
				{	break;  }
				if ( ActiveObjPtr->BoxPtr != ActiveObj.BoxPtr ) 
				{	break;  }
				if ( ActiveObjPtr->WndPtr != ActiveObj.WndPtr ) 
				{	break;  }
				if ( ActiveObjPtr->LandPtr != ActiveObj.LandPtr ) 
				{	break;  }				
				if ( ActiveObjPtr->WndRoiPtr != ActiveObj.WndRoiPtr ) 
				{	break;  }
				if ( ActiveObjPtr->WndMaskPtr != ActiveObj.WndMaskPtr ) 
				{	break;  }				
			}		
			if ( i == size ) { return; }
		}
	}	

	if ( ActiveObj.ComponentAngle < -1000 )
	{
		Check = Check;
	}
	this->m_ActiveObjList.push_back(ActiveObj);	
}
//-------------------------------------------------------------------------------------//
inline MANIPULATE_MODEL_MODE CEditModelView::GetManiModelMode() const
{
	return AOIDataCollect.GetManipulateModelMode();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SwitchManiModelMode(MANIPULATE_MODEL_MODE ManiMode)
{
	switch ( ManiMode)
	{
	case MANIPULATE_MODEL_ADD:
		//CWnd::CheckDlgButton(MODELEDIT_EDIT_MODE_CHK, FALSE);
		//CWnd::CheckDlgButton(MODELEDIT_SELECT_MODE_CHK, FALSE);		
		//CWnd::CheckDlgButton(MODELEDIT_ADD_MODE_CHK, TRUE);
		break;
	case MANIPULATE_MODEL_EDIT:
		//CWnd::CheckDlgButton(MODELEDIT_ADD_MODE_CHK, FALSE);
		//CWnd::CheckDlgButton(MODELEDIT_SELECT_MODE_CHK, FALSE);		
		//CWnd::CheckDlgButton(MODELEDIT_EDIT_MODE_CHK, TRUE);
		break;
	case MANIPULATE_MODEL_SELECT:
		//CWnd::CheckDlgButton(MODELEDIT_ADD_MODE_CHK, FALSE);		
		//CWnd::CheckDlgButton(MODELEDIT_EDIT_MODE_CHK, FALSE);
		//CWnd::CheckDlgButton(MODELEDIT_SELECT_MODE_CHK, TRUE);		
		break;
	}
	//m_ManiMode = ManiMode;	
	AOIDataCollect.SetManipulateModelMode(ManiMode);	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ResetActiveObjPosFocus()
{
	size_t       i = 0;	
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		ObjPtr->SetFocused(false);		
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ResetActiveObjPosSelect()
{
	size_t       i = 0;	
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		ObjPtr->SetFocused(false);
		ObjPtr->SetSelected(false);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CheckActiveObjFocus(TActiveObj &Obj)
{
	Obj = TActiveObj();
	if ( CURSOR_POS_NONE == m_MousePosMode )
	{	return; }
	const size_t NObjects = this->m_ActiveObjList.size();	
	for ( size_t i=0; i<NObjects; i++ )
	{
		const TActiveObj &ObjRef = m_ActiveObjList[i];
		if ( ObjRef.GetFocused() == false ) { continue; }
		Obj = ObjRef;
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CheckActiveObjFocus(TActiveObj &Obj, CAOIBox &Box)
{
	CheckActiveObjFocus(m_ActiveObj);	
	if ( NULL == m_ActiveObj.BoxPtr )
	{	
		Box = CAOIBox();	
		return;
	}
			
	CAOIBox *BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, m_ActiveObj.BoxPtr);
	if ( NULL == BoxPtr )
	{	
		Box = CAOIBox();	
		return;
	}
	Box = *(BoxPtr);	
	return;	
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModifyActiveObjPos()//執行選中物件的座標
{
	const int nWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const int nWndPy = m_MousePosCurrent.y-m_MousePosLast.y;	
	return ExecModifyActiveObjPosKernel(nWndPx, nWndPy);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModifyActiveObjPosKernel(int nWndPx, int nWndPy)//執行選中物件的座標
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return false; }
	if ( ModelPtr->GetModelEditMode() == false ) { return false; }		
	if ( (0==nWndPx) && (0==nWndPy) )	{	return false; }

	size_t      i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();
	BoxPtr = NULL;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( false == ObjPtr->GetFocused() ) { continue; }
		if ( false == ObjPtr->GetEditabled() ) { continue; }
		BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
		if ( NULL != BoxPtr )
		{	break;	}
	}
	if ( NULL == BoxPtr ) { return false; }
	if ( FN_ENABLE == AOIDataCollect.GetSystemParameter().m_LockModelBodyPosition )
	{	
		if ( ObjPtr->CheckModelBodyBox() == true )
		{	
			if ( AOIDataCollect.CheckMoveObjectMode() == false )
			{	return true;	}
		}		
	}

	const double dImagePx = nWndPx*m_ImageZoom;
	const double dImagePy = nWndPy*m_ImageZoom;
	const TPOINT2D &ImageRes = GetImageResolution();
	const double ResX = ImageRes.x;
	const double ResY = ImageRes.y;
	const double dCadPx = dImagePx*ResX;
	const double dCadPy = -1*dImagePy*ResY;//Y軸反向
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	BoxPtr    = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr    = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr   = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);

	std::vector<TActiveObj*> ObjPtrList;
	ModelPtr->BuildModelSelectedObjListForPos(ObjPtr, m_ActiveObjList, ObjPtrList);
	ModelPtr->ModifyModelObjPos(ObjPtrList, dCadPx, dCadPy, LinkMode);			
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModifyActiveObjSize(CURSOR_POS_MODE CursorMode)//執行選中物件的尺寸
{
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	if ( ModelPtr->GetModelEditMode() == false ) { return true; }	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return true; }	

	size_t      i = 0;	
	CAOIBox     *BoxPtr = NULL;
	CAOIWnd     *WndPtr = NULL;
	CAOILand    *LandPtr = NULL;
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	TActiveObj  *ObjPtr = NULL;		
	const size_t NObjects = this->m_ActiveObjList.size();
	BoxPtr = NULL;
	for ( i=0; i<NObjects; i++ )
	{
		ObjPtr = &(m_ActiveObjList[i]);
		if ( false == ObjPtr->GetFocused() ) { continue; }
		if ( false == ObjPtr->GetEditabled() ) { continue; }
		BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
		if ( NULL != BoxPtr )
		{	break;	}
	}
	if ( NULL == BoxPtr ) { return false; }
	if ( FN_ENABLE == AOIDataCollect.GetSystemParameter().m_LockModelBodyPosition )
	{		
		if ( ObjPtr->CheckModelBodyBox() == true )
		{
			if ( AOIDataCollect.CheckDoubleSideEdit() == false )
			{	return true;	}
		}		
	}

	TREGION4D dRgn;	
	const TPOINT2D &ImageRes = GetImageResolution();
	const double dWndPx = m_MousePosCurrent.x-m_MousePosLast.x;
	const double dWndPy = m_MousePosCurrent.y-m_MousePosLast.y;
	const double dImagePx = dWndPx*m_ImageZoom;
	const double dImagePy = dWndPy*m_ImageZoom;
	const double ResX = ImageRes.x;
	const double ResY = ImageRes.y;	
	const bool   DoubleSideEdit = AOIDataCollect.CheckDoubleSideEdit();		
	const int    LinkMode = ModelPtr->GetModelWndLinkMode();

	if ( true == ObjPtr->IsExceptionAngle )
	{
		double dRevPx=dImagePx*ResX;
		double dRevPy=dImagePy*ResY;		
		double Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);		
		JetAPI::RotatePos(-Angle, 0, 0, dRevPx, dRevPy);
		dRevPx =  dRevPx;
		dRevPy = -dRevPy;		
		JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dRevPx, dRevPy, dRgn);
	}
	else
	{
		const double dCadPx = dImagePx*ResX;
		const double dCadPy = -1*dImagePy*ResY;//Y軸反向	
		JetAPI::CalcModifySizeRegion(CursorMode, DoubleSideEdit, dCadPx, dCadPy, dRgn);	
	}
	
	BoxPtr = DYNAMIC_DOWNCAST(CAOIBox, ObjPtr->BoxPtr);
	WndPtr = DYNAMIC_DOWNCAST(CAOIWnd, ObjPtr->WndPtr);
	LandPtr = DYNAMIC_DOWNCAST(CAOILand, ObjPtr->LandPtr);
	WndRoiPtr = DYNAMIC_DOWNCAST(CAOIWndRoi, ObjPtr->WndRoiPtr);
	WndMaskPtr = DYNAMIC_DOWNCAST(CAOIWndMask, ObjPtr->WndMaskPtr);
	
	std::vector<TActiveObj*> ObjPtrList;
	ModelPtr->BuildModelSelectedObjListForSize(ObjPtr, m_ActiveObjList, ObjPtrList);
	ModelPtr->ModifyModelObjSize(ObjPtrList, dRgn, LinkMode);		
	UpdateActiveObjList();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CalcModelLandCount(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore, int nCountMode)//計算影像中的焊盤數量
{
	if ( NULL == ModelPtr ) { return false; }
	if ( ModelPtr->GetModelEditMode() == false ) { return false; }

	bool  IsOK = true;	
	if ( LAND_COUNT_MODE_ARRAY == nCountMode )
	{	IsOK = CalcModelLandCount_Array(ModelPtr, BoxResultList, Pitch, MinScore);  }
	else if ( LAND_COUNT_MODE_2D == nCountMode )
	{	IsOK = CalcModelLandCount_BGA_DIP(ModelPtr, BoxResultList, Pitch, MinScore);	}	
	else
	{	IsOK = CalcModelLandCount_Normal(ModelPtr, BoxResultList, Pitch, MinScore);	}	
	return IsOK;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CalcModelLandCount_Array(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore)//計算影像中的焊盤數量
{
	if ( NULL == ModelPtr ) { return false; }	
	const char fnName[] = "CEditModelView::CalcModelLandCount_Array";

	size_t       i=0,j=0;
	TREGION4D    BoxRgn;
	TREGION4D    LandRgn;
	TREGION4D    LandSelRgn;
	size_t       LandCount = 0;
	int          LandAlignID = 0;
	CAOIBox     *BoxPtr = NULL;	
	CAOILand    *LandPtr = NULL;	
	std::vector<CAOILand*> LandSelList;
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	const size_t ModelLandCount = ModelPtr->GetModelLandCount();	
	
	LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr ) {	return false; }			
	const int    LandGroupID = LandPtr->GetLandGroupID();
	const BOX_TOWARD LandToward = LandPtr->GetLandToward();
	ModelPtr->GetModelLandSelectedList(LandGroupID, LandSelList);
	const size_t LandSelCount = LandSelList.size();
	if ( 2 != LandSelCount ) { return false; }

	CString      strTitle;
	CString      strValue1;
	CString      strLabel1;
	CString      strValue2;
	CString      strLabel2;	
	CInputBoxWnd InputBox;

	double       RangeX=0, RangeY=0;	
	int          CountX=2, CountY=2;
	TPOINT2D     LandPos1, LandPos2;	
	TPOINT2D     RotatePt1, RotatePt2;	
	CAOILand    *LandPtr_1 = LandSelList[0];
	CAOILand    *LandPtr_2 = LandSelList[1];
	LandPtr_1->GetLandBoxPtr()->GetBoxPos(LandPos1);
	LandPtr_2->GetLandBoxPtr()->GetBoxPos(LandPos2);
	LandPtr_1->GetLandBoxPtr()->GetBoxPosStage(LandPos1);
	LandPtr_2->GetLandBoxPtr()->GetBoxPosStage(LandPos2);

	const double CPX = (LandPos1.x+LandPos2.x)*0.5;
	const double CPY = (LandPos1.y+LandPos2.y)*0.5;
	if ( true == IsExceptionAngle )
	{		
		RotatePt1=LandPos1;
		RotatePt2=LandPos2;		
		JetAPI::RotatePos(-AttachedAngle, CPX, CPY, RotatePt1);
		JetAPI::RotatePos(-AttachedAngle, CPX, CPY, RotatePt2);
		LandPos1 = RotatePt1;
		LandPos2 = RotatePt2;
	}
	const double PosMinX=MIN(LandPos1.x, LandPos2.x);
	const double PosMaxX=MAX(LandPos1.x, LandPos2.x);
	const double PosMinY=MIN(LandPos1.y, LandPos2.y);
	const double PosMaxY=MAX(LandPos1.y, LandPos2.y);
	RangeX = PosMaxX-PosMinX;
	RangeY = PosMaxY-PosMinY;

	strTitle = _T("Input X, Y Count");
	strLabel1.Format(_T("%s-%s"), _T("X"), AOIDataDefine.GetAxisText());
	strValue1.Format(_T("%d"), CountX);
	strLabel2.Format(_T("%s-%s"), _T("Y"), AOIDataDefine.GetAxisText());
	strValue2.Format(_T("%d"), CountY);	
	InputBox.SetParam2(strTitle, strLabel1, strValue1, strLabel2, strValue2);
	if ( InputBox.DoModal() != IDOK ) 
	{	return false; }
	CountX = ::_ttof(InputBox.m_DataEdit1);
	CountY = ::_ttof(InputBox.m_DataEdit2);	
	if ( CountX<1 || CountY<1 ) 
	{	return false;	}
	double PitchX = RangeX;
	double PitchY = RangeY;	
	const int    CountAll=CountX*CountY;
	if ( CountX > 1 ) 
	{	PitchX = RangeX/(CountX-1);	}
	if ( CountY > 1 ) 
	{	PitchY = RangeY/(CountY-1);	}
	
	//轉成機台座標
	TPOINT2D BoxRectPt, StagePt;	
	switch ( LandToward )
	{
	case BOX_TOWARD_UP:
	case BOX_TOWARD_DOWN:		
		for ( i=0; i<CountY; i++ )
		{
			StagePt.y = (i*PitchY)+PosMinY;
			for ( j=0; j<CountX; j++ )
			{
				StagePt.x = (j*PitchX)+PosMinX;	
				if ( true == IsExceptionAngle )
				{		
					RotatePt1=StagePt;					
					JetAPI::RotatePos(AttachedAngle, CPX, CPY, RotatePt1);					
					StagePt = RotatePt1;					
				}
				BoxResultList.push_back(StagePt);
			}
		}
		break;

	default:
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		for ( i=0; i<CountX; i++ )
		{
			StagePt.x = (i*PitchX)+PosMinX;
			for ( j=0; j<CountY; j++ )
			{
				StagePt.y = (j*PitchY)+PosMinY;	
				if ( true == IsExceptionAngle )
				{		
					RotatePt1=StagePt;					
					JetAPI::RotatePos(AttachedAngle, CPX, CPY, RotatePt1);					
					StagePt = RotatePt1;					
				}
				BoxResultList.push_back(StagePt);
			}
		}				
		break;
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CalcModelLandCount_BGA_DIP(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore)//計算影像中的焊盤數量
{
	if ( NULL == ModelPtr ) { return false; }	
	const char fnName[] = "CEditModelView::CalcModelLandCount_BGA_DIP";

	size_t       i=0;
	TREGION4D    BoxRgn;
	TREGION4D    LandRgn;
	TREGION4D    LandSelRgn;
	size_t       LandCount = 0;
	CAOIBox     *BoxPtr = NULL;	
	CAOILand    *LandPtr = NULL;
	const size_t ModelLandCount = ModelPtr->GetModelLandCount();	
	LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr ) {	return false; }
	const double PadExtendX = 50;
	const double PadExtendY = 50;
	const double RoiExtendX = 250;
	const double RoiExtendY = 250;
	const int    LandGroupID = LandPtr->GetLandGroupID();
	const int    LandAlignID = LandPtr->GetLandAlignID();
	const BOX_TOWARD LandToward = LandPtr->GetLandToward();
	const TPOINT2D   FovCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();	
	
	LandPtr->GetLandBoxPtr()->GetBoxRegionStage(LandRgn);
	LandRgn.minX -= PadExtendX;
	LandRgn.minY -= PadExtendY;
	LandRgn.maxX += PadExtendX;
	LandRgn.maxY += PadExtendY;

	ModelPtr->GetModelBodyBoxPtr()->GetBoxRegionStage(LandSelRgn);
	LandSelRgn.minX -= RoiExtendX;
	LandSelRgn.minY -= RoiExtendY;
	LandSelRgn.maxX += RoiExtendX;
	LandSelRgn.maxY += RoiExtendY;	
	
	TREGION4D  ImagePatRgn;
	TREGION4D  ImageRoiRgn;
	MASK_PTR   MaskPtr  = NULL;
	IMAGE_PTR  ImagePtr = NULL;
	SPACE_PTR  SpacePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false )
	{	return false; }
	
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandRgn, FovCp, ImagePatRgn) == false )
	{	return false; }
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandSelRgn, FovCp, ImageRoiRgn) == false )
	{	return false; }

	CString    str;	
	RECT       PatRect={0};
	RECT       RoiRect={0};	
	IMAGE_PTR  PatPtr = NULL;
	IMAGE_PTR  RoiPtr = NULL;	
	IMAGE_SIZE PatW=0, PatH=0, PatStep=0;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;
	BOOL       bSaved=TRUE;
	
	JetAPI::Region4DToRect(ImagePatRgn, PatRect, false);
	PatW = PatRect.right-PatRect.left;
	PatH = PatRect.bottom-PatRect.top;
	PatStep = JetAPI::GetBMPImagePixelsPerLine(PatW, BitCount, 4);
	const size_t PatBufferSize = ImageAPI.CalcBufferSize(PatStep, PatH);
	
	JetAPI::Region4DToRect(ImageRoiRgn, RoiRect, false);
	RoiW = RoiRect.right-RoiRect.left;
	RoiH = RoiRect.bottom-RoiRect.top;
	RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
	const size_t RoiBufferSize = ImageAPI.CalcBufferSize(RoiStep, RoiH);
	
	if ( JetMemory.alloc_func(PatBufferSize, PatPtr, fnName, "PatPtr") == false || 
		 JetMemory.alloc_func(RoiBufferSize, RoiPtr, fnName, "RoiPtr") == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, PatRect, PatStep, PatPtr, false) == false || 
		 ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
#ifdef _DEBUG
	if ( TRUE == bSaved )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FovImg.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PatImg.BMP"));
		ImageAPI.SaveBMPImage(str, PatW, PatH, PatStep, BitCount, PatPtr, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RoiImg.BMP"));
		ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
	}
#endif//_DEBUG	

	CJetMatch  Match;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	Match.SetMatchLibType(MatchLibType);
	Match.SetMatchDefaultParam();
	Match.SetMaxPositions(10000);
	Match.SetMinScore(MinScore/100.0);
	if ( Match.LearnPattern(PatW, PatH, PatStep, BitCount, PatPtr, true) == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
	if ( Match.Match(RoiW, RoiH, RoiStep, BitCount, RoiPtr, true) == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
	JetMemory.free_func(PatPtr);
	JetMemory.free_func(RoiPtr);
	
	int       j=0;
	size_t    k=0;
	double    dPosX=0, dPosY=0;
	double    dPosX2=0, dPosY2=0;
	int       nPosX=0, nPosY=0;	
	size_t    BoxCount = 0;
	TRECT4D   BoxRect;
	std::vector<TRECT4D> BoxRectList;	
	const int PatCX = (PatRect.left+PatRect.right)/2;
	const int PatCY = (PatRect.top+PatRect.bottom)/2;
	const int PatW2 = (int)(PatW/2);
	const int PatH2 = (int)(PatH/2);
	const int PatMargin = 0;
	const int MatchResultCount = Match.GetNumPositions();

	//濾除重疊的位置
	BoxRectList.clear();
	for ( j=0; j<MatchResultCount; j++ )
	{
		dPosX = Match.GetResultPosX(j);
		dPosY = Match.GetResultPosY(j);		

		dPosX2 = dPosX+RoiRect.left;
		dPosY2 = dPosY+RoiRect.top;		

		BoxCount = BoxRectList.size();
		for ( k=0; k<BoxCount; k++ )
		{
			BoxRect = BoxRectList[k];
			if ( dPosX2<BoxRect.left || dPosX2>BoxRect.right || dPosY2<BoxRect.top || dPosY2>BoxRect.bottom )
			{	continue; }
			break;
		}
		if ( k < BoxCount ) { continue; }
		BoxRect.left   = dPosX2-PatW2+PatMargin;
		BoxRect.right  = dPosX2+PatW2-PatMargin;
		BoxRect.top    = dPosY2-PatH2+PatMargin;
		BoxRect.bottom = dPosY2+PatH2-PatMargin;
		BoxRectList.push_back(BoxRect);
	}

	//排序	
	bool   bForward=true;
	int Index=0, Index2=0;	
	std::vector<TRECT4D>  BoxRectList2;	
	std::vector<CSortObj> SortList;
	CSortObj              SortNode, *SortNodePtr=NULL, *SortNodePtr2=NULL;	
	MODEL_LAND_DIRECTION LandDirection = ModelPtr->GetModelLandDirection();
	SortList.clear();
	SortNode.SetSortMode(SORT_BY_INT);
	BoxCount = BoxRectList.size();		
	for ( i=0; i<BoxCount; i++ )
	{
		BoxRect = BoxRectList[i];
		SortNode.SetID(i);
		switch ( LandToward )
		{
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			SortNode.SetValueInt((int)(BoxRect.left));
			break;
		case BOX_TOWARD_UP:			
		case BOX_TOWARD_DOWN:
			SortNode.SetValueInt((int)(BoxRect.top));
			break;
		}
		SortList.push_back(SortNode);
	}
	//要依據Cad的座標來排序
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount = SortList.size();

	//每一層再來排序另一個方向
	bool   NewLine = true;
	int    CurValue=0;
	int    LastValue=0;
	int    GapValue=0;	
	size_t SortCount2=0;
	std::vector<CSortObj> SortList2;

	switch ( LandToward )
	{
	case BOX_TOWARD_LEFT:
	case BOX_TOWARD_RIGHT:
		GapValue = PatW;
		break;
	case BOX_TOWARD_UP:			
	case BOX_TOWARD_DOWN:
		GapValue = PatH;		
		break;
	default:
		GapValue = MIN(PatW, PatH);
		break;
	}
	bForward = true;
	if ( MODEL_LAND_CLOCKWISE == LandDirection )
	{
		if ( BOX_TOWARD_RIGHT==LandToward || BOX_TOWARD_DOWN==LandToward )
		{	bForward = false;	}		
	}
	else
	{
		if ( BOX_TOWARD_LEFT==LandToward || BOX_TOWARD_UP==LandToward )
		{	bForward = false;	}		
	}	
	for ( i=0; i<SortCount; i++ )
	{
		if ( false == bForward )
		{	SortNodePtr = &(SortList[i]);	}
		else
		{	SortNodePtr = &(SortList[SortCount-i-1]); }
		Index = SortNodePtr->GetID();
		BoxRect = BoxRectList[Index];
		if ( false == NewLine )
		{
			CurValue = SortNodePtr->GetValueInt();
			if ( abs(CurValue-LastValue) > GapValue )
			{	NewLine = true; }
		}
		if ( true == NewLine )
		{
			
			SortCount2 = SortList2.size();		
			if ( SortCount2 > 0 ) 
			{	std::sort(SortList2.begin(), SortList2.end()); }			
			if ( true == bForward )
			{
				for ( j=0; j<SortCount2; j++ )
				{
					SortNodePtr2 = &(SortList2[j]);
					Index2 = SortNodePtr2->GetID();
					BoxRectList2.push_back(BoxRectList[Index2]);
				}
			}
			else
			{
				for ( j=SortCount2-1; j!=-1; j-- )
				{
					SortNodePtr2 = &(SortList2[j]);
					Index2 = SortNodePtr2->GetID();
					BoxRectList2.push_back(BoxRectList[Index2]);
				}
			}
			SortList2.clear();			
			NewLine = false;
		}
		LastValue = SortNodePtr->GetValueInt();		
		SortNode.SetID(Index);		
		switch ( LandToward )
		{
		case BOX_TOWARD_LEFT:			
		case BOX_TOWARD_RIGHT:
			SortNode.SetValueInt((int)(BoxRect.top));
			break;
		case BOX_TOWARD_UP:			
		case BOX_TOWARD_DOWN:
			SortNode.SetValueInt((int)(BoxRect.left));
			break;
		}
		SortList2.push_back(SortNode);		
	}	
	SortCount2 = SortList2.size();	
	if ( SortCount2 > 0 ) 
	{	std::sort(SortList2.begin(), SortList2.end()); }	
	if ( true == bForward )
	{
		for ( j=0; j<SortCount2; j++ )
		{
			SortNodePtr2 = &(SortList2[j]);
			Index2 = SortNodePtr2->GetID();
			BoxRectList2.push_back(BoxRectList[Index2]);
		}
	}
	else
	{
		for ( j=SortCount2-1; j!=-1; j-- )
		{
			SortNodePtr2 = &(SortList2[j]);
			Index2 = SortNodePtr2->GetID();
			BoxRectList2.push_back(BoxRectList[Index2]);
		}			
	}		
	
	//轉成機台座標
	TPOINT2D BoxRectPt, StagePt;
	BoxCount = BoxRectList2.size();	
	for ( i=0; i<BoxCount; i++ )
	{
		BoxRect = BoxRectList2[i];
		BoxRectPt.x = (BoxRect.left+BoxRect.right);
		BoxRectPt.y = (BoxRect.top+BoxRect.bottom);
		BoxRectPt.x = BoxRectPt.x*0.5;
		BoxRectPt.y = BoxRectPt.y*0.5;
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, BoxRectPt, FovCp, StagePt);//相機影像對應到機台
		BoxResultList.push_back(StagePt);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CalcModelLandCount_Normal(CAOIModel *ModelPtr, std::vector<TPOINT2D> &BoxResultList, double &Pitch, double MinScore)//計算影像中的焊盤數量
{	
	if ( NULL == ModelPtr ) { return false; }	
	const char fnName[] = "CEditModelView::CalcModelLandCount_Normal";

	size_t       i=0;
	TREGION4D    BoxRgn;
	TREGION4D    LandRgn;
	TREGION4D    LandSelRgn;
	size_t       LandCount = 0;
	CAOIBox     *BoxPtr = NULL;	
	CAOILand    *LandPtr = NULL;
	const size_t ModelLandCount = ModelPtr->GetModelLandCount();	
	LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr ) {	return false; }
	const double PadExtendX = 50;
	const double PadExtendY = 50;
	const double RoiExtendX = 250;
	const double RoiExtendY = 250;
	const int    LandGroupID = LandPtr->GetLandGroupID();
	const int    LandAlignID = LandPtr->GetLandAlignID();
	const BOX_TOWARD LandToward = LandPtr->GetLandToward();
	const TPOINT2D   FovCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	for ( i=0; i<ModelLandCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(i, false);
		if ( NULL == LandPtr ) { continue; }
		if ( LandPtr->GetLandToward() != LandToward ) { continue; }
		if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }		
		if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
		if ( LandPtr->GetLandSelected() == false ) { continue; }

		BoxPtr = LandPtr->GetLandBoxPtr();
		BoxPtr->GetBoxRegionStage(BoxRgn);
		if ( 0 == LandCount )
		{	LandSelRgn = LandRgn = BoxRgn;	}
		else
		{	JetAPI::UnionRegion(LandSelRgn, BoxRgn, LandSelRgn);	}
		LandCount ++;
	}
	if ( LandCount < 2 )
	{
		//尋找單邊範圍
		LandCount = 0;		
		for ( i=0; i<ModelLandCount; i++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandToward() != LandToward ) { continue; }
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }			
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }

			BoxPtr = LandPtr->GetLandBoxPtr();
			BoxPtr->SetBoxSelected(true);
			BoxPtr->GetBoxRegionStage(BoxRgn);
			if ( 0 == LandCount )
			{	LandSelRgn = LandRgn = BoxRgn;	}
			else
			{	JetAPI::UnionRegion(LandSelRgn, BoxRgn, LandSelRgn);	}
			LandCount ++;
		}
		if ( LandCount < 2 ) { return false; }
	}
	LandRgn.minX -= PadExtendX;
	LandRgn.minY -= PadExtendY;
	LandRgn.maxX += PadExtendX;
	LandRgn.maxY += PadExtendY;

	LandSelRgn.minX -= RoiExtendX;
	LandSelRgn.minY -= RoiExtendY;
	LandSelRgn.maxX += RoiExtendX;
	LandSelRgn.maxY += RoiExtendY;
	
	TREGION4D  ImagePatRgn;
	TREGION4D  ImageRoiRgn;
	MASK_PTR   MaskPtr  = NULL;
	IMAGE_PTR  ImagePtr = NULL;
	SPACE_PTR  SpacePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false )
	{	return false; }
	
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandRgn, FovCp, ImagePatRgn) == false )
	{	return false; }
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, LandSelRgn, FovCp, ImageRoiRgn) == false )
	{	return false; }

	CString    str;	
	RECT       PatRect={0};
	RECT       RoiRect={0};	
	IMAGE_PTR  PatPtr = NULL;
	IMAGE_PTR  RoiPtr = NULL;	
	IMAGE_SIZE PatW=0, PatH=0, PatStep=0;
	IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;
	BOOL       bSaved=TRUE;
	
	JetAPI::Region4DToRect(ImagePatRgn, PatRect, false);
	PatW = PatRect.right-PatRect.left;
	PatH = PatRect.bottom-PatRect.top;
	PatStep = JetAPI::GetBMPImagePixelsPerLine(PatW, BitCount, 4);
	const size_t PatBufferSize = ImageAPI.CalcBufferSize(PatStep, PatH);
	
	JetAPI::Region4DToRect(ImageRoiRgn, RoiRect, false);
	RoiW = RoiRect.right-RoiRect.left;
	RoiH = RoiRect.bottom-RoiRect.top;
	RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
	const size_t RoiBufferSize = ImageAPI.CalcBufferSize(RoiStep, RoiH);
	
	if ( JetMemory.alloc_func(PatBufferSize, PatPtr, fnName, "PatPtr") == false || 
		 JetMemory.alloc_func(RoiBufferSize, RoiPtr, fnName, "RoiPtr") == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
	if ( ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, PatRect, PatStep, PatPtr, false) == false || 
		 ImageAPI.ExtractRoiImage3(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
#ifdef _DEBUG
	if ( TRUE == bSaved )
	{
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("FovImg.BMP"));
		ImageAPI.SaveBMPImage(str, ImageW, ImageH, ImageStep, BitCount, ImagePtr, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("PatImg.BMP"));
		ImageAPI.SaveBMPImage(str, PatW, PatH, PatStep, BitCount, PatPtr, true);

		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("RoiImg.BMP"));
		ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
	}
#endif//_DEBUG	

	CJetMatch  Match;
	JET_MATCH_LIB_TYPE MatchLibType = AOIDataCollect.GetMatchLibType();
	Match.SetMatchLibType(MatchLibType);
	Match.SetMatchDefaultParam();
	Match.SetMaxPositions(1000);
	Match.SetMinScore(MinScore/100.0);
	if ( Match.LearnPattern(PatW, PatH, PatStep, BitCount, PatPtr, true) == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
	if ( Match.Match(RoiW, RoiH, RoiStep, BitCount, RoiPtr, true) == false )
	{
		JetMemory.free_func(PatPtr);
		JetMemory.free_func(RoiPtr);
		return false;
	}
	JetMemory.free_func(PatPtr);
	JetMemory.free_func(RoiPtr);
	
	int       j=0;
	size_t    k=0;
	double    dPosX=0, dPosY=0;
	double    dPosX2=0, dPosY2=0;
	int       nPosX=0, nPosY=0;	
	size_t    BoxCount = 0;
	TRECT4D   BoxRect;
	std::vector<TRECT4D> BoxRectList;	
	const int PatCX = (PatRect.left+PatRect.right)/2;
	const int PatCY = (PatRect.top+PatRect.bottom)/2;
	const int PatW2 = (int)(PatW/2);
	const int PatH2 = (int)(PatH/2);
	const int PatMargin = 0;
	const int MatchResultCount = Match.GetNumPositions();

	BoxRectList.clear();
	for ( j=0; j<MatchResultCount; j++ )
	{
		dPosX = Match.GetResultPosX(j);
		dPosY = Match.GetResultPosY(j);		

		dPosX2 = dPosX+RoiRect.left;
		dPosY2 = dPosY+RoiRect.top;
		switch ( LandToward )
		{
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			dPosX2 = PatCX;
			break;
		case BOX_TOWARD_UP:			
		case BOX_TOWARD_DOWN:
			dPosY2 = PatCY;
			break;
		}

		BoxCount = BoxRectList.size();
		for ( k=0; k<BoxCount; k++ )
		{
			BoxRect = BoxRectList[k];
			if ( dPosX2<BoxRect.left || dPosX2>BoxRect.right || dPosY2<BoxRect.top || dPosY2>BoxRect.bottom )
			{	continue; }
			break;
		}
		if ( k < BoxCount ) { continue; }
		BoxRect.left   = dPosX2-PatW2+PatMargin;
		BoxRect.right  = dPosX2+PatW2-PatMargin;
		BoxRect.top    = dPosY2-PatH2+PatMargin;
		BoxRect.bottom = dPosY2+PatH2-PatMargin;
		BoxRectList.push_back(BoxRect);
	}

	//排序	
	int Index = 0;	
	std::vector<TRECT4D>  BoxRectList2;	
	std::vector<CSortObj> SortList;
	CSortObj              SortNode, *SortNodePtr=NULL;	
	MODEL_LAND_DIRECTION LandDirection = ModelPtr->GetModelLandDirection();
	SortList.clear();
	SortNode.SetSortMode(SORT_BY_INT);
	BoxCount = BoxRectList.size();		
	for ( i=0; i<BoxCount; i++ )
	{
		BoxRect = BoxRectList[i];
		SortNode.SetID(i);
		switch ( LandToward )
		{
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			SortNode.SetValueInt((int)(BoxRect.top));
			break;
		case BOX_TOWARD_UP:			
		case BOX_TOWARD_DOWN:
			SortNode.SetValueInt((int)(BoxRect.left));
			break;
		}
		SortList.push_back(SortNode);
	}
	//要依據Cad的座標來排序
	std::sort(SortList.begin(), SortList.end());
	const size_t SortCount = SortList.size();
	if ( MODEL_LAND_CLOCKWISE == LandDirection )
	{
		for ( i=0; i<SortCount; i++ )
		{
			SortNodePtr = &(SortList[i]);
			Index = SortNodePtr->GetID();
			BoxRectList2.push_back(BoxRectList[Index]);
		}
	}
	else
	{
		for ( i=SortCount-1; i!=-1; i-- )
		{
			SortNodePtr = &(SortList[i]);
			Index = SortNodePtr->GetID();
			BoxRectList2.push_back(BoxRectList[Index]);
		}			
	}	
	//轉成機台座標
	TPOINT2D BoxRectPt, StagePt;
	BoxCount = BoxRectList2.size();	
	for ( i=0; i<BoxCount; i++ )
	{
		BoxRect = BoxRectList2[i];
		BoxRectPt.x = (BoxRect.left+BoxRect.right);
		BoxRectPt.y = (BoxRect.top+BoxRect.bottom);
		BoxRectPt.x = BoxRectPt.x*0.5;
		BoxRectPt.y = BoxRectPt.y*0.5;
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, BoxRectPt, FovCp, StagePt);//相機影像對應到機台
		BoxResultList.push_back(StagePt);
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecPopupMenu(POINT point, UINT menuID)
{
	if ( menuID == 0 ) { return false; }
	if ( GetLockUIWnd() == true ) { return true; }

	CMenu menu;
	CPoint CtrlPt = point;
	VERIFY(menu.LoadMenu(menuID));
	AOIDataCollect.SwitchMultiLanguageMenu(menu, menuID);

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{	pWndPopupOwner = pWndPopupOwner->GetParent();	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::GetShowPopupMenu() const
{
	return m_ShowPopupMenu;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SetShowPopupMenu(bool val)
{
	m_ShowPopupMenu = val;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecPopupMenu_Auto(POINT point)
{
	return ExecPopupMenu(point, IDR_MENU_MODEL_AUTO_ADD);
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecPopupMenu_Add(POINT point)
{
	return ExecPopupMenu(point, IDR_MENU_MODEL_ADD);
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecPopupMenu_Edit(POINT point)
{
	return ExecPopupMenu(point, IDR_MENU_MODEL_EDIT);
}
//-------------------------------------------------------------------------------------//
CURSOR_POS_MODE CEditModelView::CheckCursorPosMode(POINT pt)//確認鼠標座標模式
{
	RECT         Rect;
	TRECT4D      dRect;
	SIZE         szGrid;
	double       Angle = 0;
	size_t       i=0;	
	CAOIBox      *BoxPtr = NULL;
	CURSOR_POS_MODE CursorMode = CURSOR_POS_NONE;	
	TActiveObj  *ObjPtr = NULL;		
	TActiveObj  *ObjPtrActived = NULL;	
	TPOINT2D     Cp;
	TPOINT2D     WndPt = pt;
	TPOINT2D     ImagePt, ImagePt2;
	TPOINT2D     CornerPoint[4];	
	POINT        ImagePoint;	
	double       ZoomScale = m_ImageZoom;		
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) 
	{	return CursorMode; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return CursorMode; }
	if ( ModelPtr->GetModelEditMode() == false ) { return CursorMode; }		
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) 
	{	return CursorMode; }
	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	const bool MultiSelectMode = AOIDataCollect.CheckMultiSelectMode();	
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();		
	const size_t NObjects = this->m_ActiveObjList.size();		

	szGrid.cx = GetEditCheckSize();
	szGrid.cy = GetEditCheckSize();	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt, ImagePt);	
	Cp.x = Cp.y = 0;
	if ( MANIPULATE_MODEL_EDIT == ManiMode )
	{
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( false == ObjPtr->GetEditabled() ) { continue; }
			if ( false == ObjPtr->GetSelected() ) { continue; }
			//if ( false == ObjPtr->GetFocused() ) { continue; }			

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
			if ( ObjPtr->IsExceptionAngle == false )
			{				
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);
				ImagePt2 = ImagePt;
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];				
				JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);
				JetAPI::Point2DToPoint(ImagePt2, ImagePoint);
				JetAPI::Rect4DToRect(dRect, Rect);				
			}

			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	break;	}
		}
	}
	else if ( MANIPULATE_MODEL_SELECT == ManiMode )
	{	
		ObjPtrActived = NULL;	
		for ( i=0; i<NObjects; i++ )
		{
			ObjPtr = &(m_ActiveObjList[i]);
			if ( false == ObjPtr->GetEditabled() ) { continue; }

			BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;			
			if ( ObjPtr->IsExceptionAngle == false )
			{	
				JetAPI::Point2DToPoint(ImagePt, ImagePoint);
				JetAPI::Rect4DToRect(ObjPtr->Rect, Rect);				
			}
			else
			{
				Angle = JetAPI::MapCadAngleToImageAngle(ObjPtr->ComponentAngle);
				ImagePt2 = ImagePt;
				CornerPoint[0] = ObjPtr->CornerPts[0];
				CornerPoint[1] = ObjPtr->CornerPts[1];
				CornerPoint[2] = ObjPtr->CornerPts[2];
				CornerPoint[3] = ObjPtr->CornerPts[3];
				JetAPI::RotateCornerPos(-Angle, Cp.x, Cp.y, CornerPoint);
				JetAPI::RotatePos(-Angle, Cp.x, Cp.y, ImagePt2);
				JetAPI::PointsToRect(CornerPoint, 4, dRect);
				JetAPI::Point2DToPoint(ImagePt2, ImagePoint);
				JetAPI::Rect4DToRect(dRect, Rect);				
			}

			CursorMode = JetAPI::CheckCursorPosMode(Rect, szGrid, ImagePoint);
			if( CURSOR_POS_NONE != CursorMode ) 
			{	
				if ( false == MultiSelectMode )
				{
					ModelPtr->UnSelectModel();
					ModelPtr->SetModelWndActived(NULL);
					ModelPtr->SetModelLandActived(NULL);
					ProjectPtr->SetProjectActiveModelWnd(NULL);
					ProjectPtr->SetProjectActiveModelLand(NULL);
					ResetActiveObjPosSelect();
				}
				
				ObjPtrActived = ObjPtr;
				ObjPtr->SetFocused(true);			
				ObjPtr->SetSelected(true);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	BoxPtr->SetBoxSelected(true);	}
				break; 
			}
		}
		if ( NULL!=ObjPtrActived && AOIDataCollect.CheckDoubleSideEdit() == false)
		{
			for ( i=0; i<NObjects; i++ )
			{
				ObjPtr = &(m_ActiveObjList[i]);
				if ( ObjPtr == ObjPtrActived ) { continue; }
				
				ObjPtr->SetFocused(false);
				BoxPtr = (CAOIBox*)ObjPtr->BoxPtr;
				if ( NULL != BoxPtr )
				{	BoxPtr->SetBoxSelected(false);	}				
			}
		}
	}
	if ( CURSOR_POS_NONE != CursorMode )
	{
		if ( AOIDataCollect.CheckMoveObjectMode() == true )
		{	CursorMode = CURSOR_POS_INNER; }
	}
	return CursorMode;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CheckIsLinkMode() const
{
	return true;
	//if ( TRUE == CWnd::IsDlgButtonChecked(MODELEDIT_EDIT_LINK_CHK) ) { return true; }
	//return false;	
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class		
	bool bRedraw=false;
	DRAW_MODEL_MODE DrawModel;
	switch ( pMsg->message )
	{	
	case WM_KEYDOWN:
		switch ( pMsg->wParam )
		{
		case VK_DELETE:
			OnModelEditDeleteSelect();
			break;
		case VK_LEFT:	
			bRedraw = ExecModifyActiveObjPosKernel(-1, 0);
			break;
		case VK_RIGHT:
			bRedraw = ExecModifyActiveObjPosKernel(1, 0);
			break;
		case VK_UP:
			bRedraw = ExecModifyActiveObjPosKernel(0, -1);
			break;
		case VK_DOWN:
			bRedraw = ExecModifyActiveObjPosKernel(0, 1);
			break;		
		}
		if ( true == bRedraw )
		{	RedrawWnd(); }		
		break;
	case WM_KEYUP:
		switch ( pMsg->wParam )
		{
		case VK_ESCAPE:
			AOIDataCollect.CancelGatherColorMode();
			AOIDataCollect.CancelManipulateMainMode();
			DrawModel = GetDrawModelMode();			
			if ( DRAW_MODEL_EDIT != DrawModel )
			{	OnShowModelEditMode();	}
			break;		
		default:
			if ( AOIDataCollect.GetCombineColorVrKey() == pMsg->wParam )
			{	AOIDataCollect.CancelGatherColorMode();	}
			break;
		}
		AOIDataCollect.SetProjectHasModified(pMsg->wParam, GetActiveProject(), m_ActiveObjList);
		if ( true == bRedraw )
		{	RedrawWnd();	}
		break;
	}	
	return CView::PreTranslateMessage(pMsg);
}
//-------------------------------------------------------------------------------------//
LRESULT CEditModelView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND  hWnd = NULL;
	CWnd *pWnd = NULL;	
	switch ( message )
	{		
	case MSG_MAIN_FRAME_MESSAGE:
		switch ( wParam )
		{
		case WPARAM_PROJECT_NEW:
		case WPARAM_PROJECT_OPEN:
		case WPARAM_PROJECT_SWITCH:		
			SwitchProject();			
			break;
		case WPARAM_PROJECT_UPDATE:
			if ( CWnd::IsWindowVisible() == TRUE )
			{
				pWnd = (CWnd*)lParam;
				if ( pWnd != this )
				{
					UpdateComponentSelected();
					RedrawWnd();  
				}
			}
			break;
		case WPARAM_PROJECT_CLOSE:		
			CloseProject();
			if ( CWnd::IsWindowVisible() == TRUE )
			{	RedrawWnd(); }
			break;
		case WPARAM_PROJECT_PART_SELECTED:
			UpdateComponentSelected();
			break;
		case WPARAM_PROJECT_PART_DELETED:
			UpdateComponentSelected();
			break;
		case WPARAM_CALC_CURRENT_FOV_POSITION:
			CalcFovPosition();
			break;
		case WPARAM_PROJECT_SWITCH_MARK:
			break;
		case WPARAM_PROJECT_CLOSE_ACTIVE_OBJ:			
			ResetModel();
			ReleaseUniFrameBuffer();
			ReleaseShowImageBuffer();
			break;
		}
		break;
	case MSG_EDIT_MAIN_VIEW_WND:
		switch ( wParam )
		{
		case WPARAM_SWITCH_FRAME_IMAGE:
			pWnd = (CWnd*)(lParam);
			if ( pWnd != this )
			{	UpdateFrameImage();	}
			break;
		case WPARAM_UPDATE_VIEW_PART_SELECTED:
			UpdateComponentSelected();
			break;
		case WPARAM_REDRAW_VIEW_WND:		
		case WPARAM_REDRAW_PROJECT_MAP:
			RedrawWnd();			
			hWnd = GetSafeHwnd();//否吃掉重複重繪訊息
			JetAPI::RemoveMessage(hWnd, MSG_EDIT_MAIN_VIEW_WND, MSG_EDIT_MAIN_VIEW_WND);
			break;
		case WPARAM_UPDATE_ALG_IMAGE:
			UpdateImageByAlgParam();			
			break;
		case WPARAM_EXEC_WND_INSPECT:
			ExecModelWndInspection(true);
			break;		
		case WPARAM_CALC_WND_COLOR:
			ExecCalcWndColor();
			break;
		case WPARAM_EXTRACT_WND_COLOR_FILTER:
			ExecExtractWndColorFilter();
			break;
		case WPARAM_SHOW_WND_POSITION:
			ExecShowWndPosition();
			break;
		case WPARAM_TOGGLE_ENCHANGE_IMAGE_MODE://切換強化影像模式
			ExecToggleEnhanceImageMode();			
			break;
		case WPARAM_SET_DRAW_PROJECT_MODE:
			CreateMapImage();
			break;
		case WPARAM_UPDATE_PROJECT_TEST_MAP:
			CreateTestMapImage();			
			break;
		case WPARAM_BUILD_RAW_MODEL_UNI_FRAME_LIST://建立原始模組通用影像列表
		{	
			const int  nAlign = 4;
			const bool bClone = false;
			const bool bNoFilter = true;
			CAOIModel *ModelPtr = (CAOIModel*)(lParam);
			std::vector<TUNI_FRAME> UniFrameList;			
			BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, bClone, bNoFilter);
		}
			break;
		}
		break;
	case MSG_CAMERA_CALLBACK:
		if ( AOIDataCollect.CheckCanRetrieveCameraImage() == false )
		{	break; }
		switch ( wParam )
		{
		case WPARAM_CAMERA_1_CALLBACK:			
		case WPARAM_CAMERA_2_CALLBACK:			
		case WPARAM_CAMERA_3_CALLBACK:			
		case WPARAM_CAMERA_4_CALLBACK:			
		case WPARAM_CAMERA_5_CALLBACK:
			if ( this->RetrieveCameraImage(wParam, lParam, true) == false )
			{	this->LockUIWnd(false);	}
			break;
		}
		break;	
	case MSG_CAMERA_REGRAB_IMAGE:
		ExecMoveToStage();
		break;
	case MSG_INSPECTION_CALLBACK:
		switch ( wParam )
		{		
		case WPARAM_INSPECTION_FINISH://檢測狀態-檢測結束
			ExecInspection_Finish();
			break;
		case WPARAM_INSPECTION_ONLINE_FINISH:
			ExecOnlineInspection_Finish();
			break;
		case WPARAM_INSPECTION_RE_TUNING://檢測狀態-重新調機
			if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
			{
				LockUIWnd(false);
				JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
				break;
			}
			break;
		case WPARAM_INSPECTION_RE_INSPECT://檢測狀態-重新檢測
			break;
		}		
		break;	
	case MSG_SYSTEM_EXCEPTION_CALLBACK:
		LockUIWnd(false);
		AOIDataCollect.ExecSystemException(wParam, lParam);		
		break;
	}
	return CView::WindowProc(message, wParam, lParam);
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::CreateModelROI(TUNI_FRAME UniFrameList[], const TPOINT2D &StageCP, const TPOINT2D &ImageRes, CAOIModel *ModelPtr, RECT &rect)//將Frame的影像列表轉成Model的影像列表
{
	if (NULL == ModelPtr) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return false; }

	CString                 str;
	bool                    Exception = false;
	size_t                  i = 0, j = 0;
	size_t                  UniFrameCount = 0;
	IMAGE_SIZE              ImageW = 0;
	IMAGE_SIZE              ImageH = 0;

	TPOINT2D                FovCp;
	TREGION4D               CadRgn;
	TREGION4D               ImageRgn;
	TREGION4D               StageRgn;
	RECT                    RoiRect = { 0 };

	const double ImageScaleX = 1.0 / ImageRes.x;
	const double ImageScaleY = 1.0 / ImageRes.y;
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	FovCp.x = StageCP.x;
	FovCp.y = StageCP.y;
	
	ModelPtr->GetModelTotalRegionStage(StageRgn);

	//ModelPtr->SetModelImageScale(TPOINT2D(ImageScaleX, ImageScaleY));

		ImageW = UniFrameList[0].ImageW;
		ImageH = UniFrameList[0].ImageH;
		if (AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, StageRgn, FovCp, ImageRgn) == false)
		{
			return false;
		}
		JetAPI::Region4DToRect(ImageRgn, RoiRect, false);
		//JetAPI::AdjustRectByAlignW(RoiRect, nAlign);//調成4倍寬
		if (RoiRect.left<0 || RoiRect.right<0 || RoiRect.top<0 || RoiRect.bottom<0 ||
			RoiRect.left >= ImageW || RoiRect.right >= ImageW || RoiRect.top >= ImageH || RoiRect.bottom >= ImageH)
		{
			return false;
		}

		rect = RoiRect;
		//if (0 == i)
		//{
		//	CAOIRgn *Rgn = NULL;
		//	TSIZE2D  ImageSizeUm;
		//	AOIDataCollect.MapImageSizeToReal(ImageRes, RoiRect, ImageSizeUm);
		//	StageRgn.SetSize(ImageSizeUm.cx, ImageSizeUm.cy);
		//	ModelPtr->SetModelImageSize_um(ImageSizeUm);
		//	Rgn = ModelPtr->GetModelAttachedPtr();
		//	if (NULL != Rgn)
		//	{
		//		Rgn->SetRgnFrameImageSize_um(ImageSizeUm);
		//	}
		//}

	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecMoveToStage()
{
	if ( this->GetLockUIWnd() == true ) { return true; }
	if ( MotionCtrlPtr->WaitForMotionStop() == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}
	
	bool   IsOK = true;
	double PosX=0, PosY=0, PosZ=0;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();		
	IsOK = MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ, OfflineMode);
	if ( IsOK == false )
	{
		JetAPI::ShowMessageBox(MotionCtrlPtr->GetErrorString());
		return false;
	}	
	ResetImageOffset();
	if ( true == OfflineMode )
	{	return ExecUpdateFov(PosX, PosY, PosZ);	}

	return ExecGrabFov(PosX, PosY, PosZ);
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecMoveToComponentStage()
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return false; }	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	if ( false == OfflineMode ) { return false; }	
	AOIDataCollect.MoveStageToComponentOrField(ComponentPtr, true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecShowWndPosition()
{	
	const double PosX = AOIDataCollect.GetFovPositionX();
	const double PosY = AOIDataCollect.GetFovPositionY();	
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	AOIDataCollect.ResetFovTargetParam();
	
	TPOINT2D PosCad;
	TPOINT2D FovOffset;
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();	

	//想要的位置與目前圖像的機台位置偏差量
	FovOffset.x = PosX-StageCp.x;
	FovOffset.y = PosY-StageCp.y;
	//疊加機台的偏差量
	PosStage.x += FovOffset.x;
	PosStage.y += FovOffset.y;
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);		
	m_ImageOffset.x = -PosCad.x/(ImageRes.x);
	m_ImageOffset.y =  PosCad.y/(ImageRes.y);
	m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
	m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);

	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecGrabFov(double PosX, double PosY, double PosZ)
{
	CString str;
	if ( this->GetLockUIWnd() == true ) { return true; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }		
	
	HWND hWnd = CWnd::GetSafeHwnd();
	AOIDataCollect.SetCallbackWnd(hWnd);	
	//AOIDataCollect.MoveCameraToProjectFocusPos(ProjectPtr);
#ifndef LIGHT_CTRL_DISABLE
	this->LockUIWnd(true);	
	if ( AOIDataCollect.ExecGrabNextUniFrameImage() == false )
	{		
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		this->LockUIWnd(false);
		return FALSE;
	}	
	return true;
#endif//LIGHT_CTRL_DISABLE
	return false;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecUpdateFov(double PosX, double PosY, double PosZ)
{	
	const double FovSizeW = AOIDataCollect.GetFovSizeRealW();
	const double FovSizeH = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double FovSizeWd2 = FovSizeW/2;
	const double FovSizeHd2 = FovSizeH/2;
	const double FovSizeWd4 = FovSizeW/4;
	const double FovSizeHd4 = FovSizeH/4;
	const double ZoomMin = AOIDataCollect.GetImageZoomMin();
	const double ZoomMax = AOIDataCollect.GetImageZoomMax();
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();
	const double FovZoomX = FovMinW/FovSizeW;
	const double FovZoomY = FovMinH/FovSizeH;
	const double FovZoomNeed = MAX(FovZoomX, FovZoomY);	
	const double FovZoomNeedUsed = JetAPI::AdjustValue(FovZoomNeed, 0.5);

	TPOINT2D ImageRes;
	IMAGE_SIZE ImageW = 0;
	IMAGE_SIZE ImageH = 0;
	IMAGE_SIZE ImageStep = 0;
	IMAGE_SIZE BitCount = 0;
	IMAGE_PTR  ImagePtr = NULL;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	
	if ( m_ImageZoom < FovZoomNeedUsed )
	{	m_ImageZoom = FovZoomNeedUsed; }
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr)==true && m_ImageWndRect.right>0 )
	{
		//視窗轉成影像
		TPOINT2D WndPt1, WndPt2;
		TPOINT2D ImagePt1, ImagePt2;
		TPOINT2D StagePt1, StagePt2;

		WndPt1.x = m_ImageWndRect.left;
		WndPt1.y = m_ImageWndRect.top;
		WndPt2.x = m_ImageWndRect.right;
		WndPt2.y = m_ImageWndRect.bottom;

		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt1, ImagePt1);	
		ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, WndPt2, ImagePt2);	

		//影像轉成機台
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt1, m_FOVPosStage, StagePt1);
		AOIDataCollect.MapCameraPtToStage(ImageW, ImageH, ImageRes, ImagePt2, m_FOVPosStage, StagePt2);
	
		double FovViewW = ::fabs(StagePt1.x-StagePt2.x);//FovSizeWd4;
		double FovViewH = ::fabs(StagePt1.y-StagePt2.y);//FovSizeHd4;
		if ( FovMinW>FovViewW || FovMinH>FovViewH )
		{
			FovViewW = FovSizeWd4;
			FovViewH = FovSizeHd4;			
			
			const double ZoomX = TargetMinW/FovViewW;
			const double ZoomY = TargetMinH/FovViewH;
			const double ZoomBigger = MAX(ZoomX, ZoomY);
			const double ZoomMin2 = AOIDataCollect.GetImageZoomMin_Act();
			m_ImageZoom = MIN(ZoomBigger, ZoomMax);
			m_ImageZoom = MAX(m_ImageZoom, ZoomMin2);
		}		
	}
	else
	{	m_ImageZoom = 1.0;	}

	//if ( m_ImageZoom > 1.0 )
	//{	m_ImageZoom = 1.0; }	

	//PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_MAP_DOCK_PANE, FALSE);		
	ResetImageOffset();
	m_FOVPosStage.x = PosX;
	m_FOVPosStage.y = PosY;	
	m_FrameResolution = ImageRes;		

	AOIDataCollect.ResetFovTargetParam();

	double Ratio = MAX(1.0, FovZoomNeedUsed);
	FillCurrentFrames(Ratio);	

	TPOINT2D PosCad;
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);	
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);	
	m_ImageOffset.x = -PosCad.x/(ImageRes.x);
	m_ImageOffset.y =  PosCad.y/(ImageRes.y);
	m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
	m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);

	UpdateComponentSelected();
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::AdjustCurrentFrames()
{
	//確認是否重新補圖
	RECT      WndImageRect={0};
	const IMAGE_SIZE ImageW = GetImageW();
	const IMAGE_SIZE ImageH = GetImageH();	
	const int RealWndW = m_ImageWndRect.right-m_ImageWndRect.left;
	const int RealWndH = m_ImageWndRect.bottom-m_ImageWndRect.top;
	const int RealWndCpx = RealWndW/2;
	const int RealWndCpy = RealWndH/2;
	const int WndSizeW = (int)(ImageW/m_ImageZoom);
	const int WndSizeH = (int)(ImageH/m_ImageZoom);		
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	IMAGE_SIZE BasicImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
	IMAGE_SIZE BasicImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);	

	double Ratio = 1;
	double RatioW = (double)(ImageW);
	double RatioH = (double)(ImageH);
	double dBImageW = (double)(BasicImageW);
	double dBImageH = (double)(BasicImageH);
	RatioW = RatioW/dBImageW;
	RatioH = RatioH/dBImageH;
			
	Ratio = MAX(RatioW, RatioH);
	if ( Ratio < 1.0 ) 
	{	Ratio = 1.0; }

	WndImageRect.left   = RealWndCpx+m_ImageOffset.x-(WndSizeW/2);
	WndImageRect.top    = RealWndCpy+m_ImageOffset.y-(WndSizeH/2);
	WndImageRect.right  = RealWndCpx+m_ImageOffset.x+(WndSizeW/2);
	WndImageRect.bottom = RealWndCpy+m_ImageOffset.y+(WndSizeH/2);

	if ( WndImageRect.left > m_ImageWndRect.left || 
		 WndImageRect.top > m_ImageWndRect.top ||
		 WndImageRect.right < m_ImageWndRect.right ||
		 WndImageRect.bottom < m_ImageWndRect.bottom )
	{	
		CalcFovPosition();
		if ( false == OfflineMode )
		{	CWnd::PostMessage(MSG_CAMERA_REGRAB_IMAGE, NULL, NULL);	}
		else
		{	
			FillCurrentFrames(Ratio);
			BuildShowImageBuffer();
			CreateBKImage();
			RedrawWnd();
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::FillCurrentFrames(double Ratio)
{
	size_t   i=0;
	TSIZE2D  Res;
	TPOINT3D Pos;	
	const size_t MaxFrames = CEditModelView::GetMaxFrameCount();
	IMAGE_SIZE ImageW = AOIDataCollect.GetCameraImageW(PRIMARY_CAMERA_ID);
	IMAGE_SIZE ImageH = AOIDataCollect.GetCameraImageH(PRIMARY_CAMERA_ID);
	const double FOVWum = Ratio*AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = Ratio*AOIDataCollect.GetFovSizeRealH();
	
	Pos.z = 0;
	Pos.x = m_FOVPosStage.x;
	Pos.y = m_FOVPosStage.y;

	Res.cx = m_FrameResolution.x;
	Res.cy = m_FrameResolution.y;

	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();

	m_FovRatio = Ratio;	
	m_FrameStageRgn.minX = Pos.x-(FOVWum*0.5);
	m_FrameStageRgn.maxX = Pos.x+(FOVWum*0.5);
	m_FrameStageRgn.minY = Pos.y-(FOVHum*0.5);
	m_FrameStageRgn.maxY = Pos.y+(FOVHum*0.5);

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	return true; }

	HCURSOR hCursor=NULL;
	HCURSOR hOldCursor=NULL;
	CWinApp *AppPtr = ::AfxGetApp();
	if ( NULL != AppPtr )
	{	
		hCursor = AppPtr->LoadStandardCursor(IDC_WAIT); 
		hOldCursor = ::SetCursor(hCursor);
	}		
		
	ImageW = (IMAGE_SIZE)(ImageW*Ratio);
	ImageH = (IMAGE_SIZE)(ImageH*Ratio);
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	if ( ProjectPtr->FillCurrentFrame(OfflineFileMode, Pos, Res, ImageW, ImageH, m_UniFrameList, MaxFrames) == false )
	{	JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());	}	
	
	m_ImageIndex = ProjectPtr->GetProjectMapIndex();
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		if ( NULL == m_UniFrameList[i].ImagePtr ) { continue; }		
		m_UniFrameList[i].ImageW = ImageW;
		m_UniFrameList[i].ImageH = ImageH;
	}
	AOIDataCollect.SetFieldUniFrameList(m_FrameStageRgn, m_UniFrameList, MaxFrames);	
	if ( NULL != hOldCursor )
	{	::SetCursor(hOldCursor);	}

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowBitCount = 24;	
	m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(m_ShowImageW, m_ShowBitCount, 4);	
	UpdateActiveObjList();		
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::PreInitUniFrameBuffer()//預先影像記憶體
{		
	IMAGE_SIZE ImageW = 1024;
	IMAGE_SIZE ImageH = 1024;

	m_ImageIndex = 0;
	m_FrameStageRgn.maxX = (ImageW/2)*m_FrameResolution.x;
	m_FrameStageRgn.maxY = (ImageH/2)*m_FrameResolution.y;
	m_FrameStageRgn.minX = -m_FrameStageRgn.maxX;
	m_FrameStageRgn.minY = -m_FrameStageRgn.maxY;
	JetAPI::InitialUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ReleaseUniFrameBuffer()//釋放影像記憶體
{
	AOIDataCollect.ReleaseFieldUniFrameList();
	JetAPI::ClearUniFrameList(m_UniFrameList, FRAME_MAX_COUNT);	
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::GetKeepImageOffset() const//取得是否保持影像偏移值
{
	return m_KeepImageOffset;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SetKeepImageOffset(bool val)//設定是否保持影像偏移值	
{
	m_KeepImageOffset = val;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::BuildShowImageBuffer()//建立顯示的影像記憶體
{
	const char fnName[] = "CEditModelView::BuildShowImageBuffer";
	CEditModelView::ReleaseShowImageBuffer();
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return; }
	if ( NULL == ImagePtr )
	{
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{	return;		}
		::memset(m_ShowImagePtr, 0x00, sizeof(IMAGE_DATA)*ShowBufferSize);
	}
	else
	{
		m_ShowBitCount = 24;
		m_ShowImageW = ImageW;
		m_ShowImageH = ImageH;
		m_ShowImageStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, m_ShowBitCount, 4);
		const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
		const size_t ShowBufferSize = ImageAPI.CalcBufferSize(m_ShowImageStep, m_ShowImageH);
		if ( JetMemory.alloc_func(ShowBufferSize, m_ShowImagePtr, fnName, "m_ShowImagePtr") == false )
		{	return;		}
		if ( ShowBufferSize == BufferSize )
		{	::memcpy(m_ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
		else
		{
			if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, m_ShowImageStep, m_ShowImagePtr, false) == false )
			{
				ReleaseShowImageBuffer();
				return;
			}
		}
	}
	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();
	if ( DRAW_IMAGE_BY_RAW != DrawImageMode )
	{
		AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_NORMAL);
		AOIDataCollect.ExecEnhanceDisplayImage(m_ShowImageW, m_ShowImageH, m_ShowImageStep, m_ShowBitCount, m_ShowImagePtr, m_ShowImagePtr);
	}
	else
	{	AOIDataCollect.SetDrawingImageMode(DRAW_IMAGE_BY_RAW);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ReleaseShowImageBuffer()//釋放影像記憶體
{
	if ( NULL != m_ShowImagePtr )
	{	JetMemory.free_func(m_ShowImagePtr); }
	m_ShowImageW = 1024;
	m_ShowImageH = 1024;
	m_ShowImageStep = 1024*3;
	m_ShowBitCount = 24;	
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::GetCurrentFrame(IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
{
	if ( GetShowProjectMapMode() == true ) { return false; }
	if ( m_ImageIndex<0 || m_ImageIndex>=FRAME_MAX_COUNT ) { return false; }

	const unsigned int idx = m_ImageIndex;
	ImageW = m_UniFrameList[idx].ImageW;
	ImageH = m_UniFrameList[idx].ImageH;
	ImageStep = m_UniFrameList[idx].ImageStep;
	BitCount = m_UniFrameList[idx].BitCount;
	ImagePtr = m_UniFrameList[idx].ImagePtr;
	SpacePtr = m_UniFrameList[idx].SpacePtr;
	MaskPtr = m_UniFrameList[idx].MaskPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::GetFrameImage(unsigned int Index, IMAGE_SIZE &ImageW, IMAGE_SIZE &ImageH, IMAGE_SIZE &ImageStep, IMAGE_SIZE &BitCount, IMAGE_PTR &ImagePtr, SPACE_PTR &SpacePtr, MASK_PTR &MaskPtr)//取得目前影像
{
	if ( Index<0 || Index>=FRAME_MAX_COUNT ) { return false; }

	const unsigned int idx = Index;
	ImageW = m_UniFrameList[idx].ImageW;
	ImageH = m_UniFrameList[idx].ImageH;
	ImageStep = m_UniFrameList[idx].ImageStep;
	BitCount = m_UniFrameList[idx].BitCount;
	ImagePtr = m_UniFrameList[idx].ImagePtr;
	SpacePtr = m_UniFrameList[idx].SpacePtr;
	MaskPtr = m_UniFrameList[idx].MaskPtr;	
	return true;
}
//-------------------------------------------------------------------------------------//
CAOIWnd* CEditModelView::GetWndPtr()
{
	CAOIModel *ModelPtr=GetModelPtr();
	if ( NULL == ModelPtr ) { return NULL; }
	return ModelPtr->GetModelWndActived();
}
//-------------------------------------------------------------------------------------//
CAOIModel* CEditModelView::GetModelPtr()
{
	//return &m_Model;
	return m_ModelPtr;
}
//-------------------------------------------------------------------------------------//
CAOIWndMask* CEditModelView::GetWndMaskPtr()
{
	CAOIWnd *WndPtr = GetWndPtr();
	if ( NULL == WndPtr ) { return NULL; }
	return WndPtr->GetWndMaskWndActived();
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::BuildModel()//建立模組
{	
	m_ActiveObjList.clear();
	m_ImageZoom = 1.0;
	ResetImageOffset();
	MODEL_TYPE ModelType = (MODEL_TYPE)(MODEL_TYPE_CHIP);//(JetAPI::GetComboxCurSelData(m_ModelTypeCombox));	
	return true;

	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }

	if ( ModelPtr->BuildModelType(ModelType) == false )
	{	return false; }	
	
	CEditModelView::UpdateModelStats();	
	CEditModelView::SwitchManiModelMode(MANIPULATE_MODEL_EDIT);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ResetModel()
{
	m_ActiveObjList.clear();
	m_ImageZoom = 1.0;	
	SetModel(NULL);
	ResetImageOffset();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelModeDefault();
	SwitchManiModelMode(ManiMode);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SetModel(CAOIModel *Ptr)
{
	m_ModelPtr = Ptr;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::UpdateComponentSelected()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = NULL;
	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();	
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelModeDefault();

	if ( NULL != pComponent )
	{	ModelPtr = pComponent->GetComponentModelPtr();	}

	SetModel(ModelPtr);
	UpdateModelStats();	
	SwitchManiModelMode(ManiMode);	
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
int CEditModelView::GetEditLineSize()//取得編輯線的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditLineSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
int CEditModelView::GetEditCheckSize()//取得編輯線比較的尺寸
{
	const int    LineSizeLevel = AOIDataCollect.GetSystemParameter().m_EditLineSizeLevel;
	return JetAPI::GetEditCheckSize(m_ImageZoom, LineSizeLevel);
}
//-------------------------------------------------------------------------------------//
DRAW_MODEL_MODE CEditModelView::GetDrawModelMode() const
{
	//return DRAW_MODEL_EDIT;
	return AOIDataCollect.GetDrawModelMode();	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::GetModelDrawParam(TMODEL_DRAW_PARAM &DrawParam)
{	
	const TPOINT2D &ImageRes = GetImageResolution();
	DrawParam.WndRect = m_ImageWndRect;
	DrawParam.ViewCP.x = DrawParam.ViewCP.y = 0;
	DrawParam.ResolutionX = ImageRes.x;
	DrawParam.ResolutionY = ImageRes.y;
	DrawParam.Scale = m_ImageZoom;	
	DrawParam.ViewOffsetX =  m_ImageOffset.x;
	DrawParam.ViewOffsetY =  -m_ImageOffset.y;		
	return;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::MapWndPtToImagePt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &WndPt, TPOINT2D &ImagePt)
{
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, WndPt, ImagePt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::MapImagePtToWndPt_DBL(IMAGE_SIZE ImageW, IMAGE_SIZE ImageH, const RECT &WndRect, const TPOINT2D &OffsetPt, double ZoomScale, const TPOINT2D &ImagePt, TPOINT2D &WndPt)
{
	ImageAPI.MapImagePtToWndPt_DBL(ImageW, ImageH, WndRect, OffsetPt, ZoomScale, ImagePt, WndPt);
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditSwitchOnlineView()
{
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_VIEW_ONLINE_FORMVIEW, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditSwitchOnlineView(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditSwitchEditMain()
{
	AOIDataCollect.PostMainFrameWndMessage(WM_COMMAND, ID_VIEW_EDIT_MAIN_VIEW, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditSwitchEditMain(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecSwitchFrame(int FrameIndex)//切換畫面
{		
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }	
	ProjectPtr->SetProjectMapIndex(FrameIndex);
	m_ImageIndex = ProjectPtr->GetProjectMapIndex();
	BuildShowImageBuffer();
	CreateBKImage();
	RedrawWnd();
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_SWITCH_FRAME, NULL);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame01()
{
	ExecSwitchFrame(0);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame01(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame02()
{
	ExecSwitchFrame(1);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame02(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame03()
{
	ExecSwitchFrame(2);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame03(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame04()
{
	ExecSwitchFrame(3);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame04(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame05()
{
	ExecSwitchFrame(4);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame05(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame06()
{
	ExecSwitchFrame(5);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame06(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame07()
{
	ExecSwitchFrame(6);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame07(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowFrame08()
{
	ExecSwitchFrame(7);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowFrame08(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnEditModelAdd() 
{
	// TODO: Add your command handler code here	
	if ( AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false ) {	return ; }
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_ADD);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateEditModelAdd(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	BOOL bEditMode = CheckInEditMode(true);		
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
	pCmdUI->SetCheck(MANIPULATE_MODEL_ADD==AOIDataCollect.GetManipulateModelMode());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnEditModelEdit() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_EDIT);
	AOIDataCollect.SetManipulateModelModeDefault(MANIPULATE_MODEL_EDIT);	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateEditModelEdit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(MANIPULATE_MODEL_EDIT==AOIDataCollect.GetManipulateModelMode());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnEditModelSelect() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_SELECT);
	AOIDataCollect.SetManipulateModelModeDefault(MANIPULATE_MODEL_SELECT);	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateEditModelSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(MANIPULATE_MODEL_SELECT==AOIDataCollect.GetManipulateModelMode());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnEditModelAutoAdd()
{
	if (AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false) { return; }
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_AUTO_ADD);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateEditModelAutoAdd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	BOOL bEditMode = CheckInEditMode(true);
	if (FALSE == bEditMode || true == bLockUIWnd)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
	pCmdUI->SetCheck(MANIPULATE_MODEL_AUTO_ADD == AOIDataCollect.GetManipulateModelMode());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAddLandElectrode() 
{
	// TODO: Add your command handler code here	
	ExecModelAddLand(LAND_TYPE_ELECTRODE);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelAddLandElectrode(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAddLandICLead() 
{
	// TODO: Add your command handler code here
	ExecModelAddLand(LAND_TYPE_IC_LEAD);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelAddLandICLead(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAddLandConLead()
{
	ExecModelAddLand(LAND_TYPE_CON_LEAD);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelAddLandConLead(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAddLandPad() 
{
	// TODO: Add your command handler code here
	ExecModelAddLand(LAND_TYPE_PAD);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelAddLandPad(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelSmartChart()
{
	OpenChart(0);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelSmartChart(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	BOOL bEditMode = CheckInEditMode(true);
	if (FALSE == bEditMode || true == bLockUIWnd)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelDataStatic()
{
	OpenChart(1);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnScanDataStatic()
{
	OpenChart(2);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OpenChart(int type)
{
	CAOIModel *baseModelPtr = GetModelPtr();

	CAOIModel *pModelPtr;
	if (NULL == baseModelPtr) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if (NULL == ProjectPtr) { return; }

	int count = baseModelPtr->GetModelWndActivedCount();
	int componeCount = 0;

	if (count == 1)
	{
		CAOIWnd  *activeWndPtr = baseModelPtr->GetModelWndActived();

		vector<CAOIComponent*> totalComponent;

		CAOIComponent *baseComponentPtr = baseModelPtr->GetModelComponentPtr();
		if (baseComponentPtr == NULL || baseComponentPtr->GetComponentModelIsolated()) { return; }
		CAOIComponent *pComponent;
		CString baseModelName = baseComponentPtr->GetComponentModelName();

		vector<CAOIWnd*> totalWndPtr;
		CAOIWnd *baseWndPtr = baseModelPtr->GetModelWndActived();
		CAOIWnd *pWndPtr;

		//CAlgParam &baseAlgParam = baseWndPtr->GetWndAlgParam();
		CAlgParam pAlgParam;

		bool ModelIsolated;
		int NComponents = ProjectPtr->GetProjectComponentCount();
		for (int i = 0; i < NComponents; i++)
		{
			//找出該模組結果
			std::string name;
			pComponent = ProjectPtr->GetProjectComponentPtr(i, false);
			if (NULL == pComponent) { continue; }
			if (baseModelName != pComponent->GetComponentModelName()) { continue; }


			pModelPtr = pComponent->GetComponentModelPtr();
			ModelIsolated = pComponent->GetComponentModelIsolated();
			if (NULL == pModelPtr || ModelIsolated == true) { continue; }


			pWndPtr = pModelPtr->GetModelWndPtr(baseWndPtr->GetWndIndex(), true);
			if (pWndPtr->GetWndBypassed()) { continue; }
			totalComponent.push_back(pComponent);
			totalWndPtr.push_back(pWndPtr);

			if (pWndPtr->GetWndLogicResultID() != RESULT_ID_OK && pWndPtr->GetWndLogicResultID() != RESULT_ID_NG)
			{
				continue;
			}
			componeCount++;
		}

		if (componeCount == 0 && type != 2) { return; }

		INT_PTR Ret = 0;

		CStaticChartWnd StaticChartWnd;
		StaticChartWnd.SetType(type);
		StaticChartWnd.SetProject(ProjectPtr);
		StaticChartWnd.SetComponent(totalComponent);
		StaticChartWnd.SetWndPtr(totalWndPtr, baseWndPtr);
		//if (!StaticChartWnd.isSuccess()) { return; }
		Ret = StaticChartWnd.DoModal();
		if (IDCANCEL == Ret || type == 2)
		{// cancel || 是2類型
			return;
		}

		bool UpdateSelected = false;
		TWND_PARAM_CHANGED_RESULT Changed;
		Changed.bParamChanged = true;
		Changed.bWndRgnChanged = false;
		Changed.bReBuildWndUI = false;

		//CAOIWnd *WndPtr = totalWndPtr[0];
		LPCTSTR SetText = AOIDataDefine.GetSetText();
		CString AlgText = baseWndPtr->GetWndAlgTypeText();

		LogOperCtrl.SaveLogModelWndOperate(baseWndPtr, SetText, AlgText, Changed.sValueName, Changed.sValueOld, Changed.sValueNew);
		//ExecWndParamChangedUpdate(ModelPtr, WndPtr, Changed);

		baseWndPtr->SetWndModified(true);
		baseWndPtr->SetWndUIUpated_Param(false);
		baseModelPtr->ApplyModelWnd(baseWndPtr);

		DRAW_MODEL_MODE DrawModelMode = AOIDataCollect.GetDrawModelMode();
		if (DRAW_MODEL_EDIT != DrawModelMode)
		{
			UpdateSelected = true;
			AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
		}

		if (true == UpdateSelected)
		{
			PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
		}
		SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(baseWndPtr));

		//AOIDataCollect.PostMainFrameWndMessage(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
		PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_REBUILD_WND_PARAM_LIST, NULL);
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAutoAddLandVer()
{
	ExecModelAutoAddLand(1);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAutoAddLandHor()
{
	ExecModelAutoAddLand(2);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelAutoAddLandBoth()
{
	ExecModelAutoAddLand(3);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelAutoAddLand(CCmdUI* pCmdUI)
{
	//bool bLockUIWnd = GetLockUIWnd();
	//BOOL bEditMode = CheckInEditMode(true);
	//if (FALSE == bEditMode || true == bLockUIWnd)
	//{
	//	pCmdUI->Enable(FALSE);
	//}
	//else
	//{
	//	pCmdUI->Enable(TRUE);
	//}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandCount(int nCountMode)
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( ModelPtr->GetModelEditMode() == false ) { return; }

	CAOIWnd  *WndPtr = ModelPtr->GetModelWndActived();
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{
		if ( NULL != WndPtr )
		{	LandPtr = WndPtr->GetWndLandPtr(); }
		if ( NULL == LandPtr )
		{	return; }		
		ModelPtr->SetModelLandActived(LandPtr);
	}
	
	CString str;
	CString strLandCount;
	CString strCaption, strLabel, strValue;	
	CInputBoxWnd InputWnd;
	int LandCount = ModelPtr->GetModelLandCountByToward(LandPtr, true);
	BOX_TOWARD LandToward = LandPtr->GetLandToward();
	const int LandAlignID = LandPtr->GetLandAlignID();
	const int LandGroupID = LandPtr->GetLandGroupID();		
	const MODEL_TYPE ModelType = ModelPtr->GetModelType();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	MANIPULATE_MODEL_MODE ManiMode = GetManiModelMode();
	AOIDataCollect.SetManipulateModelMode(MANIPULATE_MODEL_SELECT);	
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	bool   bArray = false;
	double MinScore = 50;
	double LandPitch = 0.0;	
	size_t BeforeLandCount = 0;
	std::vector<TPOINT2D> BoxResultList;

	if ( LAND_COUNT_MODE_ARRAY == nCountMode )
	{
		bArray = true;
		if ( CalcModelLandCount(ModelPtr, BoxResultList, LandPitch, MinScore, nCountMode) == false )
		{
			AOIDataCollect.SetManipulateModelMode(ManiMode);
			return ;
		}
		BeforeLandCount = BoxResultList.size();	
	}
	else
	{
		if ( true == IsExceptionAngle )
		{
			if ( 1 == LandCount )
			{
				//選取全部同向
				ModelPtr->UnSelectModel();
				ModelPtr->SetModelLandSelectedByLandGroupID(LandGroupID, -1, true);
				LandCount = ModelPtr->GetModelLandCountByToward(LandPtr, true);
				ModelPtr->SetModelLandActived(LandPtr);
			}
			BeforeLandCount = LandCount;			
		}
		else
		{	
			const double ScoreLSL=0;
			const double ScoreUSL=100;
			str = _T("Input Similarity Score");		
			strCaption = LoadMultiLanguageString(str, str);
			str = _T("Similarity");		
			str = LoadMultiLanguageString(str, str);		
			strLabel.Format(_T("%s [%.0f ~ %.0f]"), str, ScoreLSL, ScoreUSL);
			strValue.Format(_T("%.2f"), MinScore);
			InputWnd.SetParam1(strCaption, strLabel, strValue);
			if ( InputWnd.DoModal() == IDCANCEL )
			{	
				AOIDataCollect.SetManipulateModelMode(ManiMode);
				return ; 
			}
			MinScore = ::_ttof(InputWnd.m_DataEdit1);
			if ( MinScore > ScoreUSL ) { MinScore = ScoreUSL; }
			else if ( MinScore < ScoreLSL ) { MinScore = ScoreLSL; }					
			if ( CalcModelLandCount(ModelPtr, BoxResultList, LandPitch, MinScore, nCountMode) == false )
			{
				AOIDataCollect.SetManipulateModelMode(ManiMode);
				return ;
			}
			BeforeLandCount = BoxResultList.size();	
		}	
	}	
	int NewLandCount = 0;
	const size_t BoxResultCount = BoxResultList.size();	
	if ( LAND_COUNT_MODE_ARRAY != nCountMode )
	{
		strCaption = _T("Set Lead Number");
		strCaption = LoadMultiLanguageString(strCaption, strCaption);
		strLabel = _T("Number");
		strLabel = LoadMultiLanguageString(strLabel, strLabel);
		strLandCount.Format(_T("%d"), BeforeLandCount);
		InputWnd.SetParam1(strCaption, strLabel, strLandCount);
		if ( LAND_COUNT_MODE_1D == nCountMode )
		{	InputWnd.SetReadOnly(false, false);	}
		else
		{	InputWnd.SetReadOnly(true, true);	}
		if ( InputWnd.DoModal() == IDCANCEL ) 
		{
			AOIDataCollect.SetManipulateModelMode(ManiMode);
			return;  
		}
		if ( LAND_COUNT_MODE_1D == nCountMode )
		{	NewLandCount = ::_ttoi(InputWnd.m_DataEdit1); }
		else
		{	NewLandCount = (int)(BoxResultCount); }
	}
	else
	{	NewLandCount = (int)(BoxResultCount); }	
	if ( NewLandCount == LandCount ) 
	{
		AOIDataCollect.SetManipulateModelMode(ManiMode);
		return ; 
	}

	
	if ( ModelPtr->ModifyModelLandCount(LandPtr, NewLandCount, bArray) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandCount Fault"));
		AOIDataCollect.SetManipulateModelMode(ManiMode);
		return ;
	}
	LogOperCtrl.SaveLogModelLandCount(ModelPtr, LandPtr, NewLandCount);

	if ( BoxResultCount == NewLandCount )
	{
		int          NewLandAlignID = 0;
		size_t       i=0, BoxIndex=0;
		CAOILand    *LastLandPtr=NULL;
		TSIZE2D      LandSize;
		TPOINT2D     LandPos, dPosStage, BoxPos, dPosCad, dPosCadLast;
		const size_t ModelLandCount = ModelPtr->GetModelLandCount();

		BoxIndex = 0;
		NewLandAlignID = LandAlignID;
		for ( i=0; i<ModelLandCount; i++ )
		{
			LandPtr = ModelPtr->GetModelLandPtr(i, false);
			if ( NULL == LandPtr ) { continue; }
			if ( LandPtr->GetLandGroupID() != LandGroupID ) { continue; }
			if ( LandPtr->GetLandAlignID() != LandAlignID ) { continue; }
			if ( LandPtr->GetLandToward()  != LandToward ) { continue; }
			if ( LandPtr->GetLandSelected() == false ) { continue; }
			LastLandPtr = LandPtr;
			if ( BoxIndex < BoxResultCount )
			{
				BoxPos = BoxResultList[BoxIndex];
				LandPtr->GetLandBoxPtr()->GetBoxSize(LandSize);
				LandPtr->GetLandBoxPtr()->GetBoxPosStage(LandPos);				

				dPosStage.x = BoxPos.x-LandPos.x;
				dPosStage.y = BoxPos.y-LandPos.y;
				AOIDataCollect.MapStageOffsetPtToCad(dPosStage, dPosCad);
				LandPtr->MoveLand(dPosCad.x, dPosCad.y);

				if ( 0 == BoxIndex )
				{	dPosCadLast = dPosCad; }
				else
				{
					switch ( LandToward )
					{
					case BOX_TOWARD_UP:
					case BOX_TOWARD_DOWN:
						if ( fabs(dPosCad.y-dPosCadLast.y) > LandSize.cy ) 
						{	NewLandAlignID = NewLandAlignID+1;	}
						break;
					case BOX_TOWARD_LEFT:
					case BOX_TOWARD_RIGHT:
						if ( fabs(dPosCad.x-dPosCadLast.x) > LandSize.cx ) 
						{	NewLandAlignID = NewLandAlignID+1;	}
						break;
					}
					dPosCadLast = dPosCad;
				}
				LandPtr->SetLandAlignID(NewLandAlignID);

				BoxIndex ++;
			}
		}
		ModelPtr->SetModelLandActived(LastLandPtr);
		//ModelPtr->UpdateModelLandLeadID();
	}
	ModelPtr->UpdateModelWndRgnByLinkMode();	
	ModelPtr->SetModelModifiedCount(true);
	ModelPtr->SetModelNeedSaveFiles(true);
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->UpdateModelBodyToComponent();
	AOIDataCollect.SetManipulateModelMode(ManiMode);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();

	if ( LAND_COUNT_MODE_1D == nCountMode )
	{	OnModelEditLandPitch();	}
	/*
	if ( MODEL_TYPE_BGA==ModelType || MODEL_TYPE_DIP_LEAD==ModelType || true==bArray )
	{	
		//DoNothing
	}
	else	
	{	OnModelEditLandPitch(); }
	*/
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandCount() 
{
	// TODO: Add your command handler code here
	const int nCountMode = LAND_COUNT_MODE_1D;
	ExecModelEditLandCount(nCountMode);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandCount(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandCount2D() 
{
	// TODO: Add your command handler code here
	const int nCountMode = LAND_COUNT_MODE_2D;
	ExecModelEditLandCount(nCountMode);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandCount2D(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandArray()
{
	const int nCountMode = LAND_COUNT_MODE_ARRAY;
	ExecModelEditLandCount(nCountMode);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandArray(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandPitch() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }

	CString strLabel;
	CString strCaption;
	CString strLandPitch;
	CInputBoxWnd InputWnd;
	const bool SelectedOnly = false;
	const double LandPitch = ModelPtr->GetModelLandPitchByToward(LandPtr, SelectedOnly);

	strCaption = _T("Set Lead Pitch");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Pitch (um)");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strLandPitch.Format(_T("%.0f"), LandPitch);
	InputWnd.SetParam1(strCaption, strLabel, strLandPitch);
	if ( InputWnd.DoModal() == IDCANCEL ) 
	{	return;  }

	const double NewLandPitch = ::_tcstod(InputWnd.m_DataEdit1, NULL);
	if ( ::fabs(NewLandPitch-LandPitch) < 0.001 ) { return ; }

	if ( ModelPtr->ModifyModelLandPitch(LandPtr, NewLandPitch, SelectedOnly) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandPitch Fault"));
		return ;
	}
	LogOperCtrl.SaveLogModelLandPitch(ModelPtr, LandPtr, NewLandPitch);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandPitch(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandAlign()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }

	CString str;
	const bool SelectedOnly = true;
	str = _T("Do you want to align the land position ?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )	
	{	return;  }

	if ( ModelPtr->ModifyModelLandAlign(SelectedOnly) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandAlign Fault"));
		return ;
	}
	LogOperCtrl.SaveLogModelLandSelectedAlign(ModelPtr);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandAlign(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandIncludePadAlign()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }

	size_t  i=0;
	CString str;	
	DWORD   Res=0;
	bool    bInclude=false;
	str = _T("Do you want to include the land to pad align?");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|MB_DEFBUTTON3);
	if ( IDCANCEL == Res ) { return; }
	if ( IDYES == Res ) {  bInclude=true; }
	else { bInclude = false; }	
	
	if ( ModelPtr->ModifyModelLandIncludePadAlign(LandPtr, bInclude) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandIncludePadAlign Fault"));
		return ;
	}
	LogOperCtrl.SaveLogModelLandFunc(ModelPtr, NULL, _T("Modify Model Land Include Pad-Align"));
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandIncludePadAlign()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();	
	if ( NULL != LandPtr )
	{	
		ExecModelEditLandIncludePadAlign();
		return; 
	}
	return;	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandIncludePadAlign(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandIncludePartAlign()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }

	size_t  i=0;
	CString str;	
	DWORD   Res=0;
	bool    bInclude=false;
	str = _T("Do you want to include the land to part align?");
	str = LoadMultiLanguageString(str, str);
	Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL|MB_DEFBUTTON3);
	if ( IDCANCEL == Res ) { return; }
	if ( IDYES == Res ) {  bInclude=true; }
	else { bInclude = false; }	
	
	if ( ModelPtr->ModifyModelLandIncludePartAlign(LandPtr, bInclude) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandIncludePartAlign Fault"));
		return ;
	}
	LogOperCtrl.SaveLogModelLandFunc(ModelPtr, NULL, _T("Modify Model Land Include Part-Align"));
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditLandIncludePartAlign()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();	
	if ( NULL != LandPtr )
	{	
		ExecModelEditLandIncludePartAlign();
		return; 
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditLandIncludePartAlign(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditClonePaste() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D psOffset;
	BOX_TOWARD Toward;
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	psOffset.x = 2000;
	psOffset.y = 1000;
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		ModelPtr->CloneMoveModelLandSelected(psOffset.x, psOffset.y, false);		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{		
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{
			psOffset.x = 100;
			psOffset.y = 100;			
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }			
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->MoveModelWndMaskWndSelected(WndPtr, psOffset) == false )
			{	return; }					
			UpdateModelStats();
			RedrawWnd();
			return; 
		}

		Toward = WndPtr->GetWndToward();
		if ( true == IsExceptionAngle )
		{	Toward = JetAPI::RotateToward(-ComponentAngle, Toward);	}
		switch ( Toward )
		{
		case BOX_TOWARD_UP:				
		case BOX_TOWARD_DOWN:
			psOffset.y = 0;
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			psOffset.x = 0;
			break;			
		}		
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->MoveModelWndSelected(Toward, psOffset.x, psOffset.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditClonePaste(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneRotate090() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelLandSelected(90, BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{	
			CString str;
			str = _T("Error, Can not Rotate Mask Wnd 90");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return;		
		}

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr )
		{	BodyPos.x = BodyPos.y = 0;	}
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelWndSelected(90, BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneRotate090(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneRotate180() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelLandSelected(180, BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{			
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }			
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->RotateModelWndMaskWndSelected(WndPtr, 180) == false )
			{	return; }					
			UpdateModelStats();
			RedrawWnd();
			return; 
		}

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr )
		{	BodyPos.x = BodyPos.y = 0;	}
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelWndSelected(180, BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneRotate180(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneRotate270() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelLandSelected(270, BodyPos.x, BodyPos.y) == false )
		{	return; }		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{			
			CString str;
			str = _T("Error, Can not Rotate Mask Wnd 270");
			str = LoadMultiLanguageString(str, str);
			JetAPI::ShowMessageBox(str);
			return;	
		}

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr )
		{	BodyPos.x = BodyPos.y = 0;	}
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelWndSelected(270, BodyPos.x, BodyPos.y) == false )
		{	return; }		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneRotate270(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneMirrorXPos() 
{
	// TODO: Add your command handler code here	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;	
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->MirrorYModelLandSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{		
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{			
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }			
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->MirrorYModelWndMaskWndSelected(WndPtr) == false )
			{	return; }					
			UpdateModelStats();
			RedrawWnd();
			return; 
		}

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr )
		{	BodyPos.x = BodyPos.y = 0;	}
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->MirrorYModelWndSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneMirrorXPos(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneMirrorYPos() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->MirrorXModelLandSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{			
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }			
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->MirrorXModelWndMaskWndSelected(WndPtr) == false )
			{	return; }					
			UpdateModelStats();
			RedrawWnd();
			return; 
		}

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL != LandPtr )
		{	BodyPos.x = BodyPos.y = 0;	}
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->MirrorXModelWndSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneMirrorYPos(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneDiagonal() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->MirrorXModelLandSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		if ( ModelPtr->MirrorYModelLandSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr =  ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{	
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return ; }			
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if (  ModelPtr->MirrorXModelWndMaskWndSelected(WndPtr) == false )
			{	return; }			
			if (  ModelPtr->MirrorYModelWndMaskWndSelected(WndPtr) == false )
			{	return; }			
			UpdateModelStats();
			RedrawWnd();
			return;
		}

		if (  ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if (  ModelPtr->MirrorXModelWndSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		if (  ModelPtr->MirrorYModelWndSelected(BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneDiagonal(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneCorner4() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	const bool   bLinkMode = CheckIsLinkMode();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{		
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return ; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelLandSelected(90, BodyPos.x, BodyPos.y) == false )
		{	return; }
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return ; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelLandSelected(90, BodyPos.x, BodyPos.y) == false )
		{	return; }
		if ( ModelPtr->CloneModelLandSelected() == false )
		{	return ; }
		LogOperCtrl.SaveLogModelLandSelectedClone(ModelPtr);
		if ( ModelPtr->RotateModelLandSelected(90, BodyPos.x, BodyPos.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != WndMaskPtr )
		{	
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->MirrorXModelWndMaskWndSelected(WndPtr) == false )
			{	return; }			
			WndMaskPtr = WndPtr->GetWndMaskWndActived();
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }		
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->MirrorYModelWndMaskWndSelected(WndPtr) == false )
			{	return; }			
			WndMaskPtr = WndPtr->GetWndMaskWndActived();
			if ( ModelPtr->ClonePasteModelWndMaskWnd(WndPtr, WndMaskPtr) == false )
			{	return; }		
			LogOperCtrl.SaveLogWndMaskSelectedClone(WndPtr);
			if ( ModelPtr->MirrorXModelWndMaskWndSelected(WndPtr) == false )
			{	return; }			
			UpdateModelStats();
			RedrawWnd();			
			return; 
		}

		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
			{	return ; }
			LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
			if ( ModelPtr->MirrorXModelWndSelected(BodyPos.x, BodyPos.y) == false )
			{	return; }
			if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
			{	return ; }
			LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
			if ( ModelPtr->MirrorYModelWndSelected(BodyPos.x, BodyPos.y) == false )
			{	return; }
			if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
			{	return ; }	
			LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
			if ( ModelPtr->MirrorXModelWndSelected(BodyPos.x, BodyPos.y) == false )
			{	return; }			
			UpdateModelStats();
			RedrawWnd();
			SendOutUpdatePartList();
		}
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneCorner4(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteSelect() 
{
	// TODO: Add your command handler code here	
	CString    strWnd;	
	CString    strIndex;	
	CString    str, str1;	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }

	CString strComponentName;
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();

	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	strIndex = AOIDataDefine.GetIndexText();
	if ( NULL != LandPtr )
	{
		str = _T("Do you want to delete the land");
		str = LoadMultiLanguageString(str, str);
		str1.Format(_T("%s[%s:%d]?"), str, strIndex, LandPtr->GetLandIndex()+1);
		if ( NULL != ComponentPtr )
		{	
			strComponentName = ComponentPtr->GetComponentFullName();
			str.Format(_T("%s\n%s"), strComponentName, str1); 
			str1 = str;
		}
		if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
		{	return; }
		LogOperCtrl.SaveLogModelLandSelectedDelete(ModelPtr);
		ModelPtr->DeleteModelLandSelected();		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{	
		ALG_TYPE AlgType = WndPtr->GetWndAlgType();		
		strWnd.Format(_T("Wnd:%d"), WndPtr->GetWndIndex()+1);
		CAOIWndRoi *WndRoiPtr = WndPtr->GetWndRoiWndActived();		
		const bool bWndRoiDeleted=CAOIModel::CheckModelWndRoiCanBeDeleted(AlgType);
		if ( NULL != WndRoiPtr )
		{
			if ( true == bWndRoiDeleted )
			{
				str = _T("Do you want to delete the roi wnd");
				str = LoadMultiLanguageString(str, str);
				str1.Format(_T("%s[%s, %s:%d]?"), str, strWnd, strIndex, WndRoiPtr->GetWndRoiIndex()+1);
				if ( NULL != ComponentPtr )
				{	
					strComponentName = ComponentPtr->GetComponentFullName();
					str.Format(_T("%s\n%s"), strComponentName, str1); 
					str1 = str;
				}
				if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
				{	return; }
				LogOperCtrl.SaveLogWndRoiSelectedDelete(WndPtr);
				ModelPtr->DeleteModelWndRoiWnd(WndPtr);
				CAlgParam &AlgParam = WndPtr->GetWndAlgParam();
				AlgParam.SetAlgBinParamActived(BIN_PARAM_BELONG_TO_ALG_IMAGE);
				//PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);
				UpdateModelStats();
				RedrawWnd();
				SendOutUpdatePartList();
			}
			return;
		}
		CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
		if ( NULL != MaskWndPtr )
		{
			str = _T("Do you want to delete the mask box");
			str = LoadMultiLanguageString(str, str);
			str1.Format(_T("%s[%s, %s:%d]?"), str, strWnd, strIndex, MaskWndPtr->GetWndMaskIndex()+1);			
			if ( NULL != ComponentPtr )
			{	
				strComponentName = ComponentPtr->GetComponentFullName();
				str.Format(_T("%s\n%s"), strComponentName, str1); 
				str1 = str;
			}
			if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
			{	return; }

			LogOperCtrl.SaveLogWndMaskSelectedDelete(WndPtr);
			ModelPtr->DeleteModelWndMaskWnd(WndPtr);			
			//PostMessageToMainFrameWnd(MSG_EDIT_MAIN_VIEW_WND, WPARAM_UPDATE_VIEW_PART_SELECTED, NULL);	
			UpdateModelStats();
			RedrawWnd();
			SendOutUpdatePartList();
			return;
		}

		str = _T("Do you want to delete the wnd");
		str = LoadMultiLanguageString(str, str);
		str1.Format(_T("%s[%s:%d]?"), str, strIndex, WndPtr->GetWndIndex()+1);
		if ( NULL != ComponentPtr )
		{	
			strComponentName = ComponentPtr->GetComponentFullName();
			str.Format(_T("%s\n%s"), strComponentName, str1); 
			str1 = str;
		}
		if ( JetAPI::ShowMessageBox(str1, MB_YESNO) != IDYES )
		{	return; }

		LandPtr = WndPtr->GetWndLandPtr();
		const bool   bLinkMode = CheckIsLinkMode();
		LogOperCtrl.SaveLogModelWndSelectedDelete(ModelPtr);
		ModelPtr->DeleteModelWndSelected(bLinkMode);
		if ( NULL == LandPtr )
		{
			CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
			BoxPtr->SetBoxSelected(true);
		}
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}		
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteSelect(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteOthers() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }

	CString str, str1;
	CString strComponentName;
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	
	const bool   bLinkMode = CheckIsLinkMode();
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{
		str = _T("Do you want to delete the others lands in the model?");
		str = LoadMultiLanguageString(str, str);
		if ( NULL != ComponentPtr )
		{	
			strComponentName = ComponentPtr->GetComponentFullName();
			str1.Format(_T("%s\n%s"), strComponentName, str); 
			str = str1;
		}
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return; }

		const int LandGroupID = LandPtr->GetLandGroupID();
		ModelPtr->UnSelectModelLand(true);
		ModelPtr->SetModelLandSelectedByLandGroupID(LandGroupID, -1, true);
		LandPtr->SetLandAllBoxSelected(false);
		LogOperCtrl.SaveLogModelLandSelectedDelete(ModelPtr);
		ModelPtr->DeleteModelLandSelected();
		LandPtr->GetLandBoxPtr()->SetBoxActived(true);
		LandPtr->GetLandBoxPtr()->SetBoxSelected(true);		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{	
		str = _T("Do you want to delete the others wnds in the model?");
		str = LoadMultiLanguageString(str, str);
		if ( NULL != ComponentPtr )
		{	
			strComponentName = ComponentPtr->GetComponentFullName();
			str1.Format(_T("%s\n%s"), strComponentName, str); 
			str = str1;
		}
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return; }

		const int WndBandID = WndPtr->GetWndBandID();
		const int WndGroupID = WndPtr->GetWndGroupID();
		ModelPtr->UnSelectModelWnd();
		ModelPtr->SetModelWndSelectedByWndGroupID(WndGroupID, -1, true);
		ModelPtr->SetModelWndSelectedByWndGroupID(WndGroupID, WndBandID, false);		
		LogOperCtrl.SaveLogModelWndSelectedDelete(ModelPtr);
		ModelPtr->DeleteModelWndSelected(bLinkMode);		
		WndPtr->SetWndActived(true);
		WndPtr->SetWndSelected(true);
		ModelPtr->SetModelWndActived(WndPtr);
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}		
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteOthers(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteGroup() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }

	CString str, str1;
	CString strComponentName;
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{
		str = _T("Do you want to delete the same group lands in the model?");
		str = LoadMultiLanguageString(str, str);
		if ( NULL != ComponentPtr )
		{	
			strComponentName = ComponentPtr->GetComponentFullName();
			str1.Format(_T("%s\n%s"), strComponentName, str); 
			str = str1;
		}
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return; }

		const int LandGroupID = LandPtr->GetLandGroupID();
		ModelPtr->UnSelectModelLand(true);
		ModelPtr->SetModelLandSelectedByLandGroupID(LandGroupID, -1, true);		
		LogOperCtrl.SaveLogModelLandSelectedDelete(ModelPtr);
		ModelPtr->DeleteModelLandSelected();		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{	
		str = _T("Do you want to delete the same group wnds in the model?");
		str = LoadMultiLanguageString(str, str);
		if ( NULL != ComponentPtr )
		{	
			strComponentName = ComponentPtr->GetComponentFullName();
			str1.Format(_T("%s\n%s"), strComponentName, str); 
			str = str1;
		}
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return; }

		LandPtr = WndPtr->GetWndLandPtr();
		const int WndBandID = WndPtr->GetWndBandID();
		const int WndGroupID = WndPtr->GetWndGroupID();
		ModelPtr->UnSelectModelWnd();
		ModelPtr->SetModelWndSelectedByWndGroupID(WndGroupID, -1, true);		
		LogOperCtrl.SaveLogModelWndSelectedDelete(ModelPtr);
		ModelPtr->DeleteModelWndSelected(true);

		if ( NULL == LandPtr )
		{
			CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
			BoxPtr->SetBoxSelected(true);
		}

		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteGroup(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteAll() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }

	CString str, str1;
	CString strComponentName;
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	str = _T("Do you want to clear all boxes in model?");
	str = LoadMultiLanguageString(str, str);
	if ( NULL != ComponentPtr )
	{	
		strComponentName = ComponentPtr->GetComponentFullName();
		str1.Format(_T("%s\n%s"), strComponentName, str); 
		str = str1;
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
		
	ModelPtr->ClearModelAllObjList();
	ModelPtr->RemoveModelImageFolder();//先行清除舊有的樣版資料夾
	ModelPtr->CalcModelTotalRegionAll();	
	ModelPtr->UpdateModelBodyToComponent();
	ModelPtr->SetModelChipSizeMode(CHIP_SIZE_NONE);
	LogOperCtrl.SaveLogModelContent(ModelPtr, _T("Clear All"));

	CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	BoxPtr->SetBoxSelected(true);

	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();	
	SendOutUpdatePartList();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteAll(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteAllWnd() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }

	CString str, str1;
	CString strComponentName;
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	
	str = _T("Do you want to clear all wnds in model?");
	str = LoadMultiLanguageString(str, str);
	if ( NULL != ComponentPtr )
	{			
		strComponentName = ComponentPtr->GetComponentFullName();
		str1.Format(_T("%s\n%s"), strComponentName, str); 
		str = str1;
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }
	
	ModelPtr->ClearModelFolder(MODEL_CLEAR_FOLDER_PATTERNS);
	ModelPtr->ClearModelAllWndList();	
	LogOperCtrl.SaveLogModelContent(ModelPtr, _T("Clear All Wnd"));

	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();

	CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	BoxPtr->SetBoxSelected(true);

	RedrawWnd();	
	SendOutUpdatePartList();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteAllWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteDerivative()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }

	CString str, str1;
	CString strComponentName;
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();	
	str = _T("Do you want to keep basic wnds in model?");
	str = LoadMultiLanguageString(str, str);
	if ( NULL != ComponentPtr )
	{			
		strComponentName = ComponentPtr->GetComponentFullName();
		str1.Format(_T("%s\n%s"), strComponentName, str); 
		str = str1;
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }	
	
	ModelPtr->ClearModelAllDerivative();	
	LogOperCtrl.SaveLogModelContent(ModelPtr, _T("Clear All Derivative"));

	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();

	CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	BoxPtr->SetBoxActived(true);
	BoxPtr->SetBoxSelected(true);	

	RedrawWnd();	
	SendOutUpdatePartList();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteDerivative(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteComponent()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelComponent() == false ) { return ; }
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false )
	{	return; }

	CString str, str2;
	CString FullComponentName;
	FullComponentName = ComponentPtr->GetComponentFullName();
	str = _T("Do you want to delete the component");
	str = LoadMultiLanguageString(str, str);
	str2.Format(_T("%s[%s]?"), str, FullComponentName);
	if ( JetAPI::ShowMessageBox(str2, MB_YESNO) == IDNO )
	{	return; }
	
	AOIDataCollect.ReleaseModelUniFrameList();
	ProjectPtr->SelectProjectAllComponents(false);
	ComponentPtr->SetComponentSelected(true);
	LogOperCtrl.SaveLogProjectComponentSelectedDelete(ProjectPtr);
	ProjectPtr->DeleteProjectComponentSelected();

	UpdateComponentSelected();
	SendOutUpdatePartList(MSG_MODE_BUILD, MSG_MODE_BUILD);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteComponent(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDeleteGroupLibrary()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncDelModelWnd() == false ) { return ; }
	if ( ProjectPtr->CheckProjectComponentValid(ComponentPtr) == false )
	{	return; }

	CString str, str1;
	CString strComponentName;	
	str = _T("Do you want to delete the group wnds in library?");
	str = LoadMultiLanguageString(str, str);
	if ( NULL != ComponentPtr )
	{			
		strComponentName = ComponentPtr->GetComponentFullName();
		str1.Format(_T("%s\n%s"), strComponentName, str); 
		str = str1;
	}
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return; }	
	
	ProjectPtr->DeleteProjectLibraryGroupWnd(ModelPtr);
	if ( NULL != ComponentPtr )
	{	ComponentPtr->SetComponentSelected(true); }
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();

	CAOIBox *BoxPtr = ModelPtr->GetModelBodyBoxPtr();
	BoxPtr->SetBoxActived(true);
	BoxPtr->SetBoxSelected(true);	

	RedrawWnd();	
	SendOutUpdatePartList();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDeleteGroupLibrary(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//

void CEditModelView::OnModelEditModifyPos()
{
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditModifyPos(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditModifySize()
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	
	CAOILand    *LandPtr = NULL;
	CAOIBox     *BoxPtr = NULL;		
	CAOIWndRoi  *WndRoiPtr = NULL;
	CAOIWndMask *WndMaskPtr = NULL;
	CAOIWnd     *WndPtr = ModelPtr->GetModelWndActived();
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	
	if ( NULL != WndPtr )
	{	
		if ( NULL == BoxPtr )
		{
			WndRoiPtr = WndPtr->GetWndRoiWndActived();
			if ( NULL != WndRoiPtr )
			{	BoxPtr = WndRoiPtr->GetWndRoiBoxPtr();	}
		}
		
		if ( NULL == BoxPtr )
		{
			WndMaskPtr = WndPtr->GetWndMaskWndActived();
			if ( NULL != WndMaskPtr )
			{	BoxPtr = WndMaskPtr->GetWndMaskBoxPtr(); }
		}

		if ( NULL == BoxPtr )
		{	BoxPtr = WndPtr->GetWndBoxPtr();	}		
		LandPtr = WndPtr->GetWndLandPtr();
	}	
	
	if ( NULL == BoxPtr )
	{		
		LandPtr = ModelPtr->GetModelLandActivted();	
		if ( NULL != LandPtr )
		{	BoxPtr = LandPtr->GetLandBasicBoxPtrSelected();	}
	}

	if ( NULL == BoxPtr ) 
	{
		BoxPtr = ModelPtr->GetModelBodyBoxPtr();
		if ( BoxPtr->GetBoxSelected() == false )
		{	return; }
	}

	CString   strTitle;
	CString   strLabelW;	
	CString   strLabelH;
	CString   strValueW;
	CString   strValueH;	
	CAOIBox   BoxObj;	
	TREGION4D Region;	
	TREGION4D dRegion;	
	double    PosX=0, PosY=0;
	double    SizeW=0, SizeH=0;	
	CInputBoxWnd InputBox;
	
	strLabelW = AOIDataDefine.GetSizeXText();
	strLabelH = AOIDataDefine.GetSizeYText();
	strTitle = _T("Input Box Size");	
	//strTitle = LoadMultiLanguageString(strTitle, strTitle);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);

	BoxObj = *BoxPtr;
	if ( true == IsExceptionAngle )
	{
		PosX = BoxObj.GetBoxPosX();
		PosY = BoxObj.GetBoxPosY();
		BoxObj.RotateBox(-AttachedAngle, PosX, PosY); 
	}
	BoxObj.GetBoxRegion(Region);
	SizeW = Region.GetWidth();
	SizeH = Region.GetHeight();
	strValueW.Format(_T("%.0f"), SizeW);
	strValueH.Format(_T("%.0f"), SizeH);
	InputBox.SetParam2(strTitle, strLabelW, strValueW, strLabelH, strValueH);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return; }
	strValueW = InputBox.m_DataEdit1;	
	strValueH = InputBox.m_DataEdit2;

	double NewSizeW = ::_ttof(strValueW);
	double NewSizeH = ::_ttof(strValueH);
	const int LinkMode = ModelPtr->GetModelWndLinkMode();

	dRegion.minX = -(NewSizeW-SizeW)/2;
	dRegion.maxX =  (NewSizeW-SizeW)/2;
	dRegion.minY = -(NewSizeH-SizeH)/2;
	dRegion.maxY =  (NewSizeH-SizeH)/2;
	
	ModelPtr->ModifyModelBoxSize(BoxPtr, WndPtr, LandPtr, WndRoiPtr, WndMaskPtr, dRegion, LinkMode);
	UpdateActiveObjList();	
	RedrawWnd();
	return;	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditModifySize(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditRotate090() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{		
		ModelPtr->SpinModelLandSelected(90);
		//ModelPtr->RotateModelLandSelected(90, BodyPos.x, BodyPos.y);
		LogOperCtrl.SaveLogModelLandSelectedRotate(ModelPtr, 90);
		UpdateModelStats();		
		RedrawWnd();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			//ModelPtr->RotateModelWndSelected(90, BodyPos.x, BodyPos.y);
			ModelPtr->SpinModelWndSelected(90);
			LogOperCtrl.SaveLogModelWndSelectedRotate(ModelPtr, 90);
			UpdateModelStats();
			RedrawWnd();
			return;
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditRotate090(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditRotate180() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{		
		ModelPtr->SpinModelLandSelected(180);
		//ModelPtr->RotateModelLandSelected(180, BodyPos.x, BodyPos.y);	
		LogOperCtrl.SaveLogModelLandSelectedRotate(ModelPtr, 180);
		UpdateModelStats();
		RedrawWnd();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			//ModelPtr->RotateModelWndSelected(180, BodyPos.x, BodyPos.y);
			ModelPtr->SpinModelWndSelected(180);
			LogOperCtrl.SaveLogModelWndSelectedRotate(ModelPtr, 180);
			UpdateModelStats();
			RedrawWnd();
			return;
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditRotate180(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditRotate270() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{		
		ModelPtr->SpinModelLandSelected(270);
		//ModelPtr->RotateModelLandSelected(270, BodyPos.x, BodyPos.y);		
		LogOperCtrl.SaveLogModelLandSelectedRotate(ModelPtr, 270);
		UpdateModelStats();
		RedrawWnd();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			//ModelPtr->RotateModelWndSelected(270, BodyPos.x, BodyPos.y);
			ModelPtr->SpinModelWndSelected(270);
			LogOperCtrl.SaveLogModelWndSelectedRotate(ModelPtr, 270);
			UpdateModelStats();
			RedrawWnd();
			return;
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditRotate270(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMirrorXPos() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{		
		ModelPtr->MirrorYModelLandSelected(BodyPos.x, BodyPos.y);		
		LogOperCtrl.SaveLogModelLandSelectedMirrorY(ModelPtr);
		UpdateModelStats();
		RedrawWnd();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			ModelPtr->MirrorYModelWndSelected(BodyPos.x, BodyPos.y);	
			LogOperCtrl.SaveLogModelWndSelectedMirrorY(ModelPtr);
			UpdateModelStats();
			RedrawWnd();
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMirrorXPos(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMirrorYPos() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	TPOINT2D BodyPos;
	ModelPtr->GetModelBodyPos(BodyPos);
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{		
		ModelPtr->MirrorXModelLandSelected(BodyPos.x, BodyPos.y);
		LogOperCtrl.SaveLogModelLandSelectedMirrorX(ModelPtr);
		UpdateModelStats();
		RedrawWnd();
		return;
	}
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		LandPtr = WndPtr->GetWndLandPtr();
		if ( NULL == LandPtr )
		{
			ModelPtr->MirrorXModelWndSelected(BodyPos.x, BodyPos.y);		
			LogOperCtrl.SaveLogModelWndSelectedMirrorX(ModelPtr);
			UpdateModelStats();
			RedrawWnd();
		}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMirrorYPos(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditAlignCenterPos()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }		
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		ModelPtr->AlignCenterModelWndSelected();	
		LogOperCtrl.SaveLogModelWndSelectedAlignCenter(ModelPtr);
		UpdateModelStats();
		RedrawWnd();		
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditAlignCenterPos(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditAlignCenterPosU()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }			
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);		
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		ModelPtr->AlignCenterUModelWndSelected();
		LogOperCtrl.SaveLogModelWndSelectedAlignCenterU(ModelPtr);
		UpdateModelStats();
		RedrawWnd();
		return;
	}	
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL != LandPtr )
	{
		ModelPtr->AlignCenterUModelLandSelected();	
		LogOperCtrl.SaveLogModelLandSelectedAlignCenterU(ModelPtr);
		UpdateModelStats();
		RedrawWnd();
		return;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditAlignCenterPosU(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditAlignCenterPosV()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }		
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{
		ModelPtr->AlignCenterVModelWndSelected();
		LogOperCtrl.SaveLogModelWndSelectedAlignCenterV(ModelPtr);
		UpdateModelStats();
		RedrawWnd();
		return;
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditAlignCenterPosV(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecToggleEnhanceImageMode()
{
	bool IsOK = true;
	IsOK = UpdateImageByAlgParam();
	if ( true == IsOK )
	{	return; }	
	UpdateFrameImage();
	return;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelEditWndShapeMode(BOX_SHAPE_MODE BoxShapeMode)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return false; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return false; }
	if ( WndPtr->CheckWndUsedShapeMode() == false ) { return false; }

	if ( ModelPtr->SetModelWndShapeMode(WndPtr, BoxShapeMode) == false )
	{	return false; }
	
	UpdateModelStats();
	RedrawWnd();	
	SendOutUpdatePartList();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeRectangle()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_RECTANGLE);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeRectangle(CCmdUI* pCmdUI)
{	
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeRoundRect()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_ROUND_RECT);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeRoundRect(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeEllipse()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_ELLIPSE);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeEllipse(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeCapsule()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_CAPSULE);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeCapsule(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeBullet()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_BULLET);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeBullet(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeHalfRoundRect()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_HALF_ROUND_RECT);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeHalfRoundRect(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeTShape()
{
	ExecModelEditWndShapeMode(BOX_SHAPE_T_SHAPE);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeTShape(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeParam()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndUsedShapeMode() == false ) { return; }

	CString      str;	
	CString      strCaption, strName, strValue;	
	CInputBoxWnd InputBox;
	const double Param = WndPtr->GetWndShapeParam();
	str = _T("Input Shape Param");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Param");	
	strName = LoadMultiLanguageString( str, str);
	strValue.Format(_T("%.2f"), Param);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return ; }
	str = InputBox.m_DataEdit1;
	const double NewParam = ::_ttof(str);
	if ( NewParam < 0 || NewParam>100.0 )
	{	return ; }
	if ( ModelPtr->SetModelWndShapeParam(WndPtr, NewParam) == false )	{	return ; }	
	UpdateModelStats();
	RedrawWnd();	
	SendOutUpdatePartList();
	return ;	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeParam(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditWndShapeParam2()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndUsedShapeMode() == false ) { return; }

	CString      str;	
	CString      strCaption, strName, strValue;	
	CInputBoxWnd InputBox;
	const double Param = WndPtr->GetWndShapeParam2();
	str = _T("Input Shape Param2");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Param2");	
	strName = LoadMultiLanguageString( str, str);
	strValue.Format(_T("%.2f"), Param);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return ; }
	str = InputBox.m_DataEdit1;
	const double NewParam = ::_ttof(str);
	if ( NewParam < 0 || NewParam>100.0 )
	{	return ; }
	if ( ModelPtr->SetModelWndShapeParam2(WndPtr, NewParam) == false )	{	return ; }	
	UpdateModelStats();
	RedrawWnd();	
	SendOutUpdatePartList();
	return ;	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditWndShapeParam2(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditWndGroupID()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr )
	{	return; }

	size_t  i=0;
	CString str;
	CString strLabel;
	CString strCaption;
	CString strGroupID;	
	CInputListWnd EnumWnd;
	TListNode              Node;
	std::vector<TListNode> NodelList;

	std::vector<int> WndGroupIDList;
	ALG_TYPE      AlgType = WndPtr->GetWndAlgType();
	WND_DEFECT_ID WndDefectID = WndPtr->GetWndDefectID();	
	const int WndGroupID = WndPtr->GetWndGroupID();
	const int MaxWndGorupID = ModelPtr->GetModelWndFreeGroupID();

	strCaption = _T("Modify Wnd Group ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Wnd Group ID");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strGroupID.Format(_T("%d"), WndGroupID+1);
	ModelPtr->ListModelWndGroupIDByDefectID_AlgType(WndDefectID, AlgType, WndGroupIDList);

	std::sort(WndGroupIDList.begin(), WndGroupIDList.end());
	const size_t WndGroupIDCount = WndGroupIDList.size();

	Node = TListNode();
	Node.Data = MaxWndGorupID;
	Node.Text = _T("New");
	NodelList.push_back(Node);
	for ( i=0; i<WndGroupIDCount; i++ )
	{
		Node = TListNode();
		Node.Data = WndGroupIDList[i];		
		Node.Text.Format(_T("%d"), WndGroupIDList[i]+1);
		NodelList.push_back(Node);
	}	

	EnumWnd.SetParam1(strCaption, strLabel, WndGroupID, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	
	const int InputGroupID = EnumWnd.GetSelData();
	if ( InputGroupID == WndGroupID )
	{	return; }

	int NewWndGorupID = InputGroupID;
	if ( NewWndGorupID<0 || NewWndGorupID>MaxWndGorupID )
	{	NewWndGorupID = MaxWndGorupID;	}

	std::vector<unsigned int> WndIndexList;
	ModelPtr->GetModelWndSelectedIndexList(WndGroupID, -1, WndIndexList);	//
	if ( ModelPtr->ModifyModelWndGroupID(WndIndexList, NewWndGorupID, -1) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelWndGroupID Fault"));
		return ;
	}
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandGroupID()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }

	size_t  i=0;
	CString str;
	CString strLabel;
	CString strCaption;
	CString strGroupID;
	CInputListWnd EnumWnd;
	TListNode              Node;
	std::vector<TListNode> NodelList;

	std::vector<int> LandGroupIDList;
	LAND_TYPE LandType = LandPtr->GetLandType();
	const int LandGroupID = LandPtr->GetLandGroupID();
	const int MaxLandGorupID = ModelPtr->GetModelLandFreeGroupID();

	strCaption = _T("Modify Land Group ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Land Group ID");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strGroupID.Format(_T("%d"), LandGroupID+1);
	//ListModelLandGroupIDByLandType
	ModelPtr->ListModelLandGroupIDByLandType(LandType, LandGroupIDList);

	std::sort(LandGroupIDList.begin(), LandGroupIDList.end());
	const size_t LandGroupIDCount = LandGroupIDList.size();

	Node = TListNode();
	Node.Data = MaxLandGorupID;
	Node.Text = _T("New");
	NodelList.push_back(Node);
	for ( i=0; i<LandGroupIDCount; i++ )
	{
		Node = TListNode();
		Node.Data = LandGroupIDList[i];		
		Node.Text.Format(_T("%d"), LandGroupIDList[i]+1);
		NodelList.push_back(Node);
	}	

	EnumWnd.SetParam1(strCaption, strLabel, LandGroupID, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	
	const int InputGroupID = EnumWnd.GetSelData();
	if ( InputGroupID == LandGroupID )
	{	return; }

	int NewLandGorupID = InputGroupID;
	if ( NewLandGorupID<0 || NewLandGorupID>MaxLandGorupID )
	{	NewLandGorupID = MaxLandGorupID;	}

	std::vector<size_t> LandIndexList;
	ModelPtr->GetModelLandSelectedIndexList(LandGroupID, LandIndexList);	//
	if ( ModelPtr->ModifyModelLandGroupID(LandIndexList, NewLandGorupID) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandGroupID Fault"));
		return ;
	}
	LogOperCtrl.SaveLogModelLandGroupID(ModelPtr, LandIndexList, NewLandGorupID);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditGroupID()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();	
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL != WndPtr )
	{
		ExecModelEditWndGroupID();
		return;
	}	
	if ( NULL != LandPtr )
	{	
		ExecModelEditLandGroupID();
		return; 
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditGroupID(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditWndBandID()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr )
	{	return; }

	size_t  i=0;
	CString str;
	CString strLabel;
	CString strCaption;
	CString strBandID;	
	CInputListWnd EnumWnd;
	TListNode              Node;
	std::vector<TListNode> NodelList;

	std::vector<int> WndBandIDList;	
	const int WndBandID = WndPtr->GetWndBandID();
	const int WndGroupID = WndPtr->GetWndGroupID();	
	const int MaxWndBandID = ModelPtr->GetModelWndFreeBandID(WndGroupID);

	strCaption = _T("Modify Wnd Band ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Wnd Band ID");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strBandID.Format(_T("%d"), WndBandID+1);
	ModelPtr->ListModelWndBandIDByGroupID(WndGroupID, WndBandIDList);

	std::sort(WndBandIDList.begin(), WndBandIDList.end());
	const size_t WndBandIDCount = WndBandIDList.size();

	Node = TListNode();
	Node.Data = MaxWndBandID;
	Node.Text = _T("New");
	NodelList.push_back(Node);
	for ( i=0; i<WndBandIDCount; i++ )
	{
		Node = TListNode();
		Node.Data = WndBandIDList[i];		
		Node.Text.Format(_T("%d"), WndBandIDList[i]+1);
		NodelList.push_back(Node);
	}	

	EnumWnd.SetParam1(strCaption, strLabel, WndBandID, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	
	const int InputBandID = EnumWnd.GetSelData();
	if ( InputBandID == WndBandID )
	{	return; }

	int NewWndBandID = InputBandID;
	if ( NewWndBandID<0 || NewWndBandID>MaxWndBandID )
	{	NewWndBandID = MaxWndBandID;	}

	std::vector<unsigned int> WndIndexList;
	ModelPtr->GetModelWndSelectedIndexList(WndGroupID, -1, WndIndexList);	//
	if ( ModelPtr->ModifyModelWndBandID(WndIndexList, WndGroupID, NewWndBandID) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelWndBandID Fault"));
		return ;
	}
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandAlignID()
{
	//ExecModelEditLandAlignID_v1();
	ExecModelEditLandAlignID_v2();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandAlignID_v1()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }

	size_t  i=0;
	CString str;
	CString strLabel;
	CString strCaption;
	CString strGroupID;
	CInputListWnd EnumWnd;
	TListNode              Node;
	std::vector<TListNode> NodelList;

	std::vector<int> LandAlignIDList;
	LAND_TYPE LandType = LandPtr->GetLandType();
	const int LandGroupID = LandPtr->GetLandGroupID();
	const int LandAlignID = LandPtr->GetLandAlignID();
	const int MaxLandAlignID = ModelPtr->GetModelLandFreeAlignID(LandGroupID);

	strCaption = _T("Modify Land Align ID");
	strCaption = LoadMultiLanguageString(strCaption, strCaption);
	strLabel = _T("Land Align ID");
	strLabel = LoadMultiLanguageString(strLabel, strLabel);
	strGroupID.Format(_T("%d"), LandAlignID+1);	
	ModelPtr->ListModelLandAlignIDByGroupID(LandGroupID, LandAlignIDList);

	std::sort(LandAlignIDList.begin(), LandAlignIDList.end());
	const size_t LandAlignIDCount = LandAlignIDList.size();

	Node = TListNode();
	Node.Data = MaxLandAlignID;
	Node.Text = _T("New");
	NodelList.push_back(Node);
	for ( i=0; i<LandAlignIDCount; i++ )
	{
		Node = TListNode();
		Node.Data = LandAlignIDList[i];		
		Node.Text.Format(_T("%d"), LandAlignIDList[i]+1);
		NodelList.push_back(Node);
	}	

	EnumWnd.SetParam1(strCaption, strLabel, LandAlignID, NodelList);
	if ( EnumWnd.GetSelIndex1() < 0 ) 
	{	EnumWnd.SetSelIndex1(0); }	
	if ( EnumWnd.DoModal() == IDCANCEL )
	{	return ; }	
	const int InputAlignID = EnumWnd.GetSelData();
	if ( InputAlignID == LandAlignID )
	{	return; }

	int NewLandAlignID = InputAlignID;
	if ( NewLandAlignID<0 || NewLandAlignID>MaxLandAlignID )
	{	NewLandAlignID = MaxLandAlignID;	}

	std::vector<size_t> LandIndexList;
	ModelPtr->GetModelLandSelectedIndexList(LandGroupID, LandIndexList);	//
	if ( ModelPtr->ModifyModelLandAlignID(LandIndexList, LandGroupID, NewLandAlignID) == false )
	{
		JetAPI::ShowMessageBox(_T("Error, ModifyModelLandAlignID Fault"));
		return ;
	}
	LogOperCtrl.SaveLogModelLandAlignID(ModelPtr, LandIndexList, NewLandAlignID);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditLandAlignID_v2()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL == LandPtr )
	{	return; }
	CString str;	
	std::vector<int> LandAlignIDList;
	LAND_TYPE LandType = LandPtr->GetLandType();
	const int LandGroupID = LandPtr->GetLandGroupID();
	const int LandAlignID = LandPtr->GetLandAlignID();
	const int MaxLandAlignID = ModelPtr->GetModelLandFreeAlignID(LandGroupID);

	str.Format(_T("Do you want to Link Land Aligned ID?"));
	str = LoadMultiLanguageString(str, str);
	DWORD Res = JetAPI::ShowMessageBox(str, MB_YESNOCANCEL);
	if ( IDCANCEL == Res ) { return; }
	
	std::vector<size_t> LandIndexList;
	ModelPtr->GetModelLandSelectedIndexList(LandGroupID, LandIndexList);	//
	if ( IDYES == Res )
	{	
		if ( ModelPtr->LinkModelLandAlignID(LandIndexList, LandGroupID) == false )
		{
			JetAPI::ShowMessageBox(_T("Error, LinkModelLandAlignID Fault"));
			return ;
		}
	}
	if ( IDNO == Res )
	{	
		if ( ModelPtr->UnLinkModelLandAlignID(LandIndexList, LandGroupID) == false )
		{
			JetAPI::ShowMessageBox(_T("Error, UnLinkModelLandAlignID Fault"));
			return ;
		}
	}

	std::vector<int> AlignIDList;
	const size_t IndexCount=LandIndexList.size();
	for ( size_t i=0; i<IndexCount; i++ )
	{
		LandPtr = ModelPtr->GetModelLandPtr(LandIndexList[i], true);
		if ( NULL == LandPtr ) { continue; }		
		AlignIDList.push_back(LandPtr->GetLandAlignID());		
	}

	LogOperCtrl.SaveLogModelLandAlignID(ModelPtr, LandIndexList, AlignIDList);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditBandID()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();	
	CAOILand *LandPtr = ModelPtr->GetModelLandActivted();
	if ( NULL != WndPtr )
	{
		ExecModelEditWndBandID();
		return;
	}	
	if ( NULL != LandPtr )
	{	
		ExecModelEditLandAlignID();
		return; 
	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditBandID(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecModelEditAddAllWnd(MDW_VERSION Version, UINT WndCmd)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	if ( ModelPtr->GetModelEditMode() == false ) { return; }
	if ( AOIDataCollect.OperateLevelEditFuncAddModelWnd() == false ) { return ; }

	TREGION4D Region;
	TPOINT2D CornorPos[4];		
	TMODEL_DEFAULT_WND_PARAM Param;	
	MODEL_TYPE ModelType = ModelPtr->GetModelType();		
	CString GroupName = ModelPtr->GetModelGroupName();
	CString ImageFilename  = ModelPtr->GetModelDefaultImageFilename();
	CHIP_SIZE_MODE ChipSizeMode = ModelPtr->GetModelChipSizeMode();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();
	const bool bUse3DLight = ProjectPtr->CheckProjectFrameUniqueID_3D();
	const int nLevel = AOIDataCollect.GetSystemParameter().m_ModelDefaultWndLevel;		
	CWndDefectItem ModelTestDefectItems = ModelPtr->CheckModelTestDefectItems();
	CWndDefectItem BasicTestDefectItems = AOIDataDefine.GetModelBasicDefectItem(ModelType);	

	const int nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> ModelUniFrameList;	
	const size_t   WndCount = ModelPtr->GetModelWndCount();
	if ( BuildModelUniFrameList(ModelPtr, ModelUniFrameList, nAlign, true, bNoFilter) == true )
	{
		ModelPtr->AnalysisModelProperty(ModelUniFrameList);
		JetAPI::ClearUniFrameList(ModelUniFrameList);
	}	

	ModelPtr->GetModelBodyBox().GetBoxCornerPos(CornorPos);
	JetAPI::RotateCornerPos(-ComponentAngle, 0, 0, CornorPos);
	JetAPI::PointsToRegion(CornorPos, 4, Region);
	const double ModelBodySizeW = Region.GetWidth();
	const double ModelBodySizeH = Region.GetHeight();
	const double ModelBodyHeight= ModelPtr->GetModelBodyHeight();
	const double ModelBodyMissing=JetAPI::AdjustValue(ModelBodyHeight*0.25, 10);

	if ( CHIP_SIZE_NONE == ChipSizeMode)
	{	ChipSizeMode = CAOIModel::FindModelChipSizeMode(ModelType, Region); }	
	AOIDataCollect.LoadModelDefaultWndParam(ModelType, ChipSizeMode, GroupName, Param, Version);	
	if ( MODEL_TYPE_RESISTOR_ARRAY == ModelType )
	{
		const double SizeExt = 200;
		const double Size0402_W = 1000+SizeExt;
		const double Size0402_H = 1000+SizeExt;
		//濾除0201以下的尺寸
		if ( ModelBodySizeW<Size0402_W && ModelBodySizeH<Size0402_H )
		{	Param.bTextWrong = false;	}
		else if ( ModelBodySizeW<Size0402_H && ModelBodySizeH<Size0402_W )
		{	Param.bTextWrong = false;	}
	}

	Param.eVersion = Version;
	Param.sGroupName = GroupName;	
	Param.bUse3DLight = bUse3DLight;	
	Param.eChipSizeMode = ChipSizeMode;	
	Param.dBodyMissingHeightTolerance=MAX(ModelBodyMissing, 50);		
	if ( MDW_VERSION_1 == Version )
	{
		CModelAddAllWnd  Wnd;
		Wnd.SetModelImageFilename(ImageFilename);
		Wnd.SetAddAllParam(Param, ModelTestDefectItems, BasicTestDefectItems, nLevel);	
		if ( Wnd.DoModal() == IDCANCEL )
		{	return;	}
		Wnd.GetAddAllParam(Param);
	}
	if ( MDW_VERSION_2 == Version )
	{
		CString str;
		str = _T("Do you want to add default windows by version2?");
		str = LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
		{	return; }

		ModelPtr->ClearModelFolder(MODEL_CLEAR_FOLDER_PATTERNS);
		ModelPtr->ClearModelAllWndList();	
		LogOperCtrl.SaveLogModelContent(ModelPtr, _T("Clear All Wnd"));
	}

	unsigned int   DefaultFrameIndex=0;
	unsigned int   DefaultFrameUniqueID = 0;	
	std::vector<unsigned int> FrameIndexMapList;
	ProjectPtr->BuildProjectFrameIndexMapParam(FrameIndexMapList, DefaultFrameIndex, DefaultFrameUniqueID);			
	if ( ProjectPtr->ModifyProjectModelDefaultWndParam(Param) == false )
	{
		JetAPI::ShowMessageBox(ProjectPtr->GetErrorString());
		return;
	}
	ModelPtr->AddModelDefaultWnd(Param);
	ModelPtr->InvisibleModelWnd();
	ModelPtr->UnSelectModel();
	ModelPtr->SetModelBodyBoxActived(true);		
	ModelPtr->UpdateModelFrameIndex(DefaultFrameIndex, DefaultFrameUniqueID, FrameIndexMapList);

	std::vector<CColorGroup>  ColorGroupList;
	ProjectPtr->CloneProjectColorGroupList(ColorGroupList);
	ModelPtr->UpdateModelColorGroupLinkIndex(ColorGroupList);

	CAOIWnd *WndPtr = ModelPtr->GetModelWndPtr(WndCount, true);
	if ( NULL == WndPtr )
	{	WndPtr = ModelPtr->GetModelWndPtr(0, true);	}
	const size_t ModelWndCount = ModelPtr->GetModelWndCount();
	if ( NULL != WndPtr )
	{
		const int WndGroupID = WndPtr->GetWndGroupID();
		ModelPtr->SetModelWndVisibledByWndGroupID(WndGroupID, -1, true);			
		WndPtr->SetWndSelected(true);	
	}
	ModelPtr->SetModelWndActived(WndPtr);
	AOIDataCollect.SetRibbonDefaultWndCmdID(WndCmd);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);

	CAOIComponent *pComponent = ProjectPtr->GetProjectActiveComponent();	
	AOIDataCollect.CloseActiveComponent(pComponent);
	ModelPtr->SetModelWndActived(WndPtr);

	UpdateModelStats();
	RedrawWnd();	
	SendOutUpdatePartList();
	PostMessage(WM_COMMAND, MENU_MODEL_EDIT_CREATE_BK_IMAGE, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditAddAllWnd() 
{
	// TODO: Add your command handler code here
	ExecModelEditAddAllWnd(MDW_VERSION_1, MENU_MODEL_EDIT_ADD_ALL_WND);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditAddAllWnd(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditAddAllWndv2()
{
	// TODO: Add your command handler code here
	ExecModelEditAddAllWnd(MDW_VERSION_2, MENU_MODEL_EDIT_ADD_ALL_WND_V2);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditAddAllWndv2(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditAddAllWndGroup()
{
	// TODO: Add your command handler code here
	UINT CmdID = AOIDataCollect.GetRibbonDefaultWndCmdID();
	switch (CmdID)
	{
	case MENU_MODEL_EDIT_ADD_ALL_WND_V2:
		OnModelEditAddAllWndv2();
		break;	
	default:
	case MENU_MODEL_EDIT_ADD_ALL_WND:
		OnModelEditAddAllWnd();
		break;
	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditAddAllWndGroup(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	BOOL bEditMode = CheckInEditMode(true);
	if (FALSE == bEditMode || true == bLockUIWnd)
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneAlignFiducial() 
{
	// TODO: Add your command handler code here	
	//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);
	CString str;	
	CAOIProject *ProjectPtr = CEditModelView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecComponentModelToLibrary();

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetIsNeedGrabFiducial(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetTaskMode(TASK_ALIGN_PROJECT);	
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);
	CreateMapImage();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneAlignFiducial(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIProject *ProjectPtr = GetActiveProject();
#ifndef OFFLINE_VERSION
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
#else
	pCmdUI->Enable(FALSE);
#endif//OFFLINE_VERSION	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneInspection() 
{
	// TODO: Add your command handler code here
	CString str;
	//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();
	const bool ChangeRibbonTuneID = AOIDataCollect.GetChangeRibbonTuneID();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecComponentModelToLibrary();

	DWORD dwRes=0;
	int MaxRepeatCount = 0;
	bool bSaveSpcFile = false;
	CInputBoxWnd InputBox;
	CString WndTxt, Title, Default;	
	
	if ( true == IsNeedGrabFiducial )
	{	ProjectPtr->SelectProjectAllFds(true);	 }
	else
	{	ProjectPtr->SelectProjectAllFds(false);	 }	
	
	const bool bIsRepeatTest = AOIDataCollect.GetIsRepeatTestUI();
	const bool bIsAskRepeatTest = AOIDataCollect.GetIsAskRepeatTest();
	if ( true==bIsRepeatTest && true==bIsAskRepeatTest )
	{
		WndTxt = _T("Set Repeat Count");
		WndTxt = LoadMultiLanguageString(WndTxt, WndTxt);

		Title  = _T("Max Count");
		Title = LoadMultiLanguageString(Title, Title);
		Default.Format(_T("%d"), 1);
		InputBox.SetParam1(WndTxt, Title, Default);
		if ( InputBox.DoModal() == IDCANCEL ) { return; }
		MaxRepeatCount = ::_ttoi(InputBox.m_DataEdit1);

		Title = _T("Do you want to save spc file?");
		Title = LoadMultiLanguageString(Title, Title);
		dwRes = JetAPI::ShowMessageBox(Title, MB_YESNOCANCEL|MB_DEFBUTTON2);
		if ( IDCANCEL == dwRes ) { return; }
		if ( IDYES == dwRes ) { bSaveSpcFile=true; }
		if ( IDNO == dwRes ) { bSaveSpcFile=false; }
	}
	ProjectPtr->SelectProjectAllFds(true);
	ProjectPtr->SelectProjectAllBarcodes(true);
	ProjectPtr->SelectProjectAllComponents(true);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();	
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectActiveComponent(true);
	ProjectPtr->SetProjectSpcFileSaveEnabled(bSaveSpcFile);		

	if ( true == bIsAskRepeatTest )
	{
		AOIDataCollect.SetRepeatedTestCount(0);
		AOIDataCollect.SetRepeatedTestMaxCount(MaxRepeatCount);
	}
	AOIDataCollect.SetIsAskRepeatTest(true);	
	AOIDataCollect.SetChangeRibbonTuneID(true);
	AOIDataCollect.SetIsRepeatTest(bIsRepeatTest);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetInspectingMode(INSPECTING_TUNNING);
	AOIDataCollect.ReleaseModelUniFrameList();		
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);	
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}	
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);	
	CreateMapImage();
	if ( true == ChangeRibbonTuneID )
	{	AOIDataCollect.SetRibbonTuneGroupCmdID(ID_TUNE_INSPECTION);	 }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneInspection(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneSelectedComponent() 
{
	// TODO: Add your command handler code here
	CString str;
	//this->m_ProjectMapWnd.SetDrawProjectMode(IMAGE_WND_DRAW_PROJECT_INSPECTING);	
	bool bChkStartLight = true;
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();	
	const bool ChangeRibbonTuneID = AOIDataCollect.GetChangeRibbonTuneID();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecComponentModelToLibrary();		
	
	ProjectPtr->SelectProjectFdForTuning(OfflineMode, IsNeedGrabFiducial);
	ProjectPtr->SelectProjectAllBarcodes(false);	
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->SelectProjectComponentsMasterAgent();
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectActiveComponent(true);
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);		

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetChangeRibbonTuneID(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());
	AOIDataCollect.SetInspectingMode(INSPECTING_SELECTED);	
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}			
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);	
	CreateMapImage();
	if ( true == ChangeRibbonTuneID )
	{	AOIDataCollect.SetRibbonTuneGroupCmdID(ID_TUNE_SELECTED_COMPONENT);	 }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneSelectedComponent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneSelectedModel() 
{
	// TODO: Add your command handler code here
	CString str;			
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();	
	const bool ChangeRibbonTuneID = AOIDataCollect.GetChangeRibbonTuneID();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecComponentModelToLibrary();
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	if ( NULL == ComponentPtr ) { return; }	
	if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { return; }
	
	CString ModelName = ComponentPtr->GetComponentModelName();	
	ProjectPtr->SelectProjectAllBarcodes(false);
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelName(ModelName, false);	
	ProjectPtr->SelectProjectFdForTuning(OfflineMode, IsNeedGrabFiducial);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectActiveComponent(true);
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);	

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetChangeRibbonTuneID(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetInspectingMode(INSPECTING_SELECTED);
	AOIDataCollect.ReleaseModelUniFrameList();	
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);	
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}		
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);
	CreateMapImage();
	if ( true == ChangeRibbonTuneID )
	{	AOIDataCollect.SetRibbonTuneGroupCmdID(ID_TUNE_SELECTED_MODEL);	 }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneSelectedModel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
int CEditModelView::GetMaxFrameCount()
{
	int FrameMaxCount = 1;
#ifdef _X64
	FrameMaxCount = FRAME_MAX_COUNT;
#else
	FrameMaxCount = FRAME_MAX_COUNT;//1
#endif//_X64
	return FrameMaxCount;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecUpdateToLibrary(bool bArrange)
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )	{	return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr )	{	return false; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();	
	if ( true == ModelIsolated ) { return false; }
	if ( true == bArrange )
	{	ModelPtr->LayoutModelWndListByDefectID();	}
	bool bModelModified = ModelPtr->CheckModelModified();		
	if ( true == bModelModified )
	{	ModelPtr->SetupkModelModifiedDateTime();	}	
	ProjectPtr->ApplyProjectCompnentModelToLibraryModel(ComponentPtr, true);	
	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	
	RedrawWnd();	

	//自動更新模組底圖
	const bool bNeedToGrab = ModelPtr->GetModelBKImageNeedToGrab();	
	if ( true == bNeedToGrab )
	{	OnEditModelBkImage();	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditUpdateToLibrary() 
{
	// TODO: Add your command handler code here
	ExecUpdateToLibrary(false);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditUpdateToLibrary(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	CAOIModel   *ModelPtr = GetModelPtr();	
	if ( FALSE==bEditMode || true==bLockUIWnd || NULL==ModelPtr)
	{	pCmdUI->Enable(FALSE);	}
	else
	{
		if ( NULL != ModelPtr )
		{
			if ( ModelPtr->GetModelIsolated() == true ) 
			{	pCmdUI->Enable(FALSE);	}
			else
			{	pCmdUI->Enable(TRUE);	}
		}
		else
		{	pCmdUI->Enable(TRUE);	}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditArrangeModel()
{
	ExecUpdateToLibrary(true);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditArrangeModel(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	CAOIModel   *ModelPtr = GetModelPtr();	
	if ( FALSE==bEditMode || true==bLockUIWnd || NULL==ModelPtr)
	{	pCmdUI->Enable(FALSE);	}
	else
	{
		if ( NULL != ModelPtr )
		{
			if ( ModelPtr->GetModelIsolated() == true ) 
			{	pCmdUI->Enable(FALSE);	}
			else
			{	pCmdUI->Enable(TRUE);	}
		}
		else
		{	pCmdUI->Enable(TRUE);	}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditShowModelActiveLine()
{
	bool bShow = AOIDataCollect.GetDrawModelActivedLine();
	if ( true == bShow ) 
	{	AOIDataCollect.SetDrawModelActivedLine(false); }
	else
	{	AOIDataCollect.SetDrawModelActivedLine(true); }
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditShowModelActiveLine(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(TRUE==AOIDataCollect.GetDrawModelActivedLine());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditUpdateFromLibrary()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )	{	return; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr )	{	return; }
	bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
		
	CString str, str2;
	CString ModelName = ModelPtr->GetModelName();
	str2 = _T("Do you want to update component's model from library");
	str2 = LoadMultiLanguageString(str2, str2);
	str.Format(_T("%s [%s] ?"), str2, ModelName);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO ) 
	{	return; }
	if ( true == ModelIsolated )
	{
		ComponentPtr->SetComponentModelIsolated(false);
		return; 
	}
	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->ApplyProjectModelToComponentsSelected(ModelName);
	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));	
	RedrawWnd();	

}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditUpdateFromLibrary(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	CAOIModel   *ModelPtr = GetModelPtr();	
	if ( FALSE==bEditMode || true==bLockUIWnd || NULL==ModelPtr)
	{	pCmdUI->Enable(FALSE);	}
	else
	{
		if ( NULL != ModelPtr )
		{
			if ( ModelPtr->GetModelIsolated() == true ) 
			{	pCmdUI->Enable(FALSE);	}
			else
			{	pCmdUI->Enable(TRUE);	}
		}
		else
		{	pCmdUI->Enable(TRUE);	}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditUpdateToLibraryGroup()
{
	// TODO: Add your command handler code here	
	//ExecShowLibraryWnd(false);

	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )	{	return; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr )	{	return; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated ) { return; }
	if ( ProjectPtr->ApplyProjectCompnentModelToLibraryModel(ComponentPtr, true) == false )
	{	return; }
	if ( ComponentPtr->CheckComponentType_ModelTest() == false )
	{	return; }
	
	CString ModelName = ModelPtr->GetModelName();
	CString GroupName = ModelPtr->GetModelGroupName();
	CAOIModel *ModelPtrM = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtrM ) { return ; }

	std::vector<unsigned int> SelIndexList;
	CString str, str2;	
	
	CModelUpdateToGroupWnd Wnd;
	TModelUpdateToGroupParam Param;
	AOIDataCollect.GetModelUpdateToGroupParam(Param);

	ModelPtr->GetModelWndSelectedIndexList(-1, -1, SelIndexList);
	ModelPtrM->SetModelWndSelectedIndexList(SelIndexList);

	Wnd.SetModelPtr(ModelPtrM);
	Wnd.SetUpdateParam(Param);
	if ( Wnd.DoModal() == IDCANCEL )
	{	return ; }

	Wnd.GetUpdateParam(Param);
	if ( true == Param.bUpdateDelete )
	{
		str = _T("Do you want to delete the windows in the sampe Group?");
		str = this->LoadMultiLanguageString(str, str);
		if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
		{	return; }
	}
	AOIDataCollect.SetModelUpdateToGroupParam(Param);
	ProjectPtr->ApplyProjectCompnentModelToLibraryGroup(Param, ModelPtrM);	
	ComponentPtr->SetComponentSelected(true);
	ProjectPtr->SetProjectActiveComponent(ComponentPtr);
	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));		
	CEditModelView::RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditUpdateToLibraryGroup(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	CAOIModel   *ModelPtr = GetModelPtr();	
	if ( FALSE==bEditMode || true==bLockUIWnd || NULL==ModelPtr)
	{	pCmdUI->Enable(FALSE);	}
	else
	{
		if ( NULL != ModelPtr )
		{
			if ( ModelPtr->GetModelIsolated() == true ) 
			{	pCmdUI->Enable(FALSE);	}
			else
			{	pCmdUI->Enable(TRUE);	}
		}
		else
		{	pCmdUI->Enable(TRUE);	}
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditSaveDefaultModel()
{
	ExecSaveDefaultModel();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditSaveDefaultModel(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	CAOIModel   *ModelPtr = GetModelPtr();	

	if ( FALSE==bEditMode || true==bLockUIWnd || NULL==ModelPtr)
	{	bEnable = FALSE;	}
	
	if ( NULL != ModelPtr )
	{
		if ( ModelPtr->GetModelIsolated() == true ) 
		{	bEnable = FALSE;	}
		
		if ( ModelPtr->CheckModelTypeEnabled() == false )
		{	bEnable = FALSE;	}
	}	
	pCmdUI->Enable(bEnable);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditProjecSave()
{
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd ) { return; }
	HWND hWnd = AOIDataCollect.GetMainFrameWnd();
	if ( NULL == hWnd ) { return ; }
	::SendMessage(hWnd, WM_COMMAND, ID_PROJECT_SAVE, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditProjecSave(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditShowAgentList()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();
	if ( NULL == ComponentPtr ) { return; }

	CComponentAgentListWnd Wnd;
	Wnd.SetComponentPtr(ComponentPtr);
	Wnd.DoModal();
	if ( ComponentPtr == ProjectPtr->GetProjectActiveComponent() )
	{	return; }

	PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_EDIT);	
	//PostMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_SELECTED, LPARAM_BUILD_DOCK_LIST_EDIT);
	PostMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_PART_SELECTED, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditShowAgentList(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIComponent *ComponentPtr = NULL;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL != ProjectPtr )
	{	ComponentPtr = ProjectPtr->GetProjectActiveComponent();	}
	if ( true==bLockUIWnd || NULL==ComponentPtr)
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::BackupViewParam()//備份顯示參數
{
	AOIDataCollect.SetViewImageZoom(m_ImageZoom);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::RestoreViewParam()//恢復顯示參數
{
	m_ImageZoom = AOIDataCollect.GetViewImageZoom();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::CalcFovPosition()
{	
	if ( GetLockUIWnd() == true ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }

	TPOINT2D CadOffset, StageOffset;
	const double ImageZoom = m_ImageZoom;
	const TPOINT2D StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();	
	const double ImageResolutionX = ImageRes.x;
	const double ImageResolutionY = ImageRes.y;
	const double StageCpx = StageCp.x;
	const double StageCpy = StageCp.y;
	const double ImageOffsetX = m_ImageOffset.x;
	const double ImageOffsetY = m_ImageOffset.y;
	CadOffset.x =  -ImageOffsetX*ImageResolutionX*ImageZoom;
	CadOffset.y =   ImageOffsetY*ImageResolutionY*ImageZoom;
	AOIDataCollect.MapCadOffsetPtToStage(CadOffset, StageOffset);
	const double NewStagePosX = StageCpx+StageOffset.x;
	const double NewStagePosY = StageCpy+StageOffset.y;	
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();	
	MotionCtrlPtr->XYMoveTo(NewStagePosX, NewStagePosY, OfflineMode);
	MotionCtrlPtr->WaitForMotionStop();
	ResetImageOffset();	
	m_FOVPosStage.x = NewStagePosX;
	m_FOVPosStage.y = NewStagePosY;
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ResetImageOffset()
{
	m_ImageOffset.x = 0;
	m_ImageOffset.y = 0;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::BuildModelUniFrameList(CAOIModel *ModelPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign, bool bClone, bool bNoFilter)
{
	if ( GetShowProjectMapMode() == true ) { return false; }
	CAOIWnd   *WndPtr =NULL;
	TREGION4D StageRgn;	
	ModelPtr->CalcModelTotalRegionAll();	
	ModelPtr->GetModelTotalRegionStage(StageRgn);
	WndPtr = ModelPtr->GetModelWndActived();	
	if ( AOIDataCollect.CheckModelUniFrameListModelPtr(ModelPtr) == true )
	{	
		if ( AOIDataCollect.CopyModelUniFrameList(UniFrameList, bClone) == false )
		{	return false; }				
		PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED_NO_PROCESS, (LPARAM)WndPtr);		
	}
	else
	{		
		if ( AOIDataCollect.CreateModelUniFrameList(m_UniFrameList, m_FOVPosStage, m_FrameResolution, ModelPtr, UniFrameList, nAlign, bNoFilter) == false )		
		{	AOIDataCollect.ReleaseModelUniFrameList(); }		
		else
		{	AOIDataCollect.SetModelUniFrameList(ModelPtr, StageRgn, UniFrameList, bClone);		}
		if ( false == bNoFilter)
		{
			PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_SELECTED, (LPARAM)WndPtr);		
			//PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_NO_PROCESS, (LPARAM)WndPtr);		
		}
	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::BuildModelWndUniFrameList(CAOIModel *ModelPtr,CAOIWnd *WndPtr, std::vector<TUNI_FRAME> &UniFrameList, int nAlign)
{
	if ( NULL == ModelPtr ) { return false; }
	if ( NULL == WndPtr ) { return false; }	

	CString                 str;
	bool                    Exception=false;
	size_t                  i=0, j=0;
	size_t                  UniFrameCount=0;
	IMAGE_SIZE              ImageW=0;
	IMAGE_SIZE              ImageH=0;
	IMAGE_SIZE              ImageStep=0;
	IMAGE_SIZE              BitCount=0;
	IMAGE_PTR               ImagePtr=0;
	MASK_PTR                MaskPtr=0;
	SPACE_PTR               SpacePtr=0;

	TPOINT2D                FovCp;
	TREGION4D               CadRgn;
	TREGION4D               ImageRgn;
	TREGION4D               StageRgn;
	RECT                    RoiRect={0};
	IMAGE_SIZE              RoiW=0;
	IMAGE_SIZE              RoiH=0;
	IMAGE_SIZE              RoiStep=0;
	IMAGE_SIZE              RoiBitCount=0;
	IMAGE_PTR               RoiImagePtr=0;
	MASK_PTR                RoiMaskPtr=0;
	SPACE_PTR               RoiSpacePtr=0;
	TUNI_FRAME             *UniFramePtr=NULL;
	TUNI_FRAME              UniFrame;	
	BOOL                    bSave = FALSE;	
	
	FovCp.x = m_FOVPosStage.x;
	FovCp.y = m_FOVPosStage.y;
	Exception=false;
	WndPtr->GetWndRegionStage(StageRgn);
	for ( i=0; i<FRAME_MAX_COUNT; i++ )
	{
		UniFramePtr = &(m_UniFrameList[i]);
		if ( NULL == UniFramePtr ) { continue; }
		if ( NULL == UniFramePtr->ImagePtr ) { continue; }

		ImageW = UniFramePtr->ImageW;
		ImageH = UniFramePtr->ImageH;
		ImageStep = UniFramePtr->ImageStep;
		BitCount = UniFramePtr->BitCount;
		ImagePtr = UniFramePtr->ImagePtr;
		MaskPtr = UniFramePtr->MaskPtr;
		SpacePtr = UniFramePtr->SpacePtr;
		if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, m_FrameResolution, StageRgn, FovCp, ImageRgn) == false )
		{
			Exception = true;
			break;
		}
		JetAPI::Region4DToRect(ImageRgn, RoiRect, false);
		JetAPI::AdjustRectByAlignW(RoiRect, nAlign);
		if ( RoiRect.left<0 || RoiRect.right<0 || RoiRect.top<0 || RoiRect.bottom<0 || 
			 RoiRect.left>=ImageW || RoiRect.right>=ImageW || RoiRect.top>=ImageH || RoiRect.bottom>=ImageH ) 
		{
			Exception = true;
			break;
		}
		RoiW = RoiRect.right-RoiRect.left;
		RoiH = RoiRect.bottom-RoiRect.top;
		RoiBitCount = m_UniFrameList[i].BitCount;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, RoiBitCount, 4);
		if ( NULL != ImagePtr )
		{	
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiImagePtr, false)==false )
			{
				Exception = true;
				break;
			}
		}
		if ( NULL != MaskPtr )
		{	
			if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, MaskPtr, RoiRect, RoiStep, RoiMaskPtr, false)==false )
			{
				Exception = true;
				break;
			}
		}
		if ( NULL != SpacePtr )
		{	
			if ( ImageAPI.ExtractSpaceGrayRoiImage(ImageW, ImageH, ImageStep, SpacePtr, RoiRect, RoiStep, RoiSpacePtr, false)==false )
			{
				Exception = true;
				break;
			}
		}

		UniFrame.ImageW = RoiW;
		UniFrame.ImageH = RoiH;
		UniFrame.ImageStep = RoiStep;
		UniFrame.BitCount = RoiBitCount;
		UniFrame.ImagePtr = RoiImagePtr;
		UniFrame.MaskPtr = RoiMaskPtr;
		UniFrame.SpacePtr = RoiSpacePtr;
		UniFrameList.push_back(UniFrame);
		UniFrame = TUNI_FRAME();
		RoiImagePtr = NULL;
		RoiMaskPtr = NULL;
		RoiSpacePtr = NULL;
	}
	if ( true == Exception )
	{
		UniFrameCount = UniFrameList.size();
		for ( j=0; j<UniFrameCount; j++ )
		{
			UniFrame = UniFrameList[j];
			JetMemory.free_func(UniFrame.ImagePtr);
			JetMemory.free_func(UniFrame.MaskPtr);
			JetMemory.free_func(UniFrame.SpacePtr);
		}
		UniFrameList.clear();
		return false;
	}

#ifdef _DEBUG
	bSave = TRUE;
	if ( bSave == TRUE )
	{
		UniFrameCount = UniFrameList.size();
		for ( j=0; j<UniFrameCount; j++ )
		{
			UniFrame = UniFrameList[j];

			RoiW = UniFrame.ImageW;
			RoiH = UniFrame.ImageH;
			RoiStep = UniFrame.ImageStep;
			RoiBitCount = UniFrame.BitCount;
			RoiImagePtr = UniFrame.ImagePtr;
			RoiMaskPtr = UniFrame.MaskPtr;
			RoiSpacePtr = UniFrame.SpacePtr;

			if ( NULL != RoiImagePtr )
			{
				str.Format(_T("%s\\WndImage%d.BMP"), AOIDataCollect.GetAOITempDirectory(), j+1);
				ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiImagePtr, true);
			}
			if ( NULL != RoiMaskPtr )
			{
				str.Format(_T("%s\\WndMask%d.BMP"), AOIDataCollect.GetAOITempDirectory(), j+1);
				ImageAPI.SaveBMPImage(str, RoiW, RoiH, RoiStep, RoiBitCount, RoiMaskPtr, true);
			}
		}		
	}
#endif//_DEBUG
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelComponentInspect()
{
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	if ( AOIDataCollect.CheckAIServerIsReady(false) == false )	
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return false;
	}

	std::vector<TUNI_FRAME> UniFrameList;	
	const int nAlign = 4;
	const bool bNoFilter = false;
	if ( BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, true, bNoFilter) == false )
	{	return false; }

	CString str;	
#ifdef _DEBUG
	bool bSaved=true;
	if ( true == bSaved )
	{
		bool bReverse=true;
		bool bEnhance=false;
		bool bSave3D=false;
		bool bAppend=false;
		double SpaceRatio = -1;
		str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("TestModel.PNG"));
		ImageAPI.SaveUniFrameImage(str, UniFrameList, bReverse, bEnhance, bSave3D, bAppend, SpaceRatio);
	}
#endif//_DEBUG
	CWndDefectItem TestItem;
	CWndDefectItem AlarmItem;
	CAOIRgn *ModelRgnPtr = ModelPtr->GetModelAttachedPtr();
	const int OpenMPCount = AOIDataCollect.GetOpenMPCount_General();

	//Ash:test
	//CAOIComponent* _j_cmpn = ModelPtr->GetModelComponentPtr();
	//str.Format(_T("%s\\%s_EMC.PNG"), AOIDataCollect.GetAOITempDirectory(), _j_cmpn->GetComponentName());
	//ImageAPI.SaveUniFrameImage(str, UniFrameList, true, false, true, false, -1);

	TestItem.SetAll(1);
	AlarmItem.SetAll(0);	
	ModelPtr->SetModelDefectItemTest(TestItem);
	ModelPtr->SetModelDefectItemAlarm(AlarmItem);
	ModelPtr->InitModelInspection();
	ModelPtr->SetModelOpenMPCount(OpenMPCount);
	ModelPtr->ExecModelInspection(UniFrameList);
	if ( NULL != ModelRgnPtr )
	{
		if ( ModelRgnPtr->IsKindOf(RUNTIME_CLASS(CAOIComponent)) )
		{
			CAOIComponent *ComponentPtr = dynamic_cast<CAOIComponent*>(ModelRgnPtr);
			ComponentPtr->UpdateComponentResultStagePos();
		}
	}
	JetAPI::ClearUniFrameList(UniFrameList);
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);
	UpdateActiveObjList();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelInspectComponent() 
{
	// TODO: Add your command handler code here
	ExecModelComponentInspect();
	//PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelInspectComponent(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();		
	CAOIProject *ProjectPtr = CEditModelView::GetActiveProject();	
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::UpdateImageByAlgParam()//依據演算法更新畫面
{	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	DRAW_IMAGE_MODE DrawImageMode = AOIDataCollect.GetDrawImageMode();
	MANIPULATE_MODEL_MODE ManiMode = AOIDataCollect.GetManipulateModelMode();
	if ( DRAW_IAMGE_BY_ALG != DrawImageMode ) { return false; }	
	//if ( MANIPULATE_MODEL_GATHER_COLOR == ManiMode ) { return; }	

	std::vector<CAOIWndRoi*>  WndRoiList;
	std::vector<CAOIWndMask*> MaskWndList;
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return false; }
	WndPtr->GetWndRoiWndSelectedList(WndRoiList);	
	WndPtr->GetWndMaskWndSelectedList(MaskWndList);
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::UpdateImageByAlgParam Start"));	

	size_t     i=0;		
	CAOIWndRoi      *WndRoiPtr=NULL;
	CAOIWndMask     *WndMaskPtr=NULL;
	CAlgBinaryParam  BinaryParam;
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	if ( NULL == BinParamPtr ) { return false; }
	unsigned int FrameIndex = BinParamPtr->GetBinaryFrameIndex();
	unsigned int FrameUniqueID = BinParamPtr->GetBinaryFrameUniqueID();
	const int SwitchProject3DFrame = AOIDataCollect.GetSystemParameter().m_SwitchProject3DFrame;
	ProjectPtr->SetProjectMapIndex(FrameIndex);
	AOIDataCollect.GetBinaryParamTemp(BinaryParam);
	ExecAlgImage(ModelPtr, WndPtr, BinaryParam);
	if ( DRAW_MODEL_EDIT == DrawModelMode )
	{
		int    DynValue = BinaryParam.GetDynamicThresholdValue();
		double RelValue = BinaryParam.GetRelativeAveThresholdValue();
		BinParamPtr->SetDynamicThresholdValue(DynValue);
		BinParamPtr->SetRelativeAveThresholdValue(RelValue);
	}
	WndPtr->SetWndVisibled(true);	
	ModelPtr->SetModelWndActived(WndPtr);
	
	const size_t WndRoiCount = WndRoiList.size();
	for ( i=0; i<WndRoiCount; i++ )
	{
		WndRoiPtr = WndRoiList[i];
		if ( NULL == WndRoiPtr ) { continue; }
		WndRoiPtr->SetWndRoiSelected(true);
	}	
	const size_t MaskWndCount = MaskWndList.size();
	for ( i=0; i<MaskWndCount; i++ )
	{
		WndMaskPtr = MaskWndList[i];
		if ( NULL == WndMaskPtr ) { continue; }
		WndMaskPtr->SetWndMaskSelected(true);
	}
	AOIDataCollect.SetDrawingImageMode(DRAW_IAMGE_BY_ALG);	
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::UpdateImageByAlgParam End"));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecGetWndColorFilter(CColorRGBV &rgbv)//取得檢測框抽色參數
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }		
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	if ( true == IsExceptionAngle ) { return true; }

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE    BinaryMode = BinParamPtr->GetBinaryMode();
	IMAGE_SRC_MODE ImageSourceMode = BinParamPtr->GetBinaryImageSourceMode();	
	if ( IMAGE_SRC_COLOR != ImageSourceMode ) { return true; }
	if ( BINARY_COLOR_FILTER != BinaryMode ) { return true; }	
	const bool bResetColorGroup=false;
	if ( true == bResetColorGroup )
	{
		BinParamPtr->ResetBinaryColorList();
		BinParamPtr->SetBinaryColorActiveIndex(0);
	}
	CColorRGBV *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }
	
	CString    str;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	const TPOINT2D  StageCp = GetImageStageRgnCp();
	const TPOINT2D &ImageRes = GetImageResolution();
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return true; }
	if ( NULL == ImagePtr ) { return true; }
	if ( 24 != BitCount ) { return true; }

	bool         bIsOK = false;	
	bool         bSaved = true;	
	TREGION4D    ImageRgn;		
	TREGION4D    StageRgn;
	RECT         RoiRect={0, 0, 0, 0};	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_RESULT == DrawModelMode ) 
	{	WndPtr->GetWndRegionStageRes(StageRgn);	}
	else
	{	WndPtr->GetWndRegionStage(StageRgn);	}
	AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, StageRgn, StageCp, ImageRgn);
	JetAPI::Region4DToRect(ImageRgn, RoiRect, false);	
#ifdef _DEBUG
	if ( true == bSaved ) 
	{
		IMAGE_PTR  RoiPtr=NULL;
		IMAGE_SIZE RoiW=0, RoiH=0, RoiStep=0;
		RoiW = RoiRect.right-RoiRect.left;
		RoiH = RoiRect.bottom-RoiRect.top;
		RoiStep = JetAPI::GetBMPImagePixelsPerLine(RoiW, BitCount, 4);
		if ( ImageAPI.ExtractRoiImage(ImageW, ImageH, ImageStep, BitCount, ImagePtr, RoiRect, RoiStep, RoiPtr, false) == true ) 
		{
			str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("ExtractWndColor.PNG"));
			ImageAPI.SaveImage(str, RoiW, RoiH, RoiStep, BitCount, RoiPtr, true);
			JetMemory.free_func(RoiPtr);
		}
	}
#endif//_DEBUG
	if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, RoiRect, rgbv) == false )
	{	return false;	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CEditModelView::ExecCalcWndColor()//計算檢測框顏色
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE      BinaryMode = BinParamPtr->GetBinaryMode();
	CColorRGBV      *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }

	CColorRGBV   rgbv;
	if ( ExecGetWndColorFilter(rgbv) == false )
	{	return false; }
	rgbv.ExpandColorRGBV(5, false);
	AOIDataCollect.SetColorRGBVTemp(rgbv);	
	return true;
}
//-------------------------------------------------------------------------------------//
bool  CEditModelView::ExecExtractWndColorFilter()//取得檢測框抽色參數
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }
	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE      BinaryMode = BinParamPtr->GetBinaryMode();
	CColorRGBV      *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }

	CColorRGBV   rgbv;
	if ( ExecGetWndColorFilter(rgbv) == false )
	{	return false; }

	rgbv.ExpandColorRGBV(5, false);
	rgbvPtr->CheckUsed();
	rgbvPtr->CalcShowColor();
	rgbvPtr->CopyColorRGBV(rgbv);
	rgbvPtr->CheckUsed();	
	rgbvPtr->CalcShowColor();
	rgbv = *rgbvPtr;
	WndPtr->SetWndModified(true);
	ModelPtr->ApplyModelWnd(WndPtr);	
	LogOperCtrl.SaveLogModelWndAlgColorFilterOperateExtractWndColor(WndPtr);
	ProjectPtr->UpdateProjectColorGroup(BinParamPtr);	

	CAlgBinaryParam  BinaryParam = *BinParamPtr;
	BinaryParam.ClearBinaryColorList();
	rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
	BinaryParam.AddBinaryColor(rgbv);
	ExecAlgImage(ModelPtr, WndPtr, BinaryParam);
	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_ALG_COLOR_FILTER, (LPARAM)(WndPtr));	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecGatherColorFilter(bool CombineColorMode)//吸取抽色參數
{
	const char fnName[] = "CEditModelView::ExecGatherColorFilter";	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }		

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return true; }
	CAlgParam       &AlgParam = WndPtr->GetWndAlgParam();	
	CAlgBinaryParam *BinParamPtr = AlgParam.GetAlgActiveBinParamPtr();	
	BINARY_MODE    BinaryMode = BinParamPtr->GetBinaryMode();
	IMAGE_SRC_MODE ImageSourceMode = BinParamPtr->GetBinaryImageSourceMode();	
	if ( IMAGE_SRC_COLOR != ImageSourceMode ) { return true; }
	if ( BINARY_COLOR_FILTER != BinaryMode ) { return true; }		
	CColorRGBV *rgbvPtr = BinParamPtr->GetBinaryColorActivePtr();
	if ( NULL == rgbvPtr ) { return true; }
	
	CString    str;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	if ( GetCurrentFrame(ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) { return true; }
	if ( NULL == ImagePtr ) { return true; }
	if ( 24 != BitCount ) { return true; }

	bool                    bIsOK = false;
	TPOINT2D                FovCp;	
	RECT                    RoiRect={0, 0, 0, 0};
	TREGION4D               ImageRgn;	
	BOOL                    bSaved = FALSE;
		
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();	
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);	

	CColorRGBV   rgbv;
	TPOINT2D dImagePt1, dImagePt2;
	POINT nWndPt1 = m_MousePosLast;
	POINT nWndPt2 = m_MousePosFirst;	
	PtInControlWnd(m_MousePosLast, MODELEDIT_IMAGE_WND, nWndPt1);
	PtInControlWnd(m_MousePosFirst, MODELEDIT_IMAGE_WND, nWndPt2);

	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, nWndPt1, dImagePt1);	
	ImageAPI.MapWndPtToImagePt_DBL(ImageW, ImageH, m_ImageWndRect, m_ImageOffset, m_ImageZoom, nWndPt2, dImagePt2);	
	ImageRgn.minX = MIN(dImagePt1.x, dImagePt2.x);
	ImageRgn.minY = MIN(dImagePt1.y, dImagePt2.y);
	ImageRgn.maxX = MAX(dImagePt1.x, dImagePt2.x);
	ImageRgn.maxY = MAX(dImagePt1.y, dImagePt2.y);
	JetAPI::Region4DToRect(ImageRgn, RoiRect, false);
	if ( ImageAPI.CalcColorImageColorFilter(ImageW, ImageH, ImageStep, ImagePtr, RoiRect, rgbv) == false )
	{	return false;	}

	rgbv.ExpandColorRGBV(5, false);
	rgbvPtr->CheckUsed();
	rgbvPtr->CalcShowColor();	
	if ( false==CombineColorMode || rgbvPtr->CheckUsed()==false )
	{	rgbvPtr->CopyColorRGBV(rgbv);	}
	else
	{	rgbvPtr->MergeColor(false, &rgbv);	}
	rgbvPtr->CheckUsed();	
	rgbvPtr->CalcShowColor();	
	rgbv = *rgbvPtr;
	WndPtr->SetWndModified(true);
	ModelPtr->ApplyModelWnd(WndPtr);	
	ProjectPtr->UpdateProjectColorGroup(BinParamPtr);	

	CAlgBinaryParam  BinaryParam = *BinParamPtr;
	BinaryParam.ClearBinaryColorList();
	rgbv.SetLogicMode(COLOR_LOGIC_INCLUDE);
	BinaryParam.AddBinaryColor(rgbv);
	ExecAlgImage(ModelPtr, WndPtr, BinaryParam);
	SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_PROCESS_WND, WPARAM_UPDATE_ALG_COLOR_FILTER, (LPARAM)(WndPtr));	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecAlgImage(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{	
	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )//高度值需要Leveling所以不能用Field資料
	{	ExecAlgImage_Model(ModelPtr, WndPtr, BinaryParam);	}
	else
	{	ExecAlgImage_Field(ModelPtr, WndPtr, BinaryParam); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecAlgImage_Field(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{	
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::ExecAlgImage_Field Start"));	

	const int nAlign = 4;
	const bool bClone = true;
	std::vector<TUNI_FRAME> FieldUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	if ( AOIDataCollect.CopyFieldUniFrameList(FieldUniFrameList, bClone) == false )
	{	return ; }

	size_t       i=0;
	const size_t FieldUniFrameCount = FieldUniFrameList.size();
	if ( 0 == FieldUniFrameCount ) { return; }
	if ( true == bClone )
	{
		double nX=0, nY=0, nZ=0, OffsetZ=0, OverHigh=0, OverLow=0;
		nX = ModelPtr->GetModelSpaceLeveingParamX();
		nY = ModelPtr->GetModelSpaceLeveingParamY();
		nZ = ModelPtr->GetModelSpaceLeveingParamZ();
		OffsetZ = ModelPtr->GetModelSpaceLeveingOffsetZ();
		OverHigh = ModelPtr->GetModelSpaceLeveingOverHigh();
		OverLow = ModelPtr->GetModelSpaceLeveingOverLow();		
		if ( AOIDataCollect.ModifyFieldSpaceImage(FieldUniFrameList, 0, 0, nZ, OffsetZ, OverHigh, OverLow) == false )
		{
			if ( true == bClone )
			{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
			FieldUniFrameList.clear();
			return;
		}
	}
	
	RECT       WndRect={0,0,0,0};		
	RECT       ModelRect={0,0,0,0};	
	RECT       WndExtRect={0,0,0,0};	
	RECT       ModelMaskRect={0,0,0,0};
	TREGION4D  WndImageRgn;
	TREGION4D  ModelImageRgn;
	TREGION4D  WndExtImageRgn;	
	TREGION4D  WndRegionStage;
	TREGION4D  WndExtRegionStage;
	TREGION4D  ModelRegionStage;
	TREGION4D  FieldRegionStage;	
	TPOINT2D   ImageRes, FieldCpStage;	
	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);
	TUNI_FRAME FieldUniFrame = FieldUniFrameList[0];

	WndPtr->GetWndRegionStage(WndRegionStage);
	WndPtr->GetWndExtendRegionStage(WndExtRegionStage);		
	ModelPtr->GetModelTotalRegionStage(ModelRegionStage);
	FieldRegionStage = AOIDataCollect.GetFieldUniFrameStageRegion();

	IMAGE_SIZE FieldImageW = FieldUniFrame.ImageW;
	IMAGE_SIZE FieldImageH = FieldUniFrame.ImageH;
	const double FieldRegionW = FieldRegionStage.GetWidth();
	const double FieldRegionH = FieldRegionStage.GetHeight();
	
	FieldCpStage.x = FieldRegionStage.GetCpX();
	FieldCpStage.y = FieldRegionStage.GetCpY();
	ImageRes.x = FieldRegionW;
	ImageRes.y = FieldRegionH;
	ImageRes.x = ImageRes.x/FieldImageW;
	ImageRes.y = ImageRes.y/FieldImageH;

	if ( AOIDataCollect.MapStageRegionToCamera(FieldImageW, FieldImageH, ImageRes, WndRegionStage, FieldCpStage, WndImageRgn) == false || 
	 	 AOIDataCollect.MapStageRegionToCamera(FieldImageW, FieldImageH, ImageRes, WndExtRegionStage, FieldCpStage, WndExtImageRgn) == false || 
		 AOIDataCollect.MapStageRegionToCamera(FieldImageW, FieldImageH, ImageRes, ModelRegionStage, FieldCpStage, ModelImageRgn) == false )
	{
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
		FieldUniFrameList.clear();
		return; 
	}
	JetAPI::Region4DToRect(WndImageRgn, WndRect, true);
	JetAPI::Region4DToRect(ModelImageRgn, ModelRect, true);
	JetAPI::Region4DToRect(WndExtImageRgn, WndExtRect, true);
	if ( WndRect.left<0 || WndRect.top <0 || WndRect.right>FieldImageW || WndRect.bottom>FieldImageH ) 
	{
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
		FieldUniFrameList.clear();
		return; 
	}
	RECT BoundaryRect={0,0,0,0};	
	JetAPI::SizeToRect(FieldImageW, FieldImageH, BoundaryRect);
	JetAPI::BoundaryRect(BoundaryRect, ModelRect);
	if ( BINARY_DISABLE == BinaryParam.GetBinaryMode() )
	{	ModelMaskRect = ModelRect; }
	else
	{	AOIDataCollect.AdjustModelBinaryMaskRect(ModelRect, WndExtRect, ModelMaskRect);	 }
	
	const bool bTestWnd = true;
	bool       bHeightImage = false;
	MASK_PTR   ModelMaskPtr = NULL;
	IMAGE_PTR  ModelGrayPtr = NULL;
	IMAGE_SIZE ModelMaskW=0, ModelMaskH=0, ModelMaskStep=0, ModelMaskBitCount=0;	
	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
	{	bHeightImage = true;	}
	else
	{	bHeightImage = false; }
	if ( AlgParam.ExecAlgUniFrameBinary(BinaryParam, WndRect, ModelMaskRect, FieldUniFrameList, ModelMaskW, ModelMaskH, ModelMaskStep, ModelMaskBitCount, ModelMaskPtr, ModelGrayPtr, bTestWnd) == false )
	{
		if ( true == bClone )
		{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
		FieldUniFrameList.clear();		
		return;
	}	
	if ( true == bClone )
	{	JetAPI::ClearUniFrameList(FieldUniFrameList); }
	FieldUniFrameList.clear();	

#ifdef _DEBUG
	CString   str;
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("GrayImage.PNG"));
	ImageAPI.SaveImage(str, ModelMaskW, ModelMaskH, ModelMaskStep, ModelMaskBitCount, ModelGrayPtr, true);
	str.Format(_T("%s\\%s"), AOIDataCollect.GetAOITempDirectory(), _T("MaskImage.PNG"));
	ImageAPI.SaveImage(str, ModelMaskW, ModelMaskH, ModelMaskStep, ModelMaskBitCount, ModelMaskPtr, true);
#endif//_DEBUG

	const char fnName[] = "CEditModelView::ExecAlgImage_Field";		
	TREGION4D  ImageRgn;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int FrameIndex= BinaryParam.GetBinaryFrameIndex();	
	ProjectPtr->SetProjectMapIndex(FrameIndex);
	FrameIndex = ProjectPtr->GetProjectMapIndex();
	if ( GetFrameImage(FrameIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) 
	{		
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}

	IMAGE_PTR ShowImagePtr = NULL;	
	const IMAGE_SIZE ShowBitCount = 24;	
	const IMAGE_SIZE ShowStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ShowBitCount, 4);
	const TPOINT2D  FovCp = GetImageStageRgnCp();
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ImageH);

	if ( JetMemory.alloc_func(ShowBufferSize, ShowImagePtr, fnName, "ShowImagePtr") == false )
	{	
		JetMemory.free_func(ModelMaskPtr);		
		JetMemory.free_func(ShowImagePtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}		

	//一律轉成彩色計算
	if ( BufferSize == ShowBufferSize )
	{	::memcpy(ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
	else
	{
		if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, ShowStep, ShowImagePtr, false) == false )
		{	
			JetMemory.free_func(ModelMaskPtr);			
			JetMemory.free_func(ShowImagePtr);			
			JetMemory.free_func(ModelGrayPtr);
			return ;
		}
	}	
	bool bIsOK=true;	
	const bool theSameSize=true;		
	if ( BINARY_DISABLE != BinaryParam.GetBinaryMode() ) 
	{
		MASK_DATA mask = 0xFF;
		IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;
		AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
		if ( 24 == ShowBitCount )//Color Image
		{	bIsOK = ImageAPI.ColorImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelMaskPtr, mask, mskR, mskG, mskB, Alpha);	}
		else
		{	bIsOK = ImageAPI.GrayImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelMaskPtr, mask, mskV, Alpha);	}			
		if ( false == bIsOK )
		{	
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ShowImagePtr);		
			JetMemory.free_func(ModelGrayPtr);
			return;
		}
	}
	else if ( IMAGE_SRC_COLOR != BinaryParam.GetBinaryImageSourceMode() )
	{
		//將局部的灰階影像貼上FOV的灰階指標內		
		if ( false == bHeightImage )
		{
			if ( 24 == ShowBitCount )//Color Image
			{	bIsOK = ImageAPI.PasteColorRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelGrayPtr, ModelGrayPtr, ModelGrayPtr, false, theSameSize);		}
			else
			{	bIsOK = ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelMaskRect, ModelMaskStep, ModelGrayPtr, false, theSameSize);	}			
			if ( false == bIsOK )
			{					
				JetMemory.free_func(ShowImagePtr);		
				JetMemory.free_func(ModelGrayPtr);
				return;
			}
		}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
	}
	else
	{	
		const double Offset=0.0;
		const double GainValue = BinaryParam.GetGrayGainValue();
		const bool bGrayGainEnabled = BinaryParam.CheckGrayGainEnabed();
		if ( true == bGrayGainEnabled )
		{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ModelMaskRect, ShowImagePtr, Offset, GainValue);	}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
	}	
	JetMemory.free_func(ModelMaskPtr);	
	JetMemory.free_func(ModelGrayPtr);

	ReleaseShowImageBuffer();

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowImageStep = ShowStep;
	m_ShowBitCount = ShowBitCount;
	m_ShowImagePtr = ShowImagePtr;
	
	CreateBKImage();
	RedrawWnd();	

	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::ExecAlgImage_Field End"));	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::ExecAlgImage_Model(CAOIModel *ModelPtr, CAOIWnd *WndPtr, CAlgBinaryParam &BinaryParam)
{		
	if ( NULL == ModelPtr ) { return; }
	if ( NULL == WndPtr ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::ExecAlgImage_Model Start"));

	const int nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> ModelUniFrameList;
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	if ( BuildModelUniFrameList(ModelPtr, ModelUniFrameList, nAlign, true, bNoFilter) == false )
	{	return ; }

	size_t       i=0;
	const size_t ModelniFrameCount = ModelUniFrameList.size();
	if ( 0 == ModelniFrameCount ) { return; }
	
	RECT       WndRect={0,0,0,0};	
	RECT       ModelRect={0,0,0,0};	
	RECT       WndExtRect={0,0,0,0};	
	RECT       ModelMaskRect={0,0,0,0};
	TREGION4D  ModelRegion, WndRegion, WndExtRegion;
	TPOINT2D   ModelRgnCp, ImageSale, ModelImageCp;	
	TUNI_FRAME UniFrame = ModelUniFrameList[0];	
	WndPtr->GetWndRegion(WndRegion);
	WndPtr->GetWndExtendRegion(WndExtRegion);	
	ModelPtr->GetModelTotalRegion(ModelRegion);	

	IMAGE_SIZE ModelImageW = UniFrame.ImageW;
	IMAGE_SIZE ModelImageH = UniFrame.ImageH;
	const double RegionW = ModelRegion.GetWidth();
	const double RegionH = ModelRegion.GetHeight();
	
	ModelRgnCp.x = ModelRegion.GetCpX();
	ModelRgnCp.y = ModelRegion.GetCpY();
	ImageSale.x = ModelImageW;
	ImageSale.y = ModelImageH;
	ImageSale.x = ImageSale.x/RegionW;
	ImageSale.y = ImageSale.y/RegionH;	
	ModelImageCp.x = ModelImageW;
	ModelImageCp.y = ModelImageH;
	ModelImageCp.x = ModelImageCp.x*0.5;
	ModelImageCp.y = ModelImageCp.y*0.5;	

	if ( CAOIModel::CalcModelBoxRegionRect(WndRegion, ModelImageW, ModelImageH, ModelRgnCp, ImageSale, ModelImageCp, WndRect) == false ) 
	{
		JetAPI::ClearUniFrameList(ModelUniFrameList);
		return; 
	}
	if ( CAOIModel::CalcModelBoxRegionRect(WndExtRegion, ModelImageW, ModelImageH, ModelRgnCp, ImageSale, ModelImageCp, WndExtRect) == false ) 
	{
		JetAPI::ClearUniFrameList(ModelUniFrameList);
		return; 
	}

	const bool bTestWnd = true;
	bool       bHeightImage = false;
	MASK_PTR   ModelMaskPtr = NULL;
	IMAGE_PTR  ModelGrayPtr = NULL;
	IMAGE_SIZE ModelMaskW=0, ModelMaskH=0, ModelMaskStep=0, ModelMaskBitCount=0;
	JetAPI::SizeToRect(ModelImageW, ModelImageH, ModelRect);
	if ( BINARY_DISABLE == BinaryParam.GetBinaryMode() )
	{	ModelMaskRect = ModelRect; }
	else
	{	AOIDataCollect.AdjustModelBinaryMaskRect(ModelRect, WndExtRect, ModelMaskRect);	 }

	const unsigned int FrameUniqueID = BinaryParam.GetBinaryFrameUniqueID();
	if ( FRAME_UNIQUE_ID_DLP == FrameUniqueID )
	{	bHeightImage = true;	}
	else
	{	bHeightImage = false; }
	
	if ( AlgParam.ExecAlgUniFrameBinary(BinaryParam, WndRect, ModelMaskRect, ModelUniFrameList, ModelMaskW, ModelMaskH, ModelMaskStep, ModelMaskBitCount, ModelMaskPtr, ModelGrayPtr, bTestWnd) == false )
	{
		JetAPI::ClearUniFrameList(ModelUniFrameList);
		return;
	}	
	JetAPI::ClearUniFrameList(ModelUniFrameList);

	const char fnName[] = "CEditModelView::ExecAlgImage_Model";	
	TPOINT2D   FovCp;
	TREGION4D  ImageRgn;
	MASK_PTR   MaskPtr=NULL;
	SPACE_PTR  SpacePtr=NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	unsigned int FrameIndex= BinaryParam.GetBinaryFrameIndex();		
	ProjectPtr->SetProjectMapIndex(FrameIndex);
	FrameIndex = ProjectPtr->GetProjectMapIndex();
	if ( GetFrameImage(FrameIndex, ImageW, ImageH, ImageStep, BitCount, ImagePtr, SpacePtr, MaskPtr) == false ) 
	{		
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}	
	
	IMAGE_PTR ShowImagePtr = NULL;	
	RECT  ModelRectInFov={0,0,0,0};
	const IMAGE_SIZE ShowBitCount = 24;	
	const IMAGE_SIZE ShowStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, ShowBitCount, 4);		
	const TPOINT2D StageCp = GetImageStageRgnCp();	
	const TPOINT2D &ImageRes = GetImageResolution();	
	const size_t BufferSize = ImageAPI.CalcBufferSize(ImageStep, ImageH);
	const size_t ShowBufferSize = ImageAPI.CalcBufferSize(ShowStep, ImageH);

	FovCp.x = StageCp.x;
	FovCp.y = StageCp.y;	
	//模組範圍
	ModelPtr->GetModelTotalRegionStage(ModelRegion);
	if ( AOIDataCollect.MapStageRegionToCamera(ImageW, ImageH, ImageRes, ModelRegion, FovCp, ImageRgn) == false )
	{	
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}
	ModelRectInFov.left   = JetAPI::Floor(ImageRgn.minX);
	ModelRectInFov.right  = ModelRectInFov.left+ModelMaskW;
	ModelRectInFov.top    = JetAPI::Floor(ImageRgn.minY);
	ModelRectInFov.bottom = ModelRectInFov.top+ModelMaskH;
	if ( ImageAPI.CheckRoiRect(ImageW, ImageH, ModelRectInFov) == false ) 
	{	
		JetMemory.free_func(ModelMaskPtr);
		JetMemory.free_func(ModelGrayPtr);
		return;
	}	
	
	if ( JetMemory.alloc_func(ShowBufferSize, ShowImagePtr, fnName, "ShowImagePtr") == false )
	{	
		JetMemory.free_func(ModelMaskPtr);		
		JetMemory.free_func(ShowImagePtr);
		JetMemory.free_func(ModelGrayPtr);
		return; 
	}		

	//一律轉成彩色計算
	if ( BufferSize == ShowBufferSize )
	{	::memcpy(ShowImagePtr, ImagePtr, sizeof(IMAGE_DATA)*BufferSize);	}
	else
	{
		if ( ImageAPI.RGBImageToColorImage3(ImageW, ImageH, ImageStep, ImagePtr, ImagePtr, ImagePtr, ShowStep, ShowImagePtr, false) == false )
		{	
			JetMemory.free_func(ModelMaskPtr);			
			JetMemory.free_func(ShowImagePtr);			
			JetMemory.free_func(ModelGrayPtr);
			return ;
		}
	}	
	bool bIsOK=true;	
	const bool theSameSize=false;
	if ( BINARY_DISABLE != BinaryParam.GetBinaryMode() ) 
	{
		MASK_DATA mask = 0xFF;
		IMAGE_DATA mskR=0xFF, mskG=0xFF, mskB=0x00, mskV=0xFF, Alpha=0;	
		MASK_PTR  ShowMaskPtr = NULL;
		const IMAGE_SIZE MaskBitCount = 8;
		const IMAGE_SIZE MaskStep = JetAPI::GetBMPImagePixelsPerLine(ImageW, MaskBitCount, 4);
		const size_t MaskBufferSize = ImageAPI.CalcBufferSize(MaskStep, ImageH);
		if ( JetMemory.alloc_func(MaskBufferSize, ShowMaskPtr, fnName, "ShowMaskPtr") == false )
		{	
			JetMemory.free_func(ModelMaskPtr);			
			JetMemory.free_func(ShowImagePtr);
			JetMemory.free_func(ModelGrayPtr);
			return;
		}
		//將局部的二值化影像貼上FOV的二值化指標內
		::memset(ShowMaskPtr, 0x00, sizeof(MASK_DATA)*MaskBufferSize);
		if ( ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, MaskStep, ShowMaskPtr, ModelRectInFov, ModelMaskStep, ModelMaskPtr, false, theSameSize) == false )
		{	
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ShowMaskPtr);
			JetMemory.free_func(ShowImagePtr);
			JetMemory.free_func(ModelGrayPtr);
			return; 
		}
		AOIDataCollect.GetMaskImageColor(FrameUniqueID, mskR, mskG, mskB, mskV, Alpha);
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
		if ( 24 == ShowBitCount )//Color Image
		{	bIsOK = ImageAPI.ColorImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, MaskStep, ShowMaskPtr, mask, mskR, mskG, mskB, Alpha);	}
		else
		{	bIsOK = ImageAPI.GrayImageApplyMask(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, MaskStep, ShowMaskPtr, mask, mskV, Alpha);	}	
		JetMemory.free_func(ShowMaskPtr);
		if ( false == bIsOK )
		{	
			JetMemory.free_func(ModelMaskPtr);
			JetMemory.free_func(ShowImagePtr);		
			JetMemory.free_func(ModelGrayPtr);
			return;
		}		
	}
	else if ( IMAGE_SRC_COLOR != BinaryParam.GetBinaryImageSourceMode() )
	{
		//將局部的灰階影像貼上FOV的灰階指標內		
		if ( false == bHeightImage )
		{
			if ( 24 == ShowBitCount )//Color Image
			{	bIsOK = ImageAPI.PasteColorRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, ModelMaskStep, ModelGrayPtr, ModelGrayPtr, ModelGrayPtr, false, theSameSize);		}
			else
			{	bIsOK = ImageAPI.PasteGrayRoiImage3(ImageW, ImageH, ShowStep, ShowImagePtr, ModelRectInFov, ModelMaskStep, ModelGrayPtr, false, theSameSize);	}			
			if ( false == bIsOK )
			{					
				JetMemory.free_func(ShowImagePtr);		
				JetMemory.free_func(ModelGrayPtr);
				return;
			}
		}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);
	}
	else
	{
		const double Offset=0.0;
		const double GainValue = BinaryParam.GetGrayGainValue();
		const bool bGrayGainEnabled = BinaryParam.CheckGrayGainEnabed();
		if ( true == bGrayGainEnabled )
		{	ImageAPI.ImageOffsetGain3(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ModelRectInFov, ShowImagePtr, Offset, GainValue);	}
		AOIDataCollect.ExecEnhanceDisplayImage(ImageW, ImageH, ShowStep, ShowBitCount, ShowImagePtr, ShowImagePtr);	
	}	
	JetMemory.free_func(ModelMaskPtr);	
	JetMemory.free_func(ModelGrayPtr);

	ReleaseShowImageBuffer();

	m_ShowImageW = ImageW;
	m_ShowImageH = ImageH;
	m_ShowImageStep = ShowStep;
	m_ShowBitCount = ShowBitCount;
	m_ShowImagePtr = ShowImagePtr;
	
	CreateBKImage();
	RedrawWnd();	

	AOIDataCollect.SaveUIDrawFuncLog(_T("CEditModelView::ExecAlgImage_Model End"));	
	return ;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelWndInspection(bool UpdateUI)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	const bool MultiSelectMode = AOIDataCollect.CheckMultiSelectMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode ) { return true; }	

	const int nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> UniFrameList;
	if ( BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, true, bNoFilter) == false )
	{	return false; }
	const size_t UniFrameCount = UniFrameList.size();
	if ( 0 == UniFrameCount ) 
	{	return true; }

	const double AttachedAngle = ModelPtr->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);

	UUID           WndUUID = ProjectPtr->GetProjectActiveModelWndUUID();
	CAOIWnd       *WndPtr = ModelPtr->GetModelWndPtrByUUID(WndUUID);	
	WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) 
	{
		JetAPI::ClearUniFrameList(UniFrameList);
		if ( false==MultiSelectMode && true == UpdateUI )
		{	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);	 }
		return false; 
	}
	CAOIWndRoi  *WndRoiPtr = WndPtr->GetWndRoiWndActived();
	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL!=WndRoiPtr || NULL!=MaskWndPtr )
	{
		JetAPI::ClearUniFrameList(UniFrameList);		
		if ( false==MultiSelectMode && true == UpdateUI )
		{				
			SendMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, (LPARAM)(WndPtr));
			//PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);	 
		}
		return true; 
	}

	size_t     i=0;	
	TREGION4D  ModelRgn;	
	TPOINT2D   RgnCp, Scale, ImageCp;	
	ModelPtr->GetModelTotalRegion(ModelRgn);
	CAlgParam &AlgParam = WndPtr->GetWndAlgParam();	
	TUNI_FRAME UniFrame = UniFrameList[0];
	const IMAGE_SIZE ImageW = UniFrameList[0].ImageW;
	const IMAGE_SIZE ImageH = UniFrameList[0].ImageH;
	const double RegionW = ModelRgn.GetWidth();
	const double RegionH = ModelRgn.GetHeight();	
	TPOINT2D  ModelImageScale = ModelPtr->GetModelImageScale();
	CAOILand *LandPtr = WndPtr->GetWndLandPtr();	

	WndPtr->InitWndInspection(false);
	if ( false == IsExceptionAngle )
	{
		Scale.x = ImageW;
		Scale.y = ImageH;
		Scale.x = Scale.x/RegionW;
		Scale.y = Scale.y/RegionH;	
		RgnCp.x = ModelRgn.GetCpX();
		RgnCp.y = ModelRgn.GetCpY();
		ImageCp.x = ImageW;
		ImageCp.y = ImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;	
		ModelPtr->SetModelImageScale(Scale);
		WndPtr->ExecWndInspection(ModelPtr, ModelRgn, RgnCp, Scale, ImageCp, UniFrameList);	
		JetAPI::ClearUniFrameList(UniFrameList);
	}
	else
	{
		TPOINT2D  ModelCornerPts[4];
		TREGION4D ModelRgnRotated;
		std::vector<TUNI_FRAME> UniFrameListDst;

		ModelPtr->GetModelTotalCornerPts(ModelCornerPts);
		JetAPI::RotateCornerPos(-AttachedAngle, 0, 0, ModelCornerPts);
		JetAPI::CornerPtToRegion(ModelCornerPts, ModelRgnRotated);
		if ( CAOIModel::RotateModelUniFrameList(-AttachedAngle, ModelRgn, ModelRgnRotated, UniFrameList, UniFrameListDst) == false )
		{			
			JetAPI::ClearUniFrameList(UniFrameList);
			return false;
		}
		JetAPI::ClearUniFrameList(UniFrameList);
		const IMAGE_SIZE ModelImageW = UniFrameListDst[0].ImageW;
		const IMAGE_SIZE ModelImageH = UniFrameListDst[0].ImageH;
		const double RgnWRotated = ModelRgnRotated.GetWidth();
		const double RgnHRotated = ModelRgnRotated.GetHeight();
		Scale.x = ModelImageW;
		Scale.y = ModelImageH;
		Scale.x = Scale.x/RgnWRotated;
		Scale.y = Scale.y/RgnHRotated;	
		RgnCp.x = ModelRgnRotated.GetCpX();
		RgnCp.y = ModelRgnRotated.GetCpY();
		ImageCp.x = ModelImageW;
		ImageCp.y = ModelImageH;
		ImageCp.x = ImageCp.x*0.5;
		ImageCp.y = ImageCp.y*0.5;	
		ModelPtr->SetModelImageScale(Scale);		
		WndPtr->RotateWnd(-AttachedAngle, 0, 0);
		WndPtr->ExecWndInspection(ModelPtr, ModelRgnRotated, RgnCp, Scale, ImageCp, UniFrameListDst);	
		WndPtr->RotateWnd(AttachedAngle, 0, 0);
		JetAPI::ClearUniFrameList(UniFrameListDst);
	}
	ModelPtr->SetModelImageScale(ModelImageScale);
	if ( false==MultiSelectMode && true == UpdateUI )
	{	PostMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);	}
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::SendOutUpdatePartList(int UpdateList, int UpdateWnd)
{	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	ProjectPtr->SetProjectActiveModelWnd(NULL);

	switch ( UpdateList )
	{
	case MSG_MODE_BUILD:
		SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_BUILD_PART_LIST, LPARAM_BUILD_DOCK_LIST_ALL);
		break;
	case MSG_MODE_UPDATE:
		SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, TREE_CTRL_UPDATE_NULL);
		break;	
	}	

	switch ( UpdateWnd )
	{
	case MSG_MODE_BUILD:
		SendMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_BUILD_WND_SELECTED, NULL);
		break;
	case MSG_MODE_UPDATE:
		SendMessageToMainFrameWnd(MSG_EDIT_WND_PROPERTY_WND, WPARAM_UPDATE_WND_SELECTED, NULL);
		break;
	}
	return;
}
//-------------------------------------------------------------------------------------//
BOOL CEditModelView::CheckInEditMode(bool bCheckType)//確認是否可以編輯
{
	if ( NULL == m_ModelPtr ) { return FALSE; }	
	if ( m_ModelPtr->GetModelEditMode() == false ) { return FALSE; }		
	if ( true == bCheckType )
	{
		if ( m_ModelPtr->CheckModelTypeEnabled() == false )
		{	return FALSE; }
	}
	return TRUE;

	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT != DrawModelMode )
	{	return FALSE;	}	
	return TRUE;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowModelEditMode() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	UpdateActiveObjList();
	RedrawWnd();	
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_SELECTED, NULL);	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowModelEditMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_EDIT == DrawModelMode ) 
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowModelResultMode() 
{
	// TODO: Add your command handler code here
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);
	UpdateActiveObjList();
	RedrawWnd();	
	PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_MODEL_SELECTED, NULL);	
	//PostMessageToMainFrameWnd(MSG_EDIT_IMAGE_VIEW_WND, WPARAM_UPDATE_IMAGE_WND_SELECTED, NULL);	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowModelResultMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DRAW_MODEL_MODE DrawModelMode = GetDrawModelMode();
	if ( DRAW_MODEL_RESULT == DrawModelMode ) 
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowModelLands() 
{
	// TODO: Add your command handler code here	
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return ; }
	bool bShow = true;

	ModelPtr->UnSelectModel();	
	ModelPtr->InvisibleModelWnd();
	ModelPtr->SetModelWndActived(NULL);
	ModelPtr->SetModelLandActived(NULL);
	if ( true == bShow )
	{
		ModelPtr->VisibleModelLand(false);
		ModelPtr->SetModelBodyBoxActived(true);		
	}
	else
	{
		ModelPtr->InvisibleModelLand(false);
		ModelPtr->SetModelBodyBoxActived(true);		
	}	
	ProjectPtr->SetProjectActiveModelWnd(NULL);
	ProjectPtr->SetProjectActiveModelLand(NULL);
	AOIDataCollect.SetShowModelLandBox(bShow);
	UpdateModelStats();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowModelLands(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->SetCheck(AOIDataCollect.GetShowModelLandBox());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnHideModelLands()
{
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return; }
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr ) { return ; }
	bool bShow = false;

	ModelPtr->UnSelectModel();	
	ModelPtr->InvisibleModelWnd();
	ModelPtr->SetModelWndActived(NULL);
	ModelPtr->SetModelLandActived(NULL);
	if ( true == bShow )
	{
		ModelPtr->VisibleModelLand(false);
		ModelPtr->SetModelBodyBoxActived(true);		
	}
	else
	{
		ModelPtr->InvisibleModelLand(false);
		ModelPtr->SetModelBodyBoxActived(true);		
	}	
	ProjectPtr->SetProjectActiveModelWnd(NULL);
	ProjectPtr->SetProjectActiveModelLand(NULL);
	AOIDataCollect.SetShowModelLandBox(bShow);
	UpdateModelStats();
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateHideModelLands(CCmdUI* pCmdUI)
{
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowAllComponents()
{
	DRAW_COMPONENT_MODE DrawComponentMode = AOIDataCollect.GetDrawComponentMode();
	switch ( DrawComponentMode )
	{
	case DRAW_COMPONENT_ALL:	DrawComponentMode = DRAW_COMPONENT_FOCUSED;	break;
	case DRAW_COMPONENT_FOCUSED:	DrawComponentMode = DRAW_COMPONENT_ALL;	break;
	}
	AOIDataCollect.SetDrawComponentMode(DrawComponentMode);	
	CEditModelView::RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowAllComponents(CCmdUI* pCmdUI)
{
	DRAW_COMPONENT_MODE DrawComponentMode = AOIDataCollect.GetDrawComponentMode();
	if ( DRAW_COMPONENT_ALL == DrawComponentMode )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowModelWndIndex()
{
	bool bShowWndIndex = AOIDataCollect.GetShowModelWndIndex();
	if ( true == bShowWndIndex ) { bShowWndIndex = false; }
	else { bShowWndIndex = true; }
	AOIDataCollect.SetShowModelWndIndex(bShowWndIndex);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowModelWndIndex(CCmdUI* pCmdUI)
{
	bool bShowWndIndex = AOIDataCollect.GetShowModelWndIndex();
	if ( true == bShowWndIndex )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowModelLandIndex()
{
	bool bShowLandIndex = AOIDataCollect.GetShowModelLandIndex();
	if ( true == bShowLandIndex ) { bShowLandIndex = false; }
	else { bShowLandIndex = true; }
	AOIDataCollect.SetShowModelLandIndex(bShowLandIndex);
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowModelLandIndex(CCmdUI* pCmdUI)
{
	bool bShowLandIndex = AOIDataCollect.GetShowModelLandIndex();
	if ( true == bShowLandIndex )
	{	pCmdUI->SetCheck(TRUE); }
	else
	{	pCmdUI->SetCheck(FALSE); }
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecComponentModelToLibrary()//將零件模組更新至資料庫內
{
	CAOIProject *Project = GetActiveProject();
	if ( NULL == Project ) { return true; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return true; }	
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return true; }
	AOIDataCollect.CloseActiveComponent(ComponentPtr);
	//ComponentPtr->SetComponentSelected(true);	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnEditModelBkImage() 
{
	// TODO: Add your command handler code here
	if ( GetShowProjectMapMode() == true ) { return; }
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )	{	return;	}	
	CAOIModel *ModelPtr = GetModelPtr();	
	if ( NULL == ModelPtr )	{	return;	}	
	if ( ModelPtr->GetModelUnsetState() == true ) { return; }	
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr ) { return; }
	const double ModelSizeW = ComponentPtr->GetComponentRoiSizeW();
	const double ModelSizeH = ComponentPtr->GetComponentRoiSizeH();
	const double ResX = AOIDataCollect.GetCameraResolutionX(PRIMARY_CAMERA_ID);
	const double ResY = AOIDataCollect.GetCameraResolutionY(PRIMARY_CAMERA_ID);	
	const double ComponentAngle = ComponentPtr->GetComponentAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);

	size_t     i=0;
	CString    str;
	bool       IsOK = true;
	const bool bClone=true;
	IMAGE_SIZE DstW=0, DstH=0, DstStep=0, DstBitCount=0;
	IMAGE_SIZE ImageW=0, ImageH=0, ImageStep=0, BitCount=0;	
	IMAGE_PTR  DstPtr = NULL;
	IMAGE_PTR  ImagePtr = NULL;
	IMAGE_PTR  ImagePtr2 = NULL;	
	IMAGE_SIZE ClipW=0, ClipH=0, ClipStep=0;
	IMAGE_PTR  ClipImagePtr = NULL;
	MASK_PTR   MaskPtr = NULL;
	SPACE_PTR  SpacePtr = NULL;	
	RECT       RoiRect={0,0,0,0};
	RECT       ClipRect={0,0,0,0};
	CString    ModelFolder;
	CString    ModelBkImageName;
	const int  nAlign = 4;
	const bool bNoFilter = false;
	std::vector<TUNI_FRAME> UniFrameList;
	if ( BuildModelUniFrameList(ModelPtr, UniFrameList, nAlign, bClone, bNoFilter) == false ) { return ; }
	const size_t FrameCount = UniFrameList.size();
	const double SpaceRatio = AOIDataCollect.GetSpaceToGrayRatio();
	BOOL         bSaved=FALSE;
	for ( i=0; i<FrameCount; i++ )
	{	
		ImageW = UniFrameList[i].ImageW;
		ImageH = UniFrameList[i].ImageH;
		ImageStep = UniFrameList[i].ImageStep;
		BitCount = UniFrameList[i].BitCount;
		ImagePtr = UniFrameList[i].ImagePtr;
		MaskPtr = UniFrameList[i].MaskPtr;
		SpacePtr = UniFrameList[i].SpacePtr;

		ModelFolder      = ModelPtr->GetModelFolder();
		ModelBkImageName = ModelPtr->GetModelBKImageFilename(i);		
		JetAPI::CreateFolder(ModelFolder);

		if ( NULL==MaskPtr || NULL==SpacePtr )
		{			
			if ( ImageAPI.RotateImage(-ComponentAngle, ImageW, ImageH, ImageStep, BitCount, ImagePtr, DstW, DstH , DstStep, DstPtr) == false ) { continue; }			
		}
		else
		{
			BitCount = 8;
			RoiRect.left = 0;
			RoiRect.top  = 0;
			RoiRect.right = (int)(ImageW);
			RoiRect.bottom = (int)(ImageH);
			if ( ImageAPI.SpaceGrayImageConvertToGray(ImageW, ImageH, ImageStep, SpacePtr, MaskPtr, RoiRect, ImageStep, ImagePtr2, SpaceRatio, false) == false ) { continue; }
			if ( ImageAPI.RotateImage(-ComponentAngle, ImageW, ImageH, ImageStep, BitCount, ImagePtr2, DstW, DstH , DstStep, DstPtr) == false )
			{	
				JetMemory.free_func(ImagePtr2);
				continue; 
			}
			JetMemory.free_func(ImagePtr2);			
		}

		if ( false == IsExceptionAngle )
		{	
			IsOK = ImageAPI.SaveImage(ModelBkImageName, DstW, DstH, DstStep, BitCount, DstPtr, true);	
			JetMemory.free_func(DstPtr);
		}
		else
		{
		#ifdef _DEBUG
			if ( TRUE == bSaved )
			{
				str.Format(_T("%s\\Model%d_Rotated.PNG"), AOIDataCollect.GetAOITempDirectory(), i+1);
				ImageAPI.SaveImage(str, DstW, DstH, DstStep, BitCount, DstPtr, true);	
			}
		#endif

			ClipW = JetAPI::Floor(ModelSizeW/ResX);
			ClipH = JetAPI::Floor(ModelSizeH/ResY);			
			ClipRect.left   = (DstW-ClipW)/2;
			ClipRect.top    = (DstH-ClipH)/2;
			ClipRect.right  = ClipRect.left+ClipW;
			ClipRect.bottom = ClipRect.top+ClipH;
			ClipStep = JetAPI::GetBMPImagePixelsPerLine(ClipW, BitCount, 4);

			if ( ImageAPI.ExtractRoiImage(DstW, DstH, DstStep, BitCount, DstPtr, ClipRect, ClipStep, ClipImagePtr, false) == false )
			{	
				JetMemory.free_func(DstPtr);
				continue;
			}
			JetMemory.free_func(DstPtr);
			
			IsOK = ImageAPI.SaveImage(ModelBkImageName, ClipW, ClipH, ClipStep, BitCount, ClipImagePtr, true);	
			JetMemory.free_func(ClipImagePtr);
		}
		ModelPtr->SetModelNeedSaveFiles(true);
		ModelPtr->SetModelBKImageNeedToGrab(false);		
	}
	//重新整理底圖
	const bool bModelBkImage=AOIDataCollect.GetAutoArrangeModelBkImageFiles();
	if ( true == bModelBkImage )
	{	ModelPtr->ArrangeModelBKImageFiles(); }
	if ( true == bClone )
	{	JetAPI::ClearUniFrameList(UniFrameList); }

	PostMessageToMainFrameWnd(MSG_EDIT_LIBRARY_WND, WPARAM_UPDATE_MODEL_LIST_ICON, NULL);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateEditModelBkImage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnEditModelBkImageAll()
{
	// TODO: Add your command handler code here
	CString str;	
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( true == bLockUIWnd || NULL==ProjectPtr ) { return; }	
	CAOIModel *ModelPtr = GetModelPtr();
	AOIDataCollect.CloseActiveModel(ModelPtr);

	str = _T("Do you want to save all model bk image?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) != IDYES )
	{	return ; }

	if ( true == IsNeedGrabFiducial )
	{	ProjectPtr->SelectProjectAllFds(true);	 }
	else
	{	ProjectPtr->SelectProjectAllFds(false);	 }			
	ProjectPtr->SelectProjectAllBarcodes(false);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SelectProjectComponentsForModelBkImage();
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);
	
	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetSaveModelBkImage(true);
	AOIDataCollect.SetRepeatedTestCount(0);
	AOIDataCollect.SetRepeatedTestMaxCount(0);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetInspectingMode(INSPECTING_TUNNING);
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}	
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateEditModelBkImageAll(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditCloneArrayPaste() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }
	size_t       i=0;	
	TPOINT2D     psOffset;	
	CAOILand    *LandPtr = ModelPtr->GetModelLandActivted();	
	const bool   bLinkMode = CheckIsLinkMode();
	const double ComponentAngle = ModelPtr->GetModelAttachedAngle();	
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(ComponentAngle);
	psOffset.x = 2000;
	psOffset.y = 1000;
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_EDIT);
	if ( NULL != LandPtr )
	{	
		ModelPtr->CloneMoveModelLandSelected(psOffset.x, psOffset.y, true);		
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}
	/*
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL != WndPtr )
	{		
		Toward = WndPtr->GetWndToward();
		if ( true == IsExceptionAngle )
		{	Toward = CAOIBox::RotateToward(-ComponentAngle, Toward);	}
		switch ( Toward )
		{
		case BOX_TOWARD_UP:				
		case BOX_TOWARD_DOWN:
			psOffset.y = 0;
			break;
		case BOX_TOWARD_LEFT:
		case BOX_TOWARD_RIGHT:
			psOffset.x = 0;
			break;			
		}		
		if ( ModelPtr->CloneModelWndSelected(bLinkMode) == false )
		{	return ; }
		LogOperCtrl.SaveLogModelWndSelectedClone(ModelPtr);
		if ( ModelPtr->MoveModelWndSelected(Toward, psOffset.x, psOffset.y) == false )
		{	return; }
		UpdateModelStats();
		RedrawWnd();
		SendOutUpdatePartList();
		return;
	}	*/
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditCloneArrayPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->SetCheck(MANIPULATE_MODEL_EDIT==AOIDataCollect.GetManipulateModelMode());
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnShowModelActivedLine() 
{
	// TODO: Add your command handler code here
	bool bShow = AOIDataCollect.GetDrawModelActivedLine();
	if ( true == bShow ) 
	{	AOIDataCollect.SetDrawModelActivedLine(false); }
	else
	{	AOIDataCollect.SetDrawModelActivedLine(true); }
	RedrawWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateShowModelActivedLine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(TRUE==AOIDataCollect.GetDrawModelActivedLine());
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelLinkLandWndPos() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr )  { return; }
	const int LinkMask = MODEL_LINK_LAND_WND_POS;
	const bool bLink = ModelPtr->CheckModelWndLinkMode(LinkMask);
	if ( false == bLink )
	{	ModelPtr->AddModelWndLinkMode(LinkMask);	}
	else
	{	ModelPtr->RemoveModelWndLinkMode(LinkMask);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelLinkLandWndPos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL==ModelPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }
	else
	{	
		const bool bLink = ModelPtr->CheckModelWndLinkMode(MODEL_LINK_LAND_WND_POS);
		pCmdUI->SetCheck(bLink);
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelLinkLandWndSize() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr )  { return; }
	const int LinkMask = MODEL_LINK_LAND_WND_SIZE;	
	const bool bLink = ModelPtr->CheckModelWndLinkMode(LinkMask);
	if ( false == bLink )
	{	ModelPtr->AddModelWndLinkMode(LinkMask);	}
	else
	{	ModelPtr->RemoveModelWndLinkMode(LinkMask);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelLinkLandWndSize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();		
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL==ModelPtr || true==bLockUIWnd ) 
	{ pCmdUI->Enable(FALSE); }
	else
	{	
		const bool bLink = ModelPtr->CheckModelWndLinkMode(MODEL_LINK_LAND_WND_SIZE);
		pCmdUI->SetCheck(bLink);
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelLinkLandPos() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr )  { return; }
	const int LinkMask = MODEL_LINK_LAND_POS;
	const bool bLink = ModelPtr->CheckModelWndLinkMode(LinkMask);
	if ( false == bLink )
	{	ModelPtr->AddModelWndLinkMode(LinkMask);	}
	else
	{	ModelPtr->RemoveModelWndLinkMode(LinkMask);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelLinkLandPos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();		
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL==ModelPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }
	else
	{		
		const bool bLink = ModelPtr->CheckModelWndLinkMode(MODEL_LINK_LAND_POS);		
		pCmdUI->SetCheck(bLink);
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelLinkLandSize() 
{
	// TODO: Add your command handler code here
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL == ModelPtr )  { return; }
	const int LinkMask = MODEL_LINK_LAND_SIZE;	
	const bool bLink = ModelPtr->CheckModelWndLinkMode(LinkMask);
	if ( false == bLink )
	{	ModelPtr->AddModelWndLinkMode(LinkMask);	}
	else
	{	ModelPtr->RemoveModelWndLinkMode(LinkMask);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelLinkLandSize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();		
	CAOIModel *ModelPtr = CEditModelView::GetModelPtr();
	if ( NULL==ModelPtr || true==bLockUIWnd ) 
	{	pCmdUI->Enable(FALSE); }
	else
	{		
		const bool bLink = ModelPtr->CheckModelWndLinkMode(MODEL_LINK_LAND_SIZE);		
		pCmdUI->SetCheck(bLink);
	}
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecInspection_Finish()
{	
	SwitchProject();
	m_UpdateTestMapTickCount = 0;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }			
	
	CString   OfflineFdName;
	CString   OfflineFolder;
	size_t    RepeatCount=0;
	size_t    MaxRepeatCount=0;
	bool      RepeatTest=false;
	bool      OfflineMode = AOIDataCollect.GetOfflineMode();
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode(); 			
	OFFLINE_FILE_MODE OfflineFileMode = ProjectPtr->GetProjectOfflineFileMode();
	AOIDataCollect.SetModelAttachedObj(MODEL_ATTACHED_COMPONENT);
	
	switch ( TaskMode )
	{
	case TASK_ALIGN_PROJECT:		
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);		
		break;
	case TASK_INSPECT_PROJECT:
		RepeatTest = true;
		SendMessageToMainFrameWnd(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);
		break;
	case TASK_TUNING_PROJECT:
	case TASK_TUNING_OFFLINE:		
		if ( AOIDataCollect.GetIsRepeatTest() == true )
		{			
			AOIDataCollect.AddRepeatedTestCount();
			RepeatCount = AOIDataCollect.GetRepeatedTestCount();
			MaxRepeatCount = AOIDataCollect.GetRepeatedTestMaxCount();
			if ( 0==MaxRepeatCount || RepeatCount<MaxRepeatCount )
			{	RepeatTest = true;	}
			else
			{	
				AOIDataCollect.SetIsRepeatTest(false); 
				AOIDataCollect.SetIsRepeatTestUI(false); 
			}

			if ( true == RepeatTest )
			{
				AOIDataCollect.WaitForThreadSequenceThreadFinish();
				AOIDataCollect.SetIsNeedGrabFiducial(true);
				if ( AOIDataCollect.GetShowUIWndResultList() == true ) 
				{	SendMessageToMainFrameWnd(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL); }
				//PostMessageToMainFrameWnd(MSG_INSPECTION_CALLBACK, WPARAM_INSPECTION_RE_TUNING, NULL);				
				if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
				{
					LockUIWnd(false);
					JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
					return false;
				}				
			}
		}		
		break;
	default:		
		break;
	}		
	
	if ( false == RepeatTest )
	{	ExecInspectionFinishKernel(false);	}	
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecOnlineInspection_Finish()
{
	ExecInspectionFinishKernel(true);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecInspectionFinishKernel(bool bOnline)
{
	LockUIWnd(false);
	if ( true == bOnline )
	{	
		AOIDataCollect.SetOfflineMode(false);
		AOIDataCollect.SetProjectMapMode(false);
		AOIDataCollect.ExecOnlineInspectionFinish();
	}
	CAOIProject *ProjectPtr = CEditModelView::GetActiveProject();
	if ( NULL == ProjectPtr ) { return true; }
	AOIDataCollect.SetDrawModelMode(DRAW_MODEL_RESULT);

	bool bShowResultWnd = AOIDataCollect.GetShowUIWndResultList();
	const size_t DefectComponentCount = ProjectPtr->GetProjectDefectComponentCount();
	SendMessageToMainFrameWnd(MSG_EDIT_PART_LIST_WND, WPARAM_UPDATE_PART_LIST, NULL);
	//SendMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_UPDATE, (LPARAM)(this));
	if ( true==bOnline || AOIDataCollect.GetInspectionFinishShowResultList() == true )		
	{		
		if ( DefectComponentCount==0 || true==bShowResultWnd ) 
		{	SendMessageToMainFrameWnd(MSG_EDIT_RESULT_LIST_WND, WPARAM_BUILD_RESULT_LIST, NULL);	}
		else
		{	SendMessageToMainFrameWnd(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_RESULT_LIST_DOCK_PANE, TRUE); }
	}

	if ( AOIDataCollect.SetProjectLightSetting(ProjectPtr) == false )
	{	JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());	}
	if ( AOIDataCollect.MoveStageToInspectionFinishComponent(ProjectPtr) == false )
	{	ExecMoveToStage(); }
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::LockUIWnd(bool bLock)//鎖住視窗
{
	AOIDataCollect.SetIsLockUIWnd(bLock);
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::GetLockUIWnd() const//取得是否鎖住UIWnd
{
	return AOIDataCollect.GetIsLockUIWnd();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneRepeateInspection() 
{
	// TODO: Add your command handler code here
	bool bRepeatTest = AOIDataCollect.GetIsRepeatTestUI();
	if ( true == bRepeatTest ) 
	{	bRepeatTest = false; }
	else
	{	bRepeatTest = true; }
	AOIDataCollect.SetIsRepeatTestUI(bRepeatTest);
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode(); 
	if ( TASK_TUNING_PROJECT==TaskMode || TASK_TUNING_OFFLINE==TaskMode )
	{	AOIDataCollect.SetIsRepeatTest(bRepeatTest);	}	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneRepeateInspection(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	TASK_MODE TaskMode = AOIDataCollect.GetTaskMode(); 
	const bool bIsRepeat = AOIDataCollect.GetIsRepeatTestUI();
	if ( TASK_TUNING_PROJECT==TaskMode || TASK_TUNING_OFFLINE==TaskMode )
	{	pCmdUI->Enable(TRUE); 	}	
	else
	{	pCmdUI->Enable(FALSE);	}
	pCmdUI->SetCheck(bIsRepeat);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneSelectedPartNumber() 
{
	// TODO: Add your command handler code here
	CString str;		
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();	
	const bool ChangeRibbonTuneID = AOIDataCollect.GetChangeRibbonTuneID();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecComponentModelToLibrary();
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	if ( NULL == ComponentPtr ) { return; }	
	if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { return; }

	CString PartNumber = ComponentPtr->GetComponentPartNumber();
	ProjectPtr->SelectProjectAllFds(false);
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByPartNumber(PartNumber, false);
	ProjectPtr->SelectProjectFdForTuning(OfflineMode, IsNeedGrabFiducial);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectActiveComponent(true);
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);	

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetChangeRibbonTuneID(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetInspectingMode(INSPECTING_SELECTED);
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}		
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);
	CreateMapImage();
	if ( true == ChangeRibbonTuneID )
	{	AOIDataCollect.SetRibbonTuneGroupCmdID(ID_TUNE_SELECTED_PART_NUMBER);	 }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneSelectedPartNumber(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneSelectedModelGroup()
{
	CString str;		
	const bool OfflineMode = AOIDataCollect.GetOfflineMode();
	bool IsNeedGrabFiducial = AOIDataCollect.GetIsNeedGrabFiducial();	
	const bool ChangeRibbonTuneID = AOIDataCollect.GetChangeRibbonTuneID();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return ; }
	ExecComponentModelToLibrary();
	CAOIModel     *ModelPtr = NULL;
	CAOIComponent *ComponentPtr = ProjectPtr->GetProjectActiveComponent();	
	if ( NULL == ComponentPtr ) { return; }	
	if ( ComponentPtr->CheckComponentType_ModelTest() == false ) { return; }

	ModelPtr = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CString ModelGroupName = ModelPtr->GetModelGroupName();
	ProjectPtr->SelectProjectAllFds(false);
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectComponentsByModelGroup(ModelGroupName, false);	
	ProjectPtr->SelectProjectFdForTuning(OfflineMode, IsNeedGrabFiducial);
	ProjectPtr->ResetProjectOnlineTuningDateTime();
	ProjectPtr->SetProjectSaveSpcPartImageMode(SAVE_SPC_PART_IMAGE_DISABLE);
	ProjectPtr->UpdateSelectObjToNeedToCalculateRgn();
	ProjectPtr->SelectProjectAllComponents(false);
	ProjectPtr->SelectProjectActiveComponent(true);
	ProjectPtr->SetProjectSpcFileSaveEnabled(false);	

	AOIDataCollect.SetIsRepeatTest(false);
	AOIDataCollect.SetIsRepeatTestUI(false);
	AOIDataCollect.SetChangeRibbonTuneID(true);
	AOIDataCollect.SetCallbackWnd(CWnd::GetSafeHwnd());	
	AOIDataCollect.SetInspectingMode(INSPECTING_SELECTED);
	AOIDataCollect.ReleaseModelUniFrameList();
	SendMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_PROJECT_CLOSE_ACTIVE_OBJ, NULL);
	if ( AOIDataCollect.GetOfflineMode() == false )
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_PROJECT);	}
	else
	{	AOIDataCollect.SetTaskMode(TASK_TUNING_OFFLINE);	}		
	if ( AOIDataCollect.ExecInspectionProjectTuning() == false )
	{
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());
		return;
	}
	LockUIWnd(true);
	CreateMapImage();
	if ( true == ChangeRibbonTuneID )
	{	AOIDataCollect.SetRibbonTuneGroupCmdID(ID_TUNE_SELECTED_MODEL_GROUP);	 }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneSelectedModelGroup(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::RetrieveCameraImage(WPARAM wParam, LPARAM lParam, bool bCameraCallBack)//取得相機影像
{
	CString str;	
	size_t   i=0;	
	TSIZE2D  Res;
	TPOINT3D Pos;
	IMAGE_SIZE ImageStep=0;
	IMAGE_SIZE BitCount=0;
	size_t   BuffserSize=0;
	CAMERA_ID CameraID = (CAMERA_ID)(wParam);	
	if ( PRIMARY_CAMERA_ID != CameraID ) { return false; }
	if ( LPARAM_CAMERA_BYPASS_CALLBACK == lParam )
	{	return true;	}
	
	bool bReturn=false;
	if ( CameraCtrl.RetrieveCameraImageCallback(CameraID, bReturn) == false )
	{
		JetAPI::ShowMessageBox(CameraCtrl.GetErrorString());		
		return false; 
	}
	if ( true == bReturn )
	{	return true; }

	LockUIWnd(false);
	TPOINT2D ImageRes;
	const double Ratio = 1.0;
	double PosX=0, PosY=0, PosZ=0;
	const double FOVWum = AOIDataCollect.GetFovSizeRealW();
	const double FOVHum = AOIDataCollect.GetFovSizeRealH();	
	const double FovMinW = AOIDataCollect.GetFovSizeMinW_Zoom();
	const double FovMinH = AOIDataCollect.GetFovSizeMinH_Zoom();	
	const double TargetMinW = AOIDataCollect.GetTargetMinSizeW_Zoom();
	const double TargetMinH = AOIDataCollect.GetTargetMinSizeH_Zoom();
	const double TargetOffsetX = AOIDataCollect.GetFovTargetOffsetX();
	const double TargetOffsetY = AOIDataCollect.GetFovTargetOffsetY();

	MotionCtrlPtr->GetCurrentPos(PosX, PosY, PosZ);
	ImageRes.x = AOIDataCollect.GetCameraResolutionX(CameraID);
	ImageRes.y = AOIDataCollect.GetCameraResolutionY(CameraID);

	m_FovRatio = Ratio;
	m_FOVPosStage.x = PosX;
	m_FOVPosStage.y = PosY;
	//ResetImageOffset();
	m_FrameResolution = ImageRes;
	m_FrameStageRgn.minX = PosX-(FOVWum*0.5);
	m_FrameStageRgn.maxX = PosX+(FOVWum*0.5);
	m_FrameStageRgn.minY = PosY-(FOVHum*0.5);
	m_FrameStageRgn.maxY = PosY+(FOVHum*0.5);

	TPOINT2D PosCad;	
	TPOINT2D PosStage(TargetOffsetX, TargetOffsetY);
	const bool bKeepImageOffset = GetKeepImageOffset();
	AOIDataCollect.MapStageOffsetPtToCad(PosStage, PosCad);		
	if ( false == bKeepImageOffset )
	{
		m_ImageOffset.x = -PosCad.x/(ImageRes.x);
		m_ImageOffset.y =  PosCad.y/(ImageRes.y);
		m_ImageOffset.x = m_ImageOffset.x/(m_ImageZoom);
		m_ImageOffset.y = m_ImageOffset.y/(m_ImageZoom);
	}
	SetKeepImageOffset(false);
	AOIDataCollect.SetFovTargetOffsetX(0);
	AOIDataCollect.SetFovTargetOffsetY(0);

	std::vector<TUNI_FRAME>   UniFrameList;
	std::vector<TFrameParam>  GrabFrameParamList;//影像參數列表			
	AOIDataCollect.GetGrabFrameParamList(GrabFrameParamList);
	const size_t GrabFrameParamCount = GrabFrameParamList.size();
	if ( 0 == GrabFrameParamCount ) { return false; }

	if ( true == bCameraCallBack )
	{	AOIDataCollect.ModifyCameraUniFrame(GrabFrameParamList);	}
	if ( AOIDataCollect.RetrieveCameraUniFrame(GrabFrameParamList, UniFrameList) == false )
	{	
		JetAPI::ClearUniFrameList(UniFrameList);
		JetAPI::ShowMessageBox(AOIDataCollect.GetErrorString());		
		return false;
	}
	ReleaseUniFrameBuffer();
	ReleaseShowImageBuffer();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr )
	{	m_ImageIndex = 0; }
	else
	{	m_ImageIndex = ProjectPtr->GetProjectMapIndex(); }	
	const size_t MaxFrames = GetMaxFrameCount();
	const size_t UniFrameCount = UniFrameList.size();	
	const size_t MinUniFrameCount = MIN(MaxFrames, UniFrameCount);
	for ( i=0; i<MinUniFrameCount; i++ )
	{	m_UniFrameList[i] = UniFrameList[i];	}
	for ( i=MinUniFrameCount; i<UniFrameCount; i++ )
	{	JetAPI::ClearUniFrame(UniFrameList[i]);	}
	AOIDataCollect.SetFieldUniFrameList(m_FrameStageRgn, m_UniFrameList, MaxFrames);

	BuildShowImageBuffer();
	//AOIDataCollect.ReleaseModelUniFrameList();//因為變更了FOV的位置與圖檔所以清除原有的模組資料	
	
	//UpdateActiveObjList();
	UpdateModelStats();	
	CreateBKImage();
	RedrawWnd();
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnTuneInspectionGroup() 
{
	// TODO: Add your command handler code here
	UINT TuneCmdID = AOIDataCollect.GetRibbonTuneGroupCmdID();
	switch ( TuneCmdID )
	{
	case ID_TUNE_INSPECTION:
		OnTuneInspection();
		break;
	case ID_TUNE_SELECTED_MODEL:
		OnTuneSelectedModel();
		break;
	case ID_TUNE_SELECTED_COMPONENT:
		OnTuneSelectedComponent();
		break;
	case ID_TUNE_SELECTED_PART_NUMBER:
		OnTuneSelectedPartNumber();
		break;
	case ID_TUNE_SELECTED_MODEL_GROUP:
		OnTuneSelectedModelGroup();
		break;
	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateTuneInspectionGroup(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL==ProjectPtr || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE); }
	else
	{	pCmdUI->Enable(TRUE); }
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditModify() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditClone() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditClone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDelete() 
{
	// TODO: Add your command handler code here
	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxAdd()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }

	if ( ModelPtr->AddModelWndMaskWnd(WndPtr) == false )	{	return ; }	
	LogOperCtrl.SaveLogWndMaskSelectedCreate(WndPtr);
	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxAdd(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxRotate090()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }

	if ( ModelPtr->SpinModelWndMaskWndSelected(WndPtr, 90.0) == false )	{	return ; }	
	LogOperCtrl.SaveLogWndMaskSelectedRotate(WndPtr, 90.0);

	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxRotate090(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxRotate180()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }
	
	if ( ModelPtr->SpinModelWndMaskWndSelected(WndPtr, 180.0) == false )	{	return ; }	
	LogOperCtrl.SaveLogWndMaskSelectedRotate(WndPtr, 180.0);

	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxRotate180(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxRotate270()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }

	if ( ModelPtr->SpinModelWndMaskWndSelected(WndPtr, 270.0) == false )	{	return ; }	
	LogOperCtrl.SaveLogWndMaskSelectedRotate(WndPtr, 270.0);

	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxRotate270(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecModelEditMaskBoxShapeMode(BOX_SHAPE_MODE BoxShapeMode)
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return true; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return false; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return false; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return false; }
	CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == WndMaskPtr ) { return false; }

	if ( ModelPtr->SetModelWndMaskWndShapeMode(WndPtr, BoxShapeMode) == false )
	{	return false; }
	LogOperCtrl.SaveLogWndMaskSelectedShapeMode(WndPtr, BoxShapeMode);
	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();	
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxShapeRectangle()
{
	// TODO: Add your command handler code here
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_RECTANGLE);	
	return ;
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnUpdateModelEditMaskBoxShapeRectangle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnModelEditMaskBoxShapeRoundRect()
{
	// TODO: Add your command handler code here
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_ROUND_RECT);	
	return ;
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnUpdateModelEditMaskBoxShapeRoundRect(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnModelEditMaskBoxShapeEllipse()
{
	// TODO: Add your command handler code here
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_ELLIPSE);	
	return ;
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnUpdateModelEditMaskBoxShapeEllipse(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnModelEditMaskBoxShapeCapsule()
{
	// TODO: Add your command handler code here
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_CAPSULE);	
	return ;
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnUpdateModelEditMaskBoxShapeCapsule(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnModelEditMaskBoxShapeBullet()
{
	// TODO: Add your command handler code here
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_BULLET);	
	return ;
}
//-------------------------------------------------------------------------------------//	
void CEditModelView::OnUpdateModelEditMaskBoxShapeBullet(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxShapeHalfRoundRect()
{
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_HALF_ROUND_RECT);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxShapeHalfRoundRect(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxShapeTShape()
{
	ExecModelEditMaskBoxShapeMode(BOX_SHAPE_T_SHAPE);	
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxShapeTShape(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxShapeParam()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }
	CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == WndMaskPtr ) { return; }

	CString      str;	
	CString      strCaption, strName, strValue;	
	CInputBoxWnd InputBox;
	const double Param = WndMaskPtr->GetWndMaskShapeParam();
	str = _T("Input Shape Param");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Param");	
	strName = LoadMultiLanguageString( str, str);
	strValue.Format(_T("%.2f"), Param);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return ; }
	str = InputBox.m_DataEdit1;
	const double NewParam = ::_ttof(str);
	if ( NewParam < 0 || NewParam>100.0 )
	{	return ; }
	if ( ModelPtr->SetModelWndMaskWndShapeParam(WndPtr, NewParam) == false )	{	return ; }	
	LogOperCtrl.SaveLogWndMaskSelectedShapeParam(WndPtr, NewParam);
	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxShapeParam(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxShapeParam2()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }
	CAOIWndMask *WndMaskPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == WndMaskPtr ) { return; }

	CString      str;	
	CString      strCaption, strName, strValue;	
	CInputBoxWnd InputBox;
	const double Param = WndMaskPtr->GetWndMaskShapeParam2();
	str = _T("Input Shape Param");
	strCaption = LoadMultiLanguageString(str, str);
	str = _T("Param");	
	strName = LoadMultiLanguageString( str, str);
	strValue.Format(_T("%.2f"), Param);
	InputBox.SetParam1(strCaption, strName, strValue);
	if ( InputBox.DoModal() == IDCANCEL )
	{	return ; }
	str = InputBox.m_DataEdit1;
	const double NewParam = ::_ttof(str);
	if ( NewParam < 0 || NewParam>100.0 )
	{	return ; }
	if ( ModelPtr->SetModelWndMaskWndShapeParam2(WndPtr, NewParam) == false )	{	return ; }	
	LogOperCtrl.SaveLogWndMaskSelectedShapeParam2(WndPtr, NewParam);
	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxShapeParam2(CCmdUI* pCmdUI)
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(true);	
	if ( FALSE==bEditMode || true==bLockUIWnd )
	{	pCmdUI->Enable(FALSE);	}
	else
	{	pCmdUI->Enable(TRUE);	}
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxErase()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }

	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return ; }
	bool bEraseMode = MaskWndPtr->GetWndMaskEraseMode();
	if ( true == bEraseMode ) { bEraseMode = false; }
	else { bEraseMode = true; }
	if ( ModelPtr->SetModelWndMaskWndEraseMode(WndPtr, bEraseMode) == false )	{	return ; }	
	UpdateModelStats();
	RedrawWnd();	
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxErase(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxDelete()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }

	CAOIWndMask *MaskWndPtr = WndPtr->GetWndMaskWndActived();
	if ( NULL == MaskWndPtr ) { return ; }

	CString str;
	str = _T("Do you want to delete the mask boxes selected?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return ; }

	LogOperCtrl.SaveLogWndMaskSelectedDelete(WndPtr);
	if ( ModelPtr->DeleteModelWndMaskWnd(WndPtr) == false )	{	return ; }	
	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxDelete(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditMaskBoxClear()
{
	// TODO: Add your command handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	CAOIWnd *WndPtr = ModelPtr->GetModelWndActived();
	if ( NULL == WndPtr ) { return; }
	if ( WndPtr->CheckWndAlgUsedMaskWnd() == false ) { return; }

	const size_t WndMaskBoxCount = WndPtr->GetWndMaskWndCount();
	if ( 0 == WndMaskBoxCount ) { return; }

	CString str;
	str = _T("Do you want to clear all mask boxes?");
	str = LoadMultiLanguageString(str, str);
	if ( JetAPI::ShowMessageBox(str, MB_YESNO) == IDNO )
	{	return ; }

	LogOperCtrl.SaveLogWndMaskSelectedClearAll(WndPtr);
	if ( ModelPtr->ClearModelWndMaskWndList(WndPtr) == false )	{	return ; }	
	UpdateModelStats();
	RedrawWnd();	
	//SendOutUpdatePartList();
	return ;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditMaskBoxClear(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditGlobalCloneWnd()
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return; }

	std::vector<CAOIWnd*> WndList;
	ModelPtr->GetModelWndSelectedList(WndList);
	AOIDataCollect.SetTempWndList(WndList);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditGlobalCloneWnd(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditGlobalPasteWnd()
{
	// TODO: Add your command update UI handler code here
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = this->GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return; }

	std::vector<CAOIWnd*> WndList;
	AOIDataCollect.GetTempWndList(WndList);
	const size_t Count = WndList.size();
	if ( 0 == Count ) { return; }

	ModelPtr->PasteModelWndList(WndList);	
	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditGlobalPasteWnd(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecSaveDefaultModel()//儲存預設模組 
{
	CString      str, str2;
	CAOIProject *ProjectPtr = GetActiveProject();
	if ( NULL == ProjectPtr ) { return false; }
	CAOIModel   *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr )	{	return false; }
	CAOIComponent *ComponentPtr = ModelPtr->GetModelComponentPtr();
	if ( NULL == ComponentPtr )	{	return false; }
	const bool ModelIsolated = ComponentPtr->GetComponentModelIsolated();
	if ( true == ModelIsolated ) { return false; }
	if ( AOIDataCollect.CloseActiveComponent(ComponentPtr) == false )
	{	
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false; 
	}

	CAOIModel *ModelPtr_C = ComponentPtr->GetComponentModelPtr();
	if ( NULL == ModelPtr_C ) { return false; }
	if ( ModelPtr_C->CheckModelTypeEnabled() == false )
	{	return false; }
	CString ModelName = ModelPtr_C->GetModelName();
	CString GroupName = ModelPtr_C->GetModelGroupName();
	CAOIModel *ModelPtr_M = ProjectPtr->GetProjectModelPtrByModelName(ModelName);
	if ( NULL == ModelPtr_M ) { return false; }
	if ( ModelPtr_M->CheckModelTypeEnabled() == false )
	{	return false; }
	CString filename_tmp;	
	CString filename = ModelPtr_M->GetModelDefaultFilename();
	CString filename_img = ModelPtr_M->GetModelDefaultImageFilename();
	CString TempFolder = AOIDataCollect.GetAOIDefaultModelDirectory();
	const bool bIsFileExist = JetAPI::IsFileExist(filename);
	if ( true == bIsFileExist )
	{
		str = _T("Default model exist, do you want to overwrite it?");
		str = LoadMultiLanguageString(str, str);
		str2.Format(_T("[%s]%s"), GroupName, str);
		if ( JetAPI::ShowMessageBox(str2, MB_YESNO) != IDYES )
		{	return false; }
	}	
	//因為存檔時會有許多資料夾連動，因此創建暫時的Model來儲存
	CAOIModel *ModelPtr_T = ModelPtr_M->CloneModelObj();
	if ( NULL == ModelPtr_T )
	{
		str = _T("Error, Create Default model fault");
		str = LoadMultiLanguageString(str, str);		
		JetAPI::ShowMessageBox(str);
		return false;
	}
	ModelPtr_T->SetModelAutoDeleteImageFolder(false);
	filename_tmp.Format(_T("%s\\%s"), TempFolder, _T("DefaultModelTmp.MDL"));
	if ( ModelPtr_T->SaveModelParamFile(filename_tmp) == false )
	{	
		AOIObjManager.DestroyModelObj(ModelPtr_T);
		str = AOIDataCollect.GetErrorString();
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	AOIObjManager.DestroyModelObj(ModelPtr_T);
	::DeleteFile(filename);	::Sleep(0);		
	if ( ::MoveFile(filename_tmp, filename) == FALSE )
	{
		str.Format(_T("Error, Copy file Fault\n[%s]\n[%s]"), filename_tmp, filename);
		JetAPI::ShowMessageBox(str);
		return false; 
	}
	::DeleteFile(filename_tmp);	::Sleep(0);		

	CString ModelFolder;
	CString LibraryFolder = ProjectPtr->GetProjectLibraryFolder();	

	ModelFolder.Format(_T("%s\\%s"), LibraryFolder, ModelName);
	CString ModelBkImage = AOIDataDefine.GetModelBKImageFilename(ModelFolder, ModelName, 0);
	::DeleteFile(filename_img);	::Sleep(0);
	::CopyFile(ModelBkImage, filename_img, FALSE);
	
	bool bTestLoad = false;
	if ( true == bTestLoad )
	{
		CAOIModel ModelTmp;	
		::CopyFile(filename, filename_tmp, FALSE);
		if ( ModelTmp.LoadModelParamFile(filename_tmp) == false )
		{	
			::DeleteFile(filename_tmp);	::Sleep(0);		
			str = AOIDataCollect.GetErrorString();
			JetAPI::ShowMessageBox(str);
			return false; 
		}
		::DeleteFile(filename_tmp);	::Sleep(0);		
	}
	return true;
}
//-------------------------------------------------------------------------------------//
bool CEditModelView::ExecShowLibraryWnd(bool bShow)//顯示資料庫視窗
{
	//AOIDataCollect.PostMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, bShow);
	AOIDataCollect.SendMainFrameWndMessage(MSG_MAIN_FRAME_MESSAGE, WPARAM_SHOW_PROJECT_LIBRARY_DOCK_PANE, bShow);
	return true;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditShowLibraryWnd()
{
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	ExecShowLibraryWnd(true);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditShowLibraryWnd(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd )
	{	bEnable = FALSE; }
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditPropertyWnd()
{
	const char fnName[] = "CEditModelView::OnModelEditPropertyWnd";
	bool bLockUIWnd = GetLockUIWnd();
	if ( true == bLockUIWnd ) { return; }	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	if ( ModelPtr->CheckModelTypeEnabled() == false )
	{	return; }
	
	CAOIModel *ModelPtrTmp = ModelPtr->CloneModelObj();
	if ( NULL == ModelPtrTmp ) { return; }
	const bool bCloned = true;		
	std::vector<TUNI_FRAME> UniFrameList;	
	const double AttachedAngle = ModelPtrTmp->GetModelAttachedAngle();
	const bool   IsExceptionAngle = JetAPI::CheckIsExceptionAngle(AttachedAngle);	

	AOIDataCollect.CopyModelUniFrameList(UniFrameList, bCloned);
	AOIDataCollect.SetDrawImageMode(DRAW_IMAGE_NORMAL);	
	if ( true == IsExceptionAngle ) 
	{
		size_t      i=0;
		TUNI_FRAME  UniFrameTmp;	
		std::vector<TUNI_FRAME> UniFrameListTmp;	
		const size_t FrameCount = UniFrameList.size();
		for ( i=0; i<FrameCount; i++ )
		{
			if ( ImageAPI.RotateUniImage(-AttachedAngle, UniFrameList[i], 4, fnName, UniFrameTmp) == false ) 
			{
				JetAPI::ClearUniFrameList(UniFrameList);
				JetAPI::ClearUniFrameList(UniFrameListTmp);
				return;
			}
			UniFrameListTmp.push_back(UniFrameTmp);
		}
		JetAPI::ClearUniFrameList(UniFrameList);
		UniFrameList = UniFrameListTmp;
		ModelPtrTmp->RotateModel(-AttachedAngle, 0, 0);
	}
	
	CModelPropertyWnd ModelProptyWnd;
	ModelProptyWnd.SetModelPtr(ModelPtrTmp);
	ModelProptyWnd.SetUniFrameList(UniFrameList);
	if ( ModelProptyWnd.DoModal() == IDCANCEL )
	{
		JetAPI::ClearUniFrameList(UniFrameList);	
		AOIObjManager.DestroyModelObj(ModelPtrTmp);
		return;
	}
	JetAPI::ClearUniFrameList(UniFrameList);
	if ( true == IsExceptionAngle ) 
	{	ModelPtrTmp->RotateModel(AttachedAngle, 0, 0);	}
	ModelPtr->CopyModelProperty(ModelPtrTmp);
	AOIObjManager.DestroyModelObj(ModelPtrTmp);	

	ModelPtr->UpdateModelWndRgnByLinkMode();
	ModelPtr->CalcModelTotalRegionAll();
	ModelPtr->SetModelNeedSaveFiles(true);
	ModelPtr->UpdateModelRegionToAttached();
	AOIDataCollect.CloseActiveModel(ModelPtr);
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditPropertyWnd(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	if ( true == bLockUIWnd )
	{	bEnable = FALSE; }
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditEnableAllWnds()
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassModelWnd() == false ) { return; }

	ModelPtr->SetModelWndEnabled(true);
	LogOperCtrl.SaveLogModelWndSelected(ModelPtr, AOIDataDefine.GetEnableText());

	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditEnableAllWnds(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( true==bLockUIWnd || NULL==ModelPtr )
	{	bEnable = FALSE; }
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnModelEditDisableAllWnds()
{
	bool bLockUIWnd = GetLockUIWnd();	
	BOOL bEditMode = CheckInEditMode(false);	
	if ( FALSE==bEditMode || true==bLockUIWnd ) { return; }
	CAOIModel *ModelPtr = GetModelPtr();
	if ( NULL == ModelPtr ) { return; }	
	if ( AOIDataCollect.OperateLevelEditFuncBypassModelWnd() == false ) { return; }

	ModelPtr->SetModelWndEnabled(false);
	LogOperCtrl.SaveLogModelWndSelected(ModelPtr, AOIDataDefine.GetDisableText());

	UpdateModelStats();
	RedrawWnd();
	SendOutUpdatePartList();
	return;
}
//-------------------------------------------------------------------------------------//
void CEditModelView::OnUpdateModelEditDisableAllWnds(CCmdUI* pCmdUI)
{
	BOOL bEnable=TRUE;
	bool bLockUIWnd = GetLockUIWnd();	
	CAOIModel *ModelPtr = GetModelPtr();
	if ( true==bLockUIWnd || NULL==ModelPtr )
	{	bEnable = FALSE; }
	pCmdUI->Enable(bEnable);
}
//-------------------------------------------------------------------------------------//