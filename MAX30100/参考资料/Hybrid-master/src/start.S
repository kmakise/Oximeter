.cpu cortex-m3
.syntax unified


.section .vectors

.thumb_func
.global _start
_start:
    stacktop: .word 0x20001000
    .word reset
    .word 0
	.word 0
    .word 0
    .word 0
    .word 0	@ -10
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0	@ 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0	@ 10
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0	@ 20
    .word 0
    .word 0
    .word EXTI_59_Handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
	.word 0
	.word 0	@30
    .word 0 @I2C1_EV_Handler
	.word 0
    .word 0
	.word 0
	.word 0
	.word 0
    .word USART1_Handler

