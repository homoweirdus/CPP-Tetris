#include "stdafx.h"
#include "BEndToFEnd.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <time.h>
#pragma region macros
#define PSIZE 20
#define GHOSTP 1
#define TET_TSPAINT = 0x0450
#pragma endregion
using namespace std;
#pragma region Variables
#pragma region UINTs
//w and h stnad for width and heifght ant t is total
//Pzs = size of the tiles
//npas is number of next pieces shown
//pnums is how many tiles per piece
//i forget what numss does thats karma for not commenting earlier
//lines is a list of lines that need to be cleared for the animation thingy
//board: the number represents the color, 0 = no tile
//numoclns is for the number of lines cleard because dynamic memory alocation
// i forget what coln does as well
const UINT8 boardw = 10;
const UINT8 boardh = 22;
const UINT16 boardt = boardw*boardh;
UINT8 Pzs = 20;
const UINT8 npas = 7;
UINT8* pnums = NULL;
UINT8* numss = NULL;
UINT8* lines = NULL;
UINT8 numoclns = 0;
UINT8 board[boardt] = { 0 };
UINT8 coln;
#pragma endregion
#pragma region INTs
//xuh and yuh are the arrays that are pointed to when it cant get another piece for some reason
//xpoints and ypoints are pointers to dynamically allocated arrays of pointer that point to dynamically allocated signed byte arrays
//eypp and expp are used for drawing the ghost tile
//min is the lowest number in the piece max is the opposite
//Gpos is the y position of the ghost tile
INT8 xuh[5] = { 0, 1, 0, 0, 1 };
INT8 yuh[5] = { 0, 0, 1, -1, 1 };
INT8** xpoints = NULL;
INT8** ypoints = NULL;
INT8* expp = NULL;
INT8* eypp = NULL;
INT8 minx = 0;
INT8 miny = 0;
INT8 maxx = 0;
INT8 maxy = 0;
INT8 GPos = 0;
#pragma endregion
#pragma region normal number types
//ot and to are used for ghost piece drawing i think
//tsetcount has the number of pieces
int ot = 0;
int to = 0;
short tsetcount;
unsigned long score = 0;
#pragma endregion
#pragma region bools
//used for telling what to redraw
bool renext = true;
bool reghost = true;
bool rehold = true;
bool recur = true;
bool reboard = true;
#pragma endregion
//contains hbrushes for the colors of tiles
HBRUSH* pall = NULL;
#pragma endregion
#pragma region Tblock functions
#pragma region constructors
Tblock::Tblock(int bn, INT8 *xa, INT8 *ya)
{
	//initializing variables
	blocknum = bn;
	yar = ya;
	xar = xa;
	xpos = 5;
	ypos = 2;
}
Tblock::Tblock()
{
	blocknum = 5;
	xar = &(xuh[0]);
	yar = &(yuh[0]);
	color = 1;
	xpos = 5;
	ypos = 2;
}
#pragma endregion
#pragma region drawing related
//DrawMe draws the piece
//SRct is used to get the dimentions of a rectangle to draw for DrawMe
//GSP gets the poisition of a certain part of a tile
void Tblock::DrawMe(HDC hdc, bool ghost)
{
	RECT frct;
	if (ghost == false)
	{
		expp = new INT8[blocknum];
		eypp = new INT8[blocknum];
		ot = blocknum;
		to = GPos - ypos;
		for (int bl = 0; bl < blocknum; bl++)
		{
			frct = SRct(bl);
			expp[bl] = (frct.left / Pzs);
			eypp[bl] = (frct.top / Pzs);
			FillRect(hdc, &frct, pall[color]);
			frct.bottom -= 1;
			frct.right -= 1;
			frct.top += 1;
			frct.left += 1;
			FrameRect(hdc, &frct, pall[0]);
		}

	}
	else
	{
		for (int bl = 0; bl < blocknum; bl++)
		{
			frct = SRct(bl);
			FrameRect(hdc, &frct, pall[color]);
			frct.bottom -= 3;
			frct.right -= 3;
			frct.top += 3;
			frct.left += 3;
			FrameRect(hdc, &frct, pall[color]);
		}
	}
}
RECT Tblock::SRct(int bln)
{
	RECT Trect;
	SetRect(&Trect, GSP(false, true, bln), GSP(true, true, bln), GSP(false, false, bln), GSP(true, false, bln));
	return Trect;
}
int Tblock::GSP(bool vert, bool hi, int blon)
{
	//switch statement because doing it manually is slowwer
	if (vert == true && hi == true)
	{
		switch (rotation)
		{
			case 0:
				{
					return (yar[blon] + ypos) * Pzs;
				}
			case 1:
				{
					return (-xar[blon] + ypos) * Pzs;
				}
			case 2:
				{
					return (-yar[blon] + ypos) * Pzs;
				}
			case 3:
				{
					return (xar[blon] + ypos) * Pzs;
				}
			default:
				{
					return 0;
				}
		}
	}
	if (vert == true && hi == false)
	{
		switch (rotation)
		{
			case 0:
				{
					return (yar[blon] + ypos + 1) * Pzs;
				}
			case 1:
				{
					return (-xar[blon] + ypos + 1) * Pzs;
				}
			case 2:
				{
					return (-yar[blon] + ypos + 1) * Pzs;
				}
			case 3:
				{
					return (xar[blon] + ypos + 1) * Pzs;
				}
			default:
				{
					return 0;
				}
		}
	}
	if (vert == false && hi == true)
	{
		switch (rotation)
		{
			case 0:
				{
					return (xar[blon] + xpos) * Pzs;
				}
			case 1:
				{
					return (yar[blon] + xpos) * Pzs;
				}
			case 2:
				{
					return (-xar[blon] + xpos) * Pzs;
				}
			case 3:
				{
					return (-yar[blon] + xpos) * Pzs;
				}
			default:
				{
					return 0;
				}
		}

	}
	if (vert == false && hi == false)
	{
		switch (rotation)
		{
			case 0:
				{
					return (xar[blon] + xpos + 1) * Pzs;
				}
			case 1:
				{
					return (yar[blon] + xpos + 1) * Pzs;
				}
			case 2:
				{
					return (-xar[blon] + xpos + 1) * Pzs;
				}
			case 3:
				{
					return (-yar[blon] + xpos + 1) * Pzs;
				}
			default:
				{
					return 0;
				}
		}

	}
	return 0;
}
#pragma endregion
#pragma region board related
//set updates the board
// clearl checks to see if there is lines to clear then clears them
void Tblock::Set(HWND hwind)
{
	int ston;
	for (int pie = 0; pie < blocknum; pie++)
	{
		ston = (GetRotatY(pie)) * boardw;
		ston += GetRotatX(pie);
		if (ston < boardt)
		{
			board[ston] = color;
		}
	}
	reboard = true;
	ClearL(&hwind);
}
void Tblock::ClearL(HWND* hwind)
{
	int tmmp = 1;
	vector<UINT8> numss;
	int cpnum = 0;
	UINT8 tbool;
	int lnum;
	for (int pnum = 0; pnum < blocknum; pnum++)
	{
		lnum = GetRotatY(pnum);
		for (int xnum = 0; xnum < boardw; xnum++)
		{
			tbool = board[(lnum * boardw) + xnum];
			if (tbool == 0)
			{
				break;
			}
		}
		if (tbool > 0)
		{
			//memmove(board + 10, board, 10 * lnum);
			numss.push_back(lnum);
			cpnum++;
		}

	}
	sort(numss.begin(), numss.end());
	unique(numss.begin(), numss.end());
	set<UINT8> sst(numss.begin(), numss.end());
	numoclns = sst.size();
	if (numoclns > 0)
	{
		lines = new UINT8[numoclns];
		for (std::set<UINT8>::iterator it = sst.begin(); it != sst.end(); ++it)
		{
			memmove(board + boardw, board, boardw * (*it));
			score += ((boardw * 10) * tmmp);
			lines[tmmp - 1] = (*it);
			tmmp++;
		}
	}
	numss.~vector();
	//sst.~set();
	if (numoclns > 0)
	{
		SendMessage(*hwind, 0x0453, 0, 0);
	}
	else
	{
		InvalidateRect(*hwind, NULL, TRUE);
	}

}
#pragma endregion
#pragma region other
//GetRotatX and Y get the x and y positions with rotation
// SBL randomly generates a tile for a piece to be
int Tblock::GetRotatX(int posit)
{
	switch (rotation)
	{
		case 0:
			{
				return xpos + xar[posit];
				break;
			}
		case 1:
			{
				return  xpos + yar[posit];
				break;
			}
		case 2:
			{
				return xpos + -xar[posit];
				break;
			}
		case 3:
			{
				return xpos + -yar[posit];
				break;
			}
		default:
			return 0;
			break;
	}
}
int Tblock::GetRotatY(int posit)
{
	switch (rotation)
	{
		case 0:
			{
				return ypos + yar[posit];
				break;
			}
		case 1:
			{
				return ypos - xar[posit];
				break;
			}
		case 2:
			{
				return ypos + -yar[posit];
				break;
			}
		case 3:
			{
				return ypos + xar[posit];
				break;
			}
		default:
			return 0;
			break;
	}
}
void Tblock::SBL()
{
	int rnd = rand() % tsetcount;
	xar = (xpoints[rnd]);
	yar = (ypoints[rnd]);
	blocknum = pnums[rnd];
	color = (rnd % (coln - 1)) + 1;
	rotation = 0;
	//xpos = 5;
	//ypos = 2;
	//Ghost(*this);
}
#pragma endregion
#pragma endregion
#pragma region other functions
#pragma region Can functions
bool CanDown(Tblock piece)
{
	int stornum;
	for (int pie = 0; pie < piece.blocknum; pie++)
	{
		stornum = (piece.GetRotatY(pie) + 1)*boardw;
		stornum += piece.GetRotatX(pie);
		if (stornum >= boardt || board[stornum] > 0)
		{
			return false;
		}
	}
	return true;
}
bool CanLeft(Tblock piece)
{
	int stornum;
	int grx;
	for (int pie = 0; pie < piece.blocknum; pie++)
	{
		grx = piece.GetRotatX(pie);

		stornum = (piece.GetRotatY(pie)) * boardw;
		stornum += (grx - 1);
		if (grx == 0 || board[stornum] > 0)
		{
			return false;
		}
	}
	return true;
}
bool CanRCCW(Tblock piece)
{
	if (piece.rotation == 3)
	{
		piece.rotation = 0;
	}
	else
	{
		piece.rotation++;
	}
	int stornum;
	int grx;
	for (int pie = 0; pie < piece.blocknum; pie++)
	{
		grx = piece.GetRotatX(pie);

		stornum = (piece.GetRotatY(pie)) * boardw;
		stornum += (grx);
		if (grx < 0 || grx > boardw - 1 || stornum > boardt - 1 || board[stornum] > 0)
		{
			return false;
		}
	}
	return true;
}
bool CanRCW(Tblock piece)
{
	if (piece.rotation == 0)
	{
		piece.rotation = 3;
	}
	else
	{
		piece.rotation--;
	}
	int stornum;
	int grx;
	for (int pie = 0; pie < piece.blocknum; pie++)
	{
		grx = piece.GetRotatX(pie);

		stornum = (piece.GetRotatY(pie)) * boardw;
		stornum += (grx);
		if (grx < 0 || grx > boardw - 1 || stornum > boardt - 1 || board[stornum] > 0)
		{
			return false;
		}
	}
	return true;
}
bool CanRight(Tblock piece)
{
	int stornum;
	int grx;
	for (int pie = 0; pie < piece.blocknum; pie++)
	{
		grx = piece.GetRotatX(pie);

		stornum = (piece.GetRotatY(pie)) * boardw;
		stornum += (grx + 1);
		if (grx == boardw - 1 || board[stornum] > 0)
		{
			return false;
		}
	}
	return true;
}
#pragma endregion
#pragma region Draw functions
void DrawBoard(HDC hdc)
{
	RECT currct;
	UINT8 tmp;
	for (int ycur = 0; ycur < boardh; ycur++)
	{
		for (int xcur = 0; xcur < boardw; xcur++)
		{
			currct.left = xcur * Pzs;
			currct.right = currct.left + Pzs;
			currct.top = ycur * Pzs;
			currct.bottom = currct.top + Pzs;
			/*if (board[(ycur * 10) + xcur] == true)
			{
				FillRect(hdc, &currct, tbru);
			}
			else
			{
				FillRect(hdc, &currct, emptb);
			}*/
			tmp = board[(ycur * boardw) + xcur];
			FillRect(hdc, &currct, pall[tmp]);

			if (tmp != 0)
			{
				currct.bottom--;
				currct.top++;
				currct.left++;
				currct.right--;
				FrameRect(hdc, &currct, pall[0]);
			}
		}
	}
}
void DrawNXT(HDC hdc, Tblock np, int xps, int yps, int psze)
{
	RECT rct;
	SetRect(&rct, xps, yps, (xps + (maxx - minx + 1)*psze), (yps + (maxy - miny + 1)*psze));
	FillRect(hdc, &rct, pall[0]);
	xps -= (minx*psze);
	yps -= (miny*psze);
	for (int pece = 0; pece < np.blocknum; pece++)
	{
		SetRect(&rct, xps + (np.xar[pece] * psze), yps + (np.yar[pece] * psze), xps + ((np.xar[pece] + 1) * psze), yps + ((np.yar[pece] + 1) * psze));
		//SetRect(&rct, xps, yps, xps + pece, yps + pece);
		FillRect(hdc, &rct, pall[np.color]);
	}
}
void Bout(HDC hdc, INT8 *xp, INT8 *yp, int tiles)
{
	RECT crt;
	for (int tle = 0; tle < tiles; tle++)
	{
		SetRect(&crt, xp[tle] * Pzs, yp[tle] * Pzs, (xp[tle] + 1) * Pzs, (yp[tle] + 1) * Pzs);
		FillRect(hdc, &crt, pall[0]);
		if (reghost == true)
		{
			crt.top += (to*Pzs);
			crt.bottom += (to*Pzs);
			FillRect(hdc, &crt, pall[0]);
		}
	}
	delete[] expp;
	delete[] eypp;
}
#pragma endregion
void SLN(Tblock *cur, Tblock *nex, HWND* hwnd, WPARAM* wp)
{
	int np = npas - 1;
	int rnd = rand() % tsetcount;
	(*cur).xar = (*nex).xar;
	(*cur).yar = (*nex).yar;
	(*cur).blocknum = (*nex).blocknum;
	(*cur).color = (*nex).color;
	(*cur).rotation = 0;
	(*cur).xpos = 5;
	(*cur).ypos = 2;
	for (int nc = 0; nc < npas; ++nc)
	{
		nex[nc].xar = nex[nc + 1].xar;
		nex[nc].yar = nex[nc + 1].yar;
		nex[nc].blocknum = nex[nc + 1].blocknum;
		nex[nc].color = nex[nc + 1].color;
	}
	nex[np].xar = (xpoints[rnd]);
	nex[np].yar = (ypoints[rnd]);
	nex[np].blocknum = pnums[rnd];
	nex[np].color = (rnd % (coln - 1)) + 1;
	Ghost((*cur));

	if (CanDown(*cur) == false)
	{
		SendMessage(*hwnd, 0x0451, *wp, 0);
	}
	renext = true;
}
void StartUp()
{
	UINT8 cr;
	UINT8 cg;
	UINT8 cb;
	srand(time(NULL));
	streampos size;
	std::ifstream file("Piece.tet", ios::in | ios::binary);
	if (file.is_open())
	{
		file.seekg(0, ios::beg);
		file.read((char*)&tsetcount, sizeof(short));
		pnums = new UINT8[tsetcount];
		xpoints = new INT8*[tsetcount];
		ypoints = new INT8*[tsetcount];
		//file.seekg(2, ios::cur);
		file.read((char*)pnums, sizeof(UINT8)*tsetcount);
	//	file.seekg(tsetcount, ios::cur);
		for (int curp = 0; curp < tsetcount; curp++)
		{
			(xpoints[curp]) = new INT8[(pnums[curp])];
			(ypoints[curp]) = new INT8[(pnums[curp])];
			file.read((char*)(xpoints[curp]), pnums[curp]);
			//file.seekg(pnums[curp], ios::cur);
			file.read((char*)(ypoints[curp]), pnums[curp]);
			/*if(curp != tsetcount)
			file.seekg(pnums[curp], ios::cur);*/
		}
		file.read((char*)&minx, 1);
		file.read((char*)&maxx, 1);
		file.read((char*)&miny, 1);
		file.read((char*)&maxy, 1);
		file.close();
	}
	file.open("Color.tet", ios::in | ios::binary);
	if (file.is_open())
	{
		file.read((char*)&coln, 1);
		pall = new HBRUSH[coln];
		for (int cc = 0; cc < coln; cc++)
		{
			file.read((char*)&cr, 1);
			file.read((char*)&cg, 1);
			file.read((char*)&cb, 1);
			pall[cc] = CreateSolidBrush(RGB(cr, cg, cb));
		}
		file.close();
	}
}
void Releas()
{
	for (int drp = 0; drp < tsetcount; drp++)
	{
		delete[] xpoints[drp];
		delete[] ypoints[drp];
	}
	for (int drp = 0; drp < coln; drp++)
	{
		DeleteObject(pall[drp]);
	}
	delete[] xpoints;
	delete[] ypoints;
	delete[] pnums;
	delete[] pall;
}
void Ghost(Tblock pce)
{
	INT8 temp = pce.ypos;
	while (CanDown(pce))
	{
		pce.ypos++;
	}
	GPos = pce.ypos;
	pce.ypos = temp;
	reghost = true;

}
void ResetVars()
{
	for (int bn = 0; bn < boardt; bn++)
	{
		board[bn] = 0;
	}
	renext = true;
	reboard = true;
	reghost = true;
	rehold = true;
	recur = true;
	score = 0;

}
#pragma endregion