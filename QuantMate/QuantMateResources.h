#pragma once

#include "GameResources.h"

class CQuantMateResources: public IGameResources {
public:
	static const long PAWN_WHITE = 101;
	static const long PAWN_BLACK = 102;

	static const long ROOK_WHITE = 103;
	static const long ROOK_BLACK = 104;

	static const long BISHOP_WHITE = 105;
	static const long BISHOP_BLACK = 106;

	static const long KNIGHT_WHITE = 107;
	static const long KNIGHT_BLACK = 108;

	static const long QUEEN_WHITE = 109;
	static const long QUEEN_BLACK = 110;

	static const long KING_WHITE = 111;
	static const long KING_BLACK = 112;

	void InitializeResources(CResourceManager& manager) override;

	static const std::wstring PAWN_WHITE_PATH;
	static const std::wstring PAWN_BLACK_PATH;

	static const std::wstring ROOK_WHITE_PATH;
	static const std::wstring ROOK_BLACK_PATH;

	static const std::wstring BISHOP_WHITE_PATH;
	static const std::wstring BISHOP_BLACK_PATH;

	static const std::wstring KNIGHT_WHITE_PATH;
	static const std::wstring KNIGHT_BLACK_PATH;

	static const std::wstring QUEEN_WHITE_PATH;
	static const std::wstring QUEEN_BLACK_PATH;

	static const std::wstring KING_WHITE_PATH;
	static const std::wstring KING_BLACK_PATH;
};