#pragma once

#include "SceneObject.h"
#include "Scene.h"
#include "Coordinates.h"
#include "ChessFigure.h"

#define BOARD_BACKGROUND_COLOR RGB(237, 237, 237)
#define BOARD_WHITE_COLOR RGB(255, 255, 255)
#define BOARD_BLACK_COLOR RGB(100, 100, 100)
#define BOARD_BORDER_OFFSET 20

struct BoardCell {
	BoardCell();
	BoardCell(Coordinates position, Gdiplus::RectF box);
	Coordinates Position;
	Gdiplus::RectF Box;
};


class CBoard: public ISceneObject {
public:
	enum MoveType {
		ImpossibleMove, Moved, CollapsedToAnotherPlace, FigureCancel, WhiteWins, BlackWins
	};

	CBoard(CScene* scene, Coordinates position, int width);

	Gdiplus::RectF GetBox(const Coordinates& position) const;

	bool SelectFigure(CChessFigure::FigureColor color, Coordinates position);
	MoveType MakeMove(CChessFigure::FigureColor color, Coordinates destination, bool quantum = false);

	void SetFigure(CChessFigure& figure);
	void SetKings(CChessFigure* blackKing, CChessFigure* whiteKing);

	// Inherited via ISceneObject
	void Render(HDC hdc) override;
	bool OnTimerTick() override;
	void FetchResources(CResourceManager & manager) override;
private:
	RECT boardRect;
	int startX;
	int startY;
	int squareWidth;
	int squareRest;
	CChessFigure* selectedFigure;
	CChessFigure* blackKing;
	CChessFigure* whiteKing;
	CChessFigure* figures[8][8] = { nullptr };
	CScene* scene;
	bool checkWhite;
	bool checkBlack;

	MoveType QuantMoveFigure(Coordinates destination);
	MoveType MoveFigure(Coordinates destination);
	MoveType SetFigureWithCollapse(Coordinates destination);
	//bool IsCheck(CChessFigure::FigureColor color, CChessFigure* figures[8][8]);
	//bool ValidateCheck(CChessFigure::FigureColor color, Coordinates from, Coordinates to);
	//bool IsMate(CChessFigure::FigureColor color);
	std::vector<Coordinates> GetMoves(Coordinates position);
	void GetPawnMoves(CChessFigure* figure, std::vector<Coordinates>& moves);
	void GetRookMoves(CChessFigure* figure, std::vector<Coordinates>& moves);
	void GetKnightMoves(CChessFigure* figure, std::vector<Coordinates>& moves);
	void GetBishopMoves(CChessFigure* figure, std::vector<Coordinates>& moves);
	void GetQueenMoves(CChessFigure* figure, std::vector<Coordinates>& moves);
	void GetKingMoves(CChessFigure* figure, std::vector<Coordinates>& moves);
	void AddIfNotCheck(CChessFigure::FigureColor color, Coordinates from, Coordinates to, std::vector<Coordinates>& moves);
	void Collapse(CChessFigure* figure);
	bool InBound(Coordinates coords);
};