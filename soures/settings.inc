
// - Библиотеки

#include "..\pawno\include\a_samp"
#include "..\pawno\include\crashdetect"
#include "..\pawno\include\streamer"
#include "..\pawno\include\mxINI"
#include "..\pawno\include\utils"
#include "..\pawno\include\foreach"
#include "..\pawno\include\a_mysql"
#include "..\pawno\include\mxdate"
#include "..\pawno\include\md5"
#include "..\pawno\include\inc"
#include "..\pawno\include\sscanf2"
#include "..\pawno\include\SprunkFactory"
#include "..\pawno\include\Pawn.CMD"
#include "..\pawno\include\GoodArea"

#define publics:%0(%1)		forward %0(%1); public %0(%1)
#define public:%0(%1)		forward %0(%1); public %0(%1)

#include "..\soures\s_enters"

native IsValidVehicle(vehicleid);

#define SCM			SendClientMessage

#pragma dynamic 10000