// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/PickupInterface.h"

// Add default functionality here for any IPickupInterface functions that are not pure virtual.

//设置当前重叠的物品
void IPickupInterface::SetOverlappingItem(AItem* Item)
{
}

//添加灵魂
void IPickupInterface::AddSouls(ASoul* Soul)
{
}

//添加黄金
void IPickupInterface::AddGold(ATreasure* Treasure)
{
}

