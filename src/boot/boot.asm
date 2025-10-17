bits 16    ; 16 bits code 
org 0x7c00 ; inform the assembler  of the starting location for th code 

boot:
	mov si, message ; Point SI register to message
	mov ah, 0x0e ; set higher bits to the display character command 

.loop:
	lodsb 		; load the character within the AL register, and increment SI
	cmp al, 0 	; is the AL register  a null byte 
	je halt		; jump to halt 
	int 0x10	; trigger video services interrupt 
  mov si, message;
	jmp .loop 	; loop again

halt:
	hlt		; stop


message:
	db "Test!", 0

; mark as the device bootable 
times 510-($-$$) db 0; add any additional zeroes to make 510 bytes in total
dw 0xAA55 ; write the final 2 bytes as the magic number 0xAA55,  remembering x86 little endian
