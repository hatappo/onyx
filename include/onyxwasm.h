#ifndef ONYXWASM_H
#define ONYXWASM_H

#include "bh.h"

#include "onyxastnodes.h"
#include "onyxmsgs.h"
#include "onyxir.h"

typedef u8 WasmType;

extern const WasmType WASM_TYPE_INT32;
extern const WasmType WASM_TYPE_INT64;
extern const WasmType WASM_TYPE_FLOAT32;
extern const WasmType WASM_TYPE_FLOAT64;

typedef struct WasmFuncType {
    // NOTE: For now, WASM only allows for 1 return value.
    // This may be lifted in the future.
    i32 param_count;
    WasmType return_type;
    WasmType param_types[];
} WasmFuncType;


typedef enum WasmInstructionType {
    WI_UNREACHABLE                   = 0x00,
    WI_NOP                           = 0x01,

    // NOTE: Control flow
    WI_BLOCK_START                   = 0x02,
    WI_BLOCK_END                     = 0x0B, // NOTE: These ends are not unique
    WI_LOOP_START                    = 0x03,
    WI_LOOP_END                      = 0x0B,
    WI_IF_START                      = 0x04,
    WI_ELSE                          = 0x05,
    WI_IF_END                        = 0x0B,
    WI_JUMP                          = 0x0C,
    WI_COND_JUMP                     = 0x0D,
    WI_JUMP_TABLE                    = 0x0E,
    WI_RETURN                        = 0x0F,
    WI_CALL                          = 0x10,
    WI_CALL_INDIRECT                 = 0x11,

    // NOTE: Parametric instructions
    WI_DROP                          = 0x1A,
    WI_SELECT                        = 0x1B,

    // NOTE: Variable instructions
    WI_LOCAL_GET                     = 0x20,
    WI_LOCAL_SET                     = 0x21,
    WI_LOCAL_TEE                     = 0x22,
    WI_GLOBAL_GET                    = 0x23,
    WI_GLOBAL_SET                    = 0x24,

    // NOTE: Memory instructions
    WI_I32_LOAD                      = 0x28,
    WI_I64_LOAD                      = 0x29,
    WI_F32_LOAD                      = 0x2A,
    WI_F64_LOAD                      = 0x2B,
    WI_I32_LOAD_8_S                  = 0x2C,
    WI_I32_LOAD_8_U                  = 0x2D,
    WI_I32_LOAD_16_S                 = 0x2E,
    WI_I32_LOAD_16_U                 = 0x2F,
    WI_I64_LOAD_8_S                  = 0x30,
    WI_I64_LOAD_8_U                  = 0x31,
    WI_I64_LOAD_16_S                 = 0x32,
    WI_I64_LOAD_16_U                 = 0x33,
    WI_I64_LOAD_32_S                 = 0x34,
    WI_I64_LOAD_32_U                 = 0x35,
    WI_I32_STORE                     = 0x36,
    WI_I64_STORE                     = 0x37,
    WI_F32_STORE                     = 0x38,
    WI_F64_STORE                     = 0x39,
    WI_I32_STORE_8                   = 0x3A,
    WI_I32_STORE_16                  = 0x3B,
    WI_I64_STORE_8                   = 0x3C,
    WI_I64_STORE_16                  = 0x3D,
    WI_I64_STORE_32                  = 0x3E,
    WI_MEMORY_SIZE                   = 0x3F,
    WI_MEMORY_GROW                   = 0x40,

    // NOTE: Numeric Instructions
    WI_I32_CONST                     = 0x41,
    WI_I64_CONST                     = 0x42,
    WI_F32_CONST                     = 0x43,
    WI_F64_CONST                     = 0x44,

    WI_I32_EQZ                       = 0x45,
    WI_I32_EQ                        = 0x46,
    WI_I32_NE                        = 0x47,
    WI_I32_LT_S                      = 0x48,
    WI_I32_LT_U                      = 0x49,
    WI_I32_GT_S                      = 0x4a,
    WI_I32_GT_U                      = 0x4b,
    WI_I32_LE_S                      = 0x4c,
    WI_I32_LE_U                      = 0x4d,
    WI_I32_GE_S                      = 0x4e,
    WI_I32_GE_U                      = 0x4f,

    WI_I64_EQZ                       = 0x50,
    WI_I64_EQ                        = 0x51,
    WI_I64_NE                        = 0x52,
    WI_I64_LT_S                      = 0x53,
    WI_I64_LT_U                      = 0x54,
    WI_I64_GT_S                      = 0x55,
    WI_I64_GT_U                      = 0x56,
    WI_I64_LE_S                      = 0x57,
    WI_I64_LE_U                      = 0x58,
    WI_I64_GE_S                      = 0x59,
    WI_I64_GE_U                      = 0x5a,

    WI_F32_EQ                        = 0x5b,
    WI_F32_NE                        = 0x5c,
    WI_F32_LT                        = 0x5d,
    WI_F32_GT                        = 0x5e,
    WI_F32_LE                        = 0x5f,
    WI_F32_GE                        = 0x60,

    WI_F64_EQ                        = 0x61,
    WI_F64_NE                        = 0x62,
    WI_F64_LT                        = 0x63,
    WI_F64_GT                        = 0x64,
    WI_F64_LE                        = 0x65,
    WI_F64_GE                        = 0x66,

    WI_I32_CLZ                       = 0x67,
    WI_I32_CTZ                       = 0x68,
    WI_I32_POPCNT                    = 0x69,
    WI_I32_ADD                       = 0x6a,
    WI_I32_SUB                       = 0x6b,
    WI_I32_MUL                       = 0x6c,
    WI_I32_DIV_S                     = 0x6d,
    WI_I32_DIV_U                     = 0x6e,
    WI_I32_REM_S                     = 0x6f,
    WI_I32_REM_U                     = 0x70,
    WI_I32_AND                       = 0x71,
    WI_I32_OR                        = 0x72,
    WI_I32_XOR                       = 0x73,
    WI_I32_SHL                       = 0x74,
    WI_I32_SHR_S                     = 0x75,
    WI_I32_SHR_U                     = 0x76,
    WI_I32_ROTL                      = 0x77,
    WI_I32_ROTR                      = 0x78,

    WI_I64_CLZ                       = 0x79,
    WI_I64_CTZ                       = 0x7a,
    WI_I64_POPCNT                    = 0x7b,
    WI_I64_ADD                       = 0x7c,
    WI_I64_SUB                       = 0x7d,
    WI_I64_MUL                       = 0x7e,
    WI_I64_DIV_S                     = 0x7f,
    WI_I64_DIV_U                     = 0x80,
    WI_I64_REM_S                     = 0x81,
    WI_I64_REM_U                     = 0x82,
    WI_I64_AND                       = 0x83,
    WI_I64_OR                        = 0x84,
    WI_I64_XOR                       = 0x85,
    WI_I64_SHL                       = 0x86,
    WI_I64_SHR_S                     = 0x87,
    WI_I64_SHR_U                     = 0x88,
    WI_I64_ROTL                      = 0x89,
    WI_I64_ROTR                      = 0x8a,

    WI_F32_ABS                       = 0x8b,
    WI_F32_NEG                       = 0x8c,
    WI_F32_CEIL                      = 0x8d,
    WI_F32_FLOOR                     = 0x8e,
    WI_F32_TRUNC                     = 0x8f,
    WI_F32_NEAREST                   = 0x90,
    WI_F32_SQRT                      = 0x91,
    WI_F32_ADD                       = 0x92,
    WI_F32_SUB                       = 0x93,
    WI_F32_MUL                       = 0x94,
    WI_F32_DIV                       = 0x95,
    WI_F32_MIN                       = 0x96,
    WI_F32_MAX                       = 0x97,
    WI_F32_COPYSIGN                  = 0x98,

    WI_F64_ABS                       = 0x99,
    WI_F64_NEG                       = 0x9a,
    WI_F64_CEIL                      = 0x9b,
    WI_F64_FLOOR                     = 0x9c,
    WI_F64_TRUNC                     = 0x9d,
    WI_F64_NEAREST                   = 0x9e,
    WI_F64_SQRT                      = 0x9f,
    WI_F64_ADD                       = 0xA0,
    WI_F64_SUB                       = 0xA1,
    WI_F64_MUL                       = 0xA2,
    WI_F64_DIV                       = 0xA3,
    WI_F64_MIN                       = 0xA4,
    WI_F64_MAX                       = 0xA5,
    WI_F64_COPYSIGN                  = 0xA6,

    WI_I32_FROM_I64                  = 0xA7,
    WI_I32_FROM_F32_S                = 0xA8,
    WI_I32_FROM_F32_U                = 0xA9,
    WI_I32_FROM_F64_S                = 0xAA,
    WI_I32_FROM_F64_U                = 0xAB,

    WI_I64_FROM_I32_S                = 0xAC,
    WI_I64_FROM_I32_U                = 0xAD,
    WI_I64_FROM_F32_S                = 0xAE,
    WI_I64_FROM_F32_U                = 0xAF,
    WI_I64_FROM_F64_S                = 0xB0,
    WI_I64_FROM_F64_U                = 0xB1,

    WI_F32_FROM_I32_S                = 0xB2,
    WI_F32_FROM_I32_U                = 0xB3,
    WI_F32_FROM_I64_S                = 0xB4,
    WI_F32_FROM_I64_U                = 0xB5,
    WI_F32_FROM_F64                  = 0xB6,

    WI_F64_FROM_I32_S                = 0xB7,
    WI_F64_FROM_I32_U                = 0xB8,
    WI_F64_FROM_I64_S                = 0xB9,
    WI_F64_FROM_I64_U                = 0xBA,
    WI_F64_FROM_F32                  = 0xBB,

    WI_I32_REINTERPRET_F32           = 0xBC,
    WI_I64_REINTERPRET_F64           = 0xBD,
    WI_F32_REINTERPRET_I32           = 0xBE,
    WI_F64_REINTERPRET_I64           = 0xBF,
} WasmInstructionType;

typedef union {
    struct {
        u32 i1, i2;
    };
    i64 l;
    float f;
    double d;
    ptr p;
} WasmInstructionData;

typedef struct WasmInstruction {
    WasmInstructionType type;
    WasmInstructionData data;
} WasmInstruction;

typedef struct WasmFuncLocals {
    u8 i32_count;
    u8 i64_count;
    u8 f32_count;
    u8 f64_count;
} WasmFuncLocals;

typedef struct WasmFunc {
    i32 type_idx;
    WasmFuncLocals locals;
    bh_arr(WasmInstruction) code;
} WasmFunc;

typedef struct WasmGlobal {
    WasmType type;
    u8 mutable;
    bh_arr(WasmInstruction) initial_value;
} WasmGlobal;

typedef enum WasmForeignKind {
    WASM_FOREIGN_FUNCTION   = 0x00,
    WASM_FOREIGN_TABLE      = 0x01,
    WASM_FOREIGN_MEMORY     = 0x02,
    WASM_FOREIGN_GLOBAL     = 0x03,
} WasmForeignKind;

typedef struct WasmExport {
    WasmForeignKind kind;
    i32 idx;
} WasmExport;

typedef struct WasmImport {
    WasmForeignKind kind;
    i32 idx;
    OnyxToken *mod, *name;
} WasmImport;

typedef struct OnyxWasmModule {
    bh_allocator allocator;
    OnyxMessages* msgs;

    // NOTE: Mapping from local ast node ptrs to indicies
    bh_imap local_map;
    bh_imap global_map;

    bh_arr(u8) structured_jump_target;

    // NOTE: Used internally as a map from strings that represent function types,
    // 0x7f 0x7f : 0x7f ( (i32, i32) -> i32 )
    // to the function type index if it has been created.
    bh_table(i32) type_map;
    bh_arr(WasmFuncType*) functypes; // NOTE: This have to be pointers because the type is variadic in size

    bh_arr(WasmFunc) funcs;
    bh_imap func_map; // NOTE: Maps from ast node pointers to the function index

    bh_table(WasmExport) exports;

    bh_arr(WasmImport) imports;

    bh_arr(WasmGlobal) globals;

    u16 next_type_idx;
    u16 next_func_idx;
    u16 next_global_idx;
    u16 export_count;
} OnyxWasmModule;

OnyxWasmModule onyx_wasm_module_create(bh_allocator alloc, OnyxMessages* msgs);
void onyx_wasm_module_compile(OnyxWasmModule* module, ParserOutput* program);
void onyx_wasm_module_free(OnyxWasmModule* module);
void onyx_wasm_module_write_to_file(OnyxWasmModule* module, bh_file file);

#endif