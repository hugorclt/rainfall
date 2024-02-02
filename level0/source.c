# include <stdio.h>
# include <stdlib.h>


int main(int argc, char** argv) {

	int	user_input = atoi(argv[1]);
	if (user_input == 423)
		system("sh");
	else
		printf("No !\n");
	return (0);
}
