#pragma once
class Tblock
{
public:
#pragma region Functions
#pragma region constructors
	Tblock(int bn, INT8 *xa, INT8 *ya);
	Tblock();
#pragma endregion
#pragma region Drawing related
	void DrawMe(HDC hdc, bool ghost);
	RECT SRct(int bln);
	int GSP(bool vert, bool hi, int blon);
#pragma endregion
#pragma region board related
	void Set(HWND hwind);
	void ClearL(HWND* hwind);
#pragma endregion
#pragma region other
	int GetRotatX(int posit);
	int GetRotatY(int posit);
	void SBL();
#pragma endregion
#pragma endregion
#pragma region Variables
#pragma region UINTs
	//blocknum is the number of tiles are in the piece
	UINT8 rotation = 0;
	UINT8 blocknum;
	UINT8 color;
#pragma endregion
#pragma region INTs
//xar and yar are poitners to arrays containing the x and y points for the tiles at rotation 0
// xpos and ypos are the positions with 1 step being 1 space on the board
	INT8 *xar;
	INT8 *yar;
	INT8 xpos = 23;
	INT8 ypos = 11;
#pragma endregion
#pragma endregion

};
//see the comments for functions, and variables on the cpp files
#pragma region Functions
#pragma region Drawing functions
void DrawBoard(HDC hdc);
void DrawNXT(HDC hdc, Tblock np, int xps, int yps, int psze);
void Bout(HDC hdc, INT8 *xp, INT8 *yp, int tiles);
#pragma endregion
#pragma region Can functions
bool CanDown(Tblock piece);
bool CanLeft(Tblock piece);
bool CanRight(Tblock piece);
bool CanRCCW(Tblock piece);
bool CanRCW(Tblock piece);
#pragma endregion
#pragma region rarly called
void StartUp();
void Releas();
void ResetVars();
void ResetVrs(HWND *hwnd);
#pragma endregion
#pragma region Tblock related
void Ghost(Tblock pce);
void SLN(Tblock *cur, Tblock *nex, HWND* hwnd, WPARAM* wp);
#pragma endregion
#pragma endregion
#pragma region Variables
#pragma region INTs
extern INT8 GPos;
extern INT8* expp;
extern INT8* eypp;
extern INT8 minx;
extern INT8 miny;
extern INT8 maxx;
extern INT8 maxy;
#pragma endregion
#pragma region UINTs
extern UINT8* lines;
extern UINT8 numoclns;
const extern UINT8 boardw;
const extern UINT8 boardh;
const extern UINT16 boardt;
extern UINT8 Pzs;
const extern UINT8 npas;
#pragma endregion
#pragma region normal number types
extern int ot;
extern unsigned long score;
#pragma endregion
#pragma region bools
extern bool renext;
extern bool reghost;
extern bool rehold;
extern bool recur;
extern bool reboard;
#pragma endregion
extern HBRUSH* pall;
#pragma endregion