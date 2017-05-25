#include "GameController.h"

void CGameController::SetScene(CScene * scene)
{
	this->scene = scene;
}

ControllerCommand::ControllerCommand()
{
	Command = COMMAND_IGNORE;
}
