#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_BOLD_RED  "\x1b[1;31m"
#define COLOR_YELLOW    "\x1b[1;33m"
#define COLOR_RESET     "\x1b[0m"

int showLineNumbers = 0;
int numberNonBlank = 0;
int squeezeBlank = 0;
int showEnds = 0;
int showTabs = 0;

int isBlankLine(const char *line) {
	for (const char *p = line; *p; ++p) {
		if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') {
			return 0;
		}
	}
	return 1;
}

void printLine(const char *line, int *lineNumber, int *lastBlank) {
	int blank = isBlankLine(line);

	if (squeezeBlank && blank && *lastBlank) {
		return;
	}

	if (showLineNumbers && !(numberNonBlank && blank)) {
		printf("%6d\t", (*lineNumber)++);
	}

	*lastBlank = blank;

	for (const char *p = line; *p; ++p) {
		if (*p == '\t' && showTabs) {
			fputs("^I", stdout);
		} else if (*p == '\n' && showEnds) {
			putchar('$');
			putchar('\n');
		} else {
			putchar(*p);
		}
	}
}

void printFile(const char *filename) {
	FILE *file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, COLOR_BOLD_RED "cat: cannot open '%s': %s" COLOR_RESET "\n", filename, strerror(errno));
		return;
	}

	char buffer[4096];
	int lineNumber = 1;
	int lastBlank = 0;

	while (fgets(buffer, sizeof(buffer), file)) {
		size_t len = strlen(buffer);
		int lineComplete = (len > 0 && buffer[len - 1] == '\n');

		if (lineComplete) {
			printLine(buffer, &lineNumber, &lastBlank);
		} else {
			char *lineBuffer = malloc(len + 1);
			if (!lineBuffer) {
				fprintf(stderr, COLOR_BOLD_RED "cat: memory allocation error\n" COLOR_RESET);
				fclose(file);
				return;
			}
			memcpy(lineBuffer, buffer, len);
			size_t currentLen = len;

			while (!lineComplete) {
				if (!fgets(buffer, sizeof(buffer), file)) {
					break;
				}
				len = strlen(buffer);
				lineComplete = (len > 0 && buffer[len - 1] == '\n');

				char *newBuffer = realloc(lineBuffer, currentLen + len + 1);
				if (!newBuffer) {
					free(lineBuffer);
					fprintf(stderr, COLOR_BOLD_RED "cat: memory allocation error\n" COLOR_RESET);
					fclose(file);
					return;
				}
				lineBuffer = newBuffer;

				memcpy(lineBuffer + currentLen, buffer, len);
				currentLen += len;
				lineBuffer[currentLen] = '\0';
			}

			printLine(lineBuffer, &lineNumber, &lastBlank);
			free(lineBuffer);
		}
	}

	fclose(file);
}

void printUsage() {
	fprintf(stderr, COLOR_BOLD_RED
		"Usage: cat [options] file...\n"
		"Try 'cat -h' for more information.\n"
		COLOR_RESET);
}

void printHelp() {
	printf(COLOR_YELLOW
		"Usage: cat [options] file...\n"
		"Options:\n"
		"  -n    Number all lines\n"
		"  -b    Number non-empty lines\n"
		"  -s    Squeeze blank lines\n"
		"  -E    Show $ at end of line\n"
		"  -T    Show tabs as ^I\n"
		"  -h, --help    Show this help message\n"
		COLOR_RESET);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, COLOR_BOLD_RED "cat: missing file operand\n" COLOR_RESET);
		printUsage();
		return 1;
	}

	int firstFileIndex = argc;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printHelp();
			return 0;
		}

		if (argv[i][0] == '-') {
			const char *opt = argv[i] + 1;
			if (*opt == '\0') {
				fprintf(stderr, COLOR_BOLD_RED "cat: '-' is not supported as input\n" COLOR_RESET);
				return 1;
			}
			while (*opt) {
				switch (*opt) {
					case 'n': showLineNumbers = 1; break;
					case 'b': numberNonBlank = 1; showLineNumbers = 1; break;
					case 's': squeezeBlank = 1; break;
					case 'E': showEnds = 1; break;
					case 'T': showTabs = 1; break;
					default:
						fprintf(stderr, COLOR_BOLD_RED "cat: invalid option -- '%c'\n" COLOR_RESET, *opt);
						printUsage();
						return 1;
				}
				opt++;
			}
		} else {
			firstFileIndex = i;
			break;
		}
	}

	if (firstFileIndex == argc) {
		fprintf(stderr, COLOR_BOLD_RED "cat: missing file operand\n" COLOR_RESET);
		printUsage();
		return 1;
	}

	for (int i = firstFileIndex; i < argc; i++) {
		printFile(argv[i]);
	}

	return 0;
}
