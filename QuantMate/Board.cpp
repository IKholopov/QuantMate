#include "Board.h"

CBoard::CBoard(CScene* scene, Coordinates position, int width) {
	boardRect.left = position.X;
	boardRect.top = position.Y;
	boardRect.right = position.X + width;
	boardRect.bottom = position.Y + width;
	startX = boardRect.left + BOARD_BORDER_OFFSET;
	startY = boardRect.top + BOARD_BORDER_OFFSET;
	squareWidth = (boardRect.right - boardRect.left - 2 * BOARD_BORDER_OFFSET) / 8;
	squareRest = (boardRect.right - boardRect.left - 2 * BOARD_BORDER_OFFSET) % 8;
	this->scene = scene;
	checkWhite = false;
	checkBlack = false;
}

Gdiplus::RectF CBoard::GetBox(const Coordinates& position) const
{
	Gdiplus::RectF rect;
	rect.X = startX + squareWidth * position.X + (squareRest >= position.X ? position.X : squareRest);
	rect.Width = startX + squareWidth * (position.X + 1) + (squareRest > position.X ? position.X + 1 : squareRest) - rect.X;
	rect.Y = startY + squareWidth * position.Y + (squareRest >= position.Y ? position.Y : squareRest);
	rect.Height = startY + squareWidth * (position.Y + 1) + (squareRest > position.Y ? position.Y + 1 : squareRest) - rect.Y;
	return rect;
}

bool CBoard::SelectFigure(CChessFigure::FigureColor color, Coordinates position)
{
	auto figure = figures[position.X][position.Y];
	if( figure && figure->GetColor() == color) {
		figure->SetSelected();
		selectedFigure = figure;
		return true;
	}
	return false;
}

CBoard::MoveType CBoard::MakeMove(CChessFigure::FigureColor color, Coordinates destination, bool quantum)
{
	if( !selectedFigure ) {
		throw std::logic_error("MakeMove is called without selected figure");
	}
	auto position = selectedFigure->GetBoardPosition();
	if( !quantum && position.X == destination.X && position.Y == destination.Y ) {
		selectedFigure->SetUnselected();
		return FigureCancel;
	}
	std::vector<Coordinates> moves;
	switch( selectedFigure->GetFigure() ) {
	case CChessFigure::PAWN:
		GetPawnMoves(selectedFigure, moves);
		break;
	case CChessFigure::ROOK:
		GetRookMoves(selectedFigure, moves);
		break;
	case CChessFigure::KNIGHT:
		GetKnightMoves(selectedFigure, moves);
		break;
	case CChessFigure::BISHOP:
		GetBishopMoves(selectedFigure, moves);
		break;
	case CChessFigure::QUEEN:
		GetQueenMoves(selectedFigure, moves);
		break;
	case CChessFigure::KING:
		GetKingMoves(selectedFigure, moves);
		break;
	}
	if( quantum ) {
		moves.push_back(position);
	}
	for( auto move : moves ) {
		if( move.X == destination.X && move.Y == destination.Y ) {
			if( quantum ) {
				return QuantMoveFigure(destination);
			}
			return MoveFigure(destination);
		}
	}

	return ImpossibleMove;
}

void CBoard::SetFigure(CChessFigure& figure)
{
	Coordinates boardPosition = figure.GetBoardPosition();
	figure.SetSpritePosition(Coordinates(startX + squareWidth * boardPosition.X +
											(squareRest >= boardPosition.X ? boardPosition.X : squareRest) + squareWidth / 2,
										 startY + squareWidth * boardPosition.Y + 
											(squareRest >= boardPosition.Y ? boardPosition.Y : squareRest) + squareWidth / 2));
	figures[boardPosition.X][boardPosition.Y] = &figure;
}

void CBoard::SetKings(CChessFigure* blackKing, CChessFigure* whiteKing)
{
	this->blackKing = blackKing;
	this->whiteKing = whiteKing;
}

CBoard::MoveType CBoard::QuantMoveFigure(Coordinates destination)
{
	auto position = selectedFigure->GetBoardPosition();
	auto status = Moved;
	/*if( ValidateCheck(selectedFigure->GetColor(), selectedFigure->GetBoardPosition(), destination) ) {
		return ImpossibleMove;
	}*/
	if( position.X == destination.X && position.Y == destination.Y ) {
		selectedFigure->SetUnselected();
		return status;
	}
	auto oldPosition = selectedFigure->GetOldBoardPosition();
	if( destination.X == oldPosition.X && destination.Y == oldPosition.Y ) {
		return ImpossibleMove;
	}
	auto newState = static_cast<CChessFigure*>(selectedFigure->Split());
	scene->AddObject(newState, 1);
	figures[oldPosition.X][oldPosition.Y] = selectedFigure;
	figures[position.X][position.Y] = newState;
	selectedFigure->SetBoardPosition(oldPosition);
	selectedFigure->SetUnselected();
	this->SetFigure(*selectedFigure);
	selectedFigure = newState;
	selectedFigure->SetUnselected();
	return SetFigureWithCollapse(destination);
}

CBoard::MoveType CBoard::MoveFigure(Coordinates destination)
{
	auto position = selectedFigure->GetBoardPosition();
	auto status = Moved;
	/*if( ValidateCheck(selectedFigure->GetColor(), selectedFigure->GetBoardPosition(), destination) ) {
		return ImpossibleMove;
	}*/
	
	//selectedFigure->SetUnselected();
	//this->SetFigure(*selectedFigure);
	/*if( IsCheck((CChessFigure::FigureColor)((selectedFigure->GetColor() + 1) % 2), figures) ) {
		if( selectedFigure->GetColor() != CChessFigure::WHITE ) {
			checkWhite = true;
			if( IsMate(CChessFigure::WHITE) ) {
				status = BlackWins;
			}
		} else {
			checkBlack = true;
			if( IsMate(CChessFigure::BLACK) ) {
				status = WhiteWins;
			}
		}
	}
	if( selectedFigure->GetColor() == CChessFigure::WHITE  && checkWhite) {
		checkWhite = false;
	}
	if( selectedFigure->GetColor() == CChessFigure::BLACK  && checkBlack ) {
		checkBlack = false;
	}*/
	return SetFigureWithCollapse(destination);
}

CBoard::MoveType CBoard::SetFigureWithCollapse(Coordinates destination)
{
	auto position = selectedFigure->GetBoardPosition();
	auto status = Moved;
	if( figures[destination.X][destination.Y] ) {
		this->Collapse(selectedFigure);
		this->Collapse(figures[destination.X][destination.Y]);
		if( !figures[position.X][position.Y] ) {
			return CollapsedToAnotherPlace;
		}
		if( figures[destination.X][destination.Y] ) {
			if( figures[destination.X][destination.Y]->GetFigure() == CChessFigure::KING ) {
				if( selectedFigure->GetColor() == CChessFigure::WHITE ) {
					status = WhiteWins;
				}
				else {
					status = BlackWins;
				}
			}
			scene->DeleteObject(figures[destination.X][destination.Y]);
			figures[position.X][position.Y] = nullptr;
			figures[destination.X][destination.Y] = selectedFigure;
			selectedFigure->SetBoardPosition(destination);
		}
	}
	else {
		figures[position.X][position.Y] = nullptr;
		figures[destination.X][destination.Y] = selectedFigure;
		selectedFigure->SetBoardPosition(destination);
	}
	this->SetFigure(*selectedFigure);
	return status;
}

/*bool CBoard::IsCheck(CChessFigure::FigureColor color, CChessFigure* figures[8][8])
{
	Coordinates kingCoords;
	if( color == CChessFigure::WHITE ) {
		kingCoords = whiteKing->GetBoardPosition();
	} else {
		kingCoords = blackKing->GetBoardPosition();
	}
 	//Check knights
	if( ((kingCoords.X + 1 < 8 && kingCoords.Y + 2 < 8) && figures[kingCoords.X + 1][kingCoords.Y + 2] &&
			figures[kingCoords.X + 1][kingCoords.Y + 2]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X + 1][kingCoords.Y + 2]->GetColor() != color) || 
		((kingCoords.X + 1 < 8 && kingCoords.Y - 2 >= 0) && figures[kingCoords.X + 1][kingCoords.Y - 2] &&
			figures[kingCoords.X + 1][kingCoords.Y - 2]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X + 1][kingCoords.Y - 2]->GetColor() != color) ||
		((kingCoords.X + 2 < 8 && kingCoords.Y + 1 < 8) && figures[kingCoords.X + 2][kingCoords.Y + 1] &&
			figures[kingCoords.X + 2][kingCoords.Y + 1]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X + 2][kingCoords.Y + 1]->GetColor() != color) || 
		((kingCoords.X + 2 < 8 && kingCoords.Y - 1 >= 0) && figures[kingCoords.X + 2][kingCoords.Y - 1] &&
			figures[kingCoords.X + 2][kingCoords.Y - 1]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X + 2][kingCoords.Y - 1]->GetColor() != color) ||
		((kingCoords.X - 1 >= 0 && kingCoords.Y + 2 < 8) && figures[kingCoords.X - 1][kingCoords.Y + 2] &&
			figures[kingCoords.X - 1][kingCoords.Y + 2]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X - 1][kingCoords.Y + 2]->GetColor() != color) ||
		((kingCoords.X - 1 >= 0 && kingCoords.Y - 2 >= 0) && figures[kingCoords.X - 1][kingCoords.Y - 2] &&
			figures[kingCoords.X - 1][kingCoords.Y - 2]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X - 1][kingCoords.Y - 2]->GetColor() != color) ||
		((kingCoords.X - 2 >= 0 && kingCoords.Y + 1 < 8) && figures[kingCoords.X - 2][kingCoords.Y + 1] &&
			figures[kingCoords.X - 2][kingCoords.Y + 1]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X - 2][kingCoords.Y + 1]->GetColor() != color) ||
		((kingCoords.X - 2 >= 0 && kingCoords.Y - 1 >= 0) && figures[kingCoords.X - 2][kingCoords.Y - 1] &&
			figures[kingCoords.X - 2][kingCoords.Y - 1]->GetFigure() == CChessFigure::KNIGHT &&
			figures[kingCoords.X - 2][kingCoords.Y - 1]->GetColor() != color) ) {
		return true;
	}
	//Check straight
	for( int i = kingCoords.X + 1; i < 8; ++i ) {
		if( figures[i][kingCoords.Y] ) {
			if( figures[i][kingCoords.Y]->GetColor() == color ) {
				break;
			}
			if( i == kingCoords.X + 1 && figures[i][kingCoords.Y]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[i][kingCoords.Y]->GetFigure() == CChessFigure::ROOK || figures[i][kingCoords.Y]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	for( int i = kingCoords.X - 1; i >=0; --i ) {
		if( figures[i][kingCoords.Y] ) {
			if( figures[i][kingCoords.Y]->GetColor() == color ) {
				break;
			}
			if( i == kingCoords.X - 1 && figures[i][kingCoords.Y]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[i][kingCoords.Y]->GetFigure() == CChessFigure::ROOK || figures[i][kingCoords.Y]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	for( int j = kingCoords.Y + 1; j < 8; ++j ) {
		if( figures[kingCoords.X][j] ) {
			if( figures[kingCoords.X][j]->GetColor() == color ) {
				break;
			}
			if( j == kingCoords.Y + 1 && figures[kingCoords.X][j]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[kingCoords.X][j]->GetFigure() == CChessFigure::ROOK || figures[kingCoords.X][j]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	for( int j = kingCoords.Y - 1; j >= 0; --j ) {
		if( figures[kingCoords.X][j] ) {
			if( figures[kingCoords.X][j]->GetColor() == color ) {
				break;
			}
			if( j == kingCoords.Y - 1 && figures[kingCoords.X][j]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[kingCoords.X][j]->GetFigure() == CChessFigure::ROOK || figures[kingCoords.X][j]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	//Check diagonal
	for( int i = 1; kingCoords.X + i < 8 && kingCoords.Y + i < 8; ++i ) {
		if( figures[kingCoords.X + i][kingCoords.Y + i] ) {
			if( figures[kingCoords.X + i][kingCoords.Y + i]->GetColor() == color ) {
				break;
			}
			if( i == 1 && figures[kingCoords.X + i][kingCoords.Y + i]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[kingCoords.X + i][kingCoords.Y + i]->GetFigure() == CChessFigure::BISHOP || figures[kingCoords.X + i][kingCoords.Y + i]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	for( int i = 1; kingCoords.X + i < 8 && kingCoords.Y - i >=0 ; ++i ) {
		if( figures[kingCoords.X + i][kingCoords.Y - i] ) {
			if( figures[kingCoords.X + i][kingCoords.Y - i]->GetColor() == color ) {
				break;
			}
			if( i == 1 && figures[kingCoords.X + i][kingCoords.Y - i]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[kingCoords.X + i][kingCoords.Y - i]->GetFigure() == CChessFigure::BISHOP || figures[kingCoords.X + i][kingCoords.Y - i]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	for( int i = 1; kingCoords.X - i >= 0 && kingCoords.Y + i < 8; ++i ) {
		if( figures[kingCoords.X - i][kingCoords.Y + i] ) {
			if( figures[kingCoords.X - i][kingCoords.Y + i]->GetColor() == color ) {
				break;
			}
			if( i == 1 && figures[kingCoords.X - i][kingCoords.Y + i]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[kingCoords.X - i][kingCoords.Y + i]->GetFigure() == CChessFigure::BISHOP || figures[kingCoords.X - i][kingCoords.Y + i]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	for( int i = 1; kingCoords.X - i >= 0 && kingCoords.Y - i >= 0; ++i ) {
		if( figures[kingCoords.X - i][kingCoords.Y - i] ) {
			if( figures[kingCoords.X - i][kingCoords.Y - i]->GetColor() == color ) {
				break;
			}
			if( i == 1 && figures[kingCoords.X - i][kingCoords.Y - i]->GetFigure() == CChessFigure::KING ){
				return true;
			}
			if( figures[kingCoords.X - i][kingCoords.Y - i]->GetFigure() == CChessFigure::BISHOP || figures[kingCoords.X - i][kingCoords.Y - i]->GetFigure() == CChessFigure::QUEEN ){
				return true;
			}
			break;
		}
	}
	switch( color ) {
	case CChessFigure::WHITE:
		if( kingCoords.X - 1 >= 0 && kingCoords.Y - 1>= 0 && figures[kingCoords.X - 1][kingCoords.Y - 1] &&
			figures[kingCoords.X - 1][kingCoords.Y - 1]->GetColor() != color &&
			figures[kingCoords.X - 1][kingCoords.Y - 1]->GetFigure() == CChessFigure::PAWN ) {
			return true;
		}
		if( kingCoords.X + 1 < 8 && kingCoords.Y - 1 >= 0 && figures[kingCoords.X + 1][kingCoords.Y - 1] &&
			figures[kingCoords.X + 1][kingCoords.Y - 1]->GetColor() != color &&
			figures[kingCoords.X + 1][kingCoords.Y - 1]->GetFigure() == CChessFigure::PAWN ) {
			return true;
		}
		break;
	case CChessFigure::BLACK:
		if( kingCoords.X - 1 >= 0 && kingCoords.Y + 1 < 8 && figures[kingCoords.X - 1][kingCoords.Y + 1] &&
			figures[kingCoords.X - 1][kingCoords.Y + 1]->GetColor() != color &&
			figures[kingCoords.X - 1][kingCoords.Y + 1]->GetFigure() == CChessFigure::PAWN ) {
			return true;
		}
		if( kingCoords.X + 1 < 8 && kingCoords.Y + 1 < 8 && figures[kingCoords.X + 1][kingCoords.Y + 1] && 
			figures[kingCoords.X + 1][kingCoords.Y + 1]->GetColor() != color &&
			figures[kingCoords.X + 1][kingCoords.Y + 1]->GetFigure() == CChessFigure::PAWN ) {
			return true;
		}
		break;
	}
	return false;
}*/

/*bool CBoard::ValidateCheck(CChessFigure::FigureColor color, Coordinates from, Coordinates to)
{
	bool result;
	auto oldFrom = figures[from.X][from.Y];
	auto oldTo = figures[to.X][to.Y];
	figures[to.X][to.Y] = figures[from.X][from.Y];
	figures[from.X][from.Y] = nullptr;
	if( oldFrom ) {
		oldFrom->SetBoardPosition(to);
	}
	result = IsCheck(color, figures);
	figures[from.X][from.Y] = oldFrom;
	figures[to.X][to.Y] = oldTo;
	if( oldFrom ) {
		oldFrom->SetBoardPosition(from);
	}
	return result;
}

bool CBoard::IsMate(CChessFigure::FigureColor color)
{
	for( int i = 0; i < 8; ++i ) {
		for( int j = 0; j < 8; ++j ) {
			if( figures[i][j] && figures[i][j]->GetColor() == color ) {
				auto moves = GetMoves(Coordinates(i, j));
				if( moves.size() > 0 ) {
					return false;
				}
			}
		}
	}
	return true;
}*/

std::vector<Coordinates> CBoard::GetMoves(Coordinates position)
{
	auto figure = figures[position.X][position.Y];
	std::vector<Coordinates> moves;
	switch( figure->GetFigure() ) {
	case CChessFigure::PAWN:
		GetPawnMoves(figure, moves);
		break;
	case CChessFigure::ROOK:
		GetRookMoves(figure, moves);
		break;
	case CChessFigure::KNIGHT:
		GetKnightMoves(figure, moves);
		break;
	case CChessFigure::BISHOP:
		GetBishopMoves(figure, moves);
		break;
	case CChessFigure::QUEEN:
		GetQueenMoves(figure, moves);
		break;
	case CChessFigure::KING:
		GetKingMoves(figure, moves);
		break;
	}
	return moves;
}

void CBoard::GetPawnMoves(CChessFigure* figure, std::vector<Coordinates>& moves)
{
	auto color = figure->GetColor();
	auto position = figure->GetBoardPosition();
	if( color == CChessFigure::WHITE ) {
		auto destination = Coordinates(position.X, position.Y - 1);
		if( InBound(destination) && !figures[destination.X][destination.Y] ) {
			AddIfNotCheck(color, position, destination, moves);
		}
		if( position.Y == 6 ) {
			destination = Coordinates(position.X, position.Y - 2);
			if( InBound(destination) && !figures[destination.X][destination.Y] ) {
				AddIfNotCheck(color, position, destination, moves);
			}
		}
		destination = Coordinates(position.X + 1, position.Y - 1);
		if( InBound(destination) && figures[destination.X][destination.Y] && figures[destination.X][destination.Y]->GetColor() == CChessFigure::BLACK ) {
			AddIfNotCheck(color, position, destination, moves);
		}
		destination = Coordinates(position.X - 1, position.Y - 1);
		if( InBound(destination) && figures[destination.X][destination.Y] && figures[destination.X][destination.Y]->GetColor() == CChessFigure::BLACK ) {
			AddIfNotCheck(color, position, destination, moves);
		}
	} else {
		auto destination = Coordinates(position.X, position.Y + 1);
		if( InBound(destination) && !figures[destination.X][destination.Y] ) {
			AddIfNotCheck(color, position, destination, moves);
		}
		if( position.Y == 1 ) {
			destination = Coordinates(position.X, position.Y + 2);
			if( InBound(destination) && !figures[destination.X][destination.Y] ) {
				AddIfNotCheck(color, position, destination, moves);
			}
		}
		destination = Coordinates(position.X + 1, position.Y + 1);
		if( InBound(destination) && figures[destination.X][destination.Y] && figures[destination.X][destination.Y]->GetColor() == CChessFigure::WHITE ) {
			AddIfNotCheck(color, position, destination, moves);
		}
		destination = Coordinates(position.X - 1, position.Y + 1);
		if( InBound(destination) && figures[destination.X][destination.Y] && figures[destination.X][destination.Y]->GetColor() == CChessFigure::WHITE ) {
			AddIfNotCheck(color, position, destination, moves);
		}
	}
}

void CBoard::GetRookMoves(CChessFigure* figure, std::vector<Coordinates>& moves)
{
	auto position = figure->GetBoardPosition();
	auto color = figure->GetColor();
	for( int i = position.X + 1; i < 8; ++i ) {
		if( !figures[i][position.Y] ) {
			AddIfNotCheck(color, position, Coordinates(i, position.Y), moves);
			continue;
		}
		if( figures[i][position.Y]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(i, position.Y), moves);
		break;
	}
	for( int i = position.X - 1; i >= 0; --i ) {
		if( !figures[i][position.Y] ) {
			AddIfNotCheck(color, position, Coordinates(i, position.Y), moves);
			continue;
		}
		if( figures[i][position.Y]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(i, position.Y), moves);
		break;
	}
	for( int j = position.Y + 1; j < 8; ++j ) {
		if( !figures[position.X][j] ) {
			AddIfNotCheck(color, position, Coordinates(position.X, j), moves);
			continue;
		}
		if( figures[position.X][j]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(position.X, j), moves);
		break;
	}
	for( int j = position.Y - 1; j >= 0; --j ) {
		if( !figures[position.X][j] ) {
			AddIfNotCheck(color, position, Coordinates(position.X, j), moves);
			continue;
		}
		if( figures[position.X][j]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(position.X, j), moves);
		break;
	}
}

void CBoard::GetKnightMoves(CChessFigure * figure, std::vector<Coordinates>& moves)
{
	auto position = figure->GetBoardPosition();
	auto color = figure->GetColor();
	Coordinates destination;
	destination.X = position.X + 1;
	destination.Y = position.Y - 2;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
	destination.X = position.X + 2;
	destination.Y = position.Y - 1;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	} 
	destination.X = position.X + 2;
	destination.Y = position.Y + 1;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
	destination.X = position.X + 1;
	destination.Y = position.Y + 2;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
	destination.X = position.X - 1;
	destination.Y = position.Y + 2;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
	destination.X = position.X - 2;
	destination.Y = position.Y + 1;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
	destination.X = position.X - 2;
	destination.Y = position.Y - 1;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
	destination.X = position.X - 1;
	destination.Y = position.Y - 2;
	if( InBound(destination) && (!figures[destination.X][destination.Y] || figures[destination.X][destination.Y]->GetColor() != color) ) {
		AddIfNotCheck(color, position, destination, moves);
	}
}

void CBoard::GetBishopMoves(CChessFigure* figure, std::vector<Coordinates>& moves)
{
	auto position = figure->GetBoardPosition();
	auto color = figure->GetColor();
	for( int i = 1; position.X + i < 8 && position.Y + i < 8; ++i ) {
		if( !figures[position.X + i][position.Y + i] ) {
			AddIfNotCheck(color, position, Coordinates(position.X + i, position.Y + i), moves);
			continue;
		}
		if( figures[position.X + i][position.Y + i]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(position.X + i, position.Y + i), moves);
		break;
	}
	for( int i = 1; position.X + i < 8 && position.Y - i >= 0; ++i ) {
		if( !figures[position.X + i][position.Y - i] ) {
			AddIfNotCheck(color, position, Coordinates(position.X + i, position.Y - i), moves);
			continue;
		}
		if( figures[position.X + i][position.Y - i]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(position.X + i, position.Y - i), moves);
		break;
	}
	for( int i = 1; position.X - i >= 0 && position.Y + i < 8; ++i ) {
		if( !figures[position.X - i][position.Y + i] ) {
			AddIfNotCheck(color, position, Coordinates(position.X - i, position.Y + i), moves);
			continue;
		}
		if( figures[position.X - i][position.Y + i]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(position.X - i, position.Y + i), moves);
		break;
	}
	for( int i = 1; position.X - i >= 0 && position.Y - i >= 0; ++i ) {
		if( !figures[position.X - i][position.Y - i] ) {
			AddIfNotCheck(color, position, Coordinates(position.X - i, position.Y - i), moves);
			continue;
		}
		if( figures[position.X - i][position.Y - i]->GetColor() == color ) {
			break;
		}
		AddIfNotCheck(color, position, Coordinates(position.X - i, position.Y - i), moves);
		break;
	}
}

void CBoard::GetQueenMoves(CChessFigure * figure, std::vector<Coordinates>& moves)
{
	GetRookMoves(figure, moves);
	GetBishopMoves(figure, moves);
}

void CBoard::GetKingMoves(CChessFigure * figure, std::vector<Coordinates>& moves)
{
	auto position = figure->GetBoardPosition();
	auto color = figure->GetColor();
	if( position.X + 1 < 8 ) {
		if( !figures[position.X + 1][position.Y] ) {
			AddIfNotCheck(color, position, Coordinates(position.X + 1, position.Y), moves);
		} else if( figures[position.X + 1][position.Y]->GetColor() != color ) {
			AddIfNotCheck(color, position, Coordinates(position.X + 1, position.Y), moves);
		}
		if( position.Y + 1 < 8 ) {
			if( !figures[position.X + 1][position.Y + 1] ) {
				AddIfNotCheck(color, position, Coordinates(position.X + 1, position.Y + 1), moves);
			}
			else if( figures[position.X + 1][position.Y + 1]->GetColor() != color ) {
				AddIfNotCheck(color, position, Coordinates(position.X + 1, position.Y + 1), moves);
			}
		}
		if( position.Y - 1 >= 0 ) {
			if( !figures[position.X + 1][position.Y - 1] ) {
				AddIfNotCheck(color, position, Coordinates(position.X + 1, position.Y - 1), moves);
			}
			else if( figures[position.X + 1][position.Y - 1]->GetColor() != color ) {
				AddIfNotCheck(color, position, Coordinates(position.X + 1, position.Y - 1), moves);
			}
		}
	}
	if( position.X - 1 >= 0 ) {
		if( !figures[position.X - 1][position.Y] ) {
			AddIfNotCheck(color, position, Coordinates(position.X - 1, position.Y), moves);
		}
		else if( figures[position.X - 1][position.Y]->GetColor() != color ) {
			AddIfNotCheck(color, position, Coordinates(position.X - 1, position.Y), moves);
		}
		if( position.Y + 1 < 8 ) {
			if( !figures[position.X - 1][position.Y + 1] ) {
				AddIfNotCheck(color, position, Coordinates(position.X - 1, position.Y + 1), moves);
			}
			else if( figures[position.X - 1][position.Y + 1]->GetColor() != color ) {
				AddIfNotCheck(color, position, Coordinates(position.X - 1, position.Y + 1), moves);
			}
		}
		if( position.Y - 1 >= 0 ) {
			if( !figures[position.X - 1][position.Y - 1] ) {
				AddIfNotCheck(color, position, Coordinates(position.X - 1, position.Y - 1), moves);
			}
			else if( figures[position.X - 1][position.Y - 1]->GetColor() != color ) {
				AddIfNotCheck(color, position, Coordinates(position.X - 1, position.Y - 1), moves);
			}
		}
	}
	if( position.Y + 1 < 8 ) {
		if( !figures[position.X][position.Y + 1] ) {
			AddIfNotCheck(color, position, Coordinates(position.X, position.Y + 1), moves);
		}
		else if( figures[position.X][position.Y + 1]->GetColor() != color ) {
			AddIfNotCheck(color, position, Coordinates(position.X, position.Y + 1), moves);
		}
	}
	if( position.Y - 1 >= 0 ) {
		if( !figures[position.X][position.Y - 1] ) {
			AddIfNotCheck(color, position, Coordinates(position.X, position.Y - 1), moves);
		}
		else if( figures[position.X][position.Y - 1]->GetColor() != color ) {
			AddIfNotCheck(color, position, Coordinates(position.X, position.Y - 1), moves);
		}
	}
}

void CBoard::AddIfNotCheck(CChessFigure::FigureColor color, Coordinates from, Coordinates to, std::vector<Coordinates>& moves)
{
	//if( !ValidateCheck(color, from, to) ) {
		moves.push_back(to);
	//}
}

void CBoard::Collapse(CChessFigure* figure)
{
	auto quant = figure->GetQuantObject();
	auto states = quant->GetStates();
	int id = quant->Collapse();
	for( int i = 0; i < states.size(); ++i ) {
		auto state = static_cast<CChessFigure*>(states[i]);
		if( i != id ) {
			auto position = state->GetBoardPosition();
			figures[position.X][position.Y] = nullptr;
			scene->DeleteObject(state);
		} else {
			state->ResetProbability();
		}
	}
}

bool CBoard::InBound(Coordinates coords)
{
	return coords.X >= 0 && coords.X < 8 && coords.Y >= 0 && coords.Y < 8;
}

void CBoard::Render(HDC hdc)
{
	SetDCBrushColor(hdc, BOARD_BACKGROUND_COLOR);
	FillRect(hdc, &boardRect, (HBRUSH)GetStockObject(DC_BRUSH));
	RECT squareRect;
	int yRest = squareRest;
	int yMoved = 0;
	for( int i = 0; i < 8; ++i ) {
		int xRest = squareRest;
		int xMoved = 0;
		for( int j = 0; j < 8; ++j ) {
			squareRect.left = startX + j * squareWidth + xMoved;
			squareRect.right = startX + (j + 1) * squareWidth + xMoved;
			if( xRest ) {
				xRest -= 1;
				squareRect.right += 1;
				xMoved += 1;
			}
			squareRect.top = startY + i * squareWidth + yMoved;
			squareRect.bottom = startY + (i + 1) * squareWidth + yMoved;
			if( yRest ) {
				squareRect.bottom += 1;
			}
			SetDCBrushColor(hdc, (i + j) % 2 ? BOARD_BLACK_COLOR : BOARD_WHITE_COLOR);
			FillRect(hdc, &squareRect, (HBRUSH)GetStockObject(DC_BRUSH));
		}
		if( yRest ) {
			yRest -= 1;
			yMoved += 1;
		}
	}
}

bool CBoard::OnTimerTick()
{
	return false;
}

void CBoard::FetchResources(CResourceManager & manager)
{
}

BoardCell::BoardCell()
{
}

BoardCell::BoardCell(Coordinates position, Gdiplus::RectF box): Position(position), Box(box)
{
}
