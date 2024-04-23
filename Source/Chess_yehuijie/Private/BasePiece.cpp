// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePiece.h"
#include "GameField.h"
#include "Chess_GameMode.h"

// Sets default values
ABasePiece::ABasePiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// template function that creates a components
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// every actor has a RootComponent that defines the transform in the World
	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	BoardPosition = FVector2D(0, 0);
	Piece = EPiece::None;

}

void ABasePiece::SetBoardPosition(const double InX, const double InY)
{
	BoardPosition.Set(InX, InY);
}

FVector2D ABasePiece::GetBoardPosition()
{
	return BoardPosition;
}

void ABasePiece::SetPiece(const EPiece PieceType)
{
	Piece = PieceType;
}

EPiece ABasePiece::GetPiece()
{
	return Piece;
}

// Called when the game starts or when spawned
void ABasePiece::BeginPlay()
{
	Super::BeginPlay();
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	//GameMode->GField->OnResetEvent.AddDynamic(this, &ABaseSign::SelfDestroy);

}


// Called every frame
void ABasePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePiece::SelfDestroy()
{
	Destroy();
}

