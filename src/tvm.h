/*
 * @Description: TVM
 * @Author: taowentao
 * @Date: 2020-02-02 16:12:21
 * @LastEditors: taowentao
 * @LastEditTime: 2020-02-02 16:12:45
 */
#ifndef _H_TVM_H_
#define _H_TVM_H_
#include <iostream>

namespace TVM
{
typedef int VMINT;
enum TVM_OP_CODE : VMINT
{
    //不需要用到，用来计算指令个数的,同时可以判断是否为无效操作
    START = 0,

    //mov r0,1
    //r0=1
    MOV_RI,

    //mov r0,r1
    //r0=r1
    MOV_RR,
    MOV = MOV_RR,

    //mov [r0],2
    //ds[r0]=2
    MOV_RA_I,
    MOV_AI = MOV_RA_I,

    //mov [r0],r1
    //ds[r0]=r1
    MOV_RA_R,
    MOV_AR = MOV_RA_R,

    //mov r0,[r1]
    //r0=ds[r1]
    MOV_R_RA,

    //mov r0,[0]
    //r0=ds[0]
    MOV_R_IA,

    //mov_byte r0,[100]
    //r0=((unsigned char*)ds)[100]
    MOV_BYTE_RI,

    MOV_BYTE_RR,
    MOV_BYTE = MOV_BYTE_RR,

    //add r0,1
    //r0+=1
    ADD_RI,

    //add r0,r1
    //r0+=r1
    ADD_RR,
    ADD = ADD_RR,

    //sub r0,1
    //r0-=1
    SUB_RI,

    //sub r0,r1
    //r0-=r1
    SUB_RR,
    SUB = SUB_RR,

    //mul r0,2
    //r0*=2
    MUL_RI,

    //mul,r0,r1
    //r0*=r1
    MUL_RR,
    MUL = MUL_RR,

    //div,r3,2
    //r0=r3/2
    //r1=r3%2
    DIV_RI,

    //div,r3,r4
    //r0=r3/r4
    //r1=r3%r4
    DIV_RR,
    DIV = DIV_RR,

    OR_RI,

    OR_RR,
    OR = OR_RR,

    //xor r0,10
    //r0^=10
    XOR_RI,

    //xor r0,r1
    //r0^=r1
    XOR_RR,
    XOR = XOR_RR,

    //and r0,10
    //r0&=10
    AND_RI,

    AND_RR,
    AND = AND_RR,

    NOT,

    //左移
    //shl r0,1
    //r0<<=1
    SHL_RI,

    SHL_RR,
    SHL = SHL_RR,

    //右移
    SHR_RI,
    SHR_RR,
    SHR = SHR_RR,

    //比较字符串
    //cmpsb r1,r2
    //比较r1和r2指向的地址的r0个长度的字节是否都相等,相等zf=1,否则zf=0
    CMPSB,

    //push 1
    //--sp
    //ss[sp]=1
    PUSH_I,

    //push r0
    //--sp
    //ss[sp]=r0
    PUSH_R,
    PUSH = PUSH_R,

    //pop r0
    //r0=ss[sp]
    //++sp
    POP,

    //call 100
    //push ip+1
    //ip+=100
    CALL_I,

    //call r0
    //push ip+1
    //ip+=r0
    CALL_R,
    CALL = CALL_R,

    //call_user 调用C函数 VMINT fun(void arg*)
    //参数从r0取出，返回值放入r0
    CALL_U_I,

    CALL_U_R,
    CALL_U = CALL_U_R,

    //ret
    //pop ip
    RET,

    //jmp 100
    //ip+=100
    JMP_I,

    //jmp r0
    //ip+=r0
    JMP_R,
    JMP = JMP_R,

    //cmp r1,r2
    //r1==r2 sf=0,zf=1
    //r1<r2 sf=1,zf=0
    //r1>r2 sf=0,zf=0
    CMP_RI,

    CMP_RR,
    CMP = CMP_RR,

    //je 100
    //zf==0 ip+=100
    JE_I,

    //je r1
    //zf==1 ip+=r0
    JE_R,
    JE = JE_R,

    //jne 100
    //zf==0 ip+=100
    JNE_I,

    JNE_R,
    JNE = JNE_R,

    //ja 100
    //sf==0&&zf==0 ip+=100
    JA_I,

    JA_R,
    JA = JA_R,

    JAE_I,

    JAE_R,
    JAE = JAE_R,

    //jb 100
    //sf==1&&zf==0 ip+=100
    JB_I,

    JB_R,
    JB = JB_R,

    JBE_I,

    JBE_R,
    JBE = JBE_R,

    //nop
    NOP,

    //结束虚拟机运行
    END,
};

class TCPU;
struct TVM_OP
{
    //操作码
    VMINT op_code;
    //对应的函数
    void (TCPU::*fun)();
    //几个操作数
    int op_num;
};

//对应指令操作的数组
TVM_OP ops[END - START + 1] = {0};

enum TVM_Reg
{
    R0 = 0,
    RA = R0,
    EAX = R0,

    R1,
    RB = R1,
    EBX = R1,

    R2,
    RC = R2,
    ECX = R2,

    R3,
    RD = R3,
    EDX = R3,

    R4,
    RE = R4,
    EBP = R4,

    R5,
    RF = R5,
    ESP = R5,

    R6,
    RG = R6,
    ESI = R6,

    R7,
    RH = R7,
    EDI = R7,

    R_END,
};

struct TCPU
{
    VMINT ip = 0;
    VMINT sp = 0;
    //通用寄存器
    VMINT r[R_END - R0] = {0};
    //标志寄存器
    int sf = 0;
    int zf = 0;
    //段寄存器
    VMINT *ss = nullptr;
    VMINT *ds = nullptr;
    VMINT *cs = nullptr;

public:
    TCPU(VMINT *vm_code_addr, VMINT *data_addr = nullptr, VMINT stack_size = 2048)
    {
        //cs:ip=代码入口
        cs = vm_code_addr;
        ip = 0;
        //分配堆空间
        ss = new VMINT[stack_size];
        sp = stack_size;
        ds = data_addr;
    }
    ~TCPU()
    {
        delete[] ss;
        ss = nullptr;
    }

public:
    int Run()
    {
        for (;;)
        {
            //取操作指令,然后ip+1
            VMINT op_code = cs[ip++];
            // std::cout << "do op:" << op_code << std::endl;
            switch (op_code)
            {
            case NOP:
                break;
            case END:
                return 0;
            default:
            {
                TVM_OP op = ops[op_code];
                if (op.op_code == 0)
                {
                    std::cerr << "op code " << op_code << " not support" << std::endl;
                    return -1;
                }
                (this->*(op.fun))();
                ip += op.op_num;
                break;
            }
            }
        }
        return 0;
    }

#ifdef TVM_DEBUG
private:
    void show_reg(bool newline = true)
    {
        int rn = sizeof(r) / sizeof(VMINT);
        for (int i = 0; i < rn; ++i)
        {
            printf("R%d=%d ", i, r[i]);
        }
        if (newline)
        {
            printf("\n");
        }
    }
    void show_ip(bool newline = true)
    {
        printf("cs:ip=%d:%d", cs, ip);
        if (newline)
        {
            printf("\n");
        }
    }
    void show_flag(bool newline = true)
    {
        printf("sf=%d,zf=%d", sf, zf);
        if (newline)
        {
            printf("\n");
        }
    }
#endif

private:
    void mov_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] = b;
#ifdef TVM_DEBUG
        printf("mov R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void mov_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] = r[b];
#ifdef TVM_DEBUG
        printf("mov R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void mov_ai()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        ds[r[a]] = b;
#ifdef TVM_DEBUG
        printf("mov [R%d],%d\n", a, b);
        show_reg();
#endif
    }
    void mov_ar()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        ds[r[a]] = r[b];
#ifdef TVM_DEBUG
        printf("mov [R%d],R%d\n", a, b);
        show_reg();
#endif
    }
    void mov_r_ra()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] = ds[r[b]];
#ifdef TVM_DEBUG
        printf("mov R%d,[R%d]\n", a, b);
        show_reg();
#endif
    }
    void mov_r_ia()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] = ds[b];
#ifdef TVM_DEBUG
        printf("mov R%d,[%d]\n", a, b);
        show_reg();
#endif
    }
    void mov_byte_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] = ((unsigned char *)ds)[b];
#ifdef TVM_DEBUG
        printf("mov byte R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void mov_byte_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] = ((unsigned char *)ds)[r[b]];
#ifdef TVM_DEBUG
        printf("mov byte R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void add_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] += b;
#ifdef TVM_DEBUG
        printf("add R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void add_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] += r[b];
#ifdef TVM_DEBUG
        printf("add R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void sub_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] -= b;
#ifdef TVM_DEBUG
        printf("sub R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void sub_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] -= r[b];
#ifdef TVM_DEBUG
        printf("sub R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void mul_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] *= b;
#ifdef TVM_DEBUG
        printf("mul R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void mul_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] *= r[b];
#ifdef TVM_DEBUG
        printf("mul R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void div_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[0] = a / b;
        r[1] = a % b;
#ifdef TVM_DEBUG
        printf("div R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void div_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[0] = r[a] / r[b];
        r[1] = r[a] % r[b];
#ifdef TVM_DEBUG
        printf("div R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void shl_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] <<= b;
#ifdef TVM_DEBUG
        printf("shl R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void shl_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] <<= r[b];
#ifdef TVM_DEBUG
        printf("shl R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void shr_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] >>= b;
#ifdef TVM_DEBUG
        printf("shr R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void shr_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] >>= r[b];
#ifdef TVM_DEBUG
        printf("shr R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void xor_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] ^= b;
#ifdef TVM_DEBUG
        printf("xor R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void xor_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] ^= r[b];
#ifdef TVM_DEBUG
        printf("xor R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void or_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] |= b;
#ifdef TVM_DEBUG
        printf("or R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void or_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] |= r[b];
#ifdef TVM_DEBUG
        printf("or R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void and_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] &= b;
#ifdef TVM_DEBUG
        printf("and R%d,%d\n", a, b);
        show_reg();
#endif
    }
    void and_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] &= r[b];
#ifdef TVM_DEBUG
        printf("and R%d,R%d\n", a, b);
        show_reg();
#endif
    }
    void not_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        r[a] = ~r[a];
#ifdef TVM_DEBUG
        printf("not R%d\n", a);
        show_reg();
#endif
    }
    void cmp_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        if (r[a] == b)
        {
            sf = 0;
            zf = 1;
        }
        else if (r[a] < b)
        {
            sf = 1;
            zf = 0;
        }
        else
        {
            sf = 0;
            zf = 0;
        }
#ifdef TVM_DEBUG
        printf("cmp R%d,%d\n", a, b);
        show_reg();
        show_flag();
#endif
    }
    void cmp_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        if (r[a] == r[b])
        {
            sf = 0;
            zf = 1;
        }
        else if (r[a] < r[b])
        {
            sf = 1;
            zf = 0;
        }
        else
        {
            sf = 0;
            zf = 0;
        }
#ifdef TVM_DEBUG
        printf("cmp R%d,R%d\n", a, b);
        show_reg();
        show_flag();
#endif
    }
    void cmpsb()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        const char *pca = reinterpret_cast<char *>(r[a]);
        const char *pcb = reinterpret_cast<char *>(r[b]);
        int len = r[0];
        bool f = true;
        for (int i = 0; i < len; ++i)
        {
            if (pca[i] != pcb[i])
            {
                f = false;
                break;
            }
        }
        zf = f ? 1 : 0;
    }
    void jmp_i()
    {
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        ip += a;
#ifdef TVM_DEBUG
        printf("jmp %d\n", a);
        show_reg();
        show_ip();
#endif
    }
    void jmp_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        ip += r[a];
#ifdef TVM_DEBUG
        printf("jmp R%d\n", a);
        show_reg();
        show_ip();
#endif
    }
    void je_i()
    {
        if (zf == 1)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
#ifdef TVM_DEBUG
            printf("je %d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void je_r()
    {
        if (zf == 1)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
#ifdef TVM_DEBUG
            printf("je R%d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jne_i()
    {
        if (zf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
#ifdef TVM_DEBUG
            printf("jne %d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jne_r()
    {
        if (zf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
#ifdef TVM_DEBUG
            printf("jne R%d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void ja_i()
    {
        if (sf == 0 && zf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
#ifdef TVM_DEBUG
            printf("ja %d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void ja_r()
    {
        if (sf == 0 && zf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
#ifdef TVM_DEBUG
            printf("ja R%d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jae_i()
    {
        if (zf == 1 || sf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
#ifdef TVM_DEBUG
            printf("jae %d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jae_r()
    {
        if (zf == 1 || sf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
#ifdef TVM_DEBUG
            printf("jae R%d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jb_i()
    {
        if (sf == 1 && zf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
#ifdef TVM_DEBUG
            printf("jb %d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jb_r()
    {
        if (sf == 1 && zf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
#ifdef TVM_DEBUG
            printf("jb R%d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jbe_i()
    {
        if (zf == 1 || sf == 1)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
#ifdef TVM_DEBUG
            printf("jbe %d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void jbe_r()
    {
        if (zf == 1 || sf == 1)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
#ifdef TVM_DEBUG
            printf("jbe R%d\n", a);
            show_reg();
            show_ip();
#endif
        }
    }
    void push_i()
    {
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = a;
#ifdef TVM_DEBUG
        printf("push %d\n", a);
        show_reg();
#endif
    }
    void push_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = r[a];
#ifdef TVM_DEBUG
        printf("push R%d\n", a);
        show_reg();
#endif
    }
    void pop()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        r[a] = ss[sp];
        ++sp;
#ifdef TVM_DEBUG
        printf("pop R%d\n", a);
        show_reg();
#endif
    }
    void call_i()
    {
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = ip + 1;
        ip += a;
#ifdef TVM_DEBUG
        printf("call %d\n", a);
        show_ip();
#endif
    }
    void call_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = ip + 1;
        ip += r[a];
#ifdef TVM_DEBUG
        printf("call R%d\n", a);
        show_ip();
#endif
    }
    void ret()
    {
        ip = ss[sp];
        ++sp;
#ifdef TVM_DEBUG
        printf("ret\n");
        show_ip();
#endif
    }
    void call_user_i()
    {
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        VMINT (*fp)
        (void *) = reinterpret_cast<VMINT (*)(void *)>(a);
        r[0] = fp(reinterpret_cast<void *>(r[0]));
#ifdef TVM_DEBUG
        printf("call user %d\n", a);
#endif
    }
    void call_user_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        VMINT (*fp)
        (void *) = reinterpret_cast<VMINT (*)(void *)>(r[a]);
        r[0] = fp(reinterpret_cast<void *>(r[0]));
#ifdef TVM_DEBUG
        printf("call user R%d\n", a);
#endif
    }

public:
    friend void Init();
};
void Init()
{
    ops[MOV_RI] = {MOV_RI, &TCPU::mov_ri, 2};
    ops[MOV_RR] = {MOV_RR, &TCPU::mov_rr, 2};
    ops[MOV_AI] = {MOV_AI, &TCPU::mov_ai, 2};
    ops[MOV_AR] = {MOV_AR, &TCPU::mov_ar, 2};
    ops[MOV_R_RA] = {MOV_R_RA, &TCPU::mov_r_ra, 2};
    ops[MOV_R_IA] = {MOV_R_IA, &TCPU::mov_r_ia, 2};
    ops[MOV_BYTE_RI] = {MOV_BYTE_RI, &TCPU::mov_byte_ri, 2};
    ops[MOV_BYTE_RR] = {MOV_BYTE_RR, &TCPU::mov_byte_rr, 2};
    ops[ADD_RI] = {ADD_RI, &TCPU::add_ri, 2};
    ops[ADD_RR] = {ADD_RR, &TCPU::add_rr, 2};
    ops[SUB_RI] = {SUB_RI, &TCPU::sub_ri, 2};
    ops[SUB_RR] = {SUB_RR, &TCPU::sub_rr, 2};
    ops[MUL_RI] = {MUL_RI, &TCPU::mul_ri, 2};
    ops[MUL_RR] = {MUL_RR, &TCPU::mul_rr, 2};
    ops[DIV_RI] = {DIV_RI, &TCPU::div_ri, 2};
    ops[DIV_RR] = {DIV_RR, &TCPU::div_rr, 2};
    ops[AND_RI] = {AND_RI, &TCPU::and_ri, 2};
    ops[AND_RR] = {AND_RR, &TCPU::and_rr, 2};
    ops[OR_RI] = {OR_RI, &TCPU::or_ri, 2};
    ops[OR_RR] = {OR_RR, &TCPU::or_rr, 2};
    ops[XOR_RI] = {XOR_RI, &TCPU::xor_ri, 2};
    ops[XOR_RR] = {XOR_RR, &TCPU::xor_rr, 2};
    ops[NOT] = {NOT, &TCPU::not_r, 1};
    ops[SHL_RI] = {SHL_RI, &TCPU::shl_ri, 2};
    ops[SHL_RR] = {SHL_RR, &TCPU::shl_rr, 2};
    ops[SHR_RI] = {SHR_RI, &TCPU::shr_ri, 2};
    ops[SHR_RR] = {SHR_RR, &TCPU::shr_rr, 2};

    ops[CMPSB] = {CMPSB, &TCPU::cmpsb, 2};

    ops[PUSH_I] = {PUSH_I, &TCPU::push_i, 1};
    ops[PUSH_R] = {PUSH_R, &TCPU::push_r, 1};
    ops[POP] = {POP, &TCPU::pop, 1};

    ops[CALL_I] = {CALL_I, &TCPU::call_i, 1};
    ops[CALL_R] = {CALL_R, &TCPU::call_r, 1};
    ops[CALL_U_R] = {CALL_U_R, &TCPU::call_user_r, 1};
    ops[CALL_U_I] = {CALL_U_I, &TCPU::call_user_i, 1};
    ops[RET] = {RET, &TCPU::ret, 0};

    ops[JMP_I] = {JMP_I, &TCPU::jmp_i, 1};
    ops[JMP_R] = {JMP_R, &TCPU::jmp_r, 1};

    ops[CMP_RI] = {CMP_RI, &TCPU::cmp_ri, 2};
    ops[CMP_RR] = {CMP_RR, &TCPU::cmp_rr, 2};

    ops[JE_I] = {JE_I, &TCPU::je_i, 1};
    ops[JE_R] = {JE_R, &TCPU::je_r, 1};
    ops[JNE_I] = {JNE_I, &TCPU::jne_i, 1};
    ops[JNE_R] = {JNE_R, &TCPU::jne_r, 1};
    ops[JA_I] = {JA_I, &TCPU::ja_i, 1};
    ops[JA_R] = {JA_R, &TCPU::ja_r, 1};
    ops[JAE_I] = {JAE_I, &TCPU::jae_i, 1};
    ops[JAE_R] = {JAE_R, &TCPU::jae_r, 1};
    ops[JB_I] = {JB_I, &TCPU::jb_i, 1};
    ops[JB_R] = {JB_R, &TCPU::jb_r, 1};
    ops[JBE_I] = {JBE_I, &TCPU::jbe_i, 1};
    ops[JBE_R] = {JBE_R, &TCPU::jbe_r, 1};
}
} // namespace TVM

#endif