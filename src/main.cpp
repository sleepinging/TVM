/*
 * @Description: TVM
 * @Author: taowentao
 * @Date: 2020-02-01 12:05:25
 * @LastEditors  : taowentao
 * @LastEditTime : 2020-02-02 16:16:23
 */

#include <string>

// #define TVM_DEBUG

#include "tvm.h"

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

//VM调用这个函数
int test_vm_login_su(void*){
    cout << "login success" << endl;
    return 0;
}

//VM中验证用户名和密码
bool test_vm_login(const string& name,const string&pwd){
    //密码=用户名
    using namespace TVM;
    bool r = false;
    const char *cname = name.c_str();
    const char *cpwd = pwd.c_str();
    VMINT TVM_DATA[100] = {0};
    // //原始代码
    // if (name.length() == pwd.length()){
    //     for (int i = 0; i < name.length();++i)
    //     {
    //         if (cname[i] + cpwd[i] != 219)
    //         {
    //             END;
    //         }
    //     }
    //     TVM_DATA[0] = 1;
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
        //mov r1,cname-ds //r1=(VMINT*)cname-ds
        MOV_RI,R1,addr2int(cname)-addr2int(TVM_DATA),
        //mov r2,cpwd-ds //r2=(VMINT*)cpwd-ds
        MOV_RI,R2,addr2int(cpwd)-addr2int(TVM_DATA),
        //mov r0,0
        MOV_RI,R0,0,
        //cmp r0,pwd.length()
        CMP_RI,R0,(VMINT)pwd.length(),
        //jb 1 //r0<len
        JB_I,9,
        MOV_RI,R0,0,
        MOV_RA_I,R0,1,
        CALL_U_I,addr2int((void*)test_vm_login_su),
        END,
        MOV_RR,R3,R1,
        ADD_RR,R3,R0,//r3=cname-ds+i
        MOV_BYTE_RR,R3,R3,//r3=cname[i]
        MOV_RR,R4,R2,
        ADD_RR,R4,R0,//r4=cpwd-ds+i
        MOV_BYTE_RR,R4,R4,//r4=cpwd[i]
        ADD_RR,R3,R4,
        CMP_RI,R3,219,//r3+r4==219?
        JE_I,1,
        END,
        //r3+r4==219
        ADD_RI,R0,1,
        JMP_I,-46,
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
    // test_vm();
    if(argc==3){
        if(!test_vm_login(argv[1],argv[2])){
            cout << "login fail" << endl;
        }
    }
    // cout << test_vm_login("abc", "abc") << endl;
    // cout << test_vm_login("abcdef", "zyxwvu") << endl;
    return 0;
}
