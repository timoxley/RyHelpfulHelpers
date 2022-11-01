﻿// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "RyRuntimeDialogHelpers.generated.h"

UENUM(BlueprintType)
enum class ERyRunOpenFileDialogFlags : uint8
{
    // No flags
    None = 0x00,
    // Allow multiple file selections
    Multiple = 0x01,
};

UENUM(BlueprintType)
enum class ERyRunLastDirectory : uint8
{
    UNR,
    BRUSH,
    FBX,
    FBX_ANIM,
    GENERIC_IMPORT,
    GENERIC_EXPORT,
    GENERIC_OPEN,
    GENERIC_SAVE,
    MESH_IMPORT_EXPORT,
    WORLD_ROOT,
    LEVEL,
    PROJECT,
    NEW_ASSET,
    MAX,
};

/**
* Enumerates message dialog return types.
*/
UENUM(BlueprintType)
enum class ERyRunAppReturnType : uint8
{
	No,
	Yes,
	YesAll,
	NoAll,
	Cancel,
	Ok,
	Retry,
	Continue,
};

/**
* Enumerates supported message dialog button types.
*/
UENUM(BlueprintType)
enum class ERyRunAppMsgType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	YesNoCancel,
	CancelRetryContinue,
	YesNoYesAllNoAll,
	YesNoYesAllNoAllCancel,
	YesNoYesAll,
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions related to system dialogs
*/
UCLASS()
class RYRUNTIME_API URyRuntimeDialogHelpers : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    /// Open a file open dialog box for selecting a file to open
	/// NOTE: Only works on Desktop platforms!
    UFUNCTION(BlueprintCallable, Category = "RyEditor|DialogHelpers")
    static bool OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                               TArray<FString>& OutFilenames, int32& OutFilterIndex,
                               ERyRunOpenFileDialogFlags Flag = ERyRunOpenFileDialogFlags::None,
                               const FString& FileTypes = TEXT("All Files (*)|*|Text Files (*.txt)|*.txt"));

    /// Open a directory select dialog for selecting a directory to use
    /// NOTE: Only works on Desktop platforms!
    UFUNCTION(BlueprintCallable, Category = "RyEditor|DialogHelpers")
    static bool OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName);

    /// Open a save file dialog box for setting a filename or overwrite file path
    /// NOTE: Only works on Desktop platforms!
    UFUNCTION(BlueprintCallable, Category = "RyEditor|DialogHelpers")
    static bool SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, TArray<FString>& OutFilenames,
                               ERyRunOpenFileDialogFlags Flag = ERyRunOpenFileDialogFlags::None,
                               const FString& FileTypes = TEXT("All Files (*)|*|Text Files (*.txt)|*.txt"));

    /// Get the last directory path for a specific type of directory access. Good to pass this value into DefaultPath for OpenFileDialog, SaveFileDialog and OpenDirectoryDialog
    /// NOTE: Only works on Desktop platforms with Editor!
    UFUNCTION(BlueprintPure, Category = "RyEditor|DialogHelpers")
    static FString GetLastDirectory(ERyRunLastDirectory lastDirectory = ERyRunLastDirectory::GENERIC_OPEN);

    /// Open a generic message dialog
    /// NOTE: Only works on Desktop platforms!
    UFUNCTION(BlueprintCallable, Category = "RyEditor|DialogHelpers")
    static ERyRunAppReturnType OpenMessageDialog(ERyRunAppMsgType MessageType, const FText Message, const FText WindowTitle = FText());

    /// Open a debug message dialog
    /// NOTE: Only works on Desktop platforms!
    UFUNCTION(BlueprintCallable, Category = "RyEditor|DialogHelpers")
    static void OpenDebugMessageDialog(const FText Message, const FText WindowTitle = FText());

    /// Opens a dialog containing the details of the supplied class. When the window is closed, an object of the class is returned with whatever edits were made.
    /// This is useful for creating custom import windows, etc.
	/// NOTE: Only works on Desktop platforms with Editor!
    /// NOTE: Once you are finished with the details object, destroy it using ReleaseObjectDetailsDialog or it will stick around for the entirety of the editor session.
    /// @param DetailsClass - The class to create an instance of and show details for
    /// @param WindowTitle - The title text on the window
    /// @param ButtonText - The text on the button
    /// @param ButtonPressedOut - The button was pressed, the window wasn't closed.
    /// @param WindowSize - Default Window Size
    /// @return The object created and edited in the details panel
    UFUNCTION(BlueprintCallable, Category = "RyEditor|DialogHelpers")
    static UObject* OpenObjectDetailsDialog(TSubclassOf<UObject> DetailsClass, 
                                            const FText WindowTitle, 
                                            const FText ButtonText, 
                                            bool& ButtonPressedOut, 
                                            const FVector2D WindowSize = FVector2D(400, 550));
};
