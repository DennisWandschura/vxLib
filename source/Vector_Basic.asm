include asm_include.inc

.code

; __m128 VX_CALLCONV loadFloat(const detail::vec2a<F32> &v)
; ret : xmm0
; v* : rcx
?loadFloat@vx@@YQ?AT__m128@@PEBU?$vec2a@M@detail@1@@Z proc

	VMOVQ xmm0, qword ptr[rcx]

	ret

?loadFloat@vx@@YQ?AT__m128@@PEBU?$vec2a@M@detail@1@@Z endp

; void VX_CALLCONV storeFloat(detail::vec2a<F32>* dst, const __m128 V)
; dst* : rcx
; V : xmm0
?storeFloat@vx@@YQXPEAU?$vec2a@M@detail@1@T__m128@@@Z proc

	VMOVQ qword ptr[rcx], xmm0

	ret

?storeFloat@vx@@YQXPEAU?$vec2a@M@detail@1@T__m128@@@Z endp

;  __m128i VX_CALLCONV loadInt(const I32* src);
; dest : xmm0
; src : rcx
?loadInt@vx@@YQ?AT__m128i@@PEBH@Z proc

	VMOVD	xmm0,	dword ptr[rcx]

	ret

?loadInt@vx@@YQ?AT__m128i@@PEBH@Z endp

;  void VX_CALLCONV storeInt(I32* dest, __m128i V);
; dest : rcx
; V : xmm0
?storeInt@vx@@YQXPEAHT__m128i@@@Z proc

	VMOVD	dword ptr[rcx],		xmm0

	ret

?storeInt@vx@@YQXPEAHT__m128i@@@Z endp

;  __m128i VX_CALLCONV loadInt(const U32* src);
; dest : xmm0
; src : rcx
?loadInt@vx@@YQ?AT__m128i@@PEBI@Z proc

	VMOVD	xmm0,	dword ptr[rcx]

	ret

?loadInt@vx@@YQ?AT__m128i@@PEBI@Z endp

;  void VX_CALLCONV storeInt(U32* dest, __m128i V);
; dest : rcx
; V : xmm0
?storeInt@vx@@YQXPEAIT__m128i@@@Z proc

	VMOVD	dword ptr[rcx],		xmm0

	ret

?storeInt@vx@@YQXPEAIT__m128i@@@Z endp

; int2 AddInt2(const int2 &a, const int2 &b);
?addInt2@detail@vx@@YA?AU?$vec2@H@12@AEBU312@0@Z proc
	; return value = rcx, a = rdx, b = r8
	vmovq xmm0, qword ptr[rdx]
	vmovq xmm1, qword ptr[r8]

	paddd xmm0, xmm1

	vmovq qword ptr[rcx], xmm0

	ret
?addInt2@detail@vx@@YA?AU?$vec2@H@12@AEBU312@0@Z endp

; uint2 AddUint2(const uint2 rcx, const uint2 rdx)
?addUint2@detail@vx@@YA?AU?$vec2@I@12@AEBU312@0@Z proc
	; return value = rcx, a = rdx, b = r8

	vmovq xmm0, qword ptr[rdx]
	vmovq xmm1, qword ptr[r8]

	paddd xmm0, xmm1

	vmovq qword ptr[rcx], xmm0

	ret
?addUint2@detail@vx@@YA?AU?$vec2@I@12@AEBU312@0@Z endp

; __m128 AddFloat4(const float4 &rcx, const float4 &rdx);
?addFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z proc

	movups xmm0, xmmword ptr[rdx]
	movups xmm1, xmmword ptr[r8]

	addps xmm0, xmm1

	movups xmmword ptr[rcx], xmm0

	ret
?addFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z endp

; __m128 SubFloat4(const float4 &rcx, const float4 &rdx);
?subFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z proc
	movups xmm0, xmmword ptr[rdx]
	movups xmm1, xmmword ptr[r8]

	subps xmm0, xmm1

	movups xmmword ptr[rcx], xmm0

	ret
?subFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z endp

; __m128 MulFloat4(const float4 &rcx, const float4 &rdx);
?mulFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z proc
	movups xmm0, xmmword ptr[rdx]
	movups xmm1, xmmword ptr[r8]

	mulps xmm0, xmm1

	movups xmmword ptr[rcx], xmm0

	ret
?mulFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z endp

; __m128 DivFloat4(const float4 &rcx, const float4 &rdx);
?divFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z proc
	movups xmm0, xmmword ptr[rdx]
	movups xmm1, xmmword ptr[r8]

	divps xmm0, xmm1

	movups xmmword ptr[rcx], xmm0

	ret
?divFloat4@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z endp

; vx::uint4 min(const vx::uint4 &a, const vx::uint4 &b);
?min@detail@vx@@YA?AU?$vec4@I@12@AEBU312@0@Z proc
	movdqu xmm0, xmmword ptr[rdx]
	movdqu xmm1, xmmword ptr[r8]

	pminud xmm0, xmm1

	movdqu xmmword ptr[rcx], xmm0
	ret
?min@detail@vx@@YA?AU?$vec4@I@12@AEBU312@0@Z endp

; vx::uint4 max(const vx::uint4 &a, const vx::uint4 &b);
?max@detail@vx@@YA?AU?$vec4@I@12@AEBU312@0@Z proc
	movdqu xmm0, xmmword ptr[rdx]
	movdqu xmm1, xmmword ptr[r8]

	pmaxud xmm0, xmm1

	movdqu xmmword ptr[rcx], xmm0
	ret
?max@detail@vx@@YA?AU?$vec4@I@12@AEBU312@0@Z endp

; vx::int4 min(const vx::int4 &a, const vx::int4 &b);
?min@detail@vx@@YA?AU?$vec4@H@12@AEBU312@0@Z proc
	movdqu xmm0, xmmword ptr[rdx]
	movdqu xmm1, xmmword ptr[r8]

	pminsd xmm0, xmm1

	movdqu xmmword ptr[rcx], xmm0
	ret
?min@detail@vx@@YA?AU?$vec4@H@12@AEBU312@0@Z endp

; vx::int4 max(const vx::int4 &a, const vx::int4 &b);
?max@detail@vx@@YA?AU?$vec4@H@12@AEBU312@0@Z proc
	movdqu xmm0, xmmword ptr[rdx]
	movdqu xmm1, xmmword ptr[r8]

	pmaxsd xmm0, xmm1

	movdqu xmmword ptr[rcx], xmm0
	ret
?max@detail@vx@@YA?AU?$vec4@H@12@AEBU312@0@Z endp

; vx::float2 min(
; const vx::float2 &a = rdx, 
; const vx::float2 &b = r8);
; result = rcx
?min@detail@vx@@YA?AU?$vec2@M@12@AEBU312@0@Z proc
	movss xmm0, dword ptr[rdx]
	movss xmm1, dword ptr[rdx + 4]
	UNPCKLPS xmm0, xmm1

	movss xmm1, dword ptr[r8]
	movss xmm2, dword ptr[r8 + 4]
	UNPCKLPS xmm1, xmm2

	minps xmm0, xmm1

	movss dword ptr[rcx], xmm0
	shufps xmm0, xmm0, 1
	movss dword ptr[rcx + 4], xmm0
	ret
?min@detail@vx@@YA?AU?$vec2@M@12@AEBU312@0@Z endp

; vx::float2 max(
; const vx::float2 &a = rdx, 
; const vx::float2 &b = r8);
; result = rcx
?max@detail@vx@@YA?AU?$vec2@M@12@AEBU312@0@Z proc
	movss xmm0, dword ptr[rdx]
	movss xmm1, dword ptr[rdx + 4]
	UNPCKLPS xmm0, xmm1

	movss xmm2, dword ptr[r8]
	movss xmm3, dword ptr[r8 + 4]
	UNPCKLPS xmm2, xmm3

	maxps xmm0, xmm2

	movss dword ptr[rcx], xmm0
	shufps xmm0, xmm0, 1
	movss dword ptr[rcx + 4], xmm0
	ret
?max@detail@vx@@YA?AU?$vec2@M@12@AEBU312@0@Z endp

; vx::float3 min(
; const vx::float3 &a = rdx, 
; const vx::float3 &b = r8);
; result = rcx
?min@detail@vx@@YA?AU?$vec3@M@12@AEBU312@0@Z proc
	movss xmm0, dword ptr[rdx]
	movss xmm2, dword ptr[rdx + 8]
	UNPCKLPS xmm0, xmm2
	movss xmm1, dword ptr[rdx + 4]
	UNPCKLPS xmm0, xmm1

	movss xmm1, dword ptr[r8]
	movss xmm3, dword ptr[r8 + 8]
	UNPCKLPS xmm1, xmm3
	movss xmm2, dword ptr[r8 + 4]
	UNPCKLPS xmm1, xmm2

	minps xmm0, xmm1

	movaps xmm1, xmm0
	movaps xmm2, xmm0

	shufps xmm1, xmm1, 1
	shufps xmm2, xmm2, 2

	movss dword ptr[rcx], xmm0
	movss dword ptr[rcx + 4], xmm1
	movss dword ptr[rcx + 8], xmm2
	ret
?min@detail@vx@@YA?AU?$vec3@M@12@AEBU312@0@Z endp

; vx::float3 max(
; const vx::float3 &a = rdx, 
; const vx::float3 &b = r8);
; result = rcx
?max@detail@vx@@YA?AU?$vec3@M@12@AEBU312@0@Z proc
	movss xmm0, dword ptr[rdx]
	movss xmm2, dword ptr[rdx + 8]
	UNPCKLPS xmm0, xmm2
	movss xmm1, dword ptr[rdx + 4]
	UNPCKLPS xmm0, xmm1

	movss xmm1, dword ptr[r8]
	movss xmm3, dword ptr[r8 + 8]
	UNPCKLPS xmm1, xmm3
	movss xmm2, dword ptr[r8 + 4]
	UNPCKLPS xmm1, xmm2

	maxps xmm0, xmm1

	movaps xmm1, xmm0
	movaps xmm2, xmm0

	shufps xmm1, xmm1, 1
	shufps xmm2, xmm2, 2

	movss dword ptr[rcx], xmm0
	movss dword ptr[rcx + 4], xmm1
	movss dword ptr[rcx + 8], xmm2
	ret
?max@detail@vx@@YA?AU?$vec3@M@12@AEBU312@0@Z endp

; vx::float4 max(
; const vx::float4 &a = rdx, 
; const vx::float4 &b = r8);
; result = rcx
?max@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z proc
	movups xmm0, xmmword ptr[rdx]
	movups xmm1, xmmword ptr[r8]

	maxps xmm0, xmm1

	movups xmmword ptr[rcx], xmm0
	ret
?max@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z endp

; vx::float4 min(
; const vx::float4 &a = rdx, 
; const vx::float4 &b = r8);
; result = rcx
?min@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z proc
	movups xmm0, xmmword ptr[rdx]
	movups xmm1, xmmword ptr[r8]

	minps xmm0, xmm1

	movups xmmword ptr[rcx], xmm0
	ret
?min@detail@vx@@YA?AU?$vec4@M@12@AEBU312@0@Z endp

; vx::float4 VX_CALLCONV abs(const vx::float4 &v);
?abs@detail@vx@@YA?AU?$vec4@M@12@AEBU312@@Z proc
	movups xmm0, xmmword ptr[rdx]
	movaps xmm1, xmmword ptr[g_absMask]

	andnps xmm1, xmm0

	movups xmmword ptr[rcx], xmm1

	ret
?abs@detail@vx@@YA?AU?$vec4@M@12@AEBU312@@Z endp

; __m128 VX_CALLCONV abs(__m128 v);
?abs@detail@vx@@YQ?AT__m128@@T3@@Z proc

	movaps xmm1, xmmword ptr[g_absMask]
	andnps xmm1, xmm0
	movaps xmm0, xmm1

	ret
?abs@detail@vx@@YQ?AT__m128@@T3@@Z endp

?distance2@detail@vx@@YAMAEBU?$vec2@M@12@0@Z proc
	movss xmm2, dword ptr[rcx]
	movss xmm3, dword ptr[rcx + 4]
	unpcklps xmm2, xmm3

	movss xmm0, dword ptr[rdx]
	movss xmm1, dword ptr[rdx + 4]
	unpcklps xmm0, xmm1

	subps xmm0, xmm2

	dpps xmm0, xmm0, 255

	ret
?distance2@detail@vx@@YAMAEBU?$vec2@M@12@0@Z endp

?distance@detail@vx@@YAMAEBU?$vec2@M@12@0@Z proc
	call ?distance2@detail@vx@@YAMAEBU?$vec2@M@12@0@Z

	sqrtps xmm0, xmm0

	ret
?distance@detail@vx@@YAMAEBU?$vec2@M@12@0@Z endp

?distance2@detail@vx@@YAMAEBU?$vec3@M@12@0@Z proc
	movss xmm1, dword ptr[rdx]
	movss xmm2, dword ptr[rdx + 8]
	UNPCKLPS xmm1, xmm2
	movss xmm0, dword ptr[rdx + 4]
	UNPCKLPS xmm1, xmm0

	movss xmm0, dword ptr[rcx]
	movss xmm2, dword ptr[rcx + 8]
	UNPCKLPS xmm0, xmm2
	movss xmm3, dword ptr[rcx + 4]
	UNPCKLPS xmm0, xmm3

	subps xmm0, xmm1

	dpps xmm0, xmm0, 255

	ret
?distance2@detail@vx@@YAMAEBU?$vec3@M@12@0@Z endp

?distance@detail@vx@@YAMAEBU?$vec3@M@12@0@Z proc
	call ?distance2@detail@vx@@YAMAEBU?$vec3@M@12@0@Z

	sqrtps xmm0, xmm0

	ret
?distance@detail@vx@@YAMAEBU?$vec3@M@12@0@Z endp

?distance2@detail@vx@@YAMAEBU?$vec4@M@12@0@Z proc
	movups xmm1, xmmword ptr[rcx]
	movups xmm0, xmmword ptr[rdx]

	subps xmm0, xmm1

	dpps xmm0, xmm0, 255

	ret
?distance2@detail@vx@@YAMAEBU?$vec4@M@12@0@Z endp

?distance@detail@vx@@YAMAEBU?$vec4@M@12@0@Z proc
	call ?distance2@detail@vx@@YAMAEBU?$vec4@M@12@0@Z

	sqrtps xmm0, xmm0

	ret
?distance@detail@vx@@YAMAEBU?$vec4@M@12@0@Z endp

?distance2@detail@vx@@YQMT__m128@@0@Z proc
	subps xmm0, xmm1

	dpps xmm0, xmm0, 255

	ret
?distance2@detail@vx@@YQMT__m128@@0@Z endp

?distance@detail@vx@@YQMT__m128@@0@Z proc
	call ?distance2@detail@vx@@YQMT__m128@@0@Z

	sqrtps xmm0, xmm0

	ret
?distance@detail@vx@@YQMT__m128@@0@Z endp

?length@detail@vx@@YAMAEBU?$vec2@M@12@@Z proc
	movss xmm0, dword ptr[rcx]
	movss xmm1, dword ptr[rcx + 4]
	unpcklps xmm0, xmm1

	dpps xmm0, xmm0, 255

	sqrtps xmm0, xmm0

	ret
?length@detail@vx@@YAMAEBU?$vec2@M@12@@Z endp

?length@detail@vx@@YAMAEBU?$vec3@M@12@@Z proc
	movss xmm0, dword ptr[rcx]
	movss xmm2, dword ptr[rcx + 8]
	UNPCKLPS xmm0, xmm2
	movss xmm3, dword ptr[rcx + 4]
	UNPCKLPS xmm0, xmm3

	dpps xmm0, xmm0, 255
	sqrtps xmm0, xmm0

	ret
?length@detail@vx@@YAMAEBU?$vec3@M@12@@Z endp

?length@detail@vx@@YAMAEBU?$vec4@M@12@@Z proc
	movups xmm0, xmmword ptr[rcx]

	dpps xmm0, xmm0, 255
	sqrtps xmm0, xmm0

	ret
?length@detail@vx@@YAMAEBU?$vec4@M@12@@Z endp

end