
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

// Called when an instance of this class is placed (in editor) or spawned
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
	// Spawn the Board and the Pieces
	GenerateField();
	GeneratePieces();
}

// Spawn the Board
void AGameField::GenerateField()
{
	// Generate a 8x8 Board
	for (int32 x = 0; x < Size; x++)
	{
		for (int32 y = 0; y < Size; y++)
		{
			FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y);
			ATile* Obj = nullptr;
			// Tiles with even IndexSum are Black
			if ((x + y) % 2 == 0) 
			{
				Obj = GetWorld()->SpawnActor<ATile>(TileClassBlack, Location, FRotator::ZeroRotator);
				const float TileScale = TileSize / 100;
				Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
				Obj->SetGridPosition(x, y);
				TileArray.Add(Obj);
				TileMap.Add(FVector2D(x, y), Obj);
			}	
			// Tiles with odd IndexSum are White
			if ((x + y) % 2 == 1) 
			{
				Obj = GetWorld()->SpawnActor<ATile>(TileClassWhite, Location, FRotator::ZeroRotator);
				const float TileScale = TileSize / 100;
				Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
				Obj->SetGridPosition(x, y);
				TileArray.Add(Obj);
				TileMap.Add(FVector2D(x, y), Obj);
			}
			// Set starting Positions where Pieces will be spanwed at to Occupied
			if (x == 0 || x == 1)
			{
				Obj->SetTileStatus(ETileStatus::OCCUPIED);
			}
			if (x == 6 || x == 7)
			{
				Obj->SetTileStatus(ETileStatus::OCCUPIED);
			}
		}	 
	}
}

// Spawn the different Pieces
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
	// Spawn 8 WhitePawns on the Second Row, Scale each of them, Set PieceType to Pawn, Set their (x,y) BoardPosition and Add them to PiecesArray and Map
	for (int32 y = 0; y < Size; y++) 
	{
		FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
		ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WPawnActor, Location, FRotator(0, 90, 0));
		const float PawnScale = TileSize / 110;
		Obj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		Obj->SetPiece(EPiece::Pawn);
		Obj->SetBoardPosition(x, y);
		PiecesArray.Add(Obj);
		PiecesMap.Add(FVector2D(x, y), Obj);
	}
}

void AGameField::GenerateWKing()
{
	// Spawn the WhiteKing, Scale it, Set PieceType to King, Set its (x,y) BoardPosition and Add it to PiecesArray and Map
	int32 x = 0;
	int32 y = 4;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WKingActor, Location, FRotator(0, 90, 0));
	const float KingScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KingScale, KingScale, 0.2));
	Obj->SetPiece(EPiece::King);
	Obj->SetBoardPosition(x, y);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, y), Obj);
}

void AGameField::GenerateWQueen()
{
	// Spawn the WhiteQueen, Scale it, Set PieceType to Queen, Set its (x,y) BoardPosition and Add it to PiecesArray and Map
	int32 x = 0;
	int32 y = 3;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WQueenActor, Location, FRotator(0, 90, 0));
	const float QueenScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(QueenScale, QueenScale, 0.2));
	Obj->SetPiece(EPiece::Queen);
	Obj->SetBoardPosition(x, y);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, y), Obj);
}

void AGameField::GenerateWBishops()
{
	// Spawn the WhiteBishops, Scale them, Set PieceType to Bishop, Set thier (x,y) BoardPosition and Add them to PiecesArray and Map
	int32 x = 0;
	int32 leftY = 2;
	int32 rightY = 5;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WBishopActor, Location, FRotator(0, 90, 0));
	const float BishopScale = TileSize / 110;	
	Obj->SetActorScale3D(FVector(BishopScale, BishopScale, 0.2));
	Obj->SetPiece(EPiece::Bishop);
	Obj->SetBoardPosition(x, leftY);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(WBishopActor, Location1, FRotator(0, 90, 0));
	Obj1->SetActorScale3D(FVector(BishopScale, BishopScale, 0.2));
	Obj1->SetPiece(EPiece::Bishop);
	Obj1->SetBoardPosition(x, rightY);
	PiecesArray.Add(Obj1);
	PiecesMap.Add(FVector2D(x, rightY), Obj1);
}

void AGameField::GenerateWKnights()
{
	// Spawn the WhiteKnights, Scale them, Set PieceType to Knight, Set thier (x,y) BoardPosition and Add them to PiecesArray and Map
	int32 x = 0;
	int32 leftY = 1;
	int32 rightY = 6;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WKnightActor, Location, FRotator(0, 90, 0));
	const float KnightScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj->SetPiece(EPiece::Knight);
	Obj->SetBoardPosition(x, leftY);
	PiecesArray.Add(Obj);
        PiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(WKnightActor, Location1, FRotator(0, 90, 0));
	Obj1->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj1->SetPiece(EPiece::Knight);
	Obj1->SetBoardPosition(x, rightY);
	PiecesArray.Add(Obj1);
	PiecesMap.Add(FVector2D(x, rightY), Obj1);

}

void AGameField::GenerateWRooks()
{
	// Spawn the WhiteRooks, Scale them, Set PieceType to Rook, Set thier (x,y) BoardPosition and Add them to PiecesArray and Map
	int32 x = 0;
	int32 leftY = 0;
	int32 rightY = 7;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
        ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(WRookActor, Location, FRotator(0, 90, 0));
	const float RookScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj->SetPiece(EPiece::Rook);
	Obj->SetBoardPosition(x, leftY);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
        ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(WRookActor, Location1, FRotator(0, 90, 0));
	Obj1->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj1->SetPiece(EPiece::Rook);
	Obj1->SetBoardPosition(x, rightY);
	PiecesArray.Add(Obj1);
	PiecesMap.Add(FVector2D(x, rightY), Obj1);
}

void AGameField::GenerateBPawns()
{
	int32 x = 6;
	// Spawn 8 BlackPawns on the Sixth Row, Scale each of them, Set PieceType to Pawn, Set their (x,y) BoardPosition and Add them to PiecesArray and Map
	for (int32 y = 0; y < Size; y++)
	{
		FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
		ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BPawnActor, Location, FRotator(0, -90, 0));
		const float PawnScale = TileSize / 110;
		Obj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
		Obj->SetPiece(EPiece::Pawn);
		Obj->SetBoardPosition(x, y);
		PiecesArray.Add(Obj);
	        PiecesMap.Add(FVector2D(x, y), Obj);
	}
}

void AGameField::GenerateBKing() 
{
	// Spawn the BlackKing, Scale it, Set PieceType to King, Set its (x,y) BoardPosition and Add it to PiecesArray and Map
	int32 x = 7;
	int32 y = 4;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BKingActor, Location, FRotator(0, -90, 0));
	const float KingScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KingScale, KingScale, 0.2));
	Obj->SetPiece(EPiece::King);
	Obj->SetBoardPosition(x, y);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, y), Obj);

}

void AGameField::GenerateBQueen()
{
	// Spawn the BlackQueen, Scale it, Set PieceType to Queen, Set its (x,y) BoardPosition and Add it to PiecesArray and Map
	int32 x = 7;
	int32 y = 3;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BQueenActor, Location, FRotator(0, -90, 0));
	const float QueenScale = TileSize / 110;
        Obj->SetActorScale3D(FVector(QueenScale, QueenScale, 0.2));
	Obj->SetPiece(EPiece::Queen);
	Obj->SetBoardPosition(x, y);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, y), Obj);

}

void AGameField::GenerateBBishops()
{
	// Spawn the BlackBishops, Scale them, Set PieceType to Bishop, Set thier (x,y) BoardPosition and Add them to PiecesArray and Map
	int32 x = 7;
	int32 leftY = 2;
	int32 rightY = 5;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BBishopActor, Location, FRotator(0, -90, 0));
	const float BishopScale = TileSize / 110;
	Obj->SetPiece(EPiece::Bishop);
	Obj->SetBoardPosition(x, leftY);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(BBishopActor, Location1, FRotator(0, -90, 0));
	Obj1->SetActorScale3D(FVector(BishopScale, BishopScale, 0.2));
	Obj1->SetPiece(EPiece::Bishop);
	Obj1->SetBoardPosition(x, rightY);
	PiecesArray.Add(Obj1);
	PiecesMap.Add(FVector2D(x, rightY), Obj1);
}

void AGameField::GenerateBKnights()
{
	// Spawn the BlackKnights, Scale them, Set PieceType to Knight, Set thier (x,y) BoardPosition and Add them to PiecesArray and Map
	int32 x = 7;
	int32 leftY = 1;
	int32 rightY = 6;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BKnightActor, Location, FRotator(0, -90, 0));
	const float KnightScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj->SetPiece(EPiece::Knight);
	Obj->SetBoardPosition(x, leftY);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(BKnightActor, Location1, FRotator(0, -90, 0));
	Obj1->SetActorScale3D(FVector(KnightScale, KnightScale, 0.2));
	Obj1->SetPiece(EPiece::Knight);
	Obj1->SetBoardPosition(x, rightY);
        PiecesArray.Add(Obj1);
	PiecesMap.Add(FVector2D(x, rightY), Obj1);

}

void AGameField::GenerateBRooks()
{
	// Spawn the BlackRooks, Scale them, Set PieceType to Rook, Set thier (x,y) BoardPosition and Add them to PiecesArray and Map
	int32 x = 7;
	int32 leftY = 0;
	int32 rightY = 7;

	FVector Location = AGameField::GetRelativeLocationByXYPosition(x, leftY) + FVector(0, 0, 10);
	ABasePiece* Obj = GetWorld()->SpawnActor<ABasePiece>(BRookActor, Location, FRotator(0, -90, 0));
	const float RookScale = TileSize / 110;
	Obj->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj->SetPieceToEat(EPieceToEat::NotToBeEaten);
	PiecesArray.Add(Obj);
	PiecesMap.Add(FVector2D(x, leftY), Obj);

	FVector Location1 = AGameField::GetRelativeLocationByXYPosition(x, rightY) + FVector(0, 0, 10);
	ABasePiece* Obj1 = GetWorld()->SpawnActor<ABasePiece>(BRookActor, Location1, FRotator(0, -90, 0));
	Obj1->SetActorScale3D(FVector(RookScale, RookScale, 0.2));
	Obj1->SetPiece(EPiece::Rook);
	Obj1->SetBoardPosition(x, rightY);
	PiecesArray.Add(Obj1);
	PiecesMap.Add(FVector2D(x, rightY), Obj1);

}

// return a (x,y) position given a hit (click) on a field tile
FVector2D AGameField::GetPosition(const FHitResult& Hit)
{
	return Cast<ATile>(Hit.GetActor())->GetGridPosition();
}

// return the array of tile pointers
TArray<ATile*>& AGameField::GetTileArray()
{
return TileArray;
}

 // retuns true if the Tile at (x,y) position is empty
bool AGameField::TileIsEmpty(const double InX, const double InY)
{
	for (ATile* CurrTile : TileArray)
	{
		if (CurrTile->GetGridPosition() == FVector2D(InX, InY))
		{
			if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
			{
				return true;
			}
		}
	}
	return false;
}

// return a relative position given (x,y) position 
FVector AGameField::GetRelativeLocationByXYPosition(const double InX, const double InY) const
{
	return TileSize * NormalizedCellPadding * FVector(InX, InY, 0);
}

// return (x,y) position given a relative position
FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const
{
	const double x = Location[0] / (TileSize * NormalizedCellPadding);
	const double y = Location[1] / (TileSize * NormalizedCellPadding);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
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

