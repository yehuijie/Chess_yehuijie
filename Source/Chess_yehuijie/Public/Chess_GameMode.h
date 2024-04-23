// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_PlayerInterface.h"
#include "GameField.h"
#include "GameFramework/GameModeBase.h"// quale dei due?
//#include "GameFramework/GameMode.h"
#include "Chess_GameMode.generated.h"

class AActor;

struct FPosition;
/**
 * 
 */
UCLASS()
class CHESS_YEHUIJIE_API AChess_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// tracks if the game is over
	bool IsGameOver;
	// array of player interfaces
	TArray<IChess_PlayerInterface*> Players;
	int32 CurrentPlayer;
	// tracks the number of moves in order to signal a drawn game -> registo?
	int32 MoveCounter;

	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;

	// field size
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere)
	AGameField* GField;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WPawnActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WRookActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WKnightActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> WBishopActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WQueenActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> WKingActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BPawnActor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BRookActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BKnightActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BBishopActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BQueenActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BKingActor;

	AChess_GameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// called at the start of the game
	void ChoosePlayerAndStartGame();

	// set the cell sign and the position --> move the piece?
	//void SetCellSign(const int32 PlayerNumber, const FVector& SpawnPosition);

	// get the next player index
	int32 GetNextPlayer(int32 Player);

	// called at the end of the game turn
	void TurnNextPlayer();
};
	
