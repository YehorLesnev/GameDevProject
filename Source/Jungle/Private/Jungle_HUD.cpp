// Fill out your copyright notice in the Description page of Project Settings.


#include "Jungle_HUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"

void AJungle_HUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw crosshair
	if (CrosshairTex)
	{
		// Find the center of our canvas
		FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

		// Offset by half of the texture's dimensions so that the center of the texture aligns with the center of the Canvas
		FVector2D CrosshairDrawPosition(Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f));

		// Draw the crosshair
		FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->GetResource(), FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}