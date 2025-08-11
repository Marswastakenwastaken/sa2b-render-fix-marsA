/********************************/
/*  Includes                    */
/********************************/
/****** SAMT ************************************************************************************/
#include <samt/core.h>              /* core                                                     */

/****** Render Fix ******************************************************************************/
#include <rf_core.h>                /* core                                                     */
#include <rf_font.h>                /* rf font                                                  */

/****** Self ************************************************************************************/
#include <rf_api/rfapi_internal.h>  /* parent & siblings                                        */

/********************************/
/*  Constants                   */
/********************************/
/****** API Module Version **********************************************************************/
#define FONT_API_VER                (0) /* font api version                                     */

/********************************/
/*  Export Data                 */
/********************************/
/****** Font API ********************************************************************************/
EXTERN
const RFAPI_FONT rfapi_font =
{
    .version = FONT_API_VER,

    .LoadFontFile   = RF_FontLoadFile,
    .HalfSpaceWidth = RF_FontHalfSpace,

    .SetWidthList = RF_FontSetLeft,

    .SetFont = RF_FontReplace,
    .GetFont = RF_FontGet,

    .SetChaoFont = RF_FontChaoReplace,
    .GetChaoFont = RF_FontChaoGet,
};
