// Fill out your copyright notice in the Description page of Project Settings.

#include "GameField.h"
#include "Kismet/GameplayStatics.h"
#include "BasePiece.h"
#include "Tile.h"
#include "Chess_GameMode.h"

// Sets default values
AGameField::AGameField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// size of the field (3x3)
	Size = 8;
	// tile dimension
	TileSize = 120;
	// tile padding dimension
	CellPadding = 5;

}

void AGameField::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//normalized tilepadding
	NormalizedCellPadding = FMath::RoundToDouble(((TileSize + CellPadding) / TileSize) * 100) / 100;
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
	GeneratePieces();
}

/*void AGameField::ResetField()
{
	for (ATile* Obj : TileArray)
	{
		Obj->SetTileStatus(NOT_ASSIGNED, ETileStatus::EMPTY);
	}

	// send broadcast event to registered objects 
	//OnResetEvent.Broadcast();

	//ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());
	//GameMode->IsGameOver = false;
	//GameMode->MoveCounter = 0;
	//GameMode->ChoosePlayerAndStartGame();
}*/

void AGameField::GenerateField()
{
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = nullptr;

			if ((x + y) % 2 == 0) 
			{
				Obj = GetWorld()->SpawnActor<ATile>(TileClassBlack, Location, FRotator::ZeroRotator);
				const float TileScale = TileSize / 100;
				Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
				Obj->SetGridPosition(x, y);
				TileArray.Add(Obj);
				TileMap.Add(FVector2D(x, y), Obj);
				
			}
			
			if ((x + y) % 2 == 1) 
			{
				Obj = GetWorld()->SpawnActor<ATile>(TileClassWhite, Location, FRotator::ZeroRotator);
				const float TileScale = TileSize / 100;
				Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
				Obj->SetGridPosition(x, y);
				TileArray.Add(Obj);
				TileMap.Add(FVector2D(x, y), Obj);
			}

			if (x == 0 || x == 1)
			{
				Obj->SetTileStatus(0, ETileStatus::OCCUPIED);
			}

			if (x == 6 || x == 7)
			{
				Obj->SetTileStatus(1, ETileStatus::OCCUPIED);
			}
		}
		 
	}
}

void AGameField::GeneratePieces()
{
	GenerateWPawns();
	GenerateWKing();
	GenerateWQueen();
	GenerateWRooks();
	GenerateWKnights();
	GenerateWBishops();
	GenerateBBishops();
	GenerateBRooks();
	GenerateBKnights();
	GenerateBQueen();
	GenerateBPawns();
	GenerateBKing();

}

void AGameField::GenerateWPawns()
{
	int32 x = 1;
	for (int32 y = 0; y < Size; y++) 
	{
		FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
		ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WPawnActor, Location, FRotator(0, 90, 0));
		const float PawnScale = TileSize / 110;
		Obj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		Obj->SetPiece(EPiece::Pawn);
		Obj->SetPieceStatus(EPieceStatus::NotClicked);
		Obj->SetPieceColor(EPieceColor::White);
		Obj->SetBoardPosition(x, y);
		WPiecesArray.Add(Obj);
		WPiecesMap.Add(FVector2D(x, y), Obj);
	}
}

void AGameField::GenerateWKing()
{
	int32 x = 0;
	int32 y = 4;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WKingActor, Location, FRotator(0, 90, 0));
	const float KingScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KingScale, KingScale, 0.2));
	Obj->SetPiece(EPiece::King);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, y);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, y), Obj);
}

void AGameField::GenerateWQueen()
{
	int32 x = 0;
	int32 y = 3;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WQueenActor, Location, FRotator(0, 90, 0));
	const float QueenScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(QueenScale, QueenScale, 0.2));
	Obj->SetPiece(EPiece::Queen);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, y);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, y), Obj);
}

void AGameField::GenerateWBishops()
{
	int32 x = 0;
	int32 leftY = 2;
	int32 rightY = 5;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WBishopActor, Location, FRotator(0, 90, 0));
	const float BishopScale = TileSize / 110;	
	Obj->SetActorScale3D(FVector(BishopScale, BishopScale, 0.2));
	Obj->SetPiece(EPiece::Bishop);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, leftY);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(WBishopActor, Location1, FRotator(0, 90, 0));
	Obj1->SetActorScale3D(FVector(BishopScale, BishopScale, 0.2));
	Obj1->SetPiece(EPiece::Bishop);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, rightY);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, rightY), Obj1);
}

void AGameField::GenerateWKnights()
{
	int32 x = 0;
	int32 leftY = 1;
	int32 rightY = 6;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WKnightActor, Location, FRotator(0, 90, 0));
	const float KnightScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj->SetPiece(EPiece::Knight);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, leftY);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(WKnightActor, Location1, FRotator(0, 90, 0));
	Obj1->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj1->SetPiece(EPiece::Knight);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, rightY);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, rightY), Obj1);

}

void AGameField::GenerateWRooks()
{
	int32 x = 0;
	int32 leftY = 0;
	int32 rightY = 7;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
    ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WRookActor, Location, FRotator(0, 90, 0));
	const float RookScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj->SetPiece(EPiece::Rook);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, leftY);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
        ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(WRookActor, Location1, FRotator(0, 90, 0));
	Obj1->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj1->SetPiece(EPiece::Rook);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::White);
	Obj->SetBoardPosition(x, rightY);
	WPiecesArray.Add(Obj);
	WPiecesMap.Add(FVector2D(x, rightY), Obj1);
}

void AGameField::GenerateBPawns()
{
	int32 x = 6;
	for (int32 y = 0; y < Size; y++)
	{
		FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
		ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BPawnActor, Location, FRotator(0, -90, 0));
		const float PawnScale = TileSize / 110;
		Obj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		Obj->SetPiece(EPiece::Pawn);
		Obj->SetPieceStatus(EPieceStatus::NotClicked);
	    Obj->SetPieceColor(EPieceColor::Black);
		Obj->SetBoardPosition(x, y);
		BPiecesArray.Add(Obj);
	    BPiecesMap.Add(FVector2D(x, y), Obj);
	}
}

void AGameField::GenerateBKing() 
{
	int32 x = 7;
	int32 y = 4;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BKingActor, Location, FRotator(0, -90, 0));
	const float KingScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KingScale, KingScale, 0.2));
	Obj->SetPiece(EPiece::King);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, y);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, y), Obj);

}

void AGameField::GenerateBQueen()
{
	int32 x = 7;
	int32 y = 3;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BQueenActor, Location, FRotator(0, -90, 0));
	const float QueenScale = TileSize / 110;
    Obj->SetActorScale3D(FVector(QueenScale, QueenScale, 0.2));
	Obj->SetPiece(EPiece::Queen);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, y);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, y), Obj);

}

void AGameField::GenerateBBishops()
{
	int32 x = 7;
	int32 leftY = 2;
	int32 rightY = 5;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BBishopActor, Location, FRotator(0, -90, 0));
	const float BishopScale = TileSize / 110;
	Obj->SetPiece(EPiece::Bishop);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, leftY);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(BBishopActor, Location1, FRotator(0, -90, 0));
	Obj1->SetActorScale3D(FVector(BishopScale, BishopScale, 0.2));
	Obj1->SetPiece(EPiece::Bishop);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, rightY);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, rightY), Obj1);
}

void AGameField::GenerateBKnights()
{
	int32 x = 7;
	int32 leftY = 1;
	int32 rightY = 6;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BKnightActor, Location, FRotator(0, -90, 0));
	const float KnightScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj->SetPiece(EPiece::Knight);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, leftY);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(BKnightActor, Location1, FRotator(0, -90, 0));
	Obj1->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj1->SetPiece(EPiece::Knight);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, rightY);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, rightY), Obj1);

}

void AGameField::GenerateBRooks()
{
	int32 x = 7;
	int32 leftY = 0;
	int32 rightY = 7;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BRookActor, Location, FRotator(0, -90, 0));
	const float RookScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj->SetPiece(EPiece::Rook);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, leftY);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(BRookActor, Location1, FRotator(0, -90, 0));
	Obj1->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj1->SetPiece(EPiece::Rook);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetPieceColor(EPieceColor::Black);
	Obj->SetBoardPosition(x, rightY);
	BPiecesArray.Add(Obj);
	BPiecesMap.Add(FVector2D(x, rightY), Obj1);

}

FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

TArray<ATile*>& AGameField::GetTileArray()
{
return TileArray;
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

/*FVector AGameField::GetRelativePawnLocationByXYPosition(const int32 InX, const int32 InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0.2);
}*/


FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}





/*bool AGameField::IsWinPosition(const FVector2D Position) const
{

	return false;
}

inline bool AGameField::IsValidPosition(const FVector2D Position) const
{
	return false;
}*/




// Called every frame
//void AGameField::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

