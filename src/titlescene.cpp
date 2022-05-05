#include "titlescene.h"
#include "component.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

TitleScene::TitleScene(EventManager* eventManager) {
	entMan = new EntityManager;
	this->eventManager = eventManager;
	//! NOTICE that ALL position are self calculated
	//! BUT ALL size are shrinked a half to fit the scene

	// background
	Entity* ent1 = new Entity;
	ent1->position = new PositionComponent(0.0, 0.0);
	ent1->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	ent1->size = new SizeComponent(WIN_X, WIN_Y - 200);
	entMan->entities.push_back(ent1);

	// the log
	Entity* ent2 = new Entity;
	ent2->position = new PositionComponent((WIN_X - 50) / 2, WIN_Y - 307);
	ent2->sprite = new SpriteComponent(TEX_LOG, 1.0, 0);
	ent2->size = new SizeComponent(50, 50);
	entMan->entities.push_back(ent2);

	// the stone
	Entity* ent3 = new Entity;
	ent3->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent3->sprite = new SpriteComponent(TEX_STONE, 1.0, 0);
	ent3->size = new SizeComponent(75, 36);
	entMan->entities.push_back(ent3);

	// lumberjack holding axe
	Entity* ent4 = new Entity;
	ent4->position = new PositionComponent((WIN_X - 50) / 2 + 60, WIN_Y - 365);
	ent4->sprite = new SpriteComponent(TEX_LUMBER_HOLDING, 1.0, 0);
	ent4->size = new SizeComponent(70, 107);
	entMan->entities.push_back(ent4);

	// play button
	Entity* ent6 = new Entity;
	ent6->position = new PositionComponent((WIN_X - 120) / 2, WIN_Y - 160);
	ent6->sprite = new SpriteComponent(TEX_PLAY, 1.0, 0);
	ent6->size = new SizeComponent(120, 120);
	ent6->clickable = new ClickableComponent();
	ClickListenerComponent* clc = new ClickListenerComponent(ent6);
	eventManager->AddListener(clc);
	ent6->clickListener = clc;
	entMan->entities.push_back(ent6);

	// title game
	Entity* ent7 = new Entity;
	ent7->position = new PositionComponent((WIN_X - Renderer::GetTexture(TEX_TEXT_TITLE)->w) / 2, WIN_Y / 3);
	ent7->sprite = new SpriteComponent(TEX_TEXT_TITLE, 1.0, 0);
	entMan->entities.push_back(ent7);
}

TitleScene::~TitleScene() {
	delete (entMan);
}

void TitleScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		renderSpriteSystem(entity, renderer, 0);
	}
}

void TitleScene::Responder(Event* event, EventManager* eventManager) {
	if (event->type == MOUSE_BUTT) {
		for (auto entity : entMan->entities) {
			if (entity->clickable) {
				ClickableComponent* clickable = (ClickableComponent*)entity->clickable;
				if (clickable->isClicked) {
					clickable->isClicked = false;
					eventManager->Post(new Event(CHANGE_SCENE, "GAME_SCENE"));
				}
			}
		}
	}
	if (event->type == KEYDOWN && !strcmp(event->data, "SPACE")) {
		eventManager->Post(new Event(CHANGE_SCENE, "GAME_SCENE"));
	}
}
