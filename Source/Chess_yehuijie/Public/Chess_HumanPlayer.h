
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Chess_GameInstance.h"
#include "Chess_GameMode.h"
#include "Chess_PlayerInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Chess_HumanPlayer.generated.h"

// Declaration of AChess_HumanPlayer, which inherits APawn and IChess_PlayerInterface
UCLASS()
class CHESS_YEHUIJIE_API AChess_HumanPlayer : public APawn, public IChess_PlayerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AChess_HumanPlayer();

	// camera component attacched to player pawn
	UCameraComponent* Camera;

	// game instance reference
	UChess_GameInstance* GameInstance;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

        // Keeps Track of Clicks
	bool FirstClick = true;
	bool SecondClick = false;

	// keeps track of turn
	bool IsMyTurn = false;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

        // Called when its my Turn
	virtual void OnTurn() override;
        // Called to check if its Win/Lose Position (non utilizzato)
	virtual void OnWin() override;
	virtual void OnLose() override;

	// called on left mouse click (binding)
	UFUNCTION()
	void OnClick();


};
