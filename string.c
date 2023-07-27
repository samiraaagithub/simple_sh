#include "shell.h"

/**
 * _strlen - it returns the string length
 *
 * @s: is the string whose length to check
 *
 * Return: integer length of the string
 */
int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);

	while (*s++)
		i++;
	return (i);
}

/**
 * _strcmp - performs lexicogarphic comparison of two strings.
 *
 * @s1: is the first string
 *
 * @s2: is the second string
 *
 * Return: negative for s1 < s2, positive for s1 > s2, zero for s1 == s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * starts_with - will check if needle starts with haystack
 *
 * @haystack: is string to search
 *
 * @needle: is the substring to find
 *
 * Return: the address of next char of haystack or NULL
 */
char *starts_with(const char *haystack, const char *needle)
{
	while (*needle)
		if (*needle++ != *haystack++)
			return (NULL);
	return ((char *)haystack);
}

/**
 * _strcat - it concatenates two strings
 *
 * @dest: is the destination buffer
 *
 * @src: is the source buffer
 *
 * Return: is the pointer to destination buffer
 */
char *_strcat(char *dest, char *src)
{
	char *ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (ret);
}
