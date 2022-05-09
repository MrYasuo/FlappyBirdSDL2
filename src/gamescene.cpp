#include "gamescene.h"
#include "component.h"
#include "defs.h"
#include "entity.h"
#include "renderer.h"
#include "system.h"
#include <iostream>

GameScene::GameScene(EventManager* eventManager) {
	this->eventManager = eventManager;

	entMan = new EntityManager;
	maxScore = 0;
	restartFlag = false;
	populateEntity(entMan);
}

void GameScene::DoFrame(Renderer* renderer) {
	for (auto entity : entMan->entities) {
		// background (layer 0)
		renderSpriteSystem(entity, renderer, 0);
		// branch and trunk (layer 1)
		renderSpriteSystem(entity, renderer, 1);
		// lumberjack (and stone) (layer 3)
		renderSpriteSystem(entity, renderer, 3);
	}

	// for (auto entity : entMan->entities) {
	// 	// status bar (the score bar)
	// 	HudSystem(entity, renderer);
	// }
}

void GameScene::Responder(Event* event, EventManager* eventManager) {
	if (event->type == MOUSE_BUTT) {
		for (auto entity : entMan->entities) {
			if (entity->clickable && entity->clickListener) {
				ClickableComponent* clickable = (ClickableComponent*)entity->clickable;
				ClickListenerComponent* clc = (ClickListenerComponent*)entity->clickListener;
				clickable->isClicked = false;
				if (clc->entity->sprite) {
					SpriteComponent* sprite = (SpriteComponent*)clc->entity->sprite;
					if (sprite->tName == TEX_LEFT) {
						eventManager->Post(new Event(KEYDOWN, "LEFT"));
					}
					if (sprite->tName == TEX_RIGHT) {
						eventManager->Post(new Event(KEYDOWN, "RIGHT"));
					}
				}
				break;
			}
		}
		return;
	}
	if (event->type == KEYDOWN && (!strcmp(event->data, "LEFT") || !strcmp(event->data, "RIGHT"))) {
		for (auto entity : entMan->entities) {
			if (entity->spawner) {
				SpawnerComponent* spawner = (SpawnerComponent*)entity->spawner;
				if (spawner->spawner_entities[0].second) {
					SpriteComponent* sprite = (SpriteComponent*)spawner->spawner_entities[0].second->sprite;
					if (!strcmp(event->data, "LEFT") && sprite->flip_flag == SDL_FLIP_HORIZONTAL)
						eventManager->Post(new Event(CHANGE_SCENE, "END_SCENE"));
					else if (!strcmp(event->data, "RIGHT") && sprite->flip_flag == SDL_FLIP_NONE)
						eventManager->Post(new Event(CHANGE_SCENE, "END_SCENE"));
					else
						eventManager->Post(new Event(SPAWN_BRANCH, " "));
				} else {
					eventManager->Post(new Event(SPAWN_BRANCH, " "));
				}
				break;
			}
		}
		return;
	}
}

void GameScene::populateEntity(EntityManager* entMan) {
	//! NOTICE that ALL position are self calculated
	//! BUT ALL size are shrinked a half to fit the scene

	// background
	Entity* ent1 = new Entity;
	ent1->position = new PositionComponent(0.0, 0.0);
	ent1->sprite = new SpriteComponent(TEX_BG, 1.0, 0);
	ent1->size = new SizeComponent(WIN_X, WIN_Y - 200);
	entMan->entities.push_back(ent1);

	// branch and trunk
	Entity* ent2 = new Entity;
	std::vector<SDL_RendererFlip> flip_flags;
	SpawnerComponent* spawner = new SpawnerComponent;
	// trunk
	for (int i = 1; i <= MAX_SPAWNER_COMPONENTS; i++) {
		Entity* spawner_entity1 = new Entity;
		spawner_entity1->position = new PositionComponent((WIN_X - 50) / 2, WIN_Y - 265 - Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS * i);
		spawner_entity1->cuttable = new CuttableComponent(0, Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS * (MAX_SPAWNER_COMPONENTS - i), Renderer::GetTexture(TEX_TRUNK)->w, Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS);
		spawner_entity1->size = new SizeComponent(50, Renderer::GetTexture(TEX_TRUNK)->h / MAX_SPAWNER_COMPONENTS);
		spawner_entity1->sprite = new SpriteComponent(TEX_TRUNK, 1.0, 2);

		spawner->spawner_entities.push_back(std::make_pair(spawner_entity1, nullptr));
	}
	// branch
	for (int i = 0; i < MAX_SPAWNER_COMPONENTS; i++) {
		if (i % 2 != 0) {
			Entity* spawner_entity2 = new Entity;
			SDL_RendererFlip flip_flag = getRandom(0, 1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
			if (flip_flag == SDL_FLIP_NONE)
				spawner_entity2->position = new PositionComponent(WIN_X / 2 + 10, WIN_Y - 340 - i * 80);
			else
				spawner_entity2->position = new PositionComponent(WIN_X / 2 - 135, WIN_Y - 340 - i * 80);
			spawner_entity2->sprite = new SpriteComponent(TEX_BRANCH, 1.0, 1, flip_flag);
			spawner_entity2->size = new SizeComponent(125, 80);

			spawner->spawner_entities[i].second = spawner_entity2;
		}
	}
	ent2->spawner = spawner;
	SpawnerListenerComponent* slc = new SpawnerListenerComponent(ent2);
	ent2->spawnerListener = slc;
	eventManager->AddListener(slc);
	entMan->entities.push_back(ent2);

	// the stone
	Entity* ent4 = new Entity;
	ent4->position = new PositionComponent((WIN_X - 75) / 2, WIN_Y - 275);
	ent4->sprite = new SpriteComponent(TEX_STONE, 1.0, 3);
	ent4->size = new SizeComponent(75, 36);
	entMan->entities.push_back(ent4);

	// lumberjack holding axe
	Entity* ent5 = new Entity;
	ent5->position = new PositionComponent((WIN_X - 50) / 2 + 65, WIN_Y - 350);
	ent5->sprite = new SpriteComponent(TEX_LUMBER_HOLDING, 1.0, 3);
	ent5->size = new SizeComponent(70, 107);
	ent5->animation = new AnimationComponent();
	ent5->movable = new MovableComponent();
	MoveListenerComponent* mcl = new MoveListenerComponent(ent5);
	ent5->moveListener = mcl;
	eventManager->AddListener(mcl);
	entMan->entities.push_back(ent5);

	// left button
	Entity* ent6 = new Entity;
	ent6->position = new PositionComponent((WIN_X - 120) / 5, WIN_Y - 160);
	ent6->sprite = new SpriteComponent(TEX_LEFT, 1.0, 0);
	ent6->size = new SizeComponent(120, 120);
	ent6->clickable = new ClickableComponent();
	ClickListenerComponent* clc1 = new ClickListenerComponent(ent6);
	eventManager->AddListener(clc1);
	ent6->clickListener = clc1;
	entMan->entities.push_back(ent6);

	// right button
	Entity* ent7 = new Entity;
	ent7->position = new PositionComponent((WIN_X - 120) / 5 * 4, WIN_Y - 160);
	ent7->sprite = new SpriteComponent(TEX_RIGHT, 1.0, 0);
	ent7->size = new SizeComponent(120, 120);
	ent7->clickable = new ClickableComponent();
	ClickListenerComponent* clc2 = new ClickListenerComponent(ent7);
	eventManager->AddListener(clc2);
	ent7->clickListener = clc2;
	entMan->entities.push_back(ent7);

	// Entity* ent4 = new Entity;
	// // score
	// ent4->position = new PositionComponent(40.0, 20.0);
	// ent4->size = new SizeComponent(0.0, 0.0);
	// ScoreComponent* scr = new ScoreComponent(maxScore);
	// ScoreListenerComponent* slc = new ScoreListenerComponent(scr);
	// eventManager->AddListener(slc);
	// ent4->score = scr;
	// ent4->scoreListener = slc;
	// entMan->entities.push_back(ent4);
}

// void GameScene::Restart(EventManager* eventManager) {
// 	restartFlag = false;
// 	for (auto entity : entMan->entities) {
// 		EventListener* e = (EventListener*)(entity->scoreListener);
// 		eventManager->RemoveListener(e - 1); // offset due to multiple inheritance
// 	}

// 	delete (entMan);
// 	entMan = new EntityManager;
// 	populateEntity(entMan);
// }

// void GameScene::DoPreRestart(EntityManager* entMan) {
// 	for (auto entity : entMan->entities) {
// 		// find flappy
// 		if (entity->flappyPhysics) {
// 			RotateComponent* angle = (RotateComponent*)entity->angle;
// 			FlappyPhysicsComponent* fpy = (FlappyPhysicsComponent*)entity->flappyPhysics;

// 			delete (entity->flappyPhysics);
// 			angle->angle = 90.0;
// 			angle->angleAcc = 0.0;
// 			fpy->yAcc = 15.0;
// 		}

// 		if (entity->pipe)
// 			delete (entity->pipe);
// 		if (entity->pipeSpawn)
// 			delete (entity->pipeSpawn);
// 		if (entity->anim)
// 			delete (entity->anim);
// 		if (entity->score) {
// 			ScoreComponent* scr = (ScoreComponent*)entity->score;
// 			if (scr->score > maxScore)
// 				maxScore = scr->score;
// 		}
// 	}
// }
