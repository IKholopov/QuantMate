#include "QuantMateResources.h"

#include "BitmapResource.h"

const std::wstring CQuantMateResources::PAWN_WHITE_PATH = L"res\\Chess_plt45.png";
const std::wstring CQuantMateResources::PAWN_BLACK_PATH = L"res\\Chess_pdt45.png";

const std::wstring CQuantMateResources::ROOK_WHITE_PATH = L"res\\Chess_rlt45.png";
const std::wstring CQuantMateResources::ROOK_BLACK_PATH = L"res\\Chess_rdt45.png";

const std::wstring CQuantMateResources::BISHOP_WHITE_PATH = L"res\\Chess_blt45.png";
const std::wstring CQuantMateResources::BISHOP_BLACK_PATH = L"res\\Chess_bdt45.png";

const std::wstring CQuantMateResources::KNIGHT_WHITE_PATH = L"res\\Chess_nlt45.png";
const std::wstring CQuantMateResources::KNIGHT_BLACK_PATH = L"res\\Chess_ndt45.png";

const std::wstring CQuantMateResources::QUEEN_WHITE_PATH = L"res\\Chess_qlt45.png";
const std::wstring CQuantMateResources::QUEEN_BLACK_PATH = L"res\\Chess_qdt45.png";

const std::wstring CQuantMateResources::KING_WHITE_PATH = L"res\\Chess_klt45.png";
const std::wstring CQuantMateResources::KING_BLACK_PATH = L"res\\Chess_kdt45.png";

void CQuantMateResources::InitializeResources(CResourceManager& manager)
{
	manager.RegisterResource(PAWN_WHITE,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(PAWN_WHITE_PATH))));
	manager.RegisterResource(PAWN_BLACK,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(PAWN_BLACK_PATH))));
	manager.RegisterResource(ROOK_WHITE,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(ROOK_WHITE_PATH))));
	manager.RegisterResource(ROOK_BLACK,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(ROOK_BLACK_PATH))));
	manager.RegisterResource(BISHOP_WHITE,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(BISHOP_WHITE_PATH))));
	manager.RegisterResource(BISHOP_BLACK,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(BISHOP_BLACK_PATH))));
	manager.RegisterResource(KNIGHT_WHITE,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(KNIGHT_WHITE_PATH))));
	manager.RegisterResource(KNIGHT_BLACK,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(KNIGHT_BLACK_PATH))));
	manager.RegisterResource(QUEEN_WHITE,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(QUEEN_WHITE_PATH))));
	manager.RegisterResource(QUEEN_BLACK,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(QUEEN_BLACK_PATH))));
	manager.RegisterResource(KING_WHITE,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(KING_WHITE_PATH))));
	manager.RegisterResource(KING_BLACK,
		std::move(std::unique_ptr<CBitmapResource>(new CBitmapResource(KING_BLACK_PATH))));
}