#include<stdint.h>

#define CONSTANT_UTF8 1
#define CONSTANT_INT 3
#define CONSTANT_FLOAT 4
#define CONSTANT_LONG 5
#define CONSTANT_DOUBLE 6
#define CONSTANT_CLASS 7
#define CONSTANT_STRING 8
#define CONSTANT_FIELD_REF 9
#define CONSTANT_METHOD_REF 10
#define CONSTANT_INTERFACE_METHOD_REF 11
#define CONSTANT_NAME_AND_TYPE 12
#define CONSTANT_METHOD_HANDLE 15
#define CONSTANT_METHOD_TYPE 16
#define CONSTANT_INVOKE_DYNAMIC 18

typedef struct {
	uint16_t size;
	uint8_t* content;
} UTF8;

typedef struct {
	uint16_t classIndex;
	uint16_t nameAndTypeIndex;
} MethodRef;

typedef struct {
	uint16_t nameIndex;
} Class;

typedef struct {
	uint16_t nameIndex;
	uint16_t descriptorIndex;
} NameAndTypeIndex;

typedef struct {
	uint32_t value;
} Integer;

typedef struct {
	float value;
} Float;

typedef struct {
	uint64_t value;
} Long;

typedef struct {
	double value;
} Double;

typedef struct {
	uint16_t index;
} String;

typedef struct {
	uint8_t referenceKind;
	uint16_t referenceType;
} MethodHandle;

typedef struct {
	uint16_t descriptorIndex;
} MethodType;

typedef union {
	uint8_t bytes[10];
	UTF8 utf8;
	MethodRef methodRef;
	Class class;
	NameAndTypeIndex nameAndTypeIndex;
	Integer integer;
	Float f;
	Long l;
	Double d;
	String string;
	MethodHandle methodHandle;
	MethodType methodType;
} Constant;

typedef struct {
	uint8_t tag;
	Constant* constant;
} ConstantPoolInfo;

typedef struct {
	uint16_t constantPoolCount;
	ConstantPoolInfo* constantPool[];
} ConstantPool;

ConstantPool* readConstantPool(const uint8_t** bytes);
