/*
 * @Description: Do not edit
 * @Author: taowentao
 * @Date: 2020-02-01 12:05:25
 * @LastEditors  : taowentao
 * @LastEditTime : 2020-02-01 18:02:23
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

    XOR,
    AND,
    //左移
    SHL,
    //右移
    SHR,

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
    //push ip
    //ip+=100
    CALL_I,

    //call r0
    //push ip
    //ip+=r0
    CALL_R,
    CALL = CALL_R,

    //ret
    //pop ip
    RET,

    JMP,
    CMP,
    JE,
    JNE,
    JA,
    JNA,
    JB,
    JNB,

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
            case END:
                return 0;
            default:
                std::cerr << "op code " << op << " err" << std::endl;
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
        ss[sp] = ip;
        ip += a;
    }
    void call_r()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        --sp;
        ss[sp] = ip;
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
    int TVM_DATA[100] = {0};
    using namespace TVM;
    int TVM_CODE[] = {
        MOV_RI,R1,10,//r1=10
        ADD_RI,R1,22,//r1+=22
        PUSH_R,R1,//push r1
        POP,R2,//pop r2
        MOV_RI,R0,0,//r0=0
        MOV_AR,R0,R2,//ds[r0]=r2
        END,
    };
    TCPU tc(TVM_CODE,TVM_DATA);
    tc.Run();
    return 0;
}
