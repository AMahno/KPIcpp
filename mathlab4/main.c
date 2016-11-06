#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void fillInput();
void getDeltas();
float ownDeriative(float arg);
int factorial(int n);

float argumentsTable[6][2];
float deltas[6][5];

int main(){
    for(int m = 0; m<5; m++){
        for(int k = 0; k<5; k++)
            deltas[m][k] = 0;
    }
    fillInput(argumentsTable);
    getDeltas(argumentsTable, deltas);
    for(int z = 0; z<6; z++){
        for(int x = 0; x<2; x++){
            printf("%3.4f\t", argumentsTable[z][x]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i = 0; i<6; i++){
        for(int j = 0; j<5; j++){
                if(deltas[i][j]!=0) printf("%3.4f\t", deltas[i][j]);
        }
        printf("\n");
    }
    printf("\nEnter the argument: ");
    float arg;
    scanf("%f", &arg);
    float ownDer = ownDeriative(arg);
    float realDer = -pow(1/sin(arg), 2);
    printf("Derivative calculated: %f\n", ownDer);
    printf("Derivative from analytic method: %f\n", realDer);
    printf("Absolute error: %f\n", ownDer-realDer);
    printf("Relative error: %f%\n", 100*fabs((ownDer-realDer)/ownDer));
    return 0;
}

float ownDeriative(float arg){
    float t = (arg - 1)/0.1;
    printf("t = %3.4f\n", t);
    float first = (float) deltas[0][2] * (2*t - 1)/factorial(2);
    float second = (float) deltas[0][3] * (3*pow(t,2)-6*t+2)/factorial(3);
    float third = (float) deltas[0][4] * (4*pow(t,3)-18*pow(t,2)+22*t-6)/factorial(4);
    float result = (float) (1/0.1) * (deltas[0][1] + first + second + third);
    return result;
}

void getDeltas(){
    for(int i = 0; i<6; i++){
        deltas[i][0] = argumentsTable[i][1];
    }
    for(int j = 1; j<5; j++){
        for(int z = 0; z<6-j; z++){
            deltas[z][j] = deltas[z+1][j-1]-deltas[z][j-1];
        }
    }
}

void fillInput(){
    for(int i = 0; i<6; i++){
        argumentsTable[i][0] = i*0.1+1;
        argumentsTable[i][1] = (float) 1/tan(argumentsTable[i][0]);
    }
}

int factorial(int n){
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
