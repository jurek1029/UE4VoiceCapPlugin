// Fill out your copyright notice in the Description page of Project Settings.


#include "VoiceRecorderComponent.h"
#include "MyLogger.h"
//#include "VoiceRecorderBPLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UVoiceRecorderComponent::UVoiceRecorderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//deviceName = TEXT("Line 1 (Virtual Audio Cable)");
	deviceName = TEXT("");
	sampleRate = 48000;
	numChannels = 2;
	isInitialized = false;
}

bool UVoiceRecorderComponent::StartCapture() {
	if (!voiceCapture.IsValid()) {
		MyLogger::LogE(TEXT("VoiceCapture is nullptr [UnInitilalized] from StartRecording"));
		return false;
	}


	if(!voiceCapture->IsCapturing()){
		MyLogger::Log(TEXT("Recording started"));
		OnCaptureStart.Broadcast();
		return voiceCapture->Start();
	}
	return false;
}
TSoftObjectPtr<USoundWaveProcedural> UVoiceRecorderComponent::StopCapture() {
	if (!voiceCapture.IsValid()) {
		MyLogger::LogE(TEXT("VoiceCapture is nullptr [UnInitilalized] from StopRecording"));
		return nullptr;
	}

	if (voiceCapture->IsCapturing()) {
		voiceCapture->Stop();
		MyLogger::Log(TEXT("Recording stoped"));
		OnCaptureFinished.Broadcast(soundWave.Get());
		return soundWave;
	}
	return nullptr;
}

bool UVoiceRecorderComponent::PlayRecording() {
	if (!soundWave.IsValid()) {
		MyLogger::LogE(TEXT("SoundWave Object is nullptr"));
		return false;
	}

	UGameplayStatics::PlaySound2D(this, soundWave.Get());
	MyLogger::Log(TEXT("Plaing Recorded sound"));
	return true;
}

void UVoiceRecorderComponent::LoadVoiceModule() {
	if (!FVoiceModule::IsAvailable()) {
		MyLogger::LogW(TEXT("VoiceModule is not loaded, Loading Moule"));
		FModuleManager::LoadModuleChecked<FVoiceModule>("Voice");
	}

	if (FVoiceModule::IsAvailable()) {
		voiceCapture = FVoiceModule::Get().CreateVoiceCapture(deviceName, sampleRate, numChannels);
		MyLogger::Log(TEXT("Created voice capture"));
	}
	else {
		MyLogger::LogE(TEXT("VoiceModule Couldn't be loaded"));
	}
}

bool UVoiceRecorderComponent::Initialize(FString _deviceName = TEXT(""), uint16 _sampleRate = 48000, uint8 _numChannels = 2) {

	if (isInitialized) return true;

	deviceName = _deviceName;
	sampleRate = _sampleRate;
	numChannels = _numChannels;

	if (sampleRate < 1 || sampleRate > 48000) {
		MyLogger::LogW(TEXT("Given Sample rate is incorect (1-48000) defaulted to 48000"));
		sampleRate = 48000;
	}

	if (numChannels < 1 || numChannels > 2) {
		MyLogger::LogW(TEXT("Given numer of channels is incorect (1-2) defaulted to 2"));
		numChannels = 2;
	}

	MyLogger::Log(TEXT("Initializing Voice Recorder Component"));
	soundWave = CastChecked<USoundWaveProcedural>(NewObject<USoundWaveProcedural>());
	if (!soundWave.IsValid()) {
		MyLogger::LogE(TEXT("Could't create USoundWaveProcedural object"));
		return false;
	}

	soundWave->SetSampleRate(sampleRate);
	soundWave->NumChannels = numChannels;
	soundWave->Duration = INDEFINITELY_LOOPING_DURATION;
	soundWave->SoundGroup = SOUNDGROUP_Voice;
	soundWave->bLooping = false;
	soundWave->bCanProcessAsync = true;

	LoadVoiceModule();
	isInitialized = true;
	return true;
}

bool UVoiceRecorderComponent::IsInitilaized() {
	return isInitialized;
}

TSoftObjectPtr<USoundWaveProcedural> UVoiceRecorderComponent::GetSoundWave() {
	return soundWave;
}

void UVoiceRecorderComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVoiceRecorderComponent::QueueRecordedData() {
	if (!voiceCapture.IsValid()) {
		MyLogger::LogE(TEXT("VoiceCapture is nullptr [UnInitilalized] from QueueRecordData"));
		return;
	}
	if (!soundWave.IsValid()) {
		MyLogger::LogE(TEXT("SoundWave Object is nullptr"));
		return;
	}
	if (!voiceCapture->IsCapturing()) return;

	uint32 capturedBytes;
	EVoiceCaptureState::Type CaptureState = voiceCapture->GetCaptureState(capturedBytes);

	if (CaptureState == EVoiceCaptureState::Ok) {
		uint32 readBytes;
		VoiceCaptureBuffer.SetNumUninitialized(capturedBytes);
		EVoiceCaptureState::Type GetState = voiceCapture->GetVoiceData(VoiceCaptureBuffer.GetData(), capturedBytes, readBytes);
		//MyLogger::Log(FString::Printf(TEXT("get state: %s"), EVoiceCaptureState::ToString(GetState)));

		soundWave->QueueAudio(VoiceCaptureBuffer.GetData(), readBytes);
		MyLogger::Log(FString::Printf(TEXT("Captured bytes: %d"), readBytes));
	}
	else if (CaptureState == EVoiceCaptureState::Error) {
		MyLogger::LogE("Voice capture Error");
	}
	else if (CaptureState == EVoiceCaptureState::UnInitialized) {
		MyLogger::LogE("Voice capture Uninitialized");
	}
}

void UVoiceRecorderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	QueueRecordedData();
}

