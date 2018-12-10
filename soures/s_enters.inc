
/*
	Система входов
	Автор: knox
	Создан: 09.12.2018
*/

#define MAX_ENTERS 200

enum ENTER_INFO
{
	ETR_ID,
	ETR_NAME[40],
	ETR_ENTER_INTERIOR,
	ETR_ENTER_WORLD,
	Float:ETR_ENTER_X,
	Float:ETR_ENTER_Y,
	Float:ETR_ENTER_Z,
	Float:ETR_ENTER_A,
	ETR_ENTER_TIME,
	ETR_EXIT_INTERIOR,
	ETR_EXIT_WORLD,
	Float:ETR_EXIT_X,
	Float:ETR_EXIT_Y,
	Float:ETR_EXIT_Z,
	Float:ETR_EXIT_A,
	ETR_EXIT_TIME,
	STREAMER_TAG_PICKUP:ETR_ENTER_PICK,
	STREAMER_TAG_PICKUP:ETR_EXIT_PICK,
	STREAMER_TAG_AREA:ETR_ENTER_AREA,
	STREAMER_TAG_AREA:ETR_EXIT_AREA
}
new Enter[MAX_ENTERS][ENTER_INFO];

public: LoadEnters()
{
	new rows, fields;
	cache_get_data(rows, fields);
	new groupEnter, groupExit;
	groupEnter = CreateGroupGoodAreas(GoodArea:OnEtrEnter);
	groupExit  = CreateGroupGoodAreas(GoodArea:OnEtrExit);
	for(new i; i < rows; i++)
	{
		Enter[i][ETR_ID] = cache_get_field_content_int(i, "id");
		cache_get_field_content(i, "name", Enter[i][ETR_NAME]);
		Enter[i][ETR_ENTER_INTERIOR] = cache_get_field_content_int(i, "enter_interior");
		Enter[i][ETR_ENTER_WORLD] = cache_get_field_content_int(i, "enter_world");
		Enter[i][ETR_ENTER_X] = cache_get_field_content_int(i, "enter_x");
		Enter[i][ETR_ENTER_Y] = cache_get_field_content_int(i, "enter_y");
		Enter[i][ETR_ENTER_Z] = cache_get_field_content_int(i, "enter_z");
		Enter[i][ETR_ENTER_A] = cache_get_field_content_int(i, "enter_a");
		Enter[i][ETR_ENTER_TIME] = cache_get_field_content_int(i, "enter_time");

		Enter[i][ETR_EXIT_INTERIOR] = cache_get_field_content_int(i, "exit_interior");
		Enter[i][ETR_EXIT_WORLD] = cache_get_field_content_int(i, "exit_world");
		Enter[i][ETR_EXIT_X] = cache_get_field_content_int(i, "exit_x");
		Enter[i][ETR_EXIT_Y] = cache_get_field_content_int(i, "exit_y");
		Enter[i][ETR_EXIT_Z] = cache_get_field_content_int(i, "exit_z");
		Enter[i][ETR_EXIT_A] = cache_get_field_content_int(i, "exit_a");
		Enter[i][ETR_EXIT_TIME] = cache_get_field_content_int(i, "exit_time");

		Enter[i][ETR_ENTER_PICK] = CreateDynamicPickup(19133, 23,
			Enter[i][ETR_ENTER_X], Enter[i][ETR_ENTER_Y], Enter[i][ETR_ENTER_Z],
			Enter[i][ETR_ENTER_WORLD], Enter[i][ETR_ENTER_INTERIOR]
		);

		Enter[i][ETR_EXIT_PICK] = CreateDynamicPickup(19133, 23,
			Enter[i][ETR_EXIT_X], Enter[i][ETR_EXIT_Y], Enter[i][ETR_EXIT_Z],
			Enter[i][ETR_EXIT_WORLD], Enter[i][ETR_EXIT_INTERIOR]
		);

		Enter[i][ETR_ENTER_AREA] = CreateDynamicSphere(
			Enter[i][ETR_ENTER_X], Enter[i][ETR_ENTER_Y], Enter[i][ETR_ENTER_Z], 1.5, 
			Enter[i][ETR_ENTER_WORLD], Enter[i][ETR_ENTER_INTERIOR]
		);
		AddGroupGoodAreas(groupEnter, Enter[i][ETR_ENTER_AREA], i); 

		Enter[i][ETR_EXIT_AREA] = CreateDynamicSphere(
			Enter[i][ETR_EXIT_X], Enter[i][ETR_EXIT_Y], Enter[i][ETR_EXIT_Z], 1.5, 
			Enter[i][ETR_EXIT_WORLD], Enter[i][ETR_EXIT_INTERIOR]
		);
		AddGroupGoodAreas(groupExit, Enter[i][ETR_EXIT_AREA], i);
	}
	return 1;
}

GAResponse:OnEtrEnter(playerid, response, key, index) 
{
	if(response == GA_RESPONSE_ENTER) {
		if(GetPVarInt(playerid, "EtrLockUse") > gettime()) return 1;
		SetPlayerVirtualWorld(playerid, Enter[index][ETR_EXIT_WORLD]);
		SetPlayerInterior(playerid, Enter[index][ETR_EXIT_INTERIOR]);
		SetPlayerFacingAngle(playerid, Enter[index][ETR_EXIT_A]);
		SetPlayerPos(playerid, Enter[index][ETR_EXIT_X], Enter[index][ETR_EXIT_Y], Enter[index][ETR_EXIT_Z]);
		if(Enter[index][ETR_EXIT_TIME] != 0) {
			TogglePlayerControllable(playerid, false);
			SetTimerEx("UnfreezePlayer", Enter[index][ETR_EXIT_TIME] * 1000, 0, "d", playerid);
		}
		SetPVarInt(playerid, "EtrLockUse", gettime() + 2);
	}
	return 1;
}

GAResponse:OnEtrExit(playerid, response, key, index) 
{
	if(response == GA_RESPONSE_ENTER) {
		if(GetPVarInt(playerid, "EtrLockUse") > gettime()) return 1;
		SetPlayerVirtualWorld(playerid, Enter[index][ETR_ENTER_WORLD]);
		SetPlayerInterior(playerid, Enter[index][ETR_ENTER_INTERIOR]);
		SetPlayerFacingAngle(playerid, Enter[index][ETR_ENTER_A]);
		SetPlayerPos(playerid, Enter[index][ETR_ENTER_X], Enter[index][ETR_ENTER_Y], Enter[index][ETR_ENTER_Z]);
		if(Enter[index][ETR_ENTER_TIME] != 0) {
			TogglePlayerControllable(playerid, false);
			SetTimerEx("UnfreezePlayer", Enter[index][ETR_ENTER_TIME] * 1000, 0, "d", playerid);
		}
		SetPVarInt(playerid, "EtrLockUse", gettime() + 2);
	}
	return 1;
}
