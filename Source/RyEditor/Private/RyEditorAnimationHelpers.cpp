// Copyright 2020 Sheffer Online Services. All Rights Reserved.

#include "RyEditorAnimationHelpers.h"
#include "RyEditorModule.h"

#include "Animation/AnimSequence.h"
#include "Animation/AnimMontage.h"
#include "Animation/Skeleton.h"

#include "Misc/MessageDialog.h"

#include "AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "RyEditor"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
UAnimMontage* URyEditorAnimationHelpers::CreateMontageOfAnimations(const TArray<UAnimSequence*>& AnimsIn, const FName MontageName)
{
    if(MontageName.IsNone())
        return nullptr;

    USkeleton *skel = nullptr;
    FString basePackagePath;
    for(const UAnimSequence* section : AnimsIn)
    {
        if(section)
        {
            if(basePackagePath.IsEmpty())
            {
                section->GetPathName(nullptr, basePackagePath);
                basePackagePath = FPaths::GetPath(basePackagePath);
            }
            if(!skel)
            {
                skel = section->GetSkeleton();
            }
            else if(skel != section->GetSkeleton())
            {
                FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_IncompatSkel", "Cannot create montage of sequences with different skeletons!"));
                return nullptr;
            }
        }
    }

    if(!skel || basePackagePath.IsEmpty())
        return nullptr;

    FString assetName = MontageName.ToString() + TEXT("_Montage");
    FString assetPackagePath = FString::Printf(TEXT("%s/%s"), *basePackagePath, *assetName);

    UPackage* assetPackage = FindPackage(nullptr, *assetPackagePath);
    UAnimMontage* montage = nullptr;
    if(assetPackage)
    {
        FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_PackageExists", "A package already exists with your montage name. Choose a different name."));
        return nullptr;
    }
    else
    {
        assetPackage = CreatePackage(NULL, *assetPackagePath);
        if(!assetPackage)
        {
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_NoCreatePackage", "Unable to create package for Montage asset!"));
            return nullptr;
        }

        montage = NewObject<UAnimMontage>(assetPackage, *assetName, RF_Public | RF_Standalone);
        if(!montage)
        {
            FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Montage_NoCreateAsset", "Unable to create Montage asset!"));
            return nullptr;
        }
    }

    // Create track of sequences
    float curTime = 0.0f;
    if(!montage->SlotAnimTracks.Num())
    {
        montage->SlotAnimTracks.AddDefaulted();
    }

    montage->CompositeSections.Empty();
    for(int32 createSeqIndex = 0; createSeqIndex < AnimsIn.Num(); ++createSeqIndex)
    {
        UAnimSequence* sequence = AnimsIn[createSeqIndex];
        if(!sequence)
            continue;

        FAnimSegment newSegment;
        newSegment.AnimReference = sequence;
        newSegment.StartPos = curTime;
        newSegment.AnimStartTime = 0.0f;
        newSegment.AnimEndTime = sequence->SequenceLength;
        newSegment.AnimPlayRate = 1.0f;
        newSegment.LoopingCount = 1;
        montage->AddAnimCompositeSection(sequence->GetFName(), curTime);
        curTime += sequence->SequenceLength;

        montage->SlotAnimTracks[0].AnimTrack.AnimSegments.Add(newSegment);
    }

    montage->SequenceLength = curTime;

    montage->SetSkeleton(skel);

    // Make package dirty.
    montage->MarkPackageDirty();
    montage->PostEditChange();

    // Notify asset registry of new asset
    FAssetRegistryModule::AssetCreated(montage);

    return montage;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorAnimationHelpers::CreateMontageSectionsFromSegments(UAnimMontage* MontageIn)
{
    if(!MontageIn || MontageIn->SlotAnimTracks.Num() == 0)
        return;

    float curTime = 0;
    for(int32 segIndex = 0; segIndex < MontageIn->SlotAnimTracks[0].AnimTrack.AnimSegments.Num(); ++segIndex)
    {
        FAnimSegment& segment = MontageIn->SlotAnimTracks[0].AnimTrack.AnimSegments[segIndex];
        if(segment.AnimReference)
        {
            int32 sectionIndex = MontageIn->GetSectionIndex(segment.AnimReference->GetFName());
            if(sectionIndex == INDEX_NONE)
            {
                // Add the missing section
                MontageIn->AddAnimCompositeSection(segment.AnimReference->GetFName(), curTime);
            }
            else
            {
                // Update the time of the section that exists
                FCompositeSection& section = MontageIn->GetAnimCompositeSection(sectionIndex);
                section.SetTime(curTime, EAnimLinkMethod::Absolute);
            }

            curTime += segment.AnimReference->SequenceLength;
        }
    }

    MontageIn->MarkPackageDirty();
    MontageIn->PostEditChange();
}

#undef LOCTEXT_NAMESPACE
