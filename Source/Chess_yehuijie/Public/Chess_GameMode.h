// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_PlayerInterface.h"
#include "GameField.h"
#include "BasePiece.h"
#include "GameFramework/GameModeBase.h"
#include "Chess_GameMode.generated.h"

class AActor;

struct FPosition;
/**
 * 
 */
UCLASS()
class CHESS_YEHUIJIE_API AChess_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// tracks if the game is over
	bool IsGameOver;
	// array of player interfaces
	TArray<IChess_PlayerInterface*> Players;
	int32 CurrentPlayer;
	// tracks the number of moves in order to signal a drawn game -> registo?
	int32 MoveCounter;

	int32 RandIdx;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;

	// field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
	AGameField* GField;

	// keeps track of tiles
	UPROPERTY(Transient)
	TArray<ATile*> SpawnedTileArray;
	//given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> SpawnedTileMap;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassYellow;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassRed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WPawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WRookActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WKnightActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
        TSubclassOf<AActor> WBishopActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WQueenActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WKingActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BPawnActor;
        UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BRookActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BKnightActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BBishopActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BQueenActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BKingActor;

	UPROPERTY(Transient)
	TArray<FVector2D> BlackMovesArray;
        UPROPERTY(Transient)
	TArray<FVector2D> WhiteMovesArray;

	UPROPERTY(Transient)
	TArray<FVector2D> PawnMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> QueenMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> KingMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> BishopMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> KnightMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> RookMovesArray;	

	UPROPERTY(Transient)
	TArray<FVector2D> QueenEatingMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> KingEatingMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> BishopEatingMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> KnightEatingMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> RookEatingMovesArray;
	UPROPERTY(Transient)
	TArray<FVector2D> PawnEatingMovesArray;

	UPROPERTY(Transient)
	TArray<FVector2D> WhiteChecksArray;
	UPROPERTY(Transient)
	TArray<FVector2D> BlackChecksArray;

	AChess_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	void ChoosePlayerAndStartGame();

	void GeneratePawnMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateQueenMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateKingMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateRookMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateBishopMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateKnightMoves(const FVector2D& PiecePosition, FString Color);

        void ShowMoves(const FVector2D& PiecePosition, EPiece Piece, FString Color);

	void SetPieceMovesToSpawned(const FVector2D& Position);

	void SpawnMovTile(const FVector2D& TilePosition, const FVector2D& PiecePosition);
	void SpawnEatTile(const FVector2D& TilePosition, const FVector2D& PiecePosition);

	void DestroyMoveTiles();

	void MoveClickedPiece( const FVector2D& NewPosition, FString Color);

	bool IsPieceBlack(const FVector2D& Position);
	bool IsPieceWhite(const FVector2D& Position);

	void SetPieceToNotClicked();

	void VerifyChecksByColorAfterTurn(FString Color);

	void SetTilesToNotCheckedByColorBeforeTurn(FString Color);

	bool IsPieceProtected();

	// get the next player index
	int32 GetNextPlayer(int32 Player);

	// called at the end of the game turn
	void TurnNextPlayer();

        /*void MoveBlackPiece();
        void SetBlackToMove();
        int32 GetBlackArraySize();*/
};
	
