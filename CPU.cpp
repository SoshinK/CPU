#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "Stack.h"
#include "StackRealization.cpp"

enum CONSTANTS
    {
    DEFAULTSIZE = 256,
    MAX_Q_ITERATIONS = 9999999,
    };
 
enum CPU_ERR
    {
    CPU_DEFAULT = 0,
    CPU_STACK = 2,
    CPU_ASMNOTEXIST = 4,
    CPU_PROGRAMNOTEXIST = 8,
    CPU_IP = 16
    //....
    };

enum ASMCMDS
    {
         CPU_default,
    #define ASMCMDS
    #define COMMAND(cmd_name, q_args, n_args, l_args)    \
         CPU_##cmd_name,
    #include "ASMCMDS.txt"
    #undef COMMAND
    #undef ASMCMDS
    };


//put in separate file
#define CPUERR_NOFILE "Cant read instruction file"
#define CPUERR_BADREG "Register doesnt exist(Note: you arent allowed to access ip and flag)"
#define CPUERR_UNKNOWNCMD "Unknown instruction"
#define CPUERR_ENDLESSPRC "Process seems endless. Stoping processor"

enum ASMREGS
    {
    #define ASMREGS
    #define REGISTER(reg_name, num) \
        CPUREG_##reg_name = num,
    #include "ASMREGS.txt"
    #undef REGISTER
    #undef ASMREGS
    };

#define IS_CORRECT \
    if((ok() & CPU_ASMNOTEXIST) || (ok() & CPU_PROGRAMNOTEXIST) || (ok() & CPU_IP) || (ok() & CPU_STACK)) \
        { \
        dump(); \
        assert(0); \
        }



#define Q_REG 4

class CPU
    {
    public:
        CPU(const char* filename);
        ~CPU();
        int ok();
        bool dump();
        void execute();
        void errorpush(const char* message, int ip);
        void transbin();
    private:
        int getreg(int cmd);


        int ip_;
        int flag_;
        int register_[Q_REG];    
        Stack stack_;
        
        size_t bincapacity_;
        FILE* asm_;
        int* binprogram_;
        size_t sizebin_;
    };


CPU::CPU(const char* filename):
    ip_(0),
    flag_(0),    
    bincapacity_(DEFAULTSIZE),
    sizebin_(0)
    {
    for(int i = 0; i < Q_REG; i++)
        {
        register_[i] = 0;
        }
    asm_ = fopen(filename, "rt");
    if(!asm_)
        {
        errorpush(CPUERR_NOFILE, 0);
        }
    binprogram_ = new int[bincapacity_];
    
    IS_CORRECT;
    }



CPU::~CPU()
    {
    if(asm_)fclose(asm_);
    delete [] binprogram_;
    }


#define PUTERR(condition, errorcode) \
        if(condition) \
            { \
            error |= errorcode; \
            }

int CPU::ok()
    {
    int error = 0;
    PUTERR(asm_ == NULL, CPU_ASMNOTEXIST);
    PUTERR(binprogram_ == NULL, CPU_PROGRAMNOTEXIST);
    PUTERR((ip_ < 0 || ip_ > sizebin_), CPU_IP);
    PUTERR((!(stack_.ok() & STK_FULL)) && (stack_.ok() != 0), CPU_STACK); 
    return error;
    }


#define PRINTERR(err_name, message) \
    { \
    if(error & err_name) printf("CPU:Error: " message " \n"); \
    }

bool CPU::dump() 
    {
    printf("\nCPU:***Dump has been called***\n");
    printf("Sizebin_ = %zu\n\n", sizebin_);
    printf("ip = %d\n", ip_);
    printf("flag = %d\n", flag_);
    //---------------надо допилить
    printf("ax = %d\n", register_[0]);
    printf("bx = %d\n", register_[1]);
    printf("cx = %d\n", register_[2]);
    printf("dx = %d\n", register_[3]);
    int error = ok();
    PRINTERR(CPU_ASMNOTEXIST,"assembler file doesnt exist");
    PRINTERR(CPU_PROGRAMNOTEXIST, "program data has been spoiled")
    PRINTERR(CPU_IP, "bad ip" )
    PRINTERR(CPU_STACK, "stack broken");
    stack_.dump();
    if(error == 0)
        {
        printf("CPU:Any arrors has been detected\n");
        return true;
        }
    else return false;
    }

void CPU::errorpush(const char* message, int ip)
    {
    printf("CPU:ERROR:%s - instruction number %d\n", message, ip);
    dump();
    assert(0);
    }

void CPU::transbin()
    {
    IS_CORRECT;
    int cmd = 0;
    while(fscanf(asm_, "%d", &cmd) > 0)
        {
        if(sizebin_ >= bincapacity_)
            {
            bincapacity_ *= 2;
            binprogram_ = (int*)realloc(binprogram_, bincapacity_);
            }
        binprogram_[sizebin_++] = cmd;
        }
    IS_CORRECT;
    }  
//-------------
int CPU::getreg(int cmd)
    {
    if(cmd >= CPUREG_empty_last) errorpush(CPUERR_BADREG, ip_ + 1);
    return cmd - CPUREG_empty_first - 1;
    }


void CPU::execute()
    {
    int iterations = 0;
    IS_CORRECT;
    int Break = 0;
    while(ip_ < sizebin_)
        {
	iterations++;
        switch(binprogram_[ip_])
            {
            #define CPUCMDS
            #define CPUCMD(cmd_name, code) \
                case CPU_##cmd_name:                          \
                    code                        \
                    break;
            #include "CPUCMDS.txt"
            #undef CPUCMD
            #undef CPUCMDS
                default:
                    errorpush(CPUERR_UNKNOWNCMD, ip_ + 1);
            }
        ip_++;
        if(Break == 1) break;
        if(iterations > MAX_Q_ITERATIONS) errorpush(CPUERR_ENDLESSPRC, ip_);
        }
    IS_CORRECT;
    }     

int main(int argc, char* argv[])
    {
    if(argc > 1)
        {
        CPU cpu(argv[1]);
        cpu.transbin();
        cpu.execute();
        }
    }
  
