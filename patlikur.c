#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STEP_MAX 400

char card[4]; // 1 to 10
char token[3]; //inbetween cards,
/*  0: addition
	1: substraction
	2: multiplication
	3: division
*/
int i;
float val;
int step;
	
unsigned int evaluate_chance(){
	unsigned int odd=1;
	char copy[11]= {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	
	for (i=0; i<4; i++){
		odd *= copy[card[i]];
		copy[card[i]]--;	
		}
	return(odd);
	}
	
char evaluate_set(){
	char shuffle, temp;
	
	/*
	 * among valid 24 combinations of the four dealt cards
	 * swapping position of two cards will result in two combination each
	 * so number of shuffles is decided as:
	 * 4! / 2!
	 */
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
			default: break; // '0' means no shuffle
			}
		}
	
	// shuffle token
	for (i=0; i<3; i++){
		token[i] = rand()%4;
		}
	
	// evaluate value
	val = card[0];
	for (i=0; i<3; i++){
		switch (token[i]){
			case 0: val+= (float) card[i+1]; break;
			case 1: val-= (float) card[i+1]; break;
			case 2: val*= (float) card[i+1]; break;
			case 3: val/= (float) card[i+1]; break; // does it need to be exact factors?
			default: break;
			}
		}
	
	if ((val==-24.0) && (token[2]!=0) && (token[2]!=1) )return(24);
	
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
	printf("chance of appearance: %.4f\n", evaluate_chance()/1e4);
			
	while(step<STEP_MAX){
		step++;
		if (evaluate_set()==24) break;
		//printf("step%d\n",step);
		}
	
	if (step==STEP_MAX){
		printf("computer is too dumb %d\n",step);
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
	printf("after %d steps: %d %c %d %c %d %c %d\n", step, card[0], token[0], card[1], token[1],\
		card[2], token[2], card[3]);
	
	}
	
// ended up with Monte Carlonian search to ensure constant number of operations in each iteration
// was considering Reverse Polish Notation and non-randomized exhaustive search
// todo: parenthesis rule, like (5 - (1/5)) * 5
