# 1 "src/trapEntry.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "src/trapEntry.S"
.global trapEntry
.global trapExit
.type trapEntry @function
.type trapExit @function
.align 4
trapEntry:
  addi sp, sp, -34 * 8
  .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
  sd x\index, \index * 8(sp)
  .endr

  csrr t0, sepc
  sd t0, 32 * 8 (sp)

  csrr t0, sstatus
  sd t0, 33 * 8 (sp)

  mv a5, sp
  call handleSupervisorTrap

trapExit:
  mv sp, a5

  ld t0, 32 * 8 (sp)
  csrw sepc, t0

  ld t0, 33 * 8 (sp)
  csrw sstatus, t0

  .irp index, 1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
  ld x\index, \index * 8(sp)
  .endr

  addi sp, sp, 34 * 8

  sret
