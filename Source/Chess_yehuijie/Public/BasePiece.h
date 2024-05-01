
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePiece.generated.h"

// Enumerator class thah helps keeping track of which type of Piece we're reffering to
UENUM()
enum class EPiece : uint8 
{
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King,
	None,
};

// Enumerator class that keeps track of whether a Piece was Clicked by a Player or not
UENUM()
enum class EPieceStatus : uint8
{
	Clicked,
	NotClicked,
};

// Enumerator class that keeps track of whether the suggested Moves of the CLicked Piece are Spawned or not
UENUM()
enum class EPieceMoves : uint8
{
	Spawned,
	NotSpawned,
};

// Enumerator class that keeps track of whether a Piece is in a Position that could cause it to be Eaten by another Piece
UENUM()
enum class EPieceToEat : uint8
{
	ToBeEaten,
	NotToBeEaten,	
};

// Enumerator class that keeps track of whether a Piece is currently on the Board or not
UENUM()
enum class EPieceOnBoard : uint8
{
	NotOnBoard,
	OnBoard,
};

// Declaration of ABasePiece Class, which inherits from AActor
UCLASS()
class CHESS_YEHUIJIE_API ABasePiece : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABasePiece();

	// Set and Get the (x, y) Position
	void SetBoardPosition(const double InX, const double InY);
	FVector2D GetBoardPosition();

        // Set and Get the Type of the Piece (Pawn, Rook, etc)
	void SetPiece(const EPiece PieceType);
	EPiece GetPiece();

        // Set and Get Clicked or NotClicked Piece
	void SetPieceStatus(const EPieceStatus Status);
	EPieceStatus GetPieceStatus();

	// Set and Get Current (x,y) Position of a Piece that is moving (current Position will be the Old Position after moving)
        void SetOldPosition(const double InX, const double InY);
	FVector2D GetOldPosition();

        // Set and Get whether Movement Tiles are Spawned or not
	void SetPieceMoves(const EPieceMoves Moves);
	EPieceMoves GetPieceMoves();

	// Set and Get whether a Piece is a threatened Position or not
        void SetPieceToEat(const EPieceToEat ToBeEaten);
	EPieceToEat GetPieceToEat();

        // Set and Get whether a Piece is currently on Board or not
	void SetIsPieceOnBoard(const EPieceOnBoard OnBoard);
	EPieceOnBoard GetIsPieceOnBoard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// (x, y) Position of the Piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D BoardPosition;

        // (x,y) Current Position of the Piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D OldPosition;

	// Type of the Piece
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPiece Piece;

        // Clicked/NotClicked
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPieceStatus PieceStatus;

	// if the Movement Tiler are Spawned or Not
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPieceMoves PieceMoves;

	// is the Piece possibly getting eaten
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPieceToEat PieceToEat;

	// whether or not the Piece is Currently on the Board
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPieceOnBoard IsPieceOnBoard;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// funzione copia-incollata dal TTT, è rimasta perche non ho fatto in tempo a levarla quando ero in Lab e non volevo compromettere in alcun modo il codice
	UFUNCTION()
	void SelfDestroy();

};
