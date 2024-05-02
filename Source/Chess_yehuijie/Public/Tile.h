
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

// Enumerator class that states if a Tile is Empty/Occupied or is a MovementTile
UENUM()
enum class ETileStatus : uint8
{
	EMPTY     UMETA(DisplayName = "Empty"),
	OCCUPIED      UMETA(DisplayName = "Occupied"),
	MOVE   UMETA(DisplayName = "Move"),
};

// Enumerator class that verifies if a Tile is under Check by Blacks 
UENUM()
enum class EIsCheckByBlacks : uint8
{
	Checked,
	NotChecked,
};

// Enumerator class that verifies if a Tile is under Check by Whites
UENUM()
enum class EIsCheckByWhites : uint8
{
	Checked,
	NotChecked,
};

// Declaration of ATile class, which inherits AActor
UCLASS()
class CHESS_YEHUIJIE_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	// Set/Get the status of a tile (Clicked or NotClicked)
	void SetTileStatus(const ETileStatus TileStatus);
	ETileStatus GetTileStatus();

	// Set/Get the (x, y) position
	void SetGridPosition(const double InX, const double InY);
	FVector2D GetGridPosition();

        // Set/Get whether a Tile is under Check by Blacks or not
	void SetIsCheckByBlacks(const EIsCheckByBlacks IsCheck);
	EIsCheckByBlacks GetIsCheckbyBlacks();
 
        // Set/Get whether a Tile is under Check by Blacks or not
	void SetIsCheckByWhites(const EIsCheckByWhites IsCheck);
	EIsCheckByWhites GetIsCheckbyWhites();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

        // is the Piece Clicked
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETileStatus Status;

	// (x, y) position of the Piece
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileGridPosition;
 
        // whether a Tile is under Check by Blacks
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EIsCheckByBlacks IsCheckedB;

        // whether a Tile is under Check by Blacks
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EIsCheckByWhites IsCheckedW;
};
