#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STEP_MAX 8e6

char card[4]; // 1 to 10
char token[3]; //inbetween cards,
/*  0: addition
	1: substraction
	2: multiplication
	3: division
*/
int i;
int val;

char evaluate_set(){
	char shuffle, temp;
	
	// shuffle cards
	for (i=0; i<12; i++){ // maximum number of shuffles
		shuffle= rand()%7;
		//printf("shuffle: %d\n",shuffle);
		switch(shuffle){
			case 1: temp= card[0]; card[0]= card[1]; card[1]= temp; break;
			case 2: temp= card[1]; card[1]= card[2]; card[2]= temp; break;
			case 3: temp= card[2]; card[2]= card[3]; card[3]= temp; break;
			case 4: temp= card[0]; card[0]= card[2]; card[2]= temp; break;
			case 5: temp= card[1]; card[1]= card[3]; card[3]= temp; break;
			case 6: temp= card[0]; card[0]= card[3]; card[3]= temp; break;
			default: break;
			}
		}
	
	// shuffle token
	for (i=0; i<4; i++){
		token[i] = rand()%4;
		}
	
	// evaluate value
	val = card[0];
	for (i=0; i<3; i++){
		switch (token[i]){
			case 0: val+= card[i+1]; break;
			case 1: val-= card[i+1]; break;
			case 2: val*= card[i+1]; break;
			case 3: val/= card[i+1]; break; // does it need to be exact factors?
			default: break;
			}
		}
	return(val);
	}

int main(int argc, char **argv){
	srand(time(NULL)); // get in today's mood
	if (argc==1){ // no drawn card, generate set of one
		for (i=0; i<4; i++){
			card[i] = 1+ rand()%10;
			printf("%d ", card[i]);
			}
		}
	else{
		for (i=0; i<4; i++){
			card[i] = atoi(argv[1+i]);
			printf("%d ", card[i]);
			}
		}	
	printf("deal\n");
			
	int step=2e3; // just give up already		
	while(step<STEP_MAX){
		if (evaluate_set()==24) break;
		step++;
		//printf("step%d\n",step);
		}
	
	if (step==STEP_MAX){
		printf("computer is too dumb\n");
		return(0);
		}
	
	for (i=0; i<3; i++){
		switch(token[i]){
			case 0: token[i] = '+'; break;
			case 1: token[i] = '-'; break;
			case 2: token[i] = '*'; break;
			case 3: token[i] = '/'; break;
			}
		}
	
	// print result
	printf("after %d steps: %d %c %d %c %d %c %d", step, card[0], token[0], card[1], token[1],\
		card[2], token[2], card[3]);
	
	}
	
// ended up with Monte Carlonian search
// was considering reverse Polish notation and non-randomized exhaustive search
