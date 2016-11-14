#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

float ctg(float x);

int main(){
    float a, b;
    int tests;
    printf("Enter left border: ");
    scanf("%f", &a);
    printf("Enter right border: ");
    scanf("%f", &b);
    printf("Enter number of tests: ");
    scanf("%d", &tests);
    char answer;
    int randomizer = 0;
    if(a>0.1 && b<1.57 && b>a){
        do{
            system("cls");
            float top = 0;
            if(ctg(a) > ctg(b)) top = ctg(a);
            else top = ctg(b);
            float square = (b-a)*top;
            int undersum = 0;
            int oversum = 0;
            srand(time(NULL)+randomizer);
            for(int i = 0; i<tests; i++){
                float x = ((float)rand()/(float)(RAND_MAX)) * (b-a) + a;
                float y = ((float)rand()/(float)(RAND_MAX)) * top;
                if(y <= ctg(x)){
                    undersum++;
                }else{
                    oversum++;
                }
            }
            float result = (float) square*undersum/(oversum+undersum);
            float NL = log(sin(b))-log(sin(a));
            printf("Results over line: %d\nResults under line: %d\n", oversum, undersum);
            printf("Monte-Carlo result:    %f\n", result);
            printf("Newton-Leibniz result: %f\n", NL);
            printf("Absolute error: %f\n", fabs(NL-result));
            printf("Relative error: %f%%\n", 100*fabs(NL-result)/result);
            printf("Try again? [*/n] ");
            answer = getchar();
            randomizer++;
        }while(answer != 'n');
        }else{
            printf("Dangerous arguments!\n");
        }
    return 0;
}

float ctg(float x){return 1/tan(x);}
