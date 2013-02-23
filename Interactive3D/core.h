#pragma once
#include <string>
#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

using namespace std;

// Utility macros

// Release a COM interface only if the pointer is not null
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }