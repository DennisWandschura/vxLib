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

include asm_include.inc

.code

; extern F32 invsqrt(F32 number);
?invsqrt@vx@@YAMM@Z proc
	rsqrtss xmm0, xmm0
	ret
?invsqrt@vx@@YAMM@Z endp

; extern F32 abs(F32 a);
?abs@vx@@YQMM@Z proc
	movaps xmm1, xmmword ptr[g_absMask]

	andnps xmm1, xmm0

	movaps xmm0, xmm1

	ret
?abs@vx@@YQMM@Z endp

;extern F32 min(F32 a, F32 b);
?min@vx@@YQMMM@Z proc
	minss xmm0, xmm1
	ret
?min@vx@@YQMMM@Z endp

;extern F32 max(F32 a, F32 b);
?max@vx@@YQMMM@Z proc
	maxss xmm0, xmm1
	ret
?max@vx@@YQMMM@Z endp

end