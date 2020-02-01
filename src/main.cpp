/*
 * @Description: Do not edit
 * @Author: taowentao
 * @Date: 2020-02-01 12:05:25
 * @LastEditors  : taowentao
 * @LastEditTime : 2020-02-01 19:39:52
 */

#include <iostream>

namespace TVM
{
enum TVM_OP
{
    //mov r0,1
    //r0=1
    MOV_RI = 0,

    //mov r0,r1
    //r0=r1
    MOV_RR,
    MOV = MOV_RR,

    //mov [r0],2
    //*r0=2
    MOV_AI,

    //mov [r0],r1
    //*r0=r1
    MOV_AR,

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

    //xor r0,10
    //r0|=10
    XOR_RI,

    //xor r0,r1
    //r0|=r1
    XOR_RR,
    XOR = XOR_RR,

    //and r0,10
    //r0&=10
    AND_RI,

    AND_RR,
    AND = AND_RR,

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

    // JNA,

    //jb 100
    //sf==1&&zf==0 ip+=100
    JB_I,

    JB_R,
    JB = JB_R,
    // JNB,

    //nop
    NOP,

    //结束虚拟机运行
    END,
};
enum TVM_Reg
{
    R0=0,
    R1,
    R2,
    R3,
};

struct TCPU
{
    int ip = 0;
    int sp = 0;
    //通用寄存器
    int r[4] = {0};
    //标志寄存器
    int sf = 0;
    int zf = 0;
    //段寄存器
    int *ss = nullptr;
    int *ds = nullptr;
    int *cs = nullptr;

public:
    TCPU(int *vm_code_addr,int* data_addr=nullptr, int stack_size = 2048)
    {
        //cs:ip=代码入口
        cs = vm_code_addr;
        ip = 0;
        //分配堆空间
        ss = new int[stack_size];
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
            int op = cs[ip++];
            switch (op)
            {
            case NOP:
                break;
            case MOV_RI:
                mov_ri();
                ip += 2;
                break;
            case MOV_RR:
                mov_rr();
                ip += 2;
                break;
            case MOV_AR:
                mov_ar();
                ip += 2;
                break;
            case ADD_RI:
                add_ri();
                ip += 2;
                break;
            case ADD_RR:
                add_rr();
                ip += 2;
                break;
            case SUB_RI:
                sub_ri();
                ip += 2;
                break;
            case SUB_RR:
                sub_rr();
                ip += 2;
                break;
            case MUL_RI:
                mul_ri();
                ip += 2;
                break;
            case MUL_RR:
                mul_rr();
                ip += 2;
                break;
            case DIV_RI:
                div_ri();
                ip += 2;
                break;
            case DIV_RR:
                div_rr();
                ip += 2;
                break;
            case CMP_RI:
                cmp_ri();
                ip += 2;
                break;
            case CMP_RR:
                cmp_rr();
                ip += 2;
                break;
            case JMP_I:
                jmp_i();
                ip += 1;
                break;
            case JMP_R:
                jmp_r();
                ip += 1;
                break;
            case JE_I:
                je_i();
                ip += 1;
                break;
            case JE_R:
                je_r();
                ip += 1;
                break;
            case JNE_I:
                jne_i();
                ip += 1;
                break;
            case JNE_R:
                jne_r();
                ip += 1;
                break;
            case JA_I:
                ja_i();
                ip += 1;
                break;
            case JA_R:
                ja_r();
                ip += 1;
                break;
            case JB_I:
                jb_i();
                ip += 1;
                break;
            case JB_R:
                jb_r();
                ip += 1;
                break;
            case PUSH_I:
                push_i();
                ip += 1;
                break;
            case PUSH_R:
                push_r();
                ip += 1;
                break;
            case POP:
                pop();
                ip += 1;
                break;
            case CALL_I:
                call_i();
                ip += 1;
                break;
            case CALL_R:
                call_r();
                ip += 1;
                break;
            case RET:
                ret();
                break;
            case END:
                return 0;
            default:
                std::cerr << "op code " << op << " not find" << std::endl;
                return -1;
            }
        }
        return 0;
    }
private:
    void mov_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip+1];
        r[a] = b;
    }
    void mov_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] = r[b];
    }
    void mov_ar()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        ds[r[a]] = r[b];
    }
    void add_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] += b;
    }
    void add_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] += r[b];
    }
    void sub_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] -= b;
    }
    void sub_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] -= r[b];
    }
    void mul_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] *= b;
    }
    void mul_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] *= r[b];
    }
    void div_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[0] = a/b;
        r[1] = a%b;
    }
    void div_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[0] = r[a]/r[b];
        r[1] = r[a]%r[b];
    }
    void cmp_ri(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
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
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
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
    void jmp_i(){
        //第1个操作数(立即数)
        int a = cs[ip];
        ip += a;
    }
    void jmp_r()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        ip += r[a];
    }
    void je_i()
    {
        if (zf == 1)
        {
            //第1个操作数(立即数)
            int a = cs[ip];
            ip += a;
        }
    }
    void je_r()
    {
        if (zf == 1)
        {
            //第1个操作数(寄存器)
            int a = cs[ip];
            ip += r[a];
        }
    }
    void jne_i()
    {
        if (zf == 0)
        {
            //第1个操作数(立即数)
            int a = cs[ip];
            ip += a;
        }
    }
    void jne_r()
    {
        if (zf == 0)
        {
            //第1个操作数(寄存器)
            int a = cs[ip];
            ip += r[a];
        }
    }
    void ja_i()
    {
        if (sf == 0 && zf == 0)
        {
            //第1个操作数(立即数)
            int a = cs[ip];
            ip += a;
        }
    }
    void ja_r()
    {
        if (sf == 0 && zf == 0)
        {
            //第1个操作数(寄存器)
            int a = cs[ip];
            ip += r[a];
        }
    }
    void jb_i()
    {
        if (sf == 1 && zf == 0)
        {
            //第1个操作数(立即数)
            int a = cs[ip];
            ip += a;
        }
    }
    void jb_r()
    {
        if (sf == 1 && zf == 0)
        {
            //第1个操作数(寄存器)
            int a = cs[ip];
            ip += r[a];
        }
    }
    void push_i()
    {
        //第1个操作数(立即数)
        int a = cs[ip];
        --sp;
        ss[sp] = a;
    }
    void push_r(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        --sp;
        ss[sp] = r[a];
    }
    void pop(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        r[a] = ss[sp];
        ++sp;
    }
    void call_i(){
        //第1个操作数(立即数)
        int a = cs[ip];
        --sp;
        ss[sp] = ip + 1;
        ip += a;
    }
    void call_r()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        --sp;
        ss[sp] = ip + 1;
        ip += r[a];
    }
    void ret(){
        ip = ss[sp];
        ++sp;
    }
};
} // namespace TVM

int main(int argc, char const *argv[])
{
    using namespace TVM;
    using namespace std;
    int a = 101, b = 203;
    int TVM_DATA[100] = {0};
    int TVM_CODE[] = {
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

        //测试jmp等
        MOV_RI,R0,0,//r0=0
        MOV_RI,R1,1,//r1=0
        MOV_RI,R2,3,//r2=3
        CMP_RR,R0,R1,//r0 ? r1
        JB_R,R2,
        ADD_RI,R0,1,
        ADD_RI,R0,1,//是否会跳到这
        ADD_RI,R0,1,
        END,
    };
    TCPU tc(TVM_CODE+12,TVM_DATA);
    tc.Run();
    cout<<"a+b=" << TVM_DATA[0] << endl;
    return 0;
}
