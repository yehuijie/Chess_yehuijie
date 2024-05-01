// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

//probabilmente non va bene, bisogna controllare sintassi e funzionamneto --> pero potrebbe essere utile?
UENUM()
enum class ETileStatus : uint8
{
	EMPTY     UMETA(DisplayName = "Empty"),
	OCCUPIED      UMETA(DisplayName = "Occupied"),
	MOVE   UMETA(DisplayName = "Move"),
};

UENUM()
enum class EIsCheckByBlacks : uint8
{
	Checked,
	NotChecked,
};

UENUM()
enum class EIsCheckByWhites : uint8
{
	Checked,
	NotChecked,
};

UCLASS()
class CHESS_YEHUIJIE_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	// set the player owner and the status of a tile
	void SetTileStatus(const ETileStatus TileStatus);
	// get the tile status
	ETileStatus GetTileStatus();

	// set the (x, y) position
	void SetGridPosition(const double InX, const double InY);
	// get the (x, y) position
	FVector2D GetGridPosition();

	void SetIsCheckByBlacks(const EIsCheckByBlacks IsCheck);
	EIsCheckByBlacks GetIsCheckbyBlacks();

	void SetIsCheckByWhites(const EIsCheckByWhites IsCheck);
	EIsCheckByWhites GetIsCheckbyWhites();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETileStatus Status;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileGridPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EIsCheckByBlacks IsCheckedB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EIsCheckByWhites IsCheckedW;
};
