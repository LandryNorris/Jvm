#include "attributeloader.h"
#include "instructionhelper.h"
#include <stdio.h>
#include <primitivereader.h>
#include <malloc.h>

void printProgram(Code* code) {
    const uint8_t* pc = code->program->byteCode;
    const uint8_t* start = pc;
    const uint8_t* end = pc + code->program->length;

    while(pc < end) {
        uint8_t instruction = *pc;
        long index = pc-start;
        char* name = instructionNames[instruction];
        switch(instruction) {
            //1 param
            case INSTR_ALOAD:
            case INSTR_ASTORE:
            case INSTR_BIPUSH:
            case INSTR_DLOAD:
            case INSTR_DSTORE:
            case INSTR_FLOAD:
            case INSTR_FSTORE:
            case INSTR_ILOAD:
            case INSTR_ISTORE:
            case INSTR_LDC:
            case INSTR_LLOAD:
            case INSTR_LSTORE:
            case INSTR_RET:
                printf("\t%5ld: %s %d\n", index, name, *++pc);
                break;

            //2 params
            case INSTR_ANEWARRAY:
            case INSTR_CHECKCAST:
            case INSTR_GETFIELD:
            case INSTR_GETSTATIC:
            case INSTR_GOTO:
            case INSTR_IFEQ:
            case INSTR_IFGE:
            case INSTR_IFGT:
            case INSTR_IFLE:
            case INSTR_IFLT:
            case INSTR_IFNE:
            case INSTR_IFNONNULL:
            case INSTR_IFNULL:
            case INSTR_IF_ACMPEQ:
            case INSTR_IF_ACMPNE:
            case INSTR_IF_ICMPEQ:
            case INSTR_IF_ICMPGE:
            case INSTR_IF_ICMPGT:
            case INSTR_IF_ICMPLE:
            case INSTR_IF_ICMPLT:
            case INSTR_IF_ICMPNE:
            case INSTR_IINC:
            case INSTR_INSTANCEOF:
            case INSTR_INVOKESPECIAL:
            case INSTR_INVOKEVIRTUAL:
            case INSTR_JSR:
            case INSTR_LDCW:
            case INSTR_LDC2W:
            case INSTR_NEW:
            case INSTR_PUTFIELD:
            case INSTR_PUTSTATIC:
            case INSTR_SIPUSH:
            case INSTR_INVOKESTATIC: {
                uint8_t byte1 = *++pc;
                uint8_t byte2 = *++pc;
                printf("\t%5ld: %s %d %d\n", index, name, byte1, byte2);
                break;
            }

            //these are variable size. How should I handle them?
            case INSTR_TABLESWITCH: {
                uint32_t currentInstruction = pc-start;
                uint8_t instructionOffset = currentInstruction & 0b11; //how far above a multiple of 4 our first instruction is.
                uint8_t offset = 4-(instructionOffset); //align to 4 spots.
                if(offset != 4) {
                    pc += offset;
                }
                int32_t defaultByte = readInt32(&pc);
                int32_t low = readInt32(&pc);
                int32_t high = readInt32(&pc);

                int32_t numOffsets = high - low + 1;

                int32_t* offsets = malloc(sizeof(int32_t) * numOffsets);
                int j = 0;

                for(int i = 0; i < numOffsets; i++) {
                    offsets[j++] = readInt32(&pc) + (int32_t) currentInstruction;
                }
                printf("\t%5ld %s {\n", index, name);

                for(int i = 0; i < numOffsets; i++) {
                    printf("\t\t%12d: %d\n", i, offsets[i]);
                }
                printf("\t\t     default: %d\n\t       }\n", defaultByte);
                free(offsets);
                break;
            }

            case INSTR_LOOKUPSWITCH: {
                uint32_t currentInstruction = pc-start;
                uint8_t instructionOffset = currentInstruction & 0b11; //how far above a multiple of 4 our first instruction is.
                uint8_t offset = 4-(instructionOffset); //align to 4 spots.
                pc += offset;
                uint32_t defaultByte = readuInt32(&pc) + currentInstruction;
                uint32_t numPairs = readuInt32(&pc);

                int32_t* matches = malloc(sizeof(int32_t) * numPairs);
                int32_t* offsets = malloc(sizeof(int32_t) * numPairs);;
                int j = 0;
                for(int i = 0; i < numPairs; i++) {
                    matches[j] = readInt32(&pc);
                    offsets[j] = readInt32(&pc) + currentInstruction;
                    j++;
                }
                printf("\t%5ld: %s { //%d\n", index, name, numPairs);
                for(int i = 0; i < numPairs; i++) {
                    printf("\t\t%12d: %d\n", matches[i], offsets[i]);
                }
                printf("\t\t     default: %d\n\t       }\n", defaultByte);
                free(offsets);
                break;
            }
            case INSTR_WIDE: {
                uint8_t opcode = *++pc;
                switch(opcode) {
                    case INSTR_ILOAD:
                    case INSTR_LLOAD:
                    case INSTR_FLOAD:
                    case INSTR_DLOAD:
                    case INSTR_ALOAD:
                    case INSTR_ISTORE:
                    case INSTR_LSTORE:
                    case INSTR_FSTORE:
                    case INSTR_DSTORE:
                    case INSTR_ASTORE:
                    case INSTR_RET: {
                        uint8_t byte1 = *++pc;
                        uint8_t byte2 = *++pc;
                        uint16_t value = byte2 << 8 & byte1;
                        printf("\t%5ld: %s %d\n", index, name, value);
                    }
                }
                break;
            }

            //3 params
            case INSTR_MULTIANEWARRAY:
                printf("\t%5ld: %s %d %d %d", index, name, *++pc, *++pc, *++pc);

            //4 params
            case INSTR_GOTO_W:
            case INSTR_INVOKEDYNAMIC:
            case INSTR_INVOKEINTERFACE:
            case INSTR_JSR_W:
                printf("\t%5ld: %s %d %d %d %d\n", index, name, *++pc, *++pc, *++pc, *++pc);
                break;

            //no params
            default:
                printf("\t%5ld: %s\n", index, name);
        }
        pc++;
    }
}

char* instructionNames[] = {
        "nop", "aconstnull", "iconst_m1", "iconst_0", "iconst_1", "iconst_2", "iconst_3", "iconst_4",
        "iconst_5", "lconst_0", "lconst_1", "fconst_0", "fconst_1", "fconst_2", "dconst_0", "dconst_1",
        "bipush", "sipush", "ldc", "ldcw", "ldc2w", "iload", "lload", "fload", "dload", "aload", "iload_0",
        "iload_1", "iload_2", "iload_3", "lload_0", "lload_1", "lload_2", "lload_3", "fload_0", "fload_1",
        "fload_2", "fload_3", "dload_0", "dload_1", "dload_2", "dload_3", "aload_0", "aload_1", "aload_2",
        "aload_3", "iaload", "laload", "faload", "daload", "aaload", "baload", "caload", "saload", "istore",
        "lstore", "fstore", "dstore", "astore", "istore_0", "istore_1", "istore_2", "istore_3", "lstore_0",
        "lstore_1", "lstore_2", "lstore_3", "fstore_0", "fstore_1", "fstore_2", "fstore_3", "dstore_0",
        "dstore_1", "dstore_2", "dstore_3", "astore_0", "astore_1", "astore_2", "astore_3", "iastore",
        "lastore", "fastore", "dastore", "aastore", "bastore", "castore", "sastore", "pop", "pop2", "dup",
        "dup_x1", "dup_x2", "dup2", "dup2_x1", "dup2_x2", "swap", "iadd", "ladd", "fadd", "dadd", "isub",
        "lsub", "fsub", "dsub", "imul", "lmul", "fmul", "dmul", "idiv", "ldiv", "fdiv", "ddiv", "irem",
        "lrem", "frem", "drem", "ineg", "lneg", "fneg", "dneg", "ishl", "lshl", "ishr", "lshr", "iushr",
        "lushr", "iand", "land", "ior", "lor", "ixor", "lxor", "iinc", "i2l", "i2f", "i2d", "l2i", "l2f",
        "l2d", "f2i", "f2l", "f2d", "d2i", "d2l", "d2f", "i2b", "i2c", "i2s", "lcmp", "fcmpl", "fcmpg",
        "dcmpl", "dcmpg", "ifeq", "ifne", "iflt", "ifge", "ifgt", "ifle", "if_icmpeq", "if_icmpne",
        "if_icmplt", "if_icmpge", "if_icmpgt", "if_icmple", "if_acmpeq", "if_acmpne", "goto", "jsr", "ret",
        "tableswitch", "lookupswitch", "ireturn", "lreturn", "freturn", "dreturn",
        "areturn", "return", "getstatic", "putstatic", "getfield", "putfield", "invokevirtual",
        "invokespecial", "invokestatic", "invokeinterface", "invokedynamic", "new", "newarray", "anewarray",
        "arraylength", "athrow", "checkcast", "instanceof", "monitorenter",
        "monitorexit", "wide", "multianewarray", "ifnull", "ifnonnull", "goto_w", "jsr_w", "breakpoint",
        "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd",
        "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd",
        "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd",
        "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd",
        "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd",
        "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "rsvd", "impdep1", "impdep2" };