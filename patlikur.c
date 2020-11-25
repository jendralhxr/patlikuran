#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define STEP_MAX 4000

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
	char copy[14]= {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
	
	for (i=0; i<4; i++){
		odd *= copy[card[i]];
		copy[card[i]]--;	
		}
	return(odd);
	}
	
float evaluate_set(char eval_mode){
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
	
	// sequential eval, may imply nested parenthesis
	if (eval_mode==0){
		val = card[0];
		for (i=0; i<3; i++){
			switch (token[i]){
				case 0: val+= (float) card[i+1]; break;
				case 1: val-= (float) card[i+1]; break;
				case 2: val*= (float) card[i+1]; break;
				case 3: val/= (float) card[i+1]; break;
				default: break;
				}
			}
		return(val);
		}
		
	// two parentheses	
	else if (eval_mode==2){
		float nval[3];
		for (i=0; i<3; i+=2){
			switch (token[i]){
				case 0: nval[i] = (float) card[i] + (float) card[i+1]; break;
				case 1: nval[i] = (float) card[i] - (float) card[i+1]; break;
				case 2: nval[i] = (float) card[i] * (float) card[i+1]; break;
				case 3: nval[i] = (float) card[i] / (float) card[i+1]; break;
				default: break;
				}
			}
		switch (token[1]){
			case 0: nval[1] = nval[0] + nval[2]; break;
			case 1: nval[1] = nval[0] - nval[2]; break;
			case 2: nval[1] = nval[0] * nval[2]; break;
			case 3: nval[1] = nval[0] / nval[2]; break;
			default: break;
			}
			
		return(nval[1]);
		}
	
	}

int main(int argc, char **argv){
	char hard=0;
	float result;
	srand(time(NULL)); // get in today's mood
	if (argc==1){ // no drawn card, generate set of one
		for (i=0; i<4; i++){
			card[i] = 1+ rand()%14;
			printf("%d ", card[i]);
			}
		}
	else{
		for (i=0; i<4; i++){
			card[i] = atoi(argv[1+i]);
			printf("%d ", card[i]);
			}
		}	
	printf(" -- chance of appearance: %.4f", evaluate_chance()/1e4);
	
	while(step<STEP_MAX){
		step++;
		
		// evaluating the cards and operand tokens sequentially
		result = evaluate_set(0);
		if (result== 24.0) break;
		else{ 
			if ((token[2] == 2) || (token[2] == 3)){
				// final multiplication/division is kinda advanced strat
				if (result== -24.0) 	 {hard=1; break;}
				else if (result*24== 1)  {hard=1; break;}
				else if (result*24== -1) {hard=1; break;}
				}
			}	
		
		// two parentheses operation
		result = evaluate_set(2);
		if (result== 24.0) {hard=1; break;} // I guess this also classifies as 'hard'
		
		//printf("step%d %f\n",step, result);
		}
	
	if (step==STEP_MAX){
		printf("\ncomputer is too dumb");
		printf("\n");
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
	printf("\nafter %d steps: %d %c %d %c %d %c %d\n", step, card[0], token[0], card[1], token[1],\
		card[2], token[2], card[3]);
	if (hard) printf("indeed it was a difficult one\n");
		
	}
	
// ended up with Monte Carlonian search to ensure constant number of operations in each iteration
// was considering Reverse Polish Notation and non-randomized exhaustive search
// nested parenthesis was handled liek a boss

// I should've done RPN, for real
