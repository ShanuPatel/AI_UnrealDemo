// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacters/FlyingAINPC.h"

void AFlyingAINPC::BeginPlay()
{
	Super::BeginPlay();
	MoveAI();
}
