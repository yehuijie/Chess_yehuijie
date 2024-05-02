

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BasePiece.h"
#include "BlackPiece.h"
#include "WhitePiece.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

// Declaration of AGameField Class, which inherits AActor
UCLASS()
class CHESS_YEHUIJIE_API AGameField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameField();

	// keeps track of tiles
	UPROPERTY(Transient)
	TArray<ATile*> TileArray;

	//given a position returns a tile
	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;

        // Normalizes the Cell Padding
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	// size of winning line -> copiato da TTT
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WinSize;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassWhite;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassBlack;

	// Keeps track of Pieces
	UPROPERTY(Transient)
	TArray<ABasePiece*> PiecesArray;
	UPROPERTY(Transient)
	TMap<FVector2D, ABasePiece*> PiecesMap;

        // TSubclassOf template class that provides UClass type safety, for White/Black Pieces
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWhitePiece> WPawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWhitePiece> WRookActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWhitePiece> WKnightActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWhitePiece> WBishopActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWhitePiece> WQueenActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AWhitePiece> WKingActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABlackPiece> BPawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABlackPiece> BRookActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABlackPiece> BKnightActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABlackPiece> BBishopActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABlackPiece> BQueenActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ABlackPiece> BKingActor;

	// tile padding dimension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CellPadding;

	// tile size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize;

	// Called when an instance of this class is placed (in editor) or spawned
	virtual void OnConstruction(const FTransform& Transform) override;

	// return a relative position given (x,y) position (moved from protected to public)
	FVector GetRelativeLocationByXYPosition(const double InX, const double InY) const;
	
	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

        // retuns true if the Tile at (x,y) position is empty
	bool TileIsEmpty(const double InX, const double InY);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// generate an empty game field
	void GenerateField();

        // Generate all the Pices
	void GeneratePieces();

	// Generate specific White/Black Pieces
        void GenerateWPawns();
	void GenerateWKing();
	void GenerateWQueen();
	void GenerateWBishops();
	void GenerateWKnights();
	void GenerateWRooks();

	void GenerateBPawns();
	void GenerateBKing();
	void GenerateBQueen();
	void GenerateBBishops();
	void GenerateBKnights();
	void GenerateBRooks();

	// return a (x,y) position given a hit (click) on a field tile
	FVector2D GetPosition(const FHitResult& Hit);

	// return the array of tile pointers
	TArray<ATile*>& GetTileArray();

};
