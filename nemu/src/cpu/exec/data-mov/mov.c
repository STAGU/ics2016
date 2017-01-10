#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE

void init_tlb();

make_helper(mov_r2cr) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint32_t src = reg_l(m.R_M);
	switch (m.reg) {
		case 0:	cpu.CR0.val = src;
				break;
		case 3:
			cpu.CR3.val = src;
			init_tlb();
			break;
		default: assert(0);
	}

	print_asm("mov %%%s, cr%d", regsl[m.R_M], m.reg);
	return 2;
}

make_helper(mov_cr2r) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	uint32_t src;
	switch (m.reg) {
		case 0: src = cpu.CR0.val;
				break;
		case 3: src = cpu.CR3.val;
				break;
		default: assert(0);
	}
	reg_l(m.R_M) = src;

	print_asm("mov cr%d, %%%s", m.reg, regsl[m.R_M]);
	return 2;
}

void add_sreg(uint8_t);

make_helper(mov_rm2s_l) {
	int len = 1 + decode_rm2r_w(eip + 1);
	uint8_t sr = op_dest->reg;
	sreg(sr) = (uint16_t)op_src->val;
	add_sreg(sr);

	print_asm("movw %s,%%%s", op_src->str, sregs[sr]);
	return len;
}

/* for instruction encoding overloading */

make_helper_v(mov_i2r)
make_helper_v(mov_i2rm)
make_helper_v(mov_r2rm)
make_helper_v(mov_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
