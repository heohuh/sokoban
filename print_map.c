void print_map( char ch )  //맵 정보를 화면에 출력해준다
{
	int i = 0;

	printf("  Hello %s \n\n\n", name );

	if( ch != 'd' )
	{
		for( i = 0; i < MAXLINE; i++ )
		{
			printf("%s", cur_map[i] );
		}
	}
	else
	{
		print_help();
	}

	if( ch != 'h' && ch != 'j' && ch != 'k' && ch != 'l' ) 
	{
		printf("(Command) %c \n", ch);
	}
	else
	{
		printf("(Command) \n");
	}
}