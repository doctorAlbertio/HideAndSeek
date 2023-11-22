#pragma once
// Options struct for ingame Options.
static struct Options {
public:

	//Infos for the npc class;
	static int NpcFieldOfView;// = 90;
	static int NpcViewingRange;// = 2000;
	static int NpcDifficulty;// = 10;

	//Infos for the player class
	static int CountdownTime;// = 10;
	static int WinTime;// = 180;
	static bool realIsometricView;// = false;

	//friend class UMainUserWidget;

//private:
	//static void DefaultValues()

	static void DefaultValues()
	{
		NpcFieldOfView = int(90);
		NpcViewingRange = int(2000);
		NpcDifficulty = int(10);

		CountdownTime = int(10);
		WinTime = int(180);
		realIsometricView = bool(true);
	}

};

