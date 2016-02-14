/*-------------------------------------*/
/* User stack area                     */
/*-------------------------------------*/

.section .STACK, "w"
	.align
	.space 2048	/* Reserve 2KB for user stack */
stack_base:



/*-------------------------------------*/
/* Vector table for exception handlers */
/*-------------------------------------*/

.section .VECTOR, "ax"

vector_table:
	.long	boot_por	/* 0 Power-on reset vector, PC */
	.long	stack_base	/* 1 Power-on reset vector, SP */
	.long	boot_mr		/* 2 Manual reset vector, PC */
	.long	stack_base	/* 3 Manual reset vector, SP */
	.long	trap		/* 4 General illigal instruction */
	.long	trap		/* 5 - */
	.long	trap		/* 6 Slot illigal instruction */
	.long	trap		/* 7 - */
	.long	vector_table	/* 8 - <Used by SPI-FlashBoot> */
	.long	trap		/* 9 CPU address error */
	.long	trap		/* 10 DMA address error */
	.long	trap		/* 11 NMI */
	.long	trap		/* 12 - */
	.long	trap		/* 13 FPU exception */
	.long	trap		/* 14 User debug interface */
	.long	trap		/* 15 Bank overflow */
	.long	trap		/* 16 Bank underflow */
	.long	trap		/* 17 Division by 0 */
	.long	trap		/* 18 Division overflow */
	.long	trap		/* 19 - */
	.long	trap		/* 20 - */
	.long	trap		/* 21 - */
	.long	trap		/* 22 - */
	.long	trap		/* 23 - */
	.long	trap		/* 24 - */
	.long	trap		/* 25 - */
	.long	trap		/* 26 - */
	.long	trap		/* 27 - */
	.long	trap		/* 28 - */
	.long	trap		/* 29 - */
	.long	trap		/* 30 - */
	.long	trap		/* 31 - */

	.long	trap		/* 32 TRAP instruction */
	.long	trap		/* 33 TRAP instruction */
	.long	trap		/* 34 TRAP instruction */
	.long	trap		/* 35 TRAP instruction */
	.long	trap		/* 36 TRAP instruction */
	.long	trap		/* 37 TRAP instruction */
	.long	trap		/* 38 TRAP instruction */
	.long	trap		/* 39 TRAP instruction */
	.long	trap		/* 40 TRAP instruction */
	.long	trap		/* 41 TRAP instruction */
	.long	trap		/* 42 TRAP instruction */
	.long	trap		/* 43 TRAP instruction */
	.long	trap		/* 44 TRAP instruction */
	.long	trap		/* 45 TRAP instruction */
	.long	trap		/* 46 TRAP instruction */
	.long	trap		/* 47 TRAP instruction */
	.long	trap		/* 48 TRAP instruction */
	.long	trap		/* 49 TRAP instruction */
	.long	trap		/* 50 TRAP instruction */
	.long	trap		/* 51 TRAP instruction */
	.long	trap		/* 52 TRAP instruction */
	.long	trap		/* 53 TRAP instruction */
	.long	trap		/* 54 TRAP instruction */
	.long	trap		/* 55 TRAP instruction */
	.long	trap		/* 56 TRAP instruction */
	.long	trap		/* 57 TRAP instruction */
	.long	trap		/* 58 TRAP instruction */
	.long	trap		/* 59 TRAP instruction */
	.long	trap		/* 60 TRAP instruction */
	.long	trap		/* 61 TRAP instruction */
	.long	trap		/* 62 TRAP instruction */
	.long	trap		/* 63 TRAP instruction */

	.long	trap		/* 64 IRQ0 */
	.long	trap		/* 65 IRQ1 */
	.long	trap		/* 66 IRQ2 */
	.long	trap		/* 67 IRQ3 */
	.long	trap		/* 68 IRQ4 */
	.long	trap		/* 69 IRQ5 */
	.long	trap		/* 70 IRQ6 */
	.long	trap		/* 71 IRQ7 */
	.long	trap		/* 72 - */
	.long	trap		/* 73 - */
	.long	trap		/* 74 - */
	.long	trap		/* 75 - */
	.long	trap		/* 76 - */
	.long	trap		/* 77 - */
	.long	trap		/* 78 - */
	.long	trap		/* 79 - */
	.long	trap		/* 80 PINT0 */
	.long	trap		/* 81 PINT1 */
	.long	trap		/* 82 PINT2 */
	.long	trap		/* 83 PINT3 */
	.long	trap		/* 84 PINT4 */
	.long	trap		/* 85 PINT5 */
	.long	trap		/* 86 PINT6 */
	.long	trap		/* 87 PINT7 */
	.long	trap		/* 88 - */
	.long	trap		/* 89 - */
	.long	trap		/* 90 - */
	.long	trap		/* 91 - */
	.long	trap		/* 92 - */
	.long	trap		/* 93 - */
	.long	trap		/* 94 - */
	.long	trap		/* 95 - */
	.long	trap		/* 96 - */
	.long	trap		/* 97 - */
	.long	trap		/* 98 - */
	.long	trap		/* 99 - */
	.long	trap		/* 100 - */
	.long	trap		/* 101 - */
	.long	trap		/* 102 - */
	.long	trap		/* 103 - */
	.long	trap		/* 104 - */
	.long	trap		/* 105 - */
	.long	trap		/* 106 - */
	.long	trap		/* 107 - */
	.long	trap		/* 108 DEI0 */
	.long	trap		/* 109 HEI0 */
	.long	trap		/* 110 - */
	.long	trap		/* 111 - */
	.long	trap		/* 112 DEI1 */
	.long	trap		/* 113 HEI1 */
	.long	trap		/* 114 - */
	.long	trap		/* 115 - */
	.long	trap		/* 116 DEI2 */
	.long	trap		/* 117 HEI2 */
	.long	trap		/* 118 - */
	.long	trap		/* 119 - */
	.long	trap		/* 120 DEI3 */
	.long	trap		/* 121 HEI3 */
	.long	trap		/* 122 - */
	.long	trap		/* 123 - */
	.long	trap		/* 124 DEI4 */
	.long	trap		/* 125 HEI4 */
	.long	trap		/* 126 - */
	.long	trap		/* 127 - */
	.long	trap		/* 128 DEI5 */
	.long	trap		/* 129 HEI5 */
	.long	trap		/* 130 - */
	.long	trap		/* 131 - */
	.long	trap		/* 132 DEI6 */
	.long	trap		/* 133 HEI6 */
	.long	trap		/* 134 - */
	.long	trap		/* 135 - */
	.long	trap		/* 136 DEI7 */
	.long	trap		/* 137 HEI7 */
	.long	trap		/* 138 - */
	.long	trap		/* 139 - */
	.long	trap		/* 140 DEI8 */
	.long	trap		/* 141 HEI8 */
	.long	trap		/* 142 - */
	.long	trap		/* 143 - */
	.long	trap		/* 144 DEI9 */
	.long	trap		/* 145 HEI9 */
	.long	trap		/* 146 - */
	.long	trap		/* 147 - */
	.long	trap		/* 148 DEI10 */
	.long	trap		/* 149 HEI10 */
	.long	trap		/* 150 - */
	.long	trap		/* 151 - */
	.long	trap		/* 152 DEI11 */
	.long	trap		/* 153 HEI11 */
	.long	trap		/* 154 - */
	.long	trap		/* 155 - */
	.long	trap		/* 156 DEI12 */
	.long	trap		/* 157 HEI12 */
	.long	trap		/* 158 - */
	.long	trap		/* 159 - */
	.long	trap		/* 160 DEI13 */
	.long	trap		/* 161 HEI13 */
	.long	trap		/* 162 - */
	.long	trap		/* 163 - */
	.long	trap		/* 164 DEI14 */
	.long	trap		/* 165 HEI14 */
	.long	trap		/* 166 - */
	.long	trap		/* 167 - */
	.long	trap		/* 168 DEI15 */
	.long	trap		/* 169 HEI15 */
	.long	trap		/* 170 USBI */
	.long	trap		/* 171 VIVSYNCJ */
	.long	trap		/* 172 VBUFERR */
	.long	trap		/* 173 VIFIELDE */
	.long	trap		/* 174 VOLINE */
	.long	_int_cmt_cmi0	/* 175 CMI0 */
	.long	trap		/* 176 CMI1 */
	.long	trap		/* 177 CMI */
	.long	trap		/* 178 ITI */
	.long	trap		/* 179 TGI0A */
	.long	trap		/* 180 TGI0B */
	.long	trap		/* 181 TGI0C */
	.long	trap		/* 182 TGI0D */
	.long	trap		/* 183 TGI0V */
	.long	trap		/* 184 TGI0E */
	.long	trap		/* 185 TGI0F */
	.long	trap		/* 186 TGI1A */
	.long	trap		/* 187 TGI1B */
	.long	trap		/* 188 TGI1V */
	.long	trap		/* 189 TGI1U */
	.long	trap		/* 190 TGI2A */
	.long	trap		/* 191 TGI2B */
	.long	trap		/* 192 TGI2V */
	.long	trap		/* 193 TGI2U */
	.long	trap		/* 194 TGI3A */
	.long	trap		/* 195 TGI3B */
	.long	trap		/* 196 TGI3C */
	.long	trap		/* 197 TGI3D */
	.long	trap		/* 198 TGI3V */
	.long	trap		/* 199 TGI4A */
	.long	trap		/* 200 TGI4B */
	.long	trap		/* 201 TGI4C */
	.long	trap		/* 202 TGI4D */
	.long	trap		/* 203 TGI4V */
	.long	trap		/* 204 PWM Ch1 */
	.long	trap		/* 205 PWM Ch2 */
	.long	trap		/* 206 ADI */
	.long	trap		/* 207 SSIF0 */
	.long	trap		/* 208 SSIRXI0 */
	.long	trap		/* 209 SSITXI0 */
	.long	trap		/* 210 SSII1 */
	.long	trap		/* 211 SSRTI1 */
	.long	trap		/* 212 SSII2 */
	.long	trap		/* 213 SSIRTI2 */
	.long	trap		/* 214 SSII3 */
	.long	trap		/* 215 SSIRTI3 */
	.long	trap		/* 216 SPDIFI */
	.long	trap		/* 217 STPI0 */
	.long	trap		/* 218 NAKI0 */
	.long	trap		/* 219 RXI0 */
	.long	trap		/* 220 TXI0 */
	.long	trap		/* 221 TEI0 */
	.long	trap		/* 222 STPI1 */
	.long	trap		/* 223 NAKI1 */
	.long	trap		/* 224 RXI1 */
	.long	trap		/* 225 TXI1 */
	.long	trap		/* 226 TEI1 */
	.long	trap		/* 227 STPI2 */
	.long	trap		/* 228 NAKI2 */
	.long	trap		/* 229 RXI2 */
	.long	trap		/* 230 TXI2 */
	.long	trap		/* 231 TEI2 */
	.long	trap		/* 232 BRI0 */
	.long	trap		/* 233 ERI0 */
	.long	trap		/* 234 RXI0 */
	.long	trap		/* 235 TXI0 */
	.long	trap		/* 236 BRI1 */
	.long	trap		/* 237 ERI1 */
	.long	trap		/* 238 RXI1 */
	.long	trap		/* 239 TXI1 */
	.long	trap		/* 240 BRI2 */
	.long	trap		/* 241 ERI2 */
	.long	trap		/* 242 RXI2 */
	.long	trap		/* 243 TXI2 */
	.long	trap		/* 244 BRI3 */
	.long	trap		/* 245 ERI3 */
	.long	trap		/* 246 RXI3 */
	.long	trap		/* 247 TXI3 */
	.long	trap		/* 248 BRI4 */
	.long	trap		/* 249 ERI4 */
	.long	trap		/* 250 TXI4 */
	.long	trap		/* 251 RXI4 */
	.long	trap		/* 252 BRI5 */
	.long	trap		/* 253 ERI5 */
	.long	trap		/* 254 RXI5 */
	.long	trap		/* 255 TXI5 */
	.long	trap		/* 256 BRI6 */
	.long	trap		/* 257 ERI6 */
	.long	trap		/* 258 RXI6 */
	.long	trap		/* 259 TXI6 */
	.long	trap		/* 260 BRI7 */
	.long	trap		/* 261 ERI7 */
	.long	trap		/* 262 RXI7 */
	.long	trap		/* 263 TXI7 */
	.long	trap		/* 264 SIOFI */
	.long	trap		/* 265 SPEI0 */
	.long	trap		/* 266 SPRI0 */
	.long	trap		/* 267 SPTI0 */
	.long	trap		/* 268 SPEI1 */
	.long	trap		/* 269 SPRI1 */
	.long	trap		/* 270 SPTI1 */
	.long	trap		/* 271 ERS0 */
	.long	trap		/* 272 OVR0 */
	.long	trap		/* 273 RM00 */
	.long	trap		/* 274 RM10 */
	.long	trap		/* 275 SLE0 */
	.long	trap		/* 276 ERS1 */
	.long	trap		/* 277 OVR1 */
	.long	trap		/* 278 RM01 */
	.long	trap		/* 279 RM11 */
	.long	trap		/* 280 SLE1 */
	.long	trap		/* 281 IEB */
	.long	trap		/* 282 ISY */
	.long	trap		/* 283 IERR */
	.long	trap		/* 284 ITARG */
	.long	trap		/* 285 ISEC */
	.long	trap		/* 286 IBUF */
	.long	trap		/* 287 IREADY */
	.long	trap		/* 288 FLSTEI */
	.long	trap		/* 289 FLTENDI */
	.long	trap		/* 290 FLTREQ0I */
	.long	trap		/* 291 FLTREQ1I */
	.long	trap		/* 292 SDHI3 */
	.long	trap		/* 293 SDHI0 */
	.long	trap		/* 294 SDHI1 */
	.long	trap		/* 295 - */
	.long	trap		/* 296 ARM */
	.long	trap		/* 297 PRD */
	.long	trap		/* 298 CUP */
	.long	trap		/* 299 OVF0 */
	.long	trap		/* 300 UDF0 */
	.long	trap		/* 301 CEF0 */
	.long	trap		/* 302 ODFI0 */
	.long	trap		/* 303 IDEI0 */
	.long	trap		/* 304 OVF1 */
	.long	trap		/* 305 UDF1 */
	.long	trap		/* 306 CEF1 */
	.long	trap		/* 307 ODFI1 */
	.long	trap		/* 308 IDEI1 */
	.long	trap		/* 309 - */
	.long	trap		/* 310 DCUEI */
	.long	trap		/* 311 OFFI */
	.long	trap		/* 312 IFEI */
	.long	trap		/* 313 - */
	.long	trap		/* 314 - */
	.long	trap		/* 315 - */
	.long	trap		/* 316 - */
	.long	trap		/* 317 - */
	.long	trap		/* 318 - */
	.long	trap		/* 319 - */



/*-------------------------------------*/
/* Bootstrap code                      */
/*-------------------------------------*/

.section .text, "ax"

boot_por:
boot_mr:
	MOV.L	sp_base_adr, SP		/* Initialize SP */
	MOV.L	vbr_adr, R0		/* Initialize VBR */
	LDC	R0, VBR

#	MOV.L	init_memc_func, R0	/* Initialize memory controller */
#	JSR	@R0
#	NOP

#	MOV.L	text_end_adr, R1	/* Initialize .data section */
#	MOV.L	data_start_adr, R2
#	MOV.L	data_end_adr, R3
init_data:
#	CMP/EQ	R2, R3
#	BT	init_data_end
#	MOV.L	@R1+, R0
#	MOV.L	R0, @R2+
#	BRA	init_data
#	NOP
init_data_end:

	MOV.L	bss_start_adr, R1	/* Initialize .bss section */
	MOV.L	bss_end_adr, R2
	MOV	#0, R0
clr_bss:
	CMP/EQ	R2, R1
	BT	clr_bss_end
	MOV.L	R0, @R1+
	BRA	clr_bss
	NOP
clr_bss_end:

	MOV.L	main_adr, R0		/* Start main() */
	JSR	@R0
	NOP
trap:
	BRA	trap			/* Unwanted interrupt trap */
	NOP


	.align
sp_base_adr:
	.long	stack_base
vbr_adr:
	.long	vector_table
bss_start_adr:
	.long	__bss_start__
bss_end_adr:
	.long	__bss_end__
main_adr:
	.long	_main



/*-------------------------------------*/
/* Set interrupt mask level            */
/*-------------------------------------*/
/* void set_imask (long val);    */

.global _set_imask
_set_imask:
	MOV.L	imask_adr, R0
	SHLL2	R4
	SHLL2	R4
	AND	R0, R4
	NOT	R0, R0
	STC	SR, R1
	AND	R0, R1
	OR	R4, R1
	LDC	R1, SR
	RTS
	NOP

.global _set_sp
_set_sp:
	MOV	R4, R15
	RTS
	NOP

imask_adr:
	.long	0x000000F0

