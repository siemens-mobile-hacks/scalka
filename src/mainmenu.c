#include <swilib.h>
#include <string.h>
#include "settings.h"
#include "mainmenu.h"

extern volatile int calc_gui_id;
extern CALC_SETTINGS calc_set;
extern double d_answer;

static void mm_ac(GUI *gui)
{
  extern void remove_all_operation(void);
  remove_all_operation();
  GeneralFuncF1(1);
}

static void mm_ans2x(GUI *gui)
{
  calc_set.x=d_answer;
  GeneralFuncF1(1);
}

static void mm_ans2y(GUI *gui)
{
  calc_set.y=d_answer;
  GeneralFuncF1(1);
}

static void mm_settings(GUI *gui) {
    extern char CFG_PATH[];
    WSHDR ws;
    uint16_t wsbody[128];
    CreateLocalWS(&ws, wsbody, 127);
    str_2ws(&ws, CFG_PATH, 127);
    ExecuteFile(&ws, NULL, NULL);
    GeneralFuncF1(1);
}

static void mm_exit(GUI *gui)
{
  GeneralFunc_flag1(calc_gui_id,0xFF);
  GeneralFuncF1(1);

}

static const int mmenusoftkeys[]={0,1,2};

static const SOFTKEY_DESC mmenu_sk[]=
{
  {0x0018,0x0000,(int)"Выбор"},
  {0x0001,0x0000,(int)"Назад"},
  {0x003D,0x0000,(int)LGP_DOIT_PIC}
};

static const SOFTKEYSTAB mmenu_skt=
{
  mmenu_sk,0
};

#define MAIN_MENU_ITEMS_N 5
static HEADER_DESC mmenu_hdr={{0,0,0,0},NULL,(int)"Меню",LGP_NULL};

static MENUITEM_DESC mmenu_ITEMS[MAIN_MENU_ITEMS_N]=
{
  {NULL,(int)"AC",           LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}, //0
  {NULL,(int)"ANS->X",       LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}, //0
  {NULL,(int)"ANS->Y",       LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}, //0
  {NULL,(int)"Настройки",    LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2}, //0
  {NULL,(int)"Выход",        LGP_NULL, 0, NULL, MENU_FLAG3, MENU_FLAG2} //0
};

static const MENUPROCS_DESC mmenu_HNDLS[MAIN_MENU_ITEMS_N]=
{
  mm_ac,
  mm_ans2x,
  mm_ans2y,
  mm_settings,
  mm_exit
};


static int mmenu_keyhook(GUI *gui, GUI_MSG *msg)
{
  return(0);
}

static void mmenu_ghook(GUI *gui, int cmd)
{
  if (cmd==0x0A)
  {
    DisableIDLETMR();
  }
}


static const MENU_DESC mmenu=
{
  8,mmenu_keyhook,mmenu_ghook,NULL,
  mmenusoftkeys,
  &mmenu_skt,
  0x10,//MENU_FLAG,
  NULL,
  mmenu_ITEMS,//menuitems,
  mmenu_HNDLS,//menuprocs,
  MAIN_MENU_ITEMS_N
};

int ShowMenu(void)
{
  RECT *rc = GetHeaderRECT();
  memcpy(&(mmenu_hdr.rc), rc, sizeof(RECT));
  return CreateMenu(0,0,&mmenu,&mmenu_hdr,0,MAIN_MENU_ITEMS_N,0,0);
}
