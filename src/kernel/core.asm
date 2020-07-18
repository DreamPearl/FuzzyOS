[BITS 32]

extern entry_core
global __low_va_args

[SECTION .text]
        CLI
        mov ax, 0x10
        mov es, ax
        mov ss, ax
        mov ds, ax
        mov fs, ax
        mov gs, ax
        jmp entry_core

    __low_va_args:

        mov esi, [esp + 0x4]     ; (index)
        shl esi, 2
        add esi, 4
        mov eax, [ebp + esi]
        ret
