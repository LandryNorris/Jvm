#ifndef JVM_PROJECT_INSTRUCTIONHELPER_H
#define JVM_PROJECT_INSTRUCTIONHELPER_H

#define INSTR_NOP 0x00
#define INSTR_ACONST_NULL 0x01

#define INSTR_ICONST_M1 0x02 // load -1 onto the stack
#define INSTR_ICONST_0 0x03
#define INSTR_ICONST_1 0x04
#define INSTR_ICONST_2 0x05
#define INSTR_ICONST_3 0x06
#define INSTR_ICONST_4 0x07
#define INSTR_ICONST_5 0x08

#define INSTR_LCONST_0 0x09
#define INSTR_LCONST_1 0x0a

#define INSTR_FCONST_0 0x0b
#define INSTR_FCONST_1 0x0c
#define INSTR_FCONST_2 0x0d

#define INSTR_DCONST_0 0x0e
#define INSTR_DCONST_1 0x0f

#define INSTR_BIPUSH 0x10 // push a byte onto stack
#define INSTR_SIPUSH 0x11 // push a short onto stack

#define INSTR_LDC 0x12   // push a constant from constant pool onto the stack
#define INSTR_LDCW 0x13  // push a constant from constant pool onto the stack using wide index.
#define INSTR_LDC2W 0x14 // push a Long or Double constant from constant pool onto the stack.

#define INSTR_ILOAD 0x15
#define INSTR_LLOAD 0x16
#define INSTR_FLOAD 0x17
#define INSTR_DLOAD 0x18
#define INSTR_ALOAD 0x19

#define INSTR_ILOAD_0 0x1a
#define INSTR_ILOAD_1 0x1b
#define INSTR_ILOAD_2 0x1c
#define INSTR_ILOAD_3 0x1d

#define INSTR_LLOAD_0 0x1e
#define INSTR_LLOAD_1 0x1f
#define INSTR_LLOAD_2 0x20
#define INSTR_LLOAD_3 0x21

#define INSTR_FLOAD_0 0x22
#define INSTR_FLOAD_1 0x23
#define INSTR_FLOAD_2 0x24
#define INSTR_FLOAD_3 0x25

#define INSTR_DLOAD_0 0x26
#define INSTR_DLOAD_1 0x27
#define INSTR_DLOAD_2 0x28
#define INSTR_DLOAD_3 0x29

#define INSTR_ALOAD_0 0x2a
#define INSTR_ALOAD_1 0x2b
#define INSTR_ALOAD_2 0x2c
#define INSTR_ALOAD_3 0x2d

// Load a type from an array: {type}aload
#define INSTR_IALOAD 0x2e
#define INSTR_LALOAD 0x2f
#define INSTR_FALOAD 0x30
#define INSTR_DALOAD 0x31
#define INSTR_AALOAD 0x32 // load an object reference from an array
#define INSTR_BALOAD 0x33 // load Byte/Boolean from an array
#define INSTR_CALOAD 0x34 // load character from an array
#define INSTR_SALOAD 0x35 // load short from an array

// store a type into a local variable
#define INSTR_ISTORE 0x36
#define INSTR_LSTORE 0x37
#define INSTR_FSTORE 0x38
#define INSTR_DSTORE 0x39
#define INSTR_ASTORE 0x3a

#define INSTR_ISTORE_0 0x3b
#define INSTR_ISTORE_1 0x3c
#define INSTR_ISTORE_2 0x3d
#define INSTR_ISTORE_3 0x3e

#define INSTR_LSTORE_0 0x3f
#define INSTR_LSTORE_1 0x40
#define INSTR_LSTORE_2 0x41
#define INSTR_LSTORE_3 0x42

#define INSTR_FSTORE_0 0x43
#define INSTR_FSTORE_1 0x44
#define INSTR_FSTORE_2 0x45
#define INSTR_FSTORE_3 0x46

#define INSTR_DSTORE_0 0x47
#define INSTR_DSTORE_1 0x48
#define INSTR_DSTORE_2 0x49
#define INSTR_DSTORE_3 0x4a

#define INSTR_ASTORE_0 0x4b
#define INSTR_ASTORE_1 0x4c
#define INSTR_ASTORE_2 0x4d
#define INSTR_ASTORE_3 0x4e

// store a type into an array
#define INSTR_IASTORE 0x4f
#define INSTR_LASTORE 0x50
#define INSTR_FASTORE 0x51
#define INSTR_DASTORE 0x52
#define INSTR_AASTORE 0x53
#define INSTR_BASTORE 0x54
#define INSTR_CASTORE 0x55
#define INSTR_SASTORE 0x56

#define INSTR_POP 0x57  // discard the top value of the stack
#define INSTR_POP2 0x58 // discard the top two values on the stack (or 1 if it's a long/double)

#define INSTR_DUP 0x59 // duplicate the item on top of the stack.
#define INSTR_DUP_X1 \
    0x5a // duplicate the top two values on top of the stack. Neither can be a Long/Double.
#define INSTR_DUP_X2 \
    0x5b // duplicate the top of the stack, and place it two or three spots from the top.
#define INSTR_DUP2 0x5c // duplicate top two stack words. Can be Long/Double
#define INSTR_DUP2_X1 0x5d
#define INSTR_DUP2_X2 0x5e

#define INSTR_SWAP 0x5f // swap top two items on the stack.

#define INSTR_IADD 0x60
#define INSTR_LADD 0x61
#define INSTR_FADD 0x62
#define INSTR_DADD 0x63

#define INSTR_ISUB 0x64
#define INSTR_LSUB 0x65
#define INSTR_FSUB 0x66
#define INSTR_DSUB 0x67

#define INSTR_IMUL 0x68
#define INSTR_LMUL 0x69
#define INSTR_FMUL 0x6a
#define INSTR_DMUL 0x6b

#define INSTR_IDIV 0x6c
#define INSTR_LDIV 0x6d
#define INSTR_FDIV 0x6e
#define INSTR_DDIV 0x6f

#define INSTR_IREM 0x70
#define INSTR_LREM 0x71
#define INSTR_FREM 0x72
#define INSTR_DREM 0x73

#define INSTR_INEG 0x74
#define INSTR_LNEG 0x75
#define INSTR_FNEG 0x76
#define INSTR_DNEG 0x77

#define INSTR_ISHL 0x78
#define INSTR_LSHL 0x79

#define INSTR_ISHR 0x7a
#define INSTR_LSHR 0x7b

// https://stackoverflow.com/questions/2811319/difference-between-and
#define INSTR_IUSHR 0x7c
#define INSTR_LUSHR 0x7d

#define INSTR_IAND 0x7e
#define INSTR_LAND 0x7f

#define INSTR_IOR 0x80
#define INSTR_LOR 0x81

#define INSTR_IXOR 0x82
#define INSTR_LXOR 0x83

#define INSTR_IINC 0x84

#define INSTR_I2L 0x85
#define INSTR_I2F 0x86
#define INSTR_I2D 0x87

#define INSTR_L2I 0x88
#define INSTR_L2F 0x89
#define INSTR_L2D 0x8a

#define INSTR_F2I 0x8b
#define INSTR_F2L 0x8c
#define INSTR_F2D 0x8d

#define INSTR_D2I 0x8e
#define INSTR_D2L 0x8f
#define INSTR_D2F 0x90

#define INSTR_I2B 0x91
#define INSTR_I2C 0x92
#define INSTR_I2S 0x93

#define INSTR_LCMP 0x94
#define INSTR_FCMPL 0x95
#define INSTR_FCMPG 0x96

#define INSTR_DCMPL 0x97
#define INSTR_DCMPG 0x98

#define INSTR_IFEQ 0x99
#define INSTR_IFNE 0x9a
#define INSTR_IFLT 0x9b
#define INSTR_IFGE 0x9c
#define INSTR_IFGT 0x9d
#define INSTR_IFLE 0x9e

#define INSTR_IF_ICMPEQ 0x9f
#define INSTR_IF_ICMPNE 0xa0
#define INSTR_IF_ICMPLT 0xa1
#define INSTR_IF_ICMPGE 0xa2
#define INSTR_IF_ICMPGT 0xa3
#define INSTR_IF_ICMPLE 0xa4

#define INSTR_IF_ACMPEQ 0xa5
#define INSTR_IF_ACMPNE 0xa6

#define INSTR_GOTO 0xa7

#define INSTR_JSR 0xa8

#define INSTR_RET 0xa9

#define INSTR_TABLESWITCH 0xaa
#define INSTR_LOOKUPSWITCH 0xab

#define INSTR_IRETURN 0xac
#define INSTR_LRETURN 0xad
#define INSTR_FRETURN 0xae
#define INSTR_DRETURN 0xaf

#define INSTR_ARETURN 0xb0

#define INSTR_RETURN 0xb1

#define INSTR_GETSTATIC 0xb2
#define INSTR_PUTSTATIC 0xb3

#define INSTR_GETFIELD 0xb4
#define INSTR_PUTFIELD 0xb5

#define INSTR_INVOKEVIRTUAL 0xb6
#define INSTR_INVOKESPECIAL 0xb7
#define INSTR_INVOKESTATIC 0xb8
#define INSTR_INVOKEINTERFACE 0xb9
#define INSTR_INVOKEDYNAMIC 0xba

#define INSTR_NEW 0xbb

#define INSTR_NEWARRAY 0xbc
#define INSTR_ANEWARRAY 0xbd
#define INSTR_ARRAYLENGTH 0xbe

#define INSTR_ATHROW 0xbf

#define INSTR_CHECKCAST 0xc0
#define INSTR_INSTANCEOF 0xc1

#define INSTR_MONITORENTER 0xc2
#define INSTR_MONITOREXIT 0xc3

#define INSTR_WIDE 0xc4

#define INSTR_MULTIANEWARRAY 0xc5

#define INSTR_IFNULL 0xc6
#define INSTR_IFNONNULL 0xc7

#define INSTR_GOTO_W 0xc8

#define INSTR_JSR_W 0xc9

#define INSTR_BREAKPOINT 0xca

// cb - fd are reserved

#define INSTR_IMPDEP1 0xfe
#define INSTR_IMPDEP2 0xff

extern char* instructionNames[];
extern int numParams[];

void printProgram(Code* code, const ConstantPool*);

#endif
