/*
 * @Description: Do not edit
 * @Author: taowentao
 * @Date: 2020-02-01 12:05:25
 * @LastEditors  : taowentao
 * @LastEditTime : 2020-02-02 12:23:24
 */

#include <iostream>

namespace TVM
{
enum TVM_OP_CODE
{
    //不需要用到，用来计算指令个数的,同时可以判断是否为无效操作
    START=0,

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
    OR=OR_RR,

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
    int op_code;
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
            int op_code = cs[ip++];
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
    void mov_ai(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        ds[r[a]] = b;
    }
    void mov_ar()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        ds[r[a]] = r[b];
    }
    void mov_r_ra(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] = ds[r[b]];
    }
    void mov_r_ia(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] = ds[b];
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
    void xor_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] ^= b;
    }
    void xor_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] ^= r[b];
    }
    void or_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] |= b;
    }
    void or_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] |= r[b];
    }
    void and_ri()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(立即数)
        int b = cs[ip + 1];
        r[a] &= b;
    }
    void and_rr()
    {
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
        r[a] &= r[b];
    }
    void not_r(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        r[a] = ~r[a];
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
        //第2个操作数(寄存器)
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
    void cmpsb(){
        //第1个操作数(寄存器)
        int a = cs[ip];
        //第2个操作数(寄存器)
        int b = cs[ip + 1];
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
    void jae_i()
    {
        if (zf==1 || sf == 0)
        {
            //第1个操作数(立即数)
            int a = cs[ip];
            ip += a;
        }
    }
    void jae_r()
    {
        if (zf == 1 || sf == 0)
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
    void jbe_i()
    {
        if (zf == 1 || sf == 1)
        {
            //第1个操作数(立即数)
            int a = cs[ip];
            ip += a;
        }
    }
    void jbe_r()
    {
        if (zf == 1 || sf == 1)
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

    ops[CMPSB] = {CMPSB, &TCPU::cmpsb, 2};

    ops[PUSH_I] = {PUSH_I, &TCPU::push_i, 1};
    ops[PUSH_R] = {PUSH_R, &TCPU::push_r, 1};
    ops[POP] = {POP, &TCPU::pop, 1};

    ops[CALL_I] = {CALL_I, &TCPU::call_i, 1};
    ops[CALL_R] = {CALL_R, &TCPU::call_r, 1};
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

void test_vm(){
    using namespace TVM;
    int a = 101, b = 203;
    char stra[] = "hello1";
    char strb[] = "hello2";
    char *pa = stra;
    char *pb = strb;
    int pstra = *(int *)&pa;
    int pstrb = *(int *)&pb;
    int TVM_DATA[100] = {0, 11, 22};
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

        //测试mov
        MOV_R_IA,R1,1,
        MOV_RI,R0,2,
        MOV_R_RA,R2,R0,

        //测试cmpsb
        MOV_RI,R1,pstra,
        MOV_RI,R2,pstrb,
        MOV_RI,R0,sizeof(stra)-1-1,
        CMPSB,R1,R2,

        //测试与 或 非 异或
        MOV_RI,R0,10,
        MOV_RI,R1,200,
        NOT,R0,

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
    int pname = *(int *)&cname;
    const char *cpwd = pwd.c_str();
    int ppwd = *(int *)&pwd;
    int TVM_DATA[100] = {0};
    // //原始代码
    // if (name.length() != pwd.length()){
    //     TVM_DATA[0] = 0;
    // }else{
    //     if (memcmp(cname, cpwd, pwd.c_str())==0)
    //     {
    //         TVM_DATA[0] = 1;
    //     }
    // }
    int TVM_CODE[] = {
        //mov r0,name.length()
        MOV_RI,R0,(int)name.length(),
        //mov r1,pwd.length()
        MOV_RI,R1,(int)pwd.length(),
        //cmp r0,r1
        CMP,R0,R1,
        //je 1
        JE_I,1,
        //END
        END,
        //mov r1,ds[cname-ds] //r1=*(int*)cname
        MOV_RI,R1,pname,
        //mov r2,ds[cpwd-ds] //r2=*(int*)cname
        MOV_RI,R2,ppwd,
        //mov r0,pwd.length()
        MOV_RI,R0,(int)pwd.length(),
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
    TVM::Init();
    test_vm();
    bool f=test_vm_pwd("123","234");
    cout << f << endl;
    f = test_vm_pwd("123", "123");
    cout << f << endl;
    return 0;
}
