#include "MyLogger.h"

DEFINE_LOG_CATEGORY(LogVoiceRecorder);

#define LOG_TO_SCREEN

void MyLogger::Log(FString message) {
#ifdef LOG_TO_SCREEN
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, message);
	}
#endif
	UE_LOG(LogVoiceRecorder, Display, TEXT("%s"), *message);
}

void MyLogger::LogW(FString message) {
#ifdef LOG_TO_SCREEN
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, message);
	}
#endif
	UE_LOG(LogVoiceRecorder, Warning, TEXT("%s"), *message);
}

void MyLogger::LogE(FString message) {
#ifdef LOG_TO_SCREEN
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, message);
	}
#endif
	UE_LOG(LogVoiceRecorder, Error, TEXT("%s"), *message);
}
