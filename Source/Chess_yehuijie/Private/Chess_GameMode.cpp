// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Chess_HumanPlayer.h"
#include "Chess_RandomPlayer.h"
#include "GameField.h"
//#include "Chess_MinimaxPlayer.h"
#include "EngineUtils.h"

AChess_GameMode::AChess_GameMode()
{
	PlayerControllerClass = AChess_PlayerController::StaticClass();
	DefaultPawnClass = AChess_HumanPlayer::StaticClass();
	FieldSize = 8;
}

void AChess_GameMode::BeginPlay()
{
	Super::BeginPlay();

	IsGameOver = false;

	MoveCounter = 0;

	AChess_HumanPlayer* HumanPlayer = Cast<AChess_HumanPlayer>(*TActorIterator<AChess_HumanPlayer>(GetWorld()));

	if (GameFieldClass != nullptr)
	{
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0
	Players.Add(HumanPlayer);
	// Random Player
	auto* AI = GetWorld()->SpawnActor<AChess_RandomPlayer>(FVector(), FRotator());

	// MiniMax Player
	//auto* AI = GetWorld()->SpawnActor<ATTT_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(AI);

	this->ChoosePlayerAndStartGame();
}

void AChess_GameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayer = 0; // 0 = human and always starts

	MoveCounter += 1;
	Players[CurrentPlayer]->OnTurn();
}

void AChess_GameMode::ShowMoves(const FVector2D& Position, EPiece Piece)
{
	if (Piece == EPiece::Pawn)
	{
		//while x < 6 && empty
		Position[0] ++;
		SpawnMovTile(Position)
		if (Position[1] == 1)
		{
			Position[0] ++;
		        SpawnMovTile(Position)
		}
	}
	if (Piece == EPiece::Rook)
	{
		//while empty
		for (int32 x = Position[0]; x < GField->Size; x++)
		{
			SpawnMovTile(FVector2D(x, Position[1]);
		}
		for (int32 x = Position[0]; x > 0; x--)
		{
			SpawnMovTile(FVector2D(x, Position[1]);
		}
		for (int32 y = Position[1]; y < GField->Size; y++)
		{
			SpawnMovTile(FVector2D(Position[0], y);
		}
		for (int32 y = Position[1]; y > 0; y--)
		{
			SpawnMovTile(FVector2D(Position[0], y);
		}
	}
}

void AChess_GameMode::SpawnMovTile(const FVector2D& Position/*, EPiece Piece*/)
{
	//if (Piece == EPiece::Pawn)
	//{
		int32 x = Position[0];// + 1;
		int32 y = Position[1];
		//FVector2D XYPos = Position;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x=%f,y=%f"), XYPos[0], XYPos[1]));
		
		FVector Location = GField->GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, -5);
		ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClassYellow, Location, FRotator::ZeroRotator);
		MoveTileArray.Add(Obj);
		MoveTileMap.Add(FVector2D(x, y), Obj);
		Obj->SetTileStatus(0 , ETileStatus::MOVEABLE);
	//}
}

void AChess_GameMode::DestroyMoveTiles()
{
	for (ATile* Obj : MoveTileArray)
	{
		Obj->Destroy();
	}
}

//ChangeActorLocation !!!!!
/*void AChess_GameMode::DestroyClickedPiece()
{
	for (AWhitePiece* Obj : GField->WPawnArray)
	{
		if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
		{
			Obj->Destroy();
			GField->WPawnArray.Remove(Obj);
			GField->WPawnMap.Remove(Obj->GetBoardPosition());
			break;
		}
	}
}*/

//ChangeActorLocation !!!!!
/*void AChess_GameMode::SpawnPiece(const FVector2D& Position)
{
	int32 x = Position[0];
	int32 y = Position[1];
	FVector Location = GField->GetRelativeLocationByXYPosition(x, y) + FVector(0, 0, 5);
	AWhitePiece* Obj = GetWorld()->SpawnActor<AWhitePiece>(WPawnActor, Location, FRotator(0, 90, 0));
	const float PawnScale = GField->TileSize / 110;
	Obj->SetActorScale3D(FVector(PawnScale, PawnScale, 0.2));
	Obj->SetPiece(EPiece::Pawn);
	Obj->SetPieceStatus(EPieceStatus::NotClicked);
	Obj->SetBoardPosition(x, y);
	GField->WPawnArray.Add(Obj);
	GField->WPawnMap.Add(FVector2D(x, y), Obj);
}*/

/*void AChess_GameMode::MovePiece(const FVector2D& Position, EPiece Piece)
{
	DestroyMoveTiles();
	DestroyClickedPiece();
	SpawnPiece(Position, Piece);
}*/

/*void AChess_GameMode::SetCellSign(const int32 PlayerNumber, const FVector& SpawnPosition)
{
	if (IsGameOver || PlayerNumber != CurrentPlayer)
	{
		return;
	}

	UClass* SignActor = Players[CurrentPlayer]->Sign == ESign::X ? SignXActor : SignOActor;
	FVector Location = GField->GetActorLocation() + SpawnPosition + FVector(0, 0, 10);
	GetWorld()->SpawnActor(SignActor, &Location);

	if (GField->IsWinPosition(GField->GetXYPositionByRelativeLocation(SpawnPosition)))
	{
		IsGameOver = true;
		Players[CurrentPlayer]->OnWin();
		for (int32 i = 0; i < Players.Num(); i++)
		{
			if (i != CurrentPlayer)
			{
				Players[i]->OnLose();
			}
		}
	}
	else if (MoveCounter == (FieldSize * FieldSize))
	{
		// add a timer (3 seconds)
		FTimerHandle TimerHandle;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				// function to delay
				GField->ResetField();
			}, 3, false);
	}
	else
	{
		TurnNextPlayer();
	}
}*/

int32 AChess_GameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

void AChess_GameMode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
}
