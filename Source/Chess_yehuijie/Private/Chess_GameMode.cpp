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

void AChess_GameMode::ShowMoves(const FVector2D& PiecePosition, EPiece Piece)
{

	if (Piece == EPiece::Pawn)
	{
		FVector2D NextPosition = FVector2D(PiecePosition[0] + 1, PiecePosition[1]);
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] - 1);
		if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
		{
			SpawnMovTile(NextPosition, PiecePosition);
			
			if (PiecePosition[0] == 1 && GField->TileIsEmpty(NextPosition[0] + 1, NextPosition[1]))
			{
				SpawnMovTile(FVector2D(NextPosition[0] + 1, NextPosition[1]), PiecePosition);
				
			}

		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceColor(EPieceColor::Black, EatingMove1))
		{
			SpawnEatTile(EatingMove1, PiecePosition);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceColor(EPieceColor::Black, EatingMove2))
		{
			SpawnEatTile(EatingMove2, PiecePosition);
		}
		//while x < 6 && empty
		// if white spawn, if black add to array for randmove
		
	}
	if (Piece == EPiece::Rook)
	{
		//while empty
		for (int32 x = 0; x < GField->Size; x++)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			} else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceColor(EPieceColor::Black, FVector2D(x, PiecePosition[1]))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size;
			} else
			{
				x = GField->Size;
			}
		}
		for (int32 y = 0; y < GField->Size; y++)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y)
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			} else if (!(GField->TileIsEmpty(PiecePosition[0], y) && IsPieceColor(EPieceColor::Black, FVector2D(PiecePosition[0], y)
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size;
			} else
		        {
				y = GField->Size;
			}
		}
		
		/*for (int32 x = PiecePosition[0] + 1; x < GField->Size; x++)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			} else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceColor(EPieceColor::Black, FVector2D(x, PiecePosition[1]))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size;
			} else
			{
				x = GField->Size;
			}
		}
		for (int32 x = PiecePosition[0] - 1; x > 0; x--)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			} else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceColor(EPieceColor::Black, FVector2D(x, PiecePosition[1]))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = 0;
			} else 
			{
				x = 0;
			}
		}
		for (int32 y = PiecePosition[1] + 1; y < GField->Size; y++)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y)
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			} else if (!(GField->TileIsEmpty(PiecePosition[0], y) && IsPieceColor(EPieceColor::Black, FVector2D(PiecePosition[0], y)
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size;
			} else
		        {
				y = GField->Size;
			}
		}
		for (int32 y = PiecePosition[1] - 1; y > 0; y--)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y)
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			} else if (!(GField->TileIsEmpty(PiecePosition[0], y) && IsPieceColor(EPieceColor::Black, FVector2D(PiecePosition[0], y)
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = 0;
			} else
		        {
				y = 0;
			}
		}*/
	}
        if (Piece == EPiece::Bishop)
	{
		for (int32 x = 0; x < GField->Size; x++)
		{
			for (int32 y = 0; y < GField->Size; y++)
			{
				if ((x - y) == PiecePosition[0] - PiecePosition[1])
			{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
					} else if (!(GField->TileIsEmpty(x, y) && IsPieceColor(EPieceColor::Black, FVector2D(x, y))
              	                        {
			                        SpawnEatTile(FVector2D(x, y), PiecePosition);
		 	                        x = GField->Size;
					} else
		                        {
				                x = GField->Size;
			                }
					
				}
			}
		}
		for (int32 x = 0; x < GField->Size; x++)
		{
			for (int32 y = 0; y < GField->Size; y++)
			{
				if ((x + y) == PiecePosition[0] + PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
					} else if (!(GField->TileIsEmpty(x, y) && IsPieceColor(EPieceColor::Black, FVector2D(x, y))
					{
				                SpawnEatTile(FVector2D(x, y), PiecePosition);
			                        x = GField->Size;
			                } else
		                        {
						x = GField->Size;
		                        }						
				}
			}
		}	
	}
        if (Piece = EPiece::Knight)
	{
		TArray<FVector2D> KnightLMoves;
		KnightLMoves.Add(FVector2D(2, 1));
		KnightLMoves.Add(FVector2D(2, -1));
		KnightLMoves.Add(FVector2D(-2, 1));
		KnightLMoves.Add(FVector2D(-2, -1));
		KnightLMoves.Add(FVector2D(1, 2));
		KnightLMoves.Add(FVector2D(1, -2));
		KnightLMoves.Add(FVector2D(-1, 2));
		KnightLMoves.Add(FVector2D(-1, -2));

		for (const FVector2D& Move : KnightLMoves)
		{
			//if pos0+move0<... and pos1+move1<... etc
		}
	}
	if (Piece == EPiece::King)
	{
		//empty --> no while perche si puo muovere intorno, attenzione agli scacchi!!
		// particolare, non puo andare sotto scacco --> etilestatus ischecked
		SpawnMovTile(FVector2D(PiecePosition[0] +1, PiecePosition[1]), PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0] +1, PiecePosition[1] +1),PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0], PiecePosition[1] +1), PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0] -1, PiecePosition[1] +1), PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0] -1, PiecePosition[1]), PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0] -1, PiecePosition[1] -1), PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0], PiecePosition[1] -1), PiecePosition);
		SpawnMovTile(FVector2D(PiecePosition[0] +1, PiecePosition[1] -1), PiecePosition);
	}
	
}

void AChess_GameMode::SetPieceMovesToSpawned(const FVector2D& Position)
{
	for (ABasePiece* Obj : GField->WPiecesArray)
	{
		if (Obj->GetBoardPosition() == Position)
		{
			Obj->SetPieceMoves(EPieceMoves::Spawned);
		}
	}
}

void AChess_GameMode::SpawnMovTile(const FVector2D& TilePosition, const FVector2D& PiecePosition/*, EPiece Piece*/)
{
	//if (Piece == EPiece::Pawn)
	//{
		
		//FVector2D XYPos = Position;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("x=%d,y=%d"), x, y));
		
		FVector Location = GField->GetRelativeLocationByXYPosition(TilePosition[0], TilePosition[1]) + FVector(0, 0, -5);
		ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClassYellow, Location, FRotator::ZeroRotator);
		SpawnedTileArray.Add(Obj);
		SpawnedTileMap.Add(FVector2D(TilePosition[0], TilePosition[1]), Obj);
		Obj->SetGridPosition(TilePosition[0], TilePosition[1]);
		Obj->SetTileStatus(0 , ETileStatus::MOVEABLE);
		SetPieceMovesToSpawned(PiecePosition);
	//}
}

void AChess_GameMode::SpawnEatTile(const FVector2D& TilePosition, const FVector2D& PiecePosition)
{
	FVector Location = GField->GetRelativeLocationByXYPosition(TilePosition[0], TilePosition[1]) + FVector(0, 0, -5);
	ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClassRed, Location, FRotator::ZeroRotator);
	SpawnedTileArray.Add(Obj);
	SpawnedTileMap.Add(FVector2D(TilePosition[0], TilePosition[1]), Obj);
	Obj->SetGridPosition(TilePosition[0], TilePosition[1]);
	//Obj->SetTileStatus(0, ETileStatus::MOVEABLE);
	SetPieceMovesToSpawned(PiecePosition);
}

void AChess_GameMode::DestroyMoveTiles()
{
	for (ATile* Obj : SpawnedTileArray)
	{
		Obj->Destroy();
	}
}

void AChess_GameMode::MoveClickedPiece(const FVector2D& NewPosition)
{
	for (ABasePiece* Obj : GField->WPiecesArray)
	{
		if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
		{
			Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
			Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
			Obj->SetPieceStatus(EPieceStatus::NotClicked);
		}
	}
}

bool AChess_GameMode::IsPieceColor(EPieceColor PieceColor, const FVector2D& Position)
{
	if (PieceColor == EPieceColor::Black)
	{
		for (ABasePiece* Obj : GField->BPiecesArray)
		{
			if (Obj->GetBoardPosition() == Position)
			{
				return true;
			}
		}
	}
	return false;
}

//ChangeActorLocation !!!!!
/*void AChess_GameMode::DestroyClickedPiece()
{
	for (AWhitePiece* Obj : GField->WPawnArray)
	{
		if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
		{
			//Obj->Destroy();
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
