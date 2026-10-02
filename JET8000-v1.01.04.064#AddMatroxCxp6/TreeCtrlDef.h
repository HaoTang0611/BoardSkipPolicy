#ifndef _TreeCtrlDef_H_
#define _TreeCtrlDef_H_
//-------------------------------------------------------------------------------------//
#define TREE_NODE_PANEL_ID                               1
#define TREE_NODE_FD_ID                                  2
#define TREE_NODE_BARDOE_ID                              3
#define TREE_NODE_BOARD_ID                               4
#define TREE_NODE_MARK_ID                                5
#define TREE_NODE_COMPONENT_ID                           6
#define TREE_NODE_WINDOW_ID                              7
//-------------------------------------------------------------------------------------//
//Root 節點的引數 0 ~ 255
#define INSPECT_NONE                                     0

#define INSPECT_PROJECT_ROOT_START                       1
#define INSPECT_FIDUCIAL_ROOT_START                      2
#define INSPECT_COMPONENT_PACKAGE_NODE                   3
#define INSPECT_COMPONENT_PART_NUMBER_NODE               4
#define INSPECT_COMPONENT_POSITION_NODE                  5
#define INSPECT_COMPONENT_OFFSET_NODE                    6

#define INSPECT_COMPONENT_MODEL_GROUP_NODE               7
#define INSPECT_COMPONENT_MODEL_GROUP_WINDOW_NODE        8
#define INSPECT_COMPONENT_TYPE_NODE                      9
#define INSPECT_COMPONENT_MODEL_NODE                    10

#define TREE_ITEM_TYPE_NULL                              0
#define TREE_ITEM_TYPE_OPEN_BARCODE                     11

#define TREE_ITEM_TYPE_PROJECT                          21
#define TREE_ITEM_TYPE_PROJECT_FILENAME                 22
#define TREE_ITEM_TYPE_PROJECT_MODULE                   23
#define TREE_ITEM_TYPE_PROJECT_REGION_W                 24
#define TREE_ITEM_TYPE_PROJECT_REGION_H                 25
#define TREE_ITEM_TYPE_PROJECT_FD_RANGE_W               26
#define TREE_ITEM_TYPE_PROJECT_FD_RANGE_H               27
#define TREE_ITEM_TYPE_PROJECT_BARCODE_MODE             28

#define TREE_ITEM_TYPE_PANEL                            31
#define TREE_ITEM_TYPE_FD_GROUP                         41
#define TREE_ITEM_TYPE_FD                               42

#define TREE_ITEM_TYPE_BARCODE_GROUP                    51
#define TREE_ITEM_TYPE_BARCODE                          52

#define TREE_ITEM_TYPE_BOARD                            61

#define TREE_ITEM_TYPE_COMPONENT                        71
#define TREE_ITEM_TYPE_COMPONENT_PACKAGE                72
#define TREE_ITEM_TYPE_COMPONENT_PART_NUMBER            73
#define TREE_ITEM_TYPE_COMPONENT_POSITION               74
#define TREE_ITEM_TYPE_COMPONENT_OFFSET                 75
#define TREE_ITEM_TYPE_COMPONENT_MODE                   76

#define TREE_ITEM_TYPE_MARK_GROUP                       81
#define TREE_ITEM_TYPE_MARK                             82

#define TREE_ITEM_TYPE_MODEL                            91
#define TREE_ITEM_TYPE_WINDOW                          101
//-------------------------------------------------------------------------------------//
//樹狀圖的圖示
const UINT TREE_IMAGE_LIST_PROGRAM                    =  0;
const UINT TREE_IMAGE_LIST_FD                         =  1;
const UINT TREE_IMAGE_LIST_FD_SELECTED                =  2;
const UINT TREE_IMAGE_LIST_BARCODE                    =  3;
const UINT TREE_IMAGE_LIST_BARCODE_SELECTED           =  4;
const UINT TREE_IMAGE_LIST_PANEL                      =  5;
const UINT TREE_IMAGE_LIST_PANEL_SELECTED             =  6;
const UINT TREE_IMAGE_LIST_PANEL_BYPASS               =  7;
const UINT TREE_IMAGE_LIST_PANEL_BYPASS_SELECTED      =  8;
const UINT TREE_IMAGE_LIST_BOARD                      =  9;
const UINT TREE_IMAGE_LIST_BOARD_SELECTED             = 10;
const UINT TREE_IMAGE_LIST_BOARD_BYPASS               = 11;
const UINT TREE_IMAGE_LIST_BOARD_BYPASS_SELECTED      = 12;
const UINT TREE_IMAGE_LIST_COMPONENT                  = 13;
const UINT TREE_IMAGE_LIST_COMPONENT_SELECTED         = 14;
const UINT TREE_IMAGE_LIST_COMPONENT_BYPASS           = 15;
const UINT TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED  = 16;
const UINT TREE_IMAGE_LIST_COMPONENT_SKIP             = 17;
const UINT TREE_IMAGE_LIST_COMPONENT_SKIP_SELECTED    = 18;
const UINT TREE_IMAGE_LIST_MODEL                      = 19;
const UINT TREE_IMAGE_LIST_MODEL_CHECK                = 20;
const UINT TREE_IMAGE_LIST_WINDOW                     = 21;
const UINT TREE_IMAGE_LIST_WINDOW_ENABLE              = 22;
const UINT TREE_IMAGE_LIST_WINDOW_DISABLE             = 23;
const UINT TREE_IMAGE_LIST_OTHERS                     = 24;

const UINT TREE_IMAGE_LIST_MARK                       =  TREE_IMAGE_LIST_COMPONENT;
const UINT TREE_IMAGE_LIST_MARK_SELECTED              =  TREE_IMAGE_LIST_COMPONENT_SELECTED;
const UINT TREE_IMAGE_LIST_MARK_BYPASS                =  TREE_IMAGE_LIST_COMPONENT_BYPASS;
const UINT TREE_IMAGE_LIST_MARK_BYPASS_SELECTED       =  TREE_IMAGE_LIST_COMPONENT_BYPASS_SELECTED;
//-------------------------------------------------------------------------------------//
//ItemType::0 ~ 255(8bit), ItemIndex::0 ~ 16777216(24bit)
LPARAM  MakeItemlParam(size_t ItemType, size_t ItemIndex);
//ItemType::0 ~ 255(8bit), ItemIndex::0 ~ 16777216(24bit)
void DecodeItemlParam(LPARAM lParam, unsigned int &ItemType, unsigned int &ItemIndex);
//-------------------------------------------------------------------------------------//
#endif//_TreeCtrlDef_H_