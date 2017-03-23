// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Lifted.h"
#include "LiftedGameMode.h"
#include "LiftedCharacter.h"

#include "GameLiftServerSDK.h"

ALiftedGameMode::ALiftedGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
#if WITH_GAMELIFT
	UE_LOG(LogTemp, Log, TEXT("Setting up gamelift"))

	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

	auto sdkVersion = gameLiftSdkModule->GetSdkVersion();
	if (sdkVersion.IsSuccess())
	{
		UE_LOG(LogTemp, Log, TEXT("SDKVersion [%d] : [%s]"), sdkVersion.IsSuccess(), *sdkVersion.GetResult());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("InitSDK [%d] : [%s] : [%s]"), sdkVersion.IsSuccess(), *sdkVersion.GetError().m_errorName, *sdkVersion.GetError().m_errorMessage);
	}

	auto result = gameLiftSdkModule->InitSDK();
	UE_LOG(LogTemp, Log, TEXT("InitSDK [%d] : [%s] : [%s]"), result.IsSuccess(), *result.GetError().m_errorName, *result.GetError().m_errorMessage);

	auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		UE_LOG(LogTemp, Log, TEXT("OnStartGameSession"))
		auto result = gameLiftSdkModule->ActivateGameSession();
		UE_LOG(LogTemp, Log, TEXT("ActivateGameSession [%d] : [%s] : [%s]"), result.IsSuccess(), *result.GetError().m_errorName, *result.GetError().m_errorMessage);
	};

	FProcessParameters* params = new FProcessParameters();
	params->OnStartGameSession.BindLambda(onGameSession);

	params->OnTerminate.BindLambda([=]() {
		UE_LOG(LogTemp, Log, TEXT("OnTerminate"))
		auto result = gameLiftSdkModule->ProcessEnding(); 
		UE_LOG(LogTemp, Log, TEXT("ProcessEnding [%d] : [%s] : [%s]"), result.IsSuccess(), *result.GetError().m_errorName, *result.GetError().m_errorMessage);
	});

	params->OnHealthCheck.BindLambda([]() {return true; });

	params->port = 7777;

	TArray<FString> logfiles;
	logfiles.Add(TEXT(".\\Lifted\\Saved\\Logs\\Lifted.log"));
	params->logParameters = logfiles;

	result = gameLiftSdkModule->ProcessReady(*params);
	UE_LOG(LogTemp, Log, TEXT("ProcessReady [%d] : [%s] : [%s]"), result.IsSuccess(), *result.GetError().m_errorName, *result.GetError().m_errorMessage);
	UE_LOG(LogTemp, Log, TEXT("Told Gamelift we're ready"))
#endif

}
