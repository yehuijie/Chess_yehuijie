// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Gamefield.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	Status = ETileStatus::EMPTY;
	// 0 is white, 1 is black
	//PlayerColor = -1;
	TileGridPosition = FVector2D(0, 0);
	IsCheckedB = EIsCheckByBlacks::NotChecked;
	IsCheckedW = EIsCheckByWhites::NotChecked;
}

void ATile::SetTileStatus( const ETileStatus TileStatus)
{
	
	Status = TileStatus;
}

ETileStatus ATile::GetTileStatus()
{
	return Status;
}


void ATile::SetGridPosition(const double InX, const double InY)
{
	TileGridPosition.Set(InX, InY);
}

FVector2D ATile::GetGridPosition()
{
	return TileGridPosition;
}

void ATile::SetIsCheckByBlacks(const EIsCheckByBlacks IsCheck)
{
	IsCheckedB = IsCheck;
}

EIsCheckByBlacks ATile::GetIsCheckbyBlacks()
{
	return IsCheckedB;
}

void ATile::SetIsCheckByWhites(const EIsCheckByWhites IsCheck)
{
	IsCheckedW = IsCheck;
}

EIsCheckByWhites ATile::GetIsCheckbyWhites()
{
	return IsCheckedW;
}





// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
//void ATile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

