#include "ChessFigure.h"

#include <assert.h>
#include "BitmapResource.h"

CChessFigure::CChessFigure(Figure figure, FigureColor color, Coordinates position,
													float probability, float velocity): quantObject(new CQuantObject())
{
	this->figure = figure;
	this->color = color;
	assert(position.X >= 0 && position.X < 8);
	assert(position.Y >= 0 && position.Y < 8);
	this->boardPosition = position;
	this->probability = probability;
	this->velocity = velocity;
	this->scale = 1.0f;
	quantObject->AddState(this);
}

CChessFigure::CChessFigure(Figure figure, FigureColor color, Coordinates position,
													std::shared_ptr<CQuantObject> quantObject, float probability, float velocity): quantObject(quantObject)
{
	this->figure = figure;
	this->color = color;
	assert(position.X >= 0 && position.X < 8);
	assert(position.Y >= 0 && position.Y < 8);
	this->boardPosition = position;
	this->probability = probability;
	this->velocity = velocity;
	this->scale = 1.0f;
	this->quantObject->AddState(this);
}

CChessFigure::~CChessFigure()
{
	int a = 0;
}

void CChessFigure::SetBoardPosition(Coordinates position)
{
	oldBoardPosition = boardPosition;
	boardPosition = position;
}

void CChessFigure::SetSpritePosition(Coordinates position)
{
	newSpritePosition.X = position.X;
	newSpritePosition.Y = position.Y;
}

void CChessFigure::SetSelected()
{
	sprite->SetScale(1.2f);
}

void CChessFigure::SetUnselected()
{
	sprite->SetScale(1.0f);
}

void CChessFigure::ResetProbability()
{
	this->probability = 1.0f;
}

Coordinates CChessFigure::GetBoardPosition()
{
	return boardPosition;
}

Coordinates CChessFigure::GetOldBoardPosition()
{
	return oldBoardPosition;
}

CChessFigure::FigureColor CChessFigure::GetColor()
{
	return color;
}

CChessFigure::Figure CChessFigure::GetFigure()
{
	return figure;
}

CQuantObject* CChessFigure::GetQuantObject()
{
	return quantObject.get();
}

void CChessFigure::Render(HDC hdc)
{
	auto graphics = Gdiplus::Graphics::FromHDC(hdc);
	Gdiplus::RectF rect;
	sprite->GetBounds(rect);
	Gdiplus::GraphicsPath pathRed;
	Gdiplus::GraphicsPath pathBlue;
	pathBlue.AddArc(rect, -90, 360);
	pathRed.AddArc(rect, -90, 360 * probability);
	Gdiplus::Pen penRed(Gdiplus::Color::Red, FIGURE_CHART_THICKNESS);
	Gdiplus::Pen penBlue(Gdiplus::Color::Blue, FIGURE_CHART_THICKNESS);
	graphics->DrawPath(&penBlue, &pathBlue);
	graphics->DrawPath(&penRed, &pathRed);
	delete graphics;
	sprite->Draw(hdc);
}

bool CChessFigure::OnTimerTick()
{
	if( newSpritePosition.X != sprite->GetX() || newSpritePosition.Y != sprite->GetY() ){
		int deltaX = int((newSpritePosition.X - sprite->GetX()) / std::sqrt(
			std::pow(newSpritePosition.X - sprite->GetX(), 2) +
			std::pow(newSpritePosition.Y - sprite->GetY(), 2)) * velocity);
		int deltaY = int((newSpritePosition.Y - sprite->GetY()) / std::sqrt(
			std::pow(newSpritePosition.X - sprite->GetX(), 2) +
			std::pow(newSpritePosition.Y - sprite->GetY(), 2)) * velocity);
		bool useDelta = std::abs(deltaX) < std::abs(newSpritePosition.X - sprite->GetX()) || 
			std::abs(deltaY) < std::abs(newSpritePosition.Y - sprite->GetY());
		sprite->SetX(useDelta ? sprite->GetX() + deltaX : newSpritePosition.X);
		sprite->SetY(useDelta ? sprite->GetY() + deltaY : newSpritePosition.Y);
		return true;
	}
	return false;
}

void CChessFigure::FetchResources(CResourceManager & manager)
{
	IResource* resource;
	switch( this->figure ) {
	case PAWN:
		if( color == WHITE ) {
			resource = manager.GetResource(CQuantMateResources::PAWN_WHITE);
		} else {
			resource = manager.GetResource(CQuantMateResources::PAWN_BLACK);
		}
		break;
	case ROOK:
		if( color == WHITE ) {
			resource = manager.GetResource(CQuantMateResources::ROOK_WHITE);
		} else {
			resource = manager.GetResource(CQuantMateResources::ROOK_BLACK);
		}
		break;
	case BISHOP:
		if( color == WHITE ) {
			resource = manager.GetResource(CQuantMateResources::BISHOP_WHITE);
		} else {
			resource = manager.GetResource(CQuantMateResources::BISHOP_BLACK);
		}
		break;
	case KNIGHT:
		if( color == WHITE ) {
			resource = manager.GetResource(CQuantMateResources::KNIGHT_WHITE);
		} else {
			resource = manager.GetResource(CQuantMateResources::KNIGHT_BLACK);
		}
		break;
	case QUEEN:
		if( color == WHITE ) {
			resource = manager.GetResource(CQuantMateResources::QUEEN_WHITE);
		} else {
			resource = manager.GetResource(CQuantMateResources::QUEEN_BLACK);
		}
		break;
	case KING:
		if( color == WHITE ) {
			resource = manager.GetResource(CQuantMateResources::KING_WHITE);
		} else {
			resource = manager.GetResource(CQuantMateResources::KING_BLACK);
		}
		break;
	default:
		throw std::logic_error("Invalid figure\n");
		return;
	}
	sprite = std::unique_ptr<CSprite>(new CSprite(static_cast<CBitmapResource*>(resource)->GetBitmap()));	
}

float CChessFigure::GetProbability()
{
	return probability;
}

IQuantState* CChessFigure::Split()
{
	this->probability /= 2.0f;
	auto newState = new CChessFigure(this->figure, this->color, this->boardPosition, this->quantObject, this->probability, this->velocity);
	newState->sprite = std::unique_ptr<CSprite>(new CSprite(*(this->sprite)));
	return newState;
}

