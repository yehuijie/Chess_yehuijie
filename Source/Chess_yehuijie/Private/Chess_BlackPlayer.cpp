// Classe creata dopo aver riscontrato problemi con il Random Player, perche volevo almeno che il gioco fosse a turni e che si concludesse
// in realta, non sono riuscito a spawnare correttamente questo BlackPlayer, quindi alla fine ho finito per implementare tutto nell'HumanPlayer
// mi è rimasto questa classe perche non ho fatto in tempo a levarla in Lab

#include "Chess_BlackPlayer.h"
#include "GameField.h"
#include "BasePiece.h"
#include "Chess_GameMode.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AChess_BlackPlayer::AChess_BlackPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player1;

	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// default init values
	PlayerNumber = -1;
	//Piece = EPiece::None;
}

// Called when the game starts or when spawned
void AChess_BlackPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChess_BlackPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChess_BlackPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChess_BlackPlayer::OnTurn()
{
	IsBlackTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Black Turn"));
	//GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

void AChess_BlackPlayer::OnWin()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Win!"));
	//GameInstance->SetTurnMessage(TEXT("Human Wins!"));
	//GameInstance->IncrementScoreHumanPlayer();
}

void AChess_BlackPlayer::OnLose()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("You Lose!"));
	//GameInstance->SetTurnMessage(TEXT("Human Loses!"));
}

void AChess_BlackPlayer::OnClick()
{
	/*//Structure containing information about one hit of a trace, such as point of impact and surface normal at that point
	FHitResult Hit = FHitResult(ForceInit);
	// GetHitResultUnderCursor function sends a ray from the mouse position and gives the corresponding hit results
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	if (Hit.bBlockingHit && FirstClick && !SecondClick && IsBlackTurn)
	{
		if (ABasePiece* CurrPiece = Cast<ABasePiece>(Hit.GetActor()))
		{
			EPiece CurrPieceType = CurrPiece->GetPiece();
			FVector2D PositionOnClick = CurrPiece->GetBoardPosition();
			if (CurrPiece->IsA(ABlackPiece::StaticClass()) /* && CurrPieceType == EPiece::Pawn)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("pawn"));
				CurrPiece->SetPieceStatus(EPieceStatus::Clicked);
				CurrPiece->SetOldPosition(PositionOnClick[0], PositionOnClick[1]);
				///need to edit
				GameMode->ShowMoves(PositionOnClick, CurrPieceType, "Black");
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("clicked"));
				if (CurrPiece->GetPieceMoves() == EPieceMoves::Spawned)
				{
					FirstClick = false;
					SecondClick = true;
				}
				else
				{
					CurrPiece->SetPieceStatus(EPieceStatus::NotClicked);

				}
				CurrPiece->SetPieceMoves(EPieceMoves::NotSpawned);
			}






			//if (CurrTile->GetTileStatus() == ETileStatus::OCCUPIED)
			//{

				//CurrTile->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);
				//FVector SpawnPosition = CurrTile->GetActorLocation();
				//AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
				//GameMode->SetCellSign(PlayerNumber, SpawnPosition);
				//IsMyTurn = false;
			//}
		}
	}


	if (Hit.bBlockingHit && SecondClick && !FirstClick && IsBlackTurn)
	{
		if (ATile* CurrTile = Cast<ATile>(Hit.GetActor()))
		{
			if (CurrTile->GetTileStatus() == ETileStatus::MOVE)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), OldPosition[0], OldPosition[1]));
				FVector2D NewPosition = CurrTile->GetGridPosition();
				GameMode->MoveClickedPiece(NewPosition);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("second"));
				//GameMode->DestroyMoveTiles();
				//GameMode->DestroyClickedPiece();
				//FVector2D NewPiecePosition = CurrTile->GetGridPosition();
				//GameMode->SpawnPiece(NewPiecePosition);
				SecondClick = false;
				FirstClick = true;
				IsBlackTurn = false;
			}
			else if (CurrTile->GetTileStatus() != ETileStatus::MOVE)
			{
				GameMode->DestroyMoveTiles();
				SecondClick = false;
				FirstClick = true;

			}



		}
		if (ABasePiece* PieceToEat = Cast<ABasePiece>(Hit.GetActor()))
		{
			if (PieceToEat->GetPieceToEat() == EPieceToEat::ToBeEaten) {
				FVector2D NewPosition = PieceToEat->GetBoardPosition();
				GameMode->MoveClickedPiece(NewPosition);
				SecondClick = false;
				FirstClick = true;
				IsBlackTurn = false;

			}
			else if (PieceToEat->GetPieceStatus() != EPieceStatus::Clicked)
			{
				GameMode->SetPieceToNotClicked();
				GameMode->DestroyMoveTiles();
				SecondClick = false;
				FirstClick = true;
			}


		}

	}*/
}



