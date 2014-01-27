
void bb_perror_msg_and_die(const char *s, ...)
{
}

void bb_perror_msg(const char *s, ...)
{
}


void bb_error_msg_and_die(const char *s, ...)
{
}

void bb_simple_perror_msg_and_die(const char *s)
{
	bb_error_msg_and_die("%s", s);
}

void  bb_error_msg(const char *s, ...)
{

}


void xfunc_die(void)
{

}
