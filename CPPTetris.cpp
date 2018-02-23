// CPPTetris.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "CPPTetris.h"
#include "BEndToFEnd.h"
#include <string>
#include <cmath>
#pragma region macros
#define MAX_LOADSTRING 100
#define TET_TSPAINT 0x0450
#define TET_GAMEOVER 0x0451
#define TET_NEWGAME 0x0452
#define TSB_PLAY 106
#define TSB_QUIT 107
#define TSB_SETTINGS 108
#define TET_LCLEAR 0x0453
#define VK_A 0x41
#define VK_W 0x57
#define VK_S 0x53
#define VK_D 0x44
#pragma endregion
using namespace std;
// Global Variables:
#pragma region Variables
#pragma region predefined
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];// the main window class name  
#pragma endregion
#pragma region Hbrush
//HBRUSES are usless will delete later if you see this in the github leave an issue to tell me to fix it
HBRUSH hb;
HBRUSH BSH;
HBRUSH white = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH DBSH;
#pragma endregion
#pragma region INTs
//xuih and yuih are the arrays used if somethign goes wrong
//tmp and tamp do various things as they both stand for temporary
INT8 xuih[5] = { 0, 1, 0, 0, 1 };
INT8 yuih[5] = { 0, 0, 1, -1, 1 };
INT8 tmp;
INT8* tamp;
#pragma endregion
#pragma region UINTs
//xnt is the x position of the next pieces
// ysc is the y position of the score
//nxtsz = next size: size for the next piece stuff
//mymmy is used for the size of maxy-miny
//clxt is the current tile for line clear since i needed something to store that
UINT16 xnt = ((boardw + 1)*Pzs);
UINT16 ysc = ((boardh + 1)*Pzs);
UINT8 nxtsz = 10;
UINT8 mymmy;
UINT8 clxt = 0;
UINT TimmerID = 0;
UINT CLtID = 0;
#pragma endregion
#pragma region bools
//most are used for drawing so i will only comment on the ones that arent
//isholding is true if there is a piece in the hold slot
// holdact is true when you use a hold and is set to false once you go to the next piece in order to prevent point spamming
bool isholding = false;
bool holdact = false;
bool pause = false;
bool repau = false;
bool gameover = false;
bool title = true;
bool lclear = false;
bool resize = false;
bool NG = true;
#pragma endregion
#pragma region Tblocks
//TestB is the current piece
//hold is the piece in the hold slot
//nextp isnt used beofre and should be deleted
// nexta is the array of next pieces
Tblock TestB(5, xuih, yuih);
Tblock hold(5, xuih, yuih);
Tblock nextp(5, xuih, yuih);
Tblock* nexta = NULL;
#pragma endregion
#pragma region strings
//uuuh is a normal sting version of the score
// lpMyString is a c string variant of uuuh
// the other 2 are just ones that i put in variables for no reason
wstring uuuh = to_wstring(score);
LPCWSTR lpMyString = uuuh.c_str();
const LPCWSTR pu = L"paused";
const LPCWSTR go = L"Game Over score:";
#pragma endregion
#pragma region misc
//ok stores the last keydown before a game over so that it doesnt start up as soon as you relase the key
// i EASILY could get away with using a static variable for clrct but i didnt for some reason
WPARAM ok;
RECT clrct;
#pragma endregion
#pragma region predefined function declerations
				// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
#pragma endregion
#pragma endregion
#pragma region functions
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, int nCmdShow)
{
#pragma region predone stuff
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CPPTETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
#pragma endregion
	//setting some values and calling startup
	StartUp();
	mymmy = (maxy - miny);
	nexta = new Tblock[npas];
	hold.color = 1;
	TestB.color = 1;
	hold.xpos = 13;
	hold.ypos = 2;
#pragma region predone stuff
// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPTETRIS));
	MSG msg;
// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
#pragma endregion
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPTETRIS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPPTETRIS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
  

   if (!hWnd)
   {
      return FALSE;
   }
   //TimmerID = SetTimer(hWnd, 101, 1000, NULL);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   //sends a message to paint the title screen
   SendMessage(hWnd, TET_TSPAINT, 0, 0);

   return TRUE;
}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				}
            case IDM_EXIT:
				{
					DestroyWindow(hWnd);
					break;
				}
			case TSB_PLAY:
				{
					SendMessage(hWnd, TET_NEWGAME, 1, 0);
					break;
				}
			case TSB_QUIT:
				{
					DestroyWindow(hWnd);
					break;
				}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
		break;
        }		
	case WM_TIMER:
		{
			switch (wParam)
			{
				case 101:
					{
						//101 is every second and drops the piece
						if (CanDown(TestB))
						{
							TestB.ypos++;
							InvalidateRect(hWnd, NULL, TRUE);
						}
						else
						{
							TestB.Set(hWnd);
							//TestB.SBL();
							SLN(&TestB, nexta, &hWnd, &wParam);
							holdact = false;
						}
						break;
					}
				case 102:
					{
						//102 is the clear line timer
						HDC hdec = GetDC(hWnd);
						if (clxt < boardw)
						{
							for (int ln = 0; ln < numoclns; ln++)
							{
								clrct.top = Pzs * lines[ln];
								clrct.bottom = clrct.top + Pzs;
								FillRect(hdec, &clrct, pall[0]);
							}
							clrct.right += Pzs;
							clrct.left += Pzs;
							clxt++;
						}
						else
						{
							SendMessage(hWnd, TET_LCLEAR, 1, 0);
						}
						break;
					}
			}
		}
	case WM_ERASEBKGND:
		return (LRESULT)1;
    case WM_PAINT:
        {
			RECT dorct;
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			uuuh = to_wstring(score);
			lpMyString = uuuh.c_str();
            // TODO: Add any drawing code that uses hdc here...
			if (resize == true)
			{
				//clears the screen if the window is resized
				FillRect(hdc, &ps.rcPaint, WHITE_BRUSH);
			}
			//this stupidity of an if statement basically just stops it from drawing when i dont want it to
			if (pause == false && gameover == false && title == false)
			{
				Bout(hdc, expp, eypp, ot);
				if (reboard == true || resize == true)
				{
					DrawBoard(hdc);
					reboard = false;
				}
				if (reghost == true || resize == true)
				{
					tmp = TestB.ypos;
					TestB.ypos = GPos;
					TestB.DrawMe(hdc, true);
					TestB.ypos = tmp;
					reghost = false;
				}
				TestB.DrawMe(hdc, false);
				if ((isholding == true && rehold == true) || resize == true)
				{
					//hold.DrawMe(hdc, hb);
					DrawNXT(hdc, hold, xnt, 0, nxtsz);
					rehold = false;
				}
				if (renext == true || resize == true)
				{
					int yoff = ((maxy - miny + 2) * nxtsz);
					for (int np = 0; np < npas; np++)
					{
						DrawNXT(hdc, nexta[np], xnt, yoff, nxtsz);
						yoff += ((maxy - miny + 2) * nxtsz);
					}
					renext = false;
				}
				if (NG == true)
				{
					SetRect(&dorct, 10, ysc, 100, ysc + 20);
					FillRect(hdc, &dorct, white);
					SetRect(&dorct, 240, 10, (240 + (maxx - minx + 1) * 10), (10 + (maxy - miny + 1) * 10));
					FillRect(hdc, &dorct, pall[0]);
					NG = false;
				}
				TextOut(hdc, 10, ysc, lpMyString, uuuh.length());
			}
			else if (repau == true)
			{
				//pausing stuff
				repau = false;
				SetRect(&dorct, 0, 0, (boardw*Pzs), (boardh*Pzs));
				FillRect(hdc, &dorct, white);
				//TextOut(hdc, 10, 460, lpMyString, uuuh.length());
				//TextOut(hdc, 10, 1000, pu, 7);

			}
			resize = false;
            EndPaint(hWnd, &ps);
			DeleteObject(hb);
			DeleteObject(BSH);
			DeleteObject(DBSH);
			break;
        }   
		//NOTE TO SELF: COMMENT KEYDOWN KEYUP AND SIZE ONCE I CHANGE THEM
	case WM_KEYDOWN:
		{
			if (wParam != VK_ESCAPE)
			{
				if (pause == false && gameover == false && title == false && lclear == false)
				{
					switch (wParam)
					{
						case VK_W:
						case VK_UP:
							{
								if (CanRCCW(TestB))
								{
									if (TestB.rotation == 3)
									{
										TestB.rotation = 0;
									}
									else
									{
										TestB.rotation++;
									}
									Ghost(TestB);
									InvalidateRect(hWnd, NULL, TRUE);
								}
								break;
							}
						case VK_A:
						case VK_LEFT:
							{
								if (CanLeft(TestB) == true)
								{
									TestB.xpos--;
									Ghost(TestB);
									InvalidateRect(hWnd, NULL, TRUE);
								}
								break;
							}
						case VK_D:
						case VK_RIGHT:
							{
								if (CanRight(TestB) == true)
								{
									TestB.xpos++;
									Ghost(TestB);
									InvalidateRect(hWnd, NULL, TRUE);
								}
								break;
							}
						case VK_S:
						case VK_DOWN:
							{
								if (CanDown(TestB))
								{
									TestB.ypos++;
									score++;
									InvalidateRect(hWnd, NULL, TRUE);
								}
								else
								{
									TestB.Set(hWnd);
									//TestB.SBL();
									SLN(&TestB, nexta, &hWnd, &wParam);
									holdact = false;
								}
								break;
							}
						case VK_SPACE:
						case VK_RETURN:
							{
								while (CanDown(TestB))
								{
									TestB.ypos++;
									score += 2;
								}
								TestB.Set(hWnd);
								//TestB.SBL();
								SLN(&TestB, nexta, &hWnd, &wParam);
								holdact = false;
								break;
							}
						case VK_TAB:
							{
								if (holdact == false)
								{
									if (isholding == true)
									{
										tamp = TestB.xar;
										TestB.xar = hold.xar;
										hold.xar = tamp;
										tamp = TestB.yar;
										TestB.yar = hold.yar;
										hold.yar = tamp;
										TestB.rotation = 0;
										TestB.xpos = 5;
										TestB.ypos = 2;
										tmp = TestB.blocknum;
										TestB.blocknum = hold.blocknum;
										hold.blocknum = tmp;
										tmp = TestB.color;
										TestB.color = hold.color;
										hold.color = tmp;
										Ghost(TestB);
									}
									else
									{
										hold.yar = TestB.yar;
										hold.xar = TestB.xar;
										hold.blocknum = TestB.blocknum;
										hold.color = TestB.color;
										//TestB.SBL();
										SLN(&TestB, nexta, &hWnd, &wParam);
										isholding = true;
									}
									holdact = true;
									rehold = true;
									InvalidateRect(hWnd, NULL, TRUE);
								}
								break;
							}
						default:
							break;

					}
				}
				/*else if (gameover == true)
				{
					if (wParam == VK_RETURN)
					{
						SendMessage(hWnd, TET_NEWGAME, 0, 0);
					}
				}*/
			}
			else if (pause == false && gameover == false && lclear == false)
			{
				pause = true;
				repau = true;
				KillTimer(hWnd, 101);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if(gameover == false && title == false && lclear == false)
			{
				pause = false;
				recur = true;
				renext = true;
				reboard = true;
				rehold = true;
				reghost = true;
				TimmerID = SetTimer(hWnd, 101, 1000, NULL);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
		}
		case WM_KEYUP:
			{
				if (gameover == true && wParam != ok)
				{
					if (wParam == VK_ESCAPE)
					{
						SendMessage(hWnd, TET_TSPAINT, 0, 0);
						title = true;
						gameover = false;
					}
					else
					{
						SendMessage(hWnd, TET_NEWGAME, 0, 0);
					}
				}
				else if (gameover == true)
				{
					ok = 0;
				}
				break;
			}
		case WM_SIZE:
			{
				Pzs = ((HIWORD(lParam)-20)/(boardh+1));
				xnt = ((boardw + 1)*Pzs);
				ysc = ((boardh + 1)*Pzs);
				resize = true;
				nxtsz = ((HIWORD(lParam))/ (npas + 1))/ (mymmy + 2);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
		case TET_TSPAINT:
			{
				//main menu drawing
				HDC tshdc = GetDC(hWnd);
				RECT crt;
				HWND newg;
				HWND qit;
				HWND set;
				SetRect(&crt, 0, 0, 350, 475);
				FillRect(tshdc, &crt, white);
				SetRect(&crt, 460, 100, 500, 140);
				FillRect(tshdc, &crt, pall[1]);
				SetRect(&crt, 460, 80, 500, 100);
				FillRect(tshdc, &crt, pall[1]);

				ReleaseDC(hWnd, tshdc);
				newg = CreateWindow( L"BUTTON", L"New Game", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 50, 10, 100, 20, hWnd, (HMENU)TSB_PLAY, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
				set = CreateWindow( L"BUTTON", L"Settings", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,50, 35, 100, 20, hWnd, (HMENU)TSB_SETTINGS,(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),NULL);
				qit = CreateWindow(L"BUTTON", L"quit", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 50, 60, 100, 20, hWnd, (HMENU)TSB_QUIT, (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
				break;
			}
		case TET_GAMEOVER:
			{
				KillTimer(hWnd, 101);
				gameover = true;
				HDC gdc = GetDC(hWnd);
				RECT crt;
				SetRect(&crt, 0, 0, (xnt+ (nxtsz*((maxx + 1)-minx))), (ysc + 20));
				FillRect(gdc, &crt, white);
				TextOut(gdc, 30, 20, go, 16);
				TextOut(gdc, 30, 40, lpMyString , uuuh.length());
				TextOut(gdc, 30, 60, L"press any key to try again", 26);
				ReleaseDC(hWnd, gdc);
				ok = wParam;
				break;
			}
		case TET_NEWGAME:
			{
				ResetVrs(&hWnd);
				gameover = false;
				
				if (wParam == 1)
				{
					title = false;
					SetFocus(hWnd);
					DestroyWindow(GetDlgItem(hWnd, TSB_PLAY));
					DestroyWindow(GetDlgItem(hWnd, TSB_QUIT));
					DestroyWindow(GetDlgItem(hWnd, TSB_SETTINGS));
				}
				for (int we = 0; we < npas; ++we)
				{
					nexta[we].SBL();
				}
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
		case TET_LCLEAR:
			{
				if (wParam == 0)
				{
					KillTimer(hWnd, 101);
					CLtID = SetTimer(hWnd, 102, 10, NULL);
					clrct.left = 0;
					clrct.right = Pzs;
					lclear = true;
				}
				else
				{
					KillTimer(hWnd, 102);
					TimmerID = SetTimer(hWnd, 101, 1000, NULL);
					delete[] lines;
					clxt = 0;
					numoclns = 0;
					clrct.left = 0;
					clrct.right = Pzs;
					lclear = false;
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			}
    case WM_DESTROY:
		{
			Releas();

			//delete[] TestB.xar;
			//	delete[] TestB.yar;
			PostQuitMessage(0);
			DeleteObject(white);
			break;
		}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
void ResetVrs(HWND* hWnd)
{
	isholding = false;
	holdact = false;
	NG = true;
	nextp.SBL();
	TestB.SBL();
	TestB.xpos = 5;
	TestB.ypos = 2;
	score = 0;
	hold.color = 0;
	ResetVars();
	Ghost(TestB);
	TimmerID = SetTimer(*hWnd, 101, 1000, NULL);
	//InvalidateRect(hWnd, NULL, TRUE);
}
#pragma endregion