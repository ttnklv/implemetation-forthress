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

; ( y x -- y & x )
native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

; ( y x -- y | x )
native 'or', or
  pop rax
  or [rsp], rax
  jmp next

; ( a -- a' )
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