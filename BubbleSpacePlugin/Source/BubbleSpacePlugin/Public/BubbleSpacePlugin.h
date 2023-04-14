// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FBubbleSpacePluginModule : public IModuleInterface, public FSelfRegisteringExec
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

protected:
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;

	virtual TSharedRef<SDockTab> SpawnBubbleSpaceTab(const FSpawnTabArgs& TabSpawnArgs);
	
	virtual FReply OnClickedToggleShapeButton();
	
	virtual FReply OnClickedToggleDrawHorizontalRaysButton();
	
	virtual FReply OnClickedToggleDrawVerticalRaysButton();
	
	virtual void ToggleShape();
	
	virtual void ToggleDrawHorizontalRays();
	
	virtual void ToggleDrawVerticalRays();
	
	virtual UBubbleSpaceComponent* GetBubbleSpaceComponent();
	
	virtual void OnHorizontalRaysDistanceValueChanged(const float Value);
	
	virtual void OnVerticalRaysDistanceValueChanged(const float Value);
	
	virtual float GetBubbleSpaceHorizontalRaysDistance();
	
	virtual float GetBubbleSpaceVerticalRaysDistance();
	
	virtual float GetBubbleSpaceWidthTargetValue();
	
	virtual float GetBubbleSpaceHeightTargetValue();
};
