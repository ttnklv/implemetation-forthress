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