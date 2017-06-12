void set_cache_map()        //최대 5개까지 캐시 맵에 업데이트 한다.(최신순으로)
{
	int i = 0;
	int j = 0;

	cache_size++;

	if( cache_size < 5 )   //캐시 크기가 5보다 작으면 캐시에 저장한다.
	{
		for( i = 0; i < MAXLINE; i++ )
		{
			strcpy( cache_map[cache_size][i], cur_map[i] );
		}
	} 
	else   //캐시 크기가 5라면 더 이상 저장할 곳이 없으므로 캐시를 비워준다.
	{
		cache_size = 4;
		for( i = 0; i < cache_size; i++ )
		{
			for( j = 0; j < MAXLINE; j++ )
			{
				strcpy( cache_map[i][j], cache_map[i+1][j] );
			}
		}

		for( i = 0; i < MAXLINE; i++ )    //캐시를 비운뒤 맵정보를 저장한다. 
		{
			strcpy( cache_map[cache_size][i], cur_map[i] );
		}
	}
}
