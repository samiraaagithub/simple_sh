#include "shell.h"

/**
 * input_buf - buffers chained commands
 *
 * @info: is parameter struct
 *
 * @buf: it address of buffer
 *
 * @len: it address of len var
 *
 * Return: it bytes read
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p);

#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')

			{	
				(*buf)[r - 1] = '\0'; /* it remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(info, *buf, info->histcount++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - it gets a line minus the newline
 *
 * @info: is parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* is the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* there is a command left in the chain buffer */
	{
		j = i; /* initialize new iterator to current buf position */
		p = buf + i; /*it get pointer for return */

		check_chain(info, buf, &j, i, len);
		while (j < len) /*it iterate to semicolon or end */
		{
			if (is_chain(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /*it increment past nulled ';'' */
		if (i >= len) /* it reached end of buffer? */
		{
			i = len = 0; /* it reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* it pass back pointer to current command position */
		return (_strlen(p)); /* it return length of current command */
	}

	*buf_p = buf; /*the else not a chain, pass back buffer from _getline() */
	return (r); /*this return length of buffer from _getline() */
}

/**
 * read_buf - it reads a buffer
 *
 * @info: is parameter struct
 *
 * @buf: is buffer
 *
 * @i: is size
 *
 * Return: r
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - it gets next line of input from STDIN
 *
 * @info: isparameter struct
 *
 * @ptr: is address of pointer to buffer, preallocated or NULL
 *
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - it blocks ctrl-C
 *
 * @sig_num: is the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}
