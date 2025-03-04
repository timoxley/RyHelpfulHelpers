﻿// Copyright 2020-2023 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineComponent.h"

#include "RyRuntimeComponentHelpers.generated.h"

//---------------------------------------------------------------------------------------------------------------------
/**
* Static Helper functions related to components in general. Adding extra functionality to components that exist with the
* Unreal Engine.
*/
UCLASS()
class RYRUNTIME_API URyRuntimeComponentHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// Takes an array of points and applies them to the spline by distributing them evenly across the spline.
	// The spline component should be pre-populated.
	// If there are less points than splineComponent points, the points will be interpolated to fill the gap.
	// If there are more points than splineComponent points, you will loose resolution with the spline because decimation will occur.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers")
    static void DistributePointsToSpline(class USplineComponent* splineComponent,
    								     const TArray<FVector>& points,
    								     ESplineCoordinateSpace::Type coordinateSpace,
    								     bool updateSpline = true);

	// Copy the collision properties from one primitive component to another.
	// This does not copy custom profile overrides!
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Primitives")
	static void CopyCollisionProperties(class UPrimitiveComponent* sourceMesh, class UPrimitiveComponent* destMesh);

	// Copy the materials for source mesh to dest mesh.
	// This iterates source meshes materials and assigns them to dest mesh.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Primitives")
	static void CopyMaterials(class UPrimitiveComponent* sourceMesh, class UPrimitiveComponent* destMesh);

	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Primitives")
	static void InvalidateLightingCacheDetailed(UActorComponent* component, bool invalidateBuildEnqueuedLighting, bool translationOnly);

	/**
	 * Query the world and get all components of a specific class
	 * WARNING: This is slow! Don't use this in a tick unless you account for the expense overall.
	 * @param world The world to query
	 * @param componentClass The specific class to look for, use None for "all"
	 * @param componentsOut The found components
	 */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|ComponentHelpers|Query")
	static void GetWorldComponentsByClass(UWorld* world, TSubclassOf<UActorComponent> componentClass, TArray<UActorComponent*>& componentsOut);
};
