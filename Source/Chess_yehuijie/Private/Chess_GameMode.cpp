// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Chess_HumanPlayer.h"
#include "Chess_RandomPlayer.h"
#include "Chess_BlackPlayer.h"
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

	RandIdx = 0;

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
	auto* BlackPlayer = GetWorld()->SpawnActor<AChess_BlackPlayer>(FVector(), FRotator());

	// MiniMax Player
	//auto* AI = GetWorld()->SpawnActor<ATTT_MinimaxPlayer>(FVector(), FRotator());

	// AI player = 1
	Players.Add(BlackPlayer);

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
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceBlack(EatingMove1) && !(IsPieceWhite(EatingMove1)))
		{
			SpawnEatTile(EatingMove1, PiecePosition);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceBlack(EatingMove2) && !(IsPieceWhite(EatingMove2)))
		{
			SpawnEatTile(EatingMove2, PiecePosition);
		}
		//while x < 6 && empty
		// if white spawn, if black add to array for randmove
		
	}
	if (Piece == EPiece::Rook)
	{
		
		for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			} else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size+1;
			} else
			{
				x = GField->Size+1;
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			} else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = -1;
			} else 
			{
				x = -1;
			}
		}
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y))
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			} else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size+1;
			} else
		    {
				y = GField->Size+1;
			}
		}
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y))
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			} else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = -1;
			} else
		    {
				y = -1;
			}
		}
	}
    if (Piece == EPiece::Bishop)
	{
		for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
			{
				if ((x - y) == PiecePosition[0] - PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
		 	            x = GField->Size+1;
					} else
		            {
				        x = GField->Size+1;
			        }
					
				}
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
			{
				if ((x - y) == PiecePosition[0] - PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = -1;
					}
					else
					{
						x = -1;
					}

				}
			}
		}
		for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
			{
				if ((x + y) == PiecePosition[0] + PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = GField->Size + 1;
					}
					else
					{
						x = GField->Size + 1;
					}

				}
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
			{
				if ((x + y) == PiecePosition[0] + PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = -1;
					}
					else
					{
						x = -1;
					}

				}
			}
		}
	}
        if (Piece == EPiece::Knight)
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
			if (PiecePosition[0] + Move[0] >= 0 && PiecePosition[0] + Move[0] <= GField->Size && PiecePosition[1] + Move[1] >= 0 && PiecePosition[1] + Move[1] <= GField->Size)
			{
				if (GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
				{
					SpawnMovTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
				}
				else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))))
				{
					SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
				}
			}

		}
	}
	if (Piece == EPiece::Queen)
	{
		

		for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			}
			else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size + 1;
			}
			else
			{
				x = GField->Size + 1;
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			}
			else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = -1;
			}
			else
			{
				x = -1;
			}
		}
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y))
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			}
			else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y)) ))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size + 1;
			}
			else
			{
				y = GField->Size + 1;
			}
		}
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y))
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			}
			else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y)) ))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = -1;
			}
			else
			{
				y = -1;
			}
		}
		for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
			{
				if ((x - y) == PiecePosition[0] - PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = GField->Size + 1;
					}
					else
					{
						x = GField->Size + 1;
					}

				}
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
			{
				if ((x - y) == PiecePosition[0] - PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = -1;
					}
					else
					{
						x = -1;
					}

				}
			}
		}
		for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
			{
				if ((x + y) == PiecePosition[0] + PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = GField->Size + 1;
					}
					else
					{
						x = GField->Size + 1;
					}

				}
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
			{
				if ((x + y) == PiecePosition[0] + PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = -1;
					}
					else
					{
						x = -1;
					}

				}
			}
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
		Obj->SetTileStatus(ETileStatus::MOVE);
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
	//Obj->SetTileStatus(0, ETileStatus::EAT);
	for (ABasePiece* PieceToEat : GField->BPiecesArray)
	{ 
		if (PieceToEat->GetBoardPosition() == TilePosition)
		{
			PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
		}
	}
	SetPieceMovesToSpawned(PiecePosition);
}

void AChess_GameMode::DestroyMoveTiles()
{
	for (ATile* Obj : SpawnedTileArray)
	{
		Obj->Destroy();
		//SpawnedTileArray.Remove(Obj);	
	}
}

void AChess_GameMode::MoveClickedPiece(const FVector2D& NewPosition)
{
	if (CurrentPlayer == 0)
	{
		for (ABasePiece* Obj : GField->WPiecesArray)
		{
			if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
			{
				FVector2D OldPosition = Obj->GetBoardPosition();
				for (ATile* OldTile : GField->TileArray)
				{
					if (OldTile->GetGridPosition() == OldPosition)
					{
						OldTile->SetTileStatus( ETileStatus::EMPTY);
					}
				}

				for (ABasePiece* PieceToEat : GField->BPiecesArray)
				{
					if (PieceToEat->GetBoardPosition() == NewPosition && PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
					{
						PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
						PieceToEat->SetActorLocation(PieceToEat->GetActorLocation() - FVector(0, 0, 1000));
						//PieceToEat->Destroy();

						//GField->BPiecesArray.Remove(PieceToEat);
					}


				}

				Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
				Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
				Obj->SetPieceStatus(EPieceStatus::NotClicked);
				for (ATile* NewTile : GField->TileArray)
				{
					if (NewTile->GetGridPosition() == NewPosition)
					{
						NewTile->SetTileStatus(ETileStatus::OCCUPIED);
					}
				}
			}
		}
		DestroyMoveTiles();
	}
	else if (CurrentPlayer == 1)
	{
		for (ABasePiece* Obj : GField->BPiecesArray)
		{
			if (Obj->GetBoardPosition() == GField->BPiecesArray[RandIdx]->GetBoardPosition())
			{
				FVector2D OldPosition = Obj->GetBoardPosition();
				for (ATile* OldTile : GField->TileArray)
				{
					if (OldTile->GetGridPosition() == OldPosition)
					{
						OldTile->SetTileStatus(ETileStatus::EMPTY);
					}
				}
				for (ABasePiece* PieceToEat : GField->WPiecesArray)
				{
					//for (ATile* SpawnedTile : SpawnedTileArray)
					{
						if (PieceToEat->GetBoardPosition() == NewPosition && /*SpawnedTile->GetTileType() == ETileType::Eat &&*/ PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
						{
							PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
							PieceToEat->SetActorLocation(PieceToEat->GetActorLocation() - FVector(0, 0, 1000));
							//PieceToEat->Destroy();
						}
					}
					/*if (PieceToEat->GetBoardPosition() == NewPosition && PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
					{
						PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
						PieceToEat->SetActorLocation(PieceToEat->GetActorLocation() - FVector(0, 0, 1000));
						//PieceToEat->Destroy();
						//GField->BPiecesArray.Remove(PieceToEat);
					}*/
				}
				Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
				Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
				//Obj->SetPieceStatus(EPieceStatus::NotClicked);
				for (ATile* NewTile : GField->TileArray)
				{
					if (NewTile->GetGridPosition() == NewPosition)
					{
						NewTile->SetTileStatus(ETileStatus::OCCUPIED);
					}
				}
			}
		}
	}
	TurnNextPlayer();
}

bool AChess_GameMode::IsPieceBlack(const FVector2D& Position)
{
	for (ABasePiece* Obj : GField->BPiecesArray)
	
	{
		if (Obj->GetBoardPosition() == Position)
		
		{
			if (Obj->IsA(ABlackPiece::StaticClass()))
			{
				return true;
			}
		}
	}
	return false;
}

bool AChess_GameMode::IsPieceWhite(const FVector2D& Position)
{
	for (ABasePiece* Obj : GField->WPiecesArray)

	{
		if (Obj->GetBoardPosition() == Position)

		{
			if (Obj->IsA(AWhitePiece::StaticClass()))
			{
				return true;
			}
		}
	}
	return false;
}

void AChess_GameMode::SetPieceToNotClicked()
{
	for (ABasePiece* Obj : GField->WPiecesArray)
	{
		if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
		{
			Obj->SetPieceStatus(EPieceStatus::NotClicked);
		}
	}
}

void AChess_GameMode::MoveBlackPiece()
{

	for (ABasePiece* CurrPiece : GField->BPiecesArray)
	{
		if (CurrPiece->GetBoardPosition() == GField->BPiecesArray[RandIdx]->GetBoardPosition())
			//if (CurrPiece->GetPieceStatus() == EPieceStatus::Clicked)
		{
			FVector2D PiecePosition = CurrPiece->GetBoardPosition();

			if (CurrPiece->GetPiece() == EPiece::Pawn)
			{
				FVector2D NextPosition = FVector2D(PiecePosition[0] - 1, PiecePosition[1]);
				FVector2D EatingMove1 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1);
				FVector2D EatingMove2 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1);
				if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
				{
					BlackMovesArray.Add(FVector2D(PiecePosition[0] - 1, PiecePosition[1]));

					if (PiecePosition[0] == 6 && GField->TileIsEmpty(NextPosition[0] - 1, NextPosition[1]))
					{
						BlackMovesArray.Add(FVector2D(NextPosition[0] + 1, NextPosition[1]));

					}

				}
				if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceWhite(EatingMove1) && !(IsPieceBlack(EatingMove1)))
				{
					for (ABasePiece* PieceToEat : GField->WPiecesArray)
					{
						if (PieceToEat->GetBoardPosition() == FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1))
						{
							PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
						}
					}
					BlackMovesArray.Add(FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1));
				}
				if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceWhite(EatingMove2) && !(IsPieceBlack(EatingMove2)))
				{
					for (ABasePiece* PieceToEat : GField->WPiecesArray)
					{
						if (PieceToEat->GetBoardPosition() == FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1))
						{
							PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
						}
					}
					BlackMovesArray.Add(FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1));
				}

			}
			if (CurrPiece->GetPiece() == EPiece::King)
			{
				TArray<FVector2D> KingMoves;
				KingMoves.Add(FVector2D(1, 0));
				KingMoves.Add(FVector2D(1, 1));
				KingMoves.Add(FVector2D(0, 1));
				KingMoves.Add(FVector2D(-1, 1));
				KingMoves.Add(FVector2D(-1, 0));
				KingMoves.Add(FVector2D(-1, -1));
				KingMoves.Add(FVector2D(0, -1));
				KingMoves.Add(FVector2D(1, -1));

				for (const FVector2D& Move : KingMoves)
				{
					if (PiecePosition[0] + Move[0] >= 0 && PiecePosition[0] + Move[0] <= GField->Size && PiecePosition[1] + Move[1] >= 0 && PiecePosition[1] + Move[1] <= GField->Size)
					{
						if (GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
						{
							BlackMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
						}
						else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))))
						{
							for (ABasePiece* PieceToEat : GField->WPiecesArray)
							{
								if (PieceToEat->GetBoardPosition() == FVector2D(CurrPiece->GetBoardPosition()[0] - Move[0], CurrPiece->GetBoardPosition()[1] - Move[1]))
								{
									PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
								}
							}
							BlackMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
						}
					}



				}



			}
			if (CurrPiece->GetPiece() == EPiece::Rook)
			{
				for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
				{
					if (GField->TileIsEmpty(x, PiecePosition[1]))
					{
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
					}
					else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(x, PiecePosition[1]))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
						x = GField->Size + 1;
					}
					else
					{
						x = GField->Size + 1;
					}
				}
				for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
				{
					if (GField->TileIsEmpty(x, PiecePosition[1]))
					{
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
					}
					else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(x, PiecePosition[1]))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
						x = -1;
					}
					else
					{
						x = -1;
					}
				}
				for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
				{
					if (GField->TileIsEmpty(PiecePosition[0], y))
					{
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
					}
					else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(PiecePosition[0], y))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
						y = GField->Size + 1;
					}
					else
					{
						y = GField->Size + 1;
					}
				}
				for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
				{
					if (GField->TileIsEmpty(PiecePosition[0], y))
					{
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
					}
					else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(PiecePosition[0], y))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
						y = -1;
					}
					else
					{
						y = -1;
					}
				}

			}
			if (CurrPiece->GetPiece() == EPiece::Knight)
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
					if (PiecePosition[0] + Move[0] >= 0 && PiecePosition[0] + Move[0] <= GField->Size && PiecePosition[1] + Move[1] >= 0 && PiecePosition[1] + Move[1] <= GField->Size)
					{
						if (GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
						{
							BlackMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
						}
						else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))))
						{
							for (ABasePiece* PieceToEat : GField->WPiecesArray)
							{
								if (PieceToEat->GetBoardPosition() == FVector2D(CurrPiece->GetBoardPosition()[0] - Move[0], CurrPiece->GetBoardPosition()[1] - Move[1]))
								{
									PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
								}
							}
							BlackMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
						}
					}
					//if pos0+move0<... and pos1+move1<... etc


				}
			}
			if (CurrPiece->GetPiece() == EPiece::Bishop)
			{
				for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
				{
					for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
					{
						if ((x - y) == PiecePosition[0] - PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));

								x = GField->Size + 1;
							}
							else
							{
								x = GField->Size + 1;
							}

						}
					}
				}
				for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
				{
					for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
					{
						if ((x - y) == PiecePosition[0] - PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));
								x = -1;
							}
							else
							{
								x = -1;
							}

						}
					}
				}
				for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
				{
					for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
					{
						if ((x + y) == PiecePosition[0] + PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));
								x = GField->Size + 1;
							}
							else
							{
								x = GField->Size + 1;
							}

						}
					}
				}
				for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
				{
					for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
					{
						if ((x + y) == PiecePosition[0] + PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));
								x = -1;
							}
							else
							{
								x = -1;
							}

						}
					}
				}

			}
			if (CurrPiece->GetPiece() == EPiece::Queen)
			{
				for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
				{
					if (GField->TileIsEmpty(x, PiecePosition[1]))
					{
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
					}
					else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(x, PiecePosition[1]))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
						x = GField->Size + 1;
					}
					else
					{
						x = GField->Size + 1;
					}
				}
				for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
				{
					if (GField->TileIsEmpty(x, PiecePosition[1]))
					{
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
					}
					else if (!(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(x, PiecePosition[1]))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(x, PiecePosition[1]));
						x = -1;
					}
					else
					{
						x = -1;
					}
				}
				for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
				{
					if (GField->TileIsEmpty(PiecePosition[0], y))
					{
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
					}
					else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(PiecePosition[0], y))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
						y = GField->Size + 1;
					}
					else
					{
						y = GField->Size + 1;
					}
				}
				for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
				{
					if (GField->TileIsEmpty(PiecePosition[0], y))
					{
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
					}
					else if (!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
					{
						for (ABasePiece* PieceToEat : GField->WPiecesArray)
						{
							if (PieceToEat->GetBoardPosition() == FVector2D(PiecePosition[0], y))
							{
								PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
							}
						}
						BlackMovesArray.Add(FVector2D(PiecePosition[0], y));
						y = -1;
					}
					else
					{
						y = -1;
					}
				}
				for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
				{
					for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
					{
						if ((x - y) == PiecePosition[0] - PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));

								x = GField->Size + 1;
							}
							else
							{
								x = GField->Size + 1;
							}

						}
					}
				}
				for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
				{
					for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
					{
						if ((x - y) == PiecePosition[0] - PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));
								x = -1;
							}
							else
							{
								x = -1;
							}

						}
					}
				}
				for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
				{
					for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
					{
						if ((x + y) == PiecePosition[0] + PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));
								x = GField->Size + 1;
							}
							else
							{
								x = GField->Size + 1;
							}

						}
					}
				}
				for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
				{
					for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
					{
						if ((x + y) == PiecePosition[0] + PiecePosition[1])
						{
							if (GField->TileIsEmpty(x, y))
							{
								BlackMovesArray.Add(FVector2D(x, y));
							}
							else if (!(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
							{
								for (ABasePiece* PieceToEat : GField->WPiecesArray)
								{
									if (PieceToEat->GetBoardPosition() == FVector2D(x, y))
									{
										PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
									}
								}
								BlackMovesArray.Add(FVector2D(x, y));
								x = -1;
							}
							else
							{
								x = -1;
							}

						}
					}
				}

			}
		}
	}
	if (BlackMovesArray.Num() > 0)
	{
		int32 RandMove = FMath::RandRange(0, BlackMovesArray.Num() - 1);
		FVector2D NewPosition = BlackMovesArray[RandMove];
		MoveClickedPiece(NewPosition);
		BlackMovesArray.Empty();
	}
	else
	{
		//GField->BPiecesArray[RandIdx]->SetPieceStatus(EPieceStatus::NotClicked);
		SetBlackToMove();
		MoveBlackPiece();
	}




}

void AChess_GameMode::SetBlackToMove()
{
	RandIdx = FMath::RandRange(0, GField->BPiecesArray.Num() - 1);

	while (GField->BPiecesArray[RandIdx]->GetIsPieceOnBoard() == EPieceOnBoard::NotOnBoard)
	{
		RandIdx = FMath::RandRange(0, GField->BPiecesArray.Num() - 1);

	}
	//if (GField->BPiecesArray[RandIdx]->GetIsPieceOnBoard() != EPieceOnBoard::NotOnBoard)
//GField->BPiecesArray[RandIdx]->SetPieceStatus(EPieceStatus::Clicked);
	GField->BPiecesArray[RandIdx]->SetOldPosition((GField->BPiecesArray[RandIdx]->GetBoardPosition())[0], (GField->BPiecesArray[RandIdx]->GetBoardPosition())[1]);

}

int32 AChess_GameMode::GetBlackArraySize()
{
	int32 Size = GField->BPiecesArray.Num();
	return Size;
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
