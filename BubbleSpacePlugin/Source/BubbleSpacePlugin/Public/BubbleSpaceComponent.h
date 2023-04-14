// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BubbleSpaceComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUBBLESPACEPLUGIN_API UBubbleSpaceComponent : public USceneComponent
{

	GENERATED_BODY()

public:
	UBubbleSpaceComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual float GetHorizontalRaysDistance() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
		virtual void SetHorizontalRaysDistance(const float InHorizontalRaysDistance);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual float GetVerticalRaysDistance() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
		virtual void SetVerticalRaysDistance(const float InVerticalRaysDistance);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual bool IsDrawingHorizontalRays() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
		virtual void ShouldDrawHorizontalRays(const bool InShouldDrawHorizontalRays);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual bool IsDrawingVerticalRays() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
		virtual void ShouldDrawVerticalRays(const bool InShouldDrawVerticalRays);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual bool IsDrawingShape() const;

	UFUNCTION(BlueprintCallable, Category = "Settings")
		virtual void ShouldDrawShape(const bool InShouldDrawShape);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual float GetWidthTargetValue() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Settings")
		virtual float GetHeightTargetValue() const;

	UFUNCTION(BlueprintCallable, Category = "Debug")
		virtual void ToggleShape();

	UFUNCTION(BlueprintCallable, Category = "Debug")
		virtual void ToggleDrawHRays();

	UFUNCTION(BlueprintCallable, Category = "Debug")
		virtual void ToggleDrawVRays();

protected:
	virtual void BeginPlay() override;

	virtual void DoHorizontalRay(const float InAngleOffset);

	virtual void DoHorizontalRays();

	virtual void EvaluateHorizontalAvarageDistance();

	virtual float EvaluateHorizontalTargetValue();

	virtual void EvaluateWidth();

	virtual void DoVerticalRay(const float InAngleOffset);

	virtual void DoVerticalRays();

	virtual float EvaluateVerticalTargetValue();

	virtual void EvaluateHeight();

	void DrawShape();

	virtual void LerpWidthHeightTargetValue();

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float HorizontalRaysDistance = 4000.f;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		float VerticalRaysDistance = 1000.f;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		bool bDrawHorizontalRays = false;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		bool bDrawVerticalRays = false;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
		bool bDrawShape = true;

	TArray<float, TFixedAllocator<3>> HorizontalDistances;
	TArray<float, TFixedAllocator<4>> HorizontalAvaragesDistance;
	TArray<float, TFixedAllocator<3>> VerticalDistances;
	float OldBubbleWidth = 0.f;
	float BubbleWidth = 0.f;
	float OldBubbleHeight = 0.f;
	float BubbleHeight = 0.f;
	int32 FrameCount = 16;
};
