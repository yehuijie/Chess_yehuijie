// Sets up Inputs and Allows to Click

#include "Chess_PlayerController.h"
#include "Components/InputComponent.h"

AChess_PlayerController::AChess_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void AChess_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ChessContext, 0);
	}
}

void AChess_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AChess_PlayerController::ClickOnGrid);
	}
}

// ho provato a implementare i due players, ma con scarso successo
void AChess_PlayerController::ClickOnGrid()
{
	/*const auto HumanPlayer = Cast<AChess_HumanPlayer>(GetPawn());
	if (IsValid(HumanPlayer))
	{
		HumanPlayer->OnClick();
	}
	const auto BlackPlayer = Cast<AChess_BlackPlayer>(GetPawn());
	if (IsValid(BlackPlayer))
	{
		BlackPlayer->OnClick();
	}*/
	AChess_HumanPlayer* HumanPlayer = Cast<AChess_HumanPlayer>(GetPawn());
	AChess_BlackPlayer* BlackPlayer = Cast<AChess_BlackPlayer>(GetPawn());

	if (HumanPlayer)
	{
		HumanPlayer->OnClick();
	}

	if (BlackPlayer)
	{
		BlackPlayer->OnClick();
	}
}
