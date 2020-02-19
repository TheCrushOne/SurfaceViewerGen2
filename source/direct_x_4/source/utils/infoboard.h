#pragma once

/**
@file     infoboard.h
@brief    Info out support class
@date     Created on 22/10/2014
@project  D3DBase
@author   Crush
*/

#include "utils/utils.h"
#include <vector>
#include <stack>

class Camera;
class CarMove;
class DirectXApp;
class Engine;
class OrderManager;

//struct cbiDbgLine   //pref : cdl_
//{
//    int cdl_left;
//    int cdl_top;
//    std::string cdl_type;
//    std::string cdl_val;
//    D3DCOLOR cdl_tColor;
//    D3DCOLOR cdl_vColor;
//};

typedef bool CFLAG;

/*
\enum sb_eFLinePVType
\brief Перечислимый тип условных названий параметров строк
*/
enum ParameterValueIdentifier
{
    //DI 25
    SBE_DIT,    //debug information title
    SBE_AF,     //avg fps
    SBE_AFD,    //avg fps draw
    SBE_RFM,    //render fill mode
    SBE_RCM,    //render cull mode
    SBE_TEX,    //texturing
    SBE_MPF,    //mip filtering
    SBE_MNF,    //min filtering
    SBE_MGF,    //mag filtering
    SBE_MMB,    //mipmap bias
    SBE_ZTU,    //Z-test usage
    SBE_RT,     //render type
    SBE_SSU,    //shader system usage (OBT)
    SBE_SMI,    //show main info
    SBE_SDI,    //show debug info
    SBE_SSI,    //show service info
    SBE_SH,     //show HUD
    SBE_CIT,    //camera information title
    SBE_CP,     //camera position
    SBE_CT,     //camera target
    SBE_CD,     //camera direction
    SBE_CMS,    //camera movement speed
    SBE_CMT,    //camera move type
    SBE_CCB,    //camera collision block
    SBE_CCD,    //camera collision detected

    //SI 4
    SBE_SIT,    //service information title
    SBE_VA,     //video adapter
    SBE_VD,     //video driver
    SBE_DDV,    //directx driver version

    //BI 34
    SBE_HPH,    //H press helper
    SBE_BIT,    //bind information title
    SBE_TF,     //toggle fullscreen
    SBE_CC1,    //camera control
    SBE_CC2,    //camera control(no manual only)
    SBE_CC3,    //camera control(manual only)
    SBE_CC4,    //camera control(spectator only)
    SBE_CZ,     //camera zoom
    SBE_CC,     //car control
    SBE_GS,     //gear switch up/down
    SBE_SS,     //signal switch left/right
    SBE_BGL,    //background lightning
    SBE_HML,    //head/markerlights(3 stages)
    SBE_BL,     //beam headlights(shader system only)
    SBE_HL,     //house lantern
    SBE_NL,     //neon lights(shader system only)
    SBE_TTT,    //toggle texturing type
    SBE_TCMT,   //toggle camera move type
    SBE_CMSUD,  //camera move speed up/down
    SBE_TMPF,   //toggle mip filtering
    SBE_TMNF,   //toggle min filtering
    SBE_TMGF,   //toggle mag filtering
    SBE_CMLB,   //change mipmap lodbias
    SBE_TCC,    //toggle camera collision
    SBE_TFM,    //toggle fill mode
    SBE_TSSU,   //toggle shader system usage(OBT)
    SBE_TLSD,   //toggle light sphere draw(shader system only)
    SBE_TZTU,   //toggle Z-test usage
    SBE_TRT,    //toggle render type
    SBE_WTRT,   //world transform(rotate)
    SBE_TCMC,   //toggle car manual control(OBT)
    SBE_HSTH,   //hide/show this help
    SBE_HSDI,   //hide/show debug information
    SBE_HSSI,   //hide/show service info
    SBE_HSH,    //hide/show hud

    //LI 15
    SBE_LCIT,   //lamborghini car information title
    SBE_LCT,    //LC control type
    SBE_LP,     //LC position
    SBE_LYA,    //LC yaw angle
    SBE_LLWN,   //LC last waypoint number
    SBE_LLWP,   //LC last waypoint position
    SBE_LNWP,   //LC next waypoint position
    SBE_LTR,    //LC turn radius
    SBE_LMS,    //LC move speed    
    SBE_LMW,    //LC move way
    SBE_LTA,    //LC turn angle
    SBE_LDT,    //LC drive type
    SBE_LCG,    //LC current gear
    SBE_LTS,    //LC transmission status
    SBE_LAS,    //LC acceleration status

    SBE_END,
};

enum AnchorType
{
   AT_TOPLEFT = 0,
   AT_TOPRIGHT,
   AT_BOTTOMLEFT,
   AT_BOTTOMRIGHT,
};

#define CBI_HIDE false
#define CBI_SHOW true

//#define STREAM_SHOW_DYNAMIC_CAST(s, x) x?s<<"true":s<<"false",s<<std::endl
//#define STREAM_CMT_DYNAMIC_CAST(s, x) x?(x==1?s<<"spectator":s<<"follow"):s<<"orbital",s<<std::endl
//#define STREAM_BCC_DYNAMIC_CAST(s, x) x?s<<"enabled":s<<"disabled",s<<std::endl
//#define STREAM_FMODE_DYNAMIC_CAST(s, x) x==D3DFILL_WIREFRAME?s<<"wireframe":s<<"force dword",s<<std::endl
//#define STREAM_CMODE_DYNAMIC_CAST(s, x) x==D3DCULL_NONE?s<<"none":s<<"ccw",s<<std::endl
//#define STREAM_BOOL_DYNAMIC_CAST(s, x) x?s<<"true":s<<"false",s<<std::endl
//#define STREAM_CMW_DYNAMIC_CAST(s, x) x?(x==1?s<<"turn left":s<<"turn right"):s<<"straight",s<<std::endl
//#define STREAM_FLT_DYNAMIC_CAST(s, x) x==D3DTEXF_LINEAR?s<<"linear":(x==D3DTEXF_POINT)?s<<"point":s<<"none",s<<std::endl
//#define STREAM_WTYPE_DYNAMIC_CAST(s, x) x==WBC_CYCLED?s<<"cycled waypoint guided":(x==WBC_NOCYCLED)?s<<"nocycled waypoint guided":s<<"user controlled",s<<std::endl
//#define STREAM_SHIFT_DYNAMIC_CAST(s, x) x?s<<"shifting":s<<"idle",s<<std::endl
//#define STREAM_ACC_DYNAMIC_CAST(s, x) x==WBC_IDLE?s<<"idle":(x==WBC_THROTTLE)?s<<"throttle":s<<"brake",s<<std::endl
//#define STREAM_DTYPE_DYNAMIC_CAST(s, x) x==WBC_DRIVE?s<<"D":(x==WBC_NEUTRAL)?s<<"N":s<<"R",s<<std::endl
//#define STREAM_RTYPE_DYNAMIC_CAST(s, x) x==SBM_ORDERED?s<<"ordered":(x==SBM_DISORDERED)?s<<"disordered":s<<"chaotic",s<<std::endl

#define STREAM_SHOW_DYNAMIC_CAST(x) std::string(x?"true":"false")
#define STREAM_CMT_DYNAMIC_CAST(x) std::string(x?(x==1?"spectator":"follow"):"orbital")
#define STREAM_BCC_DYNAMIC_CAST(x) std::string(x?"enabled":"disabled")
#define STREAM_FMODE_DYNAMIC_CAST(x) std::string(x==D3DFILL_WIREFRAME?"wireframe":"force dword")
#define STREAM_CMODE_DYNAMIC_CAST(x) std::string(x==D3DCULL_NONE?"none":"ccw")
#define STREAM_BOOL_DYNAMIC_CAST(x) std::string(x?"true":"false")
#define STREAM_CMW_DYNAMIC_CAST(x) std::string(x?(x==1?"turn left":"turn right"):"straight")
#define STREAM_FLT_DYNAMIC_CAST(x) std::string(x==D3DTEXF_LINEAR?"linear":(x==D3DTEXF_POINT)?"point":"none")
#define STREAM_WTYPE_DYNAMIC_CAST(x) std::string(x==WBC_CYCLED?"cycled waypoint guided":(x==WBC_NOCYCLED)?"nocycled waypoint guided":"user controlled")
#define STREAM_SHIFT_DYNAMIC_CAST(x) std::string(x?"shifting":"idle")
#define STREAM_ACC_DYNAMIC_CAST(x) std::string(x==WBC_IDLE?"idle":(x==WBC_THROTTLE)?"throttle":"brake")
#define STREAM_DTYPE_DYNAMIC_CAST(x) std::string(x==WBC_DRIVE?"D":(x==WBC_NEUTRAL)?"N":"R")
#define STREAM_DTYPE_CLR_DYNAMIC_CAST(x) (x==WBC_DRIVE?FONT_COLOR_BWE:(x==WBC_NEUTRAL)?FONT_COLOR_GN:FONT_COLOR_RD)
#define STREAM_RTYPE_DYNAMIC_CAST(x) std::string(x==SBM_ORDERED?"ordered":(x==SBM_DISORDERED)?"disordered":"chaotic")

// Основной класс для работы с 2д текстом на экране
class Infoboard
{
   //для хранения данных каждой строки
   struct line_data
   {
      //int m_pos[2];
      std::string parameter_title;
      D3DCOLOR title_color;

      int value_shift;
      std::string parameter_value;
      D3DCOLOR value_color;
      bool is_printable;

      line_data() { parameter_title = std::string(""); parameter_value = std::string(""); }
      line_data(const char* title, D3DCOLOR tColor, int shift, const char* value, D3DCOLOR vColor, bool printable)
         : parameter_title(title)
         , title_color(tColor)
         , value_shift(shift)
         , parameter_value(value)
         , value_color(vColor)
         , is_printable(printable)
      {}
   };

   struct output_geometry
   {
      int anchor_x, anchor_y;
      AnchorType type;
   };

public:
   Infoboard(std::shared_ptr<Engine>& eng, LPDIRECT3D9 d3d9, LPDIRECT3DDEVICE9 dev);
   ~Infoboard();
private:
   void initService();
   void initLine();
   void recountStatus();

   void getMainInfo();
   void getDebugInfo();
   void getServiceInfo();
   void getHudInfo();

   void updateFrameRate(float elapsedTimeSec);
   float getElapsedTimeInSeconds();

   void setOutputText(ParameterValueIdentifier startIdx, ParameterValueIdentifier finishIdx, const output_geometry& startPos);
public:
   void LRelease();
   void LRecreate(int i_nW, int i_nH, bool recount);
   void LRecreate(int i_nW, int i_nH);

   void Update();
   void Render();

   void ToggleShowMainInfo();
   void ToggleShowDbgInfo();
   void ToggleShowServiceInfo();
   void ToggleShowHUD();

   void SetOutputText(LPCSTR str);
private:
   output_geometry m_mainGeom;
   output_geometry m_debugGeom;
   output_geometry m_hudGeom;
   output_geometry m_serviceGeom;
   output_geometry m_outputGeom;
   int m_verticalGap;
   //int m_mainXLTPos, m_mainYLTPos; //left top
   //int m_dbgXLTPos, m_dbgYLTPos; //left top
   //int m_servXLTPos, m_servYLTPos; //left top
   //int m_hudXLTPos, m_hudYLTPos;  //left top
   //int m_mainWidth, m_mainHeight;  //пока что будут default
   //int m_dbgWidth, m_dbgHeight;  //пока что будут default
   //int m_servWidth, m_servHeight;  //пока что будут default
   //int m_hudWidth, m_hudHeight;  //пока что будут default

   int m_fps;

   line_data* m_data;
   std::deque<line_data> m_output;

   DWORD m_width;
   DWORD m_height;

   RECT m_fontRect;

   LPD3DXFONT m_dFont;

   LPDIRECT3D9 m_d3d9;
   LPDIRECT3DDEVICE9 m_d3dd;
   std::shared_ptr<Camera>& m_camera;
   std::shared_ptr<CarMove>& m_carmove;
   std::shared_ptr<DirectXApp>& m_main;
   std::shared_ptr<Engine>& m_engine;
   std::shared_ptr<OrderManager>& m_manager;

   CFLAG m_showMainInfo;     //хелп
   CFLAG m_showDebugInfo;    //дебаг
   CFLAG m_showServiceInfo;  //спец
   CFLAG m_showHUD;

   static const UINT DEFAULT_FONT_HEIGHT;
   static const UINT DEFAULT_FONT_GAP;
   static const UINT DEFAULT_FONT_WIDTH;

   static const DWORD DEFAULT_LINE_FIELD_COUNT;
   static const char DEFAULT_LINE_FIELD_TITLES[][uts::MAX_LENGTH];

   static const D3DCOLOR FONT_COLOR_WE;
   static const D3DCOLOR FONT_COLOR_GY;
   static const D3DCOLOR FONT_COLOR_GN;
   static const D3DCOLOR FONT_COLOR_YW;
   static const D3DCOLOR FONT_COLOR_RD;
   static const D3DCOLOR FONT_COLOR_BE;
   static const D3DCOLOR FONT_COLOR_BWE;

   //static const int DEFAULT_MAIN_WIDTH;
   //static const int DEFAULT_MAIN_HEIGHT;
   //static const int DEFAULT_DBG_WIDTH;
   //static const int DEFAULT_DBG_HEIGHT;
   //static const int DEFAULT_SERV_WIDTH;       //соединен с дебагом(не исп.)
   //static const int DEFAULT_SERV_HEIGHT;      //соединен с дебагом(не исп.)
   //static const int DEFAULT_HUD_WIDTH;
   //static const int DEFAULT_HUD_HEIGHT;

   static const int DEFAULT_MAIN_XLTPOS;
   static const int DEFAULT_MAIN_YLTPOS;
   static const int DEFAULT_DBG_XLTPOS;
   static const int DEFAULT_DBG_YLTPOS;
   static const int DEFAULT_SERV_XLTPOS;
   static const int DEFAULT_SERV_YLTPOS;
   static const int DEFAULT_HUD_XLTPOS;
   static const int DEFAULT_HUD_YLTPOS;
   static const int DEFAULT_OUTPUT_XLTPOS;
   static const int DEFAULT_OUTPUT_YLTPOS;

   static const int DEFAULT_OUTPUT_LIMIT;
};