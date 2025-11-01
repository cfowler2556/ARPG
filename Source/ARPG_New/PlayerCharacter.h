UCLASS()
class ARPG_API APlayerCharacter : public ACharacter
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* AttackSphere;

    bool bIsAttacking = false;
    float AttackTime = 0.0f;

public:
    APlayerCharacter();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION() void StartAttack();
    UFUNCTION() void OnAttackOverlap(...);  // Full sig as in cpp
    void EndAttack();
};