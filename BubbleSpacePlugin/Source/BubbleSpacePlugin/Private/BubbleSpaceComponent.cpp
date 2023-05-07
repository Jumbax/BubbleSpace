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

#ifdef UE_BUILD_DEBUG
	DrawShape();
#endif // UE_BUILD_DEBUG
}

void UBubbleSpaceComponent::DoHorizontalRay(const float Offset)
{
	FHitResult HitResult;
	const FVector Start = GetComponentLocation();
	const FVector RotationOffset = GetOwner()->GetActorForwardVector().RotateAngleAxis(Offset, FVector::UpVector) * HorizontalRaysLength;
	const FVector End = Start + RotationOffset;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, Params))
	{
		HorizontalDistances.Add(HitResult.Distance);
	}
	else
	{
		HorizontalDistances.Add(HorizontalRaysLength);
	}

#ifdef UE_BUILD_DEBUG
	if (bDrawHorizontalRays)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 2.f);
	}
#endif // UE_BUILD_DEBUG
}

void UBubbleSpaceComponent::DoHorizontalRays()
{
	float AngleOffset = 0.0f;
	for (uint8 i = 0; i < 4; i++)
	{
		for (uint8 j = 0; j < 3; j++)
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
	const uint8 MinValue = FGenericPlatformMath::Min(HorizontalDistances.Num(), 2);
	for (uint8 i = 0; i < MinValue; i++)
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
	const uint8 MinValue = FGenericPlatformMath::Min(HorizontalAvaragesDistance.Num(), 3);
	for (uint8 i = 0; i < MinValue; i++)
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
	const FVector Start = GetComponentLocation() + GetOwner()->GetActorForwardVector().RotateAngleAxis(Offset, FVector::UpVector) * 100.f;
	const FVector Direction = GetOwner()->GetActorUpVector();
	const FVector End = Start + Direction * VerticalRaysLength;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_WorldStatic, Params))
	{
		VerticalDistances.Add(HitResult.Distance);
	}
	else
	{
		VerticalDistances.Add(VerticalRaysLength);
	}

#ifdef UE_BUILD_DEBUG
	if (bDrawVerticalRays)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 2.f);
	}
#endif // UE_BUILD_DEBUG
}

void UBubbleSpaceComponent::DoVerticalRays()
{
	float AngleOffset = 0.0f;
	for (uint8 j = 0; j < 3; j++)
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
	const uint8 MinValue = FGenericPlatformMath::Min(VerticalDistances.Num(), 3);
	for (uint8 i = 0; i < MinValue; i++)
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
	const float TargetWidth = OldBubbleWidth > 0 ? OldBubbleWidth : HorizontalRaysLength;
	const float TargetHeight = OldBubbleHeight > 0 ? OldBubbleHeight : VerticalRaysLength;
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
		const FVector Start = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 150.f;
		const float ConeSize = FMath::GetMappedRangeValueClamped(FVector2D(0.f, HorizontalRaysLength), FVector2D(1.f, 70.f), BubbleWidth);
		const float ConeLength = FMath::GetMappedRangeValueClamped(FVector2D(0.f, VerticalRaysLength), FVector2D(30.f, 70.f), BubbleHeight);
		DrawDebugCone(GetWorld(), Start, -GetOwner()->GetActorUpVector(), ConeLength, FMath::DegreesToRadians(ConeSize), FMath::DegreesToRadians(ConeSize), 12, FColor::Green, false, -1.f, 0, 2.f);
	}
}

float UBubbleSpaceComponent::GetWidthTargetValue() const
{
	return BubbleWidth / HorizontalRaysLength;
}

float UBubbleSpaceComponent::GetHeightTargetValue() const
{
	return BubbleHeight / VerticalRaysLength;
}

float UBubbleSpaceComponent::GetHorizontalRaysLength() const
{
	return HorizontalRaysLength;
}

void UBubbleSpaceComponent::SetHorizontalRaysLength(const float InHorizontalRaysLength)
{
	if (InHorizontalRaysLength > 0)
	{
		HorizontalRaysLength = InHorizontalRaysLength;
	}
}

float UBubbleSpaceComponent::GetVerticalRaysLength() const
{
	return VerticalRaysLength;
}

void UBubbleSpaceComponent::SetVerticalRaysLength(const float InVerticalRaysLength)
{
	if (InVerticalRaysLength > 0)
	{
		VerticalRaysLength = InVerticalRaysLength;
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

void UBubbleSpaceComponent::ToggleDrawShape()
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