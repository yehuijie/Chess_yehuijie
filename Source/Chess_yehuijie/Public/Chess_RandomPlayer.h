// Classe non utilizzata perchè avevo riscontrato qualche problema che mi veniva difficile debuggare date le mosse Random e difficili 
// da replicare. Se fossi parito dal gioco a turni entrambi umani, sarebbe stato piu semplice.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chess_PlayerInterface.h"
#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Gamefield.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_RandomPlayer.generated.h"

UCLASS()
class CHESS_YEHUIJIE_API AChess_RandomPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_RandomPlayer();

	UChess_GameInstance* GameInstance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

};
