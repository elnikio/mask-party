
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEPPING_DEBUG_MESSAGES
#define FALSE	0
#define TRUE	1

char* characters_all = "abcdefghijklmnopqrstuvABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./1234567890:;<=>?@[\\]^_`{|}~ \t\n\r";

struct token_type {
	char* re;
	int id;
};

struct state_list {
	struct state* current;
	struct state_list* next;
};	// terminated by a NULL state_list, not a state_list whose current = NULL.

void msg(char* text) {
	#ifdef STEPPING_DEBUG_MESSAGES
	printf("\e[1;37m%s\e[0m", text);
	#endif
}

void err(char* text) {
	#ifdef STEPPING_DEBUG_MESSAGES
	printf("\e[1;31m%s\e[0m", text);
	#endif
}

void gud(char* text) {
	#ifdef STEPPING_DEBUG_MESSAGES
	printf("\e[1;32m%s\e[0m", text);
	#endif
}

struct state {
	char **inc;					// array of strings. check all the strings. pre-defined strings like \d or \w can easily be included just by adding a pointer.
	char **exc;					// array of strings, containing characters to exclude.
	int id;						// if this is an exit state, here lay the id of the matched token type.
	struct state_list *next;	// linked list of all possible states the may succeed this one.
};

void print_inc (struct state* state) {
	for (int i = 0; (state -> inc)[i] != NULL; i ++) {
			printf("%s", (state -> inc)[i]);
	}
}

struct token_type** initialize_token_types () {
	msg("Starting token initializer...\n");

	struct token_type** token_types = malloc(sizeof(struct token_type*) * 20);

	struct token_type* wht = malloc(sizeof(struct token_type));
	struct token_type* ele = malloc(sizeof(struct token_type));
	struct token_type* all = malloc(sizeof(struct token_type));
	struct token_type* any = malloc(sizeof(struct token_type));
	struct token_type* cup = malloc(sizeof(struct token_type));
	struct token_type* cap = malloc(sizeof(struct token_type));
	struct token_type* mul = malloc(sizeof(struct token_type));
	struct token_type* div = malloc(sizeof(struct token_type));
	struct token_type* add = malloc(sizeof(struct token_type));
	struct token_type* sub = malloc(sizeof(struct token_type));
	struct token_type* pow = malloc(sizeof(struct token_type));
	struct token_type* rem = malloc(sizeof(struct token_type));
	struct token_type* ite = malloc(sizeof(struct token_type));
	struct token_type* num = malloc(sizeof(struct token_type));
	struct token_type* dec = malloc(sizeof(struct token_type));
	struct token_type* str = malloc(sizeof(struct token_type));
	struct token_type* spa = malloc(sizeof(struct token_type));
	struct token_type* set = malloc(sizeof(struct token_type));
	struct token_type* var = malloc(sizeof(struct token_type));

	wht -> id = 0;
	ele -> id = 1;
	all -> id = 2;
	any -> id = 3;
	cup -> id = 4;
	cap -> id = 5;
	mul -> id = 6;
	div -> id = 7;
	add -> id = 8;
	sub -> id = 9;
	pow -> id = 10;
	rem -> id = 11;
	ite -> id = 12;
	num -> id = 13;
	dec -> id = 14;
	str -> id = 15;
	spa -> id = 16;
	set -> id = 17;
	var -> id = 18;

	wht -> re = malloc(16);
	ele -> re = malloc(16);
	all -> re = malloc(16);
	any -> re = malloc(16);
	cup -> re = malloc(16);
	cap -> re = malloc(16);
	mul -> re = malloc(16);
	div -> re = malloc(16);
	add -> re = malloc(16);
	sub -> re = malloc(16);
	pow -> re = malloc(16);
	rem -> re = malloc(16);
	ite -> re = malloc(16);
	num -> re = malloc(16);
	dec -> re = malloc(16);
	str -> re = malloc(16);
	spa -> re = malloc(16);
	set -> re = malloc(16);
	var -> re = malloc(16);

	"	. ^ $ * + ? { } [ ] \\ | ( )";

	strcpy(wht -> re, "dog");
	strcpy(ele -> re, "dick");
	strcpy(all -> re, "door");
	strcpy(any -> re, "man");
	strcpy(cup -> re, "mob");
	strcpy(cap -> re, "n");
	strcpy(mul -> re, "\\*");
	strcpy(div -> re, "/");
	strcpy(add -> re, "\\+");
	strcpy(sub -> re, "-");
	strcpy(pow -> re, "\\^");
	strcpy(rem -> re, "~");
	strcpy(ite -> re, "_");
	strcpy(num -> re, "[0-9]+");
	strcpy(dec -> re, "[0-9]+\\.[0-9]+");
	strcpy(str -> re, "\"[^\"]+\"");
	strcpy(spa -> re, "[A-Z]+");
	strcpy(set -> re, "\\|[A-Z]+");
	strcpy(var -> re, "[a-z][a-z0-9]*");

	/*
	strcpy(wht -> re, "[ \\t\\n]");
	strcpy(ele -> re, "e");
	strcpy(all -> re, "v");
	strcpy(any -> re, "#");
	strcpy(cup -> re, "u");
	strcpy(cap -> re, "n");
	strcpy(mul -> re, "\\*");
	strcpy(div -> re, "/");
	strcpy(add -> re, "\\+");
	strcpy(sub -> re, "-");
	strcpy(pow -> re, "\\^");
	strcpy(rem -> re, "~");
	strcpy(ite -> re, "_");
	strcpy(num -> re, "[0-9]+");
	strcpy(dec -> re, "[0-9]+\\.[0-9]+");
	strcpy(str -> re, "\"[^\"]+\"");
	strcpy(spa -> re, "[A-Z]+");
	strcpy(set -> re, "\\|[A-Z]+");
	strcpy(var -> re, "[a-z][a-z0-9]*");
	*/
	// greedy matching for repeats: [abc]* tries to match with largest possible repeat number, then reduces the number if fail, etc. until success.

	token_types[0] = wht;
	token_types[1] = ele;
	token_types[2] = all;
	token_types[3] = any;
	token_types[4] = cup;
	token_types[5] = cap;
	token_types[6] = mul;
	token_types[7] = div;
	token_types[8] = add;
	token_types[9] = sub;
	token_types[10] = pow;
	token_types[11] = rem;
	token_types[12] = ite;
	token_types[13] = num;
	token_types[14] = dec;
	token_types[15] = str;
	token_types[16] = spa;
	token_types[17] = set;
	token_types[18] = var;
	token_types[19] = NULL;

	gud("Tokens initialized successfully!\n");
//	err("Tokens initialization failed!\n");
	return token_types;
}
/*
struct state {
	char **inc;					// array of strings. check all the strings. pre-defined strings like \d or \w can easily be included just by adding a pointer.
	int id;						// if this is an exit state, here lay the id of the matched token type.
	struct state_list *next;	// linked list of all possible states the may succeed this one.
};
struct state_list {
	struct state* current;
	struct state_list* next;
}
*/

char string_contains(char* string, char target) {
	for (int i = 0; string[i] != '\0'; i ++)
		if (string[i] == target)
			return 1;
	return 0;
}

/*
def listTreePrint(l, depth = 0, lines = [], last = True):
    prefix = ""

    for tab in range(0, depth):
        if lines[tab] == True:
            if tab >= depth - 1:
                prefix += "├───"
            else:
                prefix += "│   "
        elif tab >= depth - 1:
            prefix += "└───"
        else:
            prefix += "    "
    print(prefix + str(l))

    if str(type(l)) == "<class 'list'>":
        for i in range(0, len(l)):
            if i == len(l) - 1:
                lastNew = True
            else:
                lastNew = False
            if lastNew:
                listTreePrint(l[i], depth + 1, lines + [False], lastNew)
            else:
                listTreePrint(l[i], depth + 1, lines + [True], lastNew)
*/

void print_scanner (struct state* scanner, int depth, char* lines, char last) {
	char* prefix = malloc(64);
	prefix[0] = '\0';

	for (int tab = 0; tab < depth; tab ++) {
		if (lines[tab] == 1) {
			if (tab >= depth - 1)
				strcat(prefix, "├───");
			else
				strcat(prefix, "│   ");
		}
		else if (tab >= depth - 1)
			strcat(prefix, "└───");
		else
			strcat(prefix, "    ");
	}
	printf("%s %d : ", prefix, scanner -> id);
	print_inc (scanner);
	printf("\n");

	struct state_list* next;
	if (scanner -> next != NULL) {
		next = scanner -> next;
		while (next != NULL) {
			char* lines_new = malloc(16);
			for (int i = 0; i < 16; i ++)
				lines_new[i] = lines[i];
			if (next -> next == NULL) {
				lines_new [depth] = 0;
				print_scanner (next -> current, depth + 1, lines_new, 1);
			}
			else {
				lines_new [depth] = 1;
				print_scanner (next -> current, depth + 1, lines_new, 0);
			}
			next = next -> next;
		}
	}
}

struct state* scanner_generator (struct token_type** token_types) {
	msg("Starting scanner generator...\n");

	// initialize root node:
	struct state* scanner = malloc(sizeof(struct state));
	scanner -> inc = malloc(sizeof(char*) * 4);	// one for nums, one for digits, one for custom, one terminating
	scanner -> inc[0] = NULL;
	scanner -> exc = malloc(sizeof(char*) * 4);	// one for nums, one for digits, one for custom, one terminating
	scanner -> exc[0] = NULL;
	scanner -> id = 0;
	scanner -> next = NULL;
	int idi = 1;	// next id to use

	char* re;

	// metacharacters bitmask:
	// 0	.	any character
	//
	
	// iterate token_types:
	for (int i = 0; token_types[i] != NULL; i ++) {
		re = token_types[i] -> re;
		#ifdef STEPPING_DEBUG_MESSAGES
		printf("\nParsing re: %s\n", re);
		#endif

		struct state* parent = scanner;

		// iterate characters in token_type:
		for (int j = 0; re[j] != '\0'; j ++){
			#ifdef STEPPING_DEBUG_MESSAGES
			//printf("Char: %c\n", re[j]);
			#endif

			// initialize new state
			struct state* child = NULL; // always assign new pointers to NULL so you can easily tell if they've been initialized or not.
			int inci = 0;

			struct state_list* branch; // this is for checking if the branch already exists
			char exists;
			switch(re[j]) {
				case '.':
					//child -> inc[inci ++] = characters_all;
					break;
				case '^':
					break;
				case '$':
					break;
				case '*':
					break;
				case '+':
					break;
				case '?':
					break;
				case '{':
					break;
				case '}':
					break;
				case '[':
					break;
				case ']':
					break;
				case '\\':
					break;
				case '|':
					break;
				case '(':
					break;
				case ')':
					break;
				default:
					// check if a state already exists for this next character:
					exists = FALSE;
					for (branch = parent -> next; branch != NULL; branch = branch -> next) {
						struct state* next = branch -> current;
						for (int k = 0; next -> inc[k] != NULL; k ++) {
							if (string_contains(next -> inc[k], re[j])) {
								exists = TRUE;
								child = next;
								break;
							}
						}
						if (exists) for (int k = 0; next -> exc[k] != NULL; k ++) {
							if (string_contains(next -> exc[k], re[j])) {
								exists = FALSE;
								break;
							}
						}
					}
					if (exists) {
						//printf("State handling char %c already exists after state %d.\n", re[j], idi - 1);
					}
					else {
						//printf("Creating state %d to handle char %c.\n", idi, re[j]);
						child = malloc(sizeof(struct state));
						child -> inc = malloc(sizeof(char*) * 4);	// one for nums, one for digits, one for custom, one terminating
						child -> inc[0] = NULL;
						child -> exc = malloc(sizeof(char*) * 4);
						child -> exc[0] = NULL;
						child -> id = idi ++;
						child -> next = NULL;

						/*
						struct state_list* parent_infertilized_egg = parent -> next;
						while (TRUE) {
							if (parent_infertilized_egg == NULL)
								break;
							else
								parent_infertilized_egg = parent_infertilized_egg -> next;
						}
						*/
						struct state_list* parent_infertilized_egg = parent -> next; // parent item in state_list of parent state
						if (parent_infertilized_egg == NULL) {
							struct state_list* womb = malloc(sizeof(struct state_list));
							womb -> current = child;
							womb -> next = NULL;
							parent -> next = womb;
							//break;
						}
						else while (TRUE) {
							if (parent_infertilized_egg -> next == NULL) {
								struct state_list* womb = malloc(sizeof(struct state_list));
								womb -> current = child;
								womb -> next = NULL;
								parent_infertilized_egg -> next = womb;
								break;
							}
							else
								parent_infertilized_egg = parent_infertilized_egg -> next;
						}
					}
					child -> inc[0] = malloc(2);
					child -> inc[0][0] = re[j];
					child -> inc[1] = NULL;
					break;
			}
			if (child != NULL) parent = child;
		}
		msg("Scanner:\n");
		char* lines = malloc(16);
		print_scanner (scanner, 0, lines, 1);
	}

	gud("Scanner generated successfully!\n");
	return scanner;
}

int main () {
	struct token_type** token_types = initialize_token_types();
	struct state* scanner = scanner_generator(token_types);

	msg("Final scanner:\n");
	char* lines = malloc(16);
	print_scanner (scanner, 0, lines, 1);
}
