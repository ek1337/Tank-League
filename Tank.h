// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();

	APlayerController* GetTankPlayerController() const
	{
		return TankPlayerController;
	}

	bool bAlive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn Stats", meta = (AllowPrivateAccess = "true"))
	float Speed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn Stats", meta = (AllowPrivateAccess = "true"))
	float TurnRate = 150.f;

	void Move(float Value);
	void Turn(float Value);

	APlayerController* TankPlayerController;
};
