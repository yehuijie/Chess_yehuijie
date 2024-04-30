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
    //AChess_BlackPlayer* BlackPlayer = Cast<AChess_BlackPlayer>(*TActorIterator<AChess_BlackPlayer>(GetWorld()));

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
	
	AChess_BlackPlayer* BlackPlayer = GetWorld()->SpawnActor<AChess_BlackPlayer>(FVector(), FRotator());
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

void AChess_GameMode::GeneratePawnMoves(const FVector2D& PiecePosition, FString Color)
{
	if (Color == "White")
	{
		FVector2D NextPosition = FVector2D(PiecePosition[0] + 1, PiecePosition[1]);
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] - 1);
		if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
		{
			PawnMovesArray.Add(NextPosition);
			//SpawnMovTile(NextPosition, PiecePosition);

			if (PiecePosition[0] == 1 && GField->TileIsEmpty(NextPosition[0] + 1, NextPosition[1]))
			{
				//SpawnMovTile(FVector2D(NextPosition[0] + 1, NextPosition[1]), PiecePosition);
				PawnMovesArray.Add(FVector2D(NextPosition[0] + 1, NextPosition[1]));
			}

		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceBlack(EatingMove1) && !(IsPieceWhite(EatingMove1)))
		{
			PawnEatingMovesArray.Add(EatingMove1);
			//SpawnEatTile(EatingMove1, PiecePosition);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceBlack(EatingMove2) && !(IsPieceWhite(EatingMove2)))
		{
			PawnEatingMovesArray.Add(EatingMove2);
			//SpawnEatTile(EatingMove2, PiecePosition);
		}
		
		//while x < 6 && empty
		// if white spawn, if black add to array for randmove
	}
	if (Color == "Black")
	{

		FVector2D NextPosition = FVector2D(PiecePosition[0] - 1, PiecePosition[1]);
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1);

		if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
		{

			PawnMovesArray.Add(NextPosition);
			//SpawnMovTile(NextPosition, PiecePosition);

			if (PiecePosition[0] == 6 && GField->TileIsEmpty(NextPosition[0] - 1, NextPosition[1]))
			{
				//SpawnMovTile(FVector2D(NextPosition[0] + 1, NextPosition[1]), PiecePosition);
				PawnMovesArray.Add(FVector2D(NextPosition[0] - 1, NextPosition[1]));
			}


		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceWhite(EatingMove1) && !(IsPieceBlack(EatingMove1)))
		{
			PawnEatingMovesArray.Add(EatingMove1);
			//SpawnEatTile(EatingMove1, PiecePosition);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceWhite(EatingMove2) && !(IsPieceBlack(EatingMove2)))
		{
			PawnEatingMovesArray.Add(EatingMove2);
			//SpawnEatTile(EatingMove2, PiecePosition);
		}
		
	}
}

void AChess_GameMode::GenerateQueenMoves(const FVector2D& PiecePosition, FString Color)
{
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		if (GField->TileIsEmpty(x, PiecePosition[1]))
		{
			QueenMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			x = GField->Size + 1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
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
			QueenMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			x = -1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
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
			QueenMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
			y = GField->Size + 1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
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
			QueenMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
			y = -1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
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
					QueenMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = GField->Size + 1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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
					QueenMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = -1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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
					QueenMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = GField->Size + 1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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
					QueenMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = -1;
				}

				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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

void AChess_GameMode::GenerateKingMoves(const FVector2D& PiecePosition, FString Color)
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
			//if pos0+move0<... and pos1+move1<... etc
			if (PiecePosition[0] + Move[0] >= 0 && PiecePosition[0] + Move[0] <= GField->Size && PiecePosition[1] + Move[1] >= 0 && PiecePosition[1] + Move[1] <= GField->Size)
			{
				if (Color == "White" && GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
				{
					KingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));

					for (int32 i = 0; i < BlackChecksArray.Num(); i++)
					{
						if (BlackChecksArray[i] == FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
						{
							KingMovesArray.Remove(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
						}
					}
				}
				if (Color == "Black" && GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
				{
					KingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));

					for (int32 i = 0; i < WhiteChecksArray.Num(); i++)
					{
						if (WhiteChecksArray[i] == FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
						{
							KingMovesArray.Remove(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
						}
					}
					//SpawnMovTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && !IsPieceProtected())
				{
					KingEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
					//SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
					
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && !IsPieceProtected())
				{
					KingEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
					//SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
					
				}
			}

		}
	
}

void AChess_GameMode::GenerateRookMoves(const FVector2D& PiecePosition, FString Color)
{
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		if (GField->TileIsEmpty(x, PiecePosition[1]))
		{
			RookMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			x = GField->Size + 1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
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
			RookMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			x = -1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			//SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
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
			RookMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
			y = GField->Size + 1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
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
			RookMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
		}
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
			y = -1;
		}
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			//SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
			y = -1;
		}
		else
		{
			y = -1;
		}
	}
}

void AChess_GameMode::GenerateBishopMoves(const FVector2D& PiecePosition, FString Color)
{
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if ((x - y) == PiecePosition[0] - PiecePosition[1])
			{
				if (GField->TileIsEmpty(x, y))
				{
					BishopMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = GField->Size + 1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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
					BishopMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = -1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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
					BishopMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = GField->Size + 1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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
					BishopMovesArray.Add(FVector2D(x, y));
					//SpawnMovTile(FVector2D(x, y), PiecePosition);
				}
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
					x = -1;
				}

				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					//SpawnEatTile(FVector2D(x, y), PiecePosition);
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

void AChess_GameMode::GenerateKnightMoves(const FVector2D& PiecePosition, FString Color)
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
				KnightMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
				//SpawnMovTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
			}
			else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && Color == "White")
			{
				KnightEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
				//SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
			}
			else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && Color == "Black")
			{
				KnightEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
				//SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
			}
		}
	}
}

/*void AChess_GameMode::GeneratePawnEatingMoves(const FVector2D& PiecePosition, FString Color)
{
	if (Color == "White")
	{
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] - 1);

		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceBlack(EatingMove1) && !(IsPieceWhite(EatingMove1)))
		{
			WhitePawnEatingMovesArray.Add(EatingMove1);
			//SpawnEatTile(EatingMove1, PiecePosition);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceBlack(EatingMove2) && !(IsPieceWhite(EatingMove2)))
		{
			WhitePawnEatingMovesArray.Add(EatingMove2);
			//SpawnEatTile(EatingMove2, PiecePosition);
		}
	}
	if (Color == "Black")
	{
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1);

		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceWhite(EatingMove1) && !(IsPieceBlack(EatingMove1)))
		{
			WhitePawnEatingMovesArray.Add(EatingMove1);
			//SpawnEatTile(EatingMove1, PiecePosition);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceWhite(EatingMove2) && !(IsPieceBlack(EatingMove2)))
		{
			WhitePawnEatingMovesArray.Add(EatingMove2);
			//SpawnEatTile(EatingMove2, PiecePosition);
		}
	}
}

void AChess_GameMode::GenerateQueenEatingMoves(const FVector2D& Position, FString Color)
{
}

void AChess_GameMode::GenerateKingEatingMoves(const FVector2D& Position, FString Color)
{
}

void AChess_GameMode::GenerateRookEatingMoves(const FVector2D& Position, FString Color)
{
}

void AChess_GameMode::GenerateBishopEatingMoves(const FVector2D& Position, FString Color)
{
}

void AChess_GameMode::GenerateKnightEatingMoves(const FVector2D& Position, FString Color)
{
}*/

void AChess_GameMode::ShowMoves(const FVector2D& PiecePosition, EPiece Piece, FString Color)
{

	if (Piece == EPiece::Pawn)
	{
		for (ABasePiece* Pawn : GField->PiecesArray)
		{

			if (Pawn != NULL)
			{
				if (Pawn->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Pawn->GetBoardPosition() == PiecePosition)
					{
						GeneratePawnMoves(PiecePosition, Color);
						for (int32 i = 0; i < PawnMovesArray.Num(); i++)
						{
							SpawnMovTile(PawnMovesArray[i], PiecePosition);
						}
						PawnMovesArray.Empty();
						for (int32 i = 0; i < PawnEatingMovesArray.Num(); i++)
						{
							SpawnEatTile(PawnEatingMovesArray[i], PiecePosition);
						}
						PawnEatingMovesArray.Empty();
					}
				}
			}
		}
		/*if (Color == "White")
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
			bool empty = !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1]));
			bool isblack = IsPieceBlack(EatingMove2);
			bool iswhite = IsPieceWhite(EatingMove2);
			if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceBlack(EatingMove2) && !(IsPieceWhite(EatingMove2)))
			{
				SpawnEatTile(EatingMove2, PiecePosition);
			}
			//while x < 6 && empty
			// if white spawn, if black add to array for randmove
		}
		if (Color == "Black")
		{

			FVector2D NextPosition = FVector2D(PiecePosition[0] - 1, PiecePosition[1]);
			FVector2D EatingMove1 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1);
			FVector2D EatingMove2 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1);
			if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
			{
				SpawnMovTile(NextPosition, PiecePosition);

				if (PiecePosition[0] == 6 && GField->TileIsEmpty(NextPosition[0] - 1, NextPosition[1]))
				{
					SpawnMovTile(FVector2D(NextPosition[0] - 1, NextPosition[1]), PiecePosition);

				}

			}
			if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceWhite(EatingMove1) && !(IsPieceBlack(EatingMove1)))
			{
				SpawnEatTile(EatingMove1, PiecePosition);
			}
			if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceWhite(EatingMove2) && !(IsPieceBlack(EatingMove2)))
			{
				SpawnEatTile(EatingMove2, PiecePosition);
			}
		}*/
	}
	if (Piece == EPiece::Rook)
	{
		for (ABasePiece* Rook : GField->PiecesArray)
		{

			if (Rook != NULL)
			{
				if (Rook->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Rook->GetBoardPosition() == PiecePosition)
					{
						GenerateRookMoves(PiecePosition, Color);
						for (int32 i = 0; i < RookMovesArray.Num(); i++)
						{
							SpawnMovTile(RookMovesArray[i], PiecePosition);
						}
						RookMovesArray.Empty();
						for (int32 i = 0; i < RookEatingMovesArray.Num(); i++)
						{
							SpawnEatTile(RookEatingMovesArray[i], PiecePosition);
						}
						RookEatingMovesArray.Empty();
					}
				}
			}
		}
	
		
		/*for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			}
			else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size+1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size + 1;
			}
			else
			{
				x = GField->Size+1;
			}
		}
		for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			} 
			else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = -1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
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
			else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size+1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size+1;
			}
			else
		    {
				y = GField->Size+1;
			}
		}
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if (GField->TileIsEmpty(PiecePosition[0], y))
			{
				SpawnMovTile(FVector2D(PiecePosition[0], y), PiecePosition);
			}
			else if (Color == "White" &&!(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = -1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = -1;
			}
			else
		    {
				y = -1;
			}
		}*/
	}
    if (Piece == EPiece::Bishop)
	{
		for (ABasePiece* Bishop : GField->PiecesArray)
		{

			if (Bishop != NULL)
			{
				if (Bishop->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Bishop->GetBoardPosition() == PiecePosition)
					{
						GenerateBishopMoves(PiecePosition, Color);
						for (int32 i = 0; i < BishopMovesArray.Num(); i++)
						{
							SpawnMovTile(BishopMovesArray[i], PiecePosition);
						}
						BishopMovesArray.Empty();
						for (int32 i = 0; i < BishopEatingMovesArray.Num(); i++)
						{
							SpawnEatTile(BishopEatingMovesArray[i], PiecePosition);
						}
						BishopEatingMovesArray.Empty();
					}
				}
			}
		}
		/*for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
			{
				if ((x - y) == PiecePosition[0] - PiecePosition[1])
				{
					if (GField->TileIsEmpty(x, y))
					{
						SpawnMovTile(FVector2D(x, y), PiecePosition);
					}
					else if (Color == "White" &&!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
		 	            x = GField->Size+1;
					}
					else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);	
						x = GField->Size+1;
					}
					else
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
					else if (Color == "White" &&!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = -1;
					}
					else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
					else if (Color == "White" &&!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = GField->Size + 1;
					}
					else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
					else if (Color == "White" &&!(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
					{
						SpawnEatTile(FVector2D(x, y), PiecePosition);
						x = -1;
					}
					
					else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
		}*/
	}
    if (Piece == EPiece::Knight)
	{
		for (ABasePiece* Knight : GField->PiecesArray)
		{

			if (Knight != NULL)
			{
				if (Knight->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Knight->GetBoardPosition() == PiecePosition)
					{
						GenerateKnightMoves(PiecePosition, Color);
						for (int32 i = 0; i < KnightMovesArray.Num(); i++)
						{
							SpawnMovTile(KnightMovesArray[i], PiecePosition);
						}
						KnightMovesArray.Empty();
						for (int32 i = 0; i < KnightEatingMovesArray.Num(); i++)
						{
							SpawnEatTile(KnightEatingMovesArray[i], PiecePosition);
						}
						KnightEatingMovesArray.Empty();
					}
				}
			}
		}
		/*TArray<FVector2D> KnightLMoves;
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
				else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))))
				{
					SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))))
				{
					SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
				}
			}

		}*/
	}
	if (Piece == EPiece::Queen)
	{
		for (ABasePiece* Queen : GField->PiecesArray)
		{

			if (Queen != NULL)
			{
				if (Queen->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Queen->GetBoardPosition() == PiecePosition)
					{
						GenerateQueenMoves(PiecePosition, Color);
						for (int32 i = 0; i < QueenMovesArray.Num(); i++)
						{
							SpawnMovTile(QueenMovesArray[i], PiecePosition);
						}
						QueenMovesArray.Empty();
						for (int32 i = 0; i < QueenEatingMovesArray.Num(); i++)
						{
							SpawnEatTile(QueenEatingMovesArray[i], PiecePosition);
						}
						QueenEatingMovesArray.Empty();
					}
				}
			}
		}
		/*for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
		{
			if (GField->TileIsEmpty(x, PiecePosition[1]))
			{
				SpawnMovTile(FVector2D(x, PiecePosition[1]), PiecePosition);
			}
			else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = GField->Size + 1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
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
			else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
			{
				SpawnEatTile(FVector2D(x, PiecePosition[1]), PiecePosition);
				x = -1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
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
			else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
			{
				SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
				y = GField->Size + 1;
			}
			else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
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
				else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
				{
					SpawnEatTile(FVector2D(PiecePosition[0], y), PiecePosition);
					y = -1;
				}
				else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
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
						else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
						{
							SpawnEatTile(FVector2D(x, y), PiecePosition);
							x = GField->Size + 1;
						}
						else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
						else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
						{
							SpawnEatTile(FVector2D(x, y), PiecePosition);
							x = -1;
						}
						else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
						else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
						{
							SpawnEatTile(FVector2D(x, y), PiecePosition);
							x = GField->Size + 1;
						}
						else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
						else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
						{
							SpawnEatTile(FVector2D(x, y), PiecePosition);
							x = -1;
						}

						else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
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
			}*/
		
	}

	if (Piece == EPiece::King)
	{
		for (ABasePiece* King : GField->PiecesArray)
		{

			if (King != NULL)
			{
				if (King->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (King->GetBoardPosition() == PiecePosition)
					{
						GenerateKingMoves(PiecePosition, Color);
						for (int32 i = 0; i < KingMovesArray.Num(); i++)
						{
							SpawnMovTile(KingMovesArray[i], PiecePosition);
						}
						KingMovesArray.Empty();
						for (int32 i = 0; i < KingEatingMovesArray.Num(); i++)
						{
							SpawnEatTile(KingEatingMovesArray[i], PiecePosition);
						}
						KingEatingMovesArray.Empty();
					}
				}
			}
		}
		/*
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
						SpawnMovTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && Color == "White")
					{
						SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
					}
					else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && Color == "Black")
					{
						SpawnEatTile(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]), PiecePosition);
					}
				}
			}
		
		//empty --> no while perche si puo muovere intorno, attenzione agli scacchi!!
		// particolare, non puo andare sotto scacco --> etilestatus ischecked
		//SpawnMovTile(FVector2D(PiecePosition[0] +1, PiecePosition[1]), PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0] +1, PiecePosition[1] +1),PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0], PiecePosition[1] +1), PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0] -1, PiecePosition[1] +1), PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0] -1, PiecePosition[1]), PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0] -1, PiecePosition[1] -1), PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0], PiecePosition[1] -1), PiecePosition);
		//SpawnMovTile(FVector2D(PiecePosition[0] +1, PiecePosition[1] -1), PiecePosition);*/
	}
	
}

void AChess_GameMode::SetPieceMovesToSpawned(const FVector2D& Position)
{
	for (ABasePiece* Obj : GField->PiecesArray)
	{
		if (Obj != NULL)
		{
			if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
			{
				if (Obj->GetBoardPosition() == Position)
				{
					Obj->SetPieceMoves(EPieceMoves::Spawned);
				}
			}
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
	for (ABasePiece* PieceToEat : GField->PiecesArray)
	{ 
		if (PieceToEat != NULL)
		{
			if (PieceToEat->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
			{
				if (PieceToEat->GetBoardPosition() == TilePosition)
				{
					PieceToEat->SetPieceToEat(EPieceToEat::ToBeEaten);
				}
			}
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
	SpawnedTileArray.Empty();
}

void AChess_GameMode::MoveClickedPiece(const FVector2D& NewPosition, FString Color)
{
	//if (CurrentPlayer == 0)
	if (Color == "White")
	{
		for (ABasePiece* Obj : GField->PiecesArray)
		{
			if (Obj != NULL)
			{
				if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
					{
						FVector2D OldPosition = Obj->GetBoardPosition();
						for (ATile* OldTile : GField->TileArray)
						{
							if (OldTile != NULL)
							{
								if (OldTile->GetGridPosition() == OldPosition)
								{
									OldTile->SetTileStatus(ETileStatus::EMPTY);
								}
							}
						}

						for (ABasePiece* PieceToEat : GField->PiecesArray)
						{
							if (PieceToEat != NULL)
							{
								if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
								{
									if (PieceToEat->GetBoardPosition() == NewPosition && PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
									{
										PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
										//PieceToEat->SetActorLocation(PieceToEat->GetActorLocation() - FVector(0, 0, 1000));
										PieceToEat->Destroy();

										//GField->BPiecesArray.Remove(PieceToEat);
									}
								}
							}


						}
						SetTilesToNotCheckedByColorBeforeTurn(Color);

						Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
						Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
						Obj->SetPieceStatus(EPieceStatus::NotClicked);

						VerifyChecksByColorAfterTurn(Color);

						for (ATile* NewTile : GField->TileArray)
						{
							if (NewTile != NULL)
							{
								if (NewTile != NULL)
								{
									if (NewTile->GetGridPosition() == NewPosition)
									{
										NewTile->SetTileStatus(ETileStatus::OCCUPIED);
									}
								}
							}
						}
					}
				}
			}
		}
		DestroyMoveTiles();
	}
	
	else if (Color == "Black")
	//else if (CurrentPlayer == 1)
	{
		for (ABasePiece* Obj : GField->PiecesArray)
		{
			if (Obj != NULL)
			{
				if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
					{
						FVector2D OldPosition = Obj->GetBoardPosition();
						for (ATile* OldTile : GField->TileArray)
						{
							if (OldTile != NULL)
							{
								if (OldTile->GetGridPosition() == OldPosition)
								{
									OldTile->SetTileStatus(ETileStatus::EMPTY);
								}
							}
						}
						for (ABasePiece* PieceToEat : GField->PiecesArray)
						{
							//for (ATile* SpawnedTile : SpawnedTileArray)
							if (PieceToEat != NULL)
							{
								if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
								{
									if (PieceToEat->GetBoardPosition() == NewPosition && /*SpawnedTile->GetTileType() == ETileType::Eat &&*/ PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
									{
										PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
										//PieceToEat->SetActorLocation(PieceToEat->GetActorLocation() - FVector(0, 0, 1000));
										PieceToEat->Destroy();
									}
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
						SetTilesToNotCheckedByColorBeforeTurn(Color);

						Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
						Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
						Obj->SetPieceStatus(EPieceStatus::NotClicked);

						VerifyChecksByColorAfterTurn(Color);

						for (ATile* NewTile : GField->TileArray)
						{
							if (NewTile != NULL)
							{
								if (NewTile != NULL)
								{
									if (NewTile->GetGridPosition() == NewPosition)
									{
										NewTile->SetTileStatus(ETileStatus::OCCUPIED);
									}
								}
							}
						}
					}
				}
			}
		}
		DestroyMoveTiles();
	}
	//TurnNextPlayer();
}

bool AChess_GameMode::IsPieceBlack(const FVector2D& Position)
{
	for (ABasePiece* Obj : GField->PiecesArray)
	
	{
		if (Obj != NULL)
		
		{
			if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
			{
				if (Obj->GetBoardPosition() == Position)
				{
					if (Obj->IsA(ABlackPiece::StaticClass()))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool AChess_GameMode::IsPieceWhite(const FVector2D& Position)
{
	for (ABasePiece* Obj : GField->PiecesArray)

	{
		if (Obj != NULL)
		{
			if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
			{
				if (Obj->GetBoardPosition() == Position)

				{
					if (Obj->IsA(AWhitePiece::StaticClass()))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

void AChess_GameMode::SetPieceToNotClicked()
{
	for (ABasePiece* Obj : GField->PiecesArray)
	{
		if (Obj != NULL)
		{
			if (Obj->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
			{
				if (Obj->GetPieceStatus() == EPieceStatus::Clicked)
				{
					Obj->SetPieceStatus(EPieceStatus::NotClicked);
				}
			}
		}
	}
}

void AChess_GameMode::VerifyChecksByColorAfterTurn(FString Color)
{
	if (Color == "Black")
	{
		BlackChecksArray.Empty();
		for (ABasePiece* Piece : GField->PiecesArray)
		{
			if (Piece != NULL)
			{
				if (Piece->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Piece->IsA(ABlackPiece::StaticClass()))
					{

						if (Piece->GetPiece() == EPiece::Pawn)
						{
							GeneratePawnMoves(Piece->GetBoardPosition(), "Black");
							PawnMovesArray.Empty();
							for (int32 i = 0; i < PawnEatingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(PawnEatingMovesArray[i]);
							}
							PawnEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Queen)
						{
							GenerateQueenMoves(Piece->GetBoardPosition(), "Black");
							for (int32 i = 0; i < QueenMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(QueenMovesArray[i]);
							}
							QueenMovesArray.Empty();
							for (int32 i = 0; i < QueenEatingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(QueenEatingMovesArray[i]);
							}
							QueenEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Rook)
						{
							GenerateRookMoves(Piece->GetBoardPosition(), "Black");
							for (int32 i = 0; i < RookMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(RookMovesArray[i]);
							}
							RookMovesArray.Empty();
							for (int32 i = 0; i < RookEatingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(RookEatingMovesArray[i]);
							}
							RookEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Knight)
						{
							GenerateKnightMoves(Piece->GetBoardPosition(), "Black");
							for (int32 i = 0; i < KnightMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(KnightMovesArray[i]);
							}
							KnightMovesArray.Empty();
							for (int32 i = 0; i < KnightEatingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(KnightEatingMovesArray[i]);
							}
							KnightEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Bishop)
						{
							GenerateBishopMoves(Piece->GetBoardPosition(), "Black");
							for (int32 i = 0; i < BishopMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(BishopMovesArray[i]);
							}
							BishopMovesArray.Empty();
							for (int32 i = 0; i < BishopEatingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(BishopEatingMovesArray[i]);
							}
							BishopEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::King)
						{
							GenerateKingMoves(Piece->GetBoardPosition(), "Black");
							for (int32 i = 0; i < KingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(KingMovesArray[i]);
							}
							KingMovesArray.Empty();
							for (int32 i = 0; i < KingEatingMovesArray.Num(); i++)
							{
								BlackChecksArray.Add(KingEatingMovesArray[i]);
							}
							KingEatingMovesArray.Empty();
						}

					}
				}
			}
		}
		for (int32 i = 0; i < BlackChecksArray.Num(); i++)
		{
			for (ATile* Tile : GField->TileArray)
			{
				if (Tile->GetGridPosition() == BlackChecksArray[i])
				{
					Tile->SetIsCheckByBlacks(EIsCheckByBlacks::Checked);
				}
			}
		}
	}
	if (Color == "White")
	{
		WhiteChecksArray.Empty();
		for (ABasePiece* Piece : GField->PiecesArray)
		{
			if (Piece != NULL)
			{
				if (Piece->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Piece->IsA(AWhitePiece::StaticClass()))
					{
						if (Piece->GetPiece() == EPiece::Pawn)
						{
							GeneratePawnMoves(Piece->GetBoardPosition(), "White");
							PawnMovesArray.Empty();
							for (int32 i = 0; i < PawnEatingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(PawnEatingMovesArray[i]);
							}
							PawnEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Queen)
						{
							GenerateQueenMoves(Piece->GetBoardPosition(), "White");
							for (int32 i = 0; i < QueenMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(QueenMovesArray[i]);
							}
							QueenMovesArray.Empty();
							for (int32 i = 0; i < QueenEatingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(QueenEatingMovesArray[i]);
							}
							QueenEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Rook)
						{
							GenerateRookMoves(Piece->GetBoardPosition(), "White");
							for (int32 i = 0; i < RookMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(RookMovesArray[i]);
							}
							RookMovesArray.Empty();
							for (int32 i = 0; i < RookEatingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(RookEatingMovesArray[i]);
							}
							RookEatingMovesArray.Empty();
						}
                        if (Piece->GetPiece() == EPiece::Knight)
						{
							GenerateKnightMoves(Piece->GetBoardPosition(), "White");
							for (int32 i = 0; i < KnightMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(KnightMovesArray[i]);
							}
							KnightMovesArray.Empty();
							for (int32 i = 0; i < KnightEatingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(KnightEatingMovesArray[i]);
							}
							KnightEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::Bishop)
						{
							GenerateBishopMoves(Piece->GetBoardPosition(), "White");
							for (int32 i = 0; i < BishopMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(BishopMovesArray[i]);
							}
							BishopMovesArray.Empty();
							for (int32 i = 0; i < BishopEatingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(BishopEatingMovesArray[i]);
							}
							BishopEatingMovesArray.Empty();
						}
						if (Piece->GetPiece() == EPiece::King)
						{
							GenerateKingMoves(Piece->GetBoardPosition(), "White");
							for (int32 i = 0; i < KingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(KingMovesArray[i]);
							}
							KingMovesArray.Empty();
							for (int32 i = 0; i < KingEatingMovesArray.Num(); i++)
							{
								WhiteChecksArray.Add(KingEatingMovesArray[i]);
							}
							KingEatingMovesArray.Empty();
						}

					}
				}
			}
		}
		for (int32 i = 0; i < WhiteChecksArray.Num(); i++)
		{
			for (ATile* Tile : GField->TileArray)
			{
				if (Tile->GetGridPosition() == WhiteChecksArray[i])
				{
					Tile->SetIsCheckByWhites(EIsCheckByWhites::Checked);
				}
			}
		}

	}

	
}

void AChess_GameMode::SetTilesToNotCheckedByColorBeforeTurn(FString Color)
{
	if (Color == "White")
	{
		for (ATile* Tile : GField->TileArray)
		{
			Tile->SetIsCheckByWhites(EIsCheckByWhites::NotChecked);
		}
	}
	if (Color == "Black")
	{
		for (ATile* Tile : GField->TileArray)
		{
			Tile->SetIsCheckByBlacks(EIsCheckByBlacks::NotChecked);
		}
	}
}

bool AChess_GameMode::IsPieceProtected()
{
	for (ABasePiece* PieceToEat : GField->PiecesArray)
	{
		if (PieceToEat != NULL)

		{
			if (PieceToEat->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
			{
				if (PieceToEat->IsA(ABlackPiece::StaticClass()))
				{
					for (int32 i = 0; i < WhiteChecksArray.Num(); i++)
					{
						if (PieceToEat->GetBoardPosition() == WhiteChecksArray[i])
						{
							return true;
						}
					}
				}
				if (PieceToEat->IsA(AWhitePiece::StaticClass()))
				{
					for (int32 i = 0; i < BlackChecksArray.Num(); i++)
					{
						if (PieceToEat->GetBoardPosition() == BlackChecksArray[i])
						{
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}



/*void AChess_GameMode::MoveBlackPiece()
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
}*/



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
