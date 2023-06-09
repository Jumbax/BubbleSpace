// Copyright Epic Games, Inc. All Rights Reserved.

#include "BubbleSpacePlugin.h"
#include "BubbleSpaceComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Input/SSlider.h"

#define LOCTEXT_NAMESPACE "FBubbleSpacePluginModule"

static const FName BubbleSpaceTab = TEXT("BubbleSpaceTab");

void FBubbleSpacePluginModule::StartupModule()
{
	const TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	TabManager->RegisterNomadTabSpawner(BubbleSpaceTab, FOnSpawnTab::CreateRaw(this, &FBubbleSpacePluginModule::SpawnBubbleSpaceTab));
}

void FBubbleSpacePluginModule::ShutdownModule()
{
	const TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	TabManager->UnregisterNomadTabSpawner(BubbleSpaceTab);
}

bool FBubbleSpacePluginModule::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("BubbleSpace")))
	{
		if (FParse::Command(&Cmd, TEXT("Tab")))
		{
			const TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
			TabManager->TryInvokeTab(BubbleSpaceTab);
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("DrawShape")))
		{
			ToggleDrawShape();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("DrawHRays")))
		{
			ToggleDrawHorizontalRays();
			return true;
		}
		else if (FParse::Command(&Cmd, TEXT("DrawVRays")))
		{
			ToggleDrawVerticalRays();
			return true;
		}
	}

	return false;
}

TSharedRef<SDockTab> FBubbleSpacePluginModule::SpawnBubbleSpaceTab(const FSpawnTabArgs& TabSpawnArgs)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight().Padding(8, 8, 8, 8)
		[
			SNew(SBorder).BorderBackgroundColor(FColor::White).Padding(8, 8, 8, 8)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 0, 2, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("ToggleShapeButton", "Toggle Draw Shape"))
		.OnClicked_Raw(this, &FBubbleSpacePluginModule::OnClickedToggleShapeButton)
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 0, 2, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("ToggleDrawHRayButton", "Toggle Draw Horizontal Rays"))
		.OnClicked_Raw(this, &FBubbleSpacePluginModule::OnClickedToggleDrawHorizontalRaysButton)
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 0, 2, 0)
		[
			SNew(SButton)
			.Text(LOCTEXT("ToggleDrawVRayButton", "Toggle Draw Vertical Rays"))
		.OnClicked_Raw(this, &FBubbleSpacePluginModule::OnClickedToggleDrawVerticalRaysButton)
		]
			]
		]
	+ SVerticalBox::Slot().AutoHeight().Padding(8, 8, 8, 8)
		[
			SNew(SBorder).BorderBackgroundColor(FColor::White).Padding(8, 8, 8, 8)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 4, 2, 2)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("HorizontalRayLengthText", "Horizontal Rays Length"))
		]
	+ SVerticalBox::Slot().AutoHeight().Padding(2, 2, 2, 2)
		[
			SNew(SSlider)
			.MinValue(0.1f)
		.MaxValue(10000.f)
		.OnValueChanged_Raw(this, &FBubbleSpacePluginModule::OnHorizontalRaysLengthValueChanged)
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 2, 2, 2)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("VerticalRayLengthText", "Vertical Rays Length"))
		]
	+ SVerticalBox::Slot().AutoHeight().Padding(2, 2, 2, 2)
		[
			SNew(SSlider)
			.MinValue(0.1f)
		.MaxValue(10000.f)
		.OnValueChanged_Raw(this, &FBubbleSpacePluginModule::OnVerticalRaysLengthValueChanged)
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 4, 2, 2)
		[
			SNew(STextBlock).Text_Lambda([this]() -> FText
				{
					return FText::Format(LOCTEXT("HorizontalRayDistanceValue", "Horizontal Rays Length Value: {HorizontalLengthValue}"), GetBubbleSpaceHorizontalRaysLength());
				})
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 4, 2, 2)
		[
			SNew(STextBlock).Text_Lambda([this]() -> FText
				{
					return FText::Format(LOCTEXT("VerticalRayDistanceValue", "Vertical Rays Length Value: {VerticalLengthValue}"), GetBubbleSpaceVerticalRaysLength());
				})
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 4, 2, 2)
		[
			SNew(STextBlock).Text_Lambda([this]() -> FText
				{
					return FText::Format(LOCTEXT("BubbleSpaceWidthValue", "Width Value: {WidthTargetValue}"), GetBubbleSpaceWidthTargetValue());
				})
		]
	+ SVerticalBox::Slot().AutoHeight().HAlign(EHorizontalAlignment::HAlign_Left).Padding(2, 4, 2, 2)
		[
			SNew(STextBlock).Text_Lambda([this]() -> FText
				{
					return FText::Format(LOCTEXT("BubbleSpaceHeightValue", "Height Value: {HeightTargetValue}"), GetBubbleSpaceHeightTargetValue());
				})
		]
			]
		]
		];
}

FReply FBubbleSpacePluginModule::OnClickedToggleShapeButton()
{
	ToggleDrawShape();
	return FReply::Handled();
}

FReply FBubbleSpacePluginModule::OnClickedToggleDrawHorizontalRaysButton()
{
	ToggleDrawHorizontalRays();
	return FReply::Handled();
}

FReply FBubbleSpacePluginModule::OnClickedToggleDrawVerticalRaysButton()
{
	ToggleDrawVerticalRays();
	return FReply::Handled();
}

UBubbleSpaceComponent* FBubbleSpacePluginModule::GetBubbleSpaceComponent()
{
	FWorldContext* WorldContext = GEditor->GetPIEWorldContext(0);
	if (WorldContext)
	{
		UWorld* World = WorldContext->World();
		if (World)
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
			if (PlayerPawn)
			{
				UBubbleSpaceComponent* BubbleSpaceComponent = Cast<UBubbleSpaceComponent>(PlayerPawn->GetComponentByClass(UBubbleSpaceComponent::StaticClass()));
				if (BubbleSpaceComponent)
				{
					return BubbleSpaceComponent;
				}
			}
		}
	}
	return nullptr;
}

void FBubbleSpacePluginModule::OnHorizontalRaysLengthValueChanged(const float Value)
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		BubbleSpaceComponent->SetHorizontalRaysLength(Value);
	}
}

void FBubbleSpacePluginModule::OnVerticalRaysLengthValueChanged(const float Value)
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		BubbleSpaceComponent->SetVerticalRaysLength(Value);
	}
}

void FBubbleSpacePluginModule::ToggleDrawShape()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		BubbleSpaceComponent->ToggleDrawShape();
	}
}

void FBubbleSpacePluginModule::ToggleDrawHorizontalRays()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		BubbleSpaceComponent->ToggleDrawHRays();
	}
}

void FBubbleSpacePluginModule::ToggleDrawVerticalRays()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		BubbleSpaceComponent->ToggleDrawVRays();
	}
}

float FBubbleSpacePluginModule::GetBubbleSpaceHorizontalRaysLength()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		return BubbleSpaceComponent->GetHorizontalRaysLength();
	}

	return 0.f;
}

float FBubbleSpacePluginModule::GetBubbleSpaceVerticalRaysLength()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		return BubbleSpaceComponent->GetVerticalRaysLength();
	}

	return 0.f;
}

float FBubbleSpacePluginModule::GetBubbleSpaceWidthTargetValue()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		return BubbleSpaceComponent->GetWidthTargetValue();
	}

	return 0.f;
}

float FBubbleSpacePluginModule::GetBubbleSpaceHeightTargetValue()
{
	UBubbleSpaceComponent* BubbleSpaceComponent = GetBubbleSpaceComponent();
	if (BubbleSpaceComponent)
	{
		return BubbleSpaceComponent->GetHeightTargetValue();
	}

	return 0.f;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBubbleSpacePluginModule, BubbleSpacePlugin)