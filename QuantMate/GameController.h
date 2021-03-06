#pragma once

#include "Scene.h"
#include "Coordinates.h"

#define GC_MOUSECLICK

#define COMMAND_IGNORE 0
#define COMMAND_REDRAW 1
#define COMMAND_EXIT 2	
#define COMMAND_REDIRECT 3

struct ControllerCommand {
	ControllerCommand();
	short Command;
	short SceneId;
	short ControllerId;
};

class CGameController {
public:
	virtual ~CGameController() {}
	virtual ControllerCommand OnMouseClick(Coordinates coordinates) = 0;
	virtual ControllerCommand OnKey(long key) = 0;
	virtual void SetScene(CScene* scene);
	virtual ControllerCommand Start() = 0;
	virtual ControllerCommand Update() = 0;
private:
	CScene* scene;
};