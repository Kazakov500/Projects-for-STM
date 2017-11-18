	PRESERVE8							; 8-битное выравнивание стека
	THUMB								; Режим Thumb (AUL) инструкций

	GET	include.s						; include-файлы
	GET	stm32f10x.s	

	AREA RESET, CODE, READONLY

	; Таблица векторов прерываний
	DCD STACK_TOP						; Указатель на вершину стека
	DCD Reset_Handler					; Вектор сброса
		
arr 	% LENGTH_OF_ARRAY * 2

	ENTRY								; Точка входа в программу

Reset_Handler	PROC					; Вектор сброса
	EXPORT  Reset_Handler				; Делаем Reset_Handler видимым вне этого файла

main	
								; Основная подпрограмма
								
	;ADD R0, PC, #primes  ; load address of primes[0] into R0
        ;LDR R1, [R0, #16]    ; load primes[4] into R1
	
	
	;ADD R0, R15, #arr 
	LDR R0, =arr
	MOV R1, #LENGTH_OF_ARRAY
	
	ENDP


BBSort		PROC
	

	mov R5, #1
sort_outer
	cmp R5, #1
	BNE sort_end
	mov R5, #0

	mov R6, R1 ; load array size to R6
	SUB  R0,R0,#1 ;dec R6
	
	

	mov R2, R0 ; load first adress of arr in R2
	
sort_inner
	LDR R3, [R2]
	LDR R4, [R2, #2]
	cmp R3, R4
	BLS nochange
	str R3, [R2, #2] ;mov DWORD PTR [R2+2], R3
	str R4, [R2] ;mov DWORD PTR [R2], R4
	mov R5, #1
nochange
	add R2, #2
	CMP R6, #0
	BLS sort_inner ; Unsigned lower or same R6
	B sort_outer
sort_end
	
	
	ENDP
	
	
    END



