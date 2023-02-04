// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


static TAutoConsoleVariable<int32> CVarMaxNumberOfSimultaneousActiveRecipes
(
    TEXT("GameVars.Requests.MaxNumberOfSimultaneousActiveRecipes"),
    3,
    TEXT("exposed this way as it's not easy to change Subsystem variables without recompiling the game"),
    ECVF_Default
);

static TAutoConsoleVariable<bool> CVarShouldUseSimpleRecipeBook
(
    TEXT("GameVars.Requests.ShouldUseSimpleRecipeBook"),
    false,
    TEXT("true - will use DT_Recipes_Simple as the recipe book \n")
    TEXT("false - will use DT_Recipes as the recipe book"),
    ECVF_SetByConsole
); 