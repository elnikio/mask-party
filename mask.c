
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STEPPING_DEBUG_MESSAGES		// print debug messages on every key step of the compilation.
#define CLEAR_SCANNER_ON_TOKEN		// clear the scanner for every new token. useful for debugging meta-characters in isolation.
#define FALSE	0
#define TRUE	1

char* characters_all = "abcdefghijklmnopqrstuvABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&'()*+,-./1234567890:;<=>?@[\\]^_`{|}~ \t\n\r";
char* characters_num = "0123456789";
char* characters_white = " \t\n\r\f\v";
char* characters_alpha = "abcdefghijklmnopqrstuvABCDEFGHIJKLMNOPQRSTUVWXYZ";
char* characters_alphanum = "abcdefghijklmnopqrstuvABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

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

void success(char* text) {
	#ifdef STEPPING_DEBUG_MESSAGES
	printf("\e[1;32m%s\e[0m", text);
	#endif
}

void highlight_bold(char* text) {
	#ifdef STEPPING_DEBUG_MESSAGES
	printf("\e[1;33m%s\e[0m", text);
	#endif
}

void highlight(char* text) {
	#ifdef STEPPING_DEBUG_MESSAGES
	printf("\e[0;33m%s\e[0m", text);
	#endif
}


struct state {
	char **inc;					// array of strings. check all the strings. pre-defined strings like \d or \w can easily be included just by adding a pointer.
	char inci;					// pointer to the top of the inc stack.
	char **exc;					// array of strings, containing characters to exclude.
	char exci;					// pointer to the top of the exc stack.
	int id;						// if this is an exit state, here lay the id of the matched token type.
	struct state_list *next;	// linked list of all possible states the may succeed this one.
};

void print_inc (struct state* state) {
	for (int i = 0; (state -> inc)[i] != NULL; i ++) {
		highlight ((state -> inc)[i]);
		if ((state -> inc)[i+1] != NULL)
			printf(", ");
	}
}

void print_exc (struct state* state) {
	for (int i = 0; (state -> exc)[i] != NULL; i ++) {
		highlight ((state -> exc)[i]);
		if ((state -> exc)[i+1] != NULL)
			printf(", ");
	}
}


struct token_type** initialize_token_types () {
	msg("Starting token initializer...\n");

	struct token_type** token_types = malloc(sizeof(struct token_type*) * 22);

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
	struct token_type* equ = malloc(sizeof(struct token_type));
	struct token_type* col = malloc(sizeof(struct token_type));

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
	equ -> id = 19;
	col -> id = 20;

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
	equ -> re = malloc(16);
	col -> re = malloc(16);

	"	. ^ $ * + ? { } [ ] \\ | ( )";

	// greedy matching - try to match the longest possible word, then backtrack if fails.
	// abcdef, then abcd, then abmnk, then akk
	
	strcpy(wht -> re, "[ \t\n]");
	strcpy(ele -> re, "e");
	strcpy(all -> re, "v");
	strcpy(any -> re, "#");
	strcpy(cup -> re, "u");
	strcpy(cap -> re, "n");
	strcpy(mul -> re, "\\*");
	strcpy(div -> re, "/");
	strcpy(add -> re, "\\+");
	strcpy(sub -> re, "\\t-");
	strcpy(pow -> re, "\\^");
	strcpy(rem -> re, "~");
	strcpy(ite -> re, "_");
	strcpy(num -> re, "[0-9]+");
	strcpy(dec -> re, "[0-9]+\\.[0-9]+");
	strcpy(str -> re, "\"[^\"]+\"");
	strcpy(spa -> re, "[A-Z]+");
	strcpy(set -> re, "\\|[A-Z]+");
	strcpy(var -> re, "[a-z][a-z0-9]*");
	strcpy(equ -> re, "=");
	strcpy(col -> re, ":");
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
	token_types[19] = equ;
	token_types[20] = col;
	token_types[21] = NULL;

	success("Tokens initialized successfully!\n");
//	err("Tokens initialization failed!\n");
	return token_types;
}

char string_contains(char* string, char target) {
	for (int i = 0; string[i] != '\0'; i ++)
		if (string[i] == target)
			return 1;
	return 0;
}

void print_scanner (struct state* scanner, int depth, char* lines, char last) {
	char* prefix = malloc(64);
	prefix[0] = '\0';

	for (int tab = 0; tab < depth; tab ++) {
		if (lines[tab] == 1) {
			if (tab >= depth - 1)
				strcat(prefix, " ├──");
			else
				strcat(prefix, " │  ");
		}
		else if (tab >= depth - 1)
			strcat(prefix, " └──");
		else
			strcat(prefix, "    ");
	}
	printf("%s %d  ", prefix, scanner -> id);
	print_inc (scanner);
	if (scanner -> exci) {
		if (scanner -> inci)
			printf(", ^");
		else
			printf("^");
	}
	print_exc (scanner);
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
	scanner -> inci = 0;
	scanner -> exc = malloc(sizeof(char*) * 4);	// one for nums, one for digits, one for custom, one terminating
	scanner -> exc[0] = NULL;
	scanner -> exci = 0;
	scanner -> id = 0;
	scanner -> next = NULL;
	int idi = 1;	// next id to use

	char* re;
	
	// iterate token_types:
	for (int i = 0; token_types[i] != NULL; i ++) {
		re = token_types[i] -> re;
		#ifdef STEPPING_DEBUG_MESSAGES
		printf("\nParsing re: ");
		highlight_bold(re);
		putchar('\n');
		#endif

		struct state* parent = scanner;
		struct state* charset_parent;	// store the state before a charset here.
		#ifdef CLEAR_SCANNER_ON_TOKEN
		scanner -> next = NULL;
		#endif
		char COMPLEMENTARY = 0;	// ✔
		char TAIL_MATCH = 0;
		char REPEAT_ZERO_PLUS = 0;
		char REPEAT_ONE_PLUS = 0;
		char CHAR_RANGE = 0;	//
		char REPEAT_ZERO_OR_ONE = 0;
		char REPEAT_RANGE = 0;
		char ESCAPING = 0;		// ✔
		char CHAR_SET = 0;		// ✔
		char CHAR_SEQUENCE = 0;
		char PIPE = 0;

		struct state* child = NULL; // always assign new pointers to NULL so you can easily tell if they've been initialized or not.
		// iterate characters in token_type:
		for (int j = 0; re[j] != '\0'; j ++){
			#ifdef STEPPING_DEBUG_MESSAGES
			//printf("Char: %c\n", re[j]);
			#endif

			// INITIALIZE NEW STATE OBJECT
			if (CHAR_SET < 2)
				child = NULL;
			struct state_list* branch; // this is for checking if the branch already exists
			char exists;

			// RESET FLAGS:
			if (COMPLEMENTARY == 2) COMPLEMENTARY = 0;
			if (TAIL_MATCH == 2) TAIL_MATCH = 0;
			if (REPEAT_ZERO_PLUS == 2) REPEAT_ZERO_PLUS = 0;
			if (REPEAT_ONE_PLUS == 2) REPEAT_ONE_PLUS = 0;
			if (REPEAT_ZERO_OR_ONE == 2) REPEAT_ZERO_OR_ONE = 0;
			if (ESCAPING == 2) ESCAPING = 0;
			ESCAPING += ESCAPING;
			CHAR_SET += CHAR_SET;
			CHAR_RANGE += CHAR_RANGE;
			COMPLEMENTARY += COMPLEMENTARY;

			// HANDLE CHAR
			switch(re[j]) {

				// METACHAR:
				case '^':
					if (ESCAPING) goto _default;
					COMPLEMENTARY ++;
					CHAR_SET /= 2;
					break;
				case '$':
					if (ESCAPING) goto _default;
					TAIL_MATCH ++;
					CHAR_SET /= 2;
					break;
				case '*':
					if (ESCAPING) goto _default;
					REPEAT_ZERO_PLUS ++;
					break;
				case '+':
					if (ESCAPING) goto _default;
					REPEAT_ONE_PLUS ++;
					break;
				case '-':
					if (ESCAPING) goto _default;
					CHAR_RANGE ++;
					break;
				case '?':
					if (ESCAPING) goto _default;
					REPEAT_ZERO_OR_ONE ++;
					break;
				case '{':
					if (ESCAPING) goto _default;
					REPEAT_RANGE = 1;
					break;
				case '}':
					if (ESCAPING) goto _default;
					REPEAT_RANGE = 0;
					break;
				case '[':
					if (ESCAPING) goto _default;
					CHAR_SET ++;
					break;
				case ']':
					if (ESCAPING) goto _default;
					CHAR_SET = 0;
					// CHECK IF STATE ALREADY EXISTS FOR COMPLETED CHAR SET:
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
					break;
				case '\\':
					if (ESCAPING) goto _default;
					ESCAPING ++;
					CHAR_SET /= 2;
					break;
				case '|':
					if (ESCAPING) goto _default;
					PIPE ++;
					break;
				case '(':
					if (ESCAPING) goto _default;
					CHAR_SEQUENCE = 1;
					CHAR_SET /= 2;
					break;
				case ')':
					if (ESCAPING) goto _default;
					CHAR_SEQUENCE = 0;
					CHAR_SET /= 2;
					break;

				// TRUE-CHAR:
				_default:
				default:
					
					// CHECK IF STATE ALREADY EXISTS (NOT FOR CHAR SETS):
					exists = FALSE;
					if (CHAR_SET == 0) {
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
					}
					if (exists) {
						//printf("State handling char %c already exists after state %d.\n", re[j], idi - 1);
					}

					// CREATE THE STATE:
					else {
						//printf("Creating state %d to handle char %c.\n", idi, re[j]);
						if (CHAR_SET > 2) {
							//goto _dictator;
							//child = parent;	///DICKCKKCKCKCKCKCKCKCKCK
						}
						else {
							_dictator:
							child = malloc(sizeof(struct state));
							child -> inc = malloc(sizeof(char*) * 4);	// one for nums, one for digits, one for custom, one terminating
							child -> inc[0] = NULL;
							child -> inci = 0;
							child -> exc = malloc(sizeof(char*) * 4);
							child -> exc[0] = NULL;
							child -> exci = 0;
							child -> id = idi ++;
							child -> next = NULL;
						}

						// ATTACH THE STATE TO PARENT
						if (CHAR_SET < 4) {
							struct state_list* parent_infertilized_egg = parent -> next; // parent item in state_list of parent state
							if (parent_infertilized_egg == NULL) {
								struct state_list* womb = malloc(sizeof(struct state_list));
								womb -> current = child;
								womb -> next = NULL;
								parent -> next = womb;
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

						// SET CHARACTER ARRAY TARGET
						char*** inc_or_exc;
						char* inci_or_exci;
						if (COMPLEMENTARY) {		// if ^ prefix is set, add chars to exclusion array.
							inc_or_exc = &(child -> exc);
							inci_or_exci = &(child -> exci);
						}
						else {						// otherwise, add chars to inclusion array.
							inc_or_exc = &(child -> inc);
							inci_or_exci = &(child -> inci);
						}
				
						// ADD THE CHARACTERS TO THE NEW STATE
						if (ESCAPING) {

							if (
								// C ESCAPE CHARS:
								re[j] == 'n' ||
								re[j] == 't' ||
								re[j] == 'v' ||
								re[j] == '\\' ||
								re[j] == '\'' ||
								re[j] == '\"' ||

								// ESCAPED REGEX METACHARS:
								re[j] == '.' ||
								re[j] == '^' ||
								re[j] == '$' ||
								re[j] == '*' ||
								re[j] == '+' ||
								re[j] == '-' ||
								re[j] == '?' ||
								re[j] == '[' ||
								re[j] == ']' ||
								re[j] == '|' ||
								re[j] == '{' ||
								re[j] == '}' ||
								re[j] == '(' ||
								re[j] == ')'
								) {
								if (COMPLEMENTARY) {
									child -> exc[child -> exci] = malloc(2);
									child -> exc[child -> exci ++][0] = re[j];
								}
								else {
									child -> inc[child -> inci] = malloc(2);
									child -> inc[child -> inci ++][0] = re[j];
								}
							}

							switch (re[j]) {
								// REGEX ESCAPE CHARS:
								case 'd':
									child -> inc[child -> inci ++] = characters_num;
								case 'D':
									child -> exc[child -> exci ++] = characters_num;
								case 's':
									child -> inc[child -> inci ++] = characters_white;
								case 'S':
									child -> exc[child -> exci ++] = characters_white;
								case 'w':
									child -> inc[child -> inci ++] = characters_alphanum;
								case 'W':
									child -> exc[child -> exci ++] = characters_alphanum;
								case 'a':
									child -> inc[child -> inci ++] = characters_alpha;
								case 'A':
									child -> exc[child -> exci ++] = characters_alpha;

							}
							if (COMPLEMENTARY)
								child -> exci ++;
							else
								child -> inci ++;
						}
						else {
							// ADD CHARS TO INC:
							if (CHAR_RANGE > 1) {
								char range = re[j] - re[j-2];
								int i = 0;
								if (COMPLEMENTARY) {
									child -> exc[child -> exci] = malloc(range + 2);
									for (; i <= range; i ++)
										child -> exc[child -> exci][i] = re[j-2] + i;
									child -> exc[child -> exci ++][i] = '\0';
								}
								else {
									child -> inc[child -> inci] = malloc(range + 2);
									for (; i <= range; i ++)
										child -> inc[child -> inci][i] = re[j-2] + i;
									child -> inc[child -> inci ++][i] = '\0';
								}
								CHAR_RANGE = 0;
							}
							else {
								if (COMPLEMENTARY) {
									child -> exc[child -> exci] = malloc(2);
									child -> exc[child -> exci ++][0] = re[j];
								}
								else {
									child -> inc[child -> inci] = malloc(2);
									child -> inc[child -> inci ++][0] = re[j];
								}
							}
						}
						if (COMPLEMENTARY)
							(child -> exc)[child -> exci]= NULL;
						else
							(child -> inc)[child -> inci]= NULL;
					}
					break;
			}
			if (child != NULL) {
				if (CHAR_SET > 0) {
					//parent = child;	// DICKCKKKCKCKCKCKkc
				}
				else {
					parent = child;
				}
			}
		}
		msg("Scanner:\n");
		char* lines = malloc(16);
		print_scanner (scanner, 0, lines, 1);
	}

	success("Scanner generated successfully!\n");
	return scanner;
}

int main () {
	struct token_type** token_types = initialize_token_types();
	struct state* scanner = scanner_generator(token_types);

	msg("Final scanner:\n");
	char* lines = malloc(16);
	print_scanner (scanner, 0, lines, 1);
}
