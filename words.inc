;Арифметические операции

; ( y x -- [ x + y ] )
native '+', plus
  pop rdx
  pop rax
  add rax, rdx
  push rax
  jmp next

; ( y x -- [x - y] )
native '-', minus
  pop rdx
  pop rax
  sub rax, rdx
  push rax
  jmp next

; ( y x -- [ x * y ] )
native '*', mul
  pop rdx
  pop rax
  imul rdx
  push rax
  jmp next

; ( y x -- [ x / y ] )
native '/', div
  pop rax
  pop rcx
  cqo
  idiv rcx
  push rax
  jmp next

; ( y x -- [ x mod y ] )
native "%", mod
  pop rcx
  pop rax
  cqo
  idiv rcx
  push rdx
  jmp next

; ( y x -- [x < y] )
native '<', less
  pop rdx
  pop rax
  cmp rax, rdx
  setl al
  movzx rax, al
  push rax
  jmp next

;Стековые комбинаторы

; ( a -- )
native 'drop', drop
  add rsp, 8
  jmp next

; ( a b -- b a )
native 'swap', swap
    pop rdx
    pop rax
    push rdx
    push rax
    jmp next

; ( a -- a a )
native 'dup', dup
  push qword [rsp]
  jmp next

;Логические операции

native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

native 'or', or
  pop rax
  or [rsp], rax
  jmp next


native 'not', not
  pop rax
  test rax, rax
  jz .setone
  xor rax, rax
  push rax
  jmp next
  .setone:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next

native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next

native "land", land
  pop rax
  pop rdx
  test rax, rax
  jz .no
  push rdx
  jmp next
  .no:
    push rax
    jmp next

; Работа со стеком возвратов

native '>r', to_r
  pop rax
  rpush rax
  jmp next

native 'r>', from_r
  rpop rax
  push rax
  jmp next

native "r@", r_fetch
  push qword [rstack]
  jmp next

; Управление выполнением

native 'docol', docol
  sub rstack, 8
  mov [rstack], pc
  add w, 8
  mov pc, w
  jmp next

native 'branch', branch
    mov pc, [pc]
    jmp next

native 'branchifz', branchifz
  pop rax
  test rax, rax
  jnz .skip
  mov pc, [pc]
  jmp next
  .skip:
    add pc, 8
    jmp next

native 'isbranch', isbranch
  pop rax
  push rax
  cmp byte[rax - 1], 2
  jne .continue
  mov byte[was_branch], 1
  .continue:
  jmp next

native 'lit', lit
    push qword [pc]
    add pc, 8
    jmp next

native 'pushlit', pushlit
  push xt_lit
  jmp next

native 'unsetbranch', unsetbranch
  mov byte[was_branch], 0
  jmp next

native 'saveword', saveword
  pop rax
  mov  qword[here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  add qword[here], rax
  jmp next

native 'savenum', savenum
  pop rax
  mov [here], rax
  xor eax, eax
  mov rax, here
  add rax, word_size
  mov qword[here], rax
  jmp next

native 'wasbranch', wasbranch
  xor rdx, rdx
  mov dl, byte[was_branch]
  push rdx
  jmp next

native 'pushmode', pushmode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

native 'buffer', buffer
  push qword input_buf
  jmp next

native 'comp_m', comp_m
  mov  rdi, cmode_message
  call print_string
  call print_newline
  jmp next

native 'inte_m', inte_m
  mov  rdi, imode_message
  call print_string
  call print_newline
  jmp next

native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native 'create', create
  ; генерация ссылки
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8

  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  ; флаги
  inc rsi
  pop rax
  mov [rsi], al
  inc rsi

  mov [here], rsi
  jmp next


native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next

native 'warn', warn
  mov  rdi, warning_message
  call print_string
  call print_newline
  mov  pc, xt_loop
  jmp next

native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next

native 'read', read
  pop rdi
  call read_word
  push rdx
  jmp next

native 'prints', prints
  pop rdi
  call print_string
  jmp next

native 'printnl', printnl
  call print_newline
  jmp next

native 'parsei', parsei
  call parse_int
  push rax
  push rdx
  jmp next

native 'compare', compare
  pop rdi
  pop rsi
  call string_equals
  push rax
  jmp next

native 'key', key
  call read_char
  push rax
  jmp next

native 'emit', emit
  pop rdi
  call print_char
  jmp next

native 'save_lw', save_lw
  push last_word
  jmp next

native 'cfa', cfa
  pop rdi
  call cfa
  push rax
  jmp next

native 'find', find
  pop rdi
  call find_word
  push rax
  jmp next

native 'buf', buf
  push qword user_buf
  jmp next

native 'rot', rot
  pop rax
  pop rdx
  pop rcx
  push rdx
  push rcx
  push rax
  jmp next

native '.', dot
  pop rdi
  call print_int
  jmp next

native '.S', show_stack
  mov rcx, rsp
  .loop:
    cmp rcx, [stack_start]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

native '!', write
  pop rax
  pop rdx
  mov [rax], rdx
  jmp next

native '@', fetch
  pop rax
  push qword[rax]
  jmp next

native 'c!', write_char
  pop rax
  pop rdx
  mov [rax], dl
  jmp next

native 'c@', fetch_char
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

native 'pushmode_addr', pushmode_addr
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

native 'isimmediate', isimmediate
  pop rax
  push rax
  xor rdx, rdx
  mov dl, byte[rax - 1]
  push rdx
  jmp next

native 'initcmd', initcmd
  pop rax
  push pc
  mov [program_stub], rax
  mov pc, program_stub
  mov w, [pc]
  pop pc
  jmp [w]

native '=', equals
  pop rdx
  pop rax
  cmp rax, rdx
  jne .nequals
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
  .nequals:
  xor rax, rax
  push rax
  jmp next
