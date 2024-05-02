


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

	// initialisation 
	BoardPosition = FVector2D(0, 0);
	OldPosition = FVector2D(0, 0);
	Piece = EPiece::None;
	PieceStatus = EPieceStatus::NotClicked;
	PieceMoves = EPieceMoves::NotSpawned;
	PieceToEat = EPieceToEat::NotToBeEaten;
	IsPieceOnBoard = EPieceOnBoard::OnBoard;
}

// Set and Get the (x, y) Position
void ABasePiece::SetBoardPosition(const double InX, const double InY)
{
	BoardPosition.Set(InX, InY);
}
FVector2D ABasePiece::GetBoardPosition()
{
	return BoardPosition;
}

// Set and Get the Type of the Piece (Pawn, Rook, etc)
void ABasePiece::SetPiece(const EPiece PieceType)
{
	Piece = PieceType;
}
EPiece ABasePiece::GetPiece()
{
	return Piece;
}

// Set and Get Clicked or NotClicked Piece
void ABasePiece::SetPieceStatus(const EPieceStatus Status)
{
	PieceStatus = Status;
}
EPieceStatus ABasePiece::GetPieceStatus()
{
	return PieceStatus;
}

// Set and Get Current (x,y) Position of a Piece that is moving (current Position will be the Old Position after moving)
void ABasePiece::SetOldPosition(const double InX, const double InY)
{
	OldPosition.Set(InX, InY);
}
FVector2D ABasePiece::GetOldPosition()
{
	return OldPosition;
}

// Set and Get Current (x,y) Position of a Piece that is moving (current Position will be the Old Position after moving)
void ABasePiece::SetPieceMoves(const EPieceMoves Moves)
{
	PieceMoves = Moves;
}
EPieceMoves ABasePiece::GetPieceMoves()
{
	return PieceMoves;
}

// Set and Get whether a Piece is a threatened Position or not
void ABasePiece::SetPieceToEat(const EPieceToEat ToBeEaten)
{
	PieceToEat = ToBeEaten;
}
EPieceToEat ABasePiece::GetPieceToEat()
{
	return PieceToEat;
}

// Set and Get whether a Piece is currently on Board or not
void ABasePiece::SetIsPieceOnBoard(const EPieceOnBoard OnBoard)
{
	IsPieceOnBoard = OnBoard;
}
EPieceOnBoard ABasePiece::GetIsPieceOnBoard()
{
	return IsPieceOnBoard;
}

// Called when the game starts or when spawned
void ABasePiece::BeginPlay()
{
	Super::BeginPlay();
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void ABasePiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// funzione copia-incollata dal TTT, è rimasta perche non ho fatto in tempo a levarla quando ero in Lab e non volevo compromette e in alcun modo il codice
void ABasePiece::SelfDestroy()
{
	Destroy();
}

