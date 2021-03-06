#pragma once
#include "defs.h"
#include "event.h"

struct Component {
	component_tag_e tag;
};

struct PositionComponent : Component {
	PositionComponent(double x, double y);

	double x;
	double y;
};

struct SpriteComponent : Component {
	SpriteComponent(texture_e tName, double scale, int layer);

	texture_e tName;
	double scale;
	int layer;
};

struct RotateComponent : Component {
	RotateComponent(double angle, double angleAcc);

	double angle;
	double angleAcc;
};

struct SizeComponent : Component {
	SizeComponent(double w, double h);

	double w;
	double h;
};

struct FlappyPhysicsComponent : Component {
	FlappyPhysicsComponent();

	double grav;
	double yAcc;
};

struct FlappyInputComponent : Component {
	FlappyInputComponent(EventManager* eventManager);

	EventManager* eventManager;
	double lift;
};

struct SpriteSpanComponent : Component {
	SpriteSpanComponent(int repeat);

	int repeat;
};

struct AnimComponent : Component {
	AnimComponent(int fCount, double decay, int w, int h, int offset);

	int fCount;
	int currFrame;
	double value;
	double decay;
	int w;
	int h;
	int offset;
};

struct PipeSpawnerComponent : Component {
	PipeSpawnerComponent(double decay);

	double value;
	double decay;
};

struct PipeComponent : Component {
	PipeComponent(double offset, double xAcc);

	double offset;
	double xAcc;
	bool hasScore;
};

struct PipeSpriteComponent : Component {
	PipeSpriteComponent(texture_e tName);

	texture_e tName;
};

struct CollidableComponent : Component {
	CollidableComponent();
};

struct ScoreComponent : Component {
	ScoreComponent(int maxScore);

	int score;
	int maxScore;
};
struct ScoreListenerComponent : Component, EventListener {
	ScoreListenerComponent(ScoreComponent* scr);
	void Responder(Event* event);

	ScoreComponent* scr;
};

struct ListenerComponent : Component, EventListener {
	ListenerComponent();
	void Responder(Event* event);
};