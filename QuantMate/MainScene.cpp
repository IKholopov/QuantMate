#include "MainScene.h"

CMainScene::CMainScene(CResourceManager& manager, int width, int height): CScene(width, height)
{
	std::vector<CChessFigure*> figures;
	notification = new CNotification(width, height);
	notification->FetchResources(manager);
	this->AddObject(notification, 5);
	menu = new CPauseMenu(width, height);
	menu->FetchResources(manager);
	this->AddObject(menu, 10);
	board = new CBoard(this, Coordinates(30, 20), width < height ? width - 80 : height - 80);
	board->FetchResources(manager);
	this->AddObject(board, -1);
	for( int i = 1; i < 8; i += 5 ) {
		for( int j = 0; j < 8; ++j ) {
			auto pawn = new CChessFigure(CChessFigure::PAWN, i == 1 ? CChessFigure::BLACK : CChessFigure::WHITE,
				Coordinates(j, i));
			figures.push_back(pawn);
		}
	}
	auto rookBlackLeft = new CChessFigure(CChessFigure::ROOK, CChessFigure::BLACK, Coordinates(0, 0));
	auto rookBlackRight = new CChessFigure(CChessFigure::ROOK, CChessFigure::BLACK, Coordinates(7, 0));
	auto rookWhiteLeft = new CChessFigure(CChessFigure::ROOK, CChessFigure::WHITE, Coordinates(0, 7));
	auto rookWhiteRight = new CChessFigure(CChessFigure::ROOK, CChessFigure::WHITE, Coordinates(7, 7));
	figures.push_back(rookBlackLeft);
	figures.push_back(rookBlackRight);
	figures.push_back(rookWhiteLeft);
	figures.push_back(rookWhiteRight);
	auto knightBlackLeft = new CChessFigure(CChessFigure::KNIGHT, CChessFigure::BLACK, Coordinates(1, 0));
	auto knightBlackRight = new CChessFigure(CChessFigure::KNIGHT, CChessFigure::BLACK, Coordinates(6, 0));
	auto knightWhiteLeft = new CChessFigure(CChessFigure::KNIGHT, CChessFigure::WHITE, Coordinates(1, 7));
	auto knightWhiteRight = new CChessFigure(CChessFigure::KNIGHT, CChessFigure::WHITE, Coordinates(6, 7));
	figures.push_back(knightBlackLeft);
	figures.push_back(knightBlackRight);
	figures.push_back(knightWhiteLeft);
	figures.push_back(knightWhiteRight);
	auto bishopBlackLeft = new CChessFigure(CChessFigure::BISHOP, CChessFigure::BLACK, Coordinates(2, 0));
	auto bishopBlackRight = new CChessFigure(CChessFigure::BISHOP, CChessFigure::BLACK, Coordinates(5, 0));
	auto bishopWhiteLeft = new CChessFigure(CChessFigure::BISHOP, CChessFigure::WHITE, Coordinates(2, 7));
	auto bishopWhiteRight = new CChessFigure(CChessFigure::BISHOP, CChessFigure::WHITE, Coordinates(5, 7));
	figures.push_back(bishopBlackLeft);
	figures.push_back(bishopBlackRight);
	figures.push_back(bishopWhiteLeft);
	figures.push_back(bishopWhiteRight);
	auto queenBlack = new CChessFigure(CChessFigure::QUEEN, CChessFigure::BLACK, Coordinates(3, 0));
	auto queenWhite = new CChessFigure(CChessFigure::QUEEN, CChessFigure::WHITE, Coordinates(3, 7));
	figures.push_back(queenBlack);
	figures.push_back(queenWhite);
	auto kingBlack = new CChessFigure(CChessFigure::KING, CChessFigure::BLACK, Coordinates(4, 0));
	auto kingWhite = new CChessFigure(CChessFigure::KING, CChessFigure::WHITE, Coordinates(4, 7));
	figures.push_back(kingBlack);
	figures.push_back(kingWhite);
	for( auto figure = figures.begin(); figure != figures.end(); ++figure ) {
		(*figure)->FetchResources(manager);
		this->AddObject(*figure, 1);
		board->SetFigure(**figure);
	}
}

CMainScene::~CMainScene()
{
}

CBoard* CMainScene::GetBoard()
{
	return board;
}

CPauseMenu* CMainScene::GetPauseMenu()
{
	return menu;
}

CNotification* CMainScene::GetNotification()
{
	return notification;
}

