// In fondo al file ci sono le funzioni che avevo iniziato a scrivere per provare a far funzionare il RandomPlayer, ma ho riscontrato
// dei problemi perchè avevo skippato il 5. ed ero passato direttamente al punto 7.

#include "Chess_GameMode.h"
#include "Chess_PlayerController.h"
#include "Chess_HumanPlayer.h"
//#include "Chess_RandomPlayer.h"
//#include "Chess_BlackPlayer.h"
#include "GameField.h"
#include "EngineUtils.h"

// Sets default values
AChess_GameMode::AChess_GameMode()
{
	// initialization
	PlayerControllerClass = AChess_PlayerController::StaticClass();
	DefaultPawnClass = AChess_HumanPlayer::StaticClass();
	FieldSize = 8;
}

// Called when the game starts or when spawned
void AChess_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// tracks if the game is over (non utilizzato)
	IsGameOver = false;

	// tracks the number of moves in order to signal a drawn game (non utilizzato)
	MoveCounter = 0;

	// random index to select a random Piece for the RandomPlayer (non utilizzato)
	RandIdx = 0;

	// Casts a HumanPlayer within the Current World, if Failed it returns nullptr
	AChess_HumanPlayer* HumanPlayer = Cast<AChess_HumanPlayer>(*TActorIterator<AChess_HumanPlayer>(GetWorld()));

	if (GameFieldClass != nullptr)
	{
		// Spawn the GameField/Chessboard and Set the FieldSize
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}

	// Set a Fixed Camera
	float CameraPosX = ((GField->TileSize * (FieldSize + ((FieldSize - 1) * GField->NormalizedCellPadding) - (FieldSize - 1))) / 2) - (GField->TileSize / 2);
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());

	// Human player = 0, add it to the Players Array
	Players.Add(HumanPlayer);

	// Manca un secondo player aggiunto nell'Array perchè non sono riuscito ad implemetare il RandomPlayer ed ho riscontrato 
	// difficolta con BlackPlayer, nel senso che non partiva il turno dei Neri
	
	//AChess_BlackPlayer* BlackPlayer = GetWorld()->SpawnActor<AChess_BlackPlayer>(FVector(), FRotator());
	//Players.Add(BlackPlayer);

	// called at the start of the game to pick who is starting
	this->ChoosePlayerAndStartGame();
}

// It's Fixed that White always starts
void AChess_GameMode::ChoosePlayerAndStartGame()
{
	// 0 = Human/White and always starts
	CurrentPlayer = 0; 
	// non utilizzato
	MoveCounter += 1;
	// Calls OnTurn of the starting Player
	Players[CurrentPlayer]->OnTurn();
}

// Called when a Pawn is Clicked, in order to generate the possible Moves and fill the respective Arrays
void AChess_GameMode::GeneratePawnMoves(const FVector2D& PiecePosition, FString Color)
{
	// If it's a White Pawn, then it Moves Forward, x=(x+1) Position
	if (Color == "White")
	{
		FVector2D NextPosition = FVector2D(PiecePosition[0] + 1, PiecePosition[1]);
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] + 1, PiecePosition[1] - 1);
		// If we're within the Board and NextPosition Tile is Empty, add the Position to PawnMovesArray
		if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
		{
			PawnMovesArray.Add(NextPosition);
			
                        // If the White Pawn is in its starting Position, then it can Move 2 Tiles Foward
			if (PiecePosition[0] == 1 && GField->TileIsEmpty(NextPosition[0] + 1, NextPosition[1]))
			{
				PawnMovesArray.Add(FVector2D(NextPosition[0] + 1, NextPosition[1]));
			}

		}
		// The Pawn can Eat Diagonally, if one of the 2 Diagonal Tiles in front of him are not Empty and are Occupied by a Black Piece, add the Position to PawnEatingMovesArray
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceBlack(EatingMove1) && !(IsPieceWhite(EatingMove1)))
		{
			PawnEatingMovesArray.Add(EatingMove1);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceBlack(EatingMove2) && !(IsPieceWhite(EatingMove2)))
		{
			PawnEatingMovesArray.Add(EatingMove2);
		}
	}
	// If it's a Black Pawn, then it Moves Backwards, x=(x-1) Position
	if (Color == "Black")
	{
		FVector2D NextPosition = FVector2D(PiecePosition[0] - 1, PiecePosition[1]);
		FVector2D EatingMove1 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] + 1);
		FVector2D EatingMove2 = FVector2D(PiecePosition[0] - 1, PiecePosition[1] - 1);

		// If we're within the Board and NextPosition Tile is Empty, add the Position to PawnMovesArray
		if (PiecePosition[0] < GField->Size - 1 && GField->TileIsEmpty(NextPosition[0], NextPosition[1]))
		{
			PawnMovesArray.Add(NextPosition);

			 // If the White Pawn is in its starting Position, then it can Move 2 Tiles Backwards
			if (PiecePosition[0] == 6 && GField->TileIsEmpty(NextPosition[0] - 1, NextPosition[1]))
			{
				PawnMovesArray.Add(FVector2D(NextPosition[0] - 1, NextPosition[1]));
			}
		}
		// The Pawn can Eat Diagonally, if one of the 2 Diagonal Tiles in behind of him are not Empty and are Occupied by a White Piece, add the Position to PawnEatingMovesArray
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove1[0], EatingMove1[1])) && IsPieceWhite(EatingMove1) && !(IsPieceBlack(EatingMove1)))
		{
			PawnEatingMovesArray.Add(EatingMove1);
		}
		if (PiecePosition[0] < GField->Size - 1 && !(GField->TileIsEmpty(EatingMove2[0], EatingMove2[1])) && IsPieceWhite(EatingMove2) && !(IsPieceBlack(EatingMove2)))
		{
			PawnEatingMovesArray.Add(EatingMove2);
		}	
	}
}

// Called when a Queen is Clicked, in order to generate the possible Moves and fill the respective Arrays
void AChess_GameMode::GenerateQueenMoves(const FVector2D& PiecePosition, FString Color)
{
	// checks that the Forward Move doesn't go out of the Board
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(x, PiecePosition[1]))
		{
			QueenMovesArray.Add(FVector2D(x, PiecePosition[1]));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			x = GField->Size + 1;
		}
	}
	// checks that the Backward Move doesn't go out of the Board
	for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(x, PiecePosition[1]))
		{
			QueenMovesArray.Add(FVector2D(x, PiecePosition[1]));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = -1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			QueenEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = -1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			x = -1;
		}
	}
	// checks that the Move to the Right doesn't go out of the Board
	for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(PiecePosition[0], y))
		{
			QueenMovesArray.Add(FVector2D(PiecePosition[0], y));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));;
			y = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			y = GField->Size + 1;
		}
	}
	// checks that the Move to the Left doesn't go out of the Board
	for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(PiecePosition[0], y))
		{
			QueenMovesArray.Add(FVector2D(PiecePosition[0], y));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = -1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			QueenEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = -1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			y = -1;
		}
	}
	// checks that the Move to the Top-Right doesn't go out of the Board
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if ((x - y) == PiecePosition[0] - PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					QueenMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = GField->Size + 1;
				}

			}
		}
	}
	// checks that the Move to the Bottom-Left doesn't go out of the Board
	for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
	{
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if ((x - y) == PiecePosition[0] - PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					QueenMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = -1;
				}

			}
		}
	}
	// checks that the Move to the Top-Left doesn't go out of the Board
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if ((x + y) == PiecePosition[0] + PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					QueenMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = GField->Size + 1;
				}

			}
		}
	}
	// checks that the Move to the Bottom-Right doesn't go out of the Board
	for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
	{
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if ((x + y) == PiecePosition[0] + PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the QueenMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					QueenMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the QueenEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					QueenEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = -1;
				}

			}
		}
	}
}

// Called when a King is Clicked, in order to generate the possible Moves and fill the respective Arrays
void AChess_GameMode::GenerateKingMoves(const FVector2D& PiecePosition, FString Color)
{
	// Array containing all the Possible Moves for the King
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
		// Durante i Tests mi è sembrato che le mosse del Re non venissero correttamente generate quando il Re dovesse interagine con dei Pedoni, ma non ho fatto in tempo a capirne il motivo
		
		// If we're within the Board and NextPosition Tile is Empty, add the Position to KingMovesArray
		if (PiecePosition[0] + Move[0] >= 0 && PiecePosition[0] + Move[0] <= GField->Size && PiecePosition[1] + Move[1] >= 0 && PiecePosition[1] + Move[1] <= GField->Size)			
		{
			// If the King would move to a Tile Checked by the opposing Color, remove the Position from KingMovesArray
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
			// If the King would move to a Tile Checked by the opposing Color, remove the Position from KingMovesArray
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
			}
			// If the NextPosition is not Empty, Occupied by the opposing Color, and the Piece is not Protected by another Piece (Meaning that if eaten, would make the King go into a Check), then add the Position to KingEatingMovesArray
			// Non funziona correttamente perche la logica dietro a IsProtected() è errata
			else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && !IsPieceProtected())
			{
				KingEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
				
			}
			// If the NextPosition is not Empty, Occupied by the opposing Color, and the Piece is not Protected by another Piece (Meaning that if eaten, would make the King go into a Check), then add the Position to KingEatingMovesArray
			// Non funziona correttamente perche la logica dietro a IsProtected() è errata
			else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && !IsPieceProtected())
			{
				KingEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
				
			}
		}
	}	
}

// Called when a Rook is Clicked, in order to generate the possible Moves and fill the respective Arrays
void AChess_GameMode::GenerateRookMoves(const FVector2D& PiecePosition, FString Color)
{
	// checks that the Forward Move doesn't go out of the Board
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the RookMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(x, PiecePosition[1]))
		{
			RookMovesArray.Add(FVector2D(x, PiecePosition[1]));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			x = GField->Size + 1;
		}
	}
	// checks that the Backward Move doesn't go out of the Board
	for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the RookMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(x, PiecePosition[1]))
		{
			RookMovesArray.Add(FVector2D(x, PiecePosition[1]));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceBlack(FVector2D(x, PiecePosition[1])) && !(IsPieceWhite(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = -1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(x, PiecePosition[1])) && IsPieceWhite(FVector2D(x, PiecePosition[1])) && !(IsPieceBlack(FVector2D(x, PiecePosition[1]))))
		{
			RookEatingMovesArray.Add(FVector2D(x, PiecePosition[1]));
			x = -1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			x = -1;
		}
	}
	// checks that the Move to the Right doesn't go out of the Board
	for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the RookMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(PiecePosition[0], y))
		{
			RookMovesArray.Add(FVector2D(PiecePosition[0], y));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = GField->Size + 1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			y = GField->Size + 1;
		}
	}
	// checks that the Move to the Left doesn't go out of the Board
	for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
	{
		// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the RookMovesArray, it Stops when Finds another Piece, so it backs out of the For
		if (GField->TileIsEmpty(PiecePosition[0], y))
		{
			RookMovesArray.Add(FVector2D(PiecePosition[0], y));
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "White" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceBlack(FVector2D(PiecePosition[0], y)) && !(IsPieceWhite(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = -1;
		}
		// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the RookEatingMovesArray
		else if (Color == "Black" && !(GField->TileIsEmpty(PiecePosition[0], y)) && IsPieceWhite(FVector2D(PiecePosition[0], y)) && !(IsPieceBlack(FVector2D(PiecePosition[0], y))))
		{
			RookEatingMovesArray.Add(FVector2D(PiecePosition[0], y));
			y = -1;
		}
		// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
		else
		{
			y = -1;
		}
	}
}

// Called when a Bishop is Clicked, in order to generate the possible Moves and fill the respective Arrays
void AChess_GameMode::GenerateBishopMoves(const FVector2D& PiecePosition, FString Color)
{
	// checks that the Move to the Top-Right doesn't go out of the Board
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if ((x - y) == PiecePosition[0] - PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the BishopMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					BishopMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = GField->Size + 1;
				}

			}
		}
	}
	// checks that the Move to the Bottom-Left doesn't go out of the Board
	for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
	{
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if ((x - y) == PiecePosition[0] - PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the BishopMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					BishopMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = -1;
				}

			}
		}
	}
	// checks that the Move to the Top-Left doesn't go out of the Board
	for (int32 x = PiecePosition[0] + 1; x <= GField->Size; x++)
	{
		for (int32 y = PiecePosition[1] - 1; y >= 0; y--)
		{
			if ((x + y) == PiecePosition[0] + PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the BishopMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					BishopMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = GField->Size + 1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
				else
				{
					x = GField->Size + 1;
				}

			}
		}
	}
	// checks that the Move to the Bottom-Right doesn't go out of the Board
	for (int32 x = PiecePosition[0] - 1; x >= 0; x--)
	{
		for (int32 y = PiecePosition[1] + 1; y <= GField->Size; y++)
		{
			if ((x + y) == PiecePosition[0] + PiecePosition[1])
			{
				// If the NextPosition Tile is Empty, it keeps adding the NextPostion to the BishopMovesArray, it Stops when Finds another Piece, so it backs out of the For
				if (GField->TileIsEmpty(x, y))
				{
					BishopMovesArray.Add(FVector2D(x, y));
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "White" && !(GField->TileIsEmpty(x, y)) && IsPieceBlack(FVector2D(x, y)) && !(IsPieceWhite(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the BishopEatingMovesArray
				else if (Color == "Black" && !(GField->TileIsEmpty(x, y)) && IsPieceWhite(FVector2D(x, y)) && !(IsPieceBlack(FVector2D(x, y))))
				{
					BishopEatingMovesArray.Add(FVector2D(x, y));
					x = -1;
				}
				// If the NextPosition is Occupied by a Piece of the same Color, get out of the For
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
	// Array containing all the Possible Moves for a Knight
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
		// If we're within the Board and NextPosition Tile is Empty, add the Position to KnightMovesArray
		if (PiecePosition[0] + Move[0] >= 0 && PiecePosition[0] + Move[0] <= GField->Size && PiecePosition[1] + Move[1] >= 0 && PiecePosition[1] + Move[1] <= GField->Size)
		{
			if (GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))
			{
				KnightMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
			}
			// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the KnightEatingMovesArray
			else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && Color == "White")
			{
				KnightEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
			}
			// If the NextPosition is Occupied by a Piece of the opposing Color, add the Position to the KnightEatingMovesArray
			else if (!(GField->TileIsEmpty(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && IsPieceWhite(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1])) && !(IsPieceBlack(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]))) && Color == "Black")
			{
				KnightEatingMovesArray.Add(FVector2D(PiecePosition[0] + Move[0], PiecePosition[1] + Move[1]));
			}
		}
	}
}

// Called after Moves are generated and added to their specific Array, Spawns the SuggestionTiles
void AChess_GameMode::ShowMoves(const FVector2D& PiecePosition, EPiece Piece, FString Color)
{
	// If the Clicked Piece is a Pawn, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// generate the PossibleMoves and Spawn Moving/Eating SuggestionTiles, then Empty each Array to prepare for the next Selected Pawn
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
	}
	// If the Clicked Piece is a Rook, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// generate the PossibleMoves and Spawn Moving/Eating SuggestionTiles, then Empty each Array to prepare for the next Selected Rook
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
	}
	// If the Clicked Piece is a Bishop, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// generate the PossibleMoves and Spawn Moving/Eating SuggestionTiles, then Empty each Array to prepare for the next Selected Bishop
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
	}
	// If the Clicked Piece is a King, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// generate the PossibleMoves and Spawn Moving/Eating SuggestionTiles, then Empty each Array to prepare for the next Selected Knight
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
	}
	// If the Clicked Piece is a Queen, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// generate the PossibleMoves and Spawn Moving/Eating SuggestionTiles, then Empty each Array to prepare for the next Selected Queen
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
	}
	// If the Clicked Piece is a King, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// generate the PossibleMoves and Spawn Moving/Eating SuggestionTiles, then Empty each Array to prepare for the next Selected King
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
	}
	
}

// Called after the SuggestionTiles are spawned, after its Set to Spawned, we're ready for the second Click from the player
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

// Called to Spawn Moving Tiles to suggest Moves
void AChess_GameMode::SpawnMovTile(const FVector2D& TilePosition, const FVector2D& PiecePosition)
{
	// Get the Positions from the PieceMoveArray and Spawn the YellowTiles Above the BoardTiles 
	FVector Location = GField->GetRelativeLocationByXYPosition(TilePosition[0], TilePosition[1]) + FVector(0, 0, -5);
	ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClassYellow, Location, FRotator::ZeroRotator);
	// Add the Tiles to the SpawnedTileArray and Map
	SpawnedTileArray.Add(Obj);
	SpawnedTileMap.Add(FVector2D(TilePosition[0], TilePosition[1]), Obj);
	// Set the (x,y) Position of the Tiles
	Obj->SetGridPosition(TilePosition[0], TilePosition[1]);
	// Set the Status of the Tile to MOVE, in order to find Moving Tiles more easily
	Obj->SetTileStatus(ETileStatus::MOVE);
	SetPieceMovesToSpawned(PiecePosition);
}

// Called to Spawn Eating Tiles to suggest Moves
void AChess_GameMode::SpawnEatTile(const FVector2D& TilePosition, const FVector2D& PiecePosition)
{
	// Get the Positions from the PieceEAtingMoveArray and Spawn the RedTiles Above the BoardTiles but Below the Pieces
	FVector Location = GField->GetRelativeLocationByXYPosition(TilePosition[0], TilePosition[1]) + FVector(0, 0, -5);
	ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClassRed, Location, FRotator::ZeroRotator);
	// Add the Tiles to the SpawnedTileArray and Map
	SpawnedTileArray.Add(Obj);
	SpawnedTileMap.Add(FVector2D(TilePosition[0], TilePosition[1]), Obj);
	// Set the (x,y) Position of the Tiles
	Obj->SetGridPosition(TilePosition[0], TilePosition[1]);
	// Set the Pieces Above EatingTimes to ToBeEaten, in order to make Eating a Piece easier
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
	// Set the Clicked Piece Moves to Spawned, this way we're ready for the second Click
	SetPieceMovesToSpawned(PiecePosition);
}

// Called after a Pieces have been moved to destory the SuggestionTiles
void AChess_GameMode::DestroyMoveTiles()
{
	// Destroy all of the Spawned Tiles to Clean the Board and Empty the SpawnedTileArray
	for (ATile* Obj : SpawnedTileArray)
	{
		Obj->Destroy();
	}
	SpawnedTileArray.Empty();
}

// Called when the Second Click is a ValidMove and therefore it moves the Piece
void AChess_GameMode::MoveClickedPiece(const FVector2D& NewPosition, FString Color)

// Prima distinguevo tra WhitePiecesArray e BlackPieces Array, quindi mi serviva differenziare tra White e Black nello spostare un pezzo
// ora invece, non servirebbe, ma non modifico il codice per timore che poi non compili piu (visto che non posso verificarlo se non al Laboratorio)
{
	//if (CurrentPlayer == 0)

	// If the Piece Clicked with the first Click is a White, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// Get the BoardPosition of the Clicked Piece and look for the Tile Below it
						FVector2D OldPosition = Obj->GetBoardPosition();
						for (ATile* OldTile : GField->TileArray)
						{
							if (OldTile != NULL)
							{
								// when found, set the Tile to Empty since the Piece is Moving out
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
									// If the New Position is an EatingTile, then Destroy the Pieace to be Eaten and set it NotOnBoard
									if (PieceToEat->GetBoardPosition() == NewPosition && PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
									{
										PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
										PieceToEat->Destroy();
									}
								}
							}
						}
						// reset Checked Tiles
						SetTilesToNotCheckedByColorBeforeTurn(Color);

						// Move the Clicked Piece to the NewPosition, Set the NewPosition and Set the Piece to NotClicked (only one Piece is allowed to be Set on Clicked at time)
						Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
						Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
						Obj->SetPieceStatus(EPieceStatus::NotClicked);

						// Verify Checks after the NewPosition of the MovedPiece
						VerifyChecksByColorAfterTurn(Color);

						// Set the NewTile of the MovedPiece to Occupied
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
		// Destroy the Spawned SuggestionTiles
		DestroyMoveTiles();
	}
	// If the Piece Clicked with the first Click is a Black, verify that it's in the Piece Array, is not NULL and is onBoard
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
						// Get the BoardPosition of the Clicked Piece and look for the Tile Below it
						FVector2D OldPosition = Obj->GetBoardPosition();
						for (ATile* OldTile : GField->TileArray)
						{
							if (OldTile != NULL)
							{
								// when found, set the Tile to Empty since the Piece is Moving out
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
									// If the New Position is an EatingTile, then Destroy the Pieace to be Eaten and set it NotOnBoard
									if (PieceToEat->GetBoardPosition() == NewPosition && /*SpawnedTile->GetTileType() == ETileType::Eat &&*/ PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten)
									{
										PieceToEat->SetIsPieceOnBoard(EPieceOnBoard::NotOnBoard);
										PieceToEat->Destroy();
									}
								}
							}
						}
						// reset Checked Tiles
						SetTilesToNotCheckedByColorBeforeTurn(Color);

						// Move the Clicked Piece to the NewPosition, Set the NewPosition and Set the Piece to NotClicked (only one Piece is allowed to be Set on Clicked at time)
						Obj->SetBoardPosition(NewPosition[0], NewPosition[1]);
						Obj->SetActorLocation(GField->GetRelativeLocationByXYPosition(NewPosition[0], NewPosition[1]) + FVector(0, 0, 10));
						Obj->SetPieceStatus(EPieceStatus::NotClicked);

						// Verify Checks after the NewPosition of the MovedPiece
						VerifyChecksByColorAfterTurn(Color);

						// Set the NewTile of the MovedPiece to Occupied
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
		// Destroy the Spawned SuggestionTiles
		DestroyMoveTiles();
	}
	//TurnNextPlayer();
}

// Called to check if a Piece is Black
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

// Called to check if a Piece is White
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

// Called when the second Click is not a ValidMove, sets Clicked Piece to NotClicked to avoid the Piece moving by mistake
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

// Called After a Piece got Moved to generate all the new Tiles that are threatening for the opposing King
void AChess_GameMode::VerifyChecksByColorAfterTurn(FString Color)
{
	if (Color == "Black")
	{
		// If the last MovedPiece is Black, Empty the BlackChecksArray and Re-Do the Checks
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
							// Generate PawnMoves, since its the Pawns, we're only interested in the EatingMoves, add to BlackChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate QueenMoves, add to BlackChecksArray
						        // Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate RookMoves, add to BlackChecksArray
						        // Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
						        // Generate KnightMoves, add to BlackChecksArray
						        // Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate BishopMoves, add to BlackChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate KingMoves, add to BlackChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
		// Set all the Tiles, which (x,y) Position is contained in BlackChecksArray, to IsCheckedByBlacks
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
		// If the last MovedPiece is White, Empty the BlackChecksArray and Re-Do the Checks
		WhiteChecksArray.Empty();
		for (ABasePiece* Piece : GField->PiecesArray)
		{
			if (Piece != NULL)
			{
				if (Piece->GetIsPieceOnBoard() == EPieceOnBoard::OnBoard)
				{
					if (Piece->IsA(AWhitePiece::StaticClass()))
					{
						// Generate PawnMoves, since its the Pawns, we're only interested in the EatingMoves, add to WhiteChecksArray
						// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate QueenMoves, add to WhiteChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate RookMoves, add to WhiteChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate KnightMoves, add to WhiteChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate BishopMoves, add to WhiteChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
							// Generate KingMoves, add to WhiteChecksArray
							// Empty both the GeneratedArrays to not have Errors when Generating Moves when it's needed to Move a Piece
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
		// Set all the Tiles, which (x,y) Position is contained in WhiteChecksArray, to IsCheckedByWhites
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

// Called Before a Piece is Moved to Reset the Checking Tiles
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

// Called when generating King moves, returns true if a Piece cannot be eaten by King cause its Protected by another Piece
bool AChess_GameMode::IsPieceProtected()
{
	// A rivedere la fuzione, penso che la logica sia errata e che quindi non verifichi se un pezzo è protetto o meno, 
        // purtroppo era una delle ultime funzioni che ho scritto
	// Non Modifico il codice per timore che poi non compili piu, visto che ci posso lavorare solo in Laboratorio
	
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

// get the next player index (non utilizzato)
int32 AChess_GameMode::GetNextPlayer(int32 Player)
{
	Player++;
	if (!Players.IsValidIndex(Player))
	{
		Player = 0;
	}
	return Player;
}

// called at the end of the game turn (non utilizzato)
void AChess_GameMode::TurnNextPlayer()
{
	MoveCounter += 1;
	CurrentPlayer = GetNextPlayer(CurrentPlayer);
	Players[CurrentPlayer]->OnTurn();
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
