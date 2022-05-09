#pragma once
#include <SDL.h>
#include <cstring>
#include <map>
#include <queue>
#include <random>
#include <time.h>
#include <utility>
#include <vector>

#define WIN_X 600
#define WIN_Y 757
#define MAX_SPAWNER_COMPONENTS 8

enum move_tag_e {
	LEFT,
	RIGHT,
	NONE
};

enum component_tag_e {
	SPRITE,
	SCORE,
	SCORELISTENER,
	POSITION,
	SIZE,
	ANGLE,
	CLICKLISTENER,
	CLICKABLE,
	MOVABLE,
	MOVELISTENER,
	SPAWNER,
	SPAWNERLISTENER,
	CUTTABLE,
	ANIMATION
};

enum texture_e {
	TEX_BG,
	TEX_BRANCH,
	TEX_LOG,
	TEX_STONE,
	TEX_TRUNK,
	TEX_PLAY,
	TEX_REFRESH,
	TEX_LEFT,
	TEX_RIGHT,
	TEX_LUMBER_DEAD,
	TEX_LUMBER_HOLDING,
	TEX_LUMBER_CUTTING,
	TEX_TEXT_TITLE
};

enum scene_e {
	TITLE,
	GAME,
	END
};

enum event_type_e {
	QUIT,
	CHANGE_SCENE,
	KEYDOWN,
	KILL_ENT,
	GAME_RESTART,
	INC_SCORE,
	MOUSE_BUTT,
	SPAWN_BRANCH,
};

int getRandom(int a, int b);
void logSDLError(const char* msg, const char* err, ...);
std::string getTextureTag(texture_e tag);