/*

	Author: knox
	Version: 1.1
	Data: 21.11.2018

*/

#if defined _GoodArea_included
	#endinput
#endif

#define _GoodArea_included

#define GA_MAX_FUNCTION_NAME 32

#if !defined MAX_GOOD_AREAS
	#define MAX_GOOD_AREAS 5000
#endif

#if !defined GOOD_AREA_TAG
	#define GOOD_AREA_TAG STREAMER_TAG_AREA:
#endif

#define AreaResponse:%0(%1) \
	forward are_%0(%1); \
	public are_%0(%1)

#define GAResponse:%0(%1) \
	forward gare_%0(%1); \
	public gare_%0(%1)

#define GoodArea: #

#define GA_RESPONSE_ENTER 0
#define GA_RESPONSE_LEAVE 1
#define GA_RESPONSE_PRESS_KEY 3

#define IsGoodAreaUsed(%0) (ga_AreaFunction[%0][0] != '\0')

static ga_AreaFunction[MAX_GOOD_AREAS][GA_MAX_FUNCTION_NAME];

/*
	OnGameModeInit
*/

public OnGameModeInit()
{
	for(new i; i < MAX_GOOD_AREAS; i++)
	{
		ga_AreaFunction[i][0] = '\0';
	}

	#if defined GoodArea_OnGameModeInit
		return GoodArea_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit GoodArea_OnGameModeInit
#if defined GoodArea_OnGameModeInit
	forward GoodArea_OnGameModeInit();
#endif

/*
	OnPlayerEnterDynamicArea
*/

public OnPlayerEnterDynamicArea(playerid, GOOD_AREA_TAG areaid)
{
	new index = GetGoodAreaIndex(areaid);
	if(index != -1) {
		return CallLocalFunction(ga_AreaFunction[index], "ddd", playerid, GA_RESPONSE_ENTER, 0);
	}
	new ga_groupid, areaindex;
	if(GetGroupGoodAreaIndex(areaid, ga_groupid, areaindex)) {
		return CallLocalFunction(ga_AreaFunction[ga_groupid], "dddd", playerid, GA_RESPONSE_ENTER, 0, areaindex);
	}

	#if defined GA_OnPlayerEnterDynamicArea
		return GA_OnPlayerEnterDynamicArea(playerid, areaid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerEnterDynamicArea
	#undef OnPlayerEnterDynamicArea
#else
	#define _ALS_OnPlayerEnterDynamicArea
#endif

#define OnPlayerEnterDynamicArea GA_OnPlayerEnterDynamicArea
#if defined GA_OnPlayerEnterDynamicArea
	forward GA_OnPlayerEnterDynamicArea(playerid, areaid);
#endif

/*
	OnPlayerLeaveDynamicArea
*/

public OnPlayerLeaveDynamicArea(playerid, GOOD_AREA_TAG areaid)
{
	new index = GetGoodAreaIndex(areaid);
	if(index != -1) {
		return CallLocalFunction(ga_AreaFunction[index], "ddd", playerid, GA_RESPONSE_LEAVE, 0);
	}
	new ga_groupid, areaindex;
	if(GetGroupGoodAreaIndex(areaid, ga_groupid, areaindex)) {
		return CallLocalFunction(ga_AreaFunction[ga_groupid], "dddd", playerid, GA_RESPONSE_LEAVE, 0, areaindex);
	}

	#if defined GA_OnPlayerLeaveDynamicArea
		return GA_OnPlayerLeaveDynamicArea(playerid, areaid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerLeaveDynamicArea
	#undef OnPlayerLeaveDynamicArea
#else
	#define _ALS_OnPlayerLeaveDynamicArea
#endif

#define OnPlayerLeaveDynamicArea GA_OnPlayerLeaveDynamicArea
#if defined GA_OnPlayerLeaveDynamicArea
	forward GA_OnPlayerLeaveDynamicArea(playerid, areaid);
#endif

/*
	OnPlayerKeyStateChange
*/

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(oldkeys == 0) {
		new index = GetPlayerGoodArea(playerid);
		if(index != -1) {
			return CallLocalFunction(ga_AreaFunction[index], "ddd", playerid, GA_RESPONSE_PRESS_KEY, newkeys);
		}
		new ga_groupid, areaindex;
		if(GetPlayerGroupGoodArea(playerid, ga_groupid, areaindex)) {
			return CallLocalFunction(ga_AreaFunction[ga_groupid], "dddd", playerid, GA_RESPONSE_PRESS_KEY, newkeys, areaindex);
		}
	}

	#if defined GA_OnPlayerKeyStateChange
		return GA_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange GA_OnPlayerKeyStateChange
#if defined GA_OnPlayerKeyStateChange
	forward GA_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

/*
	DestroyDynamicAreaEx
*/

stock DestroyDynamicAreaEx(GOOD_AREA_TAG areaid)
{
	new index = GetGoodAreaIndex(areaid);
	if(index != -1) {
		ga_AreaFunction[index][0] = '\0';
	}
	return DestroyDynamicArea(GOOD_AREA_TAG areaid);
}
#if defined _ALS_DestroyDynamicArea
	#undef DestroyDynamicArea
#else
	#define _ALS_DestroyDynamicArea
#endif
#define DestroyDynamicArea DestroyDynamicAreaEx

/*
	Group areas
*/

stock CreateGroupGoodAreas(const function[])
{
	new index = GetFreeAreaFunction();
	if(index == -1) return -1;
	ga_AreaFunction[index] = !"gare_";
	strcat(ga_AreaFunction[index], function, sizeof(ga_AreaFunction[]));
	return index;
}

stock AddGroupGoodAreas(ga_groupid, GOOD_AREA_TAG area, areaindex)
{
	new ga_data[3];
	if(!IsValidDynamicArea(area)) return 0;
	ga_data[0] = 38;
	ga_data[1] = ga_groupid;
	ga_data[2] = areaindex;
	Streamer_SetArrayData(STREAMER_TYPE_AREA, area, E_STREAMER_EXTRA_ID, ga_data);
	return 1;
}

/*
	Individual area
*/

stock SetAreaFunction(GOOD_AREA_TAG area, const function[])
{
	new index = GetGoodAreaIndex(area);
	if(!IsValidDynamicArea(area)) return 0;
	if(index == -1) {
		index = GetFreeAreaFunction();
		if(index == -1) return 0;
	}
	ga_AreaFunction[index] = !"are_";
	strcat(ga_AreaFunction[index], function, sizeof(ga_AreaFunction[]));

	new ga_data[2];
	ga_data[0] = 37;
	ga_data[1] = index;
	Streamer_SetArrayData(STREAMER_TYPE_AREA, area, E_STREAMER_EXTRA_ID, ga_data);
	return 1;
}

static stock GetFreeAreaFunction()
{
	for(new i; i < MAX_GOOD_AREAS; i++)
	{
		if(ga_AreaFunction[i][0] == '\0') return i;
	}
	return -1;
}

static stock GetGoodAreaIndex(GOOD_AREA_TAG areaid)
{
	new data[2];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data);
	if(data[0] == 37) {
		if(IsGoodAreaUsed(data[1])) {
			return data[1];
		}
	}
	return -1;
}

static stock GetGroupGoodAreaIndex(GOOD_AREA_TAG areaid, &ga_groupid, &areaindex)
{
	new data[3];
	Streamer_GetArrayData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID, data);
	if(data[0] == 38) {
		if(IsGoodAreaUsed(data[1])) {
			ga_groupid = data[1];
			areaindex = data[2];
			return 1;
		}
	}
	return 0;
}

static stock GetPlayerGoodArea(playerid)
{
	new player_areas[10], index;
	GetPlayerDynamicAreas(playerid, player_areas);
	for(new i; i < sizeof player_areas; i++)
	{
		if(IsValidDynamicArea(player_areas[i])) {
			index = GetGoodAreaIndex(player_areas[i]);
			if(index != -1) return index;
		}
	}
	return -1;
}

static stock GetPlayerGroupGoodArea(playerid, &ga_groupid, &areaindex)
{
	new player_areas[10];
	GetPlayerDynamicAreas(playerid, player_areas);
	for(new i; i < sizeof player_areas; i++)
	{
		if(IsValidDynamicArea(player_areas[i])) {
			new temp_ga_groupid, temp_areaindex;
			if(GetGroupGoodAreaIndex(player_areas[i], temp_ga_groupid, temp_areaindex)) {
				ga_groupid = temp_ga_groupid;
				areaindex = temp_areaindex;
				return 1;
			}
		}
	}
	return 0;
}
