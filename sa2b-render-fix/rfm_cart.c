/********************************/
/*  Includes                    */
/********************************/
/****** SAMT ************************************************************************************/
#include <samt/core.h>              /* core                                                     */
#include <samt/writemem.h>          /* writedata                                                */
#include <samt/writeop.h>           /* writecall                                                */
#include <samt/string.h>            /* strformat                                                */

/****** Ninja ***********************************************************************************/
#include <samt/ninja/ninja.h>       /* ninja                                                    */

/****** Game ************************************************************************************/
#include <samt/sonic/game.h>        /* stagenumber                                              */
#include <samt/sonic/light.h>       /* loadlightfile                                            */
#include <samt/sonic/datadll.h>     /* getting data_dll assets                                  */

/****** Render Fix ******************************************************************************/
#include <rf_core.h>                /* core                                                     */
#include <rf_config.h>              /* getconfig                                                */
#include <rf_file.h>                /* files                                                    */

/****** RF Utility ******************************************************************************/
#include <rfu_float.h>              /* replacefloat                                             */

/****** Self ************************************************************************************/
#include <rfm_cart.h>               /* self                                                     */

/********************************/
/*  Game Defs                   */
/********************************/
/****** Function Ptrs **************************************************************************/
#define CartSeaDisplay              FUNC_PTR(void, __cdecl, (void), 0x00621C90)



typedef struct
{
    char data0[4];
    NJS_CNK_OBJECT* pObject;
    NJS_CNK_OBJECT* pLod;
    NJS_TEXLIST* pTexlist;
    char data1[8];
    short shrt0;
}
SPECIAL_INFO;

typedef struct 
{
    int character;
    int portrait;
    NJS_CNK_OBJECT* pObject;
    char speedstat;
    char accelstat;
    char brakestat;
    char gripstat;
}
CART_MENU;

enum player00_chars {
    p0_SONIC = 0,
    p0_TAILS = 1,
    p0_KNUCKLES = 2,
    p0_SHADOW = 3,
    p0_EGGMAN = 4,
    p0_ROUGE = 5,
    p0_SONIC_EX = 6,
    p0_CHAO = 7,
    p0_KNUCKLES_EX = 8,
    p0_SHADOW_EX = 9,
    p0_EGGMAN_EX = 10,
    p0_EGGROBO = 11
};

enum specialinfo_chars {
    sp_STORYMODE_TAILS = 0,
    sp_STORYMODE_ROUGE = 1,
    sp_SONIC = 2,
    sp_KNUCKLES = 3,
    sp_TAILS = 4,
    sp_EGGMAN = 5,
    sp_SHADOW = 6,
    sp_ROUGE = 7
};

/********************************/
/*  Prototypes                  */
/********************************/
/****** Static **********************************************************************************/
static void
LoadCartLightFiles(void)
{
    c8 ubuf[32];

    mtStrFormat(ubuf, 32, "stg%02i_light.bin", ssStageNumber);

    LoadLightFile(ubuf);
}

static void
ReplaceCartModels(void)
{
    SPECIAL_INFO* ingameKarts = GetDataDllAddr(SPECIAL_INFO, "specialInfo");
    CART_MENU* menuKarts = GetDataDllAddr(CART_MENU, "player00");
    
    menuKarts[p0_SONIC].pObject = RF_ChunkLoadObjectFile("cart/cart_sonic");
    menuKarts[p0_TAILS].pObject = RF_ChunkLoadObjectFile("cart/cart_tails");
    menuKarts[p0_KNUCKLES].pObject = RF_ChunkLoadObjectFile("cart/cart_knuckles");
    menuKarts[p0_SHADOW].pObject = RF_ChunkLoadObjectFile("cart/cart_shadow");
    menuKarts[p0_EGGMAN].pObject = RF_ChunkLoadObjectFile("cart/cart_eggman");
    menuKarts[p0_ROUGE].pObject = RF_ChunkLoadObjectFile("cart/cart_rouge");

    ingameKarts[sp_STORYMODE_TAILS].pObject = RF_ChunkLoadObjectFile("cart/cart_tails_big");
    ingameKarts[sp_STORYMODE_ROUGE].pObject = RF_ChunkLoadObjectFile("cart/cart_rouge_big");
    ingameKarts[sp_STORYMODE_ROUGE].pLod = RF_ChunkLoadObjectFile("cart/cart_rouge_big_lod");

    ingameKarts[sp_SONIC].pObject = RF_ChunkLoadObjectFile("cart/cart_sonic");
    ingameKarts[sp_SONIC].pLod = RF_ChunkLoadObjectFile("cart/cart_sonic_lod");

    ingameKarts[sp_TAILS].pObject = RF_ChunkLoadObjectFile("cart/cart_tails");
    ingameKarts[sp_TAILS].pLod = RF_ChunkLoadObjectFile("cart/cart_tails_lod");

    ingameKarts[sp_KNUCKLES].pObject = RF_ChunkLoadObjectFile("cart/cart_knuckles");
    ingameKarts[sp_KNUCKLES].pLod = RF_ChunkLoadObjectFile("cart/cart_knuckles_lod");

    ingameKarts[sp_SHADOW].pObject = RF_ChunkLoadObjectFile("cart/cart_shadow");
    ingameKarts[sp_SHADOW].pLod = RF_ChunkLoadObjectFile("cart/cart_shadow_lod");

    ingameKarts[sp_EGGMAN].pObject = RF_ChunkLoadObjectFile("cart/cart_eggman");
    ingameKarts[sp_EGGMAN].pLod = RF_ChunkLoadObjectFile("cart/cart_eggman_lod");

    ingameKarts[sp_ROUGE].pObject = RF_ChunkLoadObjectFile("cart/cart_rouge");
    ingameKarts[sp_ROUGE].pLod = RF_ChunkLoadObjectFile("cart/cart_rouge_lod");
}

/****** Init ************************************************************************************/
void
RFM_CartInit(void)
{

    ReplaceCartModels();

    if (RF_ConfigGetInt(CNF_CART_KANBAN))
    {
        WriteData(0x006224EC, -1063 + 28, s32);
    }

    switch (RF_ConfigGetInt(CNF_CART_CARTSPRITE))
    {
        case CNFE_CART_CARTSPRITE_DREAMCAST:
        {
            WriteNOP(0x0061CB07, 0x0061CB0C);
            break;
        }
        case CNFE_CART_CARTSPRITE_ENABLED:
        {
            RFU_ReplaceFloat(0x0061EE25, 6.0); // sprite sub
            break;
        }
    }

    if (RF_ConfigGetInt(CNF_CART_ZANKI))
    {
        /** Okay, but how does a mistake like this occur? **/
        const f64 zanki_py = 424.0 + 3.0 + 7.0;

        RFU_ReplaceFloat(0x0044E78F, zanki_py);
    }

    // load cart light files
    WriteNOP( 0x0061A4F9, 0x0061A53D);
    WriteCall(0x0061A4F9, LoadCartLightFiles);
}
