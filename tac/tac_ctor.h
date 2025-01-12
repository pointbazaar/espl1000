#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "tac.h"

struct TAC;
uint32_t make_label();
uint32_t make_temp();

//for most TACs, there should be a dedicated constructor,
//which helps with avoiding invalid state and malformed TACs

struct TAC* makeTACLabel(uint32_t label);
struct TAC* makeTACLabelFunction(uint32_t sst_index);

struct TAC* makeTACGoto(uint32_t label);
struct TAC* makeTACReturn(uint32_t tmp);

struct TAC* makeTACIfGoto(uint32_t tmp_condition, uint32_t label_destination);
struct TAC* makeTACIfCMPGoto(uint32_t tmp1, enum TAC_OP op, uint32_t tmp2, uint32_t label_destination);

struct TAC* makeTACCopy(uint32_t dest, uint32_t src);

struct TAC* makeTACLoadLocal(uint32_t tmp, uint32_t local_index);
struct TAC* makeTACStoreLocal(uint32_t local_index, uint32_t tmp);
struct TAC* makeTACLoadLocalAddr(uint32_t tmp, uint32_t local_index);

struct TAC* makeTACConst(uint32_t tmp, int value);
struct TAC* makeTACConst16(uint32_t tmp, int value);

struct TAC* makeTACBinOp(uint32_t dest, enum TAC_OP op, uint32_t src);
struct TAC* makeTACUnaryOp(uint32_t dest, uint32_t src, enum TAC_OP op);

struct TAC* makeTACStoreConstAddr(uint64_t addr, uint64_t src);
struct TAC* makeTACLoadConstAddr(uint32_t dest, uint32_t addr);

struct TAC* makeTACParam(uint32_t dest, bool push16, uint32_t param_index);
struct TAC* makeTACCall(uint32_t tmp, uint32_t function_index);
struct TAC* makeTACICall(uint32_t tmp, uint32_t tmp_call);

struct TAC* makeTACSetupStackframe(uint32_t frame_size);
struct TAC* makeTACSetupSP();

struct TAC* makeTACNop();

// @param width   how many bytes to load
struct TAC* makeTACLoad(uint32_t tmp, uint32_t taddr, uint8_t width);

// @param width   how many bytes to store
struct TAC* makeTACStore(uint32_t taddr, uint32_t tmp, uint8_t width);
