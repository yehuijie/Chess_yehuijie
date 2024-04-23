// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "BasePiece.h"
#include "BlackPiece.h"
#include "WhitePiece.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

// macro declaration for a dynamic multicast delegate --> da capire
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NormalizedCellPadding;

	static const int32 NOT_ASSIGNED = -1;

	// BlueprintAssignable Usable with Multicast Delegates only. Exposes the property for assigning in Blueprints.
	// declare a variable of type FOnReset (delegate)
	//UPROPERTY(BlueprintAssignable)
	//FOnReset OnResetEvent; --> maybe?

	// size of field
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Size;

	// size of winning line
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WinSize;

	// TSubclassOf template class that provides UClass type safety
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassWhite;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClassBlack;

	// keeps track of pawns
	UPROPERTY(Transient)
	TArray<AWhitePiece*> WPawnArray;
	UPROPERTY(Transient)
	TMap<FVector2D, AWhitePiece*> WPawnMap;

	// keeps track of pawns
	UPROPERTY(Transient)
	TArray<AWhitePiece*> WKingArray;
	UPROPERTY(Transient)
	TMap<FVector2D, AWhitePiece*> WKingMap;
	
	UPROPERTY(Transient)
	TArray<AWhitePiece*> WQueenArray;
	UPROPERTY(Transient)
	TMap<FVector2D, AWhitePiece*> WQueenMap;
	
	UPROPERTY(Transient)
	TArray<AWhitePiece*> WBishopArray;
	UPROPERTY(Transient)
	TMap<FVector2D, AWhitePiece*> WBishopMap;
	
	UPROPERTY(Transient)
	TArray<AWhitePiece*> WKnightArray;
	UPROPERTY(Transient)
	TMap<FVector2D, AWhitePiece*> WKnightMap;
	
	UPROPERTY(Transient)
	TArray<AWhitePiece*> WRookArray;
	UPROPERTY(Transient)
	TMap<FVector2D, AWhitePiece*> WRookMap;

	UPROPERTY(Transient)
	TArray<ABlackPiece*> BPawnArray;
	UPROPERTY(Transient)
	TMap<FVector2D, ABlackPiece*> BPawnMap;
	
	UPROPERTY(Transient)
	TArray<ABlackPiece*> BKingArray;
	UPROPERTY(Transient)	
	TMap<FVector2D, ABlackPiece*> BKingMap;

	UPROPERTY(Transient)
	TArray<ABlackPiece*> BQueenArray;
	UPROPERTY(Transient)
	TMap<FVector2D, ABlackPiece*> BQueenMap;

	UPROPERTY(Transient)
	TArray<ABlackPiece*> BBishopArray;
	UPROPERTY(Transient)
	TMap<FVector2D, ABlackPiece*> BBishopMap;

	UPROPERTY(Transient)
	TArray<ABlackPiece*> BKnightArray;
	UPROPERTY(Transient)
	TMap<FVector2D, ABlackPiece*> BKnightMap;

	UPROPERTY(Transient)
	TArray<ABlackPiece*> BRookArray;
	UPROPERTY(Transient)
	TMap<FVector2D, ABlackPiece*> BRookMap;

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
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;
	
	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// remove all signs from the field
	//UFUNCTION(BlueprintCallable)
	//void ResetField();

	// generate an empty game field
	void GenerateField();

	void GeneratePieces();

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

	

	
	// check if a position is a win position
	//bool IsWinPosition(const FVector2D Position) const;

	// checking if is a valid field position
	//inline bool IsValidPosition(const FVector2D Position) const;

	//public:	
		// Called every frame
	//	virtual void Tick(float DeltaTime) override;

};
