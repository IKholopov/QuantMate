#include "QuantMateController.h"
#include "QuantMateFactory.h"

ControllerCommand CQuantMateController::OnMouseClick(Coordinates coordinates)
{
	if( finished ) {
		ControllerCommand command;
		if( notification->IsExit(coordinates) ) {
			command.Command = COMMAND_EXIT;
		}
		else if( notification->IsNewGame(coordinates) ) {
			command.SceneId = SCENE_MAIN;
			command.ControllerId = CONTROLLER_MAIN;
			command.Command = COMMAND_REDIRECT;
		}
		return command;
	}
	if( paused ) {
		ControllerCommand command;
		if( menu->IsContinue(coordinates) ) {
			SwitchPause();
			command.Command = COMMAND_REDRAW;
		}
		else if( menu->IsNewGame(coordinates) ) {
			command.SceneId = SCENE_MAIN;
			command.ControllerId = CONTROLLER_MAIN;
			command.Command = COMMAND_REDIRECT;
		}
		return command;
	}
	for( auto box = boundingRects.begin(); box != boundingRects.end(); ++box ) {
		if( coordinates.X >= box->Box.GetLeft() && coordinates.X < box->Box.GetRight() &&
			coordinates.Y >= box->Box.GetTop() && coordinates.Y < box->Box.GetBottom() ) {
			selectedPosition = box->Position;
			boxSelected = true;
		}
	}
	return ControllerCommand();
}

ControllerCommand CQuantMateController::OnKey(long key)
{
	if( finished ) {
		return ControllerCommand();
	}
	if( key == VK_ESCAPE ) {
		SwitchPause();
	}
	ControllerCommand contrCommand;
	contrCommand.Command = COMMAND_REDRAW;
	return contrCommand;
}

void CQuantMateController::SetScene(CScene * scene)
{
	CGameController::SetScene(scene);
	CMainScene* mainScene = static_cast<CMainScene*>(scene);
	board = mainScene->GetBoard();
	menu = mainScene->GetPauseMenu();
	notification = mainScene->GetNotification();
}

ControllerCommand CQuantMateController::Start()
{
	paused = false;
	boxSelected = false;
	state = ChooseFigure;
	color = CChessFigure::WHITE;
	GetBoardRects();
	return ControllerCommand();
}

void CQuantMateController::SwitchPause()
{
	paused = !paused;
	if( paused ) {
		menu->Enable();
	}
	else {
		menu->Disable();
	}
}

void CQuantMateController::OnWin(CChessFigure::FigureColor color)
{
	finished = true;
	notification->Enable(color);
}

void CQuantMateController::GetBoardRects() {
	boundingRects.clear();
	for( int i = 0; i < 8; ++i ) {
		for( int j = 0; j < 8; ++j ) {
			this->boundingRects.push_back(BoardCell(Coordinates(i, j),
				board->GetBox(Coordinates(i, j))));
		}
	}
}

ControllerCommand CQuantMateController::Update()
{
	auto command = ControllerCommand();
	if( paused ) {
		return ControllerCommand();
	}
	if( boxSelected ) {
		switch( state ) {
		case ChooseFigure:
			if( board->SelectFigure(color, selectedPosition) ){
				state = FigureChosen;
				command.Command = COMMAND_REDRAW;
			}
			break;
		case FigureChosen:
			switch( board->MakeMove(color, selectedPosition) ) {
			case CBoard::Moved:
				state = ContinuingMove;
				command.Command = COMMAND_REDRAW;
				break;
			case CBoard::CollapsedToAnotherPlace:
				color = static_cast<CChessFigure::FigureColor>((color + 1) % 2);
				state = ChooseFigure;
				command.Command = COMMAND_REDRAW;
			case CBoard::FigureCancel:
				state = ChooseFigure;
				command.Command = COMMAND_REDRAW;
				break;
			case CBoard::BlackWins:
				OnWin(CChessFigure::BLACK);
				break;
			case CBoard::WhiteWins:
				OnWin(CChessFigure::WHITE);
				break;
			}
			break;
		case ContinuingMove:
			switch( board->MakeMove(color, selectedPosition, true) ) {
			case CBoard::Moved:
			case CBoard::CollapsedToAnotherPlace:
				color = static_cast<CChessFigure::FigureColor>((color + 1) % 2);
				state = ChooseFigure;
				command.Command = COMMAND_REDRAW;
				break;
			case CBoard::BlackWins:
				OnWin(CChessFigure::BLACK);
				break;
			case CBoard::WhiteWins:
				OnWin(CChessFigure::WHITE);
				break;
			}
			break;
		}
	}
	boxSelected = false;
	return command;
}
