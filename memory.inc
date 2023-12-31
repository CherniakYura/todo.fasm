;; functions to work with memory

;; rdi - int fd
;; rsi - unsigned int n 
write_uint:
    cmp rsi, 0
    je .base_zero

    mov rcx, 10 ;; base 10 divisor
    mov rax, rsi ;; move number to rax
    mov r10, 0 ;; how many digits we processed
.next_digit:
    cmp rax, 0
    je .done
    mov rdx, 0 
    div rcx
    add rdx, '0' ;; convert reminder to ascii
    dec rsp
    mov byte [rsp], dl ;; push reminder to stack
    inc r10
    jmp .next_digit
.done:
    write rdi, rsp, r10
    add rsp, r10
    ret
.base_zero:
    dec rsp
    mov byte [rsp], '0'
    write rdi, rsp, 1
    inc rsp
    ret

;; rdi - int fd
;; rsi - char *buf
write_cstr:
    push rdi
    push rsi
    
    funcall1 strlen, rsi
    mov rdx, rax 

    mov rax, SYS_write
    pop rsi
    pop rdi
    syscall 
    ret

;; rdi - char *buf
strlen:
    xor rax, rax
.next_byte:
    cmp byte [rdi], 0
    je .done

    inc rax
    inc rdi
    jmp .next_byte
.done:
    ret

;; rdi - void *buf
;; rsi - size_t n
parse_uint:
    xor rax, rax
    xor rbx, rbx
    mov rcx, 10
.next_digit:
    cmp rsi, 0
    jle .done

    mov bl, byte [rdi]
    cmp bl, '0'
    jl .done
    cmp bl, '9'
    jg .done
    sub rbx, '0'

    mul rcx
    add rax, rbx

    inc rdi
    dec rsi
    jmp .next_digit
.done:
    ret

;; rdi - void *dest 
;; rsi - void *src
;; rdx - size_t n
memcpy:
.next_byte:
    cmp rdx, 0
    jle .done

    mov al, byte [rsi]
    mov byte [rdi], al

    inc rsi
    inc rdi
    dec rdx

    jmp .next_byte
.done:
    ret

;; rdi - void *buf
;; rsi - size_t n
;; rdx - char c
find_char:
    cmp rsi, 0
    jle .not_found

    mov al, byte [rdi]
    cmp dl, al
    je .found

    inc rdi
    dec rsi
    jmp find_char

.not_found:
    xor rax, rax
    ret
.found:
    mov rax, rdi
    ret

;; rdi - void *text 
;; rsi - size_t text_len
;; rdx - void *prefix
;; r10 - size_t prefix_len
starts_with:
    xor rax, rax
    xor rbx, rbx
.next_char:
    cmp rsi, 0
    jle .done
    cmp r10,0
    jle .done

    mov al, byte [rdi]
    mov bl, byte [rdx]
    cmp rax, rbx
    jne .done

    dec rsi
    inc rdi
    dec r10
    inc rdx
    jmp .next_char

.done:
    cmp r10, 0
    je .yes
.no:
    mov rax, 0
    ret
.yes:
    mov rax, 1
    ret
