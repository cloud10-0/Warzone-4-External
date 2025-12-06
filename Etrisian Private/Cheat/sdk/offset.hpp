#pragma once
#include <cstdint>
#include "../driver/kernel.h"
#include "../../Util/util.h"
#include "../../global.h"

namespace offsets {
    inline uint64_t ref_def_ptr = 0x14ECDE38;
    inline uint64_t name_array = 0x14ECD628;
    inline uint64_t name_array_pos = 0x2C80;
    inline uint64_t name_size = 0xC0;
    inline uint64_t loot_ptr = 0xDF4CD20;
    inline uint64_t camera_base = 0x15756530;
    inline uint64_t camera_pos = 0x204;
    inline uint64_t local_index = 0x1789A8;
    inline uint64_t local_index_pos = 0x2F0;
    inline uint64_t game_mode = 0x11A83A90;
    inline uint64_t weapon_definitions = 0x14DB50F0;
    inline uint64_t distribute = 0xD21EF78;
    namespace player {
        inline uint64_t size = 0x13DC0;
        inline uint64_t valid = 0x139E0;
        inline uint64_t pos = 0x13AB8;
        inline uint64_t team = 0x1374C;
        inline uint64_t weapon_index = 0x12AC0;
        inline uint64_t dead_1 = 0x0E62;
        inline uint64_t dead_2 = 0x0E06;
    }
    namespace bone {
        inline uint64_t bone_base = 0x42CB0;
        inline uint64_t size = 0x188;
        inline uint64_t offset = 0xD8;
    }
}

struct ref_def_view {
    Vector2 tan_half_fov;
    char pad[0xC];
    Vector3 axis[3];
};

struct ref_def_t {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
    Vector2 tan_half_fov;
    char pad1[8];  // 8-byte padding
    char pad2[4];  // 4-byte padding
    Vector3 axis[3];
};
class refdef_class {

public:
    struct key {
        int32_t ref0;
        int32_t ref1;
        int32_t ref2;
    };

    auto retrieve_ref_def() -> uintptr_t
    {
        key encrypted = read<key>(globals::g_baseAddress + offsets::ref_def_ptr);
        DWORD lowerref = encrypted.ref0 ^ (encrypted.ref2 ^ (uint64_t)(globals::g_baseAddress + offsets::ref_def_ptr)) * ((encrypted.ref2 ^ (uint64_t)(globals::g_baseAddress + offsets::ref_def_ptr)) + 2);
        DWORD upperref = encrypted.ref1 ^ (encrypted.ref2 ^ (uint64_t)(globals::g_baseAddress + offsets::ref_def_ptr + 0x4)) * ((encrypted.ref2 ^ (uint64_t)(globals::g_baseAddress + offsets::ref_def_ptr + 0x4)) + 2); \
            return (uint64_t)upperref << 32 | lowerref;
    }
    ref_def_t ref_def_nn;
};
static refdef_class* decrypt_refdef = new refdef_class();

uintptr_t decrypt_client_info()
{
    const uint64_t mb = globals::g_baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rbx = read<uintptr_t>(globals::g_baseAddress + 0x14CEFCD8);
    if (!rbx)
        return rbx;
    rdx = ~globals::g_peb;                //mov rdx, gs:[rax]
    r8 = globals::g_baseAddress;//failed to trace. base: 00007FF73CB60000 It's possibly wrong
    rcx = rbx + r8 * 1;             //lea rcx, [rbx+r8*1]
    rax = 0;                //and rax, 0xFFFFFFFFC0000000
    rax = _rotl64(rax, 0x10);               //rol rax, 0x10
    rax ^= read<uintptr_t>(globals::g_baseAddress + 0xB1EF0E5);            //xor rax, [0x0000000008969A7C]
    rax = _byteswap_uint64(rax);            //bswap rax
    rcx *= read<uintptr_t>(rax + 0x5);               //imul rcx, [rax+0x05]
    rax = globals::g_baseAddress + 0x37489F9E;            //lea rax, [0x0000000034C04926]
    rdx ^= rax;             //xor rdx, rax
    rcx += rdx;             //add rcx, rdx
    rax = rcx;              //mov rax, rcx
    rax >>= 0x13;           //shr rax, 0x13
    rcx ^= rax;             //xor rcx, rax
    rax = 0xF7B4615B6CAAA4C7;               //mov rax, 0xF7B4615B6CAAA4C7
    rbx = rcx;              //mov rbx, rcx
    rbx >>= 0x26;           //shr rbx, 0x26
    rbx ^= rcx;             //xor rbx, rcx
    rbx ^= rax;             //xor rbx, rax
    rax = 0x4FBE922616062817;               //mov rax, 0x4FBE922616062817
    rbx *= rax;             //imul rbx, rax
    return rbx;
}

uintptr_t decrypt_client_base(uintptr_t client_info)
{
    const uint64_t mb = globals::g_baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rax = read<uintptr_t>(client_info + 0x199748);
    if (!rax)
        return rax;
    r11 = ~~globals::g_peb;                //mov r11, gs:[rcx]
    rcx = r11;              //mov rcx, r11
    //failed to translate: mov [rsp+0x90], r12
    rcx <<= 0x23;           //shl rcx, 0x23
    rcx = _byteswap_uint64(rcx);            //bswap rcx
    rcx &= 0xF;
    switch (rcx) {
    case 0:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x0000000008389943]
        r15 = globals::g_baseAddress + 0x736B1CC9;            //lea r15, [0x000000007084C4D9]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x4;            //shr rcx, 0x04
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rcx = read<uintptr_t>(rcx + 0x9);                //mov rcx, [rcx+0x09]
        uintptr_t RSP_0xFFFFFFFFFFFFFFDF;
        RSP_0xFFFFFFFFFFFFFFDF = 0xEDD02482923403CB;            //mov rcx, 0xEDD02482923403CB : RBP+0xFFFFFFFFFFFFFFDF
        rcx *= RSP_0xFFFFFFFFFFFFFFDF;          //imul rcx, [rbp-0x21]
        rax *= rcx;             //imul rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx -= globals::g_baseAddress;                //sub rcx, [rbp+0x77] -- didn't find trace -> use base
        rcx += 0xFFFFFFFFC99E5582;              //add rcx, 0xFFFFFFFFC99E5582
        rax += rcx;             //add rax, rcx
        rcx = globals::g_baseAddress;                 //lea rcx, [0xFFFFFFFFFD19A665]
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x26A471A9EFBC14B9;               //mov rcx, 0x26A471A9EFBC14B9
        rax *= rcx;             //imul rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rcx ^= r15;             //xor rcx, r15
        rax -= rcx;             //sub rax, rcx
        rcx = 0x4E231C434132699A;               //mov rcx, 0x4E231C434132699A
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 1:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);              //mov r9, [0x000000000838946F]
        r14 = globals::g_baseAddress + 0x2239B1C1;            //lea r14, [0x000000001F5354E6]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x15;           //shr rcx, 0x15
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2A;           //shr rcx, 0x2A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xA;            //shr rcx, 0x0A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x14;           //shr rcx, 0x14
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rcx = 0x3E63A253C6775D5;                //mov rcx, 0x3E63A253C6775D5
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xC1F5691FD75F11C7;               //mov rcx, 0xC1F5691FD75F11C7
        rax *= rcx;             //imul rax, rcx
        rax += 0xFFFFFFFFDA4F9118;              //add rax, 0xFFFFFFFFDA4F9118
        rax += r11;             //add rax, r11
        rcx = r14;              //mov rcx, r14
        rcx = ~rcx;             //not rcx
        rcx ^= r11;             //xor rcx, r11
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 2:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x0000000008388FB7]
        rcx = 0x43AE441D8481DD04;               //mov rcx, 0x43AE441D8481DD04
        rax -= rcx;             //sub rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x26;           //shr rcx, 0x26
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x9;            //shr rcx, 0x09
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x12;           //shr rcx, 0x12
        rax ^= rcx;             //xor rax, rcx
        rdx = rax;              //mov rdx, rax
        rdx >>= 0x24;           //shr rdx, 0x24
        rdx ^= rax;             //xor rdx, rax
        rcx = r11;              //mov rcx, r11
        rax = globals::g_baseAddress + 0x424950C8;            //lea rax, [0x000000003F62ED04]
        rcx = ~rcx;             //not rcx
        rax *= rcx;             //imul rax, rcx
        rax += rdx;             //add rax, rdx
        rcx = 0x1EB0B3B479EF017;                //mov rcx, 0x1EB0B3B479EF017
        rax *= rcx;             //imul rax, rcx
        rcx = 0xF4FDCF8C05766D07;               //mov rcx, 0xF4FDCF8C05766D07
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x17;           //shr rcx, 0x17
        rax ^= rcx;             //xor rax, rcx
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rcx = rax;              //mov rcx, rax
        rdx ^= r10;             //xor rdx, r10
        rcx >>= 0x2E;           //shr rcx, 0x2E
        rdx = ~rdx;             //not rdx
        rax ^= rcx;             //xor rax, rcx
        rax *= read<uintptr_t>(rdx + 0x9);               //imul rax, [rdx+0x09]
        return rax;
    }
    case 3:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x0000000008388A7D]
        r15 = globals::g_baseAddress + 0x8952;                //lea r15, [0xFFFFFFFFFD1A2290]
        r14 = globals::g_baseAddress + 0x1488BAD0;            //lea r14, [0x0000000011A25403]
        rdx = r11;              //mov rdx, r11
        rdx = ~rdx;             //not rdx
        rcx = r15;              //mov rcx, r15
        rcx = ~rcx;             //not rcx
        rdx *= rcx;             //imul rdx, rcx
        rcx = 0x920D8D54066C3BC8;               //mov rcx, 0x920D8D54066C3BC8
        rax ^= rdx;             //xor rax, rdx
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x71B6A01168176A5F;               //mov rcx, 0x71B6A01168176A5F
        rax *= rcx;             //imul rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xB;            //shr rcx, 0x0B
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x16;           //shr rcx, 0x16
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2C;           //shr rcx, 0x2C
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x28C4EBE07CC779E5;               //mov rcx, 0x28C4EBE07CC779E5
        rax ^= rcx;             //xor rax, rcx
        rcx = r11;              //mov rcx, r11
        rcx *= r14;             //imul rcx, r14
        rax -= rcx;             //sub rax, rcx
        rax += r11;             //add rax, r11
        return rax;
    }
    case 4:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x00000000083885AD]
        r14 = ~globals::g_peb + 0x71CF;                //lea r14, [0xFFFFFFFFFD1A063D]
        rcx = 0x7BD4F3C29580BB87;               //mov rcx, 0x7BD4F3C29580BB87
        rax *= rcx;             //imul rax, rcx
        rcx = 0x646EC108C275FCD7;               //mov rcx, 0x646EC108C275FCD7
        rax -= r11;             //sub rax, r11
        rax -= rcx;             //sub rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1B;           //shr rcx, 0x1B
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x36;           //shr rcx, 0x36
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x142843BCE5FD72BB;               //mov rcx, 0x142843BCE5FD72BB
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rax *= rcx;             //imul rax, rcx
        rdx ^= r10;             //xor rdx, r10
        rdx = ~rdx;             //not rdx
        rax += r11;             //add rax, r11
        rax *= read<uintptr_t>(rdx + 0x9);               //imul rax, [rdx+0x09]
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rcx ^= r14;             //xor rcx, r14
        rax -= rcx;             //sub rax, rcx
        return rax;
    }
    case 5:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x00000000083881F1]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x9;            //shr rcx, 0x09
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x12;           //shr rcx, 0x12
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x24;           //shr rcx, 0x24
        rax ^= rcx;             //xor rax, rcx
        rcx = globals::g_baseAddress;                 //lea rcx, [0xFFFFFFFFFD198B2B]
        rcx += 0x111FC085;              //add rcx, 0x111FC085
        rcx += r11;             //add rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = 0xF8D94370868AB99;                //mov rcx, 0xF8D94370868AB99
        rax *= rcx;             //imul rax, rcx
        rcx = 0xB026072E428E1D57;               //mov rcx, 0xB026072E428E1D57
        rax *= rcx;             //imul rax, rcx
        rcx = globals::g_baseAddress;                 //lea rcx, [0xFFFFFFFFFD198CF2]
        rcx += 0x19F5;          //add rcx, 0x19F5
        rcx += r11;             //add rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x23;           //shr rcx, 0x23
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rcx = read<uintptr_t>(rcx + 0x9);                //mov rcx, [rcx+0x09]
        uintptr_t RSP_0x6F;
        RSP_0x6F = 0x5F23D3FEF0707261;          //mov rcx, 0x5F23D3FEF0707261 : RBP+0x6F
        rcx *= RSP_0x6F;                //imul rcx, [rbp+0x6F]
        rax *= rcx;             //imul rax, rcx
        return rax;
    }
    case 6:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);              //mov r9, [0x0000000008387C31]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rax -= r11;             //sub rax, r11
        rcx = globals::g_baseAddress + 0x43B5;                //lea rcx, [0xFFFFFFFFFD19CD3D]
        rcx -= r11;             //sub rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = 0x35284D873B9851A9;               //mov rcx, 0x35284D873B9851A9
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xF62B33C5DDB521B5;               //mov rcx, 0xF62B33C5DDB521B5
        rax *= rcx;             //imul rax, rcx
        rcx = 0xE5B0BD16F00B9D46;               //mov rcx, 0xE5B0BD16F00B9D46
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xA;            //shr rcx, 0x0A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x14;           //shr rcx, 0x14
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x28;           //shr rcx, 0x28
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x19;           //shr rcx, 0x19
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x32;           //shr rcx, 0x32
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 7:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x00000000083877C6]
        r15 = globals::g_baseAddress + 0x8CB4;                //lea r15, [0xFFFFFFFFFD1A133B]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rcx = globals::g_baseAddress;                 //lea rcx, [0xFFFFFFFFFD1982B8]
        rax -= rcx;             //sub rax, rcx
        rcx = 0x9A8F75E5FE8A18B5;               //mov rcx, 0x9A8F75E5FE8A18B5
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x26;           //shr rcx, 0x26
        rax ^= rcx;             //xor rax, rcx
        rax += r11;             //add rax, r11
        rcx = 0xDC35AEB9AD64C433;               //mov rcx, 0xDC35AEB9AD64C433
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2;            //shr rcx, 0x02
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x4;            //shr rcx, 0x04
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = r11 + 0x1;                //lea rcx, [r11+0x01]
        rcx *= r15;             //imul rcx, r15
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 8:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);              //mov r9, [0x0000000008387345]
        rcx = 0x4F7CA4829AB6D5E8;               //mov rcx, 0x4F7CA4829AB6D5E8
        rax ^= rcx;             //xor rax, rcx
        rax += r11;             //add rax, r11
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x24;           //shr rcx, 0x24
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x5178F05F16D45A5B;               //mov rcx, 0x5178F05F16D45A5B
        rax *= rcx;             //imul rax, rcx
        rcx = 0x2ED8CECF4C40E0F3;               //mov rcx, 0x2ED8CECF4C40E0F3
        rax ^= r11;             //xor rax, r11
        rax ^= rcx;             //xor rax, rcx
        rax ^= r11;             //xor rax, r11
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        return rax;
    }
    case 9:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x0000000008386F58]
        r15 = globals::g_baseAddress + 0xDD4D;                //lea r15, [0xFFFFFFFFFD1A5B66]
        r12 = globals::g_baseAddress + 0x66BC28B6;            //lea r12, [0x0000000063D5A6C4]
        rcx = r15;              //mov rcx, r15
        rcx ^= r11;             //xor rcx, r11
        rax += rcx;             //add rax, rcx
        rcx = 0x8BE287ECF689749;                //mov rcx, 0x8BE287ECF689749
        rax *= rcx;             //imul rax, rcx
        rax ^= r11;             //xor rax, r11
        rcx = 0x9933D7378FE6958F;               //mov rcx, 0x9933D7378FE6958F
        rax *= rcx;             //imul rax, rcx
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rdx ^= r10;             //xor rdx, r10
        rcx = r12;              //mov rcx, r12
        rcx = ~rcx;             //not rcx
        rdx = ~rdx;             //not rdx
        rcx ^= r11;             //xor rcx, r11
        rax += rcx;             //add rax, rcx
        rax *= read<uintptr_t>(rdx + 0x9);               //imul rax, [rdx+0x09]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x18;           //shr rcx, 0x18
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x30;           //shr rcx, 0x30
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 10:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x0000000008386B35]
        r15 = globals::g_baseAddress + 0x1C730DA6;            //lea r15, [0x00000000198C879C]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xD;            //shr rcx, 0x0D
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1A;           //shr rcx, 0x1A
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x34;           //shr rcx, 0x34
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x13;           //shr rcx, 0x13
        rax ^= rcx;             //xor rax, rcx
        rdx = rax;              //mov rdx, rax
        rdx >>= 0x26;           //shr rdx, 0x26
        rax ^= rdx;             //xor rax, rdx
        rcx = r15;              //mov rcx, r15
        rcx *= r11;             //imul rcx, r11
        rax -= rcx;             //sub rax, rcx
        rcx = 0x4D5CFB5CBF920449;               //mov rcx, 0x4D5CFB5CBF920449
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xDF1CB3CC3968ECE9;               //mov rcx, 0xDF1CB3CC3968ECE9
        rax *= rcx;             //imul rax, rcx
        rax ^= r11;             //xor rax, r11
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        return rax;
    }
    case 11:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x00000000083865B2]
        rcx = globals::g_baseAddress + 0xE27A;                //lea rcx, [0xFFFFFFFFFD1A5485]
        rcx -= r11;             //sub rcx, r11
        rdx = 0;                //and rdx, 0xFFFFFFFFC0000000
        rax += rcx;             //add rax, rcx
        rdx = _rotl64(rdx, 0x10);               //rol rdx, 0x10
        rdx ^= r10;             //xor rdx, r10
        rdx = ~rdx;             //not rdx
        rax *= read<uintptr_t>(rdx + 0x9);               //imul rax, [rdx+0x09]
        rcx = r11;              //mov rcx, r11
        rcx -= globals::g_baseAddress;                //sub rcx, [rbp+0x77] -- didn't find trace -> use base
        rcx += 0xFFFFFFFFBE05F030;              //add rcx, 0xFFFFFFFFBE05F030
        rax += rcx;             //add rax, rcx
        rcx = 0x8407AE81269A5D57;               //mov rcx, 0x8407AE81269A5D57
        rax *= rcx;             //imul rax, rcx
        rax -= r11;             //sub rax, r11
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x25;           //shr rcx, 0x25
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x4D58E84452B3B2CD;               //mov rcx, 0x4D58E84452B3B2CD
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 12:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x0000000008386069]
        r15 = globals::g_baseAddress + 0x2A01C819;            //lea r15, [0x00000000271B3743]
        rcx = rax;              //mov rcx, rax
        rcx >>= 0xE;            //shr rcx, 0x0E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1C;           //shr rcx, 0x1C
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x38;           //shr rcx, 0x38
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x21;           //shr rcx, 0x21
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rcx = 0x12FD53752A15F441;               //mov rcx, 0x12FD53752A15F441
        rax *= rcx;             //imul rax, rcx
        rcx = r15;              //mov rcx, r15
        rcx *= r11;             //imul rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x2;            //shr rcx, 0x02
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x4;            //shr rcx, 0x04
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x8;            //shr rcx, 0x08
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x10;           //shr rcx, 0x10
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x20;           //shr rcx, 0x20
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x438F040A11D1F693;               //mov rcx, 0x438F040A11D1F693
        rax *= rcx;             //imul rax, rcx
        rcx = 0x76A8417B55AEC887;               //mov rcx, 0x76A8417B55AEC887
        rax += rcx;             //add rax, rcx
        return rax;
    }
    case 13:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);              //mov r9, [0x0000000008385B2B]
        rsi = globals::g_baseAddress + 0xB0AA;                //lea rsi, [0xFFFFFFFFFD1A1A96]
        rcx = rsi;              //mov rcx, rsi
        rcx = ~rcx;             //not rcx
        rcx ^= r11;             //xor rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x18;           //shr rcx, 0x18
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x30;           //shr rcx, 0x30
        rax ^= rcx;             //xor rax, rcx
        rcx = 0xC32A740461B9FDC7;               //mov rcx, 0xC32A740461B9FDC7
        rax *= rcx;             //imul rax, rcx
        rcx = 0x9439B00A1FEFA912;               //mov rcx, 0x9439B00A1FEFA912
        rax ^= rcx;             //xor rax, rcx
        rcx = 0x2B3AD1E7D117AD86;               //mov rcx, 0x2B3AD1E7D117AD86
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x27;           //shr rcx, 0x27
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r9;              //xor rcx, r9
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rcx = globals::g_baseAddress;                 //lea rcx, [0xFFFFFFFFFD19652A]
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 14:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x000000000838563A]
        r14 = globals::g_baseAddress + 0x6A35;                //lea r14, [0xFFFFFFFFFD19CF2B]
        r15 = globals::g_baseAddress + 0x913D;                //lea r15, [0xFFFFFFFFFD19F628]
        rax -= r11;             //sub rax, r11
        rcx = 0xEEEEF35687DD1DF7;               //mov rcx, 0xEEEEF35687DD1DF7
        rax *= rcx;             //imul rax, rcx
        rcx = r15;              //mov rcx, r15
        rcx *= r11;             //imul rcx, r11
        rax ^= rcx;             //xor rax, rcx
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rcx = read<uintptr_t>(rcx + 0x9);                //mov rcx, [rcx+0x09]
        uintptr_t RSP_0x6F;
        RSP_0x6F = 0x571AF583F00DB5E9;          //mov rcx, 0x571AF583F00DB5E9 : RBP+0x6F
        rcx *= RSP_0x6F;                //imul rcx, [rbp+0x6F]
        rax *= rcx;             //imul rax, rcx
        rcx = rax;              //mov rcx, rax
        rdx = r14;              //mov rdx, r14
        rcx >>= 0x21;           //shr rcx, 0x21
        rdx -= r11;             //sub rdx, r11
        rdx ^= rcx;             //xor rdx, rcx
        rax ^= rdx;             //xor rax, rdx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x24;           //shr rcx, 0x24
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    case 15:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF12C);             //mov r10, [0x00000000083850EC]
        r15 = globals::g_baseAddress + 0x2941AF47;            //lea r15, [0x00000000265B0EF4]
        rcx = 0x586536E499271C5;                //mov rcx, 0x586536E499271C5
        rax *= rcx;             //imul rax, rcx
        rcx = globals::g_baseAddress + 0xB0F6;                //lea rcx, [0xFFFFFFFFFD1A0CF9]
        rdx = r11;              //mov rdx, r11
        rdx -= rcx;             //sub rdx, rcx
        rcx = r15;              //mov rcx, r15
        rcx *= r11;             //imul rcx, r11
        rax ^= rdx;             //xor rax, rdx
        rax -= rcx;             //sub rax, rcx
        uintptr_t RSP_0xFFFFFFFFFFFFFFCF;
        RSP_0xFFFFFFFFFFFFFFCF = 0xF66CBDFA6519136F;            //mov rcx, 0xF66CBDFA6519136F : RBP+0xFFFFFFFFFFFFFFCF
        rax ^= RSP_0xFFFFFFFFFFFFFFCF;          //xor rax, [rbp-0x31]
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rcx = ~rcx;             //not rcx
        rax *= read<uintptr_t>(rcx + 0x9);               //imul rax, [rcx+0x09]
        rax ^= r11;             //xor rax, r11
        rcx = globals::g_baseAddress;                 //lea rcx, [0xFFFFFFFFFD195D6F]
        rax -= rcx;             //sub rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x1E;           //shr rcx, 0x1E
        rax ^= rcx;             //xor rax, rcx
        rcx = rax;              //mov rcx, rax
        rcx >>= 0x3C;           //shr rcx, 0x3C
        rax ^= rcx;             //xor rax, rcx
        return rax;
    }
    }
}

uintptr_t decrypt_bone_base()
{
    const uint64_t mb = globals::g_baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rdx = read<uintptr_t>(globals::g_baseAddress + 0xF7BA648);
    if (!rdx)
        return rdx;
    r11 = globals::g_peb;              //mov r11, gs:[rax]
    rax = r11;              //mov rax, r11
    rax = _rotr64(rax, 0x1A);               //ror rax, 0x1A
    rax &= 0xF;
    switch (rax) {
    case 0:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x000000000863264C]
        r14 = globals::g_baseAddress + 0x9280;             //lea r14, [0xFFFFFFFFFD44C69C]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = 0x1409F0CD847A37CE;               //mov rax, 0x1409F0CD847A37CE
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        rax ^= r14;             //xor rax, r14
        rdx += rax;             //add rdx, rax
        rax = 0x3C34D747DB7928EE;               //mov rax, 0x3C34D747DB7928EE
        rdx -= rax;             //sub rdx, rax
        rax = globals::g_baseAddress + 0xDB7F;             //lea rax, [0xFFFFFFFFFD450DA7]
        rax = ~rax;             //not rax
        rax -= r11;             //sub rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = 0xC029A5A1D42718DD;               //mov rax, 0xC029A5A1D42718DD
        rdx *= rax;             //imul rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x7;            //shr rax, 0x07
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xE;            //shr rax, 0x0E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1C;           //shr rax, 0x1C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x38;           //shr rax, 0x38
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 1:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x00000000086320A8]
        r14 = globals::g_baseAddress + 0x54CA1D31;                 //lea r14, [0x00000000520E4BA9]
        rdx += r11;             //add rdx, r11
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD442D73]
        rdx += rax;             //add rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = 0x48462EAD4F11FD6D;               //mov rax, 0x48462EAD4F11FD6D
        rdx *= rax;             //imul rdx, rax
        rax = r11;              //mov rax, r11
        rax ^= r14;             //xor rax, r14
        rdx -= rax;             //sub rdx, rax
        rax = 0x83B3774C1397A303;               //mov rax, 0x83B3774C1397A303
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        rax = 0x829707C28057B2BC;               //mov rax, 0x829707C28057B2BC
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 2:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x0000000008631BC7]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x20;           //shr rax, 0x20
        rdx ^= rax;             //xor rdx, rax
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD4425EB]
        rdx ^= rax;             //xor rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = r11;              //mov rax, r11
        uintptr_t RSP_0x40;
        RSP_0x40 = globals::g_baseAddress + 0x3A246E06;            //lea rax, [0x00000000376897A9] : RSP+0x40
        rax ^= RSP_0x40;                //xor rax, [rsp+0x40]
        rdx += rax;             //add rdx, rax
        rax = 0xC391B266D5217A5F;               //mov rax, 0xC391B266D5217A5F
        rdx ^= rax;             //xor rdx, rax
        rax = 0x5B7F3E818AF67A35;               //mov rax, 0x5B7F3E818AF67A35
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1A;           //shr rax, 0x1A
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x34;           //shr rax, 0x34
        rdx ^= rax;             //xor rdx, rax
        rax = 0x19C8F1552DE67BBF;               //mov rax, 0x19C8F1552DE67BBF
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 3:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x00000000086316DF]
        rax = globals::g_baseAddress + 0xBD6;              //lea rax, [0xFFFFFFFFFD442EA3]
        rax -= r11;             //sub rax, r11
        rax ^= r11;             //xor rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = 0xAA6F288FD0E3CBF;                //mov rax, 0xAA6F288FD0E3CBF
        rdx *= rax;             //imul rdx, rax
        r15 = 0x702F07A4D309E97C;               //mov r15, 0x702F07A4D309E97C
        rdx += r15;             //add rdx, r15
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rdx ^= r11;             //xor rdx, r11
        rax = 0x65D0349BA5FED43B;               //mov rax, 0x65D0349BA5FED43B
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x12;           //shr rax, 0x12
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 4:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x00000000086312F0]
        r15 = globals::g_baseAddress + 0x8817;             //lea r15, [0xFFFFFFFFFD44A8D7]
        rdx ^= r11;             //xor rdx, r11
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD441F27]
        rdx -= rax;             //sub rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = 0x647DC95B2924B45D;               //mov rax, 0x647DC95B2924B45D
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xF;            //shr rax, 0x0F
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1E;           //shr rax, 0x1E
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x3C;           //shr rax, 0x3C
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax ^= r15;             //xor rax, r15
        rdx += rax;             //add rdx, rax
        rax = 0x66F54217655405BD;               //mov rax, 0x66F54217655405BD
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 5:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x0000000008630DCE]
        rax = r11;              //mov rax, r11
        rax -= globals::g_baseAddress;          //sub rax, [rsp+0xC8] -- didn't find trace -> use base
        rax += 0xFFFFFFFFDA207ED1;              //add rax, 0xFFFFFFFFDA207ED1
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x12;           //shr rax, 0x12
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax -= globals::g_baseAddress;             //sub rax, [rsp+0xC8] -- didn't find trace -> use base
        rax -= 0x39EDAA32;              //sub rax, 0x39EDAA32
        rdx ^= rax;             //xor rdx, rax
        rax = 0x24AC8C57718FF261;               //mov rax, 0x24AC8C57718FF261
        rdx *= rax;             //imul rdx, rax
        rax = 0x5997D68B6A65573B;               //mov rax, 0x5997D68B6A65573B
        rdx *= rax;             //imul rdx, rax
        rax = 0x5FD1C67422180770;               //mov rax, 0x5FD1C67422180770
        rdx -= rax;             //sub rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = r11;              //mov rax, r11
        rax -= globals::g_baseAddress;             //sub rax, [rsp+0xC8] -- didn't find trace -> use base
        rax += 0xFFFFFFFFE77DFE7B;              //add rax, 0xFFFFFFFFE77DFE7B
        rdx += rax;             //add rdx, rax
        return rdx;
    }
    case 6:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x00000000086308CF]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = 0x53AAB2A28C6F8FF0;               //mov rax, 0x53AAB2A28C6F8FF0
        rdx ^= rax;             //xor rdx, rax
        rdx -= r11;             //sub rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x12;           //shr rax, 0x12
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rax ^= r11;             //xor rax, r11
        rdx ^= rax;             //xor rdx, rax
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD4414E1]
        rdx += rax;             //add rdx, rax
        rdx += r11;             //add rdx, r11
        rax = 0x3EACC212565A3D5;                //mov rax, 0x3EACC212565A3D5
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 7:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x0000000008630442]
        r14 = globals::g_baseAddress + 0x8FBF;             //lea r14, [0xFFFFFFFFFD44A1CC]
        rax = globals::g_baseAddress + 0xD9BA;             //lea rax, [0xFFFFFFFFFD44E99E]
        rax = ~rax;             //not rax
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1B;           //shr rax, 0x1B
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x36;           //shr rax, 0x36
        rdx ^= rax;             //xor rdx, rax
        rax = 0xC097FE30215EF7B;                //mov rax, 0xC097FE30215EF7B
        rdx -= rax;             //sub rdx, rax
        rdx += r11;             //add rdx, r11
        rax = 0x27217EED83C00465;               //mov rax, 0x27217EED83C00465
        rdx *= rax;             //imul rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rcx = rdx;              //mov rcx, rdx
        rdx = r11;              //mov rdx, r11
        rdx ^= rcx;             //xor rdx, rcx
        rdx ^= r14;             //xor rdx, r14
        rax = 0x40FC9A08434EAB8;                //mov rax, 0x40FC9A08434EAB8
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 8:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x0000000008630031]
        rax = 0xC640566C96CFB225;               //mov rax, 0xC640566C96CFB225
        rdx *= rax;             //imul rdx, rax
        rdx ^= globals::g_baseAddress;             //xor rdx, [rsp+0xC8] -- didn't find trace -> use base
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rdx -= r11;             //sub rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x4;            //shr rax, 0x04
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x8;            //shr rax, 0x08
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x10;           //shr rax, 0x10
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x20;           //shr rax, 0x20
        rdx ^= rax;             //xor rdx, rax
        rdx += r11;             //add rdx, r11
        rax = 0x36BE6884C47C6D33;               //mov rax, 0x36BE6884C47C6D33
        rdx *= rax;             //imul rdx, rax
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        rax -= globals::g_baseAddress;             //sub rax, [rsp+0xC8] -- didn't find trace -> use base
        rax -= 0x736A3793;              //sub rax, 0x736A3793
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 9:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x000000000862FB3D]
        r15 = globals::g_baseAddress + 0x56B5;             //lea r15, [0xFFFFFFFFFD445FC2]
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        rax ^= r15;             //xor rax, r15
        rdx -= rax;             //sub rdx, rax
        rax = 0x617EE6B8548ACFF8;               //mov rax, 0x617EE6B8548ACFF8
        rdx ^= rax;             //xor rdx, rax
        rdx += r11;             //add rdx, r11
        rax = 0x44AC3A1174A702A7;               //mov rax, 0x44AC3A1174A702A7
        rdx *= rax;             //imul rdx, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rdx ^= r11;             //xor rdx, r11
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rax = rdx;              //mov rax, rdx
        rcx ^= r10;             //xor rcx, r10
        rdx >>= 0x27;           //shr rdx, 0x27
        rcx = ~rcx;             //not rcx
        rdx ^= rax;             //xor rdx, rax
        rdx *= read<uintptr_t>(rcx + 0x15);                 //imul rdx, [rcx+0x15]
        rax = 0x7915D47D16706192;               //mov rax, 0x7915D47D16706192
        rdx -= rax;             //sub rdx, rax
        return rdx;
    }
    case 10:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x000000000862F6FC]
        r15 = globals::g_baseAddress + 0x5A848877;                 //lea r15, [0x0000000057C88D43]
        rax = r15;              //mov rax, r15
        rax = ~rax;             //not rax
        rax ^= r11;             //xor rax, r11
        rdx -= rax;             //sub rdx, rax
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD4403DB]
        rdx ^= rax;             //xor rdx, rax
        rax = 0x21F6FDA360F3B27;                //mov rax, 0x21F6FDA360F3B27
        rdx *= rax;             //imul rdx, rax
        rdx -= r11;             //sub rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x6;            //shr rax, 0x06
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rax = 0x3B33D31E5AB12803;               //mov rax, 0x3B33D31E5AB12803
        rdx += rax;             //add rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x26;           //shr rax, 0x26
        rdx ^= rax;             //xor rdx, rax
        return rdx;
    }
    case 11:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x000000000862F224]
        rcx = r11;              //mov rcx, r11
        rcx = ~rcx;             //not rcx
        rax = globals::g_baseAddress + 0x28691EFC;                 //lea rax, [0x0000000025AD1C2E]
        rax = ~rax;             //not rax
        rcx += rax;             //add rcx, rax
        rdx ^= rcx;             //xor rdx, rcx
        rax = 0x4F163BACB48EBF73;               //mov rax, 0x4F163BACB48EBF73
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x14;           //shr rax, 0x14
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x28;           //shr rax, 0x28
        rdx ^= rax;             //xor rdx, rax
        rax = 0x4127EEFEDE5B92FD;               //mov rax, 0x4127EEFEDE5B92FD
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x21;           //shr rax, 0x21
        rdx ^= rax;             //xor rdx, rax
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r10;             //xor rax, r10
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x4;            //shr rax, 0x04
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x8;            //shr rax, 0x08
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x10;           //shr rax, 0x10
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x20;           //shr rax, 0x20
        rdx ^= rax;             //xor rdx, rax
        rax = 0x397EFF255639273F;               //mov rax, 0x397EFF255639273F
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 12:
    {
        r9 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);             //mov r9, [0x000000000862ECBC]
        r15 = globals::g_baseAddress + 0x41C6E8B9;                 //lea r15, [0x000000003F0AE345]
        rdx += r11;             //add rdx, r11
        rax = rdx;              //mov rax, rdx
        rax >>= 0x22;           //shr rax, 0x22
        rdx ^= rax;             //xor rdx, rax
        rax = 0x233E216C40FA2CDF;               //mov rax, 0x233E216C40FA2CDF
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rax = r11;              //mov rax, r11
        rax ^= r15;             //xor rax, r15
        rdx ^= rax;             //xor rdx, rax
        rax = 0x6773B66CDA475049;               //mov rax, 0x6773B66CDA475049
        rdx *= rax;             //imul rdx, rax
        uintptr_t RSP_0x80;
        RSP_0x80 = 0xF154E6D1B3660D73;          //mov rax, 0xF154E6D1B3660D73 : RSP+0x80
        rdx ^= RSP_0x80;                //xor rdx, [rsp+0x80]
        rax = 0;                //and rax, 0xFFFFFFFFC0000000
        rax = _rotl64(rax, 0x10);               //rol rax, 0x10
        rax ^= r9;              //xor rax, r9
        rax = ~rax;             //not rax
        rdx *= read<uintptr_t>(rax + 0x15);                 //imul rdx, [rax+0x15]
        return rdx;
    }
    case 13:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x000000000862E7A9]
        r15 = globals::g_baseAddress + 0x102B1DCA;                 //lea r15, [0x000000000D6F1343]
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD43F4C4]
        rax += 0xBA17;          //add rax, 0xBA17
        rax += r11;             //add rax, r11
        rdx += rax;             //add rdx, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rax = globals::g_baseAddress + 0x9610;             //lea rax, [0xFFFFFFFFFD448819]
        rax = ~rax;             //not rax
        rcx ^= r10;             //xor rcx, r10
        rax -= r11;             //sub rax, r11
        rcx = ~rcx;             //not rcx
        rdx += rax;             //add rdx, rax
        rdx *= read<uintptr_t>(rcx + 0x15);                 //imul rdx, [rcx+0x15]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x15;           //shr rax, 0x15
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x2A;           //shr rax, 0x2A
        rdx ^= rax;             //xor rdx, rax
        rax = 0x6A8B294107CC0501;               //mov rax, 0x6A8B294107CC0501
        rdx ^= rax;             //xor rdx, rax
        rax = 0x2EA5061AACD42452;               //mov rax, 0x2EA5061AACD42452
        rdx -= rax;             //sub rdx, rax
        rax = r11;              //mov rax, r11
        rax = ~rax;             //not rax
        rax ^= r15;             //xor rax, r15
        rdx += rax;             //add rdx, rax
        rax = 0x4EB7AE4244212391;               //mov rax, 0x4EB7AE4244212391
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 14:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x000000000862E372]
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD43F06C]
        rdx ^= rax;             //xor rdx, rax
        rax = 0xC752E26BA360D032;               //mov rax, 0xC752E26BA360D032
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x19;           //shr rax, 0x19
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x32;           //shr rax, 0x32
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xD;            //shr rax, 0x0D
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x1A;           //shr rax, 0x1A
        rdx ^= rax;             //xor rdx, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rax = rdx;              //mov rax, rdx
        rcx ^= r10;             //xor rcx, r10
        rax >>= 0x34;           //shr rax, 0x34
        rcx = ~rcx;             //not rcx
        rdx ^= rax;             //xor rdx, rax
        rdx *= read<uintptr_t>(rcx + 0x15);                 //imul rdx, [rcx+0x15]
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD43ED12]
        rdx ^= rax;             //xor rdx, rax
        rax = 0x5436A045E6437655;               //mov rax, 0x5436A045E6437655
        rdx *= rax;             //imul rdx, rax
        return rdx;
    }
    case 15:
    {
        r10 = read<uintptr_t>(globals::g_baseAddress + 0xB1EF21D);            //mov r10, [0x000000000862DED7]
        rax = rdx;              //mov rax, rdx
        rax >>= 0x6;            //shr rax, 0x06
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0xC;            //shr rax, 0x0C
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x18;           //shr rax, 0x18
        rdx ^= rax;             //xor rdx, rax
        rcx = 0;                //and rcx, 0xFFFFFFFFC0000000
        rcx = _rotl64(rcx, 0x10);               //rol rcx, 0x10
        rcx ^= r10;             //xor rcx, r10
        rax = rdx;              //mov rax, rdx
        rcx = ~rcx;             //not rcx
        rax >>= 0x30;           //shr rax, 0x30
        rdx ^= rax;             //xor rdx, rax
        rdx *= read<uintptr_t>(rcx + 0x15);                 //imul rdx, [rcx+0x15]
        rax = globals::g_baseAddress;              //lea rax, [0xFFFFFFFFFD43EA41]
        rdx += rax;             //add rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rdx ^= rax;             //xor rdx, rax
        rax = 0xB6C3A6FE99C92A23;               //mov rax, 0xB6C3A6FE99C92A23
        rdx *= rax;             //imul rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x9;            //shr rax, 0x09
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x12;           //shr rax, 0x12
        rdx ^= rax;             //xor rdx, rax
        rax = rdx;              //mov rax, rdx
        rax >>= 0x24;           //shr rax, 0x24
        rdx ^= rax;             //xor rdx, rax
        rax = 0xD7420EB04571AACF;               //mov rax, 0xD7420EB04571AACF
        rdx *= rax;             //imul rdx, rax
        rax = 0x578A3A3D4AF2D633;               //mov rax, 0x578A3A3D4AF2D633
        rdx += rax;             //add rdx, rax
        return rdx;
    }
    }
}

uint16_t get_bone_index(uint32_t bone_index)
{
    const uint64_t mb = globals::g_baseAddress;
    uint64_t rax = mb, rbx = mb, rcx = mb, rdx = mb, rdi = mb, rsi = mb, r8 = mb, r9 = mb, r10 = mb, r11 = mb, r12 = mb, r13 = mb, r14 = mb, r15 = mb;
    rdi = bone_index;
    rcx = rdi * 0x13C8;
    rax = 0xCC70CD3D3E0A7B49;               //mov rax, 0xCC70CD3D3E0A7B49
    rax = _umul128(rax, rcx, (uintptr_t*)&rdx);             //mul rcx
    r11 = globals::g_baseAddress;              //lea r11, [0xFFFFFFFFFD7A8B35]
    r10 = 0x45F86A52798F52B7;               //mov r10, 0x45F86A52798F52B7
    rdx >>= 0xC;            //shr rdx, 0x0C
    rax = rdx * 0x1409;             //imul rax, rdx, 0x1409
    rcx -= rax;             //sub rcx, rax
    rax = 0xDC9D0ECFCB6E9379;               //mov rax, 0xDC9D0ECFCB6E9379
    r8 = rcx * 0x1409;              //imul r8, rcx, 0x1409
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rdx >>= 0xD;            //shr rdx, 0x0D
    rax = rdx * 0x2522;             //imul rax, rdx, 0x2522
    r8 -= rax;              //sub r8, rax
    rax = 0x49539E3B2D066EA3;               //mov rax, 0x49539E3B2D066EA3
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = r8;               //mov rax, r8
    rax -= rdx;             //sub rax, rdx
    rax >>= 0x1;            //shr rax, 0x01
    rax += rdx;             //add rax, rdx
    rax >>= 0x9;            //shr rax, 0x09
    rcx = rax * 0x31C;              //imul rcx, rax, 0x31C
    rax = 0xD79435E50D79435F;               //mov rax, 0xD79435E50D79435F
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rdx >>= 0x4;            //shr rdx, 0x04
    rcx += rdx;             //add rcx, rdx
    rax = rcx * 0x26;               //imul rax, rcx, 0x26
    rcx = r8 + r8 * 4;              //lea rcx, [r8+r8*4]
    rcx <<= 0x3;            //shl rcx, 0x03
    rcx -= rax;             //sub rcx, rax
    rax = read<uint16_t>(rcx + r11 * 1 + 0xB2AB190);           //movzx eax, word ptr [rcx+r11*1+0xB2AB190]
    r8 = rax * 0x13C8;              //imul r8, rax, 0x13C8
    rax = r10;              //mov rax, r10
    rax = _umul128(rax, r8, (uintptr_t*)&rdx);              //mul r8
    rax = r10;              //mov rax, r10
    rdx >>= 0xB;            //shr rdx, 0x0B
    rcx = rdx * 0x1D45;             //imul rcx, rdx, 0x1D45
    r8 -= rcx;              //sub r8, rcx
    r9 = r8 * 0x39A6;               //imul r9, r8, 0x39A6
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rdx >>= 0xB;            //shr rdx, 0x0B
    rax = rdx * 0x1D45;             //imul rax, rdx, 0x1D45
    r9 -= rax;              //sub r9, rax
    rax = 0x88ECF206D1CD0DD7;               //mov rax, 0x88ECF206D1CD0DD7
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rax = 0xAAAAAAAAAAAAAAAB;               //mov rax, 0xAAAAAAAAAAAAAAAB
    rdx >>= 0xB;            //shr rdx, 0x0B
    rcx = rdx * 0xEF5;              //imul rcx, rdx, 0xEF5
    rax = _umul128(rax, r9, (uintptr_t*)&rdx);              //mul r9
    rdx >>= 0x1;            //shr rdx, 0x01
    rcx += rdx;             //add rcx, rdx
    rax = rcx + rcx * 2;            //lea rax, [rcx+rcx*2]
    rax += rax;             //add rax, rax
    rcx = r9 * 8 + 0x0;             //lea rcx, [r9*8]
    rcx -= rax;             //sub rcx, rax
    r15 = read<uint16_t>(rcx + r11 * 1 + 0xB2B27C0);           //movsx r15d, word ptr [rcx+r11*1+0xB2B27C0]
    return r15;
}