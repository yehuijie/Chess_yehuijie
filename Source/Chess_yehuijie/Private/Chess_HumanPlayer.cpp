// Fill out your copyright notice in the Description page of Project Settings.


#include "Chess_HumanPlayer.h"
#include "GameField.h"
#include "BasePiece.h"
#include "Chess_GameMode.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AChess_HumanPlayer::AChess_HumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// create a camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//set the camera as RootComponent
	SetRootComponent(Camera);
	// get the game instance reference
	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// default init values
	PlayerNumber = -1;
	//Piece = EPiece::None;

}

// Called when the game starts or when spawned
void AChess_HumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChess_HumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChess_HumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when its my Turn
void AChess_HumanPlayer::OnTurn()
{
	// Set IsMyTurn to true so that the Click for HumanPlayer works
	IsMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("White Turn"));
	//GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

// Called to check if its Win Position (non utilizzato)
void AChess_HumanPlayer::OnWin()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	//GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	//GameInstance->IncrementScoreHumanPlayer();
}

// Called to check if its Lose Position (non utilizzato)
void AChess_HumanPlayer::OnLose()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	//GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

void AChess_HumanPlayer::OnClick()
{
	//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	// Casts a GameMode within the Current World, if Failed it returns nullptr
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	// If the Click hits something, it's my turn and it's the FirstClick
	if (Hit.bBlockingHit && FirstClick && !SecondClick && IsMyTurn)
	{
		// If a Piece was clicked, save its Type and Position
		if (ABasePiece* CurrPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPiece CurrPieceType = CurrPiece->GetPiece();
			FVector2D PositionOnClick = CurrPiece->GetBoardPosition();
			// If the Piece is a WhitePiece, Set it to Clicked and Show its PossibleMoves
			if (CurrPiece->IsA(AWhitePiece::StaticClass()))
			{
				CurrPiece->SetPieceStatus(EPieceStatus::Clicked);
				// OldPosition non viene usato da nessuna parte, i metodi Get e Set di OldPosition sono inutili
				CurrPiece->SetOldPosition(PositionOnClick[0], PositionOnClick[1]);
				GameMode->ShowMoves(PositionOnClick, CurrPieceType, "White");
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("clicked"));

				// if the SuggestedTiles for the Moves are correcly Spawned, the SecondClick is enabled
				if (CurrPiece->GetPieceMoves() == EPieceMoves::Spawned) 
				{
					FirstClick = false;
					SecondClick = true;
				}
				// if the Tiles are not Spawned, Set the Piece to NotCLicked to avoid it moving by mistake
				else
				{
					CurrPiece->SetPieceStatus(EPieceStatus::NotClicked);

				}
				// SetPieceMoves to NotSpawned regardless, since if Spawned theyre getting Destroyed after SecondClick
				CurrPiece->SetPieceMoves(EPieceMoves::NotSpawned);
			}
		}
	}
	

	// If the Click hits something, it's my turn and it's the SecondClick
	if (Hit.bBlockingHit && SecondClick && !FirstClick && IsMyTurn)
	{
		// if a Tile was Clicked and its a Moving Tile, Save the NewPosition, Move the Piece and pass the turn to the BlackPlayer
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
			if (CurrTile->GetTileStatus() == ETileStatus::MOVE)
			{
				FVector2D NewPosition = CurrTile->GetGridPosition();
				GameMode->MoveClickedPiece(NewPosition, "White");
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("second"));
				SecondClick = false;
				FirstClick = true;
				IsMyTurn = false;
			}
                        // If the Clicked Tile is not a Moving Tile, Destroy the Spawned Tiles, Reset the Click to my Turn and Set the Piece to NotCLicked to avoid it moving by mistake
			else if (CurrTile->GetTileStatus() != ETileStatus::MOVE)
			{
				GameMode->SetPieceToNotClicked();
				GameMode->DestroyMoveTiles();
			        SecondClick = false;
			        FirstClick = true;
			}
		}
		// if a Piece was Clicked and its a Piece that can be Eaten, Save the NewPosition, Eat, Move and pass the Turn to the BlackPLayer
		if (ABasePiece* PieceToEat = Cast<ABasePiece>(Hit.GetActor()))
		{
			if (PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten) 
			{
				FVector2D NewPosition = PieceToEat->GetBoardPosition();
				GameMode->MoveClickedPiece(NewPosition, "White");
			        SecondClick = false;
			        FirstClick = true;
				IsMyTurn = false;
			}
		        // if the Piece Selected is not Clicked and is not ToBeEaten, Destroy the Spawned Tiles, Reset the Click to my Turn and Set the FirstPiece to NotCLicked to avoid it moving by mistake
			else if (PieceToEat->GetPieceStatus() != EPieceStatus::Clicked)
			{
				GameMode->SetPieceToNotClicked();
				GameMode->DestroyMoveTiles();
				SecondClick = false;
				FirstClick = true;
			}	
		}  
	}
	// If the Click hits something, it's Black's turn and it's the FirstClick
	if (Hit.bBlockingHit && FirstClick && !SecondClick && !IsMyTurn)
	{
		// If a Piece was clicked, save its Type and Position
		if (ABasePiece* CurrPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPiece CurrPieceType = CurrPiece->GetPiece();
			FVector2D PositionOnClick = CurrPiece->GetBoardPosition();
			// If the Piece is a BlackPiece, Set it to Clicked and Show its PossibleMoves
			if (CurrPiece->IsA(ABlackPiece::StaticClass()))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("pawn"));
				CurrPiece->SetPieceStatus(EPieceStatus::Clicked);
				// OldPosition non viene usato da nessuna parte, i metodi Get e Set di OldPosition sono inutili
				CurrPiece->SetOldPosition(PositionOnClick[0], PositionOnClick[1]);
				GameMode->ShowMoves(PositionOnClick, CurrPieceType, "Black");
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("clicked"));
				
				// if the SuggestedTiles for the Moves are correcly Spawned, the SecondClick is enabled
				if (CurrPiece->GetPieceMoves() == EPieceMoves::Spawned)
				{
					FirstClick = false;
					SecondClick = true;
				}
				// if the Tiles are not Spawned, Set the Piece to NotCLicked to avoid it moving by mistake
				else
				{
					CurrPiece->SetPieceStatus(EPieceStatus::NotClicked);
				}
				// SetPieceMoves to NotSpawned regardless, since if Spawned theyre getting Destroyed after SecondClick
				CurrPiece->SetPieceMoves(EPieceMoves::NotSpawned);
			}
		}
	}
	// If the Click hits something, it's Black's and it's the SecondClick
	if (Hit.bBlockingHit && SecondClick && !FirstClick && !IsMyTurn)
	{
		// if a Tile was Clicked and its a Moving Tile, Save the NewPosition, Move the Piece and pass the turn to the WhitePlayer(HumanPlayer)
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
			if (CurrTile->GetTileStatus() == ETileStatus::MOVE)
			{
				FVector2D NewPosition = CurrTile->GetGridPosition();
				GameMode->MoveClickedPiece(NewPosition, "Black");
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("second"));
				SecondClick = false;
				FirstClick = true;
				IsMyTurn = true;
			}
		        // if the Piece Selected is not Clicked and is not ToBeEaten, Destroy the Spawned Tiles, Reset the Click to my Turn and Set the FirstPiece to NotCLicked to avoid it moving by mistake
			else if (CurrTile->GetTileStatus() != ETileStatus::MOVE)
			{
				GameMode->SetPieceToNotClicked();
				GameMode->DestroyMoveTiles();
				SecondClick = false;
				FirstClick = true;
			}
		}
		// if a Piece was Clicked and its a Piece that can be Eaten, Save the NewPosition, Eat, Move and pass the Turn to the WhitePlayer(HumanPlayer)
		if (ABasePiece* PieceToEat = Cast<ABasePiece>(Hit.GetActor()))
		{
			if (PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten) {
				FVector2D NewPosition = PieceToEat->GetBoardPosition();
				GameMode->MoveClickedPiece(NewPosition, "Black");
				SecondClick = false;
				FirstClick = true;
				IsMyTurn = true;

			}
			// if the Piece Selected is not Clicked and is not ToBeEaten, Destroy the Spawned Tiles, Reset the Click to my Turn and Set the FirstPiece to NotCLicked to avoid it moving by mistake
			else if (PieceToEat->GetPieceStatus() != EPieceStatus::Clicked)
			{
				GameMode->SetPieceToNotClicked();
				GameMode->DestroyMoveTiles();
				SecondClick = false;
				FirstClick = true;
			}
		}
	}
}
