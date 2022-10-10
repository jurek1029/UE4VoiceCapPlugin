// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogVoiceRecorder, Log, All);

#include "CoreMinimal.h"
 
namespace MyLogger {
	void inline Log(FString message);
	void inline LogW(FString message);
	void inline LogE(FString message);
}
