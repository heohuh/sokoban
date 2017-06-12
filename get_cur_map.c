void get_cur_map( int level )  //현재 맵의 정보를 얻어온다
{
	int i = 0;
	int j = 0;

	for( i = 0; i < MAXLINE; i++ )    //맵을 초기화한다. 
	{
		for( j = 0; j < MAXWIDTH; j++ ) 
		{
			cur_map[i][j] = 0;
		}
	}

	for( i = 0; i < MAXLINE; i++ )   //레벨에 맞는 맵을 가져온다. 
	{
		strcpy( cur_map[i], map[level][i] );
	}
}
