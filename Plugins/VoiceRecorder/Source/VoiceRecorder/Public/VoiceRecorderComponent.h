// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
 
#include "VoiceModule.h"
#include "Sound/SoundWaveProcedural.h"

#include "VoiceRecorderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOICERECORDER_API UVoiceRecorderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVoiceRecorderComponent();
	bool StartCapture();
	TSoftObjectPtr<USoundWaveProcedural> StopCapture();
	bool PlayRecording();
	bool Initialize(FString DeviceName, uint16 SampleRate, uint8 NumChannels);
	bool IsInitilaized();
	TSoftObjectPtr<USoundWaveProcedural> GetSoundWave();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCaptureFinished, USoundWaveProcedural* , soundWave);
	UPROPERTY(BlueprintAssignable, Category = "VoiceRecorderComponent")
		FOnCaptureFinished OnCaptureFinished;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCaptureStart);
	UPROPERTY(BlueprintAssignable, Category = "VoiceRecorderComponent")
		FOnCaptureStart OnCaptureStart;

protected:
	virtual void BeginPlay() override;
	TSharedPtr<IVoiceCapture> voiceCapture;
	TSoftObjectPtr<USoundWaveProcedural> soundWave;

	FString deviceName;
	uint16 sampleRate;
	uint8 numChannels;

	TArray<uint8> VoiceCaptureBuffer;

private:
	void LoadVoiceModule();
	void QueueRecordedData();
	bool isInitialized;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
