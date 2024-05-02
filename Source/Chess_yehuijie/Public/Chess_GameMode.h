// In fondo al file ci sono le funzioni che avevo iniziato a scrivere per provare a far funzionare il RandomPlayer, ma ho riscontrato
// dei problemi perchè avevo skippato il 5. ed ero passato direttamente al punto 7.

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

// Declaration of AChess_GameMode, which inherits AGameModeBase
UCLASS()
class CHESS_YEHUIJIE_API AChess_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// tracks if the game is over (non utilizzato)
	bool IsGameOver;
	// array of player interfaces
	TArray<IChess_PlayerInterface*> Players;
        // tracks the player on turn 
	int32 CurrentPlayer;
	// tracks the number of moves in order to signal a drawn game (non utilizzato)
	int32 MoveCounter;
        // random index to select a random Piece for the RandomPlayer (non utilizzato)
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

	// TSubclassOf template class that provides UClass type safety, Yellow Tiles indicate Moving Tiles, Red Tiles are Eating Tiles
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassYellow;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassRed;

        // TSubclassOf template class that provides UClass type safety, one for each Piece
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

        // Array of all Black/White Moves, of all the Pieces
	UPROPERTY(Transient)
	TArray<FVector2D> BlackMovesArray;
        UPROPERTY(Transient)
	TArray<FVector2D> WhiteMovesArray;

        // Array of Moves (no EatingMoves) of each Piece
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

	// Array of Eating Moves of each Piece
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

	// Array of (x,y) Positions that are under Check by Whites/Blacks
        UPROPERTY(Transient)
	TArray<FVector2D> WhiteChecksArray;
	UPROPERTY(Transient)
	TArray<FVector2D> BlackChecksArray;

	// Sets default values for this actor's properties
        AChess_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	void ChoosePlayerAndStartGame();

	// Called when a specific Piece is Clicked, in order to generate the possible Moves and fill the respective Arrays
        void GeneratePawnMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateQueenMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateKingMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateRookMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateBishopMoves(const FVector2D& PiecePosition, FString Color);
	void GenerateKnightMoves(const FVector2D& PiecePosition, FString Color);

        // Called after Moves are generated and added to their specific Array, Spawns the SuggestionTiles
        void ShowMoves(const FVector2D& PiecePosition, EPiece Piece, FString Color);

	// Called after the SuggestionTiles are spawned, after its Set to Spawned, we're ready for the second Click from the player
        void SetPieceMovesToSpawned(const FVector2D& Position);

	// Called to Spawn Moving/Eating Tiles to suggest Moves
        void SpawnMovTile(const FVector2D& TilePosition, const FVector2D& PiecePosition);
	void SpawnEatTile(const FVector2D& TilePosition, const FVector2D& PiecePosition);

        // Called after a Pieces have been moved to destory the SuggestionTiles
	void DestroyMoveTiles();

	// Called when the Second Click is a ValidMove and therefore it moves the Piece
        void MoveClickedPiece( const FVector2D& NewPosition, FString Color);

	// Called when the second Click is not a ValidMove, sets Clicked Piece to NotClicked to avoid the Piece moving by mistake
        void SetPieceToNotClicked();

        // Called to check if a Piece is Black/White
	bool IsPieceBlack(const FVector2D& Position);
	bool IsPieceWhite(const FVector2D& Position);

	// Called After a Piece got Moved to generate all the new Tiles that are threatening for the opposing King
        void VerifyChecksByColorAfterTurn(FString Color);

        // Called Before a Piece is Moved to Reset the Checking Tiles
	void SetTilesToNotCheckedByColorBeforeTurn(FString Color);

        // Called when generating King moves, returns true if a Piece cannot be eaten by King cause its Protected by another Piece
	bool IsPieceProtected();

	// get the next player index (non utilizzato)
	int32 GetNextPlayer(int32 Player);

	// called at the end of the game turn (non utilizzato)
	void TurnNextPlayer();

        // funzioni per il RandomPlayer
        /*void MoveBlackPiece();
        void SetBlackToMove();
        int32 GetBlackArraySize();*/
};
	
