.text
.globl aligned
.type aligned, @function
aligned:
    movq %rsp, %rax
    andl $15, %eax
    cmpl $8, %eax
    sete %al
    movzbl %al, %eax
    ret
.section .note.GNU-stack,"",@progbits
