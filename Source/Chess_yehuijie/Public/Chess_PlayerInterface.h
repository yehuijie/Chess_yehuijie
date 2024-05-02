

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Chess_PlayerInterface.generated.h"

// probabilmente non utilizzato, copia di EPiece in BasePiece.h
UENUM()
enum class EPiece1 : uint8
{
	W_Pawn,
	W_Knight,
	W_Bishop,
	W_Rook,
	W_Queen,
	W_King,
	B_Pawn,
	B_Knight,
	B_Bishop,
	B_Rook,
	B_Queen,
	B_King,
	E
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UChess_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHESS_YEHUIJIE_API IChess_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	int32 PlayerNumber; // 0 for white, 1 for black
	//EPiece Piece;

	virtual void OnTurn() {};
	virtual void OnWin() {};
	virtual void OnLose() {};
};
