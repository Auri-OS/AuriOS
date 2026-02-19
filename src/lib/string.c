#include "../include/string.h"

// Count char in string
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

// Compare two string between them
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// Compare the first n byte of two string between them
int	strncmp(char *s1, char *s2, unsigned int n)
{
	unsigned int i = 0;
	while (s1[i] == s2[i] && s2[i] != '\0' && i < n - 1)
	{
		i++;
	}
	if (n == 0)
		return (0);
	return (s1[i] - s2[i]);
}

// Catenate string from src to dest
int	strcat(char *dest, char *src)
{
	unsigned int a = 0;
	unsigned int b = 0;

	while (dest[a] != '\0')
		a++;
	while (src[b] != '\0')
	{
		dest[a + b] = src[b];
		b++;
	}
	return (*dest);
}

// Catenate the first n bytes from a src string to dest 
char *strncat(char *dest, char *src, unsigned int nb)
{
	unsigned int i = 0;
	unsigned int j = 0;

	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0' && j < nb)
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

// Check if a string is in uppercase
int str_is_uppercase(char *str)
{
	int	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < 'A' || (str[i] > 'a' && str[i] < 'z') || str[i] > 'Z')
			return (0);
		i++;
	}
	return (1);
}

// Check if a string is in lowercase
int str_is_lowercase(char *str)
{
	int	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] < 'a' || (str[i] > 'A' && str[i] < 'Z') || str[i] > 'z')
			return (0);
		i++;
	}
	return (1);
}

// Set string to Upercase
char *strupcase(char *str)
{
	int	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = (str[i] + ('A' - 'a'));
		i++;
	}
	return(str);
}

//Set string to lowercase
char *strlowcase(char *str)
{
	int	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = (str[i] + ('a' - 'A'));
		i++;
	}
	return(str);
}

// Copy a string from a source to a destination
char strcpy(char *src, char *dest) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    return(*dest);
}