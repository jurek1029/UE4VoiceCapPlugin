// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VoiceRecorderComponent.h"
#include "VoiceRecorderBPLibrary.generated.h"
 

UCLASS()
class UVoiceRecorderBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Voice Recorder Init", Keywords = "VoiceRecorder Init "), Category = "VoiceRecorder")
	static bool VoiceRecordedInitFunction(UVoiceRecorderComponent* voiceRercorderCompObject, FString deviceName=TEXT(""), int32 sampleRate=48000, int32 numChannels=2);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Voice Recorder Start", Keywords = "VoiceRecorder Start"), Category = "VoiceRecorder")
	static bool VoiceRecordedStartFunction();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Voice Recorder Stop", Keywords = "VoiceRecorder Stop"), Category = "VoiceRecorder")
	static USoundWaveProcedural* VoiceRecordedStopFunction();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Voice Recorder Play", Keywords = "VoiceRecorder Play"), Category = "VoiceRecorder")
	static bool VoiceRecordedPlayFunction();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Voice Recorder Get SoundWave", Keywords = "VoiceRecorder Get SoundWave"), Category = "VoiceRecorder")
	static USoundWaveProcedural* VoiceRecordedGetSoundWaveFunction();



private:
	static UVoiceRecorderComponent* voiceRercorderCompObject;
};
