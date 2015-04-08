.code

; F32 dot(const vx::float2 &v1, const vx::float2 &v2);
; ret value = xmm0, v1 = rcx, v2 = rdx
?dot@detail@vx@@YAMAEBU?$vec2@M@12@0@Z proc
	movss xmm1, dword ptr[rcx]
	movss xmm2, dword ptr[rdx]

	mulss xmm1, xmm2

	movss xmm0, dword ptr[rcx + 4]
	movss xmm3, dword ptr[rdx + 4]

	VFMADD213SS xmm0, xmm3, xmm1

	ret
?dot@detail@vx@@YAMAEBU?$vec2@M@12@0@Z endp

; F32 dot(const vx::float3 &v1, const vx::float3 &v2);
; ret value = xmm0, v1 = rcx, v2 = rdx
?dot@detail@vx@@YAMAEBU?$vec3@M@12@0@Z proc
	movss xmm1, dword ptr[rcx]
	movss xmm2, dword ptr[rdx]

	mulss xmm1, xmm2

	movss xmm3, dword ptr[rcx + 4]
	movss xmm4, dword ptr[rdx + 4]

	VFMADD213SS xmm3, xmm4, xmm1

	movss xmm0, dword ptr[rcx + 8]
	movss xmm1, dword ptr[rdx + 8]

	VFMADD213SS xmm0, xmm1, xmm3

	ret
?dot@detail@vx@@YAMAEBU?$vec3@M@12@0@Z endp

;F32 VX_CALLCONV dot(const vx::float4 &v1, const vx::float4 &v2);
?dot@detail@vx@@YQMAEBU?$vec4@M@12@0@Z proc

	MOVUPS XMM0, xmmword ptr[RCX]
	MOVUPS XMM1, xmmword ptr[RDX]

	dpps XMM0, XMM1, 0ffh

	ret

?dot@detail@vx@@YQMAEBU?$vec4@M@12@0@Z endp

; F32 VX_CALLCONV dot(float4a v1, float4a v2);
?dot@detail@vx@@YQMT__m128@@0@Z proc
	dpps XMM0, XMM1, 0ffh

	ret

?dot@detail@vx@@YQMT__m128@@0@Z endp

end