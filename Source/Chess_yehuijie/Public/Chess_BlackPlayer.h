// Classe creata dopo aver riscontrato problemi con il Random Player, perche volevo almeno che il gioco fosse a turni e che si concludesse
// in realta, non sono riuscito a spawnare correttamente questo BlackPlayer, quindi alla fine ho finito per implementare tutto nell'HumanPlayer
// mi è rimasto questa classe perche non ho fatto in tempo a levarla in Lab

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Chess_PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_BlackPlayer.generated.h"

UCLASS()
class CHESS_YEHUIJIE_API AChess_BlackPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_BlackPlayer();

	// game instance reference
	UChess_GameInstance* GameInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool FirstClick = true;

	bool SecondClick = false;

	// keeps track of turn
	bool IsBlackTurn = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTurn() override;
	virtual void OnWin() override;
	virtual void OnLose() override;

	// called on left mouse click (binding)
	UFUNCTION()
	void OnClick();
};
