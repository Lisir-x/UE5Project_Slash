#pragma once
//#include "DrawDebugHelpers.h"

//绘制调试球
#define DRAW_SPHERE(Location) \
	if (GetWorld()) { \
		DrawDebugSphere(GetWorld(), Location, 20.f, 12, FColor::Red, true); \
	}
#define DRAW_SPHERE_COLOR(Location, Color) \
	if (GetWorld()) { \
		DrawDebugSphere(GetWorld(), Location, 10.f, 12, Color, false, 5.f); \
	}
#define DRAW_SPHERE_SingleFrame(Location) \
	if (GetWorld()) { \
		DrawDebugSphere(GetWorld(), Location, 20.f, 12, FColor::Red, false, -1.f); \
	}

//绘制调试线
#define DRAW_LINE(Start, End) \
	if (GetWorld()) { \
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1.f, 0, 1.f); \
	}
#define DRAW_LINE_SingleFrame(Start, End) \
	if (GetWorld()) { \
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1.f, 0, 1.f); \
	}

//绘制调试点
#define DRAW_POINT(Location) \
	if (GetWorld()) { \
		DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Red, true); \
	}
#define DRAW_POINT_SingleFrame(Location) \
	if (GetWorld()) { \
		DrawDebugPoint(GetWorld(), Location, 10.f, FColor::Red, false); \
	}

//绘制调试向量
#define DRAW_VECTOR(Start, End) \
	if (GetWorld()) { \
		DrawDebugDirectionalArrow(GetWorld(), Start, End, 50.f, FColor::Red, true, -1.f, 0, 1.f); \
	}
#define DRAW_VECTOR_SingleFrame(Start, End) \
	if (GetWorld()) { \
		DrawDebugDirectionalArrow(GetWorld(), Start, End, 50.f, FColor::Red, false, -1.f, 0, 1.f); \
	}
