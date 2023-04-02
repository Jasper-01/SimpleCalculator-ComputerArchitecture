#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define buffer 15
#define whiteSpace " "

typedef struct{
    int str_len;  // length of line
    char* instruction;  // whole line
    char* opcode;  // opcode for what to do with remaining inputs of line
    char* dst;  // designated register
    char* src1;  // 1st source (ADD, SUB, MUL, DIV) || only source (MOV) || value for dst (LW) || STDOUT (SW)
    char* src2;  // 2nd source (ADD, SUB, MUL, DIV)

    int* rArr[9];
} calcVars;

int hexToDec(char* inp){  // assuming "0x" is included
    int sum = 0;
    for(int i = 2; i < strlen(inp); i++){
        if(inp[i] >= '0' && inp[i] <= '9'){
            sum += (int)(inp[i]) - 48;
        } else if (inp[i] >= 'A' && inp[i] <= 'F'){
            sum += (int)(inp[i]) - 65 + 10;
        }
    }
    return sum;
}

void _arithmetic(calcVars* A){
    // temporary string / char arr
    char temp[5];

    // finding dst register
    strcpy(temp, A->dst);
    int d = (int)(temp[1]) - 48;  // ASCII value of 0 is 48
    // finding src1 register
    strcpy(temp, A->src1);
    int s1 = (int)(temp[1]) - 48;
    // finding src2 register
    strcpy(temp, A->src2);
    int s2 = (int)(temp[1]) - 48;

    int num1 = A->rArr[s1];
    int num2 = A->rArr[s2];

    if(strcmp(A->opcode, "ADD") == 0){
        A->rArr[d] = num1 + num2;
        printf("%s \t# r%d(%d) and r%d(%d) are added and saved into r%d(%d)\n",
               A->instruction, s1, num1, s2, num2, d, A->rArr[d]);
    } else if(strcmp(A->opcode, "SUB") == 0){
        A->rArr[d] = num1 - num2;
        printf("%s \t# r%d(%d) and r%d(%d) are subtracted and saved into r%d(%d)\n",
               A->instruction, s1, num1, s2, num2, d, A->rArr[d]);
    } else if(strcmp(A->opcode, "MUL") == 0){
        A->rArr[d] = num1 * num2;
        printf("%s \t# r%d(%d) and r%d(%d) are multiplied and saved into r%d(%d)\n",
               A->instruction, s1, num1, s2, num2, d, A->rArr[d]);
    } else if(strcmp(A->opcode, "DIV") == 0){
        A->rArr[d] = num1 / num2;
        printf("%s \t# r%d(%d) and r%d(%d) are divided and saved into r%d(%d)\n",
               A->instruction, s1, num1, s2, num2, d, A->rArr[d]);
    }
};

void _mov(calcVars* A){
    int d = (int)(A->dst[1]) - 48;
    int s1 = (int)(A->src1[1]) - 48;
    int temp = 0;
    temp = A->rArr[s1];
    A->rArr[d] = temp;
    printf("%s \t# r%d is saved into r%d(%d)\n", A->instruction, s1, d, A->rArr[d]);
};

// UNSOLVED
void _rst(calcVars* A){
    for(int i = 0; i<= 9; i++){
        A->rArr[i] = 0;
    }
    printf("%s \t\t# All registers have been cleared\n", A->instruction);
};

void case2char(calcVars*A){  // LW, SW
    // temporary string / char arr
    char temp[5];
    // finding dst register
    strcpy(temp, A->dst);
    int d = (int)(temp[1]) - 48;  // ASCII value of 0 is 48

    if(strcmp(A->opcode,"SW") == 0){
        // print output
        printf("%s \t# %s(%d) is displayed\n", A->instruction, A->dst, A->rArr[d]);
    } else if(strcmp(A->opcode, "LW") == 0){
        // find number to be assigned to dst
        strcpy(temp, A->src1);
        int src = hexToDec(temp);

        // assign src1 to dst
        A->rArr[d] = src;

        // print output
        printf("%s \t# %d is saved into %s\n", A->instruction, src, A->dst);
    }
}

void case3char(calcVars*A){  // ADD, SUB, MUL, DIV, MOV, RST
    if(strcmp(A->opcode, "MOV") == 0){
         _mov(A);
    } else{
        _arithmetic(A);
    }
}

void calculate(calcVars* vars){
    switch(strlen(vars->opcode)){
        case 2:  // LW, SW
            case2char(vars);
            break;
        case 3:  // ADD, SUB, MUL, DIV, MOV, RST
            case3char(vars);
            break;
        default:
            break;
    }
}

void _readInput(char* inLine, calcVars* inp){
    inp -> str_len = sizeof(inLine) + 1;
    inp -> instruction = (char*)malloc(buffer);
    strcpy(inp->instruction, inLine);
    inp->instruction[strcspn(inp->instruction, "\n")] = 0;

    char* tokenPtr;
    tokenPtr = strtok(inLine, whiteSpace);
    int checkRST = 1;

    for(int counter = 1; tokenPtr != NULL; counter++){
        switch(counter){
            case 1:
                inp -> opcode = (char*) malloc(sizeof(char) * strlen(tokenPtr));
                strcpy(inp -> opcode, tokenPtr);
                break;
            case 2:
                inp -> dst = (char*) malloc(sizeof(char) * strlen(tokenPtr));
                strcpy(inp -> dst, tokenPtr);
                checkRST = 0;
                break;
            case 3:
                inp -> src1 = (char*) malloc(sizeof(char) * strlen(tokenPtr));
                strcpy(inp -> src1, tokenPtr);
                break;
            case 4:
                inp -> src2 = (char*) malloc(sizeof(char) * strlen(tokenPtr));
                strcpy(inp -> src2, tokenPtr);
                break;
            default:
                break;
        }
        tokenPtr = strtok(NULL, whiteSpace);
    }

    if(checkRST == 1) _rst(inp);
    else calculate(inp);
};

int main() {
    FILE* fp;
    char* line = malloc(buffer);
    struct calcVars* readInp = (calcVars*)malloc(sizeof(calcVars));

    fp = fopen("C:\\Users\\Jasper\\Documents\\CLion\\C language\\CompArch\\HW1\\input.txt", "r");
    if(fp == NULL)
        exit(EXIT_FAILURE);

    while(fgets(line, buffer, fp) != NULL){
        _readInput(line, readInp);

    }
    free(line);

    return 0;
}
