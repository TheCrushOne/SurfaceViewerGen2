/**
@file     infoboard.cpp
@brief    Info processing class
@date     Created on 22/10/2014
@project  D3DBase
@author   Crush
*/

#include "stdafx.h"
#include "infoboard.h"
#include "engine/engine.h"
#include "utils/camera.h"
#include "directxapp.h"
#include "engine/subsystems/ordermanager.h"
#include "car_model/carmove.h"

//две стандартные функции directx'a, по техническим причинам вынесены в h-файл
HRESULT DXUtil_ConvertAnsiStringToWideCch(WCHAR* wstrDestination, const CHAR* strSource,
    int cchDestChar)
{
    if (wstrDestination == NULL || strSource == NULL || cchDestChar < 1)
        return E_INVALIDARG;

    int nResult = MultiByteToWideChar(CP_ACP, 0, strSource, -1,
        wstrDestination, cchDestChar);
    wstrDestination[cchDestChar - 1] = 0;

    if (nResult == 0)
        return E_FAIL;
    return S_OK;
}

HRESULT DXUtil_ConvertAnsiStringToGenericCch(TCHAR* tstrDestination, const CHAR* strSource,
    int cchDestChar)
{
    if (tstrDestination == NULL || strSource == NULL || cchDestChar < 1)
        return E_INVALIDARG;

#ifdef _UNICODE
    return DXUtil_ConvertAnsiStringToWideCch(tstrDestination, strSource, cchDestChar);
#else
    strncpy(tstrDestination, strSource, cchDestChar);
    tstrDestination[cchDestChar - 1] = '\0';
    return S_OK;
#endif    
}

const UINT Infoboard::DEFAULT_FONT_HEIGHT = 10;
const UINT Infoboard::DEFAULT_FONT_GAP = 2;
const UINT Infoboard::DEFAULT_FONT_WIDTH = 8;

const DWORD Infoboard::DEFAULT_LINE_FIELD_COUNT = 80;
//const DWORD Infoboard::CBI_DEFAULT_LINE_FIELD_LENGTH = 60;
const char Infoboard::DEFAULT_LINE_FIELD_TITLES[DEFAULT_LINE_FIELD_COUNT][uts::MAX_LENGTH] = {
#include "linetitle.hdrs"
};

const D3DCOLOR Infoboard::FONT_COLOR_WE = D3DCOLOR_ARGB(255, 255, 255, 255);
const D3DCOLOR Infoboard::FONT_COLOR_GY = D3DCOLOR_ARGB(255, 170, 170, 170);
const D3DCOLOR Infoboard::FONT_COLOR_GN = D3DCOLOR_ARGB(255, 50, 255, 50);
const D3DCOLOR Infoboard::FONT_COLOR_YW = D3DCOLOR_ARGB(255, 255, 255, 50);
const D3DCOLOR Infoboard::FONT_COLOR_RD = D3DCOLOR_ARGB(255, 255, 50, 50);
const D3DCOLOR Infoboard::FONT_COLOR_BE = D3DCOLOR_ARGB(255, 50, 50, 255);
const D3DCOLOR Infoboard::FONT_COLOR_BWE = D3DCOLOR_ARGB(255, 87, 206, 250);    //87/206/250

/*const int Infoboard::DEFAULT_MAIN_WIDTH = 200;
const int Infoboard::DEFAULT_MAIN_HEIGHT = 200;
const int Infoboard::DEFAULT_DBG_WIDTH = 200;
const int Infoboard::DEFAULT_DBG_HEIGHT = 200;
const int Infoboard::DEFAULT_SERV_WIDTH = 200;
const int Infoboard::DEFAULT_SERV_HEIGHT = 200;
const int Infoboard::DEFAULT_HUD_WIDTH = 200;
const int Infoboard::DEFAULT_HUD_HEIGHT = 200;*/

const int Infoboard::DEFAULT_MAIN_XLTPOS = 390;
const int Infoboard::DEFAULT_MAIN_YLTPOS = 15;
const int Infoboard::DEFAULT_DBG_XLTPOS = 20;
const int Infoboard::DEFAULT_DBG_YLTPOS = 15;
const int Infoboard::DEFAULT_SERV_XLTPOS = 20;
const int Infoboard::DEFAULT_SERV_YLTPOS = 350;
const int Infoboard::DEFAULT_HUD_XLTPOS = 390;
const int Infoboard::DEFAULT_HUD_YLTPOS = 15;
const int Infoboard::DEFAULT_OUTPUT_XLTPOS = 20;
const int Infoboard::DEFAULT_OUTPUT_YLTPOS = 15;

const int Infoboard::DEFAULT_OUTPUT_LIMIT = 15;

Infoboard::Infoboard(std::shared_ptr<Engine>& eng, LPDIRECT3D9 d3d9, LPDIRECT3DDEVICE9 dev)
   : m_d3d9(d3d9)
   , m_d3dd(dev)
   , m_camera(eng->GetCamera())
   , m_carmove(eng->GetCarmove())
   , m_manager(eng->GetOrderManager())
   , m_main(eng->GetMain())
   , m_engine(eng)
   , m_mainGeom{ DEFAULT_MAIN_XLTPOS, DEFAULT_MAIN_YLTPOS, AT_TOPRIGHT }
   , m_debugGeom{ DEFAULT_DBG_XLTPOS, DEFAULT_DBG_YLTPOS, AT_TOPLEFT }
   , m_hudGeom{ DEFAULT_HUD_XLTPOS, DEFAULT_HUD_YLTPOS, AT_BOTTOMRIGHT }
   , m_serviceGeom{ DEFAULT_SERV_XLTPOS, DEFAULT_SERV_YLTPOS, AT_TOPLEFT }
   , m_outputGeom{ DEFAULT_OUTPUT_XLTPOS, DEFAULT_OUTPUT_YLTPOS, AT_BOTTOMLEFT }
   , m_verticalGap(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP)
#ifdef _DEBUG
   , m_showDebugInfo(CBI_SHOW)
#else
   , m_showDebugInfo(CBI_HIDE)
#endif
   , m_showMainInfo(CBI_HIDE)
   , m_showServiceInfo(CBI_HIDE)
   , m_showHUD(CBI_HIDE)
{
   LRecreate(eng->GetWidth(), eng->GetHeight());
   initService();
}

Infoboard::~Infoboard()
{
   delete[] m_data;
   LRelease();
}

// Освобождения объектов директикса(перед ресетом устройства)
void Infoboard::LRelease()
{
   m_dFont->Release();
}


// Автовосстановление объектов директикса(вызов с форсированным пересчетом позиции(после ресета))
void Infoboard::LRecreate(int i_nW, int i_nH, bool recount)
{
   LRecreate(i_nW, i_nH);

   if (recount)
      recountStatus();  //если использовать при вызове из конструктора, все грохнется, т.к. не инициализирован массив структур
}


// Автовосстановление объектов директикса(после ресета устройства, или при старте)
void Infoboard::LRecreate(int i_nW, int i_nH)
{
   m_width = static_cast<DWORD>(i_nW);
   m_height = static_cast<DWORD>(i_nH);

   // Create a font
   HRESULT hr = D3DXCreateFontW(m_d3dd, //D3D Device
      DEFAULT_FONT_HEIGHT, //Font height
      0/*DEFAULT_FONT_WIDTH*/, //Font width
      FW_DONTCARE, //Font Weight
      0, //MipLevels
      false, //Italic
      DEFAULT_CHARSET, //CharSet
      OUT_DEFAULT_PRECIS, //OutputPrecision
      PROOF_QUALITY,//ANTIALIASED_QUALITY, //Quality
      DEFAULT_PITCH | FF_DONTCARE,//PitchAndFamily
      //TEXT("Arial"),
      TEXT("Courier"), //pFacename,   //тут нужен моноширинный шрифт(пока что там формула подсчета шифта через количество букв и ширину идет..)
      &m_dFont); //ppFont

   if (FAILED(hr))
      throw uts::AMLSS_E(uts::SBC_FONTCREATEFAILURE, "Courier");
}


// Обновление информации на экране(на каждом кадре)
void Infoboard::Update()
{
   updateFrameRate(getElapsedTimeInSeconds());

   getMainInfo();
   getDebugInfo();
   getServiceInfo();
   getHudInfo();
 //   //std::string main = getMainInfo(), dbg = getDebugInfo(), maint = getMaintenanceInfo(), hud = getHudInfo();

 //   
 //   dbg = dbg + maint;      //пока что так
 //  
	//

	//SetRect(&cbi_fontRect, //our font rect
	//	cbi_width - cbi_mainXLTPos, //Left
	//	cbi_mainYLTPos, //Top
	//	cbi_mainWidth, //width
	//	cbi_mainHeight //height
	//	);


	//cbi_dFont->DrawTextA(NULL, //pSprite
	//	(LPCSTR)(main.c_str()), //pString
	//	-1, //Count
	//	&cbi_fontRect, //pRect
	//	DT_LEFT | DT_NOCLIP,//Format,
	//	cbi_fontColor); //Color (white)
	//

	///*if ((cbi_showDebugInfo)
	//	|| (cbi_showMaintenanceInfo))*/
	//{

	//	SetRect(&cbi_fontRect, //our font rect
	//		cbi_dbgXLTPos, //Left
	//		cbi_dbgYLTPos, //Top
	//		cbi_dbgWidth, //width
	//		cbi_dbgHeight //height
	//		);


	//	cbi_dFont->DrawTextA(NULL, //pSprite
	//		(LPCSTR)(dbg.c_str()), //pString
	//		-1, //Count
	//		&cbi_fontRect, //pRect
	//		DT_LEFT | DT_NOCLIP,//Format,
	//		cbi_fontColor); //Color (white)
	//}

 //   SetRect(&cbi_fontRect, //our font rect
 //       cbi_width - cbi_hudXLTPos, //Left
 //       cbi_height - cbi_hudYLTPos, //Top
 //       cbi_hudWidth, //width
 //       cbi_hudHeight //height
 //       );


 //   cbi_dFont->DrawTextA(NULL, //pSprite
 //       (LPCSTR)(hud.c_str()), //pString
 //       -1, //Count
 //       &cbi_fontRect, //pRect
 //       DT_LEFT | DT_NOCLIP,//Format,
 //       cbi_fontColor); //Color (white)
    
}

// Передача инфы к выводу
void Infoboard::SetOutputText(LPCSTR str)
{
   const char* title = "msg: ";
   line_data data(title, FONT_COLOR_WE , strlen(title) * DEFAULT_FONT_WIDTH, str, FONT_COLOR_WE, true);
   m_output.push_front(data);
   while (m_output.size() >= DEFAULT_OUTPUT_LIMIT + 5)
      m_output.pop_back();
}

// Установка текста на экран
void Infoboard::setOutputText(ParameterValueIdentifier startIdx, ParameterValueIdentifier finishIdx, const output_geometry& startPos)
{
   bool isDownwards = startPos.type == AT_TOPLEFT || startPos.type == AT_TOPRIGHT;
   bool isRightwards = startPos.type == AT_TOPLEFT || startPos.type == AT_BOTTOMLEFT;
   int posX = isRightwards ? startPos.anchor_x : (m_width - startPos.anchor_x);
   int posY = isDownwards ? startPos.anchor_y : (m_height - m_verticalGap - startPos.anchor_y);
   DWORD range = finishIdx - startIdx;
   for (DWORD i = startIdx; i < finishIdx; i++)
   {
      const auto& elem = m_data[isDownwards ? i : (2*startIdx + range - 1 - i)];
      if (elem.is_printable)
      {
         //title
         SetRect(&m_fontRect, posX, posY, posX + 100, posY + 15);
         m_dFont->DrawTextA(NULL,
            (LPCSTR)(elem.parameter_title.c_str()),
            -1,
            &m_fontRect,
            DT_LEFT | DT_NOCLIP,
            elem.title_color
         );
         //value
         SetRect(&m_fontRect, posX + elem.value_shift, posY, posX + elem.value_shift + 100, posY + 15);
         m_dFont->DrawTextA(NULL,
            (LPCSTR)(elem.parameter_value.c_str()),
            -1,
            &m_fontRect,
            DT_LEFT | DT_NOCLIP,
            elem.value_color
         );
         posY += isDownwards ? m_verticalGap : - m_verticalGap;
      }
   }
}

// Отрисовка
void Infoboard::Render()
{
   static DWORD i = 0;

   if (m_showDebugInfo)
      setOutputText(SBE_DIT, SBE_SIT, m_debugGeom);
   if (m_showServiceInfo)
      setOutputText(SBE_SIT, SBE_HPH, m_serviceGeom);
   if (m_showMainInfo)
      setOutputText(SBE_HPH, SBE_LCIT, m_mainGeom);
   if (m_showHUD)
      setOutputText(SBE_LCIT, SBE_END, m_hudGeom);

   int posX = m_outputGeom.anchor_x, posY = m_height - m_outputGeom.anchor_y;
   size_t count = 0;
   for (auto& data : m_output)
   {
      if (count >= DEFAULT_OUTPUT_LIMIT)
         break;
      if (!data.is_printable)
         continue;
      SetRect(&m_fontRect, posX, posY, posX + 100, posY + 15);
      m_dFont->DrawTextA(NULL,
         (LPCSTR)(data.parameter_title.c_str()),
         -1,
         &m_fontRect,
         DT_LEFT | DT_NOCLIP,
         data.title_color
      );
      SetRect(&m_fontRect, posX + data.value_shift, posY, posX + data.value_shift + 100, posY + 15);
      m_dFont->DrawTextA(NULL,
         (LPCSTR)(data.parameter_value.c_str()),
         -1,
         &m_fontRect,
         DT_LEFT | DT_NOCLIP,
         data.value_color
      );
      posY -= m_verticalGap;
      count++;
   }
}

// Переключение автомата отображение основной информации(хелпа)
void Infoboard::ToggleShowMainInfo()
{
   m_showMainInfo = !m_showMainInfo;
   recountStatus();
}

// Переключение автомата отображение дебаг информации
void Infoboard::ToggleShowDbgInfo()
{
   m_showDebugInfo = !m_showDebugInfo;
   recountStatus();
}

// Переключение автомата отображение сервисной информации(система)
void Infoboard::ToggleShowServiceInfo()
{
   m_showServiceInfo = !m_showServiceInfo;
   recountStatus();
}

// Переключение автомата отображение хада
void Infoboard::ToggleShowHUD()
{
   m_showHUD = !m_showHUD;
   recountStatus();
}

// Инициализация переменных параметров инфоборда(пока что по default'у)
void Infoboard::initService()
{
   m_data = new line_data[DEFAULT_LINE_FIELD_COUNT];

   initLine();
}

// Инициализация постоянных параметров строк инфоборда
void Infoboard::initLine()
{
   for (DWORD i = 0; i < DEFAULT_LINE_FIELD_COUNT; i++)
   {
      m_data[i].parameter_title = std::string(DEFAULT_LINE_FIELD_TITLES[i]);
      m_data[i].value_shift = strlen(DEFAULT_LINE_FIELD_TITLES[i])*DEFAULT_FONT_WIDTH;
      m_data[i].title_color = FONT_COLOR_WE;
      m_data[i].value_color = FONT_COLOR_WE;
   }
   recountStatus();
}

// Инициализация постоянных параметров строк инфоборда
void Infoboard::recountStatus()
{
   //static int dockPosX = 0, dockPosY = 0;

   //dockPosX = m_dbgXLTPos;
   //дебаг
   if (m_showDebugInfo)
   {
      for (DWORD i = SBE_DIT; i < SBE_SIT; i++)
      {
         //m_data[i].m_pos[0] = m_dbgXLTPos;
         //m_data[i].m_pos[1] = m_dbgYLTPos + i*(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP);
         m_data[i].is_printable = true;
      }
      //dockPosY = m_dbgYLTPos + (SBE_SIT - SBE_DIT + 1)*(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP);
   }
   else
   {
      for (DWORD i = SBE_DIT; i < SBE_SIT; i++)
         m_data[i].is_printable = false;
      //dockPosY = m_dbgYLTPos;
   }

   //системная инфа
   if (m_showServiceInfo)
   {
      for (DWORD i = SBE_SIT; i < SBE_HPH; i++)
      {
         //m_data[i].m_pos[0] = dockPosX;
         //m_data[i].m_pos[1] = dockPosY + (i - SBE_SIT)*(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP);
         m_data[i].is_printable = true;
      }
   }
   else
   {
      for (DWORD i = SBE_SIT; i < SBE_HPH; i++)
         m_data[i].is_printable = false;
   }

   //бинды
   if (m_showMainInfo)
   {
      m_data[SBE_HPH].is_printable = false;
      for (DWORD i = SBE_BIT; i < SBE_LCIT; i++)
      {
         //m_data[i].m_pos[0] = m_width - m_mainXLTPos;
         //m_data[i].m_pos[1] = m_mainYLTPos + (i - SBE_BIT)*(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP);
         m_data[i].is_printable = true;
      }
      //dockPosY = m_mainYLTPos + (SBE_LCIT - SBE_BIT + 1)*(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP);
   }
   else
   {
      //m_data[SBE_HPH].m_pos[0] = m_width - m_mainXLTPos;
      //m_data[SBE_HPH].m_pos[1] = m_mainYLTPos;
      m_data[SBE_HPH].is_printable = true;
      for (DWORD i = SBE_BIT; i < SBE_LCIT; i++)
         m_data[i].is_printable = false;
      //dockPosY = m_mainYLTPos;
   }

   //хад
   if (m_showHUD)
   {
      for (DWORD i = SBE_LCIT; i <= SBE_LAS; i++)     //просто после ласа нету ничего
      {
         //m_data[i].m_pos[0] = m_width - m_mainXLTPos;
         //костыль...потому что снизу отступ нужен
         //m_data[i].m_pos[1] = m_height - 20 - (SBE_LAS - i)*(DEFAULT_FONT_HEIGHT + DEFAULT_FONT_GAP);
         m_data[i].is_printable = true;
      }
   }
   else
   {
      for (DWORD i = SBE_LCIT; i <= SBE_LAS; i++)
         m_data[i].is_printable = false;
   }
}

// Обновление информации о биндах(хелп)
void Infoboard::getMainInfo()
{
   if (m_engine->GetWRLDType() == MBE_ALPHADEMO)
   {
      m_data[SBE_TCMC].title_color = m_data[SBE_TCMC].value_color = FONT_COLOR_GY;
      m_data[SBE_TSSU].title_color = m_data[SBE_TSSU].value_color = FONT_COLOR_GY;
   }
   else
   {
      m_data[SBE_TCMC].title_color = m_data[SBE_TCMC].value_color = FONT_COLOR_WE;
      m_data[SBE_TSSU].title_color = m_data[SBE_TSSU].value_color = FONT_COLOR_WE;
   }

   if (m_engine->CheckShaderUsage())
   {
      m_data[SBE_TMPF].title_color = m_data[SBE_TMPF].value_color = FONT_COLOR_GY;
      m_data[SBE_TMNF].title_color = m_data[SBE_TMNF].value_color = FONT_COLOR_GY;
      m_data[SBE_TMGF].title_color = m_data[SBE_TMGF].value_color = FONT_COLOR_GY;
   }
   else
   {
      m_data[SBE_TMPF].title_color = m_data[SBE_TMPF].value_color = FONT_COLOR_WE;
      m_data[SBE_TMNF].title_color = m_data[SBE_TMNF].value_color = FONT_COLOR_WE;
      m_data[SBE_TMGF].title_color = m_data[SBE_TMGF].value_color = FONT_COLOR_WE;
   }
  //  std::string dstr = "press H to view help";
  //  if (!cbi_showMainInfo)
  //      return dstr;
  //  std::stringstream sstr;
  //  sstr << "Bind information :" << std::endl
  //      << "Alt + Enter - Toggle fullscreen(OBT(delay 4 sec.))" << std::endl
  //      << "Mouse - camera control" << std::endl
  //      << "Arrows - camera control(no car manual only)" << std::endl
  //      << "WASD - camera control(spectator type only)" << std::endl
  //      << "WASD, Arrows - car control(car manual only)" << std::endl
  //      << "X/Z - gear up/down(car manual only)" << std::endl
  //      << "[/] - signal left/right(shader system only)" << std::endl
  //      << "Numpad2,4,6,8 - camera control(car manual only)" << std::endl
  //      << "Numpad+,- - Camera zoom" << std::endl
  //      << "1 - Background lighting" << std::endl
  //      << "2 - Headlights/Markerlights(3 stages)" << std::endl
  //      << "Ctrl + 2 - Beam headlights(shader system only)" << std::endl
  //      << "3 - House lantern" << std::endl
  //      << "4 - Neon lights(shader system only)" << std::endl
  //      << "T - Toggle texturing type" << std::endl
  //      << "Space - Toggle camera move type" << std::endl
  //      << "Q - Camera movement speed up" << std::endl
  //      << "E - Camera movement speed down" << std::endl
  //      << "M - Toggle mip filtering" << std::endl
  //      << "F - Toggle min filtering" << std::endl
  //      << "G - Toggle mag filtering" << std::endl
  //      << "Shift + +/- - Mipmap bias" << std::endl
  //      << "F1 - Toggle camera collision" << std::endl
  //      << "F2 - Toggle fill mode" << std::endl
  //      << "F3 - Toggle shader system usage(OBT)" << std::endl
  //      << "F4 - Toggle light sphere draw(shader system only)" << std::endl
		//<< "F5 - Toggle Z-Test usage" << std::endl
		//<< "F6 - Toggle render type" << std::endl
  //      << "Ctrl + F12 - Toggle car manual control(OBT)" << std::endl
  //      << "H - Hide/show this help" << std::endl
  //      << "K - Hide/show debug information" << std::endl
  //      << "I - Hide/show maintenance information" << std::endl;

  //  dstr = sstr.str();
  //  
  //  return dstr;
}

// Обновление дебаг информации
void Infoboard::getDebugInfo()
{
   //std::string dstr = "";
   if (!m_showDebugInfo)
      return;
   //std::stringstream sstr;
   D3DXVECTOR3 camPos = m_camera->GetCurrentPosition();
   D3DXVECTOR3 camTarget = m_camera->GetCurrentTarget();

   m_data[SBE_AF].parameter_value = std::to_string(m_fps);
   m_data[SBE_AFD].parameter_value = std::to_string((m_fps < 60 ? 60 - m_fps : 0));
   m_data[SBE_AFD].value_color = (m_fps > 45) ? (FONT_COLOR_GN) : (m_fps > 25) ? (FONT_COLOR_YW) : (FONT_COLOR_RD);
   m_data[SBE_RFM].parameter_value = STREAM_FMODE_DYNAMIC_CAST(m_camera->GetRenderStateFillMode());
   m_data[SBE_RCM].parameter_value = STREAM_CMODE_DYNAMIC_CAST(m_camera->GetRenderStateCullMode());
   m_data[SBE_TEX].parameter_value = STREAM_BOOL_DYNAMIC_CAST(m_main->GetTexturing());
   m_data[SBE_MPF].parameter_value = STREAM_FLT_DYNAMIC_CAST(m_main->GetMipFiltration());
   m_data[SBE_MNF].parameter_value = STREAM_FLT_DYNAMIC_CAST(m_main->GetMinFiltration());
   m_data[SBE_MGF].parameter_value = STREAM_FLT_DYNAMIC_CAST(m_main->GetMagFiltration());

   if (m_engine->CheckShaderUsage())
   {
      m_data[SBE_MPF].title_color = m_data[SBE_MPF].value_color = FONT_COLOR_GY;
      m_data[SBE_MNF].title_color = m_data[SBE_MNF].value_color = FONT_COLOR_GY;
      m_data[SBE_MGF].title_color = m_data[SBE_MGF].value_color = FONT_COLOR_GY;
   }
   else
   {
      m_data[SBE_MPF].title_color = m_data[SBE_MPF].value_color = FONT_COLOR_WE;
      m_data[SBE_MNF].title_color = m_data[SBE_MNF].value_color = FONT_COLOR_WE;
      m_data[SBE_MGF].title_color = m_data[SBE_MGF].value_color = FONT_COLOR_WE;
   }

   m_data[SBE_MMB].parameter_value = std::to_string(m_main->GetMipmapBias());
   m_data[SBE_ZTU].parameter_value = STREAM_BOOL_DYNAMIC_CAST(m_main->GetZTestUsage());
   m_data[SBE_RT].parameter_value = STREAM_RTYPE_DYNAMIC_CAST(m_manager->GetRenderType());
   m_data[SBE_SSU].parameter_value = STREAM_BOOL_DYNAMIC_CAST(m_engine->CheckShaderUsage());

   if (m_engine->GetWRLDType() == MBE_ALPHADEMO)
      m_data[SBE_SSU].title_color = m_data[SBE_SSU].value_color = FONT_COLOR_GY;
   else
      m_data[SBE_SSU].title_color = m_data[SBE_SSU].value_color = FONT_COLOR_WE;

   m_data[SBE_SMI].parameter_value = STREAM_SHOW_DYNAMIC_CAST(m_showMainInfo);
   m_data[SBE_SDI].parameter_value = STREAM_SHOW_DYNAMIC_CAST(m_showDebugInfo);
   m_data[SBE_SSI].parameter_value = STREAM_SHOW_DYNAMIC_CAST(m_showServiceInfo);
   m_data[SBE_SH].parameter_value = STREAM_SHOW_DYNAMIC_CAST(m_showHUD);
   //sstr << "|| Debug information : ||" << std::endl
      //<< "avg fps : " << cbi_FPS << std::endl
      //<< "avg fps draw : " << (cbi_FPS < 60 ? 60 - cbi_FPS : 0) << std::endl;
   //sstr << "Render fill mode : ";
   //STREAM_FMODE_DYNAMIC_CAST(sstr, cbi_camera->getRenderStateFillMode());
   //sstr << "Render cull mode : ";
   //STREAM_CMODE_DYNAMIC_CAST(sstr, cbi_camera->getRenderStateCullMode());
   //sstr << "Texturing : ";
   //STREAM_BOOL_DYNAMIC_CAST(sstr, cbi_main->getTexturing());
   //sstr << "Mip filtration : ";
   //STREAM_FLT_DYNAMIC_CAST(sstr, cbi_main->getMipFiltration());
   //sstr << "Min filtration : ";
   //STREAM_FLT_DYNAMIC_CAST(sstr, cbi_main->getMinFiltration());
   //sstr << "Mag filtration : ";
   //STREAM_FLT_DYNAMIC_CAST(sstr, cbi_main->getMagFiltration());
   //sstr << "Mipmap bias : ";
   //sstr << cbi_main->getMipmapBias() << std::endl;
   //sstr << "Z-Test usage : ";
   //STREAM_BOOL_DYNAMIC_CAST(sstr, cbi_main->getZTestUsage());
   //sstr << "Render type :";
   //STREAM_RTYPE_DYNAMIC_CAST(sstr, cbi_manager->getRenderType());
   //sstr << "Shader system usage (OBT) : ";
   //STREAM_BOOL_DYNAMIC_CAST(sstr, cbi_engine->checkShaderUsage());
   //sstr << "Show main info : ";
   //STREAM_SHOW_DYNAMIC_CAST(sstr, cbi_showMainInfo);
   //sstr << "Show debug info : ";
   //STREAM_SHOW_DYNAMIC_CAST(sstr, cbi_showDebugInfo);		//не обязательно
   //sstr << "Show maintenance info : ";
   //STREAM_SHOW_DYNAMIC_CAST(sstr, cbi_showServiceInfo);
   //sstr << "|| Camera info : ||" << std::endl
   m_data[SBE_CP].parameter_value = std::string("[") + std::to_string(camPos.x) + std::string(", ") + std::to_string(camPos.y) + std::string(", ") + std::to_string(camPos.z) + std::string("]");
   //<< "CM Position : [" << camPos.x << ", " << camPos.y << ", " << camPos.z << " ]" << std::endl
   m_data[SBE_CT].parameter_value = std::string("[") + std::to_string(camTarget.x) + std::string(", ") + std::to_string(camTarget.y) + std::string(", ") + std::to_string(camTarget.z) + std::string("]");
   //<< "CM Target : [" << camTarget.x << ", " << camTarget.y << ", " << camTarget.z << " ]" << std::endl
   m_data[SBE_CD].parameter_value = std::string("[") + std::to_string(camTarget.x - camPos.x) + std::string(", ") + std::to_string(camTarget.y - camPos.y) + std::string(", ") + std::to_string(camTarget.z - camPos.z) + std::string("]");
   //<< "CM Direction : [" << camTarget.x - camPos.x << ", " << camTarget.y - camPos.y << ", " << camTarget.z - camPos.z << " ]" << std::endl
   m_data[SBE_CMS].parameter_value = std::to_string(m_camera->GetMovementSpeed());
   //<< "CM Movement speed : " << cbi_camera->getMovementSpeed() << std::endl;
   m_data[SBE_CMT].parameter_value = STREAM_CMT_DYNAMIC_CAST(m_camera->GetCameraMoveType());
   //sstr << "Camera move type : ";
   //STREAM_CMT_DYNAMIC_CAST(sstr, cbi_camera->getCameraMoveType());
   m_data[SBE_CCB].parameter_value = STREAM_BCC_DYNAMIC_CAST(m_camera->GetCameraCollision());
   //sstr << "Camera collision block : ";
   //STREAM_BCC_DYNAMIC_CAST(sstr, cbi_camera->getCameraCollision());
   m_data[SBE_CCD].parameter_value = STREAM_BOOL_DYNAMIC_CAST(m_camera->GetCurrentCollision());
   //sstr << "Camera collision detected : ";
   //STREAM_BOOL_DYNAMIC_CAST(sstr, cbi_camera->getCurrentCollision());

   //dstr = sstr.str();

   //return dstr;
}

// Обновление системной информации
void Infoboard::getServiceInfo()
{
   //std::string dstr = "";
   if (!m_showServiceInfo)
      return;
   //static std::stringstream sstr;
   static bool mounted = false;

   if (!mounted)
   {

      //Step 1
      D3DADAPTER_IDENTIFIER9 AdapterIdentifier;

      // Step 2. Assuming m_pD3D points to a valid LPDIRECT3D9 object...
      m_d3d9->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &AdapterIdentifier);

      // Get the max size of the graphics card identifier string
      const int cch = sizeof(AdapterIdentifier.Description);
      const int dn = sizeof(AdapterIdentifier.Driver);
      LARGE_INTEGER dv = AdapterIdentifier.DriverVersion;
      int Product = HIWORD(dv.HighPart);
      int Version = LOWORD(dv.HighPart);
      int SubVersion = HIWORD(dv.LowPart);
      int Build = LOWORD(dv.LowPart);

      // Define a TCHAR to hold the description
      TCHAR szDescription[cch];
      TCHAR szDriver[dn];
      char sD[cch], sDN[dn];
      // Use the unicode DX utility to convert the char string to TCHAR
      DXUtil_ConvertAnsiStringToGenericCch(szDescription, AdapterIdentifier.Description, cch);
      DXUtil_ConvertAnsiStringToGenericCch(szDriver, AdapterIdentifier.Driver, cch);
      size_t i, j;
      wcstombs_s(&i, sD, cch, szDescription, cch);
      wcstombs_s(&j, sDN, cch, szDriver, dn);
      //sstr << std::endl
      //<< "Maintenance information : " << std::endl
      m_data[SBE_VA].parameter_value = std::string(sD);
      //<< "Video Adapter : " << sD << std::endl
      m_data[SBE_VD].parameter_value = std::string(sDN);
      //<< "Video Driver : " << sDN << std::endl
      m_data[SBE_DDV].parameter_value = std::to_string(Product) + std::string(".") + std::to_string(Version) + std::string(".") + std::to_string(SubVersion) + std::string(".") + std::to_string(Build);
      //<< "DX driver version : " << Product << "." << Version << "." << SubVersion << "." << Build << std::endl;
      mounted = true;
   }
   //dstr = sstr.str();
   //return dstr;
}

// Обновление информации о движении автомобиля
void Infoboard::getHudInfo()
{
   //std::string dstr = "";
   if (!m_showHUD)
      return;
   //std::stringstream sstr;

   D3DXVECTOR3 carPos = m_carmove->GetCoordinatesWOMove();
   D3DXVECTOR3 carLastWaypoint = m_carmove->GetLastWaypoint();
   D3DXVECTOR3 carNextWaypoint = m_carmove->GetNextWaypoint();
   bool carManual = (m_carmove->GetWPType() == WBC_USERCONTROLLED);
   bool wTrfrmd = (m_engine->GetWRLDType() == MBE_ALPHADEMO);

   m_data[SBE_LCT].parameter_value = STREAM_WTYPE_DYNAMIC_CAST(m_carmove->GetWPType());
   m_data[SBE_LP].parameter_value = std::string("[") + std::to_string(carPos.x) + std::string(", ") + std::to_string(carPos.y) + std::string(", ") + std::to_string(carPos.z) + std::string("]");
   m_data[SBE_LYA].parameter_value = std::to_string(m_carmove->GetAngleYaw());
   m_data[SBE_LLWN].parameter_value = std::to_string(m_carmove->GetLastWaypointNum());
   m_data[SBE_LLWP].parameter_value = std::string("[") + std::to_string(carLastWaypoint.x) + std::string(", ") + std::to_string(carLastWaypoint.y) + std::string(", ") + std::to_string(carLastWaypoint.z) + std::string("]");
   m_data[SBE_LNWP].parameter_value = std::string("[") + std::to_string(carNextWaypoint.x) + std::string(", ") + std::to_string(carNextWaypoint.y) + std::string(", ") + std::to_string(carNextWaypoint.z) + std::string("]");
   m_data[SBE_LMS].parameter_value = std::to_string(m_carmove->GetCurrentMoveSpeed());
   m_data[SBE_LTR].parameter_value = std::to_string(m_carmove->GetCurrentTurnRadius());
   m_data[SBE_LMW].parameter_value = STREAM_CMW_DYNAMIC_CAST(m_carmove->GetCarMoveType());
   m_data[SBE_LTA].parameter_value = std::to_string(m_carmove->GetCurrentTurnAngle());
   m_data[SBE_LDT].parameter_value = STREAM_DTYPE_DYNAMIC_CAST(m_carmove->GetDriveType());
   m_data[SBE_LCG].parameter_value = std::to_string(m_carmove->GetCurrentGear());
   m_data[SBE_LTS].parameter_value = STREAM_SHIFT_DYNAMIC_CAST(m_carmove->GetShiftStatus());
   m_data[SBE_LAS].parameter_value = STREAM_ACC_DYNAMIC_CAST(m_carmove->GetAcceleratorStatus());
   if (wTrfrmd)
   {
      for (DWORD i = SBE_LCT; i <= SBE_LAS; i++)
         m_data[i].title_color = m_data[i].title_color = FONT_COLOR_GY;
      return;
   }
   else
   {
      for (DWORD i = SBE_LCT; i <= SBE_LYA; i++)
         m_data[i].title_color = m_data[i].value_color = FONT_COLOR_WE;
      m_data[SBE_LMS].title_color = m_data[SBE_LMS].value_color = FONT_COLOR_WE;
   }

   if (carManual)
   {
      m_data[SBE_LLWN].title_color = m_data[SBE_LLWN].value_color = FONT_COLOR_GY;
      m_data[SBE_LLWP].title_color = m_data[SBE_LLWP].value_color = FONT_COLOR_GY;
      m_data[SBE_LNWP].title_color = m_data[SBE_LNWP].value_color = FONT_COLOR_GY;
      m_data[SBE_LTR].title_color = m_data[SBE_LTR].value_color = FONT_COLOR_GY;

      m_data[SBE_LTA].title_color = m_data[SBE_LTA].value_color = FONT_COLOR_WE;
      m_data[SBE_LDT].title_color = FONT_COLOR_WE;
      m_data[SBE_LDT].value_color = STREAM_DTYPE_CLR_DYNAMIC_CAST(m_carmove->GetDriveType());
      m_data[SBE_LCG].title_color = m_data[SBE_LCG].value_color = FONT_COLOR_WE;
      m_data[SBE_LTS].title_color = m_data[SBE_LTS].value_color = FONT_COLOR_WE;
      m_data[SBE_LAS].title_color = m_data[SBE_LAS].value_color = FONT_COLOR_WE;
   }
   else
   {
      m_data[SBE_LLWN].title_color = m_data[SBE_LLWN].value_color = FONT_COLOR_WE;
      m_data[SBE_LLWP].title_color = m_data[SBE_LLWP].value_color = FONT_COLOR_WE;
      m_data[SBE_LNWP].title_color = m_data[SBE_LNWP].value_color = FONT_COLOR_WE;
      m_data[SBE_LTR].title_color = m_data[SBE_LTR].value_color = FONT_COLOR_WE;

      m_data[SBE_LTA].title_color = m_data[SBE_LTA].value_color = FONT_COLOR_GY;
      m_data[SBE_LDT].title_color = m_data[SBE_LDT].value_color = FONT_COLOR_GY;
      m_data[SBE_LCG].title_color = m_data[SBE_LCG].value_color = FONT_COLOR_GY;
      m_data[SBE_LTS].title_color = m_data[SBE_LTS].value_color = FONT_COLOR_GY;
      m_data[SBE_LAS].title_color = m_data[SBE_LAS].value_color = FONT_COLOR_GY;
   }
   //sstr << "|| Lamborghini Car info : ||" << std::endl
   //<< "LC Control type : ";
   //STREAM_WTYPE_DYNAMIC_CAST(sstr, cbi_carmove->getWPType());
   //sstr << "LC Position : [" << carPos.x << ", " << carPos.y << ", " << carPos.z << " ]" << std::endl
   //<< "LC Yaw angle : " << cbi_carmove->getAngleYaw() << std::endl
   //<< "LC Last waypoint number : " << cbi_carmove->getLastWaypointNum() << std::endl
   //<< "LC Last waypoint position : [" << carLastWaypoint.x << ", " << carLastWaypoint.y << ", " << carLastWaypoint.z << " ]" << std::endl
   //<< "LC Next waypoint position : [" << carNextWaypoint.x << ", " << carNextWaypoint.y << ", " << carNextWaypoint.z << " ]" << std::endl
   //<< "LC Move speed : " << cbi_carmove->getCurrentMoveSpeed() << std::endl
   //<< "LC Turn radius : " << cbi_carmove->getCurrentTurnRadius() << std::endl;
   //sstr << "LC Move way : ";
   //STREAM_CMW_DYNAMIC_CAST(sstr, cbi_carmove->getCarMoveType());
   //sstr << "LC Turn angle : " << cbi_carmove->getCurrentTurnAngle() << std::endl;
   //sstr << "LC Drive type : ";
   //STREAM_DTYPE_DYNAMIC_CAST(sstr, cbi_carmove->getDriveType());
   //sstr << "LC Current Gear : " << cbi_carmove->getCurrentGear() << std::endl;
   //sstr << "LC Transmission status : ";
   //STREAM_SHIFT_DYNAMIC_CAST(sstr, cbi_carmove->getShiftStatus());
   //sstr << "LC Accelerator status : ";
   //STREAM_ACC_DYNAMIC_CAST(sstr, cbi_carmove->getAcceleratorStatus());

   //dstr = sstr.str();

   //return dstr;
}

// Вспомогательная функция подсчета фпс
float Infoboard::getElapsedTimeInSeconds()
{
   // Returns the elapsed time (in seconds) since the last time this function
   // was called. This elaborate setup is to guard against large spikes in
   // the time returned by QueryPerformanceCounter().

   static const int MAX_SAMPLE_COUNT = 50;

   static float frameTimes[MAX_SAMPLE_COUNT];
   static float timeScale = 0.0f;
   static float actualElapsedTimeSec = 0.0f;
   static INT64 freq = 0;
   static INT64 lastTime = 0;
   static int sampleCount = 0;
   static bool initialized = false;

   INT64 time = 0;
   float elapsedTimeSec = 0.0f;

   if (!initialized)
   {
      initialized = true;
      QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&freq));
      QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&lastTime));
      timeScale = 1.0f / freq;
   }

   QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&time));
   elapsedTimeSec = (time - lastTime) * timeScale;
   lastTime = time;

   if (fabsf(elapsedTimeSec - actualElapsedTimeSec) < 1.0f)
   {
      memmove(&frameTimes[1], frameTimes, sizeof(frameTimes)-sizeof(frameTimes[0]));
      frameTimes[0] = elapsedTimeSec;

      if (sampleCount < MAX_SAMPLE_COUNT)
         ++sampleCount;
   }

   actualElapsedTimeSec = 0.0f;

   for (int i = 0; i < sampleCount; ++i)
      actualElapsedTimeSec += frameTimes[i];

   if (sampleCount > 0)
      actualElapsedTimeSec /= sampleCount;

   return actualElapsedTimeSec;
}

// Обновление фпс
void Infoboard::updateFrameRate(float elapsedTimeSec)
{
   static float accumTimeSec = 0.0f;
   static int frames = 0;

   accumTimeSec += elapsedTimeSec;

   if (accumTimeSec > 1.0f)
   {
      m_fps = frames;

      frames = 0;
      accumTimeSec = 0.0f;
   }
   else
      ++frames;
}