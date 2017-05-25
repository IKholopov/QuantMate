#include "TestController.h"

ControllerCommand CTestController::OnMouseClick(Coordinates coordinates)
{
	for( auto box = boundingRects.begin(); box != boundingRects.end(); ++box ) {
		if( coordinates.X >= box->Box.GetLeft() && coordinates.X < box->Box.GetRight() &&
				coordinates.Y >= box->Box.GetTop() && coordinates.Y < box->Box.GetBottom() ) {
			selectedPosition = box->Position;
			boxSelected = true;
		}
	}
	return ControllerCommand();
}

void CTestController::SetScene(CScene * scene)
{
	CGameController::SetScene(scene);
	CTestScene* testScene = static_cast<CTestScene*>(scene);
	board = static_cast<CBoard*>(testScene->GetBoard());
}

ControllerCommand CTestController::Start()
{
	boxSelected = false;
	state = ChooseFigure;
	color = CChessFigure::WHITE;
	GetBoardRects();
	return ControllerCommand();
}

void CTestController::GetBoardRects() {
	boundingRects.clear();
	for( int i = 0; i < 8; ++i ) {
		for( int j = 0; j < 8; ++j ) {
			this->boundingRects.push_back(BoardCell(Coordinates(i, j),
											board->GetBox(Coordinates(i, j))));
		}
	}
}

ControllerCommand CTestController::OnKey(long key)
{
	return ControllerCommand();
}

ControllerCommand CTestController::Update()
{
	auto command = ControllerCommand();
	if(boxSelected) {
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
				MessageBox(NULL, L"Check mate! Black win!", L"QuantMate", MB_OK);
				command.Command = COMMAND_EXIT;
				break;
			case CBoard::WhiteWins:
				MessageBox(NULL, L"Check mate! White win!", L"QuantMate", MB_OK);
				command.Command = COMMAND_EXIT;
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
				MessageBox(NULL, L"Check mate! Black win!", L"QuantMate", MB_OK);
				command.Command = COMMAND_EXIT;
				break;
			case CBoard::WhiteWins:
				MessageBox(NULL, L"Check mate! White win!", L"QuantMate", MB_OK);
				command.Command = COMMAND_EXIT;
				break;
			}
			break;
		}
	}
	boxSelected = false;
	return command;
}
