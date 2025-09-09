/********************************/
/*  Includes                    */
/********************************/
/****** SAMT ************************************************************************************/
#include <samt/core.h>              /* core                                                     */
#include <samt/memory.h>
#include <samt/writemem.h>          /* writedata                                                */
#include <samt/writeop.h>           /* writecall                                                */
#include <samt/funchook.h>          /* hooking                                                  */

/****** Ninja ***********************************************************************************/
#include <samt/ninja/ninja.h>       /* ninja                                                    */

/****** Game ************************************************************************************/
#include <samt/sonic/task.h>
#include <samt/sonic/camera.h>
#include <samt/sonic/njctrl.h>      /* control                                                  */
#include <samt/sonic/cart/cartcar.h>

/****** Render Fix ******************************************************************************/
#include <rf_core.h>                /* core                                                     */
#include <rf_file.h>                /* files                                                    */
#include <rf_draw.h>

#define GET_CARTWK(_tp)     ((CARTWK*)_tp->mwp)

typedef struct
{
    char gap0[8];
    Angle3 ang;
    NJS_POINT3 pos;
    char gap2[20];
    NJS_CNK_OBJECT* pObject;
    NJS_CNK_OBJECT* pFarObject;
    NJS_TEXLIST*    pTexlist;
    int headlights;
    b32 is_player;
    int character;
    int player_num;
    void* ptr1;
    char gap4[20];
    Angle angSlide;
    char gap5[132];
    Angle3 ang3;
    Angle3 ang2;
    char gap6[412];
    void* texp;
    int address_u;
    int address_v;
    int tes5;
    int min_filter;
    int mag_filter;
    int bank;
    __int32 stencil_data[1];
    void* stencil_tex;
    char gap7[28];
}
CARTWK;

#define PlayerCartSelected      DATA_ARY(s8, 0x0174B021, [2])

#define SQR(x)          ((x)*(x))

static void
cartDisplayerMod(task* tp)
{
    CARTWK* const cwp = GET_CARTWK(tp);
    CART_EXTRA_PARAM* exParam = GetDataDllAddr(CART_EXTRA_PARAM, "exParam");

    camcontwk* const camwk = cameraCurrentWork;

    NJS_POINT3 rel_pos = {
        .x = cwp->pos.x - camwk->pos.x,
        .y = cwp->pos.y - camwk->pos.y,
        .z = cwp->pos.z - camwk->pos.z,
    };

    const float fchk = (rel_pos.x * rel_pos.x) + (rel_pos.y * rel_pos.y) + (rel_pos.z * rel_pos.z);

    if (fchk > SQR(5000.f))
    {
        return;
    }

    OnControl3D(NJD_CONTROL_3D_SHADOW | NJD_CONTROL_3D_TRANS_MODIFIER);

    const NJS_POINT3* p_pos;

    if (cwp->is_player)
    {
        const int chnum = PlayerCartSelected[cwp->player_num];

        p_pos = &cwp->pos;
    }
    else
    {

        p_pos = &tp->twp->pos;
    }

    njPushMatrixEx();
    {
        njTranslate(NULL, p_pos->x, p_pos->y + 0.01f - 2.0f, p_pos->z);

        njRotateZ(NULL, cwp->ang.z);
        njRotateX(NULL, cwp->ang.x);
        njRotateY(NULL, cwp->ang.y + cwp->angSlide - 0x8000);

        njRotateZ(NULL, cwp->ang2.z + cwp->ang3.z);
        njRotateX(NULL, cwp->ang2.x + cwp->ang3.x);
        njRotateY(NULL, cwp->ang2.y + cwp->ang3.y);

        njCnkModDrawObject(exParam[cwp->character].kageObject);
    }
    njPopMatrixEx();

    OffControl3D(NJD_CONTROL_3D_SHADOW | NJD_CONTROL_3D_TRANS_MODIFIER);
}


#define courseDisplayDisplayer      FUNC_PTR(void, __cdecl, (task*), 0x00623E10)

static mt_hookinfo HookInfoCourseDisplayDisplayer[1];
static void
courseDisplayDisplayerHook(task* tp)
{
    OnControl3D(NJD_CONTROL_3D_SHADOW | NJD_CONTROL_3D_TRANS_MODIFIER);

    FuncHookCall( HookInfoCourseDisplayDisplayer, courseDisplayDisplayer(tp) );

    OffControl3D(NJD_CONTROL_3D_SHADOW | NJD_CONTROL_3D_TRANS_MODIFIER);
}

void* CreateNoStencilTexture(void);

void
CHS_CartInit(void)
{
    WriteJump(0x0061CB80, cartDisplayerMod);
    WriteCall(0x0061C60F, CreateNoStencilTexture);

    FuncHook(HookInfoCourseDisplayDisplayer, courseDisplayDisplayer, courseDisplayDisplayerHook);

    CART_EXTRA_PARAM* exParam = GetDataDllAddr(CART_EXTRA_PARAM, "exParam");
    CART_EXTRA_PARAM* exParamSpecial = GetDataDllAddr(CART_EXTRA_PARAM, "exParamSpecial");

    exParam[CART_CAR_TAILS_SPECIAL].kageObject = RF_ChunkLoadObjectFile("cart/cart_tails_big_mod");
    exParam[CART_CAR_ROUGE_SPECIAL].kageObject = RF_ChunkLoadObjectFile("cart/cart_rouge_big_mod");

    exParam[CART_CAR_SONIC].kageObject = RF_ChunkLoadObjectFile("cart/cart_sonic_mod");
    exParam[CART_CAR_KNUCKLES].kageObject = RF_ChunkLoadObjectFile("cart/cart_knuckles_mod");
    exParam[CART_CAR_TAILS].kageObject = RF_ChunkLoadObjectFile("cart/cart_tails_mod");
    exParam[CART_CAR_EGGMAN].kageObject = RF_ChunkLoadObjectFile("cart/cart_eggman_mod");
    exParam[CART_CAR_SHADOW].kageObject = RF_ChunkLoadObjectFile("cart/cart_rouge_mod");
    exParam[CART_CAR_ROUGE].kageObject = RF_ChunkLoadObjectFile("cart/cart_eggman_mod");

    exParamSpecial[0].kageObject = RF_ChunkLoadObjectFile("cart/cart_sonic_alt_mod");
    exParamSpecial[1].kageObject = RF_ChunkLoadObjectFile("cart/cart_knuckles_alt_mod");
    exParamSpecial[2].kageObject = RF_ChunkLoadObjectFile("cart/cart_chao_mod");
    exParamSpecial[3].kageObject = RF_ChunkLoadObjectFile("cart/cart_eggman_alt_mod");
    exParamSpecial[4].kageObject = RF_ChunkLoadObjectFile("cart/cart_shadow_alt_mod");
    exParamSpecial[5].kageObject = RF_ChunkLoadObjectFile("cart/cart_eggrobo_mod");
}
