/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  ABOUT                            1       /* callback function: exitAbout */
#define  ABOUT_PICTURE                    2       /* control type: picture, callback function: (none) */
#define  ABOUT_NAMETEXT                   3       /* control type: textMsg, callback function: (none) */
#define  ABOUT_IDTEXT                     4       /* control type: textMsg, callback function: (none) */

#define  CHEATMENU                        2       /* callback function: exitCheat */
#define  CHEATMENU_LEVELSELECT            2       /* control type: ring, callback function: (none) */
#define  CHEATMENU_CHEATCANVAS            3       /* control type: canvas, callback function: (none) */
#define  CHEATMENU_LIVES                  4       /* control type: numeric, callback function: (none) */
#define  CHEATMENU_POWERUPS               5       /* control type: textButton, callback function: (none) */
#define  CHEATMENU_ROUGETIMER             6       /* control type: timer, callback function: ShowRouge */

#define  GAME                             3       /* callback function: exitGame */
#define  GAME_CANVAS                      2       /* control type: canvas, callback function: (none) */
#define  GAME_TIMER                       3       /* control type: timer, callback function: game */
#define  GAME_BULLETTIMER                 4       /* control type: timer, callback function: TimeBullets */

#define  LEADER                           4       /* callback function: exitLeader */
#define  LEADER_TABLE                     2       /* control type: table, callback function: (none) */
#define  LEADER_LEADERBOARDLOGO           3       /* control type: textMsg, callback function: (none) */

#define  MENU                             5       /* callback function: exitMenu */
#define  MENU_PICTURE                     2       /* control type: picture, callback function: (none) */
#define  MENU_STARTGAME                   3       /* control type: command, callback function: startGame */
#define  MENU_LEADERSHOW                  4       /* control type: command, callback function: showLeaderBoards */
#define  MENU_QUITGAME                    5       /* control type: command, callback function: quitGameButton */
#define  MENU_CHEATS                      6       /* control type: command, callback function: cheatMenu */
#define  MENU_TEXTMSG                     7       /* control type: textMsg, callback function: (none) */
#define  MENU_KONAMICODE                  8       /* control type: command, callback function: codeLink */
#define  MENU_STRING                      9       /* control type: string, callback function: (none) */

#define  SETTINGS                         6       /* callback function: exitSettings */
#define  SETTINGS_SFXSLIDE                2       /* control type: scale, callback function: SoundSet */
#define  SETTINGS_MUSICSLIDE              3       /* control type: scale, callback function: SoundSet */
#define  SETTINGS_DIFFICULTY              4       /* control type: ring, callback function: ChangeDiff */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_SETTINGS                 2       /* callback function: OpenSettings */
#define  MENUBAR_DEMO                     3       /* callback function: OpenDemo */
#define  MENUBAR_HELP                     4       /* callback function: OpenHelp */
#define  MENUBAR_MENU1                    5       /* callback function: OpenAbout */


     /* Callback Prototypes: */

int  CVICALLBACK ChangeDiff(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cheatMenu(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK codeLink(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitAbout(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitCheat(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitGame(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitLeader(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitMenu(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK exitSettings(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK game(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK OpenAbout(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK OpenDemo(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK OpenHelp(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK OpenSettings(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK quitGameButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK showLeaderBoards(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ShowRouge(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SoundSet(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK startGame(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimeBullets(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
