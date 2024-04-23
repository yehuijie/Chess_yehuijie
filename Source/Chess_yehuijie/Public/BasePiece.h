// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePiece.generated.h"

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

UENUM()
enum class EPieceStatus : uint8
{
	Clicked,
	NotClicked,
};

UENUM()
enum class EPieceColor : uinit8
{
        White,
        Black,
};

UCLASS()
class CHESS_YEHUIJIE_API ABasePiece : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABasePiece();

	// set the (x, y) position
	void SetBoardPosition(const double InX, const double InY);

	// get the (x, y) position
	FVector2D GetBoardPosition();

	void SetPiece(const EPiece PieceType);

	EPiece GetPiece();

	void SetPieceStatus(const EPieceStatus Status);

	EPieceStatus GetPieceStatus();

        void SetPieceColor(const EPieceColor Color);

        EPieceColor GetPieceColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D BoardPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPiece Piece;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPieceStatus PieceStatus;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// destroy a base sign actor
	UFUNCTION()
	void SelfDestroy();

};
