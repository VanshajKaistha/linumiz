#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_STACK_SIZE 100
#define MAX_DIR_LENGTH 1024

typedef struct 
{
	char *directories[MAX_STACK_SIZE];
	int top;
} DirectoryStack;

void initStack(DirectoryStack *stack) 
{
	stack->top = -1;
}

void push(DirectoryStack *stack, const char *dir)
{
	if (stack->top >= MAX_STACK_SIZE - 1) 
	{
		printf("Directory stack is full.\n");
		return;
	}
	stack->top++;
	stack->directories[stack->top] = malloc(MAX_DIR_LENGTH);
	strncpy(stack->directories[stack->top], dir, MAX_DIR_LENGTH);
}

char* pop(DirectoryStack *stack) 
{
	if (stack->top < 0)
	{
		printf("Directory stack is empty.\n");
		return NULL;
	}
	char *dir = stack->directories[stack->top];
	stack->top--;
	return dir;
}

void printStack(DirectoryStack *stack)
{
	printf("Current Directory Stack:\n");
	for (int i = stack->top; i >= 0; i--) 
	{
		printf("%d: %s\n", i, stack->directories[i]);
	}
}

int main() 
{
	DirectoryStack stack;
	initStack(&stack);
	char currentDir[MAX_DIR_LENGTH];

	getcwd(currentDir, sizeof(currentDir));
	push(&stack, currentDir);

	char command[100];

	while (1) 
	{
		printf("Enter command (pushd <dir>, popd, exit): ");
		fgets(command, sizeof(command), stdin);

		command[strcspn(command, "\n")] = 0;

		if (strncmp(command, "pushd ", 6) == 0) 
		{
			char *dir = command + 6;
			getcwd(currentDir, sizeof(currentDir));
			push(&stack, currentDir);
			if (chdir(dir) == 0) 
			{
				printf("Changed directory to: %s\n", dir);
			} 
			else 
			{
				printf("Failed to change directory to: %s\n", dir);
			}
		}
		else if (strcmp(command, "popd") == 0) 
		{
			char *lastDir = pop(&stack);
			if (lastDir)
			{
				if (chdir(lastDir) == 0) 
				{
					printf("Changed directory back to: %s\n", lastDir);
				} 
				else {
					printf("Failed to change directory back to: %s\n", lastDir);
				}
				free(lastDir); 
			}
		}
		else if (strcmp(command, "exit") == 0) {
			break;
		} 
		else {
			printf("Unknown command: %s\n", command);
		}

		printStack(&stack);
	}

	while (stack.top >= 0) {
		free(pop(&stack));
	}

	return 0;
}

