//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PoolSpawnOptions.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"

#include "PooledCharacter.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UCharacterPool;
class USharedObjectPool;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Pooled Character:: Character Class Template

///  Pooled Object Template;
/// Character type capable of being spawned as member of a Pool and automatically managed by the Pool Component.
UCLASS(ClassGroup = Synaptech, Category = "Performance", Blueprintable, BlueprintType, meta = (DisplayName = "Pool Character", ShortTooltip = "Pool Character Class. Check documentation for more information."))
class OBJPOOL_API APooledCharacter : public ACharacter {
	GENERATED_UCLASS_BODY()
	//
	friend class UCharacterPool;
	friend class USharedObjectPool;
private:
	UPROPERTY()
	UCharacterPool* OwningPool;
	//
	UPROPERTY()
	bool Spawned;
	//
	UPROPERTY()
	FTimerHandle LifeSpanHandle;
protected:
	/// Finishes Spawning this Character, after deferred spawning from the Object-Pool.
	virtual void FinishSpawnFromPool(const bool Reconstruct, const FTransform& Transform);
public:
	virtual void Initialize();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//
	/// How long this Character lives after Spawn from Pool. 0 = Forever.
	UPROPERTY(Category = "Object Pool", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	float LifeSpanPool;
	//
	//
	UFUNCTION()
	void SpawnFromPool(const bool Reconstruct, const FPoolSpawnOptions &SpawnOptions, const FTransform &SpawnTransform);
	//
	/// Returns this Pooled Character to its owning Object-Pool, disabling it.
	UFUNCTION(Category = "Object Pool", BlueprintCallable, meta = (DisplayName = "Return to Pool", Keywords = "return character pool"))
	void ReturnToPool();
	//
	/// Returns reference to the Pool Component owner of this Character.
	UFUNCTION(Category = "Object Pool", BlueprintCallable, meta = (DisplayName = "Get Owning Pool", Keywords = "get owner pool"))
	UCharacterPool* GetOwningPool() const;
	//
	//
	/// Sets Character's Life Span from Pool. When it expires the object will be destroyed. If requested lifespan is 0, the timer is cleared and the actor will not be destroyed.
	UFUNCTION(Category = "Object Pool", BlueprintCallable, meta = (Keywords = "get life span"))
	virtual void SetLifeSpanPool(float InLifespan);
	//
	/// Gets Character's remaining Life Span from Pool. If zero is returned the actor lives forever.
	UFUNCTION(Category = "Object Pool", BlueprintCallable, meta = (Keywords = "get life span"))
	virtual float GetLifeSpanPool() const;
	//
	//
	/// Event called every time this Character is spawned from the Pool.
	UPROPERTY(Category = "Object Pool", BlueprintAssignable)
	FOBJP_PoolEvent OnPoolBeginPlay;
	//
	/// Event called every time this Character is sent back to the Pool.
	UPROPERTY(Category = "Object Pool", BlueprintAssignable)
	FOBJP_PoolEvent OnPoolEndPlay;
	//
	/// Event called every time this Character is spawned from the Pool.
	UFUNCTION(Category = "Object Pool", BlueprintNativeEvent, meta = (DisplayName = "On Pool Begin Play", Keywords = "pool begin play event"))
	void EVENT_OnPoolBeginPlay();
	//
	/// Event called every time this Character is sent back to the Pool.
	UFUNCTION(Category = "Object Pool", BlueprintNativeEvent, meta = (DisplayName = "On Pool End Play", Keywords = "pool end play event"))
	void EVENT_OnPoolEndPlay();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////