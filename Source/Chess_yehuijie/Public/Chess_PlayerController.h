
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Chess_HumanPlayer.h"
#include "Chess_BlackPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Chess_PlayerController.generated.h"

/**
 * 
 */

// Declaration of AChess_PlayerController Class, which inherits APlayerController
UCLASS()
class CHESS_YEHUIJIE_API AChess_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
        // Sets default values for this actor's properties
	AChess_PlayerController();

	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* ChessContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	// Called when there's a Click 
        void ClickOnGrid();

protected:
       
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
};
