#pragma once

#include <vector>
#include "SceneObject.h"
#include "QuantState.h"
#include "Sprite.h"
#include "Coordinates.h"
#include "QuantMateResources.h"
#include "QuantObject.h"

#define FIGURE_CHART_THICKNESS 5

class CChessFigure : public ISceneObject, public IQuantState {
public:
	enum Figure {
		PAWN, ROOK, BISHOP, KNIGHT, QUEEN, KING
	};
	enum FigureColor {
		WHITE, BLACK
	};
	CChessFigure(Figure figure, FigureColor color, Coordinates position, float probability=1.0f, float velocity=10.0f);
	CChessFigure(Figure figure, FigureColor color, Coordinates position, std::shared_ptr<CQuantObject> quantObject, float probability = 1.0f, float velocity = 10.0f);
	~CChessFigure();

	Coordinates GetBoardPosition();
	Coordinates GetOldBoardPosition();
	FigureColor GetColor();
	Figure GetFigure();
	CQuantObject* GetQuantObject();

	void SetBoardPosition(Coordinates position);
	void SetSpritePosition(Coordinates position);
	void SetSelected();
	void SetUnselected();
	void ResetProbability();

	// Inherited via ISceneObject
	void Render(HDC hdc) override;
	bool OnTimerTick() override;
	void FetchResources(CResourceManager & manager) override;

	// Inherited via IQuantState
	float GetProbability() override;
	IQuantState * Split() override;
private:
	Figure figure;
	FigureColor color;
	std::unique_ptr<CSprite> sprite;
	Coordinates boardPosition;
	Coordinates oldBoardPosition;
	Coordinates newSpritePosition;
	float probability;
	float scale;
	float velocity;
	std::shared_ptr<CQuantObject> quantObject;
};