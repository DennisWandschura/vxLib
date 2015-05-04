; The MIT License(MIT)

; Copyright(c) 2015 Dennis Wandschura

; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files(the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions :

; The above copyright notice and this permission notice shall be included in all
; copies or substantial portions of the Software.

; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.

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