	PRESERVE8							; 8-битное выравнивание стека
	THUMB								; Режим Thumb (AUL) инструкций

	GET	include.s						; include-файлы
	GET	stm32f10x.s	

	
	AREA RESET, CODE

	; Таблица векторов прерываний
	DCD STACK_TOP						; Указатель на вершину стека
	DCD Reset_Handler					; Вектор сброса
		
arr 	% LENGTH_OF_ARRAY * 1


	ENTRY								; Точка входа в программу

Reset_Handler	PROC					; Вектор сброса
	EXPORT  Reset_Handler				; Делаем Reset_Handler видимым вне этого файла

main	
								; Основная подпрограмма
								
	LDR R0, =STACK_TOP
	
	MOV R3, #8
	STRB R3, [R0]
	MOV R3, #7
	STRB R3, [R0, #ELEMENT_SIZE]
	MOV R3, #6
	STRB R3, [R0, #ELEMENT_SIZE*2]
	MOV R3, #5
	STRB R3, [R0, #ELEMENT_SIZE*3]
	MOV R3, #4
	STRB R3, [R0, #ELEMENT_SIZE*4]
	MOV R3, #3
	STRB R3, [R0, #ELEMENT_SIZE*5]
	MOV R3, #2
	STRB R3, [R0, #ELEMENT_SIZE*6]
	MOV R3, #1
	STRB R3, [R0, #ELEMENT_SIZE*7]

	
	MOV R1, #LENGTH_OF_ARRAY
	PUSH {r0-r12}
	BL BBSort
	POP {r0-r12}
	
	B end_program
	
	ENDP


BBSort		PROC
	mov R6, R1 ; load array size to R6
	mov R5, #ELEMENT_SIZE
sort_outer
	mov R10, #0
	cmp R5, #ELEMENT_SIZE
	BNE sort_end
	mov R5, #0
	SUB  R6,R6,#1 ;dec R6
	mov R2, R0 ; load first adress of arr in R2
sort_inner
	LDRB R3, [R2]
	LDRB R4, [R2, #ELEMENT_SIZE]
	cmp R3, R4
	BLS nochange
	STRB R3, [R2, #ELEMENT_SIZE] ;mov DWORD PTR [R2+2], R3
	STRB R4, [R2] ;mov DWORD PTR [R2], R4
	mov R5, #ELEMENT_SIZE
	add R10, #ELEMENT_SIZE
nochange
	add R2, #ELEMENT_SIZE
	CMP R10, R6
	BLT sort_inner ;
	B sort_outer
sort_end
	BX	LR
	ENDP

end_program

	END