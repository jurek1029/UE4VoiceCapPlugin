// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoiceRecorderBPLibrary.h"
#include "VoiceRecorder.h"
#include "MyLogger.h"

UVoiceRecorderBPLibrary::UVoiceRecorderBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

UVoiceRecorderComponent* UVoiceRecorderBPLibrary::voiceRercorderCompObject = nullptr;

bool UVoiceRecorderBPLibrary::VoiceRecordedInitFunction(UVoiceRecorderComponent* _voiceRercorderCompObject,
	FString deviceName, int32 sampleRate, int32 numChannels)
{
	voiceRercorderCompObject = _voiceRercorderCompObject;

	if (!voiceRercorderCompObject) {
		MyLogger::LogE(TEXT("value passed for voiceRecorderComponent is nullptr"));
		return false;
	}

	if (!voiceRercorderCompObject->IsInitilaized()) {
		voiceRercorderCompObject->Initialize(deviceName,sampleRate, numChannels);
	}

	if(!voiceRercorderCompObject->IsActive()) {
		voiceRercorderCompObject->Activate();
	}
	return true;
}


bool UVoiceRecorderBPLibrary::VoiceRecordedStartFunction()
{
	if (!voiceRercorderCompObject) {
		MyLogger::LogE(TEXT("voiceRecorderComponent is nullptr"));
		return false;
	}

	return voiceRercorderCompObject->StartCapture();
}

USoundWaveProcedural* UVoiceRecorderBPLibrary::VoiceRecordedStopFunction()
{
	if (!voiceRercorderCompObject) {
		MyLogger::LogE(TEXT("voiceRecorderComponent is nullptr"));
		return nullptr;
	}

	return voiceRercorderCompObject->StopCapture().Get();
}

bool UVoiceRecorderBPLibrary::VoiceRecordedPlayFunction()
{
	if (!voiceRercorderCompObject) {
		MyLogger::LogE(TEXT("voiceRecorderComponent is nullptr"));
		return false;
	}
	return voiceRercorderCompObject->PlayRecording();
}

USoundWaveProcedural* UVoiceRecorderBPLibrary::VoiceRecordedGetSoundWaveFunction() {
	if (!voiceRercorderCompObject) {
		MyLogger::LogE(TEXT("voiceRecorderComponent is nullptr"));
		return nullptr;
	}
	return voiceRercorderCompObject->GetSoundWave().Get();
}

