#include "Service/SaveSystem/SaveSystemBPFunctionLibrary.h"
#include "HAL/PlatformFileManager.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"

TArray<FSaveSlotInfo> USaveSystemBPFunctionLibrary::GetSaveGameSlots()
{
	TArray<FSaveSlotInfo> SaveSlots;
	IFileManager& FileManager = IFileManager::Get();

	FString SaveDir = FPaths::ProjectSavedDir() + TEXT("SaveGames/");
	FString Extension = TEXT("*.sav");
	
	TArray<FString> FileNames;
	FileManager.FindFiles(FileNames, *(SaveDir + Extension), true, false);

	for (const FString& File : FileNames)
	{
		FString FullPath = SaveDir + File;

		FDateTime ModificationTime = FileManager.GetTimeStamp(*FullPath);
		FString FormattedTime = ModificationTime.ToString(TEXT("%d.%m.%Y %H:%M"));

		FSaveSlotInfo SlotInfo;
		SlotInfo.SlotName = FPaths::GetBaseFilename(File);
		SlotInfo.SaveDateTime = FormattedTime;
		
		SaveSlots.Add(SlotInfo);
	}

	return SaveSlots;
}

UEnvellSaveGame* USaveSystemBPFunctionLibrary::GetSaveInstance(FString Filename)
{
	if (UGameplayStatics::DoesSaveGameExist(Filename, 0))
	{
		USaveGame* LoadedSlot = UGameplayStatics::LoadGameFromSlot(Filename, 0);
		return Cast<UEnvellSaveGame>(LoadedSlot);
	}

	USaveGame* NewSlot = UGameplayStatics::CreateSaveGameObject(UEnvellSaveGame::StaticClass());
	return Cast<UEnvellSaveGame>(NewSlot);
}