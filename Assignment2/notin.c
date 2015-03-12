int notin(char* a, char* b[])
{
	int i = 0;
	while (i<(sizeof(b)/4)-1)
	{
		if (a == b[i])
		{
			return 1;
		}
		else
		{
			i++;
		}
	}
	if (i == (sizeof(b)/4)-1)
	{
		return 0;
	}
}
