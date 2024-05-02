// Classe non utilizzata perchè avevo riscontrato qualche problema che mi veniva difficile debuggare date le mosse Random e difficili 
// da replicare. Se fossi parito dal gioco a turni entrambi umani, sarebbe stato piu semplice.


#include "Chess_RandomPlayer.h"


// Sets default values
AChess_RandomPlayer::AChess_RandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GameInstance = Cast<UChess_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

// Called when the game starts or when spawned
void AChess_RandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChess_RandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AChess_RandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AChess_RandomPlayer::OnTurn()
{
	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Turn"));
	GameInstance->SetTurnMessage(TEXT("AI (Random) Turn"));

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
			int32 ArraySize = GameMode->GetBlackArraySize();/*GameMode->GField->BPiecesArray.Num()
			if (ArraySize > 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("rand"));
				//int32 RandIdx = FMath::RandRange(0, ArraySize-1);
				GameMode->SetBlackToMove();
				//GField->BPiecesArray[RandIdx]->SetPieceStatus(EPieceStatus::Clicked);
				//GField->BPiecesArray[RandIdx]->SetOldPosition((GField->BPiecesArray[RandIdx]->GetBoardPosition())[0], (GField->BPiecesArray[RandIdx]->GetBoardPosition())[1]);
				GameMode->MoveBlackPiece();
				//int32 RandMove = FMath::RandRange(0, ArraySize); 
				//FVector2D NewPosition = GameMode->BlackMovesArray[RandMove];
			}
		}, 1, false);*/
	/*TArray<ATile*> FreeCells;
	AChess_GameMode* GameMode = (AChess_GameMode*)(GetWorld()->GetAuthGameMode());
	for (auto& CurrTile : GameMode->GField->GetTileArray())
	{
		if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
		{
			FreeCells.Add(CurrTile);
		}
	}

	if (FreeCells.Num() > 0)
	{
		int32 RandIdx = FMath::Rand() % FreeCells.Num();
		FVector Location = GameMode->GField->GetRelativeLocationByXYPosition((FreeCells[RandIdx])->GetGridPosition()[0], (FreeCells[RandIdx])->GetGridPosition()[1]);
		FreeCells[RandIdx]->SetTileStatus(PlayerNumber, ETileStatus::OCCUPIED);

		GameMode->SetCellSign(PlayerNumber, Location);

	}
}, 3, false);*/
}

void AChess_RandomPlayer::OnWin()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Wins!"));
	//GameInstance->SetTurnMessage(TEXT("AI Wins!"));
	//GameInstance->IncrementScoreAiPlayer();
}

void AChess_RandomPlayer::OnLose()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("AI (Random) Loses!"));
	// GameInstance->SetTurnMessage(TEXT("AI Loses!"));
}

