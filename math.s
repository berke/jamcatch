	.file	"math.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.text
.Ltext0:
	.p2align 4,,15
.globl __udivsi3
	.type	__udivsi3, @function
__udivsi3:
.LFB12:
	.file 1 "math.c"
	.loc 1 14 0
.LVL0:
	pushl	%ebp
.LCFI0:
	movl	%esp, %ebp
.LCFI1:
	movl	12(%ebp), %eax
	pushl	%edi
.LCFI2:
	movl	8(%ebp), %ecx
	pushl	%esi
.LCFI3:
	pushl	%ebx
.LCFI4:
	.loc 1 19 0
	testl	%eax, %eax
	je	.L2
.LVL1:
	.loc 1 20 0
	cmpl	%ecx, %eax
	ja	.L2
	.loc 1 24 0
	testl	%eax, %eax
	js	.L5
	leal	(%eax,%eax), %edx
.LVL2:
	cmpl	%edx, %ecx
	.p2align 4,,3
	jb	.L5
	movl	$1, %edi
.LVL3:
	.p2align 4,,3
	jmp	.L8
.LVL4:
	.p2align 4,,7
.L25:
	leal	(%edx,%edx), %eax
	cmpl	%eax, %ecx
	jb	.L9
	movl	%eax, %edx
.LVL5:
.L8:
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	testl	%edx, %edx
	jns	.L25
.L9:
	.loc 1 29 0
	testl	%edi, %edi
	movl	%edx, %eax
.LVL6:
	je	.L2
.LVL7:
.L13:
	.loc 1 30 0
	xorl	%ebx, %ebx
.LVL8:
	.p2align 4,,7
.L19:
	cmpl	%eax, %ecx
	jb	.L16
.LVL9:
	movl	%eax, %esi
	movl	%ecx, %edx
.LVL10:
	negl	%esi
	subl	%eax, %edx
	.p2align 4,,7
.L14:
	.loc 1 33 0
	addl	%esi, %edx
	addl	%edi, %ebx
	.loc 1 30 0
	leal	(%edx,%eax), %ecx
.LVL11:
	cmpl	%eax, %ecx
	jae	.L14
.LVL12:
.L16:
	.loc 1 29 0
	shrl	%edi
	je	.L17
	.loc 1 36 0
	shrl	%eax
	jmp	.L19
.LVL13:
.L2:
	.loc 1 30 0
	xorl	%ebx, %ebx
.LVL14:
.LVL15:
.L17:
	.loc 1 40 0
	movl	%ebx, %eax
.LVL16:
	popl	%ebx
.LVL17:
	popl	%esi
	popl	%edi
.LVL18:
	popl	%ebp
	ret
.LVL19:
.L5:
	.loc 1 30 0
	movl	$1, %edi
.LVL20:
	jmp	.L13
.LFE12:
	.size	__udivsi3, .-__udivsi3
	.p2align 4,,15
.globl __udivdi3
	.type	__udivdi3, @function
__udivdi3:
.LFB14:
	.loc 1 60 0
.LVL21:
	pushl	%ebp
.LCFI5:
	movl	%esp, %ebp
.LCFI6:
	pushl	%edi
.LCFI7:
	pushl	%esi
.LCFI8:
	pushl	%ebx
.LCFI9:
	subl	$28, %esp
.LCFI10:
.LVL22:
	.loc 1 60 0
	movl	20(%ebp), %ebx
	movl	16(%ebp), %ecx
	movl	8(%ebp), %eax
.LVL23:
	movl	12(%ebp), %edx
	.loc 1 64 0
	movl	%ebx, %esi
	orl	%ecx, %esi
	je	.L27
.LVL24:
	.loc 1 65 0
	cmpl	%edx, %ebx
	jbe	.L57
.LVL25:
.L27:
	.loc 1 75 0
	movl	$0, -40(%ebp)
.LVL26:
	.loc 1 85 0
	movl	-40(%ebp), %eax
.LVL27:
	.loc 1 75 0
	movl	$0, -36(%ebp)
.LVL28:
	.loc 1 85 0
	movl	-36(%ebp), %edx
.LVL29:
	addl	$28, %esp
	popl	%ebx
	popl	%esi
	popl	%edi
.LVL30:
	popl	%ebp
	ret
.LVL31:
	.p2align 4,,7
.L57:
	.loc 1 65 0
	jae	.L58
.L30:
	.loc 1 69 0
	movl	%ecx, %esi
.LVL32:
	movl	%ebx, %edi
.LVL33:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	cmpl	%edi, %edx
	jae	.L59
.L31:
	.loc 1 75 0
	movl	$1, -32(%ebp)
.LVL34:
	movl	$0, -28(%ebp)
.LVL35:
.L39:
	movl	$0, -40(%ebp)
.LVL36:
	movl	$0, -36(%ebp)
.LVL37:
	jmp	.L46
.LVL38:
	.p2align 4,,7
.L60:
	.loc 1 81 0
	shrdl	$1, %ebx, %ecx
	shrl	%ebx
.LVL39:
.L46:
	.loc 1 75 0
	cmpl	%ebx, %edx
	jb	.L43
.LVL40:
	ja	.L48
	cmpl	%ecx, %eax
	.p2align 4,,3
	jb	.L43
.L48:
	movl	%ecx, %esi
.LVL41:
	movl	%ebx, %edi
.LVL42:
	negl	%esi
	adcl	$0, %edi
	negl	%edi
	movl	%esi, -24(%ebp)
	movl	%eax, %esi
	movl	%edi, -20(%ebp)
	subl	%ecx, %esi
	movl	%edx, %edi
	sbbl	%ebx, %edi
.LVL43:
.L55:
	.loc 1 78 0
	movl	-32(%ebp), %eax
	addl	%eax, -40(%ebp)
	movl	-28(%ebp), %edx
	adcl	%edx, -36(%ebp)
	addl	-24(%ebp), %esi
	adcl	-20(%ebp), %edi
	.loc 1 75 0
	movl	%esi, %eax
	addl	%ecx, %eax
	movl	%edi, %edx
	adcl	%ebx, %edx
	cmpl	%ebx, %edx
	ja	.L55
	jb	.L43
	cmpl	%ecx, %eax
	jae	.L55
.LVL44:
	.p2align 4,,7
.L43:
	.loc 1 80 0
	movl	-28(%ebp), %edi
.LVL45:
	movl	-32(%ebp), %esi
.LVL46:
	shrdl	$1, %edi, %esi
	shrl	%edi
	movl	%edi, -28(%ebp)
	.loc 1 74 0
	orl	%esi, %edi
	.loc 1 80 0
	movl	%esi, -32(%ebp)
	.loc 1 74 0
	jne	.L60
	.loc 1 85 0
	movl	-40(%ebp), %eax
	movl	-36(%ebp), %edx
	addl	$28, %esp
	popl	%ebx
.LVL47:
	popl	%esi
	popl	%edi
	popl	%ebp
	ret
.LVL48:
.L58:
	.loc 1 65 0
	cmpl	%eax, %ecx
	ja	.L27
	jmp	.L30
.LVL49:
.L59:
	.loc 1 69 0
	.p2align 4,,9
	ja	.L33
	cmpl	%esi, %eax
	.p2align 4,,9
	jb	.L31
.L33:
	movl	$1, -32(%ebp)
.LVL50:
	movl	$0, -28(%ebp)
.LVL51:
.LVL52:
.L34:
	.loc 1 72 0
	movl	-32(%ebp), %ecx
.LVL53:
	movl	-28(%ebp), %ebx
.LVL54:
	shldl	$1, %ecx, %ebx
	addl	%ecx, %ecx
	movl	%ecx, -32(%ebp)
	.loc 1 69 0
	movl	%esi, %ecx
.LVL55:
	.loc 1 72 0
	movl	%ebx, -28(%ebp)
	.loc 1 69 0
	movl	%edi, %ebx
.LVL56:
	shldl	$1, %ecx, %ebx
	addl	%ecx, %ecx
	cmpl	%ebx, %edx
	jb	.L35
	jbe	.L61
.L37:
	movl	%ecx, %esi
	movl	%ebx, %edi
	jmp	.L34
.L61:
	cmpl	%ecx, %eax
	.p2align 4,,5
	jae	.L37
.L35:
	.loc 1 74 0
	movl	%esi, %ecx
.LVL57:
	movl	-28(%ebp), %esi
.LVL58:
	movl	%edi, %ebx
.LVL59:
	orl	-32(%ebp), %esi
	jne	.L39
.LVL60:
	jmp	.L27
.LFE14:
	.size	__udivdi3, .-__udivdi3
	.p2align 4,,15
.globl div_u32_u32
	.type	div_u32_u32, @function
div_u32_u32:
.LFB17:
	.loc 1 134 0
.LVL61:
	pushl	%ebp
.LCFI11:
	xorl	%edx, %edx
	movl	%esp, %ebp
.LCFI12:
	.loc 1 134 0
	movl	8(%ebp), %eax
	divl	12(%ebp)
	.loc 1 136 0
	popl	%ebp
	ret
.LFE17:
	.size	div_u32_u32, .-div_u32_u32
	.p2align 4,,15
.globl div_s32_s32
	.type	div_s32_s32, @function
div_s32_s32:
.LFB18:
	.loc 1 138 0
.LVL62:
	pushl	%ebp
.LCFI13:
	movl	%esp, %ebp
.LCFI14:
	.loc 1 138 0
	movl	8(%ebp), %edx
	movl	%edx, %eax
	sarl	$31, %edx
	idivl	12(%ebp)
	.loc 1 140 0
	popl	%ebp
	ret
.LFE18:
	.size	div_s32_s32, .-div_s32_s32
	.p2align 4,,15
.globl __divdi3
	.type	__divdi3, @function
__divdi3:
.LFB15:
	.loc 1 87 0
.LVL63:
	pushl	%ebp
.LCFI15:
	.loc 1 88 0
	xorl	%ecx, %ecx
	.loc 1 87 0
	movl	%esp, %ebp
.LCFI16:
	subl	$144, %esp
.LCFI17:
.LVL64:
	movl	12(%ebp), %edx
	movl	8(%ebp), %eax
	movl	%esi, -8(%ebp)
.LCFI18:
	movl	%ebx, -12(%ebp)
.LCFI19:
	.loc 1 88 0
	xorl	%ebx, %ebx
	.loc 1 87 0
	movl	%edx, -140(%ebp)
.LVL65:
	movl	20(%ebp), %edx
	movl	%eax, -144(%ebp)
	movl	16(%ebp), %eax
	movl	%edi, -4(%ebp)
.LCFI20:
	.loc 1 88 0
	movl	%edx, %esi
	orl	%eax, %esi
	je	.L69
.LVL66:
	.loc 1 89 0
	movl	-140(%ebp), %ecx
	testl	%ecx, %ecx
	js	.L186
	.loc 1 96 0
	testl	%edx, %edx
	js	.L187
.LVL67:
.LVL68:
.LBB10:
.LBB11:
	.loc 1 64 0
	testl	%eax, %eax
	je	.L140
.LBE11:
.LBE10:
	.loc 1 99 0
	movl	-144(%ebp), %ecx
.LVL69:
	xorl	%edx, %edx
.LVL70:
	xorl	%ebx, %ebx
.LVL71:
.LVL72:
.LBB12:
.LBB13:
	.loc 1 65 0
	cmpl	%ecx, %eax
	ja	.L140
	.loc 1 69 0
	movl	%eax, %esi
.LVL73:
	movl	%edx, %edi
.LVL74:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	cmpl	%edi, %ebx
	jb	.L144
	cmpl	%esi, %ecx
	jae	.L188
.L144:
	.loc 1 75 0
	movl	%eax, %esi
	movl	%edx, %edi
	movl	$1, -24(%ebp)
.LVL75:
	movl	$0, -20(%ebp)
.LVL76:
.L151:
	movl	$0, -80(%ebp)
.LVL77:
	movl	$0, -76(%ebp)
.LVL78:
	jmp	.L158
.LVL79:
	.p2align 4,,7
.L156:
	.loc 1 81 0
	shrdl	$1, %edi, %esi
	shrl	%edi
.LVL80:
.L158:
	.loc 1 75 0
	cmpl	%edi, %ebx
	jb	.L155
	ja	.L160
	cmpl	%esi, %ecx
	.p2align 4,,3
	jb	.L155
.L160:
	movl	%esi, %eax
.LVL81:
	movl	%edi, %edx
.LVL82:
	negl	%eax
	adcl	$0, %edx
	negl	%edx
	subl	%esi, %ecx
.LVL83:
	movl	%eax, -56(%ebp)
	sbbl	%edi, %ebx
.LVL84:
	movl	%edx, -52(%ebp)
.L185:
	.loc 1 78 0
	movl	-24(%ebp), %eax
	addl	%eax, -80(%ebp)
	movl	-20(%ebp), %edx
	adcl	%edx, -76(%ebp)
	addl	-56(%ebp), %ecx
	adcl	-52(%ebp), %ebx
	.loc 1 75 0
	movl	%ecx, %eax
	addl	%esi, %eax
	movl	%ebx, %edx
	adcl	%edi, %edx
	cmpl	%edi, %edx
	ja	.L185
	jb	.L154
	cmpl	%esi, %eax
	jae	.L185
.L154:
	movl	%eax, %ecx
.LVL85:
	movl	%edx, %ebx
.LVL86:
.L155:
	.loc 1 80 0
	movl	-20(%ebp), %edx
.LVL87:
	movl	-24(%ebp), %eax
.LVL88:
	shrdl	$1, %edx, %eax
	shrl	%edx
	movl	%edx, -20(%ebp)
	.loc 1 74 0
	orl	%eax, %edx
	.loc 1 80 0
	movl	%eax, -24(%ebp)
	.loc 1 74 0
	jne	.L156
	movl	-80(%ebp), %ecx
.LVL89:
	movl	-76(%ebp), %ebx
.LVL90:
.LVL91:
	.p2align 4,,7
.L69:
.LBE13:
.LBE12:
	.loc 1 102 0
	movl	%ebx, %edx
.LVL92:
	movl	-8(%ebp), %esi
.LVL93:
	movl	%ecx, %eax
.LVL94:
	movl	-12(%ebp), %ebx
	movl	-4(%ebp), %edi
.LVL95:
	movl	%ebp, %esp
	popl	%ebp
	ret
.LVL96:
	.p2align 4,,7
.L186:
	.loc 1 90 0
	testl	%edx, %edx
	js	.L189
.LVL97:
.LVL98:
.LBB14:
.LBB15:
	.loc 1 64 0
	testl	%eax, %eax
	je	.L140
.LBE15:
.LBE14:
	.loc 1 93 0
	movl	%eax, -112(%ebp)
.LVL99:
	movl	-144(%ebp), %eax
	xorl	%ebx, %ebx
.LVL100:
	movl	$0, -108(%ebp)
.LVL101:
	negl	%eax
.LVL102:
.LBB16:
.LBB17:
	.loc 1 65 0
	cmpl	%eax, -112(%ebp)
.LVL103:
.LBE17:
.LBE16:
	.loc 1 93 0
	movl	%eax, %ecx
.LVL104:
.LBB18:
.LBB19:
	.loc 1 65 0
	jbe	.L190
.LVL105:
	.p2align 4,,7
.L140:
.LBE19:
.LBE18:
.LBB20:
.LBB21:
	.loc 1 75 0
	xorl	%ecx, %ecx
.LVL106:
	xorl	%ebx, %ebx
.LVL107:
	jmp	.L69
.LVL108:
	.p2align 4,,7
.L187:
.LBE21:
.LBE20:
.LBB22:
.LBB23:
	.loc 1 64 0
	negl	%eax
.LVL109:
	je	.L140
.LBE23:
.LBE22:
	.loc 1 97 0
	movl	-144(%ebp), %ecx
.LVL110:
	xorl	%ebx, %ebx
.LVL111:
	movl	%eax, -88(%ebp)
.LVL112:
.LBB24:
.LBB25:
	.loc 1 65 0
	cmpl	%ecx, -88(%ebp)
.LBE25:
.LBE24:
	.loc 1 97 0
	movl	$0, -84(%ebp)
.LVL113:
.LBB26:
.LBB27:
	.loc 1 65 0
	ja	.L140
	.loc 1 69 0
	movl	-88(%ebp), %eax
.LVL114:
	movl	-84(%ebp), %edx
.LVL115:
	shldl	$1, %eax, %edx
	addl	%eax, %eax
	cmpl	%edx, %ebx
	jb	.L123
	cmpl	%eax, %ecx
	jb	.L123
	movl	$1, -96(%ebp)
.LVL116:
	movl	$0, -92(%ebp)
.LVL117:
.LVL118:
.L126:
	.loc 1 72 0
	movl	-96(%ebp), %esi
.LVL119:
	movl	-92(%ebp), %edi
.LVL120:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	movl	%esi, -96(%ebp)
	.loc 1 69 0
	movl	%eax, %esi
.LVL121:
	.loc 1 72 0
	movl	%edi, -92(%ebp)
	.loc 1 69 0
	movl	%edx, %edi
.LVL122:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	cmpl	%edi, %ebx
	jb	.L127
	ja	.L129
	cmpl	%esi, %ecx
	jb	.L127
.L129:
	movl	%esi, %eax
	movl	%edi, %edx
	.p2align 4,,5
	jmp	.L126
.LVL123:
.L190:
.LBE27:
.LBE26:
.LBB28:
.LBB29:
	movl	-112(%ebp), %eax
	movl	-108(%ebp), %edx
.LVL124:
	shldl	$1, %eax, %edx
	addl	%eax, %eax
	cmpl	%edx, %ebx
	movl	%eax, -144(%ebp)
.LVL125:
	movl	%edx, -140(%ebp)
.LVL126:
	jb	.L99
	cmpl	%eax, %ecx
	jb	.L99
	movl	-144(%ebp), %esi
	movl	$1, %eax
.LVL127:
	xorl	%edx, %edx
.LVL128:
	movl	-140(%ebp), %edi
.LVL129:
.L102:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	.loc 1 72 0
	shldl	$1, %eax, %edx
	addl	%eax, %eax
	.loc 1 69 0
	cmpl	%edi, %ebx
	movl	%esi, -40(%ebp)
	movl	%edi, -36(%ebp)
	jb	.L103
	ja	.L105
	cmpl	%esi, %ecx
	jb	.L103
.L105:
	movl	-40(%ebp), %esi
	movl	-36(%ebp), %edi
	movl	%esi, -144(%ebp)
	movl	%edi, -140(%ebp)
	jmp	.L102
.LVL130:
.L188:
.LBE29:
.LBE28:
.LBB30:
.LBB31:
	movl	$1, -24(%ebp)
.LVL131:
	movl	$0, -20(%ebp)
.LVL132:
.LVL133:
.L147:
	.loc 1 72 0
	movl	-24(%ebp), %eax
.LVL134:
	movl	-20(%ebp), %edx
.LVL135:
	shldl	$1, %eax, %edx
	addl	%eax, %eax
	movl	%eax, -24(%ebp)
	.loc 1 69 0
	movl	%esi, %eax
.LVL136:
	.loc 1 72 0
	movl	%edx, -20(%ebp)
	.loc 1 69 0
	movl	%edi, %edx
.LVL137:
	shldl	$1, %eax, %edx
	addl	%eax, %eax
	cmpl	%edx, %ebx
	jb	.L148
	ja	.L150
	cmpl	%eax, %ecx
	jb	.L148
.L150:
	movl	%eax, %esi
	movl	%edx, %edi
	.p2align 4,,5
	jmp	.L147
.LVL138:
.L189:
.LBE31:
.LBE30:
.LBB32:
.LBB33:
	.loc 1 64 0
	negl	%eax
.LVL139:
	.p2align 4,,3
	je	.L140
.LBE33:
.LBE32:
	.loc 1 91 0
	movl	%eax, -32(%ebp)
.LVL140:
	movl	-144(%ebp), %eax
	xorl	%ebx, %ebx
.LVL141:
	movl	$0, -28(%ebp)
.LVL142:
	negl	%eax
.LVL143:
.LBB34:
.LBB35:
	.loc 1 65 0
	cmpl	%eax, -32(%ebp)
.LVL144:
.LBE35:
.LBE34:
	.loc 1 91 0
	movl	%eax, %ecx
.LVL145:
.LBB36:
.LBB37:
	.loc 1 65 0
	ja	.L140
	.loc 1 69 0
	movl	-32(%ebp), %eax
.LVL146:
	movl	-28(%ebp), %edx
.LVL147:
	shldl	$1, %eax, %edx
	addl	%eax, %eax
	cmpl	%edx, %ebx
	jb	.L78
	cmpl	%eax, %ecx
	jb	.L78
	movl	$1, -128(%ebp)
.LVL148:
	movl	$0, -124(%ebp)
.LVL149:
.LVL150:
.L81:
	.loc 1 72 0
	movl	-128(%ebp), %esi
.LVL151:
	movl	-124(%ebp), %edi
.LVL152:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	movl	%esi, -128(%ebp)
	.loc 1 69 0
	movl	%eax, %esi
.LVL153:
	.loc 1 72 0
	movl	%edi, -124(%ebp)
	.loc 1 69 0
	movl	%edx, %edi
.LVL154:
	shldl	$1, %esi, %edi
	addl	%esi, %esi
	cmpl	%edi, %ebx
	jb	.L82
	ja	.L84
	cmpl	%esi, %ecx
	jb	.L82
.L84:
	movl	%esi, %eax
	movl	%edi, %edx
	.p2align 4,,5
	jmp	.L81
.LVL155:
	.p2align 4,,7
.L148:
.LBE37:
.LBE36:
.LBB38:
.LBB39:
	.loc 1 74 0
	movl	-20(%ebp), %edx
.LVL156:
	orl	-24(%ebp), %edx
	jne	.L151
	.p2align 4,,3
	jmp	.L140
.LVL157:
.L99:
	.loc 1 75 0
	movl	$1, %eax
.LVL158:
	xorl	%edx, %edx
.LVL159:
.L107:
	movl	$0, -120(%ebp)
.LVL160:
	movl	$0, -116(%ebp)
.LVL161:
	jmp	.L114
.LVL162:
	.p2align 4,,7
.L112:
.LBE39:
.LBE38:
.LBB40:
.LBB41:
	.loc 1 81 0
	movl	-108(%ebp), %edi
	movl	-112(%ebp), %esi
	shrdl	$1, %edi, %esi
	shrl	%edi
	movl	%esi, -112(%ebp)
	movl	%edi, -108(%ebp)
.LVL163:
.L114:
	.loc 1 75 0
	cmpl	-108(%ebp), %ebx
	jb	.L111
	ja	.L116
	cmpl	-112(%ebp), %ecx
	jb	.L111
.L116:
	movl	-112(%ebp), %esi
	movl	-108(%ebp), %edi
	negl	%esi
	adcl	$0, %edi
	negl	%edi
	movl	%esi, -48(%ebp)
	movl	%ecx, %esi
	movl	%edi, -44(%ebp)
	movl	%ebx, %edi
	subl	-112(%ebp), %esi
	sbbl	-108(%ebp), %edi
.L183:
	.loc 1 78 0
	addl	%eax, -120(%ebp)
	.loc 1 75 0
	movl	-112(%ebp), %ecx
.LVL164:
	.loc 1 78 0
	adcl	%edx, -116(%ebp)
	.loc 1 75 0
	movl	-108(%ebp), %ebx
.LVL165:
	.loc 1 78 0
	addl	-48(%ebp), %esi
	adcl	-44(%ebp), %edi
	.loc 1 75 0
	addl	%esi, %ecx
	adcl	%edi, %ebx
	cmpl	-108(%ebp), %ebx
	ja	.L183
	jb	.L111
	cmpl	-112(%ebp), %ecx
	jae	.L183
.LVL166:
	.p2align 4,,7
.L111:
	.loc 1 80 0
	shrdl	$1, %edx, %eax
	shrl	%edx
.LVL167:
	.loc 1 74 0
	movl	%edx, %esi
	orl	%eax, %esi
	jne	.L112
	movl	-120(%ebp), %ecx
.LVL168:
	movl	-116(%ebp), %ebx
.LVL169:
	negl	%ecx
	adcl	$0, %ebx
	negl	%ebx
	jmp	.L69
.LVL170:
.L123:
.LBE41:
.LBE40:
.LBB42:
.LBB43:
	.loc 1 75 0
	movl	-88(%ebp), %eax
	movl	-84(%ebp), %edx
	movl	$1, -96(%ebp)
.LVL171:
	movl	$0, -92(%ebp)
.LVL172:
.L130:
	movl	$0, -104(%ebp)
.LVL173:
	movl	$0, -100(%ebp)
.LVL174:
	jmp	.L137
.LVL175:
	.p2align 4,,7
.L135:
.LBE43:
.LBE42:
.LBB44:
.LBB45:
	.loc 1 81 0
	shrdl	$1, %edx, %eax
	shrl	%edx
.LVL176:
.L137:
	.loc 1 75 0
	cmpl	%edx, %ebx
	jb	.L134
	ja	.L139
	cmpl	%eax, %ecx
	.p2align 4,,3
	jb	.L134
.L139:
	movl	%eax, %esi
.LVL177:
	movl	%edx, %edi
	negl	%esi
	adcl	$0, %edi
	negl	%edi
	movl	%esi, -64(%ebp)
	movl	%ecx, %esi
	movl	%edi, -60(%ebp)
	subl	%eax, %esi
	movl	%ebx, %edi
	sbbl	%edx, %edi
.L184:
	.loc 1 78 0
	movl	-96(%ebp), %ecx
.LVL178:
	addl	%ecx, -104(%ebp)
	movl	-92(%ebp), %ebx
.LVL179:
	adcl	%ebx, -100(%ebp)
	addl	-64(%ebp), %esi
	adcl	-60(%ebp), %edi
	.loc 1 75 0
	movl	%esi, %ecx
	addl	%eax, %ecx
	movl	%edi, %ebx
	adcl	%edx, %ebx
	cmpl	%edx, %ebx
	ja	.L184
	jb	.L134
	cmpl	%eax, %ecx
	jae	.L184
.LVL180:
	.p2align 4,,7
.L134:
	.loc 1 80 0
	movl	-92(%ebp), %edi
	movl	-96(%ebp), %esi
.LVL181:
	shrdl	$1, %edi, %esi
	shrl	%edi
	movl	%edi, -92(%ebp)
	.loc 1 74 0
	orl	%esi, %edi
	.loc 1 80 0
	movl	%esi, -96(%ebp)
	.loc 1 74 0
	jne	.L135
	movl	-104(%ebp), %ecx
.LVL182:
	movl	-100(%ebp), %ebx
.LVL183:
	negl	%ecx
	adcl	$0, %ebx
	negl	%ebx
	jmp	.L69
.LVL184:
.L127:
	movl	-92(%ebp), %edi
.LVL185:
	orl	-96(%ebp), %edi
	jne	.L130
	jmp	.L140
.LVL186:
.L103:
.LBE45:
.LBE44:
.LBB46:
.LBB47:
	movl	-140(%ebp), %edi
	movl	-144(%ebp), %esi
	movl	%edi, -108(%ebp)
.LVL187:
	movl	%edx, %edi
	orl	%eax, %edi
	movl	%esi, -112(%ebp)
	jne	.L107
	jmp	.L140
.LVL188:
.L78:
.LBE47:
.LBE46:
.LBB48:
.LBB49:
	.loc 1 75 0
	movl	-32(%ebp), %eax
	movl	-28(%ebp), %edx
	movl	$1, -128(%ebp)
.LVL189:
	movl	$0, -124(%ebp)
.LVL190:
.L85:
	movl	$0, -136(%ebp)
.LVL191:
	movl	$0, -132(%ebp)
.LVL192:
	jmp	.L92
.LVL193:
.L90:
.LBE49:
.LBE48:
.LBB50:
.LBB51:
	.loc 1 81 0
	shrdl	$1, %edx, %eax
	shrl	%edx
.LVL194:
.L92:
	.loc 1 75 0
	cmpl	%edx, %ebx
	jb	.L89
	ja	.L94
	cmpl	%eax, %ecx
	.p2align 4,,3
	jb	.L89
.L94:
	movl	%eax, %esi
.LVL195:
	movl	%edx, %edi
	negl	%esi
	adcl	$0, %edi
	negl	%edi
	movl	%esi, -72(%ebp)
	movl	%ecx, %esi
	movl	%edi, -68(%ebp)
	subl	%eax, %esi
	movl	%ebx, %edi
	sbbl	%edx, %edi
.L182:
	.loc 1 78 0
	movl	-128(%ebp), %ecx
.LVL196:
	addl	%ecx, -136(%ebp)
	movl	-124(%ebp), %ebx
.LVL197:
	adcl	%ebx, -132(%ebp)
	addl	-72(%ebp), %esi
	adcl	-68(%ebp), %edi
	.loc 1 75 0
	movl	%esi, %ecx
	addl	%eax, %ecx
	movl	%edi, %ebx
	adcl	%edx, %ebx
	cmpl	%edx, %ebx
	ja	.L182
	jb	.L89
	cmpl	%eax, %ecx
	jae	.L182
.LVL198:
.L89:
	.loc 1 80 0
	movl	-124(%ebp), %edi
	movl	-128(%ebp), %esi
.LVL199:
	shrdl	$1, %edi, %esi
	shrl	%edi
	movl	%edi, -124(%ebp)
	.loc 1 74 0
	orl	%esi, %edi
	.loc 1 80 0
	movl	%esi, -128(%ebp)
	.loc 1 74 0
	jne	.L90
	movl	-136(%ebp), %ecx
.LVL200:
	movl	-132(%ebp), %ebx
.LVL201:
	jmp	.L69
.LVL202:
.L82:
	movl	-124(%ebp), %edi
.LVL203:
	orl	-128(%ebp), %edi
	jne	.L85
	jmp	.L140
.LBE51:
.LBE50:
.LFE15:
	.size	__divdi3, .-__divdi3
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"foo %u / %u = %u\n"
.LC1:
	.string	"%d / %d = %d (%d)\n"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
.LFB16:
	.loc 1 108 0
.LVL204:
	leal	4(%esp), %ecx
.LCFI21:
	andl	$-16, %esp
	pushl	-4(%ecx)
.LCFI22:
.LBB56:
.LBB57:
	.loc 1 27 0
	xorl	%edx, %edx
.LBE57:
.LBE56:
	.loc 1 108 0
	pushl	%ebp
.LCFI23:
	movl	%esp, %ebp
.LCFI24:
	pushl	%edi
.LCFI25:
.LBB58:
.LBB59:
	.loc 1 27 0
	movl	$2, %edi
.LVL205:
.LBE59:
.LBE58:
	.loc 1 108 0
	pushl	%esi
.LCFI26:
	pushl	%ebx
.LCFI27:
	pushl	%ecx
.LCFI28:
.LBB60:
.LBB61:
	.loc 1 27 0
	movl	$84136, %ecx
.LVL206:
.LBE61:
.LBE60:
	.loc 1 108 0
	subl	$40, %esp
.LCFI29:
.LVL207:
.L192:
.LBB62:
.LBB63:
	.loc 1 24 0
	leal	(%ecx,%ecx), %eax
	addl	$1, %edx
	cmpl	$-751619277, %eax
	ja	.L196
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	cmpl	$15, %edx
	je	.L241
	.loc 1 29 0
	movl	%eax, %ecx
	jmp	.L192
.L241:
	.loc 1 24 0
	movl	%eax, %ecx
.L196:
	.loc 1 29 0
	xorl	%esi, %esi
.LVL208:
	testl	%edi, %edi
	movl	$-751619277, %eax
.LVL209:
	je	.L242
.L199:
	.loc 1 30 0
	cmpl	%ecx, %eax
	jb	.L203
	movl	%ecx, %ebx
	subl	%ecx, %eax
.LVL210:
	negl	%ebx
.L201:
	.loc 1 33 0
	addl	%ebx, %eax
	addl	%edi, %esi
	.loc 1 30 0
	leal	(%eax,%ecx), %edx
	cmpl	%edx, %ecx
	jbe	.L201
	movl	%edx, %eax
.LVL211:
.L203:
	.loc 1 29 0
	shrl	%edi
	je	.L204
	.loc 1 36 0
	shrl	%ecx
	jmp	.L199
.L242:
	.loc 1 30 0
	xorl	%esi, %esi
.L204:
.LBE63:
.LBE62:
	.loc 1 114 0
	movl	%esi, 12(%esp)
	movl	$42068, 8(%esp)
	movl	$-751619277, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
.LVL212:
	movl	$0, -20(%ebp)
.LVL213:
.L207:
	.loc 1 118 0
	movl	$1, -28(%ebp)
.L208:
	.loc 1 119 0
	movl	-28(%ebp), %ebx
.LVL214:
	testl	%ebx, %ebx
	jne	.L243
	.loc 1 118 0
	movl	$1, -28(%ebp)
	movl	$1, -24(%ebp)
.L211:
	.loc 1 120 0
	movl	-20(%ebp), %eax
	xorl	%edx, %edx
	divl	-28(%ebp)
	movl	%eax, -32(%ebp)
.LBB64:
.LBB65:
	.loc 1 20 0
	movl	-28(%ebp), %eax
	cmpl	%eax, -20(%ebp)
	jb	.L212
	.loc 1 24 0
	testl	%eax, %eax
	js	.L214
	movl	%eax, %ecx
	addl	%ecx, %ecx
	cmpl	%ecx, -20(%ebp)
	jb	.L214
	movl	$1, %edi
.LVL215:
	jmp	.L217
.LVL216:
	.p2align 4,,7
.L218:
	leal	(%ecx,%ecx), %eax
	cmpl	%eax, -20(%ebp)
	jb	.L219
	movl	%eax, %ecx
.L217:
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	testl	%ecx, %ecx
	jns	.L218
.L219:
	.loc 1 29 0
	testl	%edi, %edi
	je	.L212
.L220:
.LBE65:
.LBE64:
	.loc 1 118 0
	movl	-20(%ebp), %eax
.LVL217:
	xorl	%ebx, %ebx
.LVL218:
	.p2align 4,,7
.L227:
.LBB66:
.LBB67:
	.loc 1 30 0
	cmpl	%ecx, %eax
	jb	.L224
	movl	%ecx, %esi
.LVL219:
	subl	%ecx, %eax
.LVL220:
	negl	%esi
	.p2align 4,,7
.L222:
	.loc 1 33 0
	addl	%esi, %eax
	addl	%edi, %ebx
	.loc 1 30 0
	leal	(%eax,%ecx), %edx
	cmpl	%ecx, %edx
	jae	.L222
	movl	%edx, %eax
.LVL221:
.L224:
	.loc 1 29 0
	shrl	%edi
	je	.L225
	.loc 1 36 0
	shrl	%ecx
	jmp	.L227
.LVL222:
.L212:
	.loc 1 30 0
	xorl	%ebx, %ebx
.LVL223:
.L225:
.LBE67:
.LBE66:
	.loc 1 122 0
	cmpl	-32(%ebp), %ebx
	je	.L229
	.loc 1 123 0
	movl	%ebx, 16(%esp)
	movl	-32(%ebp), %eax
.LVL224:
	movl	%eax, 12(%esp)
	movl	-28(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	-20(%ebp), %eax
	movl	$.LC1, (%esp)
	movl	%eax, 4(%esp)
	call	printf
.LVL225:
.L229:
	.loc 1 118 0
	movl	-24(%ebp), %eax
.LVL226:
	addl	$1, %eax
	cmpl	$999, %eax
	movl	%eax, -28(%ebp)
	jbe	.L208
	.loc 1 117 0
	addl	$1, -20(%ebp)
	cmpl	$1000, -20(%ebp)
	jne	.L207
	.loc 1 130 0
	addl	$40, %esp
	xorl	%eax, %eax
	popl	%ecx
.LVL227:
	popl	%ebx
.LVL228:
	popl	%esi
.LVL229:
	popl	%edi
.LVL230:
	popl	%ebp
	leal	-4(%ecx), %esp
	ret
.LVL231:
.L243:
	.loc 1 119 0
	movl	-28(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	.L211
.L214:
	.loc 1 118 0
	movl	-24(%ebp), %ecx
	movl	$1, %edi
.LVL232:
	jmp	.L220
.LFE16:
	.size	main, .-main
	.p2align 4,,15
.globl __divsi3
	.type	__divsi3, @function
__divsi3:
.LFB13:
	.loc 1 42 0
.LVL233:
	pushl	%ebp
.LCFI30:
	.loc 1 43 0
	xorl	%eax, %eax
	.loc 1 42 0
	movl	%esp, %ebp
.LCFI31:
	pushl	%edi
.LCFI32:
	pushl	%esi
.LCFI33:
	pushl	%ebx
.LCFI34:
	.loc 1 42 0
	movl	12(%ebp), %edx
	movl	8(%ebp), %esi
	.loc 1 43 0
	testl	%edx, %edx
	je	.L247
.LVL234:
	.loc 1 44 0
	testl	%esi, %esi
	js	.L329
	.loc 1 51 0
	testl	%edx, %edx
	js	.L330
.LVL235:
.LBB76:
.LBB77:
	.loc 1 20 0
	cmpl	%esi, %edx
.LBE77:
.LBE76:
	.loc 1 54 0
	movl	%edx, %eax
.LVL236:
	movl	%esi, %ecx
.LVL237:
.LBB78:
.LBB79:
	.loc 1 20 0
	ja	.L300
.LVL238:
	.loc 1 24 0
	addl	%edx, %edx
.LVL239:
	cmpl	%edx, %esi
	jb	.L302
.LVL240:
	movl	$1, %edi
.LVL241:
	jmp	.L304
.LVL242:
.L331:
	movl	%eax, %edx
.LVL243:
.L304:
	leal	(%edx,%edx), %eax
.LVL244:
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	cmpl	%eax, %esi
	jae	.L331
.LVL245:
	.loc 1 29 0
	testl	%edi, %edi
	movl	%edx, %eax
.LVL246:
	je	.L300
.LVL247:
.L308:
	.loc 1 30 0
	xorl	%ebx, %ebx
.LVL248:
	.p2align 4,,7
.L314:
	cmpl	%eax, %ecx
	jb	.L311
	movl	%eax, %esi
	movl	%ecx, %edx
.LVL249:
	negl	%esi
	subl	%eax, %edx
	.p2align 4,,7
.L309:
	.loc 1 33 0
	addl	%esi, %edx
	addl	%edi, %ebx
	.loc 1 30 0
	leal	(%edx,%eax), %ecx
.LVL250:
	cmpl	%eax, %ecx
	jae	.L309
.LVL251:
.L311:
	.loc 1 29 0
	shrl	%edi
	je	.L320
	.loc 1 36 0
	shrl	%eax
	jmp	.L314
.L320:
	.loc 1 29 0
	movl	%ebx, %eax
.LVL252:
.LVL253:
.L247:
.LBE79:
.LBE78:
	.loc 1 57 0
	popl	%ebx
.LVL254:
	popl	%esi
	popl	%edi
.LVL255:
	popl	%ebp
	.p2align 4,,1
	ret
.LVL256:
.L334:
	.loc 1 46 0
	movl	%edx, %eax
.LVL257:
	movl	%esi, %edx
.LVL258:
	negl	%eax
	negl	%edx
.LBB80:
.LBB81:
	.loc 1 20 0
	cmpl	%edx, %eax
	ja	.L300
	.loc 1 24 0
	leal	(%eax,%eax), %ebx
.LVL259:
	cmpl	%ebx, %edx
	jb	.L254
	movl	$1, %edi
.LVL260:
	jmp	.L256
.LVL261:
.L332:
	movl	%eax, %ebx
.LVL262:
.L256:
	leal	(%ebx,%ebx), %eax
.LVL263:
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	cmpl	%eax, %edx
	jae	.L332
	.loc 1 29 0
	testl	%edi, %edi
	jne	.L259
.LVL264:
.L300:
.LBE81:
.LBE80:
	.loc 1 57 0
	popl	%ebx
.LVL265:
.LBB82:
.LBB83:
	.loc 1 30 0
	xorl	%eax, %eax
.LVL266:
.LBE83:
.LBE82:
	.loc 1 57 0
	popl	%esi
.LVL267:
	popl	%edi
.LVL268:
	popl	%ebp
	ret
.LVL269:
.L330:
	.loc 1 52 0
	movl	%edx, %ebx
.LVL270:
	movl	%esi, %ecx
.LVL271:
	negl	%ebx
.LBB84:
.LBB85:
	.loc 1 20 0
	cmpl	%esi, %ebx
	ja	.L300
	.loc 1 24 0
	leal	(%ebx,%ebx), %edx
.LVL272:
	cmpl	%edx, %esi
	jb	.L286
	movl	$1, %edi
.LVL273:
	jmp	.L288
.LVL274:
.L333:
	movl	%eax, %edx
.L288:
	leal	(%edx,%edx), %eax
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	cmpl	%eax, %esi
	jae	.L333
	.loc 1 29 0
	testl	%edi, %edi
	movl	%edx, %ebx
	je	.L300
.L292:
.LBE85:
.LBE84:
.LBB86:
.LBB87:
	.loc 1 30 0
	xorl	%eax, %eax
.LVL275:
.L298:
.LBE87:
.LBE86:
.LBB88:
.LBB89:
	cmpl	%ebx, %ecx
	jb	.L295
.LVL276:
	movl	%ebx, %esi
.LVL277:
	movl	%ecx, %edx
.LVL278:
	negl	%esi
	subl	%ebx, %edx
	.p2align 4,,7
.L293:
	.loc 1 33 0
	addl	%esi, %edx
	addl	%edi, %eax
	.loc 1 30 0
	leal	(%edx,%ebx), %ecx
.LVL279:
	cmpl	%ebx, %ecx
	jae	.L293
.LVL280:
.L295:
	.loc 1 29 0
	shrl	%edi
	je	.L319
	.loc 1 36 0
	shrl	%ebx
	jmp	.L298
.LVL281:
.L329:
.LBE89:
.LBE88:
	.loc 1 45 0
	testl	%edx, %edx
	.p2align 4,,5
	js	.L334
	.loc 1 48 0
	movl	%esi, %ecx
.LVL282:
	negl	%ecx
.LBB90:
.LBB91:
	.loc 1 20 0
	cmpl	%ecx, %edx
	ja	.L300
	.loc 1 24 0
	leal	(%edx,%edx), %ebx
.LVL283:
	cmpl	%ebx, %ecx
	jb	.L269
	movl	$1, %edi
.LVL284:
	jmp	.L271
.LVL285:
.L335:
	movl	%eax, %ebx
.L271:
	leal	(%ebx,%ebx), %eax
	.loc 1 27 0
	addl	%edi, %edi
	.loc 1 24 0
	cmpl	%eax, %ecx
	jae	.L335
	.loc 1 29 0
	testl	%edi, %edi
	je	.L300
.L274:
.LBE91:
.LBE90:
.LBB92:
.LBB93:
	.loc 1 30 0
	xorl	%eax, %eax
.LVL286:
.L280:
.LBE93:
.LBE92:
.LBB94:
.LBB95:
	cmpl	%ebx, %ecx
	jb	.L277
.LVL287:
	movl	%ebx, %esi
.LVL288:
	movl	%ecx, %edx
.LVL289:
	negl	%esi
	subl	%ebx, %edx
	.p2align 4,,7
.L275:
	.loc 1 33 0
	addl	%esi, %edx
	addl	%edi, %eax
	.loc 1 30 0
	leal	(%edx,%ebx), %ecx
.LVL290:
	cmpl	%ebx, %ecx
	jae	.L275
.LVL291:
.L277:
	.loc 1 29 0
	shrl	%edi
	je	.L319
	.loc 1 36 0
	shrl	%ebx
	jmp	.L280
.LVL292:
.L319:
.LBE95:
.LBE94:
	.loc 1 57 0
	popl	%ebx
.LVL293:
.LVL294:
.LBB96:
.LBB97:
	.loc 1 29 0
	negl	%eax
.LVL295:
.LBE97:
.LBE96:
	.loc 1 57 0
	popl	%esi
	popl	%edi
.LVL296:
	popl	%ebp
	.p2align 4,,1
	ret
.LVL297:
.L302:
.LBB98:
.LBB99:
	.loc 1 30 0
	movl	$1, %edi
.LVL298:
	.p2align 4,,2
	jmp	.L308
.LVL299:
.L254:
	movl	%eax, %ebx
	movl	$1, %edi
.LVL300:
.L259:
	xorl	%eax, %eax
.LVL301:
.L265:
.LBE99:
.LBE98:
.LBB100:
.LBB101:
	cmpl	%ebx, %edx
	jb	.L262
.LVL302:
	movl	%ebx, %esi
.LVL303:
	subl	%ebx, %edx
.LVL304:
	negl	%esi
.L260:
	.loc 1 33 0
	addl	%esi, %edx
	addl	%edi, %eax
	.loc 1 30 0
	leal	(%edx,%ebx), %ecx
	cmpl	%ebx, %ecx
	jae	.L260
	movl	%ecx, %edx
.LVL305:
.L262:
	.loc 1 29 0
	shrl	%edi
	je	.L247
	.loc 1 36 0
	shrl	%ebx
	jmp	.L265
.LVL306:
.L286:
.LBE101:
.LBE100:
.LBB102:
.LBB103:
	.loc 1 30 0
	movl	$1, %edi
.LVL307:
	.p2align 4,,3
	jmp	.L292
.LVL308:
.L269:
	movl	%edx, %ebx
	movl	$1, %edi
.LVL309:
	.p2align 4,,3
	jmp	.L274
.LBE103:
.LBE102:
.LFE13:
	.size	__divsi3, .-__divsi3
	.section	.debug_frame,"",@progbits
.Lframe0:
	.long	.LECIE0-.LSCIE0
.LSCIE0:
	.long	0xffffffff
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -4
	.byte	0x8
	.byte	0xc
	.uleb128 0x4
	.uleb128 0x4
	.byte	0x88
	.uleb128 0x1
	.align 4
.LECIE0:
.LSFDE0:
	.long	.LEFDE0-.LASFDE0
.LASFDE0:
	.long	.Lframe0
	.long	.LFB12
	.long	.LFE12-.LFB12
	.byte	0x4
	.long	.LCFI0-.LFB12
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI1-.LCFI0
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI4-.LCFI1
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x87
	.uleb128 0x3
	.align 4
.LEFDE0:
.LSFDE2:
	.long	.LEFDE2-.LASFDE2
.LASFDE2:
	.long	.Lframe0
	.long	.LFB14
	.long	.LFE14-.LFB14
	.byte	0x4
	.long	.LCFI5-.LFB14
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI6-.LCFI5
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI10-.LCFI6
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x87
	.uleb128 0x3
	.align 4
.LEFDE2:
.LSFDE4:
	.long	.LEFDE4-.LASFDE4
.LASFDE4:
	.long	.Lframe0
	.long	.LFB17
	.long	.LFE17-.LFB17
	.byte	0x4
	.long	.LCFI11-.LFB17
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI12-.LCFI11
	.byte	0xd
	.uleb128 0x5
	.align 4
.LEFDE4:
.LSFDE6:
	.long	.LEFDE6-.LASFDE6
.LASFDE6:
	.long	.Lframe0
	.long	.LFB18
	.long	.LFE18-.LFB18
	.byte	0x4
	.long	.LCFI13-.LFB18
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI14-.LCFI13
	.byte	0xd
	.uleb128 0x5
	.align 4
.LEFDE6:
.LSFDE8:
	.long	.LEFDE8-.LASFDE8
.LASFDE8:
	.long	.Lframe0
	.long	.LFB15
	.long	.LFE15-.LFB15
	.byte	0x4
	.long	.LCFI15-.LFB15
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI16-.LCFI15
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI19-.LCFI16
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI20-.LCFI19
	.byte	0x87
	.uleb128 0x3
	.align 4
.LEFDE8:
.LSFDE10:
	.long	.LEFDE10-.LASFDE10
.LASFDE10:
	.long	.Lframe0
	.long	.LFB16
	.long	.LFE16-.LFB16
	.byte	0x4
	.long	.LCFI21-.LFB16
	.byte	0xc
	.uleb128 0x1
	.uleb128 0x0
	.byte	0x9
	.uleb128 0x4
	.uleb128 0x1
	.byte	0x4
	.long	.LCFI22-.LCFI21
	.byte	0xc
	.uleb128 0x4
	.uleb128 0x4
	.byte	0x4
	.long	.LCFI23-.LCFI22
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI24-.LCFI23
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI25-.LCFI24
	.byte	0x87
	.uleb128 0x3
	.byte	0x4
	.long	.LCFI28-.LCFI25
	.byte	0x84
	.uleb128 0x6
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.align 4
.LEFDE10:
.LSFDE12:
	.long	.LEFDE12-.LASFDE12
.LASFDE12:
	.long	.Lframe0
	.long	.LFB13
	.long	.LFE13-.LFB13
	.byte	0x4
	.long	.LCFI30-.LFB13
	.byte	0xe
	.uleb128 0x8
	.byte	0x85
	.uleb128 0x2
	.byte	0x4
	.long	.LCFI31-.LCFI30
	.byte	0xd
	.uleb128 0x5
	.byte	0x4
	.long	.LCFI34-.LCFI31
	.byte	0x83
	.uleb128 0x5
	.byte	0x86
	.uleb128 0x4
	.byte	0x87
	.uleb128 0x3
	.align 4
.LEFDE12:
	.file 2 "./types.h"
	.file 3 "/usr/include/stdio.h"
	.file 4 "/usr/include/libio.h"
	.file 5 "/usr/include/bits/types.h"
	.file 6 "/usr/lib/gcc/i486-linux-gnu/4.1.2/include/stddef.h"
	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.long	.LFB12-.Ltext0
	.long	.LCFI0-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI0-.Ltext0
	.long	.LCFI1-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI1-.Ltext0
	.long	.LFE12-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST1:
	.long	.LVL0-.Ltext0
	.long	.LVL1-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL1-.Ltext0
	.long	.LVL8-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL8-.Ltext0
	.long	.LVL9-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL9-.Ltext0
	.long	.LVL11-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL11-.Ltext0
	.long	.LVL13-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL13-.Ltext0
	.long	.LVL15-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL15-.Ltext0
	.long	.LVL19-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL19-.Ltext0
	.long	.LFE12-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0x0
	.long	0x0
.LLST2:
	.long	.LVL0-.Ltext0
	.long	.LVL1-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 4
	.long	.LVL1-.Ltext0
	.long	.LVL2-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL2-.Ltext0
	.long	.LVL4-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL4-.Ltext0
	.long	.LVL4-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL4-.Ltext0
	.long	.LVL5-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL5-.Ltext0
	.long	.LVL6-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL7-.Ltext0
	.long	.LVL10-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL12-.Ltext0
	.long	.LVL16-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL16-.Ltext0
	.long	.LVL19-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL19-.Ltext0
	.long	.LFE12-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST3:
	.long	.LVL8-.Ltext0
	.long	.LVL13-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL14-.Ltext0
	.long	.LVL17-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0x0
	.long	0x0
.LLST4:
	.long	.LVL3-.Ltext0
	.long	.LVL18-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL20-.Ltext0
	.long	.LFE12-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
.LLST5:
	.long	.LFB14-.Ltext0
	.long	.LCFI5-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI5-.Ltext0
	.long	.LCFI6-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI6-.Ltext0
	.long	.LFE14-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST6:
	.long	.LVL21-.Ltext0
	.long	.LVL23-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL23-.Ltext0
	.long	.LVL24-.Ltext0
	.value	0x7
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x75
	.sleb128 12
	.byte	0x93
	.uleb128 0x4
	.long	.LVL24-.Ltext0
	.long	.LVL27-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL27-.Ltext0
	.long	.LVL29-.Ltext0
	.value	0x7
	.byte	0x75
	.sleb128 8
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL29-.Ltext0
	.long	.LVL31-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL31-.Ltext0
	.long	.LVL38-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL38-.Ltext0
	.long	.LVL40-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL40-.Ltext0
	.long	.LVL43-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL43-.Ltext0
	.long	.LVL48-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL48-.Ltext0
	.long	.LFE14-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST7:
	.long	.LVL21-.Ltext0
	.long	.LVL24-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 8
	.long	.LVL24-.Ltext0
	.long	.LVL25-.Ltext0
	.value	0x7
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x75
	.sleb128 20
	.byte	0x93
	.uleb128 0x4
	.long	.LVL31-.Ltext0
	.long	.LVL32-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL33-.Ltext0
	.long	.LVL38-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL39-.Ltext0
	.long	.LVL41-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL44-.Ltext0
	.long	.LVL45-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL47-.Ltext0
	.long	.LVL48-.Ltext0
	.value	0x7
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x75
	.sleb128 20
	.byte	0x93
	.uleb128 0x4
	.long	.LVL48-.Ltext0
	.long	.LVL49-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL49-.Ltext0
	.long	.LVL52-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL52-.Ltext0
	.long	.LVL52-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL52-.Ltext0
	.long	.LVL53-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 16
	.long	.LVL56-.Ltext0
	.long	.LVL57-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL60-.Ltext0
	.long	.LFE14-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST8:
	.long	.LVL28-.Ltext0
	.long	.LVL31-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	.LVL37-.Ltext0
	.long	.LVL48-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	0x0
	.long	0x0
.LLST9:
	.long	.LVL25-.Ltext0
	.long	.LVL31-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	.LVL35-.Ltext0
	.long	.LVL48-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	.LVL51-.Ltext0
	.long	.LFE14-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	0x0
	.long	0x0
.LLST10:
	.long	.LFB17-.Ltext0
	.long	.LCFI11-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI11-.Ltext0
	.long	.LCFI12-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI12-.Ltext0
	.long	.LFE17-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST11:
	.long	.LFB18-.Ltext0
	.long	.LCFI13-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI13-.Ltext0
	.long	.LCFI14-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI14-.Ltext0
	.long	.LFE18-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST12:
	.long	.LFB15-.Ltext0
	.long	.LCFI15-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI15-.Ltext0
	.long	.LCFI16-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI16-.Ltext0
	.long	.LFE15-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST13:
	.long	.LVL63-.Ltext0
	.long	.LVL66-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL66-.Ltext0
	.long	.LFE15-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -144
	.long	0x0
	.long	0x0
.LLST14:
	.long	.LVL63-.Ltext0
	.long	.LVL66-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 8
	.long	.LVL66-.Ltext0
	.long	.LVL67-.Ltext0
	.value	0x7
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x75
	.sleb128 20
	.byte	0x93
	.uleb128 0x4
	.long	.LVL67-.Ltext0
	.long	.LVL68-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL68-.Ltext0
	.long	.LVL70-.Ltext0
	.value	0x7
	.byte	0x75
	.sleb128 16
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL70-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 16
	.long	.LVL96-.Ltext0
	.long	.LVL97-.Ltext0
	.value	0x7
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x75
	.sleb128 20
	.byte	0x93
	.uleb128 0x4
	.long	.LVL97-.Ltext0
	.long	.LVL98-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL98-.Ltext0
	.long	.LVL105-.Ltext0
	.value	0x7
	.byte	0x75
	.sleb128 16
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 16
	.long	.LVL108-.Ltext0
	.long	.LVL109-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL109-.Ltext0
	.long	.LVL115-.Ltext0
	.value	0x7
	.byte	0x75
	.sleb128 16
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL115-.Ltext0
	.long	.LVL123-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 16
	.long	.LVL123-.Ltext0
	.long	.LVL124-.Ltext0
	.value	0x7
	.byte	0x75
	.sleb128 16
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL124-.Ltext0
	.long	.LVL138-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 16
	.long	.LVL138-.Ltext0
	.long	.LVL139-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL139-.Ltext0
	.long	.LVL147-.Ltext0
	.value	0x7
	.byte	0x75
	.sleb128 16
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL147-.Ltext0
	.long	.LFE15-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 16
	.long	0x0
	.long	0x0
.LLST15:
	.long	.LVL72-.Ltext0
	.long	.LVL73-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL74-.Ltext0
	.long	.LVL76-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL80-.Ltext0
	.long	.LVL81-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL86-.Ltext0
	.long	.LVL87-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL91-.Ltext0
	.long	.LVL92-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL105-.Ltext0
	.long	.LVL105-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL130-.Ltext0
	.long	.LVL130-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL130-.Ltext0
	.long	.LVL134-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL137-.Ltext0
	.long	.LVL138-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL155-.Ltext0
	.long	.LVL155-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL155-.Ltext0
	.long	.LVL156-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST16:
	.long	.LVL71-.Ltext0
	.long	.LVL83-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL86-.Ltext0
	.long	.LVL89-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL105-.Ltext0
	.long	.LVL106-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL130-.Ltext0
	.long	.LVL138-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL155-.Ltext0
	.long	.LVL157-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST17:
	.long	.LVL78-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -80
	.long	0x0
	.long	0x0
.LLST18:
	.long	.LVL76-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -24
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -24
	.long	.LVL132-.Ltext0
	.long	.LVL138-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -24
	.long	.LVL155-.Ltext0
	.long	.LVL157-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -24
	.long	0x0
	.long	0x0
.LLST19:
	.long	.LVL91-.Ltext0
	.long	.LVL91-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	.LVL91-.Ltext0
	.long	.LVL91-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -144
	.long	.LVL91-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -112
	.long	.LVL101-.Ltext0
	.long	.LVL105-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -112
	.long	.LVL105-.Ltext0
	.long	.LVL105-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -144
	.long	.LVL123-.Ltext0
	.long	.LVL125-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -112
	.long	.LVL126-.Ltext0
	.long	.LVL129-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -144
	.long	.LVL129-.Ltext0
	.long	.LVL130-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	.LVL157-.Ltext0
	.long	.LVL157-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -144
	.long	.LVL157-.Ltext0
	.long	.LVL159-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -112
	.long	.LVL159-.Ltext0
	.long	.LVL170-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	.LVL186-.Ltext0
	.long	.LVL186-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -40
	.long	.LVL186-.Ltext0
	.long	.LVL186-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -144
	.long	.LVL186-.Ltext0
	.long	.LVL188-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -112
	.long	0x0
	.long	0x0
.LLST20:
	.long	.LVL102-.Ltext0
	.long	.LVL103-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL104-.Ltext0
	.long	.LVL106-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL123-.Ltext0
	.long	.LVL130-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL157-.Ltext0
	.long	.LVL164-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL166-.Ltext0
	.long	.LVL168-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL186-.Ltext0
	.long	.LVL188-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST21:
	.long	.LVL91-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -120
	.long	.LVL161-.Ltext0
	.long	.LVL170-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -120
	.long	0x0
	.long	0x0
.LLST22:
	.long	.LVL128-.Ltext0
	.long	.LVL130-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL159-.Ltext0
	.long	.LVL162-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL163-.Ltext0
	.long	.LVL167-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL186-.Ltext0
	.long	.LVL187-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST23:
	.long	.LVL91-.Ltext0
	.long	.LVL91-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL91-.Ltext0
	.long	.LVL92-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -88
	.long	.LVL105-.Ltext0
	.long	.LVL105-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -88
	.long	.LVL113-.Ltext0
	.long	.LVL114-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -88
	.long	.LVL115-.Ltext0
	.long	.LVL118-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL118-.Ltext0
	.long	.LVL119-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL122-.Ltext0
	.long	.LVL123-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL170-.Ltext0
	.long	.LVL170-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL170-.Ltext0
	.long	.LVL172-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -88
	.long	.LVL184-.Ltext0
	.long	.LVL185-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST24:
	.long	.LVL105-.Ltext0
	.long	.LVL106-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL111-.Ltext0
	.long	.LVL123-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL170-.Ltext0
	.long	.LVL178-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL180-.Ltext0
	.long	.LVL182-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL184-.Ltext0
	.long	.LVL186-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST25:
	.long	.LVL91-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -104
	.long	.LVL174-.Ltext0
	.long	.LVL184-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -104
	.long	0x0
	.long	0x0
.LLST26:
	.long	.LVL91-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -96
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -96
	.long	.LVL117-.Ltext0
	.long	.LVL123-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -96
	.long	.LVL172-.Ltext0
	.long	.LVL186-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -96
	.long	0x0
	.long	0x0
.LLST27:
	.long	.LVL91-.Ltext0
	.long	.LVL91-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	.LVL91-.Ltext0
	.long	.LVL92-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL105-.Ltext0
	.long	.LVL105-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL142-.Ltext0
	.long	.LVL146-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	.LVL147-.Ltext0
	.long	.LVL150-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL150-.Ltext0
	.long	.LVL151-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL154-.Ltext0
	.long	.LVL155-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	.LVL188-.Ltext0
	.long	.LVL188-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -32
	.long	.LVL188-.Ltext0
	.long	.LVL190-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x52
	.byte	0x93
	.uleb128 0x4
	.long	.LVL202-.Ltext0
	.long	.LVL203-.Ltext0
	.value	0x6
	.byte	0x56
	.byte	0x93
	.uleb128 0x4
	.byte	0x57
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST28:
	.long	.LVL105-.Ltext0
	.long	.LVL106-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL143-.Ltext0
	.long	.LVL144-.Ltext0
	.value	0x6
	.byte	0x50
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL145-.Ltext0
	.long	.LVL155-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL188-.Ltext0
	.long	.LVL196-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL198-.Ltext0
	.long	.LVL200-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	.LVL202-.Ltext0
	.long	.LFE15-.Ltext0
	.value	0x6
	.byte	0x51
	.byte	0x93
	.uleb128 0x4
	.byte	0x53
	.byte	0x93
	.uleb128 0x4
	.long	0x0
	.long	0x0
.LLST29:
	.long	.LVL91-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -136
	.long	.LVL192-.Ltext0
	.long	.LVL202-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -136
	.long	0x0
	.long	0x0
.LLST30:
	.long	.LVL91-.Ltext0
	.long	.LVL96-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -128
	.long	.LVL105-.Ltext0
	.long	.LVL108-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -128
	.long	.LVL149-.Ltext0
	.long	.LVL155-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -128
	.long	.LVL190-.Ltext0
	.long	.LFE15-.Ltext0
	.value	0x3
	.byte	0x75
	.sleb128 -128
	.long	0x0
	.long	0x0
.LLST31:
	.long	.LFB16-.Ltext0
	.long	.LCFI21-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI21-.Ltext0
	.long	.LCFI22-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LCFI22-.Ltext0
	.long	.LCFI23-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI23-.Ltext0
	.long	.LCFI24-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI24-.Ltext0
	.long	.LFE16-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST32:
	.long	.LVL213-.Ltext0
	.long	.LVL217-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -20
	.long	.LVL217-.Ltext0
	.long	.LVL220-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL221-.Ltext0
	.long	.LVL222-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL223-.Ltext0
	.long	.LVL224-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL225-.Ltext0
	.long	.LVL226-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST33:
	.long	.LVL213-.Ltext0
	.long	.LVL213-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -28
	.long	.LVL213-.Ltext0
	.long	.LVL213-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL213-.Ltext0
	.long	.LVL216-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -24
	.long	.LVL216-.Ltext0
	.long	.LVL217-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL222-.Ltext0
	.long	.LVL224-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL225-.Ltext0
	.long	.LVL225-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL225-.Ltext0
	.long	.LVL226-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL231-.Ltext0
	.long	.LFE16-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0x0
	.long	0x0
.LLST34:
	.long	.LVL213-.Ltext0
	.long	.LVL214-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL218-.Ltext0
	.long	.LVL222-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL223-.Ltext0
	.long	.LVL228-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0x0
	.long	0x0
.LLST35:
	.long	.LVL209-.Ltext0
	.long	.LVL210-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL211-.Ltext0
	.long	.LVL212-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST36:
	.long	.LVL206-.Ltext0
	.long	.LVL207-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL207-.Ltext0
	.long	.LVL209-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST37:
	.long	.LVL208-.Ltext0
	.long	.LVL219-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL221-.Ltext0
	.long	.LVL229-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL231-.Ltext0
	.long	.LFE16-.Ltext0
	.value	0x1
	.byte	0x56
	.long	0x0
	.long	0x0
.LLST38:
	.long	.LVL205-.Ltext0
	.long	.LVL215-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL222-.Ltext0
	.long	.LVL230-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL231-.Ltext0
	.long	.LVL232-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
.LLST39:
	.long	.LVL213-.Ltext0
	.long	.LVL230-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL231-.Ltext0
	.long	.LFE16-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
.LLST40:
	.long	.LFB13-.Ltext0
	.long	.LCFI30-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 4
	.long	.LCFI30-.Ltext0
	.long	.LCFI31-.Ltext0
	.value	0x2
	.byte	0x74
	.sleb128 8
	.long	.LCFI31-.Ltext0
	.long	.LFE13-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	0x0
	.long	0x0
.LLST41:
	.long	.LVL233-.Ltext0
	.long	.LVL234-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL234-.Ltext0
	.long	.LVL235-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL235-.Ltext0
	.long	.LVL238-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL238-.Ltext0
	.long	.LVL239-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL239-.Ltext0
	.long	.LVL242-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL242-.Ltext0
	.long	.LVL243-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL243-.Ltext0
	.long	.LVL245-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL245-.Ltext0
	.long	.LVL247-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL247-.Ltext0
	.long	.LVL256-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL256-.Ltext0
	.long	.LVL267-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL267-.Ltext0
	.long	.LVL269-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL269-.Ltext0
	.long	.LVL275-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL275-.Ltext0
	.long	.LVL276-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL276-.Ltext0
	.long	.LVL277-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL277-.Ltext0
	.long	.LVL281-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL281-.Ltext0
	.long	.LVL286-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL286-.Ltext0
	.long	.LVL287-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL287-.Ltext0
	.long	.LVL288-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL288-.Ltext0
	.long	.LVL299-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL299-.Ltext0
	.long	.LVL301-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL301-.Ltext0
	.long	.LVL302-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL302-.Ltext0
	.long	.LVL303-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL303-.Ltext0
	.long	.LVL306-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 8
	.long	.LVL306-.Ltext0
	.long	.LFE13-.Ltext0
	.value	0x1
	.byte	0x56
	.long	0x0
	.long	0x0
.LLST42:
	.long	.LVL233-.Ltext0
	.long	.LVL234-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 4
	.long	.LVL234-.Ltext0
	.long	.LVL239-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL239-.Ltext0
	.long	.LVL256-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL256-.Ltext0
	.long	.LVL258-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL258-.Ltext0
	.long	.LVL269-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL269-.Ltext0
	.long	.LVL272-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL272-.Ltext0
	.long	.LVL281-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL281-.Ltext0
	.long	.LVL286-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL286-.Ltext0
	.long	.LVL287-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL287-.Ltext0
	.long	.LVL289-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL289-.Ltext0
	.long	.LVL308-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 12
	.long	.LVL308-.Ltext0
	.long	.LFE13-.Ltext0
	.value	0x1
	.byte	0x52
	.long	0x0
	.long	0x0
.LLST43:
	.long	.LVL236-.Ltext0
	.long	.LVL239-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL239-.Ltext0
	.long	.LVL242-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL242-.Ltext0
	.long	.LVL244-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL246-.Ltext0
	.long	.LVL249-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL251-.Ltext0
	.long	.LVL252-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL264-.Ltext0
	.long	.LVL264-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL264-.Ltext0
	.long	.LVL266-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL297-.Ltext0
	.long	.LVL297-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL297-.Ltext0
	.long	.LVL299-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST44:
	.long	.LVL235-.Ltext0
	.long	.LVL236-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL237-.Ltext0
	.long	.LVL240-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL240-.Ltext0
	.long	.LVL242-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL242-.Ltext0
	.long	.LVL250-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL251-.Ltext0
	.long	.LVL256-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL264-.Ltext0
	.long	.LVL269-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL297-.Ltext0
	.long	.LVL299-.Ltext0
	.value	0x1
	.byte	0x56
	.long	0x0
	.long	0x0
.LLST45:
	.long	.LVL248-.Ltext0
	.long	.LVL254-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0x0
	.long	0x0
.LLST46:
	.long	.LVL241-.Ltext0
	.long	.LVL255-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL264-.Ltext0
	.long	.LVL268-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL298-.Ltext0
	.long	.LVL299-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
.LLST47:
	.long	.LVL253-.Ltext0
	.long	.LVL254-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL257-.Ltext0
	.long	.LVL259-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL259-.Ltext0
	.long	.LVL261-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL261-.Ltext0
	.long	.LVL263-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL264-.Ltext0
	.long	.LVL265-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL299-.Ltext0
	.long	.LVL299-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL299-.Ltext0
	.long	.LVL301-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST48:
	.long	.LVL253-.Ltext0
	.long	.LVL256-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL258-.Ltext0
	.long	.LVL269-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL299-.Ltext0
	.long	.LVL304-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL305-.Ltext0
	.long	.LVL306-.Ltext0
	.value	0x1
	.byte	0x52
	.long	0x0
	.long	0x0
.LLST49:
	.long	.LVL253-.Ltext0
	.long	.LVL254-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL301-.Ltext0
	.long	.LVL306-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST50:
	.long	.LVL253-.Ltext0
	.long	.LVL255-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL260-.Ltext0
	.long	.LVL268-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL300-.Ltext0
	.long	.LVL306-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
.LLST51:
	.long	.LVL264-.Ltext0
	.long	.LVL264-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL264-.Ltext0
	.long	.LVL264-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL264-.Ltext0
	.long	.LVL265-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL270-.Ltext0
	.long	.LVL272-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL272-.Ltext0
	.long	.LVL274-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL274-.Ltext0
	.long	.LVL275-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL280-.Ltext0
	.long	.LVL281-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL292-.Ltext0
	.long	.LVL292-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL292-.Ltext0
	.long	.LVL293-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL306-.Ltext0
	.long	.LVL306-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL306-.Ltext0
	.long	.LVL308-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0x0
	.long	0x0
.LLST52:
	.long	.LVL264-.Ltext0
	.long	.LVL269-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL271-.Ltext0
	.long	.LVL279-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL280-.Ltext0
	.long	.LVL281-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL292-.Ltext0
	.long	.LVL297-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL306-.Ltext0
	.long	.LVL308-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0x0
	.long	0x0
.LLST53:
	.long	.LVL275-.Ltext0
	.long	.LVL281-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL292-.Ltext0
	.long	.LVL295-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST54:
	.long	.LVL264-.Ltext0
	.long	.LVL268-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL273-.Ltext0
	.long	.LVL281-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL292-.Ltext0
	.long	.LVL296-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL307-.Ltext0
	.long	.LVL308-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
.LLST55:
	.long	.LVL264-.Ltext0
	.long	.LVL264-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL264-.Ltext0
	.long	.LVL265-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL283-.Ltext0
	.long	.LVL285-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL285-.Ltext0
	.long	.LVL286-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL308-.Ltext0
	.long	.LFE13-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0x0
	.long	0x0
.LLST56:
	.long	.LVL264-.Ltext0
	.long	.LVL269-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL282-.Ltext0
	.long	.LVL290-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL291-.Ltext0
	.long	.LVL297-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL308-.Ltext0
	.long	.LFE13-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0x0
	.long	0x0
.LLST57:
	.long	.LVL286-.Ltext0
	.long	.LVL294-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0x0
	.long	0x0
.LLST58:
	.long	.LVL264-.Ltext0
	.long	.LVL268-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL284-.Ltext0
	.long	.LVL296-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL309-.Ltext0
	.long	.LFE13-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0x0
	.long	0x0
	.section	.debug_info
	.long	0x786
	.value	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.Ldebug_line0
	.long	.Letext0
	.long	.Ltext0
	.long	.LASF60
	.byte	0x1
	.long	.LASF61
	.long	.LASF62
	.uleb128 0x2
	.long	.LASF0
	.byte	0x1
	.byte	0x8
	.uleb128 0x2
	.long	.LASF1
	.byte	0x2
	.byte	0x7
	.uleb128 0x3
	.string	"u32"
	.byte	0x2
	.byte	0xe
	.long	0x3e
	.uleb128 0x2
	.long	.LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x3
	.string	"u64"
	.byte	0x2
	.byte	0xf
	.long	0x50
	.uleb128 0x2
	.long	.LASF3
	.byte	0x8
	.byte	0x7
	.uleb128 0x2
	.long	.LASF4
	.byte	0x1
	.byte	0x6
	.uleb128 0x2
	.long	.LASF5
	.byte	0x2
	.byte	0x5
	.uleb128 0x3
	.string	"s32"
	.byte	0x2
	.byte	0x13
	.long	0x70
	.uleb128 0x4
	.string	"int"
	.byte	0x4
	.byte	0x5
	.uleb128 0x3
	.string	"s64"
	.byte	0x2
	.byte	0x14
	.long	0x82
	.uleb128 0x2
	.long	.LASF6
	.byte	0x8
	.byte	0x5
	.uleb128 0x2
	.long	.LASF7
	.byte	0x1
	.byte	0x6
	.uleb128 0x5
	.long	.LASF8
	.byte	0x6
	.byte	0xd6
	.long	0x3e
	.uleb128 0x2
	.long	.LASF9
	.byte	0x4
	.byte	0x7
	.uleb128 0x5
	.long	.LASF10
	.byte	0x5
	.byte	0x3b
	.long	0x82
	.uleb128 0x5
	.long	.LASF11
	.byte	0x5
	.byte	0x90
	.long	0xb8
	.uleb128 0x2
	.long	.LASF12
	.byte	0x4
	.byte	0x5
	.uleb128 0x5
	.long	.LASF13
	.byte	0x5
	.byte	0x91
	.long	0xa2
	.uleb128 0x2
	.long	.LASF2
	.byte	0x4
	.byte	0x7
	.uleb128 0x6
	.byte	0x4
	.uleb128 0x7
	.byte	0x4
	.long	0x89
	.uleb128 0x8
	.long	0x299
	.long	.LASF44
	.byte	0x94
	.byte	0x3
	.byte	0x2e
	.uleb128 0x9
	.long	.LASF14
	.byte	0x4
	.value	0x10c
	.long	0x70
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0x9
	.long	.LASF15
	.byte	0x4
	.value	0x111
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x9
	.long	.LASF16
	.byte	0x4
	.value	0x112
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x9
	.long	.LASF17
	.byte	0x4
	.value	0x113
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.uleb128 0x9
	.long	.LASF18
	.byte	0x4
	.value	0x114
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x10
	.uleb128 0x9
	.long	.LASF19
	.byte	0x4
	.value	0x115
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x14
	.uleb128 0x9
	.long	.LASF20
	.byte	0x4
	.value	0x116
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x18
	.uleb128 0x9
	.long	.LASF21
	.byte	0x4
	.value	0x117
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x1c
	.uleb128 0x9
	.long	.LASF22
	.byte	0x4
	.value	0x118
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x20
	.uleb128 0x9
	.long	.LASF23
	.byte	0x4
	.value	0x11a
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x24
	.uleb128 0x9
	.long	.LASF24
	.byte	0x4
	.value	0x11b
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x28
	.uleb128 0x9
	.long	.LASF25
	.byte	0x4
	.value	0x11c
	.long	0xd3
	.byte	0x2
	.byte	0x23
	.uleb128 0x2c
	.uleb128 0x9
	.long	.LASF26
	.byte	0x4
	.value	0x11e
	.long	0x2d7
	.byte	0x2
	.byte	0x23
	.uleb128 0x30
	.uleb128 0x9
	.long	.LASF27
	.byte	0x4
	.value	0x120
	.long	0x2dd
	.byte	0x2
	.byte	0x23
	.uleb128 0x34
	.uleb128 0x9
	.long	.LASF28
	.byte	0x4
	.value	0x122
	.long	0x70
	.byte	0x2
	.byte	0x23
	.uleb128 0x38
	.uleb128 0x9
	.long	.LASF29
	.byte	0x4
	.value	0x126
	.long	0x70
	.byte	0x2
	.byte	0x23
	.uleb128 0x3c
	.uleb128 0x9
	.long	.LASF30
	.byte	0x4
	.value	0x128
	.long	0xad
	.byte	0x2
	.byte	0x23
	.uleb128 0x40
	.uleb128 0x9
	.long	.LASF31
	.byte	0x4
	.value	0x12c
	.long	0x2c
	.byte	0x2
	.byte	0x23
	.uleb128 0x44
	.uleb128 0x9
	.long	.LASF32
	.byte	0x4
	.value	0x12d
	.long	0x57
	.byte	0x2
	.byte	0x23
	.uleb128 0x46
	.uleb128 0x9
	.long	.LASF33
	.byte	0x4
	.value	0x12e
	.long	0x2e3
	.byte	0x2
	.byte	0x23
	.uleb128 0x47
	.uleb128 0x9
	.long	.LASF34
	.byte	0x4
	.value	0x132
	.long	0x2f3
	.byte	0x2
	.byte	0x23
	.uleb128 0x48
	.uleb128 0x9
	.long	.LASF35
	.byte	0x4
	.value	0x13b
	.long	0xbf
	.byte	0x2
	.byte	0x23
	.uleb128 0x4c
	.uleb128 0x9
	.long	.LASF36
	.byte	0x4
	.value	0x144
	.long	0xd1
	.byte	0x2
	.byte	0x23
	.uleb128 0x54
	.uleb128 0x9
	.long	.LASF37
	.byte	0x4
	.value	0x145
	.long	0xd1
	.byte	0x2
	.byte	0x23
	.uleb128 0x58
	.uleb128 0x9
	.long	.LASF38
	.byte	0x4
	.value	0x146
	.long	0xd1
	.byte	0x2
	.byte	0x23
	.uleb128 0x5c
	.uleb128 0x9
	.long	.LASF39
	.byte	0x4
	.value	0x147
	.long	0xd1
	.byte	0x2
	.byte	0x23
	.uleb128 0x60
	.uleb128 0x9
	.long	.LASF40
	.byte	0x4
	.value	0x148
	.long	0x90
	.byte	0x2
	.byte	0x23
	.uleb128 0x64
	.uleb128 0x9
	.long	.LASF41
	.byte	0x4
	.value	0x14a
	.long	0x70
	.byte	0x2
	.byte	0x23
	.uleb128 0x68
	.uleb128 0x9
	.long	.LASF42
	.byte	0x4
	.value	0x14c
	.long	0x2f9
	.byte	0x2
	.byte	0x23
	.uleb128 0x6c
	.byte	0x0
	.uleb128 0xa
	.long	.LASF43
	.byte	0x4
	.byte	0xb0
	.uleb128 0x8
	.long	0x2d7
	.long	.LASF45
	.byte	0xc
	.byte	0x4
	.byte	0xb6
	.uleb128 0xb
	.long	.LASF46
	.byte	0x4
	.byte	0xb7
	.long	0x2d7
	.byte	0x2
	.byte	0x23
	.uleb128 0x0
	.uleb128 0xb
	.long	.LASF47
	.byte	0x4
	.byte	0xb8
	.long	0x2dd
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0xb
	.long	.LASF48
	.byte	0x4
	.byte	0xbc
	.long	0x70
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x0
	.uleb128 0x7
	.byte	0x4
	.long	0x2a0
	.uleb128 0x7
	.byte	0x4
	.long	0xd9
	.uleb128 0xc
	.long	0x2f3
	.long	0x89
	.uleb128 0xd
	.long	0xca
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.byte	0x4
	.long	0x299
	.uleb128 0xc
	.long	0x309
	.long	0x89
	.uleb128 0xd
	.long	0xca
	.byte	0x27
	.byte	0x0
	.uleb128 0xe
	.long	0x340
	.byte	0x1
	.long	.LASF49
	.byte	0x1
	.byte	0xe
	.byte	0x1
	.long	0x33
	.byte	0x1
	.uleb128 0xf
	.string	"x"
	.byte	0x1
	.byte	0xd
	.long	0x33
	.uleb128 0xf
	.string	"y"
	.byte	0x1
	.byte	0xd
	.long	0x33
	.uleb128 0x10
	.string	"z"
	.byte	0x1
	.byte	0xf
	.long	0x33
	.uleb128 0x10
	.string	"a"
	.byte	0x1
	.byte	0x10
	.long	0x33
	.byte	0x0
	.uleb128 0x11
	.long	0x37a
	.long	0x309
	.long	.LFB12
	.long	.LFE12
	.long	.LLST0
	.uleb128 0x12
	.long	0x31b
	.long	.LLST1
	.uleb128 0x12
	.long	0x324
	.long	.LLST2
	.uleb128 0x13
	.long	0x32d
	.long	.LLST3
	.uleb128 0x13
	.long	0x336
	.long	.LLST4
	.byte	0x0
	.uleb128 0xe
	.long	0x3b1
	.byte	0x1
	.long	.LASF50
	.byte	0x1
	.byte	0x3c
	.byte	0x1
	.long	0x45
	.byte	0x1
	.uleb128 0xf
	.string	"x"
	.byte	0x1
	.byte	0x3b
	.long	0x45
	.uleb128 0xf
	.string	"y"
	.byte	0x1
	.byte	0x3b
	.long	0x45
	.uleb128 0x10
	.string	"z"
	.byte	0x1
	.byte	0x3d
	.long	0x45
	.uleb128 0x10
	.string	"a"
	.byte	0x1
	.byte	0x3e
	.long	0x45
	.byte	0x0
	.uleb128 0x11
	.long	0x3eb
	.long	0x37a
	.long	.LFB14
	.long	.LFE14
	.long	.LLST5
	.uleb128 0x12
	.long	0x38c
	.long	.LLST6
	.uleb128 0x12
	.long	0x395
	.long	.LLST7
	.uleb128 0x13
	.long	0x39e
	.long	.LLST8
	.uleb128 0x13
	.long	0x3a7
	.long	.LLST9
	.byte	0x0
	.uleb128 0x14
	.long	0x421
	.byte	0x1
	.long	.LASF51
	.byte	0x1
	.byte	0x86
	.byte	0x1
	.long	0x33
	.long	.LFB17
	.long	.LFE17
	.long	.LLST10
	.uleb128 0x15
	.string	"x"
	.byte	0x1
	.byte	0x85
	.long	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x15
	.string	"y"
	.byte	0x1
	.byte	0x85
	.long	0x33
	.byte	0x2
	.byte	0x91
	.sleb128 4
	.byte	0x0
	.uleb128 0x14
	.long	0x457
	.byte	0x1
	.long	.LASF52
	.byte	0x1
	.byte	0x8a
	.byte	0x1
	.long	0x33
	.long	.LFB18
	.long	.LFE18
	.long	.LLST11
	.uleb128 0x15
	.string	"x"
	.byte	0x1
	.byte	0x89
	.long	0x65
	.byte	0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x15
	.string	"y"
	.byte	0x1
	.byte	0x89
	.long	0x65
	.byte	0x2
	.byte	0x91
	.sleb128 4
	.byte	0x0
	.uleb128 0x14
	.long	0x573
	.byte	0x1
	.long	.LASF53
	.byte	0x1
	.byte	0x57
	.byte	0x1
	.long	0x77
	.long	.LFB15
	.long	.LFE15
	.long	.LLST12
	.uleb128 0x16
	.string	"x"
	.byte	0x1
	.byte	0x56
	.long	0x77
	.long	.LLST13
	.uleb128 0x16
	.string	"y"
	.byte	0x1
	.byte	0x56
	.long	0x77
	.long	.LLST14
	.uleb128 0x17
	.long	0x4c8
	.long	0x37a
	.long	.Ldebug_ranges0+0x0
	.byte	0x1
	.byte	0x63
	.uleb128 0x12
	.long	0x3cf
	.long	.LLST15
	.uleb128 0x12
	.long	0x3c6
	.long	.LLST16
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x40
	.uleb128 0x13
	.long	0x39e
	.long	.LLST17
	.uleb128 0x13
	.long	0x3a7
	.long	.LLST18
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.long	0x502
	.long	0x37a
	.long	.Ldebug_ranges0+0x80
	.byte	0x1
	.byte	0x5d
	.uleb128 0x12
	.long	0x3cf
	.long	.LLST19
	.uleb128 0x12
	.long	0x3c6
	.long	.LLST20
	.uleb128 0x18
	.long	.Ldebug_ranges0+0xb8
	.uleb128 0x13
	.long	0x39e
	.long	.LLST21
	.uleb128 0x13
	.long	0x3a7
	.long	.LLST22
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.long	0x53c
	.long	0x37a
	.long	.Ldebug_ranges0+0xf0
	.byte	0x1
	.byte	0x61
	.uleb128 0x12
	.long	0x3cf
	.long	.LLST23
	.uleb128 0x12
	.long	0x3c6
	.long	.LLST24
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x118
	.uleb128 0x13
	.long	0x39e
	.long	.LLST25
	.uleb128 0x13
	.long	0x3a7
	.long	.LLST26
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.long	0x37a
	.long	.Ldebug_ranges0+0x140
	.byte	0x1
	.byte	0x5b
	.uleb128 0x12
	.long	0x3cf
	.long	.LLST27
	.uleb128 0x12
	.long	0x3c6
	.long	.LLST28
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x168
	.uleb128 0x13
	.long	0x39e
	.long	.LLST29
	.uleb128 0x13
	.long	0x3a7
	.long	.LLST30
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.long	0x64d
	.byte	0x1
	.long	.LASF54
	.byte	0x1
	.byte	0x6c
	.byte	0x1
	.long	0x70
	.long	.LFB16
	.long	.LFE16
	.long	.LLST31
	.uleb128 0x1a
	.long	.LASF55
	.byte	0x1
	.byte	0x6b
	.long	0x70
	.uleb128 0x1a
	.long	.LASF56
	.byte	0x1
	.byte	0x6b
	.long	0x64d
	.uleb128 0x1b
	.string	"i"
	.byte	0x1
	.byte	0x6d
	.long	0x33
	.long	.LLST32
	.uleb128 0x1b
	.string	"j"
	.byte	0x1
	.byte	0x6d
	.long	0x33
	.long	.LLST33
	.uleb128 0x10
	.string	"k"
	.byte	0x1
	.byte	0x6d
	.long	0x33
	.uleb128 0x1b
	.string	"l"
	.byte	0x1
	.byte	0x6d
	.long	0x33
	.long	.LLST34
	.uleb128 0x1b
	.string	"x"
	.byte	0x1
	.byte	0x6e
	.long	0x33
	.long	.LLST35
	.uleb128 0x1b
	.string	"y"
	.byte	0x1
	.byte	0x6e
	.long	0x33
	.long	.LLST36
	.uleb128 0x17
	.long	0x622
	.long	0x309
	.long	.Ldebug_ranges0+0x190
	.byte	0x1
	.byte	0x72
	.uleb128 0x1c
	.long	0x35e
	.uleb128 0x1c
	.long	0x355
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x1b8
	.uleb128 0x13
	.long	0x32d
	.long	.LLST37
	.uleb128 0x13
	.long	0x336
	.long	.LLST38
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.long	0x309
	.long	.Ldebug_ranges0+0x1e0
	.byte	0x1
	.byte	0x79
	.uleb128 0x1c
	.long	0x35e
	.uleb128 0x1c
	.long	0x355
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x1f8
	.uleb128 0x1d
	.long	0x32d
	.uleb128 0x13
	.long	0x336
	.long	.LLST39
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.byte	0x4
	.long	0xd3
	.uleb128 0x14
	.long	0x76f
	.byte	0x1
	.long	.LASF57
	.byte	0x1
	.byte	0x2a
	.byte	0x1
	.long	0x65
	.long	.LFB13
	.long	.LFE13
	.long	.LLST40
	.uleb128 0x16
	.string	"x"
	.byte	0x1
	.byte	0x29
	.long	0x65
	.long	.LLST41
	.uleb128 0x16
	.string	"y"
	.byte	0x1
	.byte	0x29
	.long	0x65
	.long	.LLST42
	.uleb128 0x17
	.long	0x6c4
	.long	0x309
	.long	.Ldebug_ranges0+0x210
	.byte	0x1
	.byte	0x36
	.uleb128 0x12
	.long	0x35e
	.long	.LLST43
	.uleb128 0x12
	.long	0x355
	.long	.LLST44
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x250
	.uleb128 0x13
	.long	0x32d
	.long	.LLST45
	.uleb128 0x13
	.long	0x336
	.long	.LLST46
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.long	0x6fe
	.long	0x309
	.long	.Ldebug_ranges0+0x290
	.byte	0x1
	.byte	0x2e
	.uleb128 0x12
	.long	0x35e
	.long	.LLST47
	.uleb128 0x12
	.long	0x355
	.long	.LLST48
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x2a8
	.uleb128 0x13
	.long	0x32d
	.long	.LLST49
	.uleb128 0x13
	.long	0x336
	.long	.LLST50
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.long	0x738
	.long	0x309
	.long	.Ldebug_ranges0+0x2c0
	.byte	0x1
	.byte	0x34
	.uleb128 0x12
	.long	0x35e
	.long	.LLST51
	.uleb128 0x12
	.long	0x355
	.long	.LLST52
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x2e0
	.uleb128 0x13
	.long	0x32d
	.long	.LLST53
	.uleb128 0x13
	.long	0x336
	.long	.LLST54
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.long	0x309
	.long	.Ldebug_ranges0+0x300
	.byte	0x1
	.byte	0x30
	.uleb128 0x12
	.long	0x35e
	.long	.LLST55
	.uleb128 0x12
	.long	0x355
	.long	.LLST56
	.uleb128 0x18
	.long	.Ldebug_ranges0+0x318
	.uleb128 0x13
	.long	0x32d
	.long	.LLST57
	.uleb128 0x13
	.long	0x336
	.long	.LLST58
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.long	.LASF58
	.byte	0x3
	.byte	0x8e
	.long	0x2dd
	.byte	0x1
	.byte	0x1
	.uleb128 0x1e
	.long	.LASF59
	.byte	0x3
	.byte	0x8f
	.long	0x2dd
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x10
	.uleb128 0x6
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x16
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xe
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0xf
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x10
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x11
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x12
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x13
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x14
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x15
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0x16
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x17
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x1
	.uleb128 0x13
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x18
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x19
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x1a
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x1e
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.long	0x6d
	.value	0x2
	.long	.Ldebug_info0
	.long	0x78a
	.long	0x340
	.string	"__udivsi3"
	.long	0x3b1
	.string	"__udivdi3"
	.long	0x3eb
	.string	"div_u32_u32"
	.long	0x421
	.string	"div_s32_s32"
	.long	0x457
	.string	"__divdi3"
	.long	0x573
	.string	"main"
	.long	0x653
	.string	"__divsi3"
	.long	0x0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0x0
	.value	0x0
	.value	0x0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0x0
	.long	0x0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.LBB10-.Ltext0
	.long	.LBE10-.Ltext0
	.long	.LBB48-.Ltext0
	.long	.LBE48-.Ltext0
	.long	.LBB42-.Ltext0
	.long	.LBE42-.Ltext0
	.long	.LBB38-.Ltext0
	.long	.LBE38-.Ltext0
	.long	.LBB30-.Ltext0
	.long	.LBE30-.Ltext0
	.long	.LBB20-.Ltext0
	.long	.LBE20-.Ltext0
	.long	.LBB12-.Ltext0
	.long	.LBE12-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB11-.Ltext0
	.long	.LBE11-.Ltext0
	.long	.LBB49-.Ltext0
	.long	.LBE49-.Ltext0
	.long	.LBB43-.Ltext0
	.long	.LBE43-.Ltext0
	.long	.LBB39-.Ltext0
	.long	.LBE39-.Ltext0
	.long	.LBB31-.Ltext0
	.long	.LBE31-.Ltext0
	.long	.LBB21-.Ltext0
	.long	.LBE21-.Ltext0
	.long	.LBB13-.Ltext0
	.long	.LBE13-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB14-.Ltext0
	.long	.LBE14-.Ltext0
	.long	.LBB46-.Ltext0
	.long	.LBE46-.Ltext0
	.long	.LBB40-.Ltext0
	.long	.LBE40-.Ltext0
	.long	.LBB28-.Ltext0
	.long	.LBE28-.Ltext0
	.long	.LBB18-.Ltext0
	.long	.LBE18-.Ltext0
	.long	.LBB16-.Ltext0
	.long	.LBE16-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB15-.Ltext0
	.long	.LBE15-.Ltext0
	.long	.LBB47-.Ltext0
	.long	.LBE47-.Ltext0
	.long	.LBB41-.Ltext0
	.long	.LBE41-.Ltext0
	.long	.LBB29-.Ltext0
	.long	.LBE29-.Ltext0
	.long	.LBB19-.Ltext0
	.long	.LBE19-.Ltext0
	.long	.LBB17-.Ltext0
	.long	.LBE17-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB22-.Ltext0
	.long	.LBE22-.Ltext0
	.long	.LBB44-.Ltext0
	.long	.LBE44-.Ltext0
	.long	.LBB26-.Ltext0
	.long	.LBE26-.Ltext0
	.long	.LBB24-.Ltext0
	.long	.LBE24-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB23-.Ltext0
	.long	.LBE23-.Ltext0
	.long	.LBB45-.Ltext0
	.long	.LBE45-.Ltext0
	.long	.LBB27-.Ltext0
	.long	.LBE27-.Ltext0
	.long	.LBB25-.Ltext0
	.long	.LBE25-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB32-.Ltext0
	.long	.LBE32-.Ltext0
	.long	.LBB50-.Ltext0
	.long	.LBE50-.Ltext0
	.long	.LBB36-.Ltext0
	.long	.LBE36-.Ltext0
	.long	.LBB34-.Ltext0
	.long	.LBE34-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB33-.Ltext0
	.long	.LBE33-.Ltext0
	.long	.LBB51-.Ltext0
	.long	.LBE51-.Ltext0
	.long	.LBB37-.Ltext0
	.long	.LBE37-.Ltext0
	.long	.LBB35-.Ltext0
	.long	.LBE35-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB56-.Ltext0
	.long	.LBE56-.Ltext0
	.long	.LBB62-.Ltext0
	.long	.LBE62-.Ltext0
	.long	.LBB60-.Ltext0
	.long	.LBE60-.Ltext0
	.long	.LBB58-.Ltext0
	.long	.LBE58-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB57-.Ltext0
	.long	.LBE57-.Ltext0
	.long	.LBB63-.Ltext0
	.long	.LBE63-.Ltext0
	.long	.LBB61-.Ltext0
	.long	.LBE61-.Ltext0
	.long	.LBB59-.Ltext0
	.long	.LBE59-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB64-.Ltext0
	.long	.LBE64-.Ltext0
	.long	.LBB66-.Ltext0
	.long	.LBE66-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB65-.Ltext0
	.long	.LBE65-.Ltext0
	.long	.LBB67-.Ltext0
	.long	.LBE67-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB76-.Ltext0
	.long	.LBE76-.Ltext0
	.long	.LBB102-.Ltext0
	.long	.LBE102-.Ltext0
	.long	.LBB98-.Ltext0
	.long	.LBE98-.Ltext0
	.long	.LBB92-.Ltext0
	.long	.LBE92-.Ltext0
	.long	.LBB86-.Ltext0
	.long	.LBE86-.Ltext0
	.long	.LBB82-.Ltext0
	.long	.LBE82-.Ltext0
	.long	.LBB78-.Ltext0
	.long	.LBE78-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB77-.Ltext0
	.long	.LBE77-.Ltext0
	.long	.LBB103-.Ltext0
	.long	.LBE103-.Ltext0
	.long	.LBB99-.Ltext0
	.long	.LBE99-.Ltext0
	.long	.LBB93-.Ltext0
	.long	.LBE93-.Ltext0
	.long	.LBB87-.Ltext0
	.long	.LBE87-.Ltext0
	.long	.LBB83-.Ltext0
	.long	.LBE83-.Ltext0
	.long	.LBB79-.Ltext0
	.long	.LBE79-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB80-.Ltext0
	.long	.LBE80-.Ltext0
	.long	.LBB100-.Ltext0
	.long	.LBE100-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB81-.Ltext0
	.long	.LBE81-.Ltext0
	.long	.LBB101-.Ltext0
	.long	.LBE101-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB84-.Ltext0
	.long	.LBE84-.Ltext0
	.long	.LBB96-.Ltext0
	.long	.LBE96-.Ltext0
	.long	.LBB88-.Ltext0
	.long	.LBE88-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB85-.Ltext0
	.long	.LBE85-.Ltext0
	.long	.LBB97-.Ltext0
	.long	.LBE97-.Ltext0
	.long	.LBB89-.Ltext0
	.long	.LBE89-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB90-.Ltext0
	.long	.LBE90-.Ltext0
	.long	.LBB94-.Ltext0
	.long	.LBE94-.Ltext0
	.long	0x0
	.long	0x0
	.long	.LBB91-.Ltext0
	.long	.LBE91-.Ltext0
	.long	.LBB95-.Ltext0
	.long	.LBE95-.Ltext0
	.long	0x0
	.long	0x0
	.section	.debug_str,"MS",@progbits,1
.LASF49:
	.string	"__udivsi3"
.LASF11:
	.string	"__off_t"
.LASF15:
	.string	"_IO_read_ptr"
.LASF27:
	.string	"_chain"
.LASF8:
	.string	"size_t"
.LASF33:
	.string	"_shortbuf"
.LASF12:
	.string	"long int"
.LASF21:
	.string	"_IO_buf_base"
.LASF3:
	.string	"long long unsigned int"
.LASF57:
	.string	"__divsi3"
.LASF50:
	.string	"__udivdi3"
.LASF6:
	.string	"long long int"
.LASF28:
	.string	"_fileno"
.LASF16:
	.string	"_IO_read_end"
.LASF14:
	.string	"_flags"
.LASF22:
	.string	"_IO_buf_end"
.LASF31:
	.string	"_cur_column"
.LASF10:
	.string	"__quad_t"
.LASF30:
	.string	"_old_offset"
.LASF35:
	.string	"_offset"
.LASF37:
	.string	"__pad2"
.LASF39:
	.string	"__pad4"
.LASF61:
	.string	"math.c"
.LASF45:
	.string	"_IO_marker"
.LASF58:
	.string	"stdin"
.LASF2:
	.string	"unsigned int"
.LASF51:
	.string	"div_u32_u32"
.LASF9:
	.string	"long unsigned int"
.LASF19:
	.string	"_IO_write_ptr"
.LASF60:
	.string	"GNU C 4.1.2 20060928 (prerelease) (Ubuntu 4.1.1-13ubuntu5)"
.LASF47:
	.string	"_sbuf"
.LASF1:
	.string	"short unsigned int"
.LASF34:
	.string	"_lock"
.LASF29:
	.string	"_flags2"
.LASF41:
	.string	"_mode"
.LASF23:
	.string	"_IO_save_base"
.LASF20:
	.string	"_IO_write_end"
.LASF43:
	.string	"_IO_lock_t"
.LASF44:
	.string	"_IO_FILE"
.LASF48:
	.string	"_pos"
.LASF26:
	.string	"_markers"
.LASF0:
	.string	"unsigned char"
.LASF5:
	.string	"short int"
.LASF52:
	.string	"div_s32_s32"
.LASF32:
	.string	"_vtable_offset"
.LASF7:
	.string	"char"
.LASF53:
	.string	"__divdi3"
.LASF59:
	.string	"stdout"
.LASF46:
	.string	"_next"
.LASF13:
	.string	"__off64_t"
.LASF17:
	.string	"_IO_read_base"
.LASF25:
	.string	"_IO_save_end"
.LASF36:
	.string	"__pad1"
.LASF38:
	.string	"__pad3"
.LASF40:
	.string	"__pad5"
.LASF62:
	.string	"/home/berke/jam"
.LASF42:
	.string	"_unused2"
.LASF56:
	.string	"argv"
.LASF24:
	.string	"_IO_backup_base"
.LASF55:
	.string	"argc"
.LASF4:
	.string	"signed char"
.LASF54:
	.string	"main"
.LASF18:
	.string	"_IO_write_base"
	.ident	"GCC: (GNU) 4.1.2 20060928 (prerelease) (Ubuntu 4.1.1-13ubuntu5)"
	.section	.note.GNU-stack,"",@progbits
