/********************************/
/*  Includes                    */
/********************************/
/****** SAMT ************************************************************************************/
#include <samt/core.h>              /* core                                                     */
#include <samt/writemem.h>          /* writedata                                                */
#include <samt/writeop.h>           /* writecall                                                */
#include <samt/string.h>            /* strformat                                                */
#include <samt/funchook.h>          /* hooking                                                  */

/****** Ninja ***********************************************************************************/
#include <samt/ninja/ninja.h>       /* ninja                                                    */

/****** Game ************************************************************************************/
#include <samt/sonic/game.h>        /* stagenumber                                              */
#include <samt/sonic/light.h>       /* loadlightfile                                            */
#include <samt/sonic/cart/cartcar.h>/* cart                                                     */

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
    CAR_MODEL_INFO ingameKarts[9] = specialInfo;
    CAR_INFO menuKarts[2][6] = player00;
    
    menuKarts[0][0].obj = RF_ChunkLoadObjectFile("cart/cart_sonic");
    menuKarts[0][1].obj = RF_ChunkLoadObjectFile("cart/cart_tails");
    menuKarts[0][2].obj = RF_ChunkLoadObjectFile("cart/cart_knuckles");
    menuKarts[0][3].obj = RF_ChunkLoadObjectFile("cart/cart_shadow");
    menuKarts[0][4].obj = RF_ChunkLoadObjectFile("cart/cart_eggman");
    menuKarts[0][5].obj = RF_ChunkLoadObjectFile("cart/cart_rouge");

    ingameKarts[CART_CAR_TAILS_SPECIAL].carObj = RF_ChunkLoadObjectFile("cart/cart_tails_big");
    ingameKarts[CART_CAR_ROUGE_SPECIAL].carObj = RF_ChunkLoadObjectFile("cart/cart_rouge_big");
    ingameKarts[CART_CAR_ROUGE_SPECIAL].carFarObj = RF_ChunkLoadObjectFile("cart/cart_rouge_big_lod");

    ingameKarts[CART_CAR_SONIC].carObj = RF_ChunkLoadObjectFile("cart/cart_sonic");
    ingameKarts[CART_CAR_SONIC].carFarObj = RF_ChunkLoadObjectFile("cart/cart_sonic_lod");

    ingameKarts[CART_CAR_TAILS].carObj = RF_ChunkLoadObjectFile("cart/cart_tails");
    ingameKarts[CART_CAR_TAILS].carFarObj = RF_ChunkLoadObjectFile("cart/cart_tails_lod");

    ingameKarts[CART_CAR_KNUCKLES].carObj = RF_ChunkLoadObjectFile("cart/cart_knuckles");
    ingameKarts[CART_CAR_KNUCKLES].carFarObj = RF_ChunkLoadObjectFile("cart/cart_knuckles_lod");

    ingameKarts[CART_CAR_SHADOW].carObj = RF_ChunkLoadObjectFile("cart/cart_shadow");
    ingameKarts[CART_CAR_SHADOW].carFarObj = RF_ChunkLoadObjectFile("cart/cart_shadow_lod");

    ingameKarts[CART_CAR_EGGMAN].carObj = RF_ChunkLoadObjectFile("cart/cart_eggman");
    ingameKarts[CART_CAR_EGGMAN].carFarObj = RF_ChunkLoadObjectFile("cart/cart_eggman_lod");

    ingameKarts[CART_CAR_ROUGE].carObj = RF_ChunkLoadObjectFile("cart/cart_rouge");
    ingameKarts[CART_CAR_ROUGE].carFarObj = RF_ChunkLoadObjectFile("cart/cart_rouge_lod");
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
