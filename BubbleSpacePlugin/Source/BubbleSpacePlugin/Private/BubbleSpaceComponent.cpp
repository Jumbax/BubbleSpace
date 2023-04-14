// Fill out your copyright notice in the Description page of Project Settings.

#include "BubbleSpaceComponent.h"

UBubbleSpaceComponent::UBubbleSpaceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBubbleSpaceComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBubbleSpaceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	EvaluateWidth();
	EvaluateHeight();
	LerpWidthHeightTargetValue();
	DrawShape();
}

void UBubbleSpaceComponent::DoHorizontalRay(const float Offset)
{
	FHitResult HitResult;
	FVector Start = GetComponentLocation();
	FVector RotationOffset = GetOwner()->GetActorForwardVector().RotateAngleAxis(Offset, FVector::UpVector) * HorizontalRaysDistance;
	FVector End = Start + RotationOffset;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, Params))
	{
		HorizontalDistances.Add(HitResult.Distance);
	}
	else
	{
		HorizontalDistances.Add(HorizontalRaysDistance);
	}
	if (bDrawHorizontalRays)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 2.f);
	}

}

void UBubbleSpaceComponent::DoHorizontalRays()
{
	float AngleOffset = 0.0f;
	for (int32 i = 0; i < 4; i++)
	{
		for (int32 j = 0; j < 3; j++)
		{
			DoHorizontalRay(AngleOffset);
			AngleOffset += 120.f;
		}
		EvaluateHorizontalAvarageDistance();
		HorizontalDistances.Empty();
		AngleOffset += 30.f;
	}
}

void UBubbleSpaceComponent::EvaluateHorizontalAvarageDistance()
{
	if (HorizontalDistances.Num() <= 0)
	{
		return;
	}
	HorizontalDistances.Sort();
	float AvarageDistance = 0.f;
	int MinValue = FGenericPlatformMath::Min(HorizontalDistances.Num(), 2);
	for (int32 i = 0; i < MinValue; i++)
	{
		AvarageDistance += HorizontalDistances[i];
	}
	AvarageDistance /= MinValue;
	HorizontalAvaragesDistance.Add(AvarageDistance);
}

float UBubbleSpaceComponent::EvaluateHorizontalTargetValue()
{
	if (HorizontalAvaragesDistance.Num() <= 0)
	{
		return 0.f;
	}
	float TargetValue = 0.f;
	HorizontalAvaragesDistance.Sort();
	int MinValue = FGenericPlatformMath::Min(HorizontalAvaragesDistance.Num(), 3);
	for (int32 i = 0; i < MinValue; i++)
	{
		TargetValue += HorizontalAvaragesDistance[i];
	}
	TargetValue /= MinValue;
	return TargetValue;
}

void UBubbleSpaceComponent::EvaluateWidth()
{
	DoHorizontalRays();
	OldBubbleWidth = EvaluateHorizontalTargetValue();
	HorizontalAvaragesDistance.Empty();
}

void UBubbleSpaceComponent::DoVerticalRay(const float Offset)
{
	FHitResult HitResult;
	FVector Start = GetComponentLocation() + GetOwner()->GetActorForwardVector().RotateAngleAxis(Offset, FVector::UpVector) * 100.f;
	FVector Direction = GetOwner()->GetActorUpVector();
	FVector End = Start + Direction * VerticalRaysDistance;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, Params))
	{
		VerticalDistances.Add(HitResult.Distance);
	}
	else
	{
		VerticalDistances.Add(VerticalRaysDistance);
	}
	if (bDrawVerticalRays)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 2.f);
	}
}

void UBubbleSpaceComponent::DoVerticalRays()
{
	float AngleOffset = 0.0f;
	for (int32 j = 0; j < 3; j++)
	{
		DoVerticalRay(AngleOffset);
		AngleOffset += 120.f;
	}
}

float UBubbleSpaceComponent::EvaluateVerticalTargetValue()
{
	if (VerticalDistances.Num() <= 0)
	{
		return 0.f;
	}
	float TargetValue = 0.f;
	VerticalDistances.Sort();
	int MinValue = FGenericPlatformMath::Min(VerticalDistances.Num(), 3);
	for (int32 i = 0; i < MinValue; i++)
	{
		TargetValue += VerticalDistances[i];
	}
	TargetValue /= MinValue;
	return TargetValue;
}

void UBubbleSpaceComponent::EvaluateHeight()
{
	DoVerticalRays();
	OldBubbleHeight = EvaluateVerticalTargetValue();
	VerticalDistances.Empty();
}

void UBubbleSpaceComponent::LerpWidthHeightTargetValue()
{
	float TargetWidth = OldBubbleWidth > 0 ? OldBubbleWidth : HorizontalRaysDistance;
	float TargetHeight = OldBubbleHeight > 0 ? OldBubbleHeight : VerticalRaysDistance;
	FrameCount++;
	if (FrameCount >= 16)
	{
		FrameCount = 0;
	}
	BubbleWidth = FMath::Lerp(BubbleWidth, TargetWidth, (float)FrameCount / 16);
	BubbleHeight = FMath::Lerp(BubbleHeight, TargetHeight, (float)FrameCount / 16);
}

void UBubbleSpaceComponent::DrawShape()
{
	if (bDrawShape)
	{
		FVector Start = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 150.f;
		float ConeSize = FMath::GetMappedRangeValueClamped(FVector2D(0.f, HorizontalRaysDistance), FVector2D(1.f, 70.f), BubbleWidth);
		float ConeLength = FMath::GetMappedRangeValueClamped(FVector2D(0.f, VerticalRaysDistance), FVector2D(30.f, 70.f), BubbleHeight);
		DrawDebugCone(GetWorld(), Start, -GetOwner()->GetActorUpVector(), ConeLength, FMath::DegreesToRadians(ConeSize), FMath::DegreesToRadians(ConeSize), 12, FColor::Green, false, -1.f, 0, 2.f);
	}
}

float UBubbleSpaceComponent::GetWidthTargetValue() const
{
	return BubbleWidth / HorizontalRaysDistance;
}

float UBubbleSpaceComponent::GetHeightTargetValue() const
{
	return BubbleHeight / VerticalRaysDistance;
}

float UBubbleSpaceComponent::GetHorizontalRaysDistance() const
{
	return HorizontalRaysDistance;
}

void UBubbleSpaceComponent::SetHorizontalRaysDistance(const float InHorizontalRaysDistance)
{
	if (InHorizontalRaysDistance > 0)
	{
		HorizontalRaysDistance = InHorizontalRaysDistance;
	}
}

float UBubbleSpaceComponent::GetVerticalRaysDistance() const
{
	return VerticalRaysDistance;
}

void UBubbleSpaceComponent::SetVerticalRaysDistance(const float InVerticalRaysDistance)
{
	if (InVerticalRaysDistance > 0)
	{
		VerticalRaysDistance = InVerticalRaysDistance;
	}
}

bool UBubbleSpaceComponent::IsDrawingHorizontalRays() const
{
	return bDrawHorizontalRays;
}

void UBubbleSpaceComponent::ShouldDrawHorizontalRays(const bool InShouldDrawHorizontalRays)
{
	bDrawHorizontalRays = InShouldDrawHorizontalRays;
}

bool UBubbleSpaceComponent::IsDrawingVerticalRays() const
{
	return bDrawVerticalRays;
}

void UBubbleSpaceComponent::ShouldDrawVerticalRays(const bool InDrawVerticalRays)
{
	bDrawVerticalRays = InDrawVerticalRays;
}

bool UBubbleSpaceComponent::IsDrawingShape() const
{
	return bDrawShape;
}

void UBubbleSpaceComponent::ShouldDrawShape(const bool InShouldDrawShape)
{
	bDrawShape = InShouldDrawShape;
}

void UBubbleSpaceComponent::ToggleShape()
{
	bDrawShape = !bDrawShape;
}

void UBubbleSpaceComponent::ToggleDrawHRays()
{
	bDrawHorizontalRays = !bDrawHorizontalRays;
}

void UBubbleSpaceComponent::ToggleDrawVRays()
{
	bDrawVerticalRays = !bDrawVerticalRays;
}