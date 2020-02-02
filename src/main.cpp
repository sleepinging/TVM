/*
 * @Description: TVM
 * @Author: taowentao
 * @Date: 2020-02-01 12:05:25
 * @LastEditors  : taowentao
 * @LastEditTime : 2020-02-02 14:19:23
 */

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
};

struct TCPU
{
    VMINT ip = 0;
    VMINT sp = 0;
    //通用寄存器
    VMINT r[4] = {0};
    //标志寄存器
    int sf = 0;
    int zf = 0;
    //段寄存器
    VMINT *ss = nullptr;
    VMINT *ds = nullptr;
    VMINT *cs = nullptr;

public:
    TCPU(VMINT *vm_code_addr,VMINT* data_addr=nullptr, VMINT stack_size = 2048)
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
    int Run(){
        for (;;){
            //取操作指令,然后ip+1
            VMINT op_code = cs[ip++];
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
private:
    void mov_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip+1];
        r[a] = b;
    }
    void mov_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] = r[b];
    }
    void mov_ai(){
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        ds[r[a]] = b;
    }
    void mov_ar()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        ds[r[a]] = r[b];
    }
    void mov_r_ra(){
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] = ds[r[b]];
    }
    void mov_r_ia(){
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] = ds[b];
    }
    void mov_byte_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] = ((unsigned char *)ds)[b];
    }
    void mov_byte_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] = ((unsigned char *)ds)[r[b]];
    }
    void add_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] += b;
    }
    void add_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] += r[b];
    }
    void sub_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] -= b;
    }
    void sub_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] -= r[b];
    }
    void mul_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] *= b;
    }
    void mul_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] *= r[b];
    }
    void div_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[0] = a/b;
        r[1] = a%b;
    }
    void div_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[0] = r[a]/r[b];
        r[1] = r[a]%r[b];
    }
    void shl_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] <<= b;
    }
    void shl_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] <<= r[b];
    }
    void shr_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] >>= b;
    }
    void shr_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] >>= r[b];
    }
    void xor_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] ^= b;
    }
    void xor_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] ^= r[b];
    }
    void or_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] |= b;
    }
    void or_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] |= r[b];
    }
    void and_ri()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(立即数)
        VMINT b = cs[ip + 1];
        r[a] &= b;
    }
    void and_rr()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        //第2个操作数(寄存器)
        VMINT b = cs[ip + 1];
        r[a] &= r[b];
    }
    void not_r(){
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        r[a] = ~r[a];
    }
    void cmp_ri(){
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
    }
    void cmpsb(){
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
            if(pca[i]!=pcb[i]){
                f = false;
                break;
            }
        }
        zf = f ? 1 : 0;
    }
    void jmp_i(){
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        ip += a;
    }
    void jmp_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        ip += r[a];
    }
    void je_i()
    {
        if (zf == 1)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
        }
    }
    void je_r()
    {
        if (zf == 1)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
        }
    }
    void jne_i()
    {
        if (zf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
        }
    }
    void jne_r()
    {
        if (zf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
        }
    }
    void ja_i()
    {
        if (sf == 0 && zf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
        }
    }
    void ja_r()
    {
        if (sf == 0 && zf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
        }
    }
    void jae_i()
    {
        if (zf==1 || sf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
        }
    }
    void jae_r()
    {
        if (zf == 1 || sf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
        }
    }
    void jb_i()
    {
        if (sf == 1 && zf == 0)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
        }
    }
    void jb_r()
    {
        if (sf == 1 && zf == 0)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
        }
    }
    void jbe_i()
    {
        if (zf == 1 || sf == 1)
        {
            //第1个操作数(立即数)
            VMINT a = cs[ip];
            ip += a;
        }
    }
    void jbe_r()
    {
        if (zf == 1 || sf == 1)
        {
            //第1个操作数(寄存器)
            VMINT a = cs[ip];
            ip += r[a];
        }
    }
    void push_i()
    {
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = a;
    }
    void push_r(){
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = r[a];
    }
    void pop(){
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        r[a] = ss[sp];
        ++sp;
    }
    void call_i(){
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = ip + 1;
        ip += a;
    }
    void call_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        --sp;
        ss[sp] = ip + 1;
        ip += r[a];
    }
    void ret(){
        ip = ss[sp];
        ++sp;
    }
    void call_user_i()
    {
        //第1个操作数(立即数)
        VMINT a = cs[ip];
        VMINT (*fp)(void *) = reinterpret_cast<VMINT (*)(void *)>(a);
        r[0] = fp(reinterpret_cast<void *>(r[0]));
    }
    void call_user_r()
    {
        //第1个操作数(寄存器)
        VMINT a = cs[ip];
        VMINT (*fp)(void *) = reinterpret_cast<VMINT (*)(void *)>(r[a]);
        r[0] = fp(reinterpret_cast<void *>(r[0]));
    }

public:
    friend void Init();
};
void Init(){
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

#include <string>

using namespace std;

TVM::VMINT user_fun_sub(void* arg){
    struct args{TVM::VMINT a;TVM::VMINT b;};
    args *pargs = (args *)arg;
    return pargs->a - pargs->b;
}

TVM::VMINT addr2int(const void* addr){
    const TVM::VMINT *paddr = (const TVM::VMINT *)addr;
    return *(TVM::VMINT *)&paddr;
}

void test_vm(){
    using namespace TVM;
    VMINT a = 101, b = 203;
    char stra[] = "hello1";
    char strb[] = "hello2";
    VMINT TVM_DATA[100] = {0, 11, 22};

    VMINT TVM_CODE[] = {
        //子函数add
        POP,R3,//保存IP
        POP,R0,
        POP,R1,
        ADD,R0,R1,//r0+=r1
        PUSH,R3,//恢复IP
        RET,

        MOV_RI,R1,a,//r1=a
        MOV_RI,R2,b,//r2=b
        PUSH,R1,
        PUSH,R2,
        CALL_I,-24,//到这里结束往前20单位
        MOV_RI,R1,0,//r1=0
        MOV_AR,R1,R0,//ds[r1]=r0

        //测试mov
        MOV_R_IA,R1,1,
        MOV_RI,R0,2,
        MOV_R_RA,R2,R0,

        //测试cmpsb
        MOV_RI,R1,addr2int(stra),
        MOV_RI,R2,addr2int(strb),
        MOV_RI,R0,sizeof(stra)-1-1,
        CMPSB,R1,R2,

        //测试与 或 非 异或
        MOV_RI,R0,10,
        MOV_RI,R1,200,
        NOT,R0,

        //测试左右移
        MOV_RI,R0,10,
        MOV_RI,R1,2,
        SHL_RI,R0,1,
        SHR_RR,R0,R1,

        //测试用户函数user_fun_sub调用,20-2
        MOV_RI,R1,20,
        MOV_RI,R2,2,
        //data[10:]里面放参数
        MOV_RI,R0,10,
        MOV_RA_R,R0,R1,//data[10]=r1
        MOV_RI,R0,11,
        MOV_RA_R,R0,R2,//data[11]=r2
        MOV_RI,R0,addr2int(TVM_DATA)+(VMINT)sizeof(VMINT)*10,
        CALL_U_I,addr2int((void*)user_fun_sub),

        //测试mov byte
        MOV_RI,R0,8,
        MOV_BYTE_RR,R2,R0,

        // //测试jmp等
        // MOV_RI,R0,0,//r0=0
        // MOV_RI,R1,1,//r1=1
        // MOV_RI,R2,3,//r2=3
        // CMP_RR,R0,R1,//r0 ? r1
        // JAE_R,R2,
        // ADD_RI,R0,1,
        // ADD_RI,R0,1,//是否会跳到这
        // ADD_RI,R0,1,
        END,
    };
    TCPU tc(TVM_CODE+12,TVM_DATA);
    tc.Run();
    cout<<"a+b=" << TVM_DATA[0] << endl;
}

bool test_vm_pwd(const string& name,const string&pwd){
    //密码=用户名
    using namespace TVM;
    bool r = true;
    const char *cname = name.c_str();
    const char *cpwd = pwd.c_str();
    VMINT TVM_DATA[100] = {0};
    // //原始代码
    // if (name.length() != pwd.length()){
    //     TVM_DATA[0] = 0;
    // }else{
    //     if (memcmp(cname, cpwd, pwd.c_str())==0)
    //     {
    //         TVM_DATA[0] = 1;
    //     }
    // }
    VMINT TVM_CODE[] = {
        //mov r0,name.length()
        MOV_RI,R0,(VMINT)name.length(),
        //mov r1,pwd.length()
        MOV_RI,R1,(VMINT)pwd.length(),
        //cmp r0,r1
        CMP,R0,R1,
        //je 1
        JE_I,1,
        //END
        END,
        //mov r1,cname-ds //r1=(VMINT*)cname
        MOV_RI,R1,addr2int(cname),
        //mov r2,cpwd-ds //r2=(VMINT*)cpwd
        MOV_RI,R2,addr2int(cpwd),
        //mov r0,pwd.length()
        MOV_RI,R0,(VMINT)pwd.length(),
        //cmpsb r1,r2
        CMPSB,R1,R2,
        //jne 6 //不相等就跳过赋值1
        JNE_I,6,
        //mov r0,0
        MOV_RI,R0,0,
        //mov [r0],1
        MOV_RA_I,R0,1,
        END,
    };
    TCPU tc(TVM_CODE, TVM_DATA);
    tc.Run();
    r = TVM_DATA[0] == 1;
    return r;
}

int main(int argc, char const *argv[])
{
    const char *str1 = "123";
    const char *str2 = "234";
    for(int i=0;i<3;++i){
        if(str1[i]+1!=str2[i]){
            return 0;
        }
    }

    TVM::Init();
    test_vm();
    bool f=test_vm_pwd("123","234");
    cout << f << endl;
    f = test_vm_pwd("123", "123");
    cout << f << endl;
    return 0;
}
