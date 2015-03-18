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