#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>	// Import time for random generation

#define SERVER_PORT 5432
#define MAX_PENDING 1
#define MAX_LINE 256


int main()
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
  unsigned int l;
	int s, new_s;

	char *loc;	// Initialize variable that will search for '?'
	char msg[256];	// Initialize char array to store messages
	char q[3];	// Initialize '?' char that can be appended to strings


	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);

	/* wait for connection, then receive and print text */
	while(1) {
		l = sizeof(sin);
		if ((new_s = accept(s, (struct sockaddr *)&sin, &l)) < 0) {
			perror("simplex-talk: accept");
			exit(1);
		}
		while ((len = recv(new_s, buf, sizeof(buf), 0))){

			// Print received message to server
			printf("%s", buf);

			// Search for a '?' in the client's message
			loc = strchr(buf, '?');

			// If client message NOT a question:
			if (loc == NULL){

				// Generate random number between 1 and 3
				srand(time(0)); // Current time as seed of random number generator
				int rand_num = (rand() % (3)) + 1;
				
				// Copy '?\n' to the q string
				strcpy(q, "?\n");

				// If random number is 1, then send response #1 to client
				if (rand_num == 1){
					strcpy(msg, "What makes you think ");
					strcat(msg, buf);	// Add client message to question
					// Remove newline
					len = strlen(msg);
					if (msg[len-1] == '\n'){
						msg[len-1] = 0;
					}
					strcat(msg, q);	// Punctuate the question
					// Send the server response to client
					send(new_s, msg, strlen(msg)+1,0);

				}

				// If random number is 2, send response #2 to client
				else if (rand_num == 2){
					// Format the question using client message
					char wrd[9];
					strcpy(msg, "How long since ");
					strcpy(wrd, " started?\n");
					strcat(msg, buf);
					// Remove newline
					len = strlen(msg);
					if (msg[len-1] == '\n'){
						msg[len-1] = 0;
					}
					strcat(msg, wrd);	// Punctuate
					// Send server response to client
					send(new_s, msg, strlen(msg)+1,0);

				}
				
				// Third possible response
				else {
					// Format the response
					strcpy(msg, "Did you come to me because ");
					strcat(msg, buf);
					// Remove newline
					len = strlen(msg);
					if (msg[len-1] == '\n'){
						msg[len-1] = 0;
					}
					strcat(msg, q);	// Punctuate
					// Send server response to client
					send(new_s, msg, strlen(msg)+1,0);

				}
			}

			else{ // If client asks a question

				// Generate random num between 1 and 5
				srand(time(0)); // Current time as seed of random num generator
				int rand_num = (rand() % (5)) + 1;
				
				// Format and send response #1
				if (rand_num == 1){
					strcpy(msg, "Tell me more...\n");
					send(new_s, msg, strlen(msg)+1,0);

				}
				
				// Format and send response #2
				else if (rand_num == 2){
					strcpy(msg, "I'm sorry to hear that.\n");
					send(new_s, msg, strlen(msg)+1,0);

				}

				// Format and send response #3
				else if (rand_num == 3){
					strcpy(msg, "How does that make you feel?\n");
					send(new_s, msg, strlen(msg)+1,0);

				}

				// Format and send response #4
				else if (rand_num == 4){
					strcpy(msg, "Is it really?\n");
					send(new_s, msg, strlen(msg)+1,0);

				}

				// Format and send response #5
				else{
					strcpy(msg, "Are you sure?\n");
					send(new_s, msg, strlen(msg)+1,0);

				}	
			}
		}
	
		fputs(buf, stdout);
		close(new_s);
	}
}
