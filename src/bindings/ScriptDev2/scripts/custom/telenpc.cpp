/////////////////////////////////////
//  TeleNPC v1.8.01
//  by Nicolai Dutka
//  http://nicolaidutka.archongames.com
//  Original code by Dythzar and Soco11
///////////////////////////////////////

#include "precompiled.h"
#include "../../../../shared/Config/Config.h"
#include "../../config.h"

bool GossipHello_TeleNPC(Player *player, Creature *_Creature)
{
	Config SD2Config;
	if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG,true))
		error_log("SD2: Unable to open configuration file");

	if(SD2Config.GetBoolDefault("TeleNPC.OnlyGMs", false)) // If TeleNPC.OnlyGMs is enabled in scriptdev2.conf
		if(player->GetSession()->GetSecurity() == SEC_PLAYER)
		{
			//_Creature->MonsterWhisper(player->GetGUID(), "Sorry, I am only allowed to teleport game masters.");
			return true;
		}

    player->SetTaxiCheater(true);

	bool EnableResSickness = SD2Config.GetBoolDefault("TeleNPC.EnableRemoveResSickness", true);
	bool EnableShoppingMall = SD2Config.GetBoolDefault("TeleNPC.EnableShoppingMall", false);

	// Main Menu for Alliance
    if ( player->GetTeam() == ALLIANCE )
	{
		player->ADD_GOSSIP_ITEM( 5, "Darnassus"                    , GOSSIP_SENDER_MAIN, 1000);
		player->ADD_GOSSIP_ITEM( 5, "Exodar"                       , GOSSIP_SENDER_MAIN, 1005);
		player->ADD_GOSSIP_ITEM( 5, "Ironforge"                    , GOSSIP_SENDER_MAIN, 1010);
		player->ADD_GOSSIP_ITEM( 5, "Stormwind"                    , GOSSIP_SENDER_MAIN, 1015);
		player->ADD_GOSSIP_ITEM( 5, "Shattrath City"               , GOSSIP_SENDER_MAIN, 3000);
		player->ADD_GOSSIP_ITEM( 5, "Dalaran"                      , GOSSIP_SENDER_MAIN, 3005);
		player->ADD_GOSSIP_ITEM( 5, "Gadegetzan"				   , GOSSIP_SENDER_MAIN, 3010);
		player->ADD_GOSSIP_ITEM( 7, "PVP ->"	    			   , GOSSIP_SENDER_MAIN, 4000);
		// player->ADD_GOSSIP_ITEM( 7, "Dungeons ->"				   , GOSSIP_SENDER_MAIN, 5000);
	}
	else // Main Menu for Horde
	{
		player->ADD_GOSSIP_ITEM( 5, "Orgrimmar"                    , GOSSIP_SENDER_MAIN, 2000);
		player->ADD_GOSSIP_ITEM( 5, "Silvermoon"                   , GOSSIP_SENDER_MAIN, 2005);
		player->ADD_GOSSIP_ITEM( 5, "Thunder Bluff"                , GOSSIP_SENDER_MAIN, 2010);
		player->ADD_GOSSIP_ITEM( 5, "Undercity"                    , GOSSIP_SENDER_MAIN, 2015);
		player->ADD_GOSSIP_ITEM( 5, "Shattrath City"               , GOSSIP_SENDER_MAIN, 3000);
		player->ADD_GOSSIP_ITEM( 5, "Dalaran"                      , GOSSIP_SENDER_MAIN, 3005);
		player->ADD_GOSSIP_ITEM( 5, "Gadegetzan"				   , GOSSIP_SENDER_MAIN, 3010);
		player->ADD_GOSSIP_ITEM( 7, "PVP ->"	    			   , GOSSIP_SENDER_MAIN, 4000);
		// player->ADD_GOSSIP_ITEM( 7, "Dungeons ->"				   , GOSSIP_SENDER_MAIN, 5000);
	}

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

return true;
}

void SendDefaultMenu_TeleNPC(Player *player, Creature *_Creature, uint32 action )
{

// Not allow in combat
if(!player->getAttackers().empty())
{
    player->CLOSE_GOSSIP_MENU();
    _Creature->MonsterWhisper("You are in combat!", LANG_UNIVERSAL, NULL);
	return;
}

Config SD2Config;
if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG,true))
		error_log("SD2: Unable to open configuration file");
bool EnableResSickness = SD2Config.GetBoolDefault("TeleNPC.EnableRemoveResSickness", true);

switch(action)
{

case 4000: //Dungeons
	player->ADD_GOSSIP_ITEM( 7, "Gurubashi Arena"			  , GOSSIP_SENDER_MAIN, 4010);
	player->ADD_GOSSIP_ITEM( 7, "Dire Maul Arena"	      	  , GOSSIP_SENDER_MAIN, 4015);
	player->ADD_GOSSIP_ITEM( 7, "<- Main Menu"			      , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
break;

case 5000: //Dungeons
	player->ADD_GOSSIP_ITEM( 7, "Kalimdor ->"				  , GOSSIP_SENDER_MAIN, 5010);
	player->ADD_GOSSIP_ITEM( 7, "Eastern Kingdoms ->"		  , GOSSIP_SENDER_MAIN, 5015);
	player->ADD_GOSSIP_ITEM( 7, "Outland ->"				  , GOSSIP_SENDER_MAIN, 5020);
	player->ADD_GOSSIP_ITEM( 7, "Northrend ->"				  , GOSSIP_SENDER_MAIN, 5025);
	player->ADD_GOSSIP_ITEM( 7, "<- Main Menu"			      , GOSSIP_SENDER_MAIN, 5005);

	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());
break;

case 5005: //Back To Main Menu
    // Main Menu for Alliance
    if ( player->GetTeam() == ALLIANCE )
	{
		player->ADD_GOSSIP_ITEM( 5, "Darnassus"                    , GOSSIP_SENDER_MAIN, 1000);
		player->ADD_GOSSIP_ITEM( 5, "Exodar"                       , GOSSIP_SENDER_MAIN, 1005);
		player->ADD_GOSSIP_ITEM( 5, "Ironforge"                    , GOSSIP_SENDER_MAIN, 1010);
		player->ADD_GOSSIP_ITEM( 5, "Stormwind"                    , GOSSIP_SENDER_MAIN, 1015);
		player->ADD_GOSSIP_ITEM( 5, "Shattrath City"               , GOSSIP_SENDER_MAIN, 3000);
		player->ADD_GOSSIP_ITEM( 5, "Dalaran"                      , GOSSIP_SENDER_MAIN, 3005);
		player->ADD_GOSSIP_ITEM( 5, "Gadegetzan"				   , GOSSIP_SENDER_MAIN, 3010);
		player->ADD_GOSSIP_ITEM( 7, "PVP ->"	    			   , GOSSIP_SENDER_MAIN, 4000);
		// player->ADD_GOSSIP_ITEM( 7, "Dungeons ->"				   , GOSSIP_SENDER_MAIN, 5000);
	}
	else // Main Menu for Horde
	{
		player->ADD_GOSSIP_ITEM( 5, "Orgrimmar"                    , GOSSIP_SENDER_MAIN, 2000);
		player->ADD_GOSSIP_ITEM( 5, "Silvermoon"                   , GOSSIP_SENDER_MAIN, 2005);
		player->ADD_GOSSIP_ITEM( 5, "Thunder Bluff"                , GOSSIP_SENDER_MAIN, 2010);
		player->ADD_GOSSIP_ITEM( 5, "Undercity"                    , GOSSIP_SENDER_MAIN, 2015);
		player->ADD_GOSSIP_ITEM( 5, "Shattrath City"               , GOSSIP_SENDER_MAIN, 3000);
		player->ADD_GOSSIP_ITEM( 5, "Dalaran"                      , GOSSIP_SENDER_MAIN, 3005);
		player->ADD_GOSSIP_ITEM( 5, "Gadegetzan"				   , GOSSIP_SENDER_MAIN, 3010);
		player->ADD_GOSSIP_ITEM( 7, "PVP ->"	    			   , GOSSIP_SENDER_MAIN, 4000);
		player->ADD_GOSSIP_ITEM( 7, "Dungeons ->"				   , GOSSIP_SENDER_MAIN, 5000);
	}


	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 5010: //Kalimdor
	player->ADD_GOSSIP_ITEM( 5, "Blackfathom Deeps"					, GOSSIP_SENDER_MAIN, 6000);
	player->ADD_GOSSIP_ITEM( 5, "Blackwing Lair"					, GOSSIP_SENDER_MAIN, 6005);
	player->ADD_GOSSIP_ITEM( 5, "Caverns of Time"				    , GOSSIP_SENDER_MAIN, 6010);
	player->ADD_GOSSIP_ITEM( 5, "Deadmines"							, GOSSIP_SENDER_MAIN, 6015);
	player->ADD_GOSSIP_ITEM( 5, "Dire Maul"							, GOSSIP_SENDER_MAIN, 6020);
	player->ADD_GOSSIP_ITEM( 5, "Maraudon"							, GOSSIP_SENDER_MAIN, 6025);
	player->ADD_GOSSIP_ITEM( 5, "Molten Core"						, GOSSIP_SENDER_MAIN, 6030);
	player->ADD_GOSSIP_ITEM( 5, "Onyxia's Lair"						, GOSSIP_SENDER_MAIN, 6035);
	// player->ADD_GOSSIP_ITEM( 5, "Ragefire Chasm"					, GOSSIP_SENDER_MAIN, 6040);
	player->ADD_GOSSIP_ITEM( 5, "Razorfen Downs"					, GOSSIP_SENDER_MAIN, 6045);
	player->ADD_GOSSIP_ITEM( 5, "Razorfen Kraul"					, GOSSIP_SENDER_MAIN, 6050);
	player->ADD_GOSSIP_ITEM( 5, "Ruins of Ahn'Qiraj"				, GOSSIP_SENDER_MAIN, 6055);
	player->ADD_GOSSIP_ITEM( 5, "Temple of Ahn'Qiraj"				, GOSSIP_SENDER_MAIN, 6060);
	player->ADD_GOSSIP_ITEM( 5, "Wailing Caverns"					, GOSSIP_SENDER_MAIN, 6065);
	player->ADD_GOSSIP_ITEM( 5, "Zul'Farrak"						, GOSSIP_SENDER_MAIN, 6070);
	player->ADD_GOSSIP_ITEM( 7, "<- Back"						    , GOSSIP_SENDER_MAIN, 5000);
	
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 5015: //Eastern Kingdoms

	player->ADD_GOSSIP_ITEM( 5, "Blackrock Depths"					, GOSSIP_SENDER_MAIN, 7000);
	player->ADD_GOSSIP_ITEM( 5, "Blackrock Spire"					, GOSSIP_SENDER_MAIN, 7005);
	player->ADD_GOSSIP_ITEM( 5, "Gnomeregan"						, GOSSIP_SENDER_MAIN, 7010);
	player->ADD_GOSSIP_ITEM( 5, "Isle Of Quel'Danas"				, GOSSIP_SENDER_MAIN, 7015);
	player->ADD_GOSSIP_ITEM( 5, "Karazhan"						    , GOSSIP_SENDER_MAIN, 7020);
	player->ADD_GOSSIP_ITEM( 5, "Scarlet Monastery"					, GOSSIP_SENDER_MAIN, 7025);
	player->ADD_GOSSIP_ITEM( 5, "Scholomance"						, GOSSIP_SENDER_MAIN, 7030);
	player->ADD_GOSSIP_ITEM( 5, "Shadowfang Keep"					, GOSSIP_SENDER_MAIN, 7035);
	player->ADD_GOSSIP_ITEM( 5, "Stratholme"						, GOSSIP_SENDER_MAIN, 7040);
	player->ADD_GOSSIP_ITEM( 5, "Sunken Temple"						, GOSSIP_SENDER_MAIN, 7045);
	// player->ADD_GOSSIP_ITEM( 5, "The Stockade"						, GOSSIP_SENDER_MAIN, 7050);
	player->ADD_GOSSIP_ITEM( 5, "Uldaman"							, GOSSIP_SENDER_MAIN, 7055);
	player->ADD_GOSSIP_ITEM( 5, "Zul'Aman"						    , GOSSIP_SENDER_MAIN, 7060);
	player->ADD_GOSSIP_ITEM( 5, "Zul'Gurub"							, GOSSIP_SENDER_MAIN, 7065);
	player->ADD_GOSSIP_ITEM( 7, "<- Back"						    , GOSSIP_SENDER_MAIN, 5000);
	
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 5020: //Outland
	
	player->ADD_GOSSIP_ITEM( 5, "Auchindoun"					    , GOSSIP_SENDER_MAIN, 8000);
	player->ADD_GOSSIP_ITEM( 5, "Black Temple"					    , GOSSIP_SENDER_MAIN, 8005);
	player->ADD_GOSSIP_ITEM( 5, "Coilfang Reservoir"			    , GOSSIP_SENDER_MAIN, 8010);
	player->ADD_GOSSIP_ITEM( 5, "Gruul's Lair"					    , GOSSIP_SENDER_MAIN, 8015);
	player->ADD_GOSSIP_ITEM( 5, "Hellfire Citadel"				    , GOSSIP_SENDER_MAIN, 8020);
	player->ADD_GOSSIP_ITEM( 5, "Tempest Keep"					    , GOSSIP_SENDER_MAIN, 8025);
	player->ADD_GOSSIP_ITEM( 7, "<- Back"							, GOSSIP_SENDER_MAIN, 5000);
	
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

case 5025: //Northrend
	
	player->ADD_GOSSIP_ITEM( 5, "Ahn'Kahet"						    , GOSSIP_SENDER_MAIN, 9000);
	player->ADD_GOSSIP_ITEM( 5, "Azjol-Nerub"					    , GOSSIP_SENDER_MAIN, 9005);
	player->ADD_GOSSIP_ITEM( 5, "Drak'Tharon Keep"				    , GOSSIP_SENDER_MAIN, 9010);
	player->ADD_GOSSIP_ITEM( 5, "Gun'Drak"						    , GOSSIP_SENDER_MAIN, 9015);
	player->ADD_GOSSIP_ITEM( 5, "Icecrown Citadel"				    , GOSSIP_SENDER_MAIN, 9020);
	player->ADD_GOSSIP_ITEM( 5, "Naxxramas"						    , GOSSIP_SENDER_MAIN, 9025);
	player->ADD_GOSSIP_ITEM( 5, "The Nexus"						    , GOSSIP_SENDER_MAIN, 9030);
	player->ADD_GOSSIP_ITEM( 5, "The Obsidian Sanctum"			    , GOSSIP_SENDER_MAIN, 9035);
	player->ADD_GOSSIP_ITEM( 5, "Utgarde Keep"					    , GOSSIP_SENDER_MAIN, 9040);
	player->ADD_GOSSIP_ITEM( 5, "Utgarde Pinnacle"				    , GOSSIP_SENDER_MAIN, 9045);
	player->ADD_GOSSIP_ITEM( 5, "Ulduar"						    , GOSSIP_SENDER_MAIN, 9050);
	player->ADD_GOSSIP_ITEM( 5, "Violet Hold"					    , GOSSIP_SENDER_MAIN, 9055);
	player->ADD_GOSSIP_ITEM( 7, "<- Back"						    , GOSSIP_SENDER_MAIN, 5000);
	
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,_Creature->GetGUID());

break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  ALLIANCE

case 1000: // Darnassus
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 9947.52f, 2482.73f, 1316.21f, 0.0f);
break;

case 1005: // Exodar
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, -3954.20f, -11656.54f, -138.69f, 0.0f);
break;

case 1010: // Ironforge
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -4924.07f, -951.95f, 501.55f, 5.40f);
break;

case 1015: // Stormwind
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -8960.14f, 516.266f, 96.3568f, 0.0f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  HORDE

case 2000: // Orgrimmar
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 1552.5f, -4420.66f, 8.94802f, 0.0f);
break;

case 2005: // Silvermoon
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 9338.74f, -7277.27f, 13.7895f, 0.0f);
break;

case 2010: // Thunder Bluff
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -1290.0f, 147.033997f, 129.682007f, 4.919000f);
break;

case 2015: // Undercity
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, 1819.71f, 238.79f, 60.5321f, 0.0f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  NEUTRAL

case 3000:// Shattrath City
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, -1850.209961f, 5435.821777f, -10.961435f, 3.403913f);
break;

case 3005:// Dalaran
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 5805.1098f, 639.1240f, 659.64f, 2.74f);
break;

case 3010: //Gadgetzan
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -7173.26f, -3785.60f, 8.37f,6.13f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  KALIMDOR

case 4010:// Gurubashi Arena
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -13248.087891f, 189.451904f, 31.176882f, 1.34f);
break;
  
case 4015:// Dire Maul Arena
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -3764.0563f, 1087.0646f, 131.97f, 1.52f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  KALIMDOR


case 6000:// Blackfathom Deeps
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 4248.72f, 744.35f, -24.67f, 1.34f);
break;
  
case 6005:// Blackwing Lair
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(469, -7672.61f, -1107.21f, 396.65f, 3.75f);
break;

case 6010:// Caverns of Time
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -8173.66f, -4746.36f, 33.84f, 4.94f);
break;

case 6015:// Deadmines
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -11208.2f, 1675.92f, 24.57f, 1.48f);
break;

case 6020:// Dire Maul
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -3960.95f, 1130.64f, 161.05f, 0.0f);
break;

case 6025:// Maraudon
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -1431.33f, 2962.34f, 98.23f, 4.74f);
break;

case 6030:// Molten Core
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(230, 1114.85f, -457.76f, -102.81f, 3.83f);
break;

case 6035:// Onyxia's Lair
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -4707.44f, -3726.82f, 54.6723f, 3.8f);
break;

case 6040:// Ragefire Chasm
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, 1814.47f, -4419.46f, -18.78f, 5.28f);
break;

case 6045:// Razorfen Downs
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -4657.88f, -2525.59f, 81.4f, 4.16f);
break;
    
case 6050:// Razorfen Kraul
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -4463.6f, -1664.53f, 82.26f, 0.85f);
break;

case 6055:// Ruins of Ahn'Qiraj
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -8413.33f, 1501.27f, 29.64f, 2.61f);
break;

case 6060:// Temple of Ahn'Qiraj
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -8245.837891f, 1983.736206f, 129.071686f, 0.936195f);
break;

case 6065:// Wailing Caverns
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -722.53f,-2226.30f,16.94f,2.71f);
break;

case 6070:// Zul'Farrak
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(1, -6801.9f, -2890.22f, 8.88f, 6.25f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  EASTERN KINGDOMS

case 7000:// Blackrock Depths
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -7180.57, -920.04f, 165.49f, 5.02f);
break;

case 7005:// Blackrock Spire
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -7526.77f, -1225.64f, 285.73f, 5.31f);
break;

case 7010:// Gnomeregan
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -5163.32f, 927.18f, 257.158, 1.44f);
break;
 
case 7015:// Isle Of Quel'Danas
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 12947.4f,-6893.31f,5.68398f,3.09154f);
break;

case 7020:// Karazhan
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -11119.6f, -2011.42f, 47.09f, 0.65f);
break;

case 7025:// Scarlet Monastery
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, 2843.89f,-693.74f,139.32f,5.11f);
break;

case 7030:// Scholomance
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, 1273.06f, -2574.01f, 92.66f, 2.06f);
break;

case 7035:// Shadowfang Keep
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -239.54f, 1550.8f, 76.89f, 1.18f);
break;

case 7040:// Stratholme
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, 3370.76f, -3343.63f, 142.26f, 5.23f);
break;

case 7045:// Sunken Temple
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -10346.92f, -3851.90f, -43.41f, 6.09f);
break;

case 7050:// The Stockade
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -8766.89f, 844.6f, 88.43f, 0.69f);
break;
 
case 7055:// Uldaman
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -6070.72f, -2955.33f, 209.78f, 0.05f);
break;

case 7060:// Zul'Aman
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 6851.09f, -7979.71f, 183.54f, 4.72f);
break;

case 7065:// Zul'Gurub
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(0, -11916.4f, -1216.23f, 92.28f, 4.75f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  OUTLAND

case 8000:// Auchindoun
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, -3322.92f, 4931.02f, -100.56f, 1.86f);
break;

case 8005:// Black Temple
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, -3649.1f, 317.33f, 35.19f, 2.97f);
break;

case 8010:// Coilfang Reservoir
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 721.08f, 6846.77f, -68.75f, 0.34f);
break;

case 8015:// Gruul's Lair
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 3539.01f, 5082.36f, 1.69f, 0.0f);
break;

case 8020:// Hellfire Citadel
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, -292.71f, 3146.77f, 31.60f, 2.05f);
break;

case 8025:// Tempest Keep
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(530, 3087.62f, 1376.27f, 184.8f, 4.63f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  NORTHREND

case 9000:// Ahn'Kahet
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 3646.76f, 2045.17f, 1.79f, 4.37f);
break;

case 9005:// Azjol-Nerub
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 3675.43f, 2169.00f, 35.90f, 2.29f);
break;

case 9010:// Drak'Tharon Keep
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 4450.86f, -2045.25f, 162.83f, 0.00f);
break;

case 9015:// Gun'Drak
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 6937.54f, -4455.98f, 450.68f, 1.00f);
break;

case 9020:// Icecrown Citadel
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 6139.24f, 2233.72f, 510.79f, 3.56f);
break;

case 9025:// Naxxramas
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 3125.184814f, -3748.024658f, 136.049393f, 0.0f);
break;

case 9030:// The Nexus
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 3789.95f, 6961.48f, 104.23f, 0.28f);
break;

case 9035:// The Obsidian Sanctum
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 3625.78f, 280.4f, -120.14f, 3.25f);
break;

case 9040:// Ulduar
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 1219.72f, -4865.28f, 41.25f, 0.31f);
break;

case 9045:// Utgarde Keep
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 1245.69f, -4856.59f, 216.86f, 3.45f);
break;

case 9050:// Utgarde Pinnacle
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 8976.24f, -1281.33f, 1059.01f, 0.58f);
break;

case 9055:// Violet Hold
	player->CLOSE_GOSSIP_MENU();
	player->TeleportTo(571, 5679.82f, 486.80f, 652.40f, 4.08f);
break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////  GUILD


} // end of switch
} //end of function



bool GossipSelect_TeleNPC(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
	// Main menu
	if (sender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_TeleNPC( player, _Creature, action );

return true;
}

void AddSC_telenpc()
{
    Script *newscript;
	newscript = new Script;
	newscript->Name="telenpc";
	newscript->pGossipHello = &GossipHello_TeleNPC;
	newscript->pGossipSelect = &GossipSelect_TeleNPC;
	newscript->pItemHello = NULL;
	newscript->pGOHello = NULL;
	newscript->pAreaTrigger = NULL;
	newscript->pItemQuestAccept = NULL;
	newscript->pGOQuestAccept = NULL;
	newscript->pGOChooseReward = NULL;
	newscript->RegisterSelf();
}
