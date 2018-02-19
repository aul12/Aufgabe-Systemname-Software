#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LINE_LENGTH 32
#define INPUT_COUNT 5

double convertLine(char* input, int length){
	int state = 0;

	long long integerRepr = 0;
	signed int decShift = 0;
	signed int exp = 0;
	signed int expSign = 1;
	signed char sign = 1;

	for(int c=0; c<length; c++){
		switch(state){
			case 0:
				if(input[c] == '.'){
					state = 3;
				} else if(input[c] == '+' || input[c] == '-') {
					state = 1;

					sign = input[c]=='+'?1:-1;
				} else if(input[c] >= '0' && input[c] <= '9') {
					state = 2;
					
					integerRepr = 10*integerRepr + input[c] - '0';
				}
				break;
			case 1:
				if(input[c] == '.'){
					state = 3;
				} else if(input[c] >= '0' && input[c] <= '9'){
					state = 2;

					integerRepr = 10*integerRepr + input[c] - '0';
				}
				break;
			case 2:
				if(input[c] == '.'){
					state = 4;
				} else if(input[c] == 'E' || input[c] == 'e') {
					state = 5;
				} else if(input[c] >= '0' && input[c] <= '9') {
					state = 2;
					
					integerRepr = 10*integerRepr + input[c] - '0';
				}
				break;
			case 3:
				if(input[c] >= '0' && input[c] <= 9) {
					state = 4;
					decShift = -1;	
					integerRepr = 10*integerRepr + input[c] - '0';
				}
				break;
			case 4:
				if(input[c] == 'E' || input[c] == 'e') {
					state = 5;
				} else if(input[c] >= '0' || input[c] <= '9') {
					state = 4;
		
					integerRepr = 10*integerRepr + input[c] - '0';
					decShift--;
				} 
				break;
			case 5:
				if(input[c] == '+' || input[c] == '-'){
					state = 6;
					expSign = input[c]=='+'?1:-1;
				} else if(input[c] >= '0' && input[c] <= '9'){
					state = 7;
					exp = exp*10 + input[c]-'0';
				}
				break;
			case 6:
				if(input[c] >= '0' && input[c] <= '9'){
					state = 7;
					exp = exp*10 + input[c]-'0';
				}
				break;
			case 7:
				if(input[c] >= '0' && input[c] <= '9'){
					state = 7;
					exp = exp*10 + input[c]-'0';
				}
				break;
			default:
				state = 0;
				break;

		}
	}

	return sign * integerRepr * pow(10, decShift + expSign*exp);	
}

double kahanSum(double* input, int length){
    double sum = 0.0;
    double c = 0.0;
    for(int i = 0; i < length; i++) {
        double y = input[i] - c;
        double t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    return sum;
}

int main(void){
    double sumNaive = 0, sumKahan = 0;

    double inputs[INPUT_COUNT];

    for(int inputCount=0; inputCount<INPUT_COUNT; inputCount++){
        char input[MAX_LINE_LENGTH];
        int c = 0;
        int in = 0;
        while(c < MAX_LINE_LENGTH &&  (in = getchar()) != EOF && in != '\n'){
            input[c++] = (char) in;
        }
        double dbl = convertLine(input, c);

        printf("%f\n", dbl);

        inputs[inputCount] = dbl;
    }


    sumKahan = kahanSum(inputs, INPUT_COUNT);

    for(int c=0; c<INPUT_COUNT; c++){
        sumNaive += inputs[c];
    }

    printf("Regular sum: %.17lg\n", sumNaive);
    printf("Kahan sum: %.17lg\n", sumKahan);
    printf("Difference: %lg\n", fabs(sumNaive - sumKahan));

	return 0;
}
